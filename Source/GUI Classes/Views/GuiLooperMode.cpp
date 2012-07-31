//
//  GuiLooperMode.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 25/09/2011.
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

#include "GuiLooperMode.h"
#include "../../File and Settings/AppSettings.h"
#include "../Binary Data/ImageSliderBinaryData.h"
#include "GlobalValues.h"
#include "MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GuiLooperMode::GuiLooperMode(MainComponent &ref)
: mainComponentRef(ref)
										
{
    //currentlySelectedPad = 99;
    
    //create circle background graphic
    addAndMakeVisible(circleBackgroundRight = new GuiCircleBackground());
    
    fileChooser = new FilenameComponent ("audiofile",
										 File::nonexistent,
										 false, false, false,
										 "*.wav; *.aif; *.aiff",
										 String::empty,
										 "(choose a WAV or AIFF file)");
	fileChooser->addListener (this);					
	fileChooser->setBrowseButtonText ("+");
	fileChooser->setMaxNumberOfRecentFiles (20);
	addAndMakeVisible (fileChooser);
    fileChooser->addMouseListener(this, true);
    
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

    
    addAndMakeVisible(triggerModeMenu = new ComboBox());
    triggerModeMenu->addListener(this);
    triggerModeMenu->addItem("Hold", 1);
    triggerModeMenu->addItem("Toggle", 2);
    triggerModeMenu->addItem("Toggle Release", 3);
    triggerModeMenu->addItem("Latch", 4);
    triggerModeMenu->addItem("Latch Max", 5);
    triggerModeMenu->addItem("Trigger", 6);
    triggerModeMenu->setSelectedId(2, true);
    triggerModeMenu->addMouseListener(this, true);
    
    addAndMakeVisible(loopButton = new TextButton("Loop"));
    loopButton->addListener(this);
    loopButton->addMouseListener(this, true);
    loopButton->setClickingTogglesState(true);
    loopButton->setToggleState(0, false);
    
    addAndMakeVisible(indestructibleButton = new TextButton("indestruct"));
    indestructibleButton->addListener(this);
    indestructibleButton->addMouseListener(this, true);
    indestructibleButton->setClickingTogglesState(true);
    indestructibleButton->setToggleState(0, false);
    
    addAndMakeVisible(finishLoopButton = new TextButton("Finish"));
    finishLoopButton->addListener(this);
    finishLoopButton->addMouseListener(this, true);
    finishLoopButton->setClickingTogglesState(true);
    finishLoopButton->setToggleState(0, false);
    
    addAndMakeVisible(stickyButton = new TextButton("Sticky"));
    stickyButton->addListener(this);
    stickyButton->addMouseListener(this, true);
    stickyButton->setClickingTogglesState(true);
    stickyButton->setToggleState(0, false);
    
    addAndMakeVisible(fxDial = new GuiFxDial(mainComponentRef));
	fxDial->setInterceptsMouseClicks(false, true);
    fxDial->addMouseListener(this, false);
	
	
	addAndMakeVisible (waveform = new DemoThumbnailComp());
	waveform->setInterceptsMouseClicks(false, false);
    
    addAndMakeVisible(quantizeModeMenu = new ComboBox());
    quantizeModeMenu->addItem("Free", 1);
    quantizeModeMenu->addItem("Quantized", 2);
    quantizeModeMenu->setSelectedId(1, true);
    quantizeModeMenu->addMouseListener(this, true);
    quantizeModeMenu->addListener(this);
    
    addAndMakeVisible(pressureModeMenu = new ComboBox());
    //ideally, we should have an 'off' here instead of having a 'no effect' elsewhere
    pressureModeMenu->addItem("Effect processing", 1);
    pressureModeMenu->addItem("Playback manipulation", 2);
    pressureModeMenu->setSelectedId(1, true);
    pressureModeMenu->addMouseListener(this, true);
    pressureModeMenu->addListener(this);
    
}


GuiLooperMode::~GuiLooperMode()
{
    
    deleteAllChildren();
    
}


void GuiLooperMode::resized()
{
    circleBackgroundRight->setBounds(780, 402, 230, 230);
	
    waveform->setBounds(790, 412, 210, 210);
	
    fileChooser->setBounds(RIGHT_CIRCLE_X, 475, COMPONENT_W, COMPONENT_H);
    
    gainSlider->setBounds(845, 495, 45, 45);
	panSlider->setBounds(900, 495, 45, 45);
        
    triggerModeMenu->setBounds(RIGHT_CIRCLE_X, 538, COMPONENT_W, COMPONENT_H);
    loopButton->setBounds(RIGHT_CIRCLE_X, 558, 50, 15);
    indestructibleButton->setBounds(RIGHT_CIRCLE_X+50, 558, 50, 15);
    finishLoopButton->setBounds(RIGHT_CIRCLE_X, 573, 50, 15);
    stickyButton->setBounds(RIGHT_CIRCLE_X+50, 573, 50, 15);
    
    fxDial->setBounds(14, 402, 230, 230);
	
	//quantizeModeMenu->setBounds(800, 155, 100, 20);
    
	//gainSlider->setBounds(965, 150, 45, 45);
	//panSlider->setBounds(965, 200, 45, 45);
    
    //test playback manipulation stuff
    //pressureModeMenu->setBounds(10, getHeight()-60, 50, 20);

    
}




