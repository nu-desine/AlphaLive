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
    /*
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
     */
    
    for (int i = 0; i < 4; i++)
    {
        if (i == 0)
            controlButtons.insert(i, new SettingsButton(translate("SCENE SWITCH"), (270 * (M_PI / 180)), 
                                                        (315 * (M_PI / 180)),
                                                        0.3f, -90, 0.45, 0.8));
        if (i == 1)
            controlButtons.insert(i, new SettingsButton(translate("MIDI PROGRAM"), (315 * (M_PI / 180)),
                                                        (2 * M_PI) , 0.3f, -90, 0.45, 0.8));
        if (i == 2)
            controlButtons.insert(i, new SettingsButton(translate("SCENE/PROGRAM"), 0.0f, (45 * (M_PI / 180)), 
                                                        0.3f, 90, 0.55, 0.2));
        if (i == 3)
            controlButtons.insert(i, new SettingsButton(translate("OSC MESSAGE"), (45 * (M_PI / 180)), 
                                                        (90 * (M_PI / 180)), 0.3f, 90, 0.55, 0.3));
        
        controlButtons[i]->addListener(this);
        controlButtons[i]->setOpaque(false);
        controlButtons[i]->setRadioGroupId (43);
        controlButtons[i]->addMouseListener(this, false);
        addAndMakeVisible(controlButtons[i]);
        
    }
    
    controlButtons[0]->setToggleState(true, false);
    
    
    addAndMakeVisible(sceneNumberSlider = new AlphaSlider());
    sceneNumberSlider->setRange(1, NO_OF_SCENES, 1);
    sceneNumberSlider->addListener(this);
    sceneNumberSlider->setValue(1, false);
    sceneNumberSlider->addMouseListener(this, true);
    sceneNumberSlider->setVisible(false);
    
    //for some reason using a textEditor was creating gui problems, so using a label instead now
    addChildComponent(oscIpAddressEditor = new Label());
    oscIpAddressEditor->setText("127.0.0.1", false);
    oscIpAddressEditor->setColour(Label::textColourId, Colours::grey);
    oscIpAddressEditor->setColour(Label::backgroundColourId, Colours::lightgrey);
    oscIpAddressEditor->setJustificationType(Justification::centred);
    oscIpAddressEditor->setEditable(true);
    oscIpAddressEditor->addMouseListener(this, true);
    oscIpAddressEditor->addListener(this);
    //AT SOME POINT USE LABEL::CREATEEDITORCOMPONENT() TO SET INPUT RESTRICTIONS TO THE EDITOR
    
    addChildComponent(oscPortNumberSlider = new AlphaSlider());
    oscPortNumberSlider->setRange(0, 65535, 1);
    oscPortNumberSlider->addListener(this);
    oscPortNumberSlider->setValue(5004, false);
    oscPortNumberSlider->addMouseListener(this, true);
    
    addChildComponent(midiProgramChangeNumberSlider = new AlphaSlider());
    midiProgramChangeNumberSlider->setRange(0, 127, 1);
    midiProgramChangeNumberSlider->setValue(1, false);
    midiProgramChangeNumberSlider->addListener(this);
    midiProgramChangeNumberSlider->addMouseListener(this, true);
    midiProgramChangeNumberSlider->setVisible(false);
    
    //---------------channel buttons---------------------
	
	
	for (int i = 0; i <= 15; i++)
	{
		midiChannelButtons.insert(i, new AlphaTextButton());
		midiChannelButtons[i]->setButtonText(String(i + 1));
		midiChannelButtons[i]->setClickingTogglesState(true);
		midiChannelButtons[i]->setToggleState(false, false);	
		midiChannelButtons[i]->setRadioGroupId (12);
		midiChannelButtons[i]->addListener(this);
        midiChannelButtons[i]->addMouseListener(this, true);
		midiChannelButtons[i]->setOpaque(false);
        
        addChildComponent(midiChannelButtons[i]);
	}
    
    /*
    addAndMakeVisible(midiProgramChangeChannelSlider = new AlphaSlider());
    midiProgramChangeChannelSlider->setRange(1, 16, 1);
    midiProgramChangeChannelSlider->setValue(1, false);
    midiProgramChangeChannelSlider->addListener(this);
    midiProgramChangeChannelSlider->addMouseListener(this, true);
    midiProgramChangeChannelSlider->setVisible(false);
     */


    
}



