//
//  GuiMidiMode.cpp
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

#include "GuiMidiMode.h"
#include "../../File and Settings/AppSettings.h"
#include "../../Functionality Classes/Other/LayoutsAndScales.cpp"
#include "../Binary Data/MainBinaryData.h"
#include "GlobalValues.h"
#include "../../Application/CommonInfoBoxText.h"
#include "MainComponent.h"


#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



//==============================================================================
GuiMidiMode::GuiMidiMode(MainComponent &ref)
                                    : mainComponentRef(ref)
{
    
    //----------------trigger settings button-------------------
	Image *triggerSettingsImage = new Image(ImageCache::getFromMemory(MainBinaryData::triggersettingsicon_png, MainBinaryData::triggersettingsicon_pngSize));
	addAndMakeVisible(triggerSettingsButton = new ModeButton(triggerSettingsImage));
	//triggerSettingsButton->setButtonText("TRIGGER");
	triggerSettingsButton->setRadioGroupId (1234);
	triggerSettingsButton->setClickingTogglesState(true);
	triggerSettingsButton->setToggleState(true, dontSendNotification);
	triggerSettingsButton->addListener(this);
	triggerSettingsButton->addMouseListener(this, true);
	triggerSettingsButton->setOpaque(false);
	
	//----------------pressure settings button-------------------
	
	Image *pressureSettingsImage = new Image(ImageCache::getFromMemory(MainBinaryData::pressuresettingsicon_png, MainBinaryData::pressuresettingsicon_pngSize));
	addAndMakeVisible(pressureSettingsButton = new ModeButton(pressureSettingsImage));
	//pressureSettingsButton->setButtonText("PRESSURE");
	pressureSettingsButton->setRadioGroupId (1234);
	pressureSettingsButton->setClickingTogglesState(true);
	pressureSettingsButton->setToggleState(false, dontSendNotification);
	pressureSettingsButton->addListener(this);
	pressureSettingsButton->addMouseListener(this, true);
	pressureSettingsButton->setOpaque(false);
    
    
    //----------------------Trigger mode buttons------------------
	
	Image *standardIcon = new Image(ImageCache::getFromMemory(MainBinaryData::standardicon_png, MainBinaryData::standardicon_pngSize));
	Image *toggleIcon = new Image(ImageCache::getFromMemory(MainBinaryData::toggleicon_png, MainBinaryData::toggleicon_pngSize));
	Image *latchIcon = new Image(ImageCache::getFromMemory(MainBinaryData::latchicon_png, MainBinaryData::latchicon_pngSize));
	Image *triggerIcon = new Image(ImageCache::getFromMemory(MainBinaryData::triggericon_png, MainBinaryData::triggericon_pngSize));
	
    for (int i = 0; i < 4; i++)
    {
        if (i == 0)
        triggerModeButtons.insert(i, new SettingsButtonImage(standardIcon, (270 * (M_PI / 180)), 
                                                            (315 * (M_PI / 180)),
                                                            0.4f, 0.6f));
        else if (i == 1)
            triggerModeButtons.insert(i, new SettingsButtonImage(toggleIcon, (315 * (M_PI / 180)),
                                                            (2 * M_PI) , 0.4f, 0.6f));
        else if (i == 2)
            triggerModeButtons.insert(i, new SettingsButtonImage(latchIcon, 0.0f, (45 * (M_PI / 180)), 
                                                            0.4f, 0.6f));
        else if (i == 3)
            triggerModeButtons.insert(i, new SettingsButtonImage(triggerIcon, (45 * (M_PI / 180)), 
                                                            (90 * (M_PI / 180)), 0.4f, 0.6f));
        
        triggerModeButtons[i]->addListener(this);
        triggerModeButtons[i]->setOpaque(false);
        triggerModeButtons[i]->setRadioGroupId (43);
        triggerModeButtons[i]->addMouseListener(this, false);
        addAndMakeVisible(triggerModeButtons[i]);
    }
    
    triggerModeButtons[0]->setToggleState(true, dontSendNotification);
    
    
    //--------------pressure mode buttons--------------------------
	
	Image *patIcon = new Image(ImageCache::getFromMemory(MainBinaryData::polyphonicaftertouchicon_png, MainBinaryData::polyphonicaftertouchicon_pngSize));
	Image *catIcon = new Image(ImageCache::getFromMemory(MainBinaryData::channelaftertouchicon_png, MainBinaryData::channelaftertouchicon_pngSize));
	Image *mwheelIcon = new Image(ImageCache::getFromMemory(MainBinaryData::modwheelicon_png, MainBinaryData::modwheelicon_pngSize));
	Image *ccIcon = new Image(ImageCache::getFromMemory(MainBinaryData::ccmessageicon_png, MainBinaryData::ccmessageicon_pngSize));
	Image *pbUpIcon = new Image(ImageCache::getFromMemory(MainBinaryData::pitchbendupicon_png, MainBinaryData::pitchbendupicon_pngSize));
	Image *pbDownIcon = new Image(ImageCache::getFromMemory(MainBinaryData::pitchbenddownicon_png, MainBinaryData::pitchbenddownicon_pngSize));

	for (int i = 0; i < 6; i++)
    {
        if (i == 0)
            pressureModeButtons.insert(i, new SettingsButtonImage(patIcon, (270 * (M_PI / 180)), 
                                                             (315 * (M_PI / 180)), 0.4f, 0.6f));
        if (i == 1)
            pressureModeButtons.insert(i, new SettingsButtonImage(catIcon, (315 * (M_PI / 180)),
                                                             (2 * M_PI) , 0.4f, 0.6f));
        if (i == 2)
            pressureModeButtons.insert(i, new SettingsButtonImage(mwheelIcon, 0.0f, (45 * (M_PI / 180)), 
                                                             0.4f,0.6f));
        if (i == 3)
            pressureModeButtons.insert(i, new SettingsButtonImage(ccIcon, (45 * (M_PI / 180)), (90 * (M_PI / 180)), 
                                                             0.4f, 0.6f));
        if (i == 4)
            pressureModeButtons.insert(i, new SettingsButtonImage(pbUpIcon, (225 * (M_PI / 180)), 
                                                             (270 * (M_PI / 180)),0.4f,0.6f));
        if (i == 5)
            pressureModeButtons.insert(i, new SettingsButtonImage(pbDownIcon, (180 * (M_PI / 180)),
                                                             (225 * (M_PI/ 180)) , 0.4f, 0.6f));
        
        pressureModeButtons[i]->addListener(this);
        pressureModeButtons[i]->setOpaque(false);
        pressureModeButtons[i]->setRadioGroupId (53);
        pressureModeButtons[i]->addMouseListener(this, false);
        addChildComponent(pressureModeButtons[i]);
    }
    
    pressureModeButtons[0]->setToggleState(true, dontSendNotification);
    
     
    //---------------channel buttons---------------------
	for (int i = 0; i <= 15; i++)
	{
		channelButtons.insert(i, new AlphaTextButton());
		channelButtons[i]->setButtonText(String(i + 1));
		channelButtons[i]->setClickingTogglesState(true);
		channelButtons[i]->setToggleState(false, dontSendNotification);
		channelButtons[i]->setRadioGroupId (12);
		channelButtons[i]->addListener(this);
        channelButtons[i]->addMouseListener(this, true);
		channelButtons[i]->setOpaque(false);
        addAndMakeVisible(channelButtons[i]);
		
	}
    
    channelButtons[0]->setToggleState(true, dontSendNotification);
    
    addAndMakeVisible(dynamicChannelButton = new AlphaTextButton);
    dynamicChannelButton->setButtonText(translate("DYN"));
    dynamicChannelButton->setClickingTogglesState(true);
    dynamicChannelButton->addListener(this);
    dynamicChannelButton->addMouseListener(this, true);

    //----------------quantise button-------------------
	
	Image *quantiseIcon = new Image(ImageCache::getFromMemory(MainBinaryData::quantiseicon_png, MainBinaryData::quantiseicon_pngSize));
	addAndMakeVisible(quantiseButton = new ModeButton(quantiseIcon));
	quantiseButton->setClickingTogglesState(true);
	quantiseButton->setToggleState(false, dontSendNotification);
	quantiseButton->addListener(this);
	quantiseButton->addMouseListener(this, true);
	quantiseButton->setOpaque(false);
    
    //------------------Pressure minimum range slider------------------------------
    addChildComponent(pressureMinRangeSlider = new AlphaRotarySlider((90 * (M_PI / 180)), (315 * (M_PI / 180)), 290));
	pressureMinRangeSlider->setRotaryParameters((90 * (M_PI / 180)), (315 * (M_PI / 180)),true);
    pressureMinRangeSlider->setRange(0, 127, 1);
    pressureMinRangeSlider->addListener(this);
    pressureMinRangeSlider->setValue(0, dontSendNotification);
    pressureMinRangeSlider->addMouseListener(this, true);
    
    
    //------------------Pressure maximum range slider------------------------------
    addChildComponent(pressureMaxRangeSlider = new AlphaRotarySlider((90 * (M_PI / 180)), (315 * (M_PI / 180)), 270));
	pressureMaxRangeSlider->setRotaryParameters((90 * (M_PI / 180)), (315 * (M_PI / 180)),true);
    pressureMaxRangeSlider->setRange(0, 127, 1);
    pressureMaxRangeSlider->addListener(this);
    pressureMaxRangeSlider->setValue(127, dontSendNotification);
    pressureMaxRangeSlider->addMouseListener(this, true);
    
	Image *destructIcon = new Image(ImageCache::getFromMemory(MainBinaryData::indestructableicon_png, MainBinaryData::indestructableicon_pngSize));
	addAndMakeVisible(indestructibleButton = new ModeButton(destructIcon));
    indestructibleButton->addListener(this);
    indestructibleButton->addMouseListener(this, true);
    indestructibleButton->setClickingTogglesState(true);
    indestructibleButton->setToggleState(false, dontSendNotification);
    
	Image *stickyIcon = new Image(ImageCache::getFromMemory(MainBinaryData::stickyicon_png, MainBinaryData::stickyicon_pngSize));
    addChildComponent(stickyButton = new ModeButton(stickyIcon));
    stickyButton->addListener(this);
    stickyButton->addMouseListener(this, true);
    stickyButton->setClickingTogglesState(true);
    stickyButton->setToggleState(false, dontSendNotification);

    
    //---------------CC controller Slider-------------------------------------
    addChildComponent(ccControllerSlider = new AlphaSlider());
    ccControllerSlider->setRange(0, 127, 1);
    ccControllerSlider->addListener(this);
    ccControllerSlider->addMouseListener(this, true);
    ccControllerSlider->setValue(-999);
    
	//---------------reverse pressure range button-------------------------------------
    addChildComponent(reversePressureRangeButton = new AlphaTextButton());
    reversePressureRangeButton->setButtonText(translate("REV"));
    reversePressureRangeButton->addListener(this);
    reversePressureRangeButton->addMouseListener(this, true);

	//PLEASE NOTE: ANYTHING PLACED AFTER HERE WILL NOT BE HIDDEN BY THE STATUS/PRESSURE OFF GRAPHIC

    //---------------status off bg-------------------------------------
    addChildComponent(notSelected = new GuiCircleBackground());
    
    
    //---------------pressure status button-------------------------------------
    
    addChildComponent(pressureStatusButton = new GuiSwitch());
    pressureStatusButton->addListener(this);
    pressureStatusButton->setClickingTogglesState(true);
    pressureStatusButton->setToggleState(true, dontSendNotification);
    pressureStatusButton->addMouseListener(this, false);
    
    //---------------parameter label -------------------------------------
    addChildComponent(parameterHoverLabel = new Label("value label", String()));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setFont(Font(11 + AlphaTheme::getInstance()->fontSizeAddition));
    parameterHoverLabel->addMouseListener(this, true);
    
    
    //---------------note status button-------------------------------------
    addAndMakeVisible(noteStatusButton = new GuiSwitch());
    noteStatusButton->addListener(this);
    noteStatusButton->setClickingTogglesState(true);
    noteStatusButton->setToggleState(true, dontSendNotification);
    noteStatusButton->addMouseListener(this, true);
   
}

