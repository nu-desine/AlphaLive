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
#include "../Binary Data/BinaryDataNew.h"
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
	Image *triggerSettingsImage = new Image(ImageCache::getFromMemory(BinaryDataNew::triggersettingsicon_png, BinaryDataNew::triggersettingsicon_pngSize));
	addAndMakeVisible(triggerSettingsButton = new ModeButton(triggerSettingsImage));
	//triggerSettingsButton->setButtonText("TRIGGER");
	triggerSettingsButton->setRadioGroupId (1234);
	triggerSettingsButton->setClickingTogglesState(true);
	triggerSettingsButton->setToggleState(false, false);	
	triggerSettingsButton->addListener(this);
	triggerSettingsButton->addMouseListener(this, true);
	triggerSettingsButton->setOpaque(false);
    triggerSettingsButton->setToggleState(true, false);
	
	//----------------pressure settings button-------------------
	
	Image *pressureSettingsImage = new Image(ImageCache::getFromMemory(BinaryDataNew::pressuresettingsicon_png, BinaryDataNew::pressuresettingsicon_pngSize));
	addAndMakeVisible(pressureSettingsButton = new ModeButton(pressureSettingsImage));
	//pressureSettingsButton->setButtonText("PRESSURE");
	pressureSettingsButton->setRadioGroupId (1234);
	pressureSettingsButton->setClickingTogglesState(true);
	pressureSettingsButton->setToggleState(false, false);	
	pressureSettingsButton->addListener(this);
	pressureSettingsButton->addMouseListener(this, true);
	pressureSettingsButton->setOpaque(false);
    
    
    //----------------------Trigger mode buttons------------------
	
	Image *standardIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::standardicon_png, BinaryDataNew::standardicon_pngSize));
	Image *toggleIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::toggleicon_png, BinaryDataNew::toggleicon_pngSize));
	Image *latchIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::latchicon_png, BinaryDataNew::latchicon_pngSize));
	Image *triggerIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::triggericon_png, BinaryDataNew::triggericon_pngSize));
	
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
    
    triggerModeButtons[0]->setToggleState(true, false);
    
    
    //--------------pressure mode buttons--------------------------
	
	Image *patIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::polyphonicaftertouchicon_png, BinaryDataNew::polyphonicaftertouchicon_pngSize));
	Image *catIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::channelaftertouchicon_png, BinaryDataNew::channelaftertouchicon_pngSize));
	Image *mwheelIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::modwheelicon_png, BinaryDataNew::modwheelicon_pngSize));
	Image *ccIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::ccmessageicon_png, BinaryDataNew::ccmessageicon_pngSize));
	Image *pbUpIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::pitchbendupicon_png, BinaryDataNew::pitchbendupicon_pngSize));
	Image *pbDownIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::pitchbenddownicon_png, BinaryDataNew::pitchbenddownicon_pngSize));

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
    
    pressureModeButtons[0]->setToggleState(true, false);
    
     
    //---------------channel buttons---------------------
	for (int i = 0; i <= 15; i++)
	{
		channelButtons.insert(i, new AlphaTextButton());
		channelButtons[i]->setButtonText(String(i + 1));
		channelButtons[i]->setClickingTogglesState(true);
		channelButtons[i]->setToggleState(false, false);	
		channelButtons[i]->setRadioGroupId (12);
		channelButtons[i]->addListener(this);
        channelButtons[i]->addMouseListener(this, true);
		channelButtons[i]->setOpaque(false);
        addAndMakeVisible(channelButtons[i]);
		
	}
    
    channelButtons[0]->setToggleState(true, false);

    //----------------quantise button-------------------
	
	Image *quantiseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::quantiseicon_png, BinaryDataNew::quantiseicon_pngSize));
	addAndMakeVisible(quantiseButton = new ModeButton(quantiseIcon));
	quantiseButton->setClickingTogglesState(true);
	quantiseButton->setToggleState(false, false);	
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
    
	Image *destructIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::indestructableicon_png, BinaryDataNew::indestructableicon_pngSize));
	addChildComponent(indestructibleButton = new ModeButton(destructIcon));
    indestructibleButton->addListener(this);
    indestructibleButton->addMouseListener(this, true);
    indestructibleButton->setClickingTogglesState(true);
    indestructibleButton->setToggleState(false, false);
    
	Image *stickyIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::stickyicon_png, BinaryDataNew::stickyicon_pngSize));
    addChildComponent(stickyButton = new ModeButton(stickyIcon));
    stickyButton->addListener(this);
    stickyButton->addMouseListener(this, true);
    stickyButton->setClickingTogglesState(true);
    stickyButton->setToggleState(false, false);

    
    //---------------CC controller Slider-------------------------------------
    addChildComponent(ccControllerSlider = new AlphaSlider());
    ccControllerSlider->setRange(0, 127, 1);
    ccControllerSlider->addListener(this);
    ccControllerSlider->addMouseListener(this, true);
    ccControllerSlider->setValue(-999);
    

    //---------------status off bg-------------------------------------
    addChildComponent(notSelected = new GuiCircleBackground());
    
    
    //---------------pressure status button-------------------------------------
    
    addChildComponent(pressureStatusButton = new GuiSwitch());
    pressureStatusButton->addListener(this);
    pressureStatusButton->setClickingTogglesState(true);
    pressureStatusButton->setToggleState(true, false);
    pressureStatusButton->addMouseListener(this, false);
    
    //---------------parameter label -------------------------------------
    addChildComponent(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaColours::blue);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
    
    
    //---------------note status button-------------------------------------
    addAndMakeVisible(noteStatusButton = new GuiSwitch());
    noteStatusButton->addListener(this);
    noteStatusButton->setClickingTogglesState(true);
    noteStatusButton->setToggleState(true, false);
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

}

