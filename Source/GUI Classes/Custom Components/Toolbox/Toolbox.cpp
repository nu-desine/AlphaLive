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

enum
{
    DRUM_BANKS = 0,
    SEQUENCES,
    MIDI_PRESETS,
    SAMPLER_PRESETS,
    SEQUENCER_PRESETS,
    CONTROLLER_PRESETS,
    EFFECT_PRESETS,
    SCENE_PRESETS,
    AUDIO_SAMPLES
    
};

Toolbox::Toolbox(MainComponent &parent) : 
                    parentRef(parent),
                    audioSamplesTree (0),
                    thread ("tree and list thread")

{
    currentList = 0;
    
    for (int i = 0; i < 9; i++)
        contentLists.insert(i, new DirectoryContentsList(0, thread));
    
    contentLists[DRUM_BANKS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Drum Banks"), false, true);
    contentLists[SEQUENCES]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Sequences"), false, true);
    contentLists[MIDI_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/MIDI Mode"), false, true);
    contentLists[SAMPLER_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Sampler Mode"), false, true);
    contentLists[SEQUENCER_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Sequencer Mode"), false, true);
    contentLists[CONTROLLER_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Controller Mode"), false, true);
    contentLists[EFFECT_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Effects"), false, true);
    contentLists[SCENE_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Scene Presets"), false, true);
    
    contentLists[AUDIO_SAMPLES]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Audio Samples"), true, true);
    
    thread.startThread();
    
    //By default FileTreeComponent's and FileListComponent's display file extention, file size, and file modifcation date
    //To remove these attributes, need to hack FileListTreeItem::paintItem in juce_FileTreeComponent.cpp
    //and FileListItemComponent::paint() in juce_FileListComponent.cpp
    //could this be done using LookAndFeel instead?
    
    audioSamplesTree = new FileTreeComponent (*contentLists[AUDIO_SAMPLES]);
    audioSamplesTree->addListener(this);
    audioSamplesTree->setIndentSize(10);
    //to change the row height of a file tree component, need to hack FileTreeListItem::getItemHeight() in juce_FileTreeComponent.cpp
    //could this be done using LookAndFeel instead?
    //Update - I think i need to use the TreeViewItem to create my own items and then I can overide getItemHeight()
    //to set the size. However for now I'm lazy and I'm just going to hack the Juce Library code.
    
    for (int i = 0; i < 8; i++)
    {
        fileLists.insert(i, new FileListComponent (*contentLists[i]));
        fileLists[i]->addListener(this);
        fileLists[i]->setRowHeight(16);
    }
    
    scalesListBox = new XmlAttributesListBox (File("/Users/Liam/Desktop/AlphaSphere Software Dev/AlphaLive NEW vDEV/Other Files/scales.xml"), true, *this);
    layoutsListBox = new XmlAttributesListBox (File("/Users/Liam/Desktop/AlphaSphere Software Dev/AlphaLive NEW vDEV/Other Files/notational_arrangements.xml"), false, *this);

    //create tabbed component 
    addChildComponent(tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth(22);
}


Toolbox::~Toolbox()
{
    fileLists.clear();
    contentLists.clear();
    thread.stopThread(50);
    
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
    //should I only call deselect on last click component that can be found from currentList?
    for (int i = 0; i < 8; i++)
        fileLists[i]->deselectAllFiles();
    audioSamplesTree->deselectAllFiles();
    layoutsListBox->deselectAllRows();
    scalesListBox->deselectAllRows();
    
    tabbedComponent->clearTabs();
    tabbedComponent->setVisible(true);
    
    int padNum = selectedPads[0];
    
    if (PAD_SETTINGS->getMode() == 1)
    {
        if (selectedPads.size() == 48)
            tabbedComponent->addTab(translate("Layouts"), Colours::darkgrey, layoutsListBox, false);
            
        if (MULTI_PADS)
            tabbedComponent->addTab(translate("Scales"), Colours::darkgrey, scalesListBox, false);
        
        tabbedComponent->addTab(translate("Mode Presets"), Colours::darkgrey, fileLists[MIDI_PRESETS], false);
    }
    else if (PAD_SETTINGS->getMode() == 2)
    {
        if (MULTI_PADS)
        {
            tabbedComponent->addTab(translate("Banks"), Colours::darkgrey, fileLists[DRUM_BANKS], false);
        }
        
        tabbedComponent->addTab(translate("Samples"), Colours::darkgrey, audioSamplesTree, false);
        tabbedComponent->addTab(translate("Effects"), Colours::darkgrey, fileLists[EFFECT_PRESETS], false);
        tabbedComponent->addTab(translate("Mode Presets"), Colours::darkgrey, fileLists[SAMPLER_PRESETS], false);
        
    }
    else if (PAD_SETTINGS->getMode() == 3)
    {
        if (PAD_SETTINGS->getSequencerMode() == 1)
            tabbedComponent->addTab(translate("Scales"), Colours::darkgrey, scalesListBox, false);
        
        else if (PAD_SETTINGS->getSequencerMode() == 2)
        {
            tabbedComponent->addTab(translate("Banks"), Colours::darkgrey, fileLists[DRUM_BANKS], false);
            //the below will only be useable if we get drag-and-drop working
            //tabbedComponent->addTab(translate("Samples"), Colours::darkgrey, audioSamplesTree, false);
            tabbedComponent->addTab(translate("Effects"), Colours::darkgrey, fileLists[EFFECT_PRESETS], false);
        }
        
        tabbedComponent->addTab(translate("Sequences"), Colours::darkgrey, fileLists[SEQUENCES], false);
        tabbedComponent->addTab(translate("Mode Presets"), Colours::darkgrey, fileLists[SEQUENCER_PRESETS], false);
    
    }
    else if (PAD_SETTINGS->getMode() == 4)
    {
        tabbedComponent->addTab(translate("Mode Presets"), Colours::darkgrey, fileLists[CONTROLLER_PRESETS], false);
    }
    else
    {
        //tabbedComponent->addTab(translate("Scene Presets"), Colours::darkgrey, scenePresetsList, false);
        tabbedComponent->setVisible(false);
        
    }
    
}

          
//callback functions for the filetree and filelist components
void Toolbox::selectionChanged()
{
  
}

void Toolbox::fileClicked (const File& file, const MouseEvent& e)
{
    //The below is needed so that when fileDoubleClicked is called we know what filebrowser called it
    
    if (audioSamplesTree->isMouseOver(true))
        currentList = AUDIO_SAMPLES;
    
    for (int i = 0; i < 8; i++)
    {
        if (fileLists[i]->isMouseOver(true))
            currentList = i;
    }

}

void Toolbox::fileDoubleClicked (const File& file)
{
    if (currentList == AUDIO_SAMPLES)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerAudioFilePath(file);
        }
        
        parentRef.getGuiSamplerMode()->setAudioFileDisplay(file);
    }
}

void Toolbox::browserRootChanged (const File&)
{
  
}


//callback function for the custom list boxes
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
    
     
    else 
    {
        //selectedPads must be sorted for notational arrangements!
        DefaultElementComparator<int> sorter;
        selectedPads.sort (sorter);
        
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