GuiMidiMode::~GuiMidiMode()
{
    channelButtons.clear();
    triggerModeButtons.clear();
    pressureModeButtons.clear();
    deleteAllChildren();
}

//==============================================================================
void GuiMidiMode::resized()
{
    notSelected->setBounds(0, 0, getWidth(), getHeight());
	
    noteStatusButton->setBounds(816, 393, 58, 58);
	pressureStatusButton->setBounds(816, 393, 58, 58);
	
    dynamicChannelButton->setBounds(647, 402, 26, 26);
    
	channelButtons[0]->setBounds(649,439,21, 21);
	channelButtons[1]->setBounds(656,467,21, 21);
	channelButtons[2]->setBounds(667,495,21, 21);
	channelButtons[3]->setBounds(682,520,21, 21);
	channelButtons[4]->setBounds(700,542,21, 21);
	channelButtons[5]->setBounds(722,562,21, 21);
	channelButtons[6]->setBounds(747,577,21, 21);
	channelButtons[7]->setBounds(774,589,21, 21);
	channelButtons[8]->setBounds(803,596,21, 21);
	channelButtons[9]->setBounds(832,599,21, 21);
	channelButtons[10]->setBounds(861,597,21, 21);
	channelButtons[11]->setBounds(890,590,21, 21);
	channelButtons[12]->setBounds(917,579,21, 21);
	channelButtons[13]->setBounds(942,564,21, 21);
	channelButtons[14]->setBounds(965,545,21, 21);
	channelButtons[15]->setBounds(984,523,21, 21);
	
	quantiseButton->setBounds(681, 288,32, 32);
	triggerSettingsButton->setBounds(789, 221,42, 42);
	pressureSettingsButton->setBounds(847, 219,42, 42);
	
    //can we give the below more specific bounds?
    //if not the below can be put into a for loop
	triggerModeButtons[0]->setBounds(728, 305, 234, 234);
	triggerModeButtons[1]->setBounds(728, 305, 234, 234);
	triggerModeButtons[2]->setBounds(728, 305, 234, 234);
	triggerModeButtons[3]->setBounds(728, 305, 234, 234);
	
	indestructibleButton->setBounds(853, 496,32, 32);
	
	stickyButton->setBounds(853, 496,32, 32);
    
    pressureMinRangeSlider->setBounds(700, 277, 290, 290);
    pressureMaxRangeSlider->setBounds(710, 287, 270, 270);
    parameterHoverLabel->setBounds(832, 453, 26, 10);
	
    //can we give the below more specific bounds?
    //if not the below can be put into a for loop
	pressureModeButtons[0]->setBounds(728, 305, 234, 234);
	pressureModeButtons[1]->setBounds(728, 305, 234, 234);
	pressureModeButtons[2]->setBounds(728, 305, 234, 234);
	pressureModeButtons[3]->setBounds(728, 305, 234, 234);
	pressureModeButtons[4]->setBounds(728, 305, 234, 234);
	pressureModeButtons[5]->setBounds(728, 305, 234, 234);
	
	ccControllerSlider->setBounds(890, 431, 58, 58);
    
    //reversePressureRangeButton->setBounds(753, 309, 21, 21); //at the end of the range sliders
    reversePressureRangeButton->setBounds(969, 395, 21, 21); //at the beginning of the range sliders

}

