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
    MIDI_PRESETS = 0,
    SAMPLER_PRESETS,
    SEQUENCER_PRESETS,
    CONTROLLER_PRESETS,
    EFFECT_PRESETS,
    
    SAMPLE_BANKS,
    AUDIO_SAMPLES,
    SEQUENCES,
    SCENE_PRESETS,
    
    NO_OF_LISTS //essentially this will equal the number of items above
};

Toolbox::Toolbox(MainComponent &parent) : 
                    mainComponentRef(parent),
                    thread ("tree and list thread")

{

    currentList = 0;
    
    bankFileFilter = new WildcardFileFilter("*alphabank", "Drum Kits;Instruments", "Audio bank files");
    padFileFilter = new WildcardFileFilter("*alphapad", String::empty, "Pad settings files");
    seqFileFilter = new WildcardFileFilter("*alphaseq;*alphaseqset", "*", "AlphaLive sequencer files");
    audioFileFilter = new WildcardFileFilter("*wav;*aiff;*aif", "*", "Audio files");
    sceneFileFilter = new WildcardFileFilter("*alphascene", "*", "Scene files");
     
    String appDir(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() + File::separatorString);
    
    File banksFile(appDir + "Library/Audio Library/AlphaBanks");
    if (banksFile.exists() == false)
        banksFile = File::nonexistent;
    File midiPresetsFile(appDir + "Library/Pad Presets/MIDI Mode");
    if (midiPresetsFile.exists() == false)
        midiPresetsFile = File::nonexistent;
    File samplerPresetsFile(appDir + "Library/Pad Presets/Sampler Mode");
    if (samplerPresetsFile.exists() == false)
        samplerPresetsFile = File::nonexistent;
    File sequencerPresetsFile(appDir + "Library/Pad Presets/Sequencer Mode");
    if (sequencerPresetsFile.exists() == false)
        sequencerPresetsFile = File::nonexistent;
    File controllerPresetsFile(appDir + "Library/Pad Presets/Controller Mode");
    if (controllerPresetsFile.exists() == false)
        controllerPresetsFile = File::nonexistent;
    File effectPresetsFile(appDir + "Library/Pad Presets/Effects");
    if (effectPresetsFile.exists() == false)
        effectPresetsFile = File::nonexistent;
    File scenePresetsFile(appDir + "Library/Scene Presets");
    if (scenePresetsFile.exists() == false)
        scenePresetsFile = File::nonexistent;
    File audioSamplesFile(appDir + "Library/Audio Library");
    if (audioSamplesFile.exists() == false)
        audioSamplesFile = File::nonexistent;
    File sequencesFile(appDir + "Library/Sequences");
    if (sequencesFile.exists() == false)
        sequencesFile = File::nonexistent;
    
    contentLists.insert(MIDI_PRESETS, new DirectoryContentsList(padFileFilter, thread));
    contentLists[MIDI_PRESETS]->setDirectory (midiPresetsFile, false, true);
    
    contentLists.insert(SAMPLER_PRESETS, new DirectoryContentsList(padFileFilter, thread));
    contentLists[SAMPLER_PRESETS]->setDirectory (samplerPresetsFile, false, true);
    
    contentLists.insert(SEQUENCER_PRESETS, new DirectoryContentsList(padFileFilter, thread));
    contentLists[SEQUENCER_PRESETS]->setDirectory (sequencerPresetsFile, false, true);
    
    contentLists.insert(CONTROLLER_PRESETS, new DirectoryContentsList(padFileFilter, thread));
    contentLists[CONTROLLER_PRESETS]->setDirectory (controllerPresetsFile, false, true);
    
    contentLists.insert(EFFECT_PRESETS, new DirectoryContentsList(padFileFilter, thread));
    contentLists[EFFECT_PRESETS]->setDirectory (effectPresetsFile, false, true);
    
    
    contentLists.insert(SAMPLE_BANKS, new DirectoryContentsList(bankFileFilter, thread));
    contentLists[SAMPLE_BANKS]->setDirectory (banksFile, true, true);
    
    contentLists.insert(AUDIO_SAMPLES, new DirectoryContentsList(audioFileFilter, thread));
    contentLists[AUDIO_SAMPLES]->setDirectory (audioSamplesFile, true, true);
    
    contentLists.insert(SEQUENCES, new DirectoryContentsList(seqFileFilter, thread));
    contentLists[SEQUENCES]->setDirectory (sequencesFile, true, true);
    
    contentLists.insert(SCENE_PRESETS, new DirectoryContentsList(sceneFileFilter, thread));
    contentLists[SCENE_PRESETS]->setDirectory (scenePresetsFile, true, true);
    
    thread.startThread();
    
    treeLists.insert(0, new FileTreeComponent (*contentLists[SAMPLE_BANKS]));
    treeLists.insert(1, new FileTreeComponent (*contentLists[AUDIO_SAMPLES]));
    treeLists.insert(2, new FileTreeComponent (*contentLists[SEQUENCES]));
    treeLists.insert(3, new FileTreeComponent (*contentLists[SCENE_PRESETS]));
    
    noOfTreeLists = treeLists.size();
    noOfFileLists = NO_OF_LISTS - noOfTreeLists;
      
    for (int i = 0; i < noOfTreeLists; i++)
    {
        treeLists[i]->addListener(this);
        treeLists[i]->setIndentSize(10);
        treeLists[i]->addMouseListener(this, true);
        treeLists[i]->setItemHeight(16 + (AlphaTheme::getInstance()->fontSizeAddition));
    }
    
    for (int i = 0; i < noOfFileLists; i++)
    {
        fileLists.insert(i, new FileListComponent (*contentLists[i]));
        fileLists[i]->addListener(this);
        fileLists[i]->setRowHeight(16 + (AlphaTheme::getInstance()->fontSizeAddition));
        fileLists[i]->addMouseListener(this, true);
    }
    
    File scalesFile(appDir + "Application Data/library_scales.xml");
    if (scalesFile.exists() == false)
        scalesFile = File::nonexistent;
    
    File layoutsFile(appDir + "Application Data/library_notational_arrangements.xml");
    if (layoutsFile.exists() == false)
        layoutsFile = File::nonexistent;
    
    File userScalesFile(appDir + "Application Data/user_scales.xml");
    if (userScalesFile.exists() == false)
        userScalesFile = File::nonexistent;
    
    File userLayoutsFile(appDir + "Application Data/user_notational_arrangements.xml");
    if (userLayoutsFile.exists() == false)
        userLayoutsFile = File::nonexistent;
    
    scalesListBox = new XmlAttributesListBox (scalesFile, userScalesFile, true, *this);
    scalesListBox->addMouseListener(this, true);
    layoutsListBox = new XmlAttributesListBox (layoutsFile, userLayoutsFile, false, *this);
    layoutsListBox->addMouseListener(this, true);

    //create tabbed component 
    addChildComponent(tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth(22 + (AlphaTheme::getInstance()->fontSizeAddition * 2));
    tabbedComponent->addMouseListener(this, true);
}


Toolbox::~Toolbox()
{
    treeLists.clear();
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
    g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.2f));
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
//    for (int i = 0; i < noOfFileLists; i++)
//        fileLists[i]->deselectAllFiles();
//    for (int i = 0; i < noOfTreeLists; i++)
//        treeLists[i]->deselectAllFiles();
//    layoutsListBox->deselectAllRows();
//    scalesListBox->deselectAllRows();
    
    int currentTabIndex = tabbedComponent->getCurrentTabIndex();
    
    tabbedComponent->clearTabs();
    tabbedComponent->setVisible(true);
    
    int mode = 0;
    int seqMode = 0;
    
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        mode = PAD_SETTINGS->getMode();
        seqMode = PAD_SETTINGS->getSequencerMode();
    }
    
    else if(MULTI_PADS)
    {
        //==================================================================================================
        int mode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMode() != mode_)
            {
                mode = 0;
                break;
            }
            if (i == selectedPads.size()-1)
                mode = mode_;
        }
        
        //==================================================================================================
        int seqMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMode() != seqMode_)
            {
                seqMode = 0;
                break;
            }
            if (i == selectedPads.size()-1)
                seqMode = seqMode_;
        }
    }
    
    
    if (mode == 1)
    {
        if (selectedPads.size() == 48)
            tabbedComponent->addTab(translate("Layouts"), AlphaTheme::getInstance()->foregroundColourDarker, layoutsListBox, false);
            
        if (MULTI_PADS)
            tabbedComponent->addTab(translate("Scales"), AlphaTheme::getInstance()->foregroundColourDarker, scalesListBox, false);
        
        tabbedComponent->addTab(translate("Mode Presets"), AlphaTheme::getInstance()->foregroundColourDarker, fileLists[MIDI_PRESETS], false);
    }
    else if (mode == 2)
    {
        if (MULTI_PADS)
        {
            tabbedComponent->addTab(translate("Banks"), AlphaTheme::getInstance()->foregroundColourDarker, treeLists[SAMPLE_BANKS-noOfFileLists], false);
        }
        
        tabbedComponent->addTab(translate("Samples"), AlphaTheme::getInstance()->foregroundColourDarker, treeLists[AUDIO_SAMPLES-noOfFileLists], false);
        tabbedComponent->addTab(translate("Effects"), AlphaTheme::getInstance()->foregroundColourDarker, fileLists[EFFECT_PRESETS], false);
        tabbedComponent->addTab(translate("Mode Presets"), AlphaTheme::getInstance()->foregroundColourDarker, fileLists[SAMPLER_PRESETS], false);

    }
    else if (mode == 3)
    {
        if (seqMode == 1)
            tabbedComponent->addTab(translate("Scales"), AlphaTheme::getInstance()->foregroundColourDarker, scalesListBox, false);
        
        else if (seqMode == 2)
        {
            tabbedComponent->addTab(translate("Banks"), AlphaTheme::getInstance()->foregroundColourDarker, treeLists[SAMPLE_BANKS-noOfFileLists], false);
            //the below will only be useable if we get drag-and-drop working
            //tabbedComponent->addTab(translate("Samples"), Colours::darkgrey, treeLists[AUDIO_SAMPLES-noOfFileLists], false);
            tabbedComponent->addTab(translate("Effects"), AlphaTheme::getInstance()->foregroundColourDarker, fileLists[EFFECT_PRESETS], false);
        }
        
        tabbedComponent->addTab(translate("Sequences"), AlphaTheme::getInstance()->foregroundColourDarker, treeLists[SEQUENCES-noOfFileLists], false);
        tabbedComponent->addTab(translate("Mode Presets"), AlphaTheme::getInstance()->foregroundColourDarker, fileLists[SEQUENCER_PRESETS], false);
    }
