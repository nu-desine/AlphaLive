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
    addAndMakeVisible(wetMixSlider = new AlphaImageKnob(2));
    wetMixSlider->sliderComponent()->setRange(0.0, 1.0);
    wetMixSlider->sliderComponent()->setValue(0.7, false);
    wetMixSlider->sliderComponent()->addListener(this);
    wetMixSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(feedbackSlider = new AlphaImageKnob(2));
    feedbackSlider->sliderComponent()->setRange(0.0, 1.0);
    feedbackSlider->sliderComponent()->setValue(0.5, false);
    feedbackSlider->sliderComponent()->addListener(this);
    feedbackSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(delayTimeSlider = new AlphaImageKnob(1)); //how many decimal points?
    delayTimeSlider->sliderComponent()->setRange(0.0, 4000.0);
    delayTimeSlider->sliderComponent()->setValue(500.0, false);
    delayTimeSlider->sliderComponent()->addListener(this);
    delayTimeSlider->sliderComponent()->addMouseListener(this, true);
    delayTimeSlider->setVisible(false);
    
    addAndMakeVisible(delayTimeMenu = new ComboBox());
    delayTimeMenu->addListener(this);
    delayTimeMenu->addMouseListener(this, true);
    delayTimeMenu->addItem("4 Beats", 1);
    delayTimeMenu->addItem("2 Beats", 2);
    delayTimeMenu->addItem("1 Beat", 3);
    delayTimeMenu->addItem("Half Beat", 4);
    delayTimeMenu->addItem("Quarter Beat", 5);
    delayTimeMenu->setSelectedId(3, true);
    
    addAndMakeVisible(syncButton = new TextButton("sync", "Tempo Sync"));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);
    
    addAndMakeVisible(lpfFrequencySlider = new AlphaImageKnob(0));
    lpfFrequencySlider->sliderComponent()->setRange(30, 20000, 1);
    lpfFrequencySlider->sliderComponent()->setValue(20000, false);
    lpfFrequencySlider->sliderComponent()->addListener(this);
    lpfFrequencySlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(hpfFrequencySlider = new AlphaImageKnob(0));
    hpfFrequencySlider->sliderComponent()->setRange(30, 20000, 1);
    hpfFrequencySlider->sliderComponent()->setValue(30, false);
    hpfFrequencySlider->sliderComponent()->addListener(this);
    hpfFrequencySlider->sliderComponent()->addMouseListener(this, true);
    
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
    tempo = AppSettings::Instance()->getGlobalTempo();
    
    setInterceptsMouseClicks(false, true);
}

GuiDelay::~GuiDelay()
{
    deleteAllChildren();
}



void GuiDelay::resized()
{
    feedbackSlider->setBounds(getWidth()/3, 40, 35, 35);
    lpfFrequencySlider->setBounds(((getWidth()/3)*2)-35, 40, 35, 35);
    wetMixSlider->setBounds((getWidth()/3)-35, 70, 35, 35);
    hpfFrequencySlider->setBounds((getWidth()/3)*2, 70, 35, 35);
    delayTimeSlider->setBounds((getWidth()/2)-(35/2), 70, 35, 35);
    delayTimeMenu->setBounds(((getWidth()/2)-(35/2))-10, 80, 50, 15);
    syncButton->setBounds(((getWidth()/2)-(35/2))-3, 105, 40, 15);
    
    alphaTouchMenu->setBounds(65, 125, 100, 15);
    reverseButton->setBounds(68, 155, 40, 20);
    intensitySlider->setBounds(120, 145, 40, 40);
}



void GuiDelay::sliderValueChanged (Slider *slider)
{
    if (slider == wetMixSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperFxDelayMix(wetMixSlider->sliderComponent()->getValue());
        }
    }
    
    else if (slider == delayTimeSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperFxDelayTime(delayTimeSlider->sliderComponent()->getValue());
        }
        
    }
    
    else if (slider == feedbackSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperFxDelayFeedback(feedbackSlider->sliderComponent()->getValue());
        }
        
    }
    
    else if (slider == lpfFrequencySlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperFxDelayLpfFreq(lpfFrequencySlider->sliderComponent()->getValue());
        }
        
    }
    
    else if (slider == hpfFrequencySlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperFxDelayHpfFreq(hpfFrequencySlider->sliderComponent()->getValue());
        }
        
    }
    
    else if (slider == intensitySlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperFxDelayAtIntensity(intensitySlider->sliderComponent()->getValue());
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
            PAD_SETTINGS->setLooperFxDelayAlphaTouch(alphaTouchMenu->getSelectedId());
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
        
        delayTimeSlider->sliderComponent()->setValue(delayTime, false);
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperFxDelayTime(delayTime);
            PAD_SETTINGS->setLooperFxDelayTimeMenu(delayTimeMenu->getSelectedId());
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
            PAD_SETTINGS->setLooperFxDelayAtReverse(reverseButton->getToggleState());
        }
    }
    
    else if (button == syncButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setLooperFxDelaySync(syncButton->getToggleState());
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
        wetMixSlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxDelayMix(), false);
        delayTimeSlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxDelayTime(), false);
        delayTimeMenu->setSelectedId(PAD_SETTINGS->getLooperFxDelayTimeMenu(), true);
        feedbackSlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxDelayFeedback(), false);
        lpfFrequencySlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxDelayLpfFreq(), false);
        hpfFrequencySlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxDelayHpfFreq(), false);
        syncButton->setToggleState(PAD_SETTINGS->getLooperFxDelaySync(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getLooperFxDelayAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getLooperFxDelayAtReverse(), false);
        intensitySlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxDelayAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        wetMixSlider->sliderComponent()->setValue(0.7, false);
        delayTimeSlider->sliderComponent()->setValue(500.0, false);
        delayTimeMenu->setSelectedId(3, true);
        feedbackSlider->sliderComponent()->setValue(0.5, false);
        lpfFrequencySlider->sliderComponent()->setValue(5000, false);
        hpfFrequencySlider->sliderComponent()->setValue(50, false);
        syncButton->setToggleState(1, false);
        
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->sliderComponent()->setValue(1.0, false);
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