void GuiMidiMode::paint (Graphics& g)
{
    parameterHoverLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->mainColour);
    
    //=============================================================================
    //draw background circles for particular controls
    
	ColourGradient fillGradient(AlphaTheme::getInstance()->childBackgroundColour,845 , 461, AlphaTheme::getInstance()->backgroundColour, 845 , 383, false);

	g.setGradientFill(fillGradient);
	
	g.fillEllipse(802, 379, 86, 86);
	
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	
	g.fillEllipse(786,218, 48, 48);
	g.fillEllipse(844,216, 48, 48);
	
	g.fillEllipse(678,285, 38, 38);
	g.fillEllipse(850,493, 38, 38);
	
    //dynamic channel button
    g.fillEllipse(644,399, 32, 32);
    
    //channel buttons
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
    
    //reverse pressure range button
    if (pressureSettingsButton->getToggleState())
        g.fillEllipse(966, 392, 27, 27);
	
    //=============================================================================
    //draw outlines for the background circles for particular controls
    
	g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
	
	g.drawEllipse(678,285, 38, 38, 1.0);
	g.drawEllipse(850,493, 38, 38, 1.0);
	
    //dynamic channel button
    g.drawEllipse(644,399, 32, 32, 1.0);
    
    //channel buttons
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
    
    //reverse pressure range button
    if (pressureSettingsButton->getToggleState())
        g.drawEllipse(966, 392, 27, 27, 1.0);

}


