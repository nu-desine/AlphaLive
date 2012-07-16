//
//  TestGuiSeqSamplesMode.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 17/10/2011.
//  Copyright 2011 nu desine.
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

#include "GuiSeqSamplesMode.h"
#include "../../File and Settings/AppSettings.h"
#include "../Binary Data/ImageSliderBinaryData.h"
#include "GlobalValues.h"
#include "MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[currentlySelectedPad]
#define PAD_SETTINGS_i AppSettings::Instance()->padSettings[i]

GuiSeqSamplesMode::GuiSeqSamplesMode(MainComponent &ref)
                                        :   mainComponentRef(ref)
{
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        audioSampleChooser[row] = new FilenameComponent ("audiofile ",
                                             File::nonexistent,
                                             false, false, false,
                                             "*.wav; *.aif; *.aiff",
                                             String::empty,
                                             "(choose a WAV or AIFF file)");
        audioSampleChooser[row]->addListener (this);					
        audioSampleChooser[row]->setBrowseButtonText ("+");	
        addAndMakeVisible (audioSampleChooser[row]);
        audioSampleChooser[row]->setVisible(false);
        audioSampleChooser[row]->addMouseListener(this, true);

    }
    
    //create gain slider
    addAndMakeVisible(gainSlider = new AlphaImageKnob(2));
    gainSlider->sliderComponent()->setRange(0.0, 2.0);
    gainSlider->sliderComponent()->addListener(this);
	gainSlider->sliderComponent()->setValue(0.7, false);
    gainSlider->sliderComponent()->addMouseListener(this, true);
	
	//create pan slider
    addAndMakeVisible(panSlider = new AlphaImageKnob(2, true));
    panSlider->sliderComponent()->setRange(0.0, 1.0);
    panSlider->sliderComponent()->addListener(this);
    panSlider->sliderComponent()->setValue(0.5, false);
    panSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(drumBankMenu = new ComboBox());
    drumBankMenu->addListener(this);
    drumBankMenu->addItem("None Selected...", 1);
    drumBankMenu->addItem("Drum Bank 1", 2);
    drumBankMenu->addItem("Drum Bank 2", 3);
    drumBankMenu->addItem("Drum Bank 3", 4);
    drumBankMenu->setSelectedId(1, true);
    drumBankMenu->addMouseListener(this, true);
    
}

GuiSeqSamplesMode::~GuiSeqSamplesMode()
{
    deleteAllChildren();
}

void GuiSeqSamplesMode::setCurrentlySelectedPad (int padNumber)
{
    currentlySelectedPad = padNumber;
}

void GuiSeqSamplesMode::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/dissabled any unneeded components. 
    
    //if an individual pad number is currently selected
    if(currentlySelectedPad < 99)
    {
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            audioSampleChooser[row]->setCurrentFile(PAD_SETTINGS->getSequencerSamplesAudioFilePath(row), true, false);
        }
        
        drumBankMenu->setSelectedId(1, true);
        gainSlider->sliderComponent()->setValue(PAD_SETTINGS->getSequencerGain(), true);
        panSlider->sliderComponent()->setValue(PAD_SETTINGS->getSequencerPan(), true);
        
    }
    
    
    //if 'all pads' selected
    if(currentlySelectedPad == 99)
    {
        //set to a default setting
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            audioSampleChooser[row]->setCurrentFile(File::nonexistent, false, false);
        }
        
        drumBankMenu->setSelectedId(1, true);
        gainSlider->sliderComponent()->setValue(0.7, true);
        panSlider->sliderComponent()->setValue(0.5, true);
        
    }
    
    //if a 'row' is selected
    if(currentlySelectedPad > 99)
    {
        //set to a default setting
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            audioSampleChooser[row]->setCurrentFile(File::nonexistent, false, false);
        }
        
        drumBankMenu->setSelectedId(1, true);
        gainSlider->sliderComponent()->setValue(0.7, true);
        panSlider->sliderComponent()->setValue(0.5, true);
        
    }

    
}



