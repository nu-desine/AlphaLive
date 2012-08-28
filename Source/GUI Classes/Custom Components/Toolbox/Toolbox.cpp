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
    
    xmlFilter = new WildcardFileFilter("*xml", String::empty, "XML files");
    seqFileFilter = new WildcardFileFilter("*alphaseq;*alphaseqset", String::empty, "AlphaLive sequencer files");
    audioFileFilter = new WildcardFileFilter("*wav;*aiff;*aif", "*", "Audio files");
        
    contentLists.insert(DRUM_BANKS, new DirectoryContentsList(xmlFilter, thread));
    contentLists[DRUM_BANKS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Audio Library/nu Banks"), false, true);
    
    contentLists.insert(SEQUENCES, new DirectoryContentsList(seqFileFilter, thread));
    contentLists[SEQUENCES]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Sequences"), false, true);
    
    contentLists.insert(MIDI_PRESETS, new DirectoryContentsList(xmlFilter, thread));
    contentLists[MIDI_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/MIDI Mode"), false, true);
    
    contentLists.insert(SAMPLER_PRESETS, new DirectoryContentsList(xmlFilter, thread));
    contentLists[SAMPLER_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Sampler Mode"), false, true);
    
    contentLists.insert(SEQUENCER_PRESETS, new DirectoryContentsList(xmlFilter, thread));
    contentLists[SEQUENCER_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Sequencer Mode"), false, true);
    
    contentLists.insert(CONTROLLER_PRESETS, new DirectoryContentsList(xmlFilter, thread));
    contentLists[CONTROLLER_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Controller Mode"), false, true);
    
    contentLists.insert(EFFECT_PRESETS, new DirectoryContentsList(xmlFilter, thread));
    contentLists[EFFECT_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Pad Presets/Effects"), false, true);
    
    contentLists.insert(SCENE_PRESETS, new DirectoryContentsList(xmlFilter, thread));
    contentLists[SCENE_PRESETS]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Scene Presets"), false, true);
    
    contentLists.insert(AUDIO_SAMPLES, new DirectoryContentsList(audioFileFilter, thread));
    contentLists[AUDIO_SAMPLES]->setDirectory (File("/Users/Liam/Desktop/AlphaSphere Software Dev/Application Directory Example/Library/Audio Library"), true, true);
    
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
     int padNum = selectedPads[0];
    
    //====================================================================================
    //====================================================================================
    
    if (currentList == AUDIO_SAMPLES)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerAudioFilePath(file);
        }
        
        parentRef.getGuiSamplerMode()->setAudioFileDisplay(file);
    }
    
    
    //====================================================================================
    //====================================================================================
    
    else if (currentList == DRUM_BANKS)
    {
        ScopedPointer<XmlElement> xmlData (XmlDocument::parse(file));
        
        int numOfSamples = xmlData->getChildElement(0)->getIntAttribute("numSamples");
        String fileDirPath (file.getParentDirectory().getFullPathName());
        StringArray sampleFilePaths;
        
        for (int i = 0; i < numOfSamples; i++)
        {
            sampleFilePaths.add(fileDirPath + xmlData->getChildElement(0)->getStringAttribute("sample" + String(i+1)));
        }
        

        if (PAD_SETTINGS->getMode() == 2) //Sampler Mode
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSamplerAudioFilePath(File(sampleFilePaths[i]));
            }
            
            parentRef.getGuiSamplerMode()->setAudioFileDisplay(File(sampleFilePaths[0]));
        }
        
        else if (PAD_SETTINGS->getMode() == 3) //Sequencer Mode
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                
                for (int row = 0; row < 12; row++)
                {
                    PAD_SETTINGS->setSequencerSamplesAudioFilePath(File(sampleFilePaths[row]), row);
                }
            }
            
        }
        
    }
    
    //====================================================================================
    //====================================================================================
    
    else if (currentList == SEQUENCES)
    {
        
        if (file.getFileExtension() == ".alphaseq")
        {
            int currentSeqNumber = parentRef.getGuiSequencerMode()->getCurrentSequenceNumber()-1;
            parentRef.getAppDocumentStateRef().loadSequence(currentSeqNumber, selectedPads, false, file);
        }
        
        else if (file.getFileExtension() == ".alphaseqset")
        {
            parentRef.getAppDocumentStateRef().loadSequenceSet(selectedPads, false, file);
        }
        
    }
    
}



void Toolbox::browserRootChanged (const File&)
{
  
}




//callback function for the custom list boxes
void Toolbox::noteLayoutSelected (String layout, bool isScale)
{
    //called when either a scale or notational arrangement is selected
    
    //get the individual MIDI notes from the string and put them into an array
    StringArray tokens;
    tokens.addTokens(layout, " ", String::empty);
    
    //====================================================================================
    //====================================================================================
    
    if (isScale) //the layout is a scale as opposed to a notational arrangement
    {
        int padNum = selectedPads[0]; //only need to check first pad as we should only be here if all pads equal to same mode
        if (PAD_SETTINGS->getMode() == 1) //midi mode
        {
            int rootNote = PAD_SETTINGS->getMidiNote(); //will this be appropriate (root note = selectedPads[0] note)?
            
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
        
        else if (PAD_SETTINGS->getMode() == 3) //sequencer mode
        {
            int rootNote = PAD_SETTINGS->getSequencerMidiNote(0); //will this be appropriate (root note = selectedPads[0] note)?
            
            parentRef.getGuiPiano()->clearPiano();
            
            for (int padIndex = 0; padIndex < selectedPads.size(); padIndex++)
            {
                for (int row = 0; row < 12; row++)
                {
                    padNum = selectedPads[padIndex];
                    
                    int noteToApply = tokens[row].getIntValue() + rootNote;
                    if (noteToApply > 119)
                        noteToApply = -500;
                    
                    PAD_SETTINGS->setSequencerMidiNote(noteToApply, row);
                    
                    if (padIndex == 0)
                    {
                        if (noteToApply >= 0)
                            parentRef.getGuiPiano()->setNoteData(noteToApply);
                    }
                }
            }
        }
    }
    
    //====================================================================================
    //====================================================================================
    
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