void GuiMidiMode::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

//==============================================================================
void GuiMidiMode::comboBoxChanged (ComboBox* comboBox)
{
    //This method takes the selected IDs from the combo box and sets it to the correct
    //variable in the relevent padSettings index/es
    

}
    

void GuiMidiMode::sliderValueChanged (Slider* slider)
{

    //min pressure range slider
    if (slider == pressureMinRangeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiMinPressureRange(pressureMinRangeSlider->getValue());
        }
        
         parameterHoverLabel->setText(String(slider->getValue()), dontSendNotification);
        
    }
    //max pressure range slider
    if (slider == pressureMaxRangeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiMaxPressureRange(pressureMaxRangeSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue()), dontSendNotification);
    }
        
    //CC Controller Number
    if (slider == ccControllerSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiCcController(ccControllerSlider->getValue());
        }
    }

}



void GuiMidiMode::buttonClicked (Button* button)
{
    if(button == triggerSettingsButton)
		setDisplay(1);        
		
	
	else if(button == pressureSettingsButton)
        setDisplay(2);
    
    else if(button == quantiseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setQuantizeMode(button->getToggleState());
        }
    }
   
    else if(button == pressureStatusButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiPressureStatus(pressureStatusButton->getToggleState());
        }
        
        if(pressureStatusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);
    }
    
    
    else if(button == noteStatusButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiNoteStatus(noteStatusButton->getToggleState());
        }
        
        if (noteStatusButton->getToggleStateValue()==true)
        {
            notSelected->setVisible(false);
            mainComponentRef.getGuiPiano()->setActive(true);
            mainComponentRef.getGuiPiano()->updateDisplay();
        }
        else
        {
            notSelected->setVisible(true);
            
            mainComponentRef.getGuiPiano()->setActive(false);
        }
        
        //if note status is set to off, make sure non of the pads are set to auto
        //channel mode. dynamic channel mode can't work with pressure-only pads, as there is
        //no way of determining if a channel is 'active' or not if no note data is being used.
        if (button->getToggleState() == false)
        {
            dynamicChannelButton->setToggleState(false, dontSendNotification);
            dynamicChannelButton->setEnabled(false);
            dynamicChannelButton->setAlpha(0.4);
        }
        else
        {
            dynamicChannelButton->setEnabled(true);
            dynamicChannelButton->setAlpha(1.0);
        }
    }
    
    
    else if (button == indestructibleButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiIndestructible(button->getToggleState());
        }
    }
    
    else if (button == stickyButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiSticky(button->getToggleState());
        }
    }
    
    else if (button == dynamicChannelButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiDynamicChannelStatus(button->getToggleState());
        }
        
        //if dynamic channels mode is turned on, dissable the radio group functionality
        //of the channels buttons and get and display the group of channels that can be used
        if (button->getToggleState() == true)
        {
            for (int chan = 0; chan < 16; chan++)
            {
                channelButtons[chan]->setRadioGroupId(0, dontSendNotification);
                
                if (SINGLE_PAD)
                {
                    int padNum = selectedPads[0];
                    channelButtons[chan]->setToggleState(PAD_SETTINGS->getMidiDynamicChannels(chan), dontSendNotification);
                }
                else if (MULTI_PADS)
                {
                    int autoChans_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiDynamicChannels(chan);
                    for (int i = 1; i < selectedPads.size(); i++)
                    {
                        int padNum = selectedPads[i];
                        if (PAD_SETTINGS->getMidiDynamicChannels(chan) != autoChans_)
                        {
                            channelButtons[chan]->setToggleState(true, dontSendNotification);
                            break;
                        }
                        if (i == selectedPads.size()-1)
                            channelButtons[chan]->setToggleState(autoChans_, dontSendNotification);
                    }
                }
            }
        }
        //if dynamic channels mode is turned off, set the channel buttons to be a radio group
        //and get and display the static channel of this pad.
        else
        {
            for (int chan = 0; chan < 16; chan++)
            {
                channelButtons[chan]->setRadioGroupId(12, dontSendNotification);
                channelButtons[chan]->setToggleState(false, dontSendNotification);
            }
            
            if (SINGLE_PAD)
            {
                int padNum = selectedPads[0];
                channelButtons[PAD_SETTINGS->getMidiChannel()-1]->setToggleState(true, dontSendNotification);
            }
            else if (MULTI_PADS)
            {
                int channel_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiChannel();
                for (int i = 1; i < selectedPads.size(); i++)
                {
                    int padNum = selectedPads[i];
                    if (PAD_SETTINGS->getMidiChannel() != channel_)
                    {
                        for (int i = 0; i <16; i++)
                            channelButtons[i]->setToggleState(0, dontSendNotification);
                        break;
                    }
                    if (i == selectedPads.size()-1)
                        channelButtons[channel_-1]->setToggleState(true, dontSendNotification);
                }
            }
        }
    }
    
    //reverse pressure range button
    else if(button == reversePressureRangeButton)
    {
        //reverse/swap the min and max pressure values
        //within PadSettings for each selected pad
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            int minPressure = PAD_SETTINGS->getMidiMinPressureRange();
            int maxPressure = PAD_SETTINGS->getMidiMaxPressureRange();
            
            PAD_SETTINGS->setMidiMinPressureRange(maxPressure);
            PAD_SETTINGS->setMidiMaxPressureRange(minPressure);
        }
        
        //reverse/swap the min and max pressure values
        //on the current GUI. This needs to be done
        //seperatly from that of above incase the sliders
        //are currently showing default values due to selecting
        //multiple pads containing different values. Also,
        //we only want to update the GUI once, unlike above.
        {
            int minPressure = pressureMinRangeSlider->getValue();
            int maxPressure = pressureMaxRangeSlider->getValue();
            
            pressureMinRangeSlider->setValue(maxPressure, dontSendNotification);
            pressureMaxRangeSlider->setValue(minPressure, dontSendNotification);
        }
        
    }
    
    
    //channel buttons
    for (int chan = 0; chan < 16; chan++)
    {
        //Selecting a static channel
        if (dynamicChannelButton->getToggleState() == false)
        {
            if (button == channelButtons[chan])
            {
                for (int i = 0; i < selectedPads.size(); i++)
                {
                    int padNum = selectedPads[i];
                    PAD_SETTINGS->setMidiChannel(chan + 1);
                }
                
                break;
            }
        }
        //selecting the group of channels for dynamic-channel mode
        else if (dynamicChannelButton->getToggleState() == true)
        {
            if (button == channelButtons[chan])
            {
                //don't allow the user to deselect all MIDI channel buttons
                bool noChannelSelected = true;
                for (int i = 0; i < 16; i++)
                {
                    if (channelButtons[i]->getToggleState() == true)
                    {
                        noChannelSelected = false;
                        break;
                    }
                }
                    
                if (noChannelSelected == false)
                {
                    for (int i = 0; i < selectedPads.size(); i++)
                    {
                        int padNum = selectedPads[i];
                        PAD_SETTINGS->setMidiDynamicChannels(chan, button->getToggleState());
                    }
                }
                else
                {
                    button->setToggleState(true, dontSendNotification);
                }
                
                break;
            }
        }
        
    }
    
    //triggerMode buttons
    for (int trig = 0; trig < 4; trig++)
    {
        if (button == triggerModeButtons[trig])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setMidiTriggerMode(trig+1);
            }
            
            break;
        }
    }
    
    //pressureMode buttons
    for (int pres = 0; pres < 6; pres++)
    {
        if (button == pressureModeButtons[pres])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setMidiPressureMode(pres+1);
            }
            
            if (pres == 3)
                ccControllerSlider->setVisible(true);
            else
                ccControllerSlider->setVisible(false);
            
            break;
        }
    }
}

