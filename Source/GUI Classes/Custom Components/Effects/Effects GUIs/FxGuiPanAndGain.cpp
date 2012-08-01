//
//  FxGuiPanAndGain.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 22/12/2011.
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

#include "FxGuiPanAndGain.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiGainAndPan::GuiGainAndPan(MainComponent &ref)
                            : mainComponentRef(ref)
{
    addAndMakeVisible(gainSlider = new AlphaImageKnob(2));
    gainSlider->sliderComponent()->setRange(0.0, 1.0);
    gainSlider->sliderComponent()->setValue(0.5, false);
    gainSlider->sliderComponent()->addListener(this);
    gainSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(panSlider = new AlphaImageKnob(2, true));
    panSlider->sliderComponent()->setRange(0.0, 1.0);
    panSlider->sliderComponent()->setValue(0.5, false);
    panSlider->sliderComponent()->addListener(this);
    panSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem("Off", 1);
    alphaTouchMenu->addItem("Gain", 2);
    alphaTouchMenu->addItem("Pan", 3);
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

GuiGainAndPan::~GuiGainAndPan()
{
    deleteAllChildren();
}



void GuiGainAndPan::resized()
{
    gainSlider->setBounds(65, 55, 40, 40);
    panSlider->setBounds(128, 55, 40, 40);
    
    alphaTouchMenu->setBounds(65, 125, 100, 15);
    reverseButton->setBounds(68, 155, 40, 20);
    intensitySlider->setBounds(120, 145, 40, 40);
}



void GuiGainAndPan::sliderValueChanged (Slider *slider)
{
    if (slider == gainSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxGainPanGain(gainSlider->sliderComponent()->getValue());
        }
        
    }
    
    
    if (slider == panSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxGainPanPan(panSlider->sliderComponent()->getValue());
        }
        
    }    
    
    if (slider == intensitySlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxGainPanAtIntensity(intensitySlider->sliderComponent()->getValue());
        }
    }
    
    
}

void GuiGainAndPan::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxGainPanAlphaTouch(alphaTouchMenu->getSelectedId());
        }
    
    }
    
}


void GuiGainAndPan::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxGainPanAtReverse(reverseButton->getToggleState());
        }
        
    }
}

void GuiGainAndPan::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiGainAndPan::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        gainSlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxGainPanGain(), false);
        panSlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxGainPanPan(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getSamplerFxGainPanAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getSamplerFxGainPanAtReverse(), false);
        intensitySlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxGainPanAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        gainSlider->sliderComponent()->setValue(0.5, false);
        panSlider->sliderComponent()->setValue(0.5, false);
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->sliderComponent()->setValue(1.0, false);
    }
    
}

void GuiGainAndPan::mouseEnter (const MouseEvent &e)
{
    if (gainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Gain Control. Sets the gain for the selected pad/pads.");
    }
    else if (panSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Pan Control. Sets the pan level for the selected pad/pads.");
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

void GuiGainAndPan::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}