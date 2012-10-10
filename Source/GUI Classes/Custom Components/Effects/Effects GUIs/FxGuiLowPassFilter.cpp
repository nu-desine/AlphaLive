//
//  FxGuiLowPassFilter.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 13/12/2011.
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

#include "FxGuiLowPassFilter.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/BinaryDataNew.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiLowpassFilter::GuiLowpassFilter(MainComponent &ref)
                                : mainComponentRef(ref)
{
    addAndMakeVisible(mixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	mixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    mixSlider->setRange(0.0, 1.0);
    mixSlider->addListener(this);
    mixSlider->addMouseListener(this, true);
    
	addAndMakeVisible(frequencySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	frequencySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    frequencySlider->setRange(30, 20000, 1);
    frequencySlider->addListener(this);
    frequencySlider->addMouseListener(this, true);
    
    addAndMakeVisible(bandwidthSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	bandwidthSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    bandwidthSlider->setRange(0.0, 100.0);
    bandwidthSlider->addListener(this);
    bandwidthSlider->addMouseListener(this, true);
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Mix"), 2);
    alphaTouchMenu->addItem(translate("Cut-Off Frequency"), 3);
    alphaTouchMenu->addItem(translate("Bandwidth/Resonance"), 4);
    alphaTouchMenu->setSelectedId(1, true);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::inverticon_png, BinaryDataNew::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0);
    intensitySlider->setValue(1.0, false);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaColours::lightblue);
    
    //---------------parameter label -------------------------------------
    addAndMakeVisible(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaColours::blue);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
    
    setInterceptsMouseClicks(false, true);
}

GuiLowpassFilter::~GuiLowpassFilter()
{
    deleteAllChildren();
}



void GuiLowpassFilter::resized()
{
    mixSlider->setBounds(97, 97, 130, 130);
    frequencySlider->setBounds(87, 87, 150, 150);
    bandwidthSlider->setBounds(77, 77, 170, 170);
	intensitySlider->setBounds(67, 67, 190, 190);
    
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    parameterHoverLabel->setBounds(144, 187, 36, 15);
}



void GuiLowpassFilter::sliderValueChanged (Slider *slider)
{
    if (slider == mixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxLpfMix(mixSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }
    
    
    if (slider == frequencySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxLpfFreq(frequencySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 0), false);
    }
    
    
    if (slider == bandwidthSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxLpfBandwidth(bandwidthSlider->getValue());
        }
    
        parameterHoverLabel->setText(String(slider->getValue(), 2), false);
    }
    
    
    if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxLpfAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }


}

void GuiLowpassFilter::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxLpfAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
    }

}


void GuiLowpassFilter::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxLpfAtReverse(reverseButton->getToggleState());
        }

    }
}

void GuiLowpassFilter::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiLowpassFilter::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        mixSlider->setValue(PAD_SETTINGS->getPadFxLpfMix(), false);
        frequencySlider->setValue(PAD_SETTINGS->getPadFxLpfFreq(), false);
        bandwidthSlider->setValue(PAD_SETTINGS->getPadFxLpfBandwidth(), false);
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxLpfAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxLpfAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxLpfAtIntensity(), false);
    }
    
    
    else if(MULTI_PADS)
    {
        mixSlider->setValue(1.0, false);
        frequencySlider->setValue(1000, false);
        bandwidthSlider->setValue(5.0, false);
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, false);
    }

}

void GuiLowpassFilter::mouseEnter (const MouseEvent &e)
{
    if (mixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Low-Pass Filter Mix Control. Sets the wet/dry ratio for Low-pass filter for the selected pads."));
        parameterHoverLabel->setText(String(mixSlider->getValue(), 3), false);
    }
    else if (frequencySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Low-Pass Filter Cut-Off Frequency Control. Sets the cut-off frequency for Low-Pass filter for the selected pads."));
        parameterHoverLabel->setText(String(frequencySlider->getValue(), 0), false);
    }
    else if (bandwidthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Low-Pass Filter Bandwidth Control. Sets the bandwidth for Low-Pass filter for the selected pads."));
        parameterHoverLabel->setText(String(bandwidthSlider->getValue(), 2), false);
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

void GuiLowpassFilter::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, false);
    
}