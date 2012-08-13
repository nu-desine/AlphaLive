//
//  FxGuiReverb.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 21/12/2011.
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

#include "FxGuiReverb.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiReverb::GuiReverb(MainComponent &ref)
                        : mainComponentRef(ref)
{
    
    addAndMakeVisible(mixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	mixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    mixSlider->setRange(0.0, 1.0);
    mixSlider->setValue(0.7, false);
    mixSlider->addListener(this);
    mixSlider->addMouseListener(this, true);
    
	addAndMakeVisible(roomSizeSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	roomSizeSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    roomSizeSlider->setRange(0, 1.0);
    roomSizeSlider->setValue(0.5, false);
    roomSizeSlider->addListener(this);
    roomSizeSlider->addMouseListener(this, true);
    
    addAndMakeVisible(dampingSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	dampingSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    dampingSlider->setRange(0.0, 1.0);
    dampingSlider->setValue(0.5, false);
    dampingSlider->addListener(this);
    dampingSlider->addMouseListener(this, true);
    
    addAndMakeVisible(widthSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	widthSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    widthSlider->setRange(0.0, 1.0);
    widthSlider->setValue(0.5, false);
    widthSlider->addListener(this);
    widthSlider->addMouseListener(this, true);
    
    
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem("Off", 1);
    alphaTouchMenu->addItem("Mix", 2);
    alphaTouchMenu->addItem("Room Size", 3);
    alphaTouchMenu->addItem("Damping", 4);
    alphaTouchMenu->addItem("Width", 5);
    alphaTouchMenu->setSelectedId(1, true);
    
    addAndMakeVisible(reverseButton = new AlphaTextButton("INVERT"));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0);
    intensitySlider->setValue(1.0, false);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaColours::lightblue);
    
    
    setInterceptsMouseClicks(false, true);
}

GuiReverb::~GuiReverb()
{
    deleteAllChildren();
}



void GuiReverb::resized()
{
    mixSlider->setBounds(97, 97, 130, 130);
    roomSizeSlider->setBounds(87, 87, 150, 150);
    dampingSlider->setBounds(77, 77, 170, 170);
	widthSlider->setBounds(67, 67, 190, 190);
    intensitySlider->setBounds(57, 57, 210, 210);
    
    alphaTouchMenu->setBounds(119, 192, 87, 20);
    reverseButton->setBounds(211,211, 32, 32);
}



void GuiReverb::sliderValueChanged (Slider *slider)
{
    if (slider == mixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbMix(mixSlider->getValue());
        }
        
    }
    
    else if (slider == roomSizeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbRoomSize(roomSizeSlider->getValue());
        }
        
    }
    
    else if (slider == dampingSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbDamping(dampingSlider->getValue());
        }
        
    }
    
    else if (slider == widthSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbWidth(widthSlider->getValue());
        }
        
    }
    

    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbAtIntensity(intensitySlider->getValue());
        }
    }
    
    
}

void GuiReverb::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbAlphaTouch(alphaTouchMenu->getSelectedId());
        }
    }
    
}


void GuiReverb::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbAtReverse(reverseButton->getToggleState());
        }
        
    }
}

void GuiReverb::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiReverb::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        mixSlider->setValue(PAD_SETTINGS->getSamplerFxReverbMix(), false);
        roomSizeSlider->setValue(PAD_SETTINGS->getSamplerFxReverbRoomSize(), false);
        dampingSlider->setValue(PAD_SETTINGS->getSamplerFxReverbDamping(), false);
        widthSlider->setValue(PAD_SETTINGS->getSamplerFxReverbWidth(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getSamplerFxReverbAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getSamplerFxReverbAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getSamplerFxReverbAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        mixSlider->setValue(0.7, false);
        roomSizeSlider->setValue(0.5, false);
        dampingSlider->setValue(0.5, false);
        widthSlider->setValue(0.5, false);
        
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, false);
    }
    
    
}

void GuiReverb::mouseEnter (const MouseEvent &e)
{
    if (mixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Reverb Mix Control. Sets the wet/dry ratio for reverb level for the selected pad/pads.");
    }
    else if (roomSizeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Reverb Room Size Control. Sets the reverb room size for the selected pad/pads. A larger room size will create a longer reverberation time.");
    }
    else if (dampingSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Reverb Damping Control. Sets the reverb damping level for the selected pad/pads. A larger damping value create a more subtle reverberation tail.");
    }
    else if (widthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Reverb Width Control. Sets the reverb width for the selected pad/pads. A larger width will create a wider reverb sound in the stereo spread.");
    }
        
    else if (alphaTouchMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("AlphaTouch Menu. Sets the effect parameter that the pads pressure will control for the selected pad/pads.");
    }
    else if (reverseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("AlphaTouch Reverse Button. Activate this button to reverse/invert the direction of the modulated created by the pressure of the selected pad/pads.");
    }
    else if (intensitySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("AlphaTouch Intensity Control. Sets the intensity/range of modulation created by the pressure of the selected pad/pads.");
    }
    
}

void GuiReverb::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}