void GuiMidiMode::paint (Graphics& g)
{
    //std::cout << "Paint midi mode... ";
	ColourGradient fillGradient(AlphaColours::nearlyblack,845 , 461, Colours::black, 845 , 383, false);
	g.setGradientFill(fillGradient);
	
	g.fillEllipse(802, 379, 86, 86);
	
	g.setColour(Colours::black);
	
	g.fillEllipse(786,218, 48, 48);
	g.fillEllipse(844,216, 48, 48);
	
	g.fillEllipse(678,285, 38, 38);
	g.fillEllipse(850,493, 38, 38);
	
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
	
	g.drawEllipse(678,285, 38, 38, 1.0);
	g.drawEllipse(850,493, 38, 38, 1.0);
	
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
        
         parameterHoverLabel->setText(String(slider->getValue()), false);
        
    }
    //max pressure range slider
    if (slider == pressureMaxRangeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiMaxPressureRange(pressureMaxRangeSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue()), false);
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
        
        if(noteStatusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);
        
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
    
    
    //channel buttons
    for (int chan = 0; chan < 16; chan++)
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
        
        quantiseButton->setToggleState(PAD_SETTINGS->getQuantizeMode(), false);
        channelButtons[PAD_SETTINGS->getMidiChannel()-1]->setToggleState(true, false);
        pressureMinRangeSlider->setValue(PAD_SETTINGS->getMidiMinPressureRange(), dontSendNotification);
        pressureMaxRangeSlider->setValue(PAD_SETTINGS->getMidiMaxPressureRange(), dontSendNotification);
        pressureModeButtons[PAD_SETTINGS->getMidiPressureMode()-1]->setToggleState(true, false);
        triggerModeButtons[PAD_SETTINGS->getMidiTriggerMode()-1]->setToggleState(true, false);
        ccControllerSlider->setValue(PAD_SETTINGS->getMidiCcController());
        indestructibleButton->setToggleState(PAD_SETTINGS->getMidiIndestructible(), false);
        stickyButton->setToggleState(PAD_SETTINGS->getMidiSticky(), false);
        
        pressureStatusButton->setToggleState(PAD_SETTINGS->getMidiPressureStatus(), false);
        noteStatusButton->setToggleState(PAD_SETTINGS->getMidiNoteStatus(), false);
        
    }
    
    else if(MULTI_PADS)
    {
        /*
        quantiseButton->setToggleState(false, false);
        channelButtons[0]->setToggleState(true, false);
        pressureMinRangeSlider->setValue(0);
        pressureMaxRangeSlider->setValue(127);
        pressureModeButtons[0]->setToggleState(true, false); //ideally nothing should be selected here
        triggerModeButtons[0]->setToggleState(true, false); // '' ''
        indestructibleButton->setToggleState(0, false);
        stickyButton->setToggleState(0, false);
        ccControllerSlider->setValue(12);
        pressureStatusButton->setToggleState(true, false);
        noteStatusButton->setToggleState(true, false);
         */
        //==================================================================================================
        int quantiseMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getQuantizeMode();
        //loop through all selected pads expect for the first one
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            //if setting of this pad does NOT match setting of last pad, set default and break
            if (PAD_SETTINGS->getQuantizeMode() != quantiseMode_)
            {
                quantiseButton->setToggleState(0, false);
                break;
            }
            //if this is the last 'natural' iteraction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
                quantiseButton->setToggleState(quantiseMode_, false);
        }
        
        //==================================================================================================
        int channel_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiChannel();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiChannel() != channel_)
            {
                for (int i = 0; i <16; i++)
                    channelButtons[i]->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                channelButtons[channel_-1]->setToggleState(true, false);
        }
        
        //==================================================================================================
        int pressureMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiPressureMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiPressureMode() != pressureMode_)
            {
                for (int i = 0; i < 6; i++)
                    pressureModeButtons[i]->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                pressureModeButtons[pressureMode_-1]->setToggleState(true, false);
        }
        
        //==================================================================================================
        int triggerMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiTriggerMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiTriggerMode() != triggerMode_)
            {
                for (int i = 0; i < 4; i++)
                    triggerModeButtons[i]->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                triggerModeButtons[triggerMode_-1]->setToggleState(true, false);
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
                indestructibleButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                indestructibleButton->setToggleState(indestructible_, false);
        }
        
        //==================================================================================================
        int sticky_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiSticky();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiSticky() != sticky_)
            {
                stickyButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                stickyButton->setToggleState(sticky_, false);
        }
        
        //==================================================================================================
        int noteStatus_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiNoteStatus();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiNoteStatus() != noteStatus_)
            {
                noteStatusButton->setToggleState(1, false);
                break;
            }
            if (i == selectedPads.size()-1)
                noteStatusButton->setToggleState(noteStatus_, false);
        }
        
        //==================================================================================================
        int pressureStatus_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMidiPressureStatus();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getMidiPressureStatus() != pressureStatus_)
            {
                pressureStatusButton->setToggleState(1, false);
                break;
            }
            if (i == selectedPads.size()-1)
                pressureStatusButton->setToggleState(pressureStatus_, false);
        }
        
        
    }

    //set visibility of certain components
    
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
        
        
        if(noteStatusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);
        
        //should we be calling repaint here? Compare will Sampler Mode
        
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
        //parameterHoverLabel->setText(String::empty, false);
        
        
        if(pressureStatusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);

        //should we be calling repaint here? Compare will Sampler Mode
    }
    
}


void GuiMidiMode::mouseEnter (const MouseEvent &e)
{
    
    for (int i = 0; i < 16; i++)
    {
        if (channelButtons[i]->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiChannelButtons) + " " + String(i+1) + ".");
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
    
    
    //update parameterHoverLabel
    if (pressureMinRangeSlider->isMouseOver(true))
        parameterHoverLabel->setText(String(pressureMinRangeSlider->getValue()), false);
    else if (pressureMaxRangeSlider->isMouseOver(true))
        parameterHoverLabel->setText(String(pressureMaxRangeSlider->getValue()), false);
}

void GuiMidiMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
    
    if(e.eventComponent == pressureMinRangeSlider || e.eventComponent == pressureMaxRangeSlider)
        parameterHoverLabel->setText(String::empty, false);
}


