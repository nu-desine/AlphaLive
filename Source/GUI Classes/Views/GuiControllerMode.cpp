//
//  TestGuiControllerMode.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 29/09/2011.
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

#include "GuiControllerMode.h"
#include "GlobalValues.h"
#include "../Custom Components/Scene/SceneValues.h"
#include "../../File and Settings/AppSettings.h"
#include "MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiControllerMode::GuiControllerMode(MainComponent &ref)
                                            :   mainComponentRef(ref)
{
    //currentlySelectedPad = 99;
    
    addAndMakeVisible(speakerLeft = new GuiSpeaker());
    addAndMakeVisible(circleBackgroundSmall = new GuiCircleBackground());
    addAndMakeVisible(circleBackgroundRight = new GuiCircleBackground());
    
    addAndMakeVisible(controlMenu = new ComboBox());
    controlMenu->addListener(this);
    controlMenu->addMouseListener(this, true);
    controlMenu->addItem("None Selected...", 1);
    controlMenu->addItem("Scene Switcher", 2);
    controlMenu->addItem("MIDI Program Change", 4);
    controlMenu->addItem("Scene & MIDI Program Switcher", 5);
    controlMenu->addItem("OSC Ouput", 3);
    controlMenu->setSelectedId(1, true);
    
    addAndMakeVisible(sceneNumberSlider = new AlphaSlider());
    sceneNumberSlider->setRange(1, NO_OF_SCENES, 1);
    sceneNumberSlider->addListener(this);
    sceneNumberSlider->setValue(1, false);
    sceneNumberSlider->addMouseListener(this, true);
    sceneNumberSlider->setVisible(false);
    
    //for some reason using a textEditor was creating gui problems, so using a label instead now
    addAndMakeVisible(oscIpAddressEditor = new Label());
    oscIpAddressEditor->setText("127.0.0.1", false);
    oscIpAddressEditor->setColour(Label::textColourId, Colours::grey);
    oscIpAddressEditor->setColour(Label::backgroundColourId, Colours::lightgrey);
    oscIpAddressEditor->setJustificationType(Justification::centred);
    oscIpAddressEditor->setEditable(true);
    oscIpAddressEditor->addMouseListener(this, true);
    oscIpAddressEditor->addListener(this);
    oscIpAddressEditor->setVisible(false);
    //AT SOME POINT USE LABEL::CREATEEDITORCOMPONENT() TO SET INPUT RESTRICTIONS TO THE EDITOR
    
    addAndMakeVisible(oscPortNumberSlider = new AlphaSlider());
    oscPortNumberSlider->setRange(0, 65535, 1);
    oscPortNumberSlider->addListener(this);
    oscPortNumberSlider->setValue(5004, false);
    oscPortNumberSlider->addMouseListener(this, true);
    oscPortNumberSlider->setVisible(false);
    
    addAndMakeVisible(midiProgramChangeNumberSlider = new AlphaSlider());
    midiProgramChangeNumberSlider->setRange(0, 127, 1);
    midiProgramChangeNumberSlider->setValue(1, false);
    midiProgramChangeNumberSlider->addListener(this);
    midiProgramChangeNumberSlider->addMouseListener(this, true);
    midiProgramChangeNumberSlider->setVisible(false);
    
    addAndMakeVisible(midiProgramChangeChannelSlider = new AlphaSlider());
    midiProgramChangeChannelSlider->setRange(1, 16, 1);
    midiProgramChangeChannelSlider->setValue(1, false);
    midiProgramChangeChannelSlider->addListener(this);
    midiProgramChangeChannelSlider->addMouseListener(this, true);
    midiProgramChangeChannelSlider->setVisible(false);


    
}



GuiControllerMode::~GuiControllerMode()
{
    deleteAllChildren();
}