void GuiSeqSamplesMode::resized()
{    
    for (float row = NO_OF_ROWS-1, i = 415.0; row >= 0; row--)
    {
        audioSampleChooser[int(row)]->setBounds(680, i, 75, 10);
        
        i += 12.5;
    }
    
    gainSlider->setBounds(835, 495, 45, 45);
	panSlider->setBounds(910, 495, 45, 45);
    
    drumBankMenu->setBounds(RIGHT_CIRCLE_X, 560, COMPONENT_W, COMPONENT_H);
    
    
}




void GuiSeqSamplesMode::comboBoxChanged (ComboBox* comboBox)
{
    
    if (comboBox == drumBankMenu)
    {
        if (drumBankMenu->getSelectedId() != 1) //if a bank is selected
        {
            bool exitCommand = false;
            bool audioFilesNotFound = false;
            
            //find directory which holds the audio files based on the current application file location
            File drumBankDirectory = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() + "/Audio Samples";
            
            //========================================================================================================================================
            //if can't find it, prompt the user to manually search for it. 
            if (drumBankDirectory.exists() == false)
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Directory Missing!", "The directory entitled 'Audio Samples' which should be located in the AlphaLive folder alongside this application can not be found. You must locate it manually in order to load up the drum bank sounds!");
                
                FileChooser loadDirectoryChooser("Select the AlphaLive Audio Samples Directory...", File::nonexistent, "*Audio Samples");
                
                if (loadDirectoryChooser.browseForDirectory() == true)
                {
                    //get users selected directory (which should be the audio samples directory)
                    File foundDirectory = loadDirectoryChooser.getResult();
                    //check to see if it is the right directory 
                    //Whats the best way to do this? Look for a hidden file which I'll put it as an identifier? Use File::getChildFiles like used when looking for duplicate audio files)
                    if (foundDirectory.getFileNameWithoutExtension() == "Audio Samples")//correct dir
                    {
                        //copy it to where it should be. Don't delete the old dir incase they've selected an incorrect folder and the check above didn't flag it
                        foundDirectory.copyDirectoryTo(drumBankDirectory);
                    }
                    else //incorrect dir
                    {
                        AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Incorrect Directory!", "You did not select the correct directory!");
                        exitCommand = true;
                    }
                }
                else //user pressed cancel button;
                {
                    exitCommand = true;
                }
                
            }
            //========================================================================================================================================
            
            if (exitCommand == false)
            {
                //if individual pad number is selected
                if(currentlySelectedPad < 99)
                {
                    for (int seqRow = 0; seqRow <= 11; seqRow++)
                    {
                        //get correct audio file based on bank selected
                        File audioFile = drumBankDirectory.getFullPathName() + "/Kit"+String(drumBankMenu->getSelectedId()-1)+"/Kit"+String(drumBankMenu->getSelectedId()-1)+"_drum"+String(seqRow+1)+".aif";
                        //if it exists put audio file into correct padSettings variable
                        if (audioFile.existsAsFile() == true)
                        {
                            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioFile, seqRow);
                        }
                        else //set a flag to let the user know at least one audio file was not found
                        {
                            audioFilesNotFound = true;
                        }
                    }
                    
                }
                
                //if 'all pads' selected
                if(currentlySelectedPad == 99)
                {
                    for(int i = 0; i <= 47; i++)
                    {
                        for (int seqRow = 0; seqRow <= 11; seqRow++)
                        {
                            //get correct audio file based on bank selected
                            File audioFile = drumBankDirectory.getFullPathName() + "/Kit"+String(drumBankMenu->getSelectedId()-1)+"/Kit"+String(drumBankMenu->getSelectedId()-1)+"_drum"+String(seqRow+1)+".aif";
                            //if it exists put audio file into correct padSettings variable
                            if (audioFile.existsAsFile() == true)
                            {
                                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioFile, seqRow);
                            }
                            else //set a flag to let the user know at least one audio file was not found
                            {
                                audioFilesNotFound = true;
                            }
                        }

                        
                    }
                }
                
                //if a 'row' is selected
                if(currentlySelectedPad > 99)
                {
                    int row = currentlySelectedPad - 99; 
                    
                    for(int i = (row*8)-8; i <= (row*8)-1; i++) 
                    {
                        for (int seqRow = 0; seqRow <= 11; seqRow++)
                        {
                            //get correct audio file based on bank selected
                            File audioFile = drumBankDirectory.getFullPathName() + "/Kit"+String(drumBankMenu->getSelectedId()-1)+"/Kit"+String(drumBankMenu->getSelectedId()-1)+"_drum"+String(seqRow+1)+".aif";
                            //if it exists put audio file into correct padSettings variable
                            if (audioFile.existsAsFile() == true)
                            {
                                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioFile, seqRow);
                            }
                            else //set a flag to let the user know at least one audio file was not found
                            {
                                audioFilesNotFound = true;
                            }
                        }

                        
                    }
                }
            }
            
            else if (exitCommand == true) //if the app has been prompted to not load anything up
            {
                //reset ComboBox
                drumBankMenu->setSelectedId(1, true);
            }
            //========================================================================================================================================
            
            
            if (audioFilesNotFound == true)
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Missing Files", "One or more audio samples were not found. You have done something TERRIBLE!");
            }
            
        }

    }

    
    
}