void GuiMidiMode::updateDisplay()
{
    
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/dissabled any unneeded components. 
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        //get the settings of the pad selected (in padMenu ComboBox)
        //set the stored values on the GUI components
        //Don't broadcast any changes to the component Listeners. Only want to update the GUI here
        
        quantiseButton->setToggleState(PAD_SETTINGS->getQuantizeMode(), dontSendNotification);
        pressureMinRangeSlider->setValue(PAD_SETTINGS->getMidiMinPressureRange(), dontSendNotification);
        pressureMaxRangeSlider->setValue(PAD_SETTINGS->getMidiMaxPressureRange(), dontSendNotification);
        pressureModeButtons[PAD_SETTINGS->getMidiPressureMode()-1]->setToggleState(true, dontSendNotification);
        triggerModeButtons[PAD_SETTINGS->getMidiTriggerMode()-1]->setToggleState(true, dontSendNotification);
        ccControllerSlider->setValue(PAD_SETTINGS->getMidiCcController(), dontSendNotification);
        indestructibleButton->setToggleState(PAD_SETTINGS->getMidiIndestructible(), dontSendNotification);
        stickyButton->setToggleState(PAD_SETTINGS->getMidiSticky(), dontSendNotification);
        
        pressureStatusButton->setToggleState(PAD_SETTINGS->getMidiPressureStatus(), dontSendNotification);
        noteStatusButton->setToggleState(PAD_SETTINGS->getMidiNoteStatus(), dontSendNotification);
        
        dynamicChannelButton->setToggleState(PAD_SETTINGS->getMidiDynamicChannelStatus(), dontSendNotification);
        
        if (dynamicChannelButton->getToggleState() == false) //static channel
        {
            for (int chan = 0; chan < 16; chan++)
                channelButtons[chan]->setRadioGroupId(12, dontSendNotification);
            
            channelButtons[PAD_SETTINGS->getMidiChannel()-1]->setToggleState(true, dontSendNotification);
        }
        else
        {
            for (int chan = 0; chan < 16; chan++)
            {
                channelButtons[chan]->setRadioGroupId(0, dontSendNotification);
                channelButtons[chan]->setToggleState(PAD_SETTINGS->getMidiDynamicChannels(chan), dontSendNotification);
            }
        }
        
    }
    
    else if(MULTI_PADS)
    {
        //==================================================================================================
        int quantiseMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getQuantizeMode();
        //loop through all selected pads expect for the first one
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            //if setting of this pad does NOT match setting of last pad, set default and break
            if (PAD_SETTINGS->getQuantizeMode() != quantiseMode_)
            {
                quantiseButton->setToggleState(0, dontSendNotification);
                break;
            }
            //if this is the last 'natural' iteraction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
                quantiseButton->setToggleState(quantiseMode_, dontSendNotification);
        }
        
        //==================================================================================================
        int pressureMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiPressureMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiPressureMode() != pressureMode_)
            {
                for (int i = 0; i < 6; i++)
                    pressureModeButtons[i]->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                pressureModeButtons[pressureMode_-1]->setToggleState(true, dontSendNotification);
        }
        
        //==================================================================================================
        int triggerMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiTriggerMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiTriggerMode() != triggerMode_)
            {
                for (int i = 0; i < 4; i++)
                    triggerModeButtons[i]->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                triggerModeButtons[triggerMode_-1]->setToggleState(true, dontSendNotification);
        }
        
        //==================================================================================================
        int pressureMinRange_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiMinPressureRange();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiMinPressureRange() != pressureMinRange_)
            {
                pressureMinRangeSlider->setValue(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
               pressureMinRangeSlider->setValue(pressureMinRange_, dontSendNotification);
        }
        
        //==================================================================================================
        int pressureMaxRange_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiMaxPressureRange();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiMaxPressureRange() != pressureMaxRange_)
            {
                pressureMaxRangeSlider->setValue(127, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                pressureMaxRangeSlider->setValue(pressureMaxRange_, dontSendNotification);
        }
        
        //==================================================================================================
        int ccNumber_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiCcController();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiCcController() != ccNumber_)
            {
                ccControllerSlider->setValue(-999);
                break;
            }
            if (i == selectedPads.size()-1)
                ccControllerSlider->setValue(ccNumber_);
        }
        
        //==================================================================================================
        int indestructible_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiIndestructible();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiIndestructible() != indestructible_)
            {
                indestructibleButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                indestructibleButton->setToggleState(indestructible_, dontSendNotification);
        }
        
        //==================================================================================================
        int sticky_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiSticky();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiSticky() != sticky_)
            {
                stickyButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                stickyButton->setToggleState(sticky_, dontSendNotification);
        }
        
        //==================================================================================================
        int noteStatus_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiNoteStatus();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiNoteStatus() != noteStatus_)
            {
                noteStatusButton->setToggleState(1, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                noteStatusButton->setToggleState(noteStatus_, dontSendNotification);
        }
        
        //==================================================================================================
        int pressureStatus_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiPressureStatus();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiPressureStatus() != pressureStatus_)
            {
                pressureStatusButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                pressureStatusButton->setToggleState(pressureStatus_, dontSendNotification);
        }
        
        //==================================================================================================
        int dynamicChannelStatus_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiDynamicChannelStatus();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiDynamicChannelStatus() != dynamicChannelStatus_)
            {
                dynamicChannelButton->setToggleState(false, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                dynamicChannelButton->setToggleState(dynamicChannelStatus_, dontSendNotification);
        }
        
        
        //==================================================================================================
        
        //static MIDI channel
        if (dynamicChannelButton->getToggleState() == false)
        {
            for (int i = 0; i < 16; i++)
                channelButtons[i]->setRadioGroupId(12, dontSendNotification);
            
            int channel_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiChannel();
            for (int i = 1; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                if (PAD_SETTINGS->getMidiChannel() != channel_)
                {
                    for (int i = 0; i <16; i++)
                        channelButtons[i]->setToggleState(0, dontSendNotification);
                    break;
                }
                if (i == selectedPads.size()-1)
                    channelButtons[channel_-1]->setToggleState(true, dontSendNotification);
            }
        }
        
        //dynamic-channel mode channels
        else
        {
            for (int chan = 0; chan < 16; chan++)
            {
                channelButtons[chan]->setRadioGroupId(0, dontSendNotification);
                
                int autoChans_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiDynamicChannels(chan);
                for (int i = 1; i < selectedPads.size(); i++)
                {
                    int padNum = selectedPads[i];
                    if (PAD_SETTINGS->getMidiDynamicChannels(chan) != autoChans_)
                    {
                        channelButtons[chan]->setToggleState(true, dontSendNotification);
                        break;
                    }
                    if (i == selectedPads.size()-1)
                        channelButtons[chan]->setToggleState(autoChans_, dontSendNotification);
                }
            }
        }
        
    }

    //set visibility of certain components
    
    if (noteStatusButton->getToggleState() == true)
    {
        dynamicChannelButton->setEnabled(true);
        dynamicChannelButton->setAlpha(1.0);
    }
    else
    {
        dynamicChannelButton->setEnabled(false);
        dynamicChannelButton->setAlpha(0.4);
    }
    
    notSelected->setVisible(false);
    
    if (triggerSettingsButton->getToggleState() == true)
        setDisplay(1);
    else if (pressureSettingsButton->getToggleState() == true)
        setDisplay(2);

}