void GuiControllerMode::resized()
{
    speakerLeft->setBounds(14, 402, 230, 230);
    circleBackgroundSmall->setBounds(850, 251, 142, 142);
    circleBackgroundRight->setBounds(780, 402, 230, 230);
    
    controlMenu->setBounds(RIGHT_CIRCLE_X, 445, COMPONENT_W, COMPONENT_H);
    
    //only one of these (or a group of these) will be displayed at any time depending on the value of control menu
    sceneNumberSlider->setBounds(RIGHT_CIRCLE_X, 480, COMPONENT_W, COMPONENT_H);

    oscIpAddressEditor->setBounds(RIGHT_CIRCLE_X, 480, COMPONENT_W, COMPONENT_H);
    oscPortNumberSlider-> setBounds(RIGHT_CIRCLE_X, 505, COMPONENT_W, COMPONENT_H);
    
    midiProgramChangeNumberSlider->setBounds(RIGHT_CIRCLE_X, 505, COMPONENT_W, COMPONENT_H);
    midiProgramChangeChannelSlider->setBounds(RIGHT_CIRCLE_X, 530, COMPONENT_W, COMPONENT_H);
}



void GuiControllerMode::paint (Graphics& g)
{
    
}



void GuiControllerMode::setCurrentlySelectedPad (Array <int> selectedPads_)
{
    selectedPads = selectedPads_;
}




void GuiControllerMode::comboBoxChanged (ComboBox* comboBox)
{
    //==============================================================================
    //pressure mode combobox
    if (comboBox == controlMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setControllerControl(controlMenu->getSelectedId());
        }
        
         
        //==============set what other components are visible===============
        
        //first set other componets invisble (saves having to set the visibility of each one in each next if statement)... 
        sceneNumberSlider->setVisible(false);
        oscIpAddressEditor->setVisible(false);
        oscPortNumberSlider->setVisible(false);
        midiProgramChangeNumberSlider->setVisible(false);
        midiProgramChangeChannelSlider->setVisible(false);
        
        //...and then set the right one visible
        if (controlMenu->getSelectedId() == 2) //scene switcher control selected
        {
            sceneNumberSlider->setVisible(true);
        }
        else if(controlMenu->getSelectedId() == 3) //OSC output control selected
        {
            oscIpAddressEditor->setVisible(true);
            oscPortNumberSlider->setVisible(true);
        }
        else if(controlMenu->getSelectedId() == 4) //MIDI program change
        {
            midiProgramChangeNumberSlider->setVisible(true);
            midiProgramChangeChannelSlider->setVisible(true);
        }
        else if(controlMenu->getSelectedId() == 5) //Dual Scene/MIDI program change
        {
            sceneNumberSlider->setVisible(true);
            midiProgramChangeNumberSlider->setVisible(true);
            midiProgramChangeChannelSlider->setVisible(true);
        }
        
        
    }
    
    //==============================================================================

    
}

void GuiControllerMode::sliderValueChanged (Slider* slider)
{
    
    //channel slider
    if (slider == sceneNumberSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setControllerSceneNumber(sceneNumberSlider->getValue());
        }
    }
    
    
    
    //OSC port number slider slider
    else if (slider == oscPortNumberSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setControllerOscPort(oscPortNumberSlider->getValue());
        }
        
    }
    
    
    
    //MIDI program change number slider
    else if (slider == midiProgramChangeNumberSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setControllerMidiProgramChangeNumber(midiProgramChangeNumberSlider->getValue());
        }
        
    }
    
    
    //MIDI program change channel slider
    else if (slider == midiProgramChangeChannelSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setControllerMidiProgramChangeChannel(midiProgramChangeChannelSlider->getValue());
        }
        
    }

    
}

void GuiControllerMode::labelTextChanged (Label* labelThatHasChanged)
{
    if (labelThatHasChanged == oscIpAddressEditor)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setControllerOscIpAddress(oscIpAddressEditor->getText());
        }
    }
}

void GuiControllerMode::textEditorReturnKeyPressed (TextEditor& editor)
{
    /*
    if (&editor == ipAddressEditor)
    {
        std::cout << "text added!!\n";
    }
     */
}