GuiControllerMode::~GuiControllerMode()
{
    controlButtons.clear();
    midiChannelButtons.clear();
    deleteAllChildren();
}



void GuiControllerMode::resized()
{
	
	controlButtons[0]->setBounds(684, 261, 322, 322);
	controlButtons[1]->setBounds(684, 261, 322, 322);
	controlButtons[2]->setBounds(684, 261, 322, 322);
	controlButtons[3]->setBounds(684, 261, 322, 322);
    
    //only one of these (or a group of these) will be displayed at any time depending on the value of control menu
    sceneNumberSlider->setBounds(816, 393, 58, 58);
	
    oscIpAddressEditor->setBounds(900, 430, 98, 20);
    oscPortNumberSlider-> setBounds(816, 393, 58, 58);
    
    midiProgramChangeNumberSlider->setBounds(816, 393, 58, 58);
	
	midiChannelButtons[0]->setBounds(649,439,21, 21);
	midiChannelButtons[1]->setBounds(656,467,21, 21);
	midiChannelButtons[2]->setBounds(667,495,21, 21);
	midiChannelButtons[3]->setBounds(682,520,21, 21);
	midiChannelButtons[4]->setBounds(700,542,21, 21);
	midiChannelButtons[5]->setBounds(722,562,21, 21);
	midiChannelButtons[6]->setBounds(747,577,21, 21);
	midiChannelButtons[7]->setBounds(774,589,21, 21);
	midiChannelButtons[8]->setBounds(803,596,21, 21);
	midiChannelButtons[9]->setBounds(832,599,21, 21);
	midiChannelButtons[10]->setBounds(861,597,21, 21);
	midiChannelButtons[11]->setBounds(890,590,21, 21);
	midiChannelButtons[12]->setBounds(917,579,21, 21);
	midiChannelButtons[13]->setBounds(942,564,21, 21);
	midiChannelButtons[14]->setBounds(965,545,21, 21);
	midiChannelButtons[15]->setBounds(984,523,21, 21);
}



void GuiControllerMode::paint (Graphics& g)
{
    
	ColourGradient fillGradient(AlphaColours::nearlyblack,845 , 461, Colours::black, 845 , 383, false);
	g.setGradientFill(fillGradient);
	
	g.fillEllipse(802, 379, 86, 86);
	
	if (drawButtons == 1) 
    {
		drawButtonsBackground(g);
	}
    
}



void GuiControllerMode::drawButtonsBackground(Graphics &g)
{
	//this draws an outline around the midi channel buttons
    
	g.setColour(Colours::black);
	
	g.fillEllipse(646,436, 27, 27);
	g.fillEllipse(653,464, 27, 27);
	g.fillEllipse(664,492, 27, 27);
	g.fillEllipse(679,517, 27, 27);
	g.fillEllipse(697,539, 27, 27);
	g.fillEllipse(719,559, 27, 27);
	g.fillEllipse(744,574, 27, 27);
	g.fillEllipse(771,586, 27, 27);
	
	g.fillEllipse(800,593, 27, 27);
	g.fillEllipse(829,596, 27, 27);
	g.fillEllipse(858,594, 27, 27);
	g.fillEllipse(887,587, 27, 27);
	g.fillEllipse(914,576, 27, 27);
	g.fillEllipse(939,561, 27, 27);
	g.fillEllipse(962,542, 27, 27);
	g.fillEllipse(981,520, 27, 27);
	
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	
	
	g.drawEllipse(646,436, 27, 27, 1.0);
	g.drawEllipse(653,464, 27, 27, 1.0);
	g.drawEllipse(664,492, 27, 27, 1.0);
	g.drawEllipse(679,517, 27, 27, 1.0);
	g.drawEllipse(697,539, 27, 27, 1.0);
	g.drawEllipse(719,559, 27, 27, 1.0);
	g.drawEllipse(744,574, 27, 27, 1.0);
	g.drawEllipse(771,586, 27, 27, 1.0);
    g.drawEllipse(800,593, 27, 27, 1.0);
	g.drawEllipse(829,596, 27, 27, 1.0);
	g.drawEllipse(858,594, 27, 27, 1.0);
	g.drawEllipse(887,587, 27, 27, 1.0);
	g.drawEllipse(914,576, 27, 27, 1.0);
	g.drawEllipse(939,561, 27, 27, 1.0);
	g.drawEllipse(962,542, 27, 27, 1.0);
	g.drawEllipse(981,520, 27, 27, 1.0);
	
}