void GuiMidiMode::setDisplay(int settingsType)
{
    if (settingsType == 1) //trigger settings
    {
        
        noteStatusButton->setVisible(true);
        for (int i = 0; i < 4; i++)
            triggerModeButtons[i]->setVisible(true);
        
        indestructibleButton->setVisible(true);
        
        
        pressureStatusButton->setVisible(false);
        for (int i = 0; i < 6; i++)
            pressureModeButtons[i]->setVisible(false);
        
        stickyButton->setVisible(false);
        pressureMinRangeSlider->setVisible(false);
        pressureMaxRangeSlider->setVisible(false);
        ccControllerSlider->setVisible(false);
        parameterHoverLabel->setVisible(false);
        
        reversePressureRangeButton->setVisible(false);
        
        
        if(noteStatusButton->getToggleState()==true)
        {
            notSelected->setVisible(false);
        }
        else
        {
            notSelected->setVisible(true);
        }
        
        repaint(750, 200, 274, 469);
        
    }
    
    else if (settingsType == 2) //pressure settings
    {
        
        noteStatusButton->setVisible(false);
        for (int i = 0; i < 4; i++)
            triggerModeButtons[i]->setVisible(false);
        
        indestructibleButton->setVisible(false);
        
        
        pressureStatusButton->setVisible(true);
        for (int i = 0; i < 6; i++)
            pressureModeButtons[i]->setVisible(true);
        
        if(pressureModeButtons[3]->getToggleStateValue() == true)
            ccControllerSlider->setVisible(true);
        else
            ccControllerSlider->setVisible(false);
        
        stickyButton->setVisible(true);
        pressureMinRangeSlider->setVisible(true);
        pressureMaxRangeSlider->setVisible(true);
        parameterHoverLabel->setVisible(true);
        //parameterHoverLabel->setText(String(), false);
        
        reversePressureRangeButton->setVisible(true);
        
        if(pressureStatusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);
        
        

        repaint(750, 200, 274, 469);
    }
    
    //Set piano display
    if(noteStatusButton->getToggleState()==true)
    {
        mainComponentRef.getGuiPiano()->setActive(true);
        mainComponentRef.getGuiPiano()->updateDisplay();
    }
    else
    {
        mainComponentRef.getGuiPiano()->setActive(false);
    }
    
}

