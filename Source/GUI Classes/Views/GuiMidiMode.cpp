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
    
    //currentlySelectedPad = 99;
    
    //general graphics stuff
    addAndMakeVisible(circleBackgroundLeft = new GuiCircleBackground());
    addAndMakeVisible(circleBackgroundRight = new GuiCircleBackground());

    //--------scales circular piano (for selecting root note and scale)--------
    addAndMakeVisible(circlePianoForScales = new GuiCirclePianoForScales());
    circlePianoForScales->setInterceptsMouseClicks(false, true);
    circlePianoForScales->addMouseListener(this, true);
    
    //--------layout circular piano (for selecting key, octave, and layout)--------
    addAndMakeVisible(circlePianoOneOctave = new GuiCirclePianoOneOctave());
    circlePianoOneOctave->setInterceptsMouseClicks(false, true);
    circlePianoOneOctave->addMouseListener(this, true);
    
    //---------------note number circular piano-----------------------------
    addAndMakeVisible(circlePiano = new GuiCirclePiano());
	circlePiano->setInterceptsMouseClicks(false, true);
    circlePiano->addMouseListener(this, true);
    
    
    //------------------Velocity slider------------------------------
    addAndMakeVisible(velocitySlider = new AlphaImageKnob(0));
    velocitySlider->sliderComponent()->setRange(0, 127, 1);
    velocitySlider->sliderComponent()->addListener(this);
    velocitySlider->sliderComponent()->setValue(110, true);
    velocitySlider->sliderComponent()->addMouseListener(this, true);
    
    
    //------------------Channel slider------------------------------
    //create channel dial
	Image *dialImage = new Image(ImageFileFormat::loadFrom(ImageSliderBinaryData::channeldial_png, ImageSliderBinaryData::channeldial_pngSize));
    addAndMakeVisible(channelSlider = new ImageSlider(dialImage, 16, true));
    channelSlider->setSliderStyle(Slider::Rotary);
    channelSlider->setRotaryParameters(0, M_PI*2, false);
    channelSlider->setRange(1, 16, 1);
    channelSlider->addListener(this);
    channelSlider->setOpaque(false);
    channelSlider->addMouseListener(this, false);
    
    
    //------------------Pressure minimum range slider------------------------------
    addAndMakeVisible(pressureMinRangeSlider = new AlphaSlider());
    pressureMinRangeSlider->setRange(0, 127, 1);
    pressureMinRangeSlider->addListener(this);
    pressureMinRangeSlider->setValue(0, false);
    pressureMinRangeSlider->addMouseListener(this, true);
    
    
    //------------------Pressure maximum range slider------------------------------
    addAndMakeVisible(pressureMaxRangeSlider = new AlphaSlider());
    pressureMaxRangeSlider->setRange(0, 127, 1);
    pressureMaxRangeSlider->addListener(this);
    pressureMaxRangeSlider->setValue(127, false);
    pressureMaxRangeSlider->addMouseListener(this, true);
    
    
    //----------------pressure mode combobox-------------------------------
    addAndMakeVisible(pressureModeMenu = new ComboBox());
    pressureModeMenu->addListener(this);
    pressureModeMenu->addMouseListener(this, true);
    
    pressureModeMenu->addItem("Polyphonic Aftertouch", 1);
    pressureModeMenu->addItem("Channel Aftertouch", 3);
    pressureModeMenu->addItem("CC Data", 2);
    pressureModeMenu->addItem("Mod Wheel", 6);
    pressureModeMenu->addItem("Pitch Bend (Up)", 4);
    pressureModeMenu->addItem("Pitch Bend (Down)", 5);
    pressureModeMenu->setSelectedId(1, true);
    
    
    
    //----------------pressure play state combobox-------------------------------
    addAndMakeVisible(triggerModeMenu = new ComboBox());
    triggerModeMenu->addListener(this);
    triggerModeMenu->addMouseListener(this, true);
    triggerModeMenu->addListener(this);
    triggerModeMenu->addItem("Hold", 1);
    triggerModeMenu->addItem("Toggle", 2);
    triggerModeMenu->addItem("Toggle Release", 3);
    triggerModeMenu->addItem("Latch", 4);
    triggerModeMenu->addItem("Latch Max", 5);
    triggerModeMenu->addItem("Trigger", 6);
    triggerModeMenu->setSelectedId(1, true);
    triggerModeMenu->addMouseListener(this, true);
    
    addAndMakeVisible(indestructibleButton = new TextButton("indestruct"));
    indestructibleButton->addListener(this);
    indestructibleButton->addMouseListener(this, true);
    indestructibleButton->setClickingTogglesState(true);
    indestructibleButton->setToggleState(0, false);
    
    addAndMakeVisible(stickyButton = new TextButton("Sticky"));
    stickyButton->addListener(this);
    stickyButton->addMouseListener(this, true);
    stickyButton->setClickingTogglesState(true);
    stickyButton->setToggleState(0, false);
    
    
    
    //---------------pressure status button-------------------------------------
    
    addAndMakeVisible(pressureStatusButton = new GuiSwitch());
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
    
    //---------------CC controller Slider-------------------------------------
    addAndMakeVisible(ccControllerSlider = new AlphaSlider());
    ccControllerSlider->setRange(0, 127, 1);
    ccControllerSlider->addListener(this);
    ccControllerSlider->addMouseListener(this, true);
    
    ccControllerSlider->setVisible(false);
    
    //---------------exclusive group Slider-----------------------------------
    addAndMakeVisible(exclusiveGroupSlider = new AlphaSlider());
    exclusiveGroupSlider->setRange(1, 16, 1);
    exclusiveGroupSlider->setValue(1, false);
    exclusiveGroupSlider->addListener(this);
    
    //---------------trigger mode menu---------------------
    addAndMakeVisible(quantizeModeMenu = new ComboBox());
    quantizeModeMenu->addItem("Free", 1);
    quantizeModeMenu->addItem("Quantized", 2);
    quantizeModeMenu->setSelectedId(1, true);
    quantizeModeMenu->addMouseListener(this, true);
    quantizeModeMenu->addListener(this);
    
    //these need to be on top of all other components
    addAndMakeVisible(speakerLeft = new GuiSpeaker());
    speakerLeft->setVisible(false);
    addAndMakeVisible(speakerRight = new GuiSpeaker());
    speakerRight->setVisible(false);
}

