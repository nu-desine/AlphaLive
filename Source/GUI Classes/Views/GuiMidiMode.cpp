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
#include "../Binary Data/ImageSliderBinaryData.h"
#include "GlobalValues.h"
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
	
	addAndMakeVisible(triggerSettingsButton = new AlphaTextButton("TRIGGER"));
	//triggerSettingsButton->setButtonText("TRIGGER");
	triggerSettingsButton->setRadioGroupId (1234);
	triggerSettingsButton->setClickingTogglesState(true);
	triggerSettingsButton->setToggleState(false, false);	
	triggerSettingsButton->addListener(this);
	triggerSettingsButton->addMouseListener(this, true);
	triggerSettingsButton->setOpaque(false);
    triggerSettingsButton->setToggleState(true, false);
	
	//----------------pressure settings button-------------------
	
	addAndMakeVisible(pressureSettingsButton = new AlphaTextButton("PRESSURE"));
	//pressureSettingsButton->setButtonText("PRESSURE");
	pressureSettingsButton->setRadioGroupId (1234);
	pressureSettingsButton->setClickingTogglesState(true);
	pressureSettingsButton->setToggleState(false, false);	
	pressureSettingsButton->addListener(this);
	pressureSettingsButton->addMouseListener(this, true);
	pressureSettingsButton->setOpaque(false);
    
    
    //----------------------Trigger mode buttons------------------
    for (int i = 0; i < 4; i++)
    {
        if (i == 0)
        triggerModeButtons.insert(i, new SettingsButton("STANDARD", (270 * (M_PI / 180)), 
                                                            (315 * (M_PI / 180)),
                                                            0.4f, -90, 0.45, 0.75));
        else if (i == 1)
            triggerModeButtons.insert(i, new SettingsButton("TOGGLE", (315 * (M_PI / 180)),
                                                            (2 * M_PI) , 0.4f, -90, 0.45, 0.68));
        else if (i == 2)
            triggerModeButtons.insert(i, new SettingsButton("LATCH", 0.0f, (45 * (M_PI / 180)), 
                                                            0.4f, 90, 0.55, 0.3));
        else if (i == 3)
            triggerModeButtons.insert(i, new SettingsButton("TRIGGER", (45 * (M_PI / 180)), 
                                                            (90 * (M_PI / 180)), 0.4f, 90, 0.55, 0.25));
        
        triggerModeButtons[i]->addListener(this);
        triggerModeButtons[i]->setOpaque(false);
        triggerModeButtons[i]->setRadioGroupId (43);
        triggerModeButtons[i]->addMouseListener(this, false);
        addAndMakeVisible(triggerModeButtons[i]);
    }
    
    triggerModeButtons[0]->setToggleState(true, false);
    
    
    //--------------pressure mode buttons--------------------------
	for (int i = 0; i < 6; i++)
    {
        if (i == 0)
            pressureModeButtons.insert(i, new SettingsButton("PAT", (270 * (M_PI / 180)), 
                                                             (315 * (M_PI / 180)), 0.4f, -90, 0.45, 0.7));
        if (i == 1)
            pressureModeButtons.insert(i, new SettingsButton("CAT", (315 * (M_PI / 180)),
                                                             (2 * M_PI) , 0.4f, -90, 0.45, 0.7));
        if (i == 2)
            pressureModeButtons.insert(i, new SettingsButton("M.Wh", 0.0f, (45 * (M_PI / 180)), 
                                                             0.4f, 90, 0.55, 0.5));
        if (i == 3)
            pressureModeButtons.insert(i, new SettingsButton("CC", (45 * (M_PI / 180)), (90 * (M_PI / 180)), 
                                                             0.4f, 90, 0.55, 0.5));
        if (i == 4)
            pressureModeButtons.insert(i, new SettingsButton("PB UP", (225 * (M_PI / 180)), 
                                                             (270 * (M_PI / 180)),0.4f, -90, 0.45, 0.7));
        if (i == 5)
            pressureModeButtons.insert(i, new SettingsButton("PB DOWN", (180 * (M_PI / 180)),
                                                             (225 * (M_PI/ 180)) , 0.4f, -90, 0.45, 0.7));
        
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
	
	addAndMakeVisible(quantiseButton = new AlphaTextButton("Q"));
	//quantiseButton->setButtonText("Q");
	quantiseButton->setClickingTogglesState(true);
	quantiseButton->setToggleState(false, false);	
	quantiseButton->addListener(this);
	quantiseButton->addMouseListener(this, true);
	quantiseButton->setOpaque(false);
    
    //------------------Pressure minimum range slider------------------------------
    addChildComponent(pressureMinRangeSlider = new AlphaSlider());
    pressureMinRangeSlider->setRange(0, 127, 1);
    pressureMinRangeSlider->addListener(this);
    pressureMinRangeSlider->setComponentValue(0);
    pressureMinRangeSlider->addMouseListener(this, true);
    
    
    //------------------Pressure maximum range slider------------------------------
    addChildComponent(pressureMaxRangeSlider = new AlphaSlider());
    pressureMaxRangeSlider->setRange(0, 127, 1);
    pressureMaxRangeSlider->addListener(this);
    pressureMaxRangeSlider->setComponentValue(127);
    pressureMaxRangeSlider->addMouseListener(this, true);
    
    addAndMakeVisible(indestructibleButton = new AlphaTextButton("indestruct"));
    indestructibleButton->addListener(this);
    indestructibleButton->addMouseListener(this, true);
    indestructibleButton->setClickingTogglesState(true);
    indestructibleButton->setToggleState(false, false);
    
    addChildComponent(stickyButton = new AlphaTextButton("Sticky"));
    stickyButton->addListener(this);
    stickyButton->addMouseListener(this, true);
    stickyButton->setClickingTogglesState(true);
    stickyButton->setToggleState(false, false);

    
    //---------------CC controller Slider-------------------------------------
    addChildComponent(ccControllerSlider = new AlphaSlider());
    ccControllerSlider->setRange(0, 127, 1);
    ccControllerSlider->addListener(this);
    ccControllerSlider->addMouseListener(this, true);
    

    //---------------status off bg-------------------------------------
    addChildComponent(notSelected = new GuiCircleBackground());
    
    
    //---------------pressure status button-------------------------------------
    
    addChildComponent(pressureStatusButton = new GuiSwitch());
    pressureStatusButton->addListener(this);
    pressureStatusButton->setClickingTogglesState(true);
    pressureStatusButton->setToggleState(true, false);
    pressureStatusButton->addMouseListener(this, false);
    
    
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
    
    pressureMinRangeSlider->setBounds(800, 540, 42, 42);
    pressureMaxRangeSlider->setBounds(848, 540, 42, 42);
	
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
    /*
    //velocity slider
    if (slider == velocitySlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiVelocity(velocitySlider->sliderComponent()->getValue());
        }
    }
     */

    //min pressure range slider
    if (slider == pressureMinRangeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiMinPressureRange(pressureMinRangeSlider->getValue());
        }
        
    }
    
    
    //max pressure range slider
    if (slider == pressureMaxRangeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiMaxPressureRange(pressureMaxRangeSlider->getValue());
        }
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
                PAD_SETTINGS->setMidiTriggerMode(trig);
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
                PAD_SETTINGS->setMidiPressureMode(pres);
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
        
        //velocitySlider->sliderComponent()->setValue(PAD_SETTINGS->getMidiVelocity(), true);
        channelButtons[PAD_SETTINGS->getMidiChannel()-1]->setToggleState(true, false);
        pressureMinRangeSlider->setComponentValue(PAD_SETTINGS->getMidiMinPressureRange());
        pressureMaxRangeSlider->setComponentValue(PAD_SETTINGS->getMidiMaxPressureRange());
        pressureModeButtons[PAD_SETTINGS->getMidiPressureMode()]->setToggleState(true, false);
        triggerModeButtons[PAD_SETTINGS->getMidiTriggerMode()]->setToggleState(true, false);
        ccControllerSlider->setComponentValue(PAD_SETTINGS->getMidiCcController());
        indestructibleButton->setToggleState(PAD_SETTINGS->getMidiIndestructible(), false);
        stickyButton->setToggleState(PAD_SETTINGS->getMidiSticky(), false);
        
        pressureStatusButton->setToggleState(PAD_SETTINGS->getMidiPressureStatus(), false);
        noteStatusButton->setToggleState(PAD_SETTINGS->getMidiNoteStatus(), false);
        
    }
    
    else if(MULTI_PADS)
    {
        //set default values
        //velocitySlider->sliderComponent()->setValue(110, true);
        channelButtons[0]->setToggleState(true, false);
        pressureMinRangeSlider->setComponentValue(0);
        pressureMaxRangeSlider->setComponentValue(127);
        pressureModeButtons[0]->setToggleState(true, false); //ideally nothing should be selected here
        triggerModeButtons[0]->setToggleState(true, false); // '' ''
        indestructibleButton->setToggleState(0, false);
        stickyButton->setToggleState(0, false);
        ccControllerSlider->setComponentValue(12);
        pressureStatusButton->setToggleState(true, false);
        noteStatusButton->setToggleState(true, false);
        
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
        
        
        if(pressureStatusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);

        //should we be calling repaint here? Compare will Sampler Mode
    }
    
}


