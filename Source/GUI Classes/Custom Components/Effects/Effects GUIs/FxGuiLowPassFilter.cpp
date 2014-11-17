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
#include "../../../Binary Data/MainBinaryData.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiLowpassFilter::GuiLowpassFilter(MainComponent &ref)
                                : mainComponentRef(ref)
{
    addAndMakeVisible(mixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	mixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    mixSlider->setRange(0.0, 1.0, 0.001);
    mixSlider->addListener(this);
    mixSlider->addMouseListener(this, true);
    
	addAndMakeVisible(frequencySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	frequencySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    frequencySlider->setRange(30, 20000, 1);
    frequencySlider->setSkewFactor(0.35);
    frequencySlider->addListener(this);
    frequencySlider->addMouseListener(this, true);
    
    addAndMakeVisible(bandwidthSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	bandwidthSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    bandwidthSlider->setRange(1.0, 100.0, 0.1);
    bandwidthSlider->addListener(this);
    bandwidthSlider->addMouseListener(this, true);
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Mix"), 2);
    alphaTouchMenu->addItem(translate("Cut-Off Frequency"), 3);
    alphaTouchMenu->addItem(translate("Bandwidth/Resonance"), 4);
    alphaTouchMenu->setSelectedId(1, dontSendNotification);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(MainBinaryData::inverticon_png, MainBinaryData::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0, 0.001);
    intensitySlider->setValue(1.0, dontSendNotification);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    
    //---------------parameter label -------------------------------------
    addAndMakeVisible(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setFont(Font(11 + AlphaTheme::getInstance()->fontSizeAddition));
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

void GuiLowpassFilter::paint(Graphics &g)
{
    parameterHoverLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->mainColour);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaTheme::getInstance()->mainColourLighter);
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
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
    
    if (slider == frequencySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxLpfFreq(frequencySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 0), dontSendNotification);
    }
    
    
    if (slider == bandwidthSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxLpfBandwidth(bandwidthSlider->getValue());
        }
    
        parameterHoverLabel->setText(String(slider->getValue(), 2), dontSendNotification);
    }
    
    
    if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxLpfAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
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
        
        mixSlider->setValue(PAD_SETTINGS->getPadFxLpfMix(), dontSendNotification);
        frequencySlider->setValue(PAD_SETTINGS->getPadFxLpfFreq(), dontSendNotification);
        bandwidthSlider->setValue(PAD_SETTINGS->getPadFxLpfBandwidth(), dontSendNotification);
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxLpfAlphaTouch(), dontSendNotification);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxLpfAtReverse(), dontSendNotification);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxLpfAtIntensity(), dontSendNotification);
    }
    
    
    else if(MULTI_PADS)
    {
        mixSlider->setValue(1.0, dontSendNotification);
        frequencySlider->setValue(1000, dontSendNotification);
        bandwidthSlider->setValue(5.0, dontSendNotification);
        alphaTouchMenu->setSelectedId(0, dontSendNotification);
        reverseButton->setToggleState(0, dontSendNotification);
        intensitySlider->setValue(1.0, dontSendNotification);
    }

}

void GuiLowpassFilter::mouseEnter (const MouseEvent &e)
{
    if (mixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Wet/Dry Mix. Sets the Wet/Dry mix for the Low-pass filter on the selected pads."));
        parameterHoverLabel->setText(String(mixSlider->getValue(), 3), dontSendNotification);
    }
    else if (frequencySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Cut-Off Frequency. Sets the cut-off frequency for the Low-Pass filter on the selected pads."));
        parameterHoverLabel->setText(String(frequencySlider->getValue(), 0), dontSendNotification);
    }
    else if (bandwidthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Bandwidth. Sets the bandwidth for the Low-Pass filter on the selected pads."));
        parameterHoverLabel->setText(String(bandwidthSlider->getValue(), 2), dontSendNotification);
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
        parameterHoverLabel->setText(String(intensitySlider->getValue(), 3), dontSendNotification);
    }
    
}

void GuiLowpassFilter::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, dontSendNotification);
    
}