void GuiControllerMode::textEditorTextChanged (TextEditor& editor)
{
    /*
    if (&editor == ipAddressEditor)
    {
        std::cout << "text changed!!\n";
    }
     */
}

void GuiControllerMode::buttonClicked (Button* button)
{
    
}

void GuiControllerMode::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/dissabled any unneeded components. 
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        controlMenu->setSelectedId(PAD_SETTINGS->getControllerControl(), true);
        sceneNumberSlider->setValue(PAD_SETTINGS->getControllerPresentNumber(), false);
        oscIpAddressEditor->setText(PAD_SETTINGS->getControllerOscIpAddress(), false);
        oscPortNumberSlider->setValue(PAD_SETTINGS->getControllerOscPort(), false);
        midiProgramChangeNumberSlider->setValue(PAD_SETTINGS->getControllerMidiProgramChangeNumber(), false);
        midiProgramChangeChannelSlider->setValue(PAD_SETTINGS->getControllerMidiProgramChangeChannel(), false);
        
    }
    
    else if(MULTI_PADS)
    {
        controlMenu->setSelectedId(1, true);
        sceneNumberSlider->setValue(1, false);
        oscIpAddressEditor->setText("127.0.0.1", false);
        oscPortNumberSlider->setValue(5004, false);
        midiProgramChangeNumberSlider->setValue(1, false);
        midiProgramChangeChannelSlider->setValue(1, false);
    }
    

    // ============show/hide relevent components==============
    //first set other componets invisble (saves having to set the visibility of each one in each next if statement)... 
    sceneNumberSlider->setVisible(false);
    oscIpAddressEditor->setVisible(false);
    oscPortNumberSlider->setVisible(false);
    midiProgramChangeNumberSlider->setVisible(false);
    midiProgramChangeChannelSlider->setVisible(false);
    
    //...and then set the right one visible
    if (controlMenu->getSelectedId() == 2) //scene switcher control selected
    {
        sceneNumberSlider->setVisible(true);
    }
    else if(controlMenu->getSelectedId() == 3) //OSC output control selected
    {
        oscIpAddressEditor->setVisible(true);
        oscPortNumberSlider->setVisible(true);
    }
    else if(controlMenu->getSelectedId() == 4) //MIDI program change
    {
        midiProgramChangeNumberSlider->setVisible(true);
        midiProgramChangeChannelSlider->setVisible(true);
    }
    else if(controlMenu->getSelectedId() == 5) //Dual Scene/MIDI program change
    {
        sceneNumberSlider->setVisible(true);
        midiProgramChangeNumberSlider->setVisible(true);
        midiProgramChangeChannelSlider->setVisible(true);
    }

    
    
}


void GuiControllerMode::mouseEnter (const MouseEvent &e)
{
    if (controlMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Control Drop-Down Menu. Sets and displays the control for the selected pad/pads.");
    }
    else if (sceneNumberSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Scene Number Selector. Sets and displays the scene number for the selected pad/pads. When the pad is pressed it will cause settings to be loaded from the selected scene.");
    }
    else if (oscIpAddressEditor->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("IP Address Editor. Sets and displays the IP address of the device that you want to send OSC messages to from the selected pad/pads. Set to 127.0.0.1 to send OSC messages somewhere on the same computer.");
    }
    else if (oscPortNumberSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Port Number Selector. Sets and displays the UDP/TCP port number that you want to send OSC messages over from the selected pad/pads. Warning: Do not select port 5003 as this is AlphaLive's listening port!");
    }
    else if (midiProgramChangeNumberSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("MIDI Program Change Number Selector. Sets and displays the MIDI Program Change Number for the selected pad/pads.");
    }
    else if (midiProgramChangeChannelSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("MIDI Program Change Channel Selector. Sets and displays the MIDI Program Change Channel for the selected pad/pads.");
    }
    
    
}

void GuiControllerMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty); 
}