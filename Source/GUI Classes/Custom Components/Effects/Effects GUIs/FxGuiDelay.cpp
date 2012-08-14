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

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiDelay::GuiDelay(MainComponent &ref)
:           mainComponentRef(ref)
{
    addAndMakeVisible(wetMixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	wetMixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    wetMixSlider->setRange(0.0, 1.0);
    wetMixSlider->setValue(0.7, false);
    wetMixSlider->addListener(this);
    wetMixSlider->addMouseListener(this, true);
    
	addAndMakeVisible(feedbackSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	feedbackSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    feedbackSlider->setRange(0, 1.0);
    feedbackSlider->setValue(0.5, false);
    feedbackSlider->addListener(this);
    feedbackSlider->addMouseListener(this, true);
    
    addAndMakeVisible(delayTimeSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	delayTimeSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    delayTimeSlider->setRange(0.0, 4000.0);
    delayTimeSlider->setValue(500.0, false);
    delayTimeSlider->addListener(this);
    delayTimeSlider->addMouseListener(this, true);
    
    addAndMakeVisible(lpfFrequencySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	lpfFrequencySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    lpfFrequencySlider->setRange(30, 20000, 1);
    lpfFrequencySlider->setValue(20000, false);
    lpfFrequencySlider->addListener(this);
    lpfFrequencySlider->addMouseListener(this, true);
    
    addAndMakeVisible(hpfFrequencySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	hpfFrequencySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    hpfFrequencySlider->setRange(30, 20000);
    hpfFrequencySlider->setValue(30, false);
    hpfFrequencySlider->addListener(this);
    hpfFrequencySlider->addMouseListener(this, true);
    

    
    addAndMakeVisible(delayTimeMenu = new ComboBox());
    delayTimeMenu->addListener(this);
    delayTimeMenu->addMouseListener(this, true);
    delayTimeMenu->addItem("4 Beats", 1);
    delayTimeMenu->addItem("2 Beats", 2);
    delayTimeMenu->addItem("1 Beat", 3);
    delayTimeMenu->addItem("Half Beat", 4);
    delayTimeMenu->addItem("Quarter Beat", 5);
    delayTimeMenu->setSelectedId(4, true);
    
    addAndMakeVisible(syncButton = new AlphaTextButton("SYNC"));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);

    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem("Off", 1);
    alphaTouchMenu->addItem("Wet Mix", 2);
    alphaTouchMenu->addItem("Delay Time", 3);
    alphaTouchMenu->addItem("Feedback", 4);
    alphaTouchMenu->addItem("LPF Frequency", 5);
    alphaTouchMenu->addItem("HPF Frequency", 6);
    alphaTouchMenu->setSelectedId(1, true);
    
    addAndMakeVisible(reverseButton = new AlphaTextButton("INVERT"));
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
    
    alphaTouchMenu->setBounds(119, 192, 87, 20);
    reverseButton->setBounds(211,211, 32, 32);
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
            PAD_SETTINGS->setSamplerFxDelayMix(wetMixSlider->getValue());
        }
    }
    
    else if (slider == delayTimeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxDelayTime(delayTimeSlider->getValue());
        }
        
    }
    
    else if (slider == feedbackSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxDelayFeedback(feedbackSlider->getValue());
        }
        
    }
    
    else if (slider == lpfFrequencySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxDelayLpfFreq(lpfFrequencySlider->getValue());
        }
        
    }
    
    else if (slider == hpfFrequencySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxDelayHpfFreq(hpfFrequencySlider->getValue());
        }
        
    }
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxDelayAtIntensity(intensitySlider->getValue());
        }
        
    }
    

    
}

void GuiDelay::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxDelayAlphaTouch(alphaTouchMenu->getSelectedId());
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
            PAD_SETTINGS->setSamplerFxDelayTime(delayTime);
            PAD_SETTINGS->setSamplerFxDelayTimeMenu(delayTimeMenu->getSelectedId());
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
            PAD_SETTINGS->setSamplerFxDelayAtReverse(reverseButton->getToggleState());
        }
    }
    
    else if (button == syncButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxDelaySync(syncButton->getToggleState());
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
        wetMixSlider->setValue(PAD_SETTINGS->getSamplerFxDelayMix(), false);
        delayTimeSlider->setValue(PAD_SETTINGS->getSamplerFxDelayTime(), false);
        delayTimeMenu->setSelectedId(PAD_SETTINGS->getSamplerFxDelayTimeMenu(), true);
        feedbackSlider->setValue(PAD_SETTINGS->getSamplerFxDelayFeedback(), false);
        lpfFrequencySlider->setValue(PAD_SETTINGS->getSamplerFxDelayLpfFreq(), false);
        hpfFrequencySlider->setValue(PAD_SETTINGS->getSamplerFxDelayHpfFreq(), false);
        syncButton->setToggleState(PAD_SETTINGS->getSamplerFxDelaySync(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getSamplerFxDelayAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getSamplerFxDelayAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getSamplerFxDelayAtIntensity(), false);
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
        mainComponentRef.setInfoTextBoxText("Wet Mix Control. Sets the delayed signal level for the selected pad/pads.");
    }
    else if (delayTimeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Delay Time Control. Sets the delay time in milliseconds for the selected pad/pads. If you would like to set the delay time based on note length, click on the 'Tempo Sync' button.");
    }
    else if (delayTimeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Delay Time Menu. Sets the delay time in note length for the selected pad/pads. If you would like to set the delay time in milliseconds, click on the 'Tempo Sync' button.");
    }
    else if (syncButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Tempo Sync Button. Turn this button on to sync the delay to the tempo, else you can set the tempo is milliseconds.");
    }
    else if (feedbackSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Delay Feedback Control. Sets the feedback level for the selected pad/pads. Please note that high values can create a delay that will get louder and never die out.");
    }
    else if (lpfFrequencySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Low Pass Filter Frequency Control. Sets the LPF cutoff frequency for the selected pad/pads. Please note that if this value is lower than the HPF cutoff frequency the delay signal will not be audible.");
    }
    else if (hpfFrequencySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("High Pass Filter Frequency Control. Sets the HPF cutoff frequency for the selected pad/pads. Please note that if this value is higher than the LPF cutoff frequency the delay signal will not be audible.");
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

void GuiDelay::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}
