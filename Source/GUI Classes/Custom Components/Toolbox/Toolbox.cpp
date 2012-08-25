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
                    audioSamplesTree (0),
                    audioSamplesDirectory (0, thread),
                    drumBanksDirectory(0, thread),
                    sequencesDirectory (0, thread),
                    modeMidiPresetsDirectory (0, thread),
                    modeSamplerPresetsDirectory(0, thread),
                    modeSequencerPresetsDirectory (0, thread),
                    modeControllerPresetsDirectory(0, thread),
                    effectPresetsDirectory (0, thread),
                    scenePresetsDirectory (0, thread),
                    thread ("tree and list thread")

{
    audioSamplesDirectory.setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Audio Samples"), true, true);
    drumBanksDirectory.setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Drum Banks"), false, true);
    sequencesDirectory.setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Sequences"), false, true);
    modeMidiPresetsDirectory.setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/MIDI Mode"), false, true);
    modeSamplerPresetsDirectory.setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Sampler Mode"), false, true);
    modeSequencerPresetsDirectory.setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Sequencer Mode"), false, true);
    modeControllerPresetsDirectory.setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Controller Mode"), false, true);
    effectPresetsDirectory.setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Effects"), false, true);
    scenePresetsDirectory.setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Scene Presets"), false, true);
    
    thread.startThread();
    
    //By default FileTreeComponent's and FileListComponent's display file extention, file size, and file modifcation date
    //To remove these attributes, need to hack FileListTreeItem::paintItem in juce_FileTreeComponent.cpp
    //and FileListItemComponent::paint() in juce_FileListComponent.cpp
    //could this be done using LookAndFeel instead?
    
    audioSamplesTree = new FileTreeComponent (audioSamplesDirectory);
    audioSamplesTree->addListener(this);
    audioSamplesTree->setIndentSize(10);
    //to change the row height of a file tree component, need to hack FileTreeListItem::getItemHeight() in juce_FileTreeComponent.cpp
    //could this be done using LookAndFeel instead?
    //Update - I think i need to use the TreeViewItem to create my own items and then I can overide getItemHeight()
    //to set the size. However for now I'm lazy and I'm just going to hack the Juce Library code.
    
    drumBanksList = new FileListComponent (drumBanksDirectory);
    drumBanksList->addListener(this);
    drumBanksList->setRowHeight(16);
    sequencesList = new FileListComponent (sequencesDirectory);
    sequencesList->addListener(this);
    sequencesList->setRowHeight(16);
    modeMidiPresetsList = new FileListComponent (modeMidiPresetsDirectory);
    modeMidiPresetsList->addListener(this);
    modeMidiPresetsList->setRowHeight(16);
    modeSamplerPresetsList = new FileListComponent (modeSamplerPresetsDirectory);
    modeSamplerPresetsList->addListener(this);
    modeSamplerPresetsList->setRowHeight(16);
    modeSequencerPresetsList = new FileListComponent (modeSequencerPresetsDirectory);
    modeSequencerPresetsList->addListener(this);
    modeSequencerPresetsList->setRowHeight(16);
    modeControllerPresetsList = new FileListComponent (modeControllerPresetsDirectory);
    modeControllerPresetsList->addListener(this);
    modeControllerPresetsList->setRowHeight(16);
    effectPresetsList = new FileListComponent (effectPresetsDirectory);
    effectPresetsList->addListener(this);
    effectPresetsList->setRowHeight(16);
    scenePresetsList = new FileListComponent (scenePresetsDirectory);
    scenePresetsList->addListener(this);
    scenePresetsList->setRowHeight(16);
    
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
        if (selectedPads.size() == 48)
            tabbedComponent->addTab(translate("Layouts"), Colours::darkgrey, layoutsListBox, false);
            
        if (MULTI_PADS)
            tabbedComponent->addTab(translate("Scales"), Colours::darkgrey, scalesListBox, false);
        
        tabbedComponent->addTab(translate("Mode Presets"), Colours::darkgrey, modeMidiPresetsList, false);
    }
    else if (PAD_SETTINGS->getMode() == 2)
    {
        if (MULTI_PADS)
        {
            tabbedComponent->addTab(translate("Banks"), Colours::darkgrey, drumBanksList, false);
        }
        
        tabbedComponent->addTab(translate("Samples"), Colours::darkgrey, audioSamplesTree, false);
        tabbedComponent->addTab(translate("Effects"), Colours::darkgrey, effectPresetsList, false);
        tabbedComponent->addTab(translate("Mode Presets"), Colours::darkgrey, modeSamplerPresetsList, false);
        
    }
    else if (PAD_SETTINGS->getMode() == 3)
    {
        if (PAD_SETTINGS->getSequencerMode() == 1)
            tabbedComponent->addTab(translate("Scales"), Colours::darkgrey, scalesListBox, false);
        
        else if (PAD_SETTINGS->getSequencerMode() == 2)
        {
            tabbedComponent->addTab(translate("Banks"), Colours::darkgrey, drumBanksList, false);
            tabbedComponent->addTab(translate("Samples"), Colours::darkgrey, audioSamplesTree, false);
            tabbedComponent->addTab(translate("Effects"), Colours::darkgrey, effectPresetsList, false);
        }
        
        tabbedComponent->addTab(translate("Sequences"), Colours::darkgrey, sequencesList, false);
        tabbedComponent->addTab(translate("Mode Presets"), Colours::darkgrey, modeSequencerPresetsList, false);
    
    }
    else if (PAD_SETTINGS->getMode() == 4)
    {
        tabbedComponent->addTab(translate("Mode Presets"), Colours::darkgrey, modeControllerPresetsList, false);
    }
    else
    {
        tabbedComponent->addTab(translate("Scene Presets"), Colours::darkgrey, scenePresetsList, false);
        //tabbedComponent->setVisible(false);
        
    }
    
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

