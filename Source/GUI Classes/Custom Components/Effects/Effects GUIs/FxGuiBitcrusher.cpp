//
//  FxGuiBitcrusher.cpp
//  AlphaLive
//
//  Created by Felix Godden on the 02/10/2012
//  Copyright 2012 nu desine.
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

#include "FxGuiBitcrusher.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/BinaryDataNew.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiBitcrusher::GuiBitcrusher(MainComponent &ref)
:           mainComponentRef(ref)
{
	
	addAndMakeVisible(wetDryMixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	wetDryMixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    wetDryMixSlider->setRange(0, 1.0);
    wetDryMixSlider->addListener(this);
    wetDryMixSlider->addMouseListener(this, true);
	
	addAndMakeVisible(smoothingSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	smoothingSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    smoothingSlider->setRange(0, 1.0);
    smoothingSlider->addListener(this);
    smoothingSlider->addMouseListener(this, true);
	
	addAndMakeVisible(crushSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	crushSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    crushSlider->setRange(1, 8, 1.);
    crushSlider->addListener(this);
    crushSlider->addMouseListener(this, true);
	
	addAndMakeVisible(downsampleSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	downsampleSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    downsampleSlider->setRange(1, 16, 1.);
    downsampleSlider->addListener(this);
    downsampleSlider->addMouseListener(this, true);
	
    addAndMakeVisible(inputGainSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	inputGainSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    inputGainSlider->setRange(0.0, 1.0);
    inputGainSlider->addListener(this);
    inputGainSlider->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 230));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaColours::lightblue);
    
    addAndMakeVisible(syncButton = new AlphaTextButton(translate("SYNC")));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);    
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Input Gain"), 2);
    alphaTouchMenu->addItem(translate("Downsample"), 3);
	alphaTouchMenu->addItem(translate("Crush"), 4);
    alphaTouchMenu->addItem(translate("Smoothing"), 5);
	alphaTouchMenu->addItem(translate("Wet/Dry Mix"), 6);
    alphaTouchMenu->setSelectedId(1, true);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::inverticon_png, BinaryDataNew::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    //---------------parameter label -------------------------------------
    addAndMakeVisible(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaColours::blue);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
	
    setInterceptsMouseClicks(false, true);
}

GuiBitcrusher::~GuiBitcrusher()
{
    deleteAllChildren();
}



void GuiBitcrusher::resized()
{
	wetDryMixSlider->setBounds(97, 97, 130, 130);
    smoothingSlider->setBounds(87, 87, 150, 150);
	crushSlider->setBounds(77, 77, 170, 170);
	downsampleSlider->setBounds(67, 67, 190, 190);
	inputGainSlider->setBounds(57, 57, 210, 210);
	
    intensitySlider->setBounds(47, 47, 230, 230);
    
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    parameterHoverLabel->setBounds(144, 187, 36, 15);
    
}



void GuiBitcrusher::sliderValueChanged (Slider *slider)
{
    if (slider == inputGainSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxBitcrusherInputGain(inputGainSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
	
	else if (slider == downsampleSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxBitcrusherDownsample(downsampleSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
    
    else if (slider == crushSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxBitcrusherCrush(crushSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
	
	else if (slider == smoothingSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxBitcrusherSmoothing(smoothingSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
	
	else if (slider == wetDryMixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxBitcrusherWetDryMix(wetDryMixSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxBitcrusherAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
    
}

void GuiBitcrusher::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxBitcrusherAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
    }
    
}


void GuiBitcrusher::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxBitcrusherAtReverse(reverseButton->getToggleState());
        }
        
    }
    
}

void GuiBitcrusher::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiBitcrusher::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        inputGainSlider->setValue(PAD_SETTINGS->getPadFxBitcrusherInputGain(), false);
		downsampleSlider->setValue(PAD_SETTINGS->getPadFxBitcrusherCrush(), false);
        crushSlider->setValue(PAD_SETTINGS->getPadFxBitcrusherCrush(), false);
		smoothingSlider->setValue(PAD_SETTINGS->getPadFxBitcrusherSmoothing(), false);
		wetDryMixSlider->setValue(PAD_SETTINGS->getPadFxBitcrusherWetDryMix(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxBitcrusherAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxBitcrusherAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxBitcrusherAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        inputGainSlider->setValue(0.7, true);
		downsampleSlider->setValue(16, true);
        crushSlider->setValue(8, true);
		smoothingSlider->setValue(1.0, true);
		wetDryMixSlider->setValue(1.0, true);
        syncButton->setToggleState(true, true);
        
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, false);
    }
    
}

void GuiBitcrusher::mouseEnter (const MouseEvent &e)
{
    if (inputGainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Input Gain Control. Sets the gain of the signal to be bitcrushed."));
        parameterHoverLabel->setText(String(inputGainSlider->getValue(), 3), false);
    }
	else if (downsampleSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Downsample Contol. Sets the amount of downsampling effecting the bitcrushing."));
        parameterHoverLabel->setText(String(downsampleSlider->getValue(), 3), false);
    }
    else if (crushSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Crush Contol. Sets the amount of bitcrushing effecting the signal."));
        parameterHoverLabel->setText(String(crushSlider->getValue(), 3), false);
    }
	else if (smoothingSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Smoothing Control. Sets the Smoothing of the effected signal."));
        parameterHoverLabel->setText(String(smoothingSlider->getValue(), 3), false);
    }
	else if (wetDryMixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Wet/Dry Mix Control. Sets the balance of distorted signal to clean signal."));
        parameterHoverLabel->setText(String(wetDryMixSlider->getValue(), 3), false);
    }
	
    else if (alphaTouchMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::alphaTouchMenu));
    }
    else if (reverseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::inverseButton));
    }
    else if (intensitySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::intensitySlider));
        parameterHoverLabel->setText(String(intensitySlider->getValue(), 3), false);
    }
    
}

void GuiBitcrusher::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, false);
    
}