void GuiMidiMode::mouseEnter (const MouseEvent &e)
{
    /*
    if (e.eventComponent == channelSlider)
    {
        mainComponentRef.setInfoTextBoxText("MIDI Channel Dial. Sets and displays the selected pads MIDI channel.");
    }
    else if (velocitySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("MIDI Velocity Knob. Sets and displays the MIDI velocity for the selected pad/pads.");
    }
    else if (circlePianoOneOctave->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Layout Selector. Sets the MIDI note layout that covers the entire set of pads. Use the middle drop-down menu to set the layout, the piano at the bottom to set the key, and the top set of buttons to set the octave.");
    }
    else if (circlePianoForScales->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Scale Selector. Sets the MIDI scale for the selected row of pads. Use the middle drop-down menu to set the scale and the piano at the bottom to set the root note. The top set of buttons can be used to transpose the piano range.");
    }
    else if (circlePiano->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("MIDI note Keyboard. Sets and displays the MIDI note for the selected pad. Use the top set of buttons to switch between octaves.");
    }
    else if (noteStatusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Note-Status Switch. If this switch is 'off' the selected pad/pads will not trigger any MIDI note-on or note-off messages.");
    }
    else if (pressureStatusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Pressure-Status Switch. If this switch is 'off' the selected pad/pads will not trigger any MIDI pressure-related data.");
    }
    else if (pressureMinRangeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Pressure Minimum Range Selector. Along with the Maximum Range Selector below, it sets and displays the MIDI pressure-related data range for the selected pad/pads.");
    }
    else if (pressureMaxRangeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Pressure Maximum Range Selector. Along with the Minimum Range Selector above, it sets and displays the MIDI pressure-related data range for the selected pad/pads.");
    }
    else if (pressureModeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Pressure Mode Drop-down Menu. Sets and displays what type of MIDI data will be created from the selected pad/pads pressure value.");
    }
    else if (triggerModeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Pressure TriggerMode Drop-down Menu. Sets and displays the pressure TriggerMode for the selected pad/pads. Pressure TriggerModes determine how interation with a pad controls the creation of MIDI data.");
    }
    else if (ccControllerSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("MIDI CC Controller Selector. Sets and displays the MIDI CC controller number for the selected pad/pads.");
    }
    */
    
}

void GuiMidiMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}


