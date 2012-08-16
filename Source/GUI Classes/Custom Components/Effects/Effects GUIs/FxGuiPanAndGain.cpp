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
#include "../../../Binary Data/BinaryDataNew.h"
//#include "../../../AlphaLiveLookandFeel.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiGainAndPan::GuiGainAndPan(MainComponent &ref)
                            : mainComponentRef(ref)
{
    addAndMakeVisible(gainSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	gainSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    gainSlider->setRange(0.0, 1.0);
    gainSlider->setValue(0.5, false);
    gainSlider->addListener(this);
    gainSlider->addMouseListener(this, true);
    
    addAndMakeVisible(panSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	panSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    panSlider->setRange(0.0, 1.0);
    panSlider->setValue(0.5, false);
    panSlider->addListener(this);
    panSlider->addMouseListener(this, true);
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem("Off", 1);
    alphaTouchMenu->addItem("Gain", 2);
    alphaTouchMenu->addItem("Pan", 3);
    alphaTouchMenu->setSelectedId(1, true);
    
	Image *reverseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::inverticon_png, BinaryDataNew::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0);
    intensitySlider->setValue(1.0, false);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaColours::lightblue);
    
    //currentlySelectedPad = 99;
    
    setInterceptsMouseClicks(false, true);
}

GuiGainAndPan::~GuiGainAndPan()
{
    deleteAllChildren();
}



void GuiGainAndPan::resized()
{
    gainSlider->setBounds(97, 97, 130, 130);
    panSlider->setBounds(87, 87, 150, 150);
    intensitySlider->setBounds(77, 77, 170, 170);
	
    alphaTouchMenu->setBounds(119, 192, 87, 20);
    reverseButton->setBounds(211,211, 32, 32);
}



void GuiGainAndPan::sliderValueChanged (Slider *slider)
{
    if (slider == gainSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxGainPanGain(gainSlider->getValue());
        }
        
    }
    
    
    if (slider == panSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxGainPanPan(panSlider->getValue());
        }
        
    }    
    
    if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxGainPanAtIntensity(intensitySlider->getValue());
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
        gainSlider->setValue(PAD_SETTINGS->getSamplerFxGainPanGain(), false);
        panSlider->setValue(PAD_SETTINGS->getSamplerFxGainPanPan(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getSamplerFxGainPanAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getSamplerFxGainPanAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getSamplerFxGainPanAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        gainSlider->setValue(0.5, false);
        panSlider->setValue(0.5, false);
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, false);
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