//
//  Toolbox.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 18/07/2012.
//  Copyright 2012 nu desine.
//
//  This file is part of AlphaLive.
//
//  AlphaLive is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2, 
//  as published by the Free Software Foundation.
//  
//  AlphaLive is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "Toolbox.h"
#include "../../../File and Settings/AppSettings.h"
#include "../../Views/MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)

Toolbox::Toolbox(MainComponent &parent) : 
                    parentRef(parent),
                    demoFileTreeComp (0),
                    thread ("file tree thread"),
                    demoTreeDirectoryList (0, thread),
                    demoListDirectoryList(0, thread)
{
    demoTreeDir = "/Users/Liam/Desktop/AlphaSphere Software Dev/Audio Samples/Library/Loops"; //what if it doesn't exist?
    //demoTreeDir = File::getSpecialLocation(File::userMusicDirectory);
    demoTreeDirectoryList.setDirectory (demoTreeDir, true, true);
    demoListDir = File::getSpecialLocation(File::userDocumentsDirectory);
    demoListDirectoryList.setDirectory (demoListDir, true, true);
    thread.startThread();
    
    //By default FileTreeComponent's and FileListComponent's display file extention, file size, and file modifcation date
    //To remove these attributes, need to hack FileListTreeItem::paintItem in juce_FileTreeComponent.cpp
    //and FileListItemComponent::paint() in juce_FileListComponent.cpp
    //could this be done using LookAndFeel instead?
    
    addChildComponent(demoFileTreeComp = new FileTreeComponent (demoTreeDirectoryList));
    demoFileTreeComp->addListener(this);
    demoFileTreeComp->setIndentSize(10);
    //to change the row height, need to hack FileTreeListItem::getItemHeight() in juce_FileTreeComponent.cpp
    //could this be done using LookAndFeel instead?
    //Update - I think i need to use the TreeViewItem to create my own items and then I can overide getItemHeight()
    //to set the size. However for now I'm lazy and I'm just going to hack the Juce Library code.
    
    addChildComponent(demoFileListComp = new FileListComponent (demoListDirectoryList));
    demoFileListComp->addListener(this);
    demoFileListComp->setRowHeight(16);
    
    scalesListBox = new XmlAttributesListBox (File("/Users/Liam/Desktop/AlphaSphere Software Dev/AlphaLive NEW vDEV/Other Files/scales.xml"), true, *this);
    layoutsListBox = new XmlAttributesListBox (File("/Users/Liam/Desktop/AlphaSphere Software Dev/AlphaLive NEW vDEV/Other Files/notational_arrangements.xml"), false, *this);

    //create tabbed component 
    addChildComponent(tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth(22);
}


Toolbox::~Toolbox()
{
    thread.stopThread(50);
    //deleteAllChildren();
}

//==============================================================================

void Toolbox::resized()
{
    tabbedComponent->setBounds(5, 5, getWidth()-10, getHeight()-10);
    
}

void Toolbox::paint(Graphics& g)
{
    g.setColour(Colours::grey.withAlpha(0.2f));
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 10);
}

void Toolbox::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    updateDisplay();
}

void Toolbox::updateDisplay()
{
    tabbedComponent->clearTabs();
    tabbedComponent->setVisible(true);
    
    int padNum = selectedPads[0];
    
    if (PAD_SETTINGS->getMode() == 1)
    {
        if (MULTI_PADS)
        {
            tabbedComponent->addTab(translate("Scales"), Colours::darkgrey, scalesListBox, false);
        }
        
        if (selectedPads.size() == 48)
        {
            tabbedComponent->addTab(translate("Layouts"), Colours::darkgrey, layoutsListBox, false);
        }
    }
    else if (PAD_SETTINGS->getMode() == 3)
    {
        if (MULTI_PADS)
        {
            tabbedComponent->addTab(translate("Scales"), Colours::darkgrey, scalesListBox, false);
        }
    }
    else
        tabbedComponent->setVisible(false);
    
    //tabbedComponent->addTab(translate("File Tree"), Colours::darkgrey, demoFileTreeComp, false);
    //tabbedComponent->addTab(translate("File List"), Colours::darkgrey, demoFileListComp, false);
}



void Toolbox::selectionChanged()
{
    std::cout << "selection changed...\n";
}

void Toolbox::fileClicked (const File& file, const MouseEvent& e)
{
    std::cout << "file clicked...\n";
    
}

void Toolbox::fileDoubleClicked (const File& file)
{
    std::cout << "file double clicked...\n";
}

void Toolbox::browserRootChanged (const File&)
{
    std::cout << "browser root changed...\n";
}

void Toolbox::noteLayoutSelected (String layout, bool isScale)
{
    //called when either a scale or notational arrangement is selected
    
    StringArray tokens;
    tokens.addTokens(layout, " ", String::empty);
    
    if (isScale) //the layout is a scale as opposed to a notational arrangement
    {
        int padNum = selectedPads[0]; //only need to check first pad as we should only be here if all pads equal to same mode
        if (PAD_SETTINGS->getMode() == 1) //midi mode
        {
            int rootNote = PAD_SETTINGS->getMidiNote(); //will this be appropriate?
            
            parentRef.getGuiPiano()->clearPiano();
            for (int i = 0; i < selectedPads.size(); i++)
            {
                padNum = selectedPads[i];
                int noteToApply = tokens[i].getIntValue() + rootNote;
                if (noteToApply > 119)
                    noteToApply = 119;
                
                PAD_SETTINGS->setMidiNote(noteToApply);
                parentRef.getGuiPiano()->setNoteData(noteToApply);
            }
        }
    }
    else //selectedPads must be sorted! Don;t need to check if midi mode like above as layouts only apply to midi mode
    {
        int padNum = selectedPads[0]; //only need to check first pad as we should only be here if all pads equal to same mode
        
        int rootNote = PAD_SETTINGS->getMidiNote(); //will this be appropriate?
        
        parentRef.getGuiPiano()->clearPiano();
        for (int i = 0; i < selectedPads.size(); i++)
        {
            padNum = selectedPads[i];
            int noteToApply = tokens[i].getIntValue() + rootNote;
            if (noteToApply > 119)
                noteToApply = 119;
            
            PAD_SETTINGS->setMidiNote(noteToApply);
            parentRef.getGuiPiano()->setNoteData(noteToApply);
        }
        
    }
    
}