void GuiControllerMode::setCurrentlySelectedPad (Array <int> selectedPads_)
{
    selectedPads = selectedPads_;
}




void GuiControllerMode::comboBoxChanged (ComboBox* comboBox)
{
    /*
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
*/
    
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
    
    
    /*
    //MIDI program change channel slider
    else if (slider == midiProgramChangeChannelSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setControllerMidiProgramChangeChannel(midiProgramChangeChannelSlider->getValue());
        }
        
    }
     */

    
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
    
    for (int control = 0; control < 4; control++)
    {
        if (button == controlButtons[control])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setControllerControl(control + 1);
            }
            
            //set what components are displayed
            setDisplay(control);            

            break;
        }
    }
    
    //channel buttons
    for (int chan = 0; chan < 16; chan++)
    {
        if (button == midiChannelButtons[chan])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setControllerMidiProgramChangeChannel(chan + 1);
            }
            
            break;
        }
        
    }
    
}

void GuiControllerMode::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/dissabled any unneeded components. 
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        controlButtons[PAD_SETTINGS->getControllerControl()]->setToggleState(true, false);
        sceneNumberSlider->setComponentValue(PAD_SETTINGS->getControllerPresentNumber());
        oscIpAddressEditor->setText(PAD_SETTINGS->getControllerOscIpAddress(), false);
        oscPortNumberSlider->setComponentValue(PAD_SETTINGS->getControllerOscPort());
        midiProgramChangeNumberSlider->setComponentValue(PAD_SETTINGS->getControllerMidiProgramChangeNumber());
        midiChannelButtons[PAD_SETTINGS->getControllerMidiProgramChangeChannel()-1]->setToggleState(true, false);
        
        setDisplay(PAD_SETTINGS->getControllerControl());
    }
    
    else if(MULTI_PADS)
    {
        controlButtons[0]->setToggleState(true, false);
        sceneNumberSlider->setComponentValue(1);
        oscIpAddressEditor->setText("127.0.0.1", false);
        oscPortNumberSlider->setComponentValue(5004);
        midiProgramChangeNumberSlider->setComponentValue(1);
        midiChannelButtons[0]->setToggleState(true, false);
        
        setDisplay(0);
    }
    
}




void GuiControllerMode::setDisplay (int controlSelected)
{
    //==============set what other components are visible===============
    
    //first set other componets invisble (saves having to set the visibility of each one in each next if statement)
    sceneNumberSlider->setVisible(false);
    oscIpAddressEditor->setVisible(false);
    oscPortNumberSlider->setVisible(false);
    midiProgramChangeNumberSlider->setVisible(false);
    for (int i = 0; i <= 15; i++)
        midiChannelButtons[i]->setVisible(false);
    
    //...and then set the right one visible
    if (controlSelected == 0) //scene switcher control selected
    {
        sceneNumberSlider->setVisible(true);
        drawButtons = 0;
        repaint(600, 200, 424, 469); // << SET REPAINT BOUNDS
    }
    else if(controlSelected == 1) //MIDI program change
    {
        midiProgramChangeNumberSlider->setBounds(816, 393, 58, 58);
        midiProgramChangeNumberSlider->setVisible(true);
        
        for (int i = 0; i <= 15; i++)
            midiChannelButtons[i]->setVisible(true);
        
        drawButtons = 1;
        repaint(600, 200, 424, 469); // << SET REPAINT BOUNDS
    }
    else if(controlSelected == 2) //Dual Preset/MIDI program change
    {
        midiProgramChangeNumberSlider->setBounds(816, 480, 58, 58);
        sceneNumberSlider->setVisible(true);
        midiProgramChangeNumberSlider->setVisible(true);
        
        for (int i = 0; i <= 15; i++)
            midiChannelButtons[i]->setVisible(true);
        
        drawButtons = 1;
        repaint(600, 200, 424, 469); // << SET REPAINT BOUNDS
        
    }
    else if(controlSelected == 3) //OSC output control selected
    {
        oscIpAddressEditor->setVisible(true);
        oscPortNumberSlider->setVisible(true);
        drawButtons = 0;
        repaint(600, 200, 424, 469); // << SET REPAINT BOUNDS
    }

    
}



void GuiControllerMode::mouseEnter (const MouseEvent &e)
{
    /*
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
    
    */
}

void GuiControllerMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty); 
}