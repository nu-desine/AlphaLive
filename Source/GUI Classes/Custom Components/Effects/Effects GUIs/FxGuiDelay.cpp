//
//  FxGuiDelay.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/07/2012.
//  Copyright 2012 nu desine. All rights reserved.
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

#include "FxGuiDelay.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/BinaryDataNew.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiDelay::GuiDelay(MainComponent &ref)
:           mainComponentRef(ref)
{
    addAndMakeVisible(wetMixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	wetMixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    wetMixSlider->setRange(0.0, 1.0);
    wetMixSlider->addListener(this);
    wetMixSlider->addMouseListener(this, true);
    
	addAndMakeVisible(feedbackSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	feedbackSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    feedbackSlider->setRange(0, 1.0);
    feedbackSlider->addListener(this);
    feedbackSlider->addMouseListener(this, true);
    
    addAndMakeVisible(delayTimeSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	delayTimeSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    delayTimeSlider->setRange(0.0, 4000.0);
    delayTimeSlider->addListener(this);
    delayTimeSlider->addMouseListener(this, true);
    
    addAndMakeVisible(lpfFrequencySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	lpfFrequencySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    lpfFrequencySlider->setRange(30, 20000, 1);
    lpfFrequencySlider->addListener(this);
    lpfFrequencySlider->addMouseListener(this, true);
    
    addAndMakeVisible(hpfFrequencySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	hpfFrequencySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    hpfFrequencySlider->setRange(30, 20000, 1);
    hpfFrequencySlider->addListener(this);
    hpfFrequencySlider->addMouseListener(this, true);
    

    
    addAndMakeVisible(delayTimeMenu = new ComboBox());
    delayTimeMenu->addListener(this);
    delayTimeMenu->addMouseListener(this, true);
    delayTimeMenu->addItem(translate("4 Beats"), 1);
    delayTimeMenu->addItem(translate("2 Beats"), 2);
    delayTimeMenu->addItem(translate("1 Beat"), 3);
    delayTimeMenu->addItem(translate("Half Beat"), 4);
    delayTimeMenu->addItem(translate("Quarter Beat"), 5);
    delayTimeMenu->setSelectedId(4, true);
    
    addAndMakeVisible(syncButton = new AlphaTextButton(translate("SYNC")));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);

    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Wet Mix"), 2);
    alphaTouchMenu->addItem(translate("Delay Time"), 3);
    alphaTouchMenu->addItem(translate("Feedback"), 4);
    alphaTouchMenu->addItem(translate("LPF Frequency"), 5);
    alphaTouchMenu->addItem(translate("HPF Frequency"), 6);
    alphaTouchMenu->setSelectedId(1, true);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::inverticon_png, BinaryDataNew::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 230));
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
    
    tempo = AppSettings::Instance()->getGlobalTempo();
    
    setInterceptsMouseClicks(false, true);
}

GuiDelay::~GuiDelay()
{
    deleteAllChildren();
}



void GuiDelay::resized()
{
    wetMixSlider->setBounds(97, 97, 130, 130);
    feedbackSlider->setBounds(87, 87, 150, 150);
    delayTimeSlider->setBounds(77, 77, 170, 170);
	lpfFrequencySlider->setBounds(67, 67, 190, 190);
    hpfFrequencySlider->setBounds(57, 57, 210, 210);
    
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    parameterHoverLabel->setBounds(144, 187, 36, 15);
    intensitySlider->setBounds(47, 47, 230, 230);
    
    delayTimeMenu->setBounds(119, 22, 87, 20);
    syncButton->setBounds(130, 235, 32, 32);
    
    
}



void GuiDelay::sliderValueChanged (Slider *slider)
{
    if (slider == wetMixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayMix(wetMixSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }
    
    else if (slider == delayTimeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayTime(delayTimeSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 2), false);
    }
    
    else if (slider == feedbackSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayFeedback(feedbackSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }
    
    else if (slider == lpfFrequencySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayLpfFreq(lpfFrequencySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 0), false);
    }
    
    else if (slider == hpfFrequencySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayHpfFreq(hpfFrequencySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 0), false);
    }
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }
    

    
}

void GuiDelay::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
    }
    
    else if (comboBox == delayTimeMenu)
    {
        //work out delay time (NOT sample number needed) based on beat length selected and the tempo.
        float delayTime;
        tempo = AppSettings::Instance()->getGlobalTempo();
        
        switch (delayTimeMenu->getSelectedId())
        {
            case 1: // 4 beats
                delayTime = 240000.0/tempo;
                break;
            case 2: // 2 beats
                delayTime = 120000.0/tempo;
                break;
            case 3: // 1 beat
                delayTime = 60000.0/tempo;
                break;
            case 4: //half beat
                delayTime = 30000.0/tempo;
                break;
            case 5: // quarter beat
                delayTime = 15000.0/tempo;
                break;
            default: // 1 beat
                delayTime = 60000.0/tempo;
                break;
        }
        
        delayTimeSlider->setValue(delayTime, false);
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayTime(delayTime);
            PAD_SETTINGS->setPadFxDelayTimeMenu(delayTimeMenu->getSelectedId());
        }
        
    }
    
}


void GuiDelay::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayAtReverse(reverseButton->getToggleState());
        }
    }
    
    else if (button == syncButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelaySync(syncButton->getToggleState());
        }
        
        delayTimeSlider->setVisible(false);
        delayTimeMenu->setVisible(false);
        
        if (syncButton->getToggleState() == 0) //off
            delayTimeSlider->setVisible(true);
        else //on
            delayTimeMenu->setVisible(true);
            
        
    }
}

void GuiDelay::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiDelay::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    delayTimeSlider->setVisible(false);
    delayTimeMenu->setVisible(false);
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        wetMixSlider->setValue(PAD_SETTINGS->getPadFxDelayMix(), false);
        delayTimeSlider->setValue(PAD_SETTINGS->getPadFxDelayTime(), false);
        delayTimeMenu->setSelectedId(PAD_SETTINGS->getPadFxDelayTimeMenu(), true);
        feedbackSlider->setValue(PAD_SETTINGS->getPadFxDelayFeedback(), false);
        lpfFrequencySlider->setValue(PAD_SETTINGS->getPadFxDelayLpfFreq(), false);
        hpfFrequencySlider->setValue(PAD_SETTINGS->getPadFxDelayHpfFreq(), false);
        syncButton->setToggleState(PAD_SETTINGS->getPadFxDelaySync(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxDelayAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxDelayAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxDelayAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        wetMixSlider->setValue(0.7, false);
        delayTimeSlider->setValue(500.0, false);
        delayTimeMenu->setSelectedId(3, true);
        feedbackSlider->setValue(0.5, false);
        lpfFrequencySlider->setValue(5000, false);
        hpfFrequencySlider->setValue(50, false);
        syncButton->setToggleState(1, false);
        
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, false);
    }
    
    
    if (syncButton->getToggleState() == 0) //off
        delayTimeSlider->setVisible(true);
    else //on
        delayTimeMenu->setVisible(true);
    
}

//need this?? not currently being used
void GuiDelay::setTempo (float value)
{
    tempo = value;
    //OR
    tempo = AppSettings::Instance()->getGlobalTempo();
}

void GuiDelay::mouseEnter (const MouseEvent &e)
{
    if (wetMixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Wet Mix Control. Sets and displays the delayed signal level for the selected pads."));
        parameterHoverLabel->setText(String(wetMixSlider->getValue(), 3), false);
    }
    else if (delayTimeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Delay Time Control. Sets the delay time in milliseconds for the selected pads. If you would like to set the delay time based on note length, click on the 'Sync' button."));
        parameterHoverLabel->setText(String(delayTimeSlider->getValue(), 2), false);
    }
    else if (delayTimeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Delay Time Menu. Sets the delay time in note length for the selected pads. If you would like to set the delay time in milliseconds, click on the 'Sync' button."));
    }
    else if (syncButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Tempo Sync Button. Turn this button on to sync the delay to the tempo, else you can set the tempo is milliseconds."));
    }
    else if (feedbackSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Delay Feedback Control. Sets the feedback level for the selected pad/pads. Please note that high values can create a delay that will get louder and never die out."));
        parameterHoverLabel->setText(String(feedbackSlider->getValue(), 3), false);
    }
    else if (lpfFrequencySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Low Pass Filter Frequency Control. Sets the LPF cutoff frequency for the selected pads. Please note that if this value is lower than the HPF cutoff frequency the delay signal will not be audible."));
        parameterHoverLabel->setText(String(lpfFrequencySlider->getValue(), 0), false);
    }
    else if (hpfFrequencySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("High Pass Filter Frequency Control. Sets the HPF cutoff frequency for the selected pads. Please note that if this value is higher than the LPF cutoff frequency the delay signal will not be audible."));
        parameterHoverLabel->setText(String(hpfFrequencySlider->getValue(), 0), false);
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

void GuiDelay::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, false);
    
}