void GuiMidiMode::changeView (int view)
{
    // view - 1 - trigger
    // view - 2 - pressure
    // view - 0 - switch to other view
    
    switch (view)
    {
        case 1:
            triggerSettingsButton->triggerClick();
            break;
        case 2:
            pressureSettingsButton->triggerClick();
            break;
        default:
            if (triggerSettingsButton->getToggleState())
                pressureSettingsButton->triggerClick();
            else if (pressureSettingsButton->getToggleState())
                triggerSettingsButton->triggerClick();
            break;
    }
}


void GuiMidiMode::mouseEnter (const MouseEvent &e)
{
    
    for (int i = 0; i < 16; i++)
    {
        if (channelButtons[i]->isMouseOver(true))
        {
            if (dynamicChannelButton->getToggleState() == false) //static MIDI channel
            {
                mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiChannelButtons) + " " + String(i+1) + ".");
            }
            else //Dynamic MIDI channels
            {
                mainComponentRef.setInfoTextBoxText(translate("MIDI Channel Buttons. Sets and displays the group of possible MIDI channels that the selected pads could be applied to. Click an inactive button to add the channel to the group, or click an active button to remove the channel from the group. However these buttons are used to select the pads static MIDI channel when the Dynamic MIDI Channel Mode is turned off."));
            }
            
            break;
        }
    }
    
    if (quantiseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::quantizeButton));
    }
    else if (triggerSettingsButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerSettingsButton));
    }
    else if (pressureSettingsButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureSettingsButton));
    }
    
    else if (triggerModeButtons[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerModeStandard) + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    else if (triggerModeButtons[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerModeToggle) + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    else if (triggerModeButtons[2]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerModeLatch) + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    else if (triggerModeButtons[3]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerModeTrigger) + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    
    
    else if (pressureModeButtons[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Polyphonic Aftertouch.") + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (pressureModeButtons[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureChannelAT) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (pressureModeButtons[2]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureModWheel) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (pressureModeButtons[3]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureCC) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (pressureModeButtons[4]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressurePitchUp) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (pressureModeButtons[5]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressurePitchDown) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    if (noteStatusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Note Status Switch. If this switch is set to 'off' the selected pads will not trigger any MIDI note-on or note-off messages."));
    }
    else if (pressureStatusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureStatusButton));
    }
    else if (pressureMinRangeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiMinPressureRangeSlider));
    }
    else if (pressureMaxRangeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiMaxPressureRangeSlider));
    }
    else if (ccControllerSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiCcSlider));
    }
    else if (indestructibleButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::indestructibleButton));
    }
    else if (stickyButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::stickyButton));
    }
    else if (dynamicChannelButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Dynamic MIDI Channel Mode button. Dynamic MIDI Channel Mode is a feature that allows individual channels to be dynamically applied to each pressed MIDI pad. Channels are applied to pads in the order they are pressed, and when this mode is turned on you can use the 16 MIDI channel buttons to select the possible channels that the selected pads could be applied to. This feature can be used as an alternative to polyphonic aftertouch when it is not available. Turn on this button to activate this mode. This feature is not available to pressure-only pads."));
    }
    else if (reversePressureRangeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiReversePressureRangeButton));
    }
    
    
    //update parameterHoverLabel
    if (pressureMinRangeSlider->isMouseOver(true))
        parameterHoverLabel->setText(String(pressureMinRangeSlider->getValue()), dontSendNotification);
    else if (pressureMaxRangeSlider->isMouseOver(true))
        parameterHoverLabel->setText(String(pressureMaxRangeSlider->getValue()), dontSendNotification);
}

void GuiMidiMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String());
    
    
    if(e.eventComponent == pressureMinRangeSlider || e.eventComponent == pressureMaxRangeSlider)
        parameterHoverLabel->setText(String(), dontSendNotification);
}