void GuiLooperMode::comboBoxChanged (ComboBox* comboBox)
{
    //This method takes the selected IDs from the combo box and sets it to the correct
    //variable in the relevent padSettings index/es
    
    //==============================================================================
    //play state combobox
    if (comboBox == triggerModeMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperTriggerMode(triggerModeMenu->getSelectedId());
        }
    }
    
    
    //==============================================================================
    //trigger mode combobox
    if (comboBox == quantizeModeMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperQuantizeMode(quantizeModeMenu->getSelectedId());
        }
        
    }
    
    //==============================================================================
    /*
    //pressure mode combobox
    if (comboBox == pressureModeMenu)
    {
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            //store the value of this combo box in the pad settings of that pad
            //PAD_SETTINGS->setLooperQuantizeMode(quantizeModeMenu->getSelectedId());
        }
        
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                //PAD_SETTINGS_i->setLooperQuantizeMode(quantizeModeMenu->getSelectedId());
            }
        }
        
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int row = currentlySelectedPad - 99; 
            
            for(int i = (row*8)-8; i <= (row*8)-1; i++) 
            {
                //i = pad number
                //PAD_SETTINGS_i->setLooperQuantizeMode(quantizeModeMenu->getSelectedId());
            }
        }
        
        if (pressureModeMenu->getSelectedId() == 1) //DSP effects
        {
            fxDial->setVisible(true);
        }
        else if (pressureModeMenu->getSelectedId() == 2) //playback manipulation
        {
            fxDial->setVisible(false);
        }
    }
     */


}


void GuiLooperMode::sliderValueChanged (Slider* slider)
{
    //==============================================================================
    //gain slider
    if (slider == gainSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperGain(gainSlider->sliderComponent()->getValue());
        }
    }
    
    //==============================================================================
    //pan slider
    if (slider == panSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperPan(panSlider->sliderComponent()->getValue());
        }
        
    }
        //==============================================================================

}


void GuiLooperMode::filenameComponentChanged (FilenameComponent* filenameComponent)
{
    if (filenameComponent == fileChooser)
    {
        // this is called when the user changes the filename in the file chooser box
        File audioFile (filenameComponent->getCurrentFile().getFullPathName());
        
        if(audioFile.existsAsFile())
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setLooperAudioFilePath(audioFile);
				
				waveform->setFile (audioFile);
				
            }
            
        }
        
        else
        {
            AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                         "AlphaLive",
                                         "Couldn't open file!\n\n");
        }	
    }
    
}



void GuiLooperMode::buttonClicked (Button* button)
{
    if (button == loopButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperShouldLoop(button->getToggleState());
        }
    }
    
    else if (button == indestructibleButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperIndestructible(button->getToggleState());
        }
    }
    
    else if (button == finishLoopButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperShouldFinishLoop(button->getToggleState());
        }
    }
    else if (button == stickyButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperSticky(button->getToggleState());
        }
    }
}


void GuiLooperMode::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    fxDial->setCurrentlySelectedPad(selectedPads);
}

void GuiLooperMode::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/dissabled any unneeded components. 
    
    //if an individual pad number is currently selected
    if (SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        //fileChooser->setVisible(true);
        //fileChooserLabel->setVisible(true);
        
        fileChooser->setCurrentFile(PAD_SETTINGS->getLooperAudioFilePath(), true, false);
		File audioFile(PAD_SETTINGS->getLooperAudioFilePath());
		waveform->setFile (audioFile);
		
        gainSlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperGain(), false);
        panSlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperPan(), false);
        triggerModeMenu->setSelectedId(PAD_SETTINGS->getLooperTriggerMode(), true);
        loopButton->setToggleState(PAD_SETTINGS->getLooperShouldLoop(), false);
        indestructibleButton->setToggleState(PAD_SETTINGS->getLooperIndestructible(), false);
        finishLoopButton->setToggleState(PAD_SETTINGS->getLooperShouldFinishLoop(), false);
        stickyButton->setToggleState(PAD_SETTINGS->getLooperSticky(), false);
        quantizeModeMenu->setSelectedId(PAD_SETTINGS->getLooperQuantizeMode(), true);
        
        
    }

    else if (MULTI_PADS)
    {
        //fileChooser->setVisible(false);
        //fileChooserLabel->setVisible(false);
        
        //set default values
        fileChooser->setCurrentFile(File::nonexistent, false, false);
        gainSlider->sliderComponent()->setValue(0.7, false);
        panSlider->sliderComponent()->setValue(0.5, false);
        triggerModeMenu->setSelectedId(2, true);
        loopButton->setToggleState(1, false);
        indestructibleButton->setToggleState(0, false);
        finishLoopButton->setToggleState(0, false);
        stickyButton->setToggleState(0, false);
        quantizeModeMenu->setSelectedId(1, true);
        

        
    }

    fxDial->updateDisplay();
}



void GuiLooperMode::mouseEnter (const MouseEvent &e)
{
    
    if (fileChooser->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Audio File Selector. Sets and displays the filepath name of the audio file for the selected pad/pads. Use the '+' button to open a File Browser Window, or use the drop-down menu to select from recently selected files, as well as view the currently selected file.");
    }
    else if (panSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Pan Knob. Sets and displays the panning/stereo positioning of the selected pad/pads audio signal.");
    }
    else if (gainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Gain Knob. Sets and displays the gain/volume of the selected pad/pads audio signal.");
    }
    else if (triggerModeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("TriggerMode Drop-down Menu. \nSets and displays the looper TriggerMode for the selected pad/pads. Looper TriggerModes determine how interation with a pad controls the playback and modification of an audio file.");
    }
    else if (fxDial->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Looper FX Dial. Sets and displays the audio processing effect that the selected pad/pads pressure controls.");
    }
    else if (quantizeModeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Trigger Mode Menu. The trigger mode determines the start time of the loop. 'Free' will trigger the loop as soon as the pad is pressed, 'Quantized' will trigger the loop at the next quantized time value set using the global clock.");
    }
    
}

void GuiLooperMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}