//    else if (mode == 4)
//    {
//        tabbedComponent->addTab(translate("Mode Presets"), AlphaTheme::getInstance()->foregroundColourDarker, fileLists[CONTROLLER_PRESETS], false);
//    }
    else
    {
        if (selectedPads.size() != 48)
            tabbedComponent->setVisible(false);
    }
    
    if (selectedPads.size() == 48)
        tabbedComponent->addTab(translate("Scene Presets"), AlphaTheme::getInstance()->foregroundColourDarker, treeLists[SCENE_PRESETS-noOfFileLists], false);
    
    int noOfTabs = tabbedComponent->getNumTabs();
    
    if (currentTabIndex >= noOfTabs || currentTabIndex < 0)
        currentTabIndex = 0;
    
    tabbedComponent->setCurrentTabIndex (currentTabIndex);
    
    //tabbedComponent->getTabbedButtonBar().setMinimumTabScaleFactor(1);
}

          
//callback functions for the filetree and filelist components
void Toolbox::selectionChanged()
{
  
}

void Toolbox::fileClicked (const File& file, const MouseEvent& e)
{
    //The below is needed so that when fileDoubleClicked is called we know what filebrowser called it
    
    if (treeLists[SAMPLE_BANKS-noOfFileLists]->isMouseOver(true))
        currentList = SAMPLE_BANKS;
    else if (treeLists[AUDIO_SAMPLES-noOfFileLists]->isMouseOver(true))
        currentList = AUDIO_SAMPLES;
    else if (treeLists[SEQUENCES-noOfFileLists]->isMouseOver(true))
        currentList = SEQUENCES;
    else if (treeLists[SCENE_PRESETS-noOfFileLists]->isMouseOver(true))
        currentList = SCENE_PRESETS;
    
    for (int i = 0; i < noOfFileLists; i++)
    {
        if (fileLists[i]->isMouseOver(true))
            currentList = i;
    }

}

