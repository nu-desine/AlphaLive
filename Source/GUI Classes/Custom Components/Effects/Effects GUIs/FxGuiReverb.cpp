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
    addAndMakeVisible(mixSlider = new AlphaImageKnob(2));
    mixSlider->sliderComponent()->setRange(0.0, 1.0);
    mixSlider->sliderComponent()->setValue(0.7, false);
    mixSlider->sliderComponent()->addListener(this);
    mixSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(roomSizeSlider = new AlphaImageKnob(2));
    roomSizeSlider->sliderComponent()->setRange(0.0, 1.0);
    roomSizeSlider->sliderComponent()->setValue(0.5, false);
    roomSizeSlider->sliderComponent()->addListener(this);
    roomSizeSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(dampingSlider = new AlphaImageKnob(2));
    dampingSlider->sliderComponent()->setRange(0.0, 1.0);
    dampingSlider->sliderComponent()->setValue(0.5, false);
    dampingSlider->sliderComponent()->addListener(this);
    dampingSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(widthSlider = new AlphaImageKnob(2));
    widthSlider->sliderComponent()->setRange(0.0, 1.0);
    widthSlider->sliderComponent()->setValue(0.5, false);
    widthSlider->sliderComponent()->addListener(this);
    widthSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(freezeModeSlider = new AlphaImageKnob(2));
    freezeModeSlider->sliderComponent()->setRange(0.0, 1.0);
    freezeModeSlider->sliderComponent()->setValue(0.2, false);
    freezeModeSlider->sliderComponent()->addListener(this);
    freezeModeSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem("Off", 1);
    alphaTouchMenu->addItem("Mix", 2);
    alphaTouchMenu->addItem("Room Size", 3);
    alphaTouchMenu->addItem("Damping", 4);
    alphaTouchMenu->addItem("Width", 5);
    //alphaTouchMenu->addItem("Freeze Mode", 6);
    alphaTouchMenu->setSelectedId(1, true);
    
    addAndMakeVisible(reverseButton = new TextButton("Invert", "Invert"));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaImageKnob(2));
    intensitySlider->sliderComponent()->setRange(0.0, 1.0);
    intensitySlider->sliderComponent()->setValue(1.0, false);
    intensitySlider->sliderComponent()->addListener(this);
    intensitySlider->sliderComponent()->addMouseListener(this, true);
    
    //currentlySelectedPad = 99;
    
    setInterceptsMouseClicks(false, true);
}

GuiReverb::~GuiReverb()
{
    deleteAllChildren();
}



void GuiReverb::resized()
{
    mixSlider->setBounds(getWidth()/3, 40, 35, 35);
    roomSizeSlider->setBounds(((getWidth()/3)*2)-35, 40, 35, 35);
    dampingSlider->setBounds((getWidth()/3)-35, 70, 35, 35);
    //freezeModeSlider->setBounds((getWidth()/2)-(35/2), 70, 35, 35);
    widthSlider->setBounds((getWidth()/3)*2, 70, 35, 35);
    
    alphaTouchMenu->setBounds(65, 125, 100, 15);
    reverseButton->setBounds(68, 155, 40, 20);
    intensitySlider->setBounds(120, 145, 40, 40);
}



void GuiReverb::sliderValueChanged (Slider *slider)
{
    if (slider == mixSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbMix(mixSlider->sliderComponent()->getValue());
        }
        
    }
    
    else if (slider == roomSizeSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbRoomSize(roomSizeSlider->sliderComponent()->getValue());
        }
        
    }
    
    else if (slider == dampingSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbDamping(dampingSlider->sliderComponent()->getValue());
        }
        
    }
    
    else if (slider == widthSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbWidth(widthSlider->sliderComponent()->getValue());
        }
        
    }
    
    else if (slider == freezeModeSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbFreezeMode(freezeModeSlider->sliderComponent()->getValue());
        }
        
    }

    
    else if (slider == intensitySlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxReverbAtIntensity(intensitySlider->sliderComponent()->getValue());
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
        mixSlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxReverbMix(), false);
        roomSizeSlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxReverbRoomSize(), false);
        dampingSlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxReverbDamping(), false);
        widthSlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxReverbWidth(), false);
        freezeModeSlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxReverbFreezeMode(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getSamplerFxReverbAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getSamplerFxReverbAtReverse(), false);
        intensitySlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxReverbAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        mixSlider->sliderComponent()->setValue(0.7, false);
        roomSizeSlider->sliderComponent()->setValue(0.5, false);
        dampingSlider->sliderComponent()->setValue(0.5, false);
        widthSlider->sliderComponent()->setValue(0.5, false);
        freezeModeSlider->sliderComponent()->setValue(0.2, false);
        
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->sliderComponent()->setValue(1.0, false);
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
    else if (freezeModeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Reverb Freeze Mode Control. Sets the reverb freeze mode level for the selected pad/pads. Values over 0.5 will put the reverb into a continuous feedback loop.");
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