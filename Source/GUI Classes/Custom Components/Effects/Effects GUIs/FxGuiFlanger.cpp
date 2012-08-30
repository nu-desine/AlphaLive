//
//  FxGuiFlanger.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 14/07/2012.
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

#include "FxGuiFlanger.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/BinaryDataNew.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiFlanger::GuiFlanger(MainComponent &ref)
:           mainComponentRef(ref)
{
    addAndMakeVisible(mixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	mixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    mixSlider->setRange(0.0, 1.0);
    mixSlider->setValue(0.7, false);
    mixSlider->addListener(this);
    mixSlider->addMouseListener(this, true);
    
	addAndMakeVisible(rateSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	rateSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    rateSlider->setRange(0, 20.0);
    rateSlider->setValue(0.25, false);
    rateSlider->addListener(this);
    rateSlider->addMouseListener(this, true);
    
    addAndMakeVisible(feedbackSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	feedbackSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    feedbackSlider->setRange(0.0, 1.0);
    feedbackSlider->setValue(0.9, false);
    feedbackSlider->addListener(this);
    feedbackSlider->addMouseListener(this, true);
    
    addAndMakeVisible(flangerIntensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190)); //LFO depth
	flangerIntensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    flangerIntensitySlider->setRange(0.0, 1.0);
    flangerIntensitySlider->setValue(0.1, false);
    flangerIntensitySlider->addListener(this);
    flangerIntensitySlider->addMouseListener(this, true);
    
    
    addAndMakeVisible(rateMenu = new ComboBox());
    rateMenu->addListener(this);
    rateMenu->addMouseListener(this, true);
    rateMenu->addItem("8 Bars", 1);
    rateMenu->addItem("4 Bars", 2);
    rateMenu->addItem("2 Bars", 3);
    rateMenu->addItem("1 Bar", 4);
    rateMenu->addItem("1/2", 5);
    rateMenu->addItem("1/4", 6);
    rateMenu->addItem("1/8", 7);
    rateMenu->addItem("1/16", 8);
    rateMenu->addItem("1/32", 9);
    rateMenu->setSelectedId(4, true);
    
    addAndMakeVisible(syncButton = new AlphaTextButton("SYNC"));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);
    
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem("Off", 1);
    alphaTouchMenu->addItem("Mix", 2);
    alphaTouchMenu->addItem("Rate", 3);
    alphaTouchMenu->addItem("Feedback", 4);
    alphaTouchMenu->addItem("Intensity", 5);
    alphaTouchMenu->setSelectedId(1, true);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::inverticon_png, BinaryDataNew::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
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

    tempo = AppSettings::Instance()->getGlobalTempo();
    
    setInterceptsMouseClicks(false, true);
}

GuiFlanger::~GuiFlanger()
{
    deleteAllChildren();
}



void GuiFlanger::resized()
{
    
    mixSlider->setBounds(97, 97, 130, 130);
    rateSlider->setBounds(87, 87, 150, 150);
    feedbackSlider->setBounds(77, 77, 170, 170);
	flangerIntensitySlider->setBounds(67, 67, 190, 190);
    
    alphaTouchMenu->setBounds(119, 192, 87, 20);
    reverseButton->setBounds(211,211, 32, 32);
    intensitySlider->setBounds(57, 57, 210, 210);
    
    rateMenu->setBounds(119, 22, 87, 20);
    syncButton->setBounds(130, 235, 32, 32);
}



void GuiFlanger::sliderValueChanged (Slider *slider)
{
    if (slider == mixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerMix(mixSlider->getValue());
        }
        
    }

    else if (slider == rateSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerRate(rateSlider->getValue());
        }
        
    }
    
    
    else if (slider == feedbackSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerFeedback(feedbackSlider->getValue());
        }
    }
    
    
    else if (slider == flangerIntensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerIntensity(flangerIntensitySlider->getValue());
        }
        
    }
    
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerAtIntensity(intensitySlider->getValue());
        }
    }
    
    
    
}

void GuiFlanger::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
    }
    
    else if (comboBox == rateMenu)
    {
        //work out LFO rate based on rate selected and the tempo.
        tempo = AppSettings::Instance()->getGlobalTempo();
        double bps = tempo/60.0;
        double lfoRate;
        
        switch (rateMenu->getSelectedId())
        {
            case 1: // 8 bars
                lfoRate = bps * 0.03125;
                break;
            case 2: // 4 bars
                lfoRate = bps * 0.0625;
                break;
            case 3: // 2 bars
                lfoRate = bps * 0.125;
                break;
            case 4: // 1 bar - 4 beats
                lfoRate = bps * 0.25;
                break;
            case 5: // 1/2 - 2 beats
                lfoRate = bps * 0.5;
                break;
            case 6: // 1/4 - 1 beat
                lfoRate = bps * 1.0;
                break;
            case 7: // 1/8 - half beat
                lfoRate = bps * 2.0;
                break;
            case 8: // 1/16 - quarter beat
                lfoRate = bps * 4.0;
                break;
            case 9: // 1/32 - 8th beat
                lfoRate = bps * 8.0;
                break;
            default: // 1/4 - 1 beat
                lfoRate = bps * 1.0;
                break;
                
        }
         
        
        rateSlider->setValue(lfoRate, false);
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerRate(lfoRate);
            PAD_SETTINGS->setPadFxFlangerRateMenu(rateMenu->getSelectedId());
        }
        
    }
    
}


void GuiFlanger::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerAtReverse(reverseButton->getToggleState());
        }
    }
    
    else if (button == syncButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerSync(syncButton->getToggleState());
        }
        
        
        rateSlider->setVisible(false);
        rateMenu->setVisible(false);
        
        if (syncButton->getToggleState() == 0) //off
            rateSlider->setVisible(true);
        else //on
            rateMenu->setVisible(true);
        
    }
}

void GuiFlanger::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiFlanger::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    rateSlider->setVisible(false);
    rateMenu->setVisible(false);
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        mixSlider->setValue(PAD_SETTINGS->getPadFxFlangerMix(), false);
        rateSlider->setValue(PAD_SETTINGS->getPadFxFlangerRate(), false);
        feedbackSlider->setValue(PAD_SETTINGS->getPadFxFlangerFeedback(), false);
        flangerIntensitySlider->setValue(PAD_SETTINGS->getPadFxFlangerIntensity(), false);
        rateMenu->setSelectedId(PAD_SETTINGS->getPadFxFlangerRateMenu(), true);
        syncButton->setToggleState(PAD_SETTINGS->getPadFxFlangerSync(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxFlangerAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxFlangerAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxFlangerAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        mixSlider->setValue(0.7, false);
        rateSlider->setValue(0.25, false);
        feedbackSlider->setValue(0.9, false);
        flangerIntensitySlider->setValue(0.1, false);
        rateMenu->setSelectedId(3, true); //??????????????????
        syncButton->setToggleState(1, false);
        
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, false);
    }
    
    
    if (syncButton->getToggleState() == 0) //off
        rateSlider->setVisible(true);
    else //on
        rateMenu->setVisible(true);
    
}

//need this?? not currently being used
void GuiFlanger::setTempo (float value)
{
    tempo = value;
    //OR
    tempo = AppSettings::Instance()->getGlobalTempo();
}

void GuiFlanger::mouseEnter (const MouseEvent &e)
{
    if (mixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Mix Control. Sets the mix of the Flanger effect for the selected pad/pads.");
    }
    else if (rateSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("LFO Rate Control. Sets the rate in Hz for the selected pad/pads. If you would like to set the rate based on the tempo, click on the 'Tempo Sync' button.");
    }
    else if (rateMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("LFO Rate Menu. Sets the LFO rate for the selected pad/pads. If you would like to set the rate in Hz, click on the 'Tempo Sync' button.");
    }
    else if (syncButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Tempo Sync Button. Turn this button on to sync the LFO to the tempo, else you can set the LFO is Hz.");
    }
    else if (feedbackSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Feedback Selector. Sets the flanger feedback for the selected pad/pads.");
    }
    else if (flangerIntensitySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Flanger Intensity Selector. Sets the intensity of the effecy for the selected pad/pads.");
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

void GuiFlanger::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}