void Toolbox::fileDoubleClicked (const File& file)
{
    if (file.isDirectory() == false)
    {
         int padNum = selectedPads[0];

        //====================================================================================
        //====================================================================================

        if (currentList == AUDIO_SAMPLES)
        {
            if (file.getFileExtension() == ".wav" || file.getFileExtension() == ".aif" || file.getFileExtension() == ".aiff")
            {
                for (int i = 0; i < selectedPads.size(); i++)
                {
                    int padNum = selectedPads[i];
                    PAD_SETTINGS->setSamplerAudioFilePath(file);
                }
                
                mainComponentRef.getGuiSamplerMode()->setAudioFileDisplay(file);
            }
        }


        //====================================================================================
        //====================================================================================

        else if (currentList == SAMPLE_BANKS)
        {
            if (file.getFileExtension() == ".alphabank")
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
                        if (i >= numOfSamples)
                            break;
                        
                        int padNum = selectedPads[i];
                        PAD_SETTINGS->setSamplerAudioFilePath(File(sampleFilePaths[i]));
                    }
                    
                    mainComponentRef.getGuiSamplerMode()->setAudioFileDisplay(File(sampleFilePaths[0]));
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
        }

        //====================================================================================
        //====================================================================================

        else if (currentList == SEQUENCES)
        {
            
            if (file.getFileExtension() == ".alphaseq")
            {
                int currentSeqNumber = mainComponentRef.getGuiSequencerMode()->getCurrentSequenceNumber()-1;
                mainComponentRef.getAppDocumentStateRef().loadSequence(currentSeqNumber, selectedPads, false, file);
            }
            
            else if (file.getFileExtension() == ".alphaseqset")
            {
                mainComponentRef.getAppDocumentStateRef().loadSequenceSet(selectedPads, false, file);
            }
            
        }

        //====================================================================================
        //====================================================================================

        else if (file.getFileExtension() == ".alphapad")
        {
            mainComponentRef.getAppDocumentStateRef().loadPadFromDisk(selectedPads, false, file);
        }
        
        else if (file.getFileExtension() == ".alphascene")
        {
            int currentScene = mainComponentRef.getAppDocumentStateRef().getCurrentlySelectedScene();
            
            bool shouldLoad = mainComponentRef.getAppDocumentStateRef().loadSceneFromDisk(currentScene, false, file);
            
            if (shouldLoad == true)
            {
                mainComponentRef.getAppDocumentStateRef().loadFromScene(currentScene);
            }
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
            
            mainComponentRef.getGuiPiano()->clearPiano();
            for (int i = 0; i < selectedPads.size(); i++)
            {
                padNum = selectedPads[i];
                int noteToApply = tokens[i].getIntValue() + rootNote;
                if (noteToApply > 119)
                    noteToApply = 119;
                else if (noteToApply < 0)
                    noteToApply = rootNote;
                
                PAD_SETTINGS->setMidiNote(noteToApply);
                mainComponentRef.getGuiPiano()->setNoteData(noteToApply);
            }
        }
        
        else if (PAD_SETTINGS->getMode() == 3) //sequencer mode
        {
            int rootNote = PAD_SETTINGS->getSequencerMidiNote(0); //will this be appropriate (root note = selectedPads[0] note)?
            
            mainComponentRef.getGuiPiano()->clearPiano();
            
            for (int padIndex = 0; padIndex < selectedPads.size(); padIndex++)
            {
                for (int row = 0; row < 12; row++)
                {
                    padNum = selectedPads[padIndex];
                    
                    int noteToApply = tokens[row].getIntValue() + rootNote;
                    if (noteToApply > 119)
                        noteToApply = -500;
                    else if (noteToApply < 0)
                        noteToApply = rootNote;
                    
                    PAD_SETTINGS->setSequencerMidiNote(noteToApply, row);
                    
                    if (padIndex == 0)
                    {
                        if (noteToApply >= 0)
                            mainComponentRef.getGuiPiano()->setNoteData(noteToApply);
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
        
        mainComponentRef.getGuiPiano()->clearPiano();
        for (int i = 0; i < selectedPads.size(); i++)
        {
            padNum = selectedPads[i];
            int noteToApply = tokens[i].getIntValue() + rootNote;
            if (noteToApply > 119)
                noteToApply = 119;
            
            PAD_SETTINGS->setMidiNote(noteToApply);
            mainComponentRef.getGuiPiano()->setNoteData(noteToApply);
        }
        
    }
    
}

void Toolbox::setTabColour()
{
    //This can't be called in paint as it calls repaint and would cause loops and high CPU.
    for (int i = 0; i < tabbedComponent->getNumTabs(); i++)
        tabbedComponent->setTabBackgroundColour(i, AlphaTheme::getInstance()->foregroundColourDarker);
}


void Toolbox::mouseEnter (const MouseEvent &e)
{
    /*
     if (tabbedComponent->isMouseOver(true))
     {
         mainComponentRef.setInfoTextBoxText (translate("Toolbox. This component will display a set of items and presets that can be applied to the selected pads based on their mode. Click on a tab to view the available items."));
     }
     */
    
    if (treeLists[AUDIO_SAMPLES-noOfFileLists]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("AlphaLive Sample Library Browser. Double-click any files here to apply them to the selected pads."));
    }
    else if (treeLists[SAMPLE_BANKS-noOfFileLists]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sample Banks. Double-click any items here to apply a bank of audio samples to either the sequence grid of Sequencer pads, or to a set of Sampler pads. Banks will be applied to a set of Sampler pads in the order that they were selected. Each bank contains 16 samples."));
    }
    else if (treeLists[SEQUENCES-noOfFileLists]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sequence Arrangements. Double-click any items here to apply pre-made sequences to the selected pads. These can either be single sequences or sequences sets for each pad."));
    }
    else if (fileLists[MIDI_PRESETS]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("MIDI Mode Pad Presets. Double-click any items here to apply preset MIDI settings to the selected pads. These presets will not contain any note or channel settings."));
    }
    else if (fileLists[SAMPLER_PRESETS]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sampler Mode Pad Presets. Double-click any items here to apply preset Sampler settings to the selected pads. These presets will not contain any audio files."));
    }
    else if (fileLists[SEQUENCER_PRESETS]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sequencer Mode Pad Presets. Double-click any items here to apply preset Sequencer settings to the selected pads. These presets will not contain any sequence arrangements, audio files, or MIDI note or channel settings."));
    }
    else if (fileLists[CONTROLLER_PRESETS]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Controller Mode Pad Presets. Double-click any items here to apply preset Controller settings to the selected pads."));
    }
    else if (fileLists[EFFECT_PRESETS]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Effects Presets. Double-click any items here to apply preset effects to the selected pads."));
    }
    else if (treeLists[SCENE_PRESETS-noOfFileLists]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Scene Presets. Double-click any items here to apply template mappings or settings to the entire set of pads of the currently selected scene."));
    }
    
    
    else if (scalesListBox->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("MIDI Scales. Double-click any items here to apply a scale of MIDI notes to either a set of MIDI pads, or to the sequence grid of Sequencer pads. Scales will be applied to a set of MIDI pads in the order that they were selected."));
    }
    else if (layoutsListBox->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("MIDI Notational Arrangements. Double-click any items here to apply an arrangement of MIDI notes to the entire set of pads. As opposed to scales, notational arrangements are based on a pads position in relation to other pads. Therefore the notes are applied to the set of pads in numerical order."));
    }
    
}

void Toolbox::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}