void GuiSeqSamplesMode::sliderValueChanged (Slider* slider)
{
    if (slider == gainSlider->sliderComponent())
    {
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerGain(gainSlider->sliderComponent()->getValue());
        }
        
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerGain(gainSlider->sliderComponent()->getValue());            
            }
        }
        
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int row = currentlySelectedPad - 99; 
            
            for(int i = (row*8)-8; i <= (row*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerGain(gainSlider->sliderComponent()->getValue());
            }
        }
    }
    
    //pan slider
    if (slider == panSlider->sliderComponent())
    {
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerPan(panSlider->sliderComponent()->getValue());
        }
        
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerPan(panSlider->sliderComponent()->getValue());            
            }
        }
        
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int row = currentlySelectedPad - 99; 
            
            for(int i = (row*8)-8; i <= (row*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerPan(panSlider->sliderComponent()->getValue());
            }
        }
        
        
    }

    
}

void GuiSeqSamplesMode::buttonClicked (Button* button)
{
    
}

void GuiSeqSamplesMode::filenameComponentChanged (FilenameComponent* filenameComponent)
{
    
    if (filenameComponent == audioSampleChooser[0])
    {
        int row = 0;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[1])
    {
        int row = 1;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[2])
    {
        int row = 2;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[3])
    {
        int row = 3;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[4])
    {
        int row = 4;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[5])
    {
        int row = 5;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[6])
    {
        int row = 6;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[7])
    {
        int row = 7;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[8])
    {
        int row = 8;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[9])
    {
        int row = 9;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[10])
    {
        int row = 10;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }
    if (filenameComponent == audioSampleChooser[11])
    {
        int row = 11;
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this slider in the pad settings of that pad
            PAD_SETTINGS->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
        }
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);          
            }
        }
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int alphaRow = currentlySelectedPad - 99; 
            
            for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
            {
                //i = pad number
                PAD_SETTINGS_i->setSequencerSamplesAudioFilePath(audioSampleChooser[row]->getCurrentFile(), row);
            }
        }
    }

}


void GuiSeqSamplesMode::setComponentsVisible(bool isVisible)
{
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        audioSampleChooser[row]->setVisible(isVisible);
        
    } 
}


void GuiSeqSamplesMode::mouseEnter (const MouseEvent &e)
{
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        if (audioSampleChooser[row]->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText("Row " + String(row+1) + " Audio Sample Selector. Sets and displays the filepath name of the audio file for sequencer grid row " + String(row+1) + " for the selected pad/pads."/*Use the '+' button to open a File Browser Window, or use the drop-down menu to select from recently selected files, as well as view the currently selected file.*/);
        }
    }
    if (panSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Pan Knob. Sets and displays the panning/stereo positioning of the audio signal of the sequence for the selected pad/pads.");
    }
    else if (gainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Gain Knob. Sets and displays the gain/volume of the audio signal of the sequence for the selected pad/pads.");
    }
    else if (drumBankMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Drum Bank Drop-Down Menu. Sets a set of built-in drum samples to the sequencer for the selected pad/pads. FEATURE COMING SOON!");
    }
    
    
}

void GuiSeqSamplesMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty); 
}