GuiMidiMode::~GuiMidiMode()
{
    deleteAllChildren();
}

//==============================================================================
void GuiMidiMode::resized()
{
    circleBackgroundLeft->setBounds(14, 402, 230, 230);
    circleBackgroundRight->setBounds(780, 402, 230, 230);
    speakerLeft->setBounds(14, 402, 230, 230);
    speakerRight->setBounds(780, 402, 230, 230);
    
    //the following three sets only one is visible at any time
    //circlePianoOneOctave->setBounds(780, 402, 230, 230);
    //circlePianoForScales->setBounds(780, 402, 230, 230);
    circlePiano->setBounds(780, 402, 230, 230);
    
    //----------------------------------------------------------------------------------------------
    velocitySlider->setBounds(873, 495, 45, 45);
    channelSlider->setBounds(850, 251, 142, 142);
    
    pressureMinRangeSlider->setBounds(LEFT_CIRCLE_X, 440, COMPONENT_W, COMPONENT_H);
    pressureMaxRangeSlider->setBounds(LEFT_CIRCLE_X, 470, COMPONENT_W, COMPONENT_H);
    
    pressureModeMenu->setBounds(LEFT_CIRCLE_X, 500, COMPONENT_W, COMPONENT_H);
    ccControllerSlider->setBounds(LEFT_CIRCLE_X, 530, COMPONENT_W, COMPONENT_H);
    triggerModeMenu->setBounds(LEFT_CIRCLE_X, 570, COMPONENT_W, COMPONENT_H);
    indestructibleButton->setBounds(LEFT_CIRCLE_X, 593, 50, 15);
    stickyButton->setBounds(LEFT_CIRCLE_X+50, 593, 50, 15);
    
    pressureStatusButton->setBounds(165, 360, 45, 45);
    noteStatusButton->setBounds(810, 360, 45, 45);
    
    exclusiveGroupSlider->setBounds(871, 210, COMPONENT_W, COMPONENT_H);
    
    quantizeModeMenu->setBounds(800, 155, 100, 20);
    
    
}

void GuiMidiMode::paint (Graphics& g)
{
    
}


void GuiMidiMode::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    circlePiano->setCurrentlySelectedPad(selectedPads);
    //circlePianoForScales->setCurrentlySelectedPad(padNumber);
}

//==============================================================================
void GuiMidiMode::comboBoxChanged (ComboBox* comboBox)
{
    //This method takes the selected IDs from the combo box and sets it to the correct
    //variable in the relevent padSettings index/es
    
    //==============================================================================
    //pressure mode combobox
    if (comboBox == pressureModeMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiPressureMode(pressureModeMenu->getSelectedId());
        }
        
        if (pressureModeMenu->getSelectedId() == 2)
            ccControllerSlider->setVisible(true);
        else
            ccControllerSlider->setVisible(false);
    }
    
    //==============================================================================
    //pressure triggerMode combo box
    else if (comboBox == triggerModeMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiTriggerMode(triggerModeMenu->getSelectedId());
        }
    }
    
    
    //==============================================================================
    //trigger mode combobox
    if (comboBox == quantizeModeMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiQuantizeMode(quantizeModeMenu->getSelectedId());
        }
    }

}

void GuiMidiMode::sliderValueChanged (Slider* slider)
{
    
    //velocity slider
    if (slider == velocitySlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiVelocity(velocitySlider->sliderComponent()->getValue());
        }
    }
    
    
    //channel slider
    if (slider == channelSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiChannel(channelSlider->getValue());
        }
        
    }

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
    
    
    //Exclusive group
    if (slider == exclusiveGroupSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiExclusiveGroup(exclusiveGroupSlider->getValue());
        }
    }

}

void GuiMidiMode::buttonClicked (Button* button)
{
    
    if(button == pressureStatusButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiPressureStatus(pressureStatusButton->getToggleState());
        }
        
        //update speaker display
        if(pressureStatusButton->getToggleStateValue()==true)
            speakerLeft->setVisible(false);
        else
            speakerLeft->setVisible(true);
    }
    
    
    else if(button == noteStatusButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMidiNoteStatus(noteStatusButton->getToggleState());
        }
        
        //update speaker display
        if(noteStatusButton->getToggleStateValue()==true)
            speakerRight->setVisible(false);
        else
            speakerRight->setVisible(true);
        
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
}

void GuiMidiMode::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/dissabled any unneeded components. 
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        //update circle piano component
        circlePiano->updateDisplay();
        //circlePiano->setVisible(true);
        
        //circlePianoOneOctave->setVisible(false);
        //circlePianoForScales->setVisible(false);
        
        //get the settings of the pad selected (in padMenu ComboBox)
        //set the stored values on the GUI components
        //Don't broadcast any changes to the component Listeners. Only want to update the GUI here
        int i;
        i = PAD_SETTINGS->getMidiNote();
        i = PAD_SETTINGS->getMidiVelocity();
        velocitySlider->sliderComponent()->setValue(i, true);
        i = PAD_SETTINGS->getMidiChannel();
        channelSlider->setValue(i, false);
        i = PAD_SETTINGS->getMidiMinPressureRange();
        pressureMinRangeSlider->setValue(i, false);
        i = PAD_SETTINGS->getMidiMaxPressureRange();
        pressureMaxRangeSlider->setValue(i, false);
        i = PAD_SETTINGS->getMidiPressureMode();
        pressureModeMenu->setSelectedId(i, true);
        i = PAD_SETTINGS->getMidiTriggerMode();
        triggerModeMenu->setSelectedId(i, true);
        i = PAD_SETTINGS->getMidiCcController();
        ccControllerSlider->setValue(i, false);
        i = PAD_SETTINGS->getMidiExclusiveGroup();
        exclusiveGroupSlider->setValue(i, false);
        quantizeModeMenu->setSelectedId(PAD_SETTINGS->getMidiQuantizeMode(), true);
        indestructibleButton->setToggleState(PAD_SETTINGS->getMidiIndestructible(), false);
        stickyButton->setToggleState(PAD_SETTINGS->getMidiSticky(), false);
        
        bool b;
        
        b = PAD_SETTINGS->getMidiPressureStatus();
        pressureStatusButton->setToggleState(b, false);
        //update speaker display
        if(pressureStatusButton->getToggleStateValue()==true)
            speakerLeft->setVisible(false);
        else
            speakerLeft->setVisible(true);
        
        b = PAD_SETTINGS->getMidiNoteStatus();
        noteStatusButton->setToggleState(b, false);
        //update speaker display
        if(noteStatusButton->getToggleStateValue()==true)
            speakerRight->setVisible(false);
        else
            speakerRight->setVisible(true);
        
    }
    
    else if(MULTI_PADS)
    {
        //circlePianoOneOctave->updateDisplay();
        //circlePianoOneOctave->setVisible(true);
        
        //set default values
        velocitySlider->sliderComponent()->setValue(110, true);
        channelSlider->setValue(1, false);
        pressureMinRangeSlider->setValue(0, false);
        pressureMaxRangeSlider->setValue(127, false);
        pressureModeMenu->setSelectedId(1, true);
        triggerModeMenu->setSelectedId(2, true);
        indestructibleButton->setToggleState(0, false);
        stickyButton->setToggleState(0, false);
        pressureStatusButton->setToggleState(true, false);
        pressureStatusButton->setButtonText("Pressure Data On");
        noteStatusButton->setToggleState(true, false);
        noteStatusButton->setButtonText("Note Data On");
        ccControllerSlider->setValue(12, false);
        exclusiveGroupSlider->setValue(1, false);
        quantizeModeMenu->setSelectedId(1, true);
        
        //circlePianoForScales->setVisible(false);
        circlePiano->updateDisplay();
        //circlePiano->setVisible(true);
        speakerLeft->setVisible(false);
        speakerRight->setVisible(false);
        
    }

    
    if (pressureModeMenu->getSelectedId() == 2)
    {
        ccControllerSlider->setVisible(true);
    }
    else
    {
        ccControllerSlider->setVisible(false);
    }

}


void GuiMidiMode::mouseEnter (const MouseEvent &e)
{
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
    else if (exclusiveGroupSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Exclusive Group. If the pad is set to a group other than group 1 the MIDI pad will be in 'Exclusive Mode' - playing this MIDI note will turn off the previously playing MIDI note of the same group. The default group 1 allows does not enable this exclusive mode.");
    }
    else if (quantizeModeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Trigger Mode Menu. The trigger mode determines the start time of the MIDI note. 'Free' will trigger the loop as soon as the pad is pressed, 'Quantized' will trigger the loop at the next quantized time value set using the global clock.");
    }
    
}

void GuiMidiMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}


