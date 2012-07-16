//
//  FxGuiBandPassFilter.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 20/12/2011.
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

#include "FxGuiBandPassFilter.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[currentlySelectedPad]
#define PAD_SETTINGS_i AppSettings::Instance()->padSettings[i]


GuiBandPassFilter::GuiBandPassFilter(MainComponent &ref)
: mainComponentRef(ref)
{
    addAndMakeVisible(mixSlider = new AlphaImageKnob(2));
    mixSlider->sliderComponent()->setRange(0.0, 1.0);
    mixSlider->sliderComponent()->setValue(1.0, false);
    mixSlider->sliderComponent()->addListener(this);
    mixSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(frequencySlider = new AlphaImageKnob(0));
    frequencySlider->sliderComponent()->setRange(30, 20000, 1);
    frequencySlider->sliderComponent()->setValue(1000, false);
    frequencySlider->sliderComponent()->addListener(this);
    frequencySlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(bandwidthSlider = new AlphaImageKnob(2));
    bandwidthSlider->sliderComponent()->setRange(1.0, 100.0);
    bandwidthSlider->sliderComponent()->setValue(5, false);
    bandwidthSlider->sliderComponent()->addListener(this);
    bandwidthSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem("Off", 1);
    alphaTouchMenu->addItem("Mix", 2);
    alphaTouchMenu->addItem("Cut-Off Frequency", 3);
    alphaTouchMenu->addItem("Bandwidth/Resonance", 4);
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
    
    currentlySelectedPad = 99;
    
    setInterceptsMouseClicks(false, true);
}

GuiBandPassFilter::~GuiBandPassFilter()
{
    deleteAllChildren();
}



void GuiBandPassFilter::resized()
{
    mixSlider->setBounds(getWidth()/2-(40/2), 40, 40, 40);
    frequencySlider->setBounds(65, 70, 40, 40);
    bandwidthSlider->setBounds(128, 70, 40, 40);
    
    alphaTouchMenu->setBounds(65, 125, 100, 15);
    reverseButton->setBounds(68, 155, 40, 20);
    intensitySlider->setBounds(120, 145, 40, 40);
}



void GuiBandPassFilter::sliderValueChanged (Slider *slider)
{
    if (slider == mixSlider->sliderComponent())
    {
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            PAD_SETTINGS->setLooperFxBpfMix(mixSlider->sliderComponent()->getValue());
        }
        
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setLooperFxBpfMix(mixSlider->sliderComponent()->getValue());
            }
        }
        
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int row = currentlySelectedPad - 99; 
            
            for(int i = (row*8)-8; i <= (row*8)-1; i++) 
            {
                PAD_SETTINGS_i->setLooperFxBpfMix(mixSlider->sliderComponent()->getValue());
            }
        }
    }
    
    
    if (slider == frequencySlider->sliderComponent())
    {
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            PAD_SETTINGS->setLooperFxBpfFreq(frequencySlider->sliderComponent()->getValue());
        }
        
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setLooperFxBpfFreq(frequencySlider->sliderComponent()->getValue());
            }
        }
        
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int row = currentlySelectedPad - 99; 
            
            for(int i = (row*8)-8; i <= (row*8)-1; i++) 
            {
                PAD_SETTINGS_i->setLooperFxBpfFreq(frequencySlider->sliderComponent()->getValue());
            }
        }
    }
    
    
    if (slider == bandwidthSlider->sliderComponent())
    {
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            PAD_SETTINGS->setLooperFxBpfBandwidth(bandwidthSlider->sliderComponent()->getValue());
        }
        
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setLooperFxBpfBandwidth(bandwidthSlider->sliderComponent()->getValue());
            }
        }
        
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int row = currentlySelectedPad - 99; 
            
            for(int i = (row*8)-8; i <= (row*8)-1; i++) 
            {
                PAD_SETTINGS_i->setLooperFxBpfBandwidth(bandwidthSlider->sliderComponent()->getValue());
            }
        }
    }
    
    
    if (slider == intensitySlider->sliderComponent())
    {
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            PAD_SETTINGS->setLooperFxBpfAtIntensity(intensitySlider->sliderComponent()->getValue());
        }
        
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setLooperFxBpfAtIntensity(intensitySlider->sliderComponent()->getValue());
            }
        }
        
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int row = currentlySelectedPad - 99; 
            
            for(int i = (row*8)-8; i <= (row*8)-1; i++) 
            {
                PAD_SETTINGS_i->setLooperFxBpfAtIntensity(intensitySlider->sliderComponent()->getValue());
            }
        }
    }
    
    
}

void GuiBandPassFilter::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            PAD_SETTINGS->setLooperFxBpfAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setLooperFxBpfAlphaTouch(alphaTouchMenu->getSelectedId());
            }
        }
        
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int row = currentlySelectedPad - 99; 
            
            for(int i = (row*8)-8; i <= (row*8)-1; i++) 
            {
                PAD_SETTINGS_i->setLooperFxBpfAlphaTouch(alphaTouchMenu->getSelectedId());
            }
        }
    }
    
}


void GuiBandPassFilter::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        //if individual pad number is selected
        if(currentlySelectedPad < 99)
        {
            PAD_SETTINGS->setLooperFxBpfAtReverse(reverseButton->getToggleState());
        }
        
        //if 'all pads' selected
        if(currentlySelectedPad == 99)
        {
            for(int i = 0; i <= 47; i++)
            {
                PAD_SETTINGS_i->setLooperFxBpfAtReverse(reverseButton->getToggleState());
            }
        }
        
        //if a 'row' is selected
        if(currentlySelectedPad > 99)
        {
            int row = currentlySelectedPad - 99; 
            
            for(int i = (row*8)-8; i <= (row*8)-1; i++) 
            {
                PAD_SETTINGS_i->setLooperFxBpfAtReverse(reverseButton->getToggleState());
            }
        }
        
    }
}

void GuiBandPassFilter::setCurrentlySelectedPad (int padNumber)
{
    currentlySelectedPad = padNumber;
}

void GuiBandPassFilter::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    
    //if an individual pad number is currently selected
    if(currentlySelectedPad < 99)
    {
        mixSlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxBpfMix(), false);
        frequencySlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxBpfFreq(), false);
        bandwidthSlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxBpfBandwidth(), false);
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getLooperFxBpfAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getLooperFxBpfAtReverse(), false);
        intensitySlider->sliderComponent()->setValue(PAD_SETTINGS->getLooperFxBpfAtIntensity(), false);
    }
    
    //if 'all pads' selected
    if(currentlySelectedPad == 99)
    {
        mixSlider->sliderComponent()->setValue(1.0, false);
        frequencySlider->sliderComponent()->setValue(1000, false);
        bandwidthSlider->sliderComponent()->setValue(5.0, false);
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->sliderComponent()->setValue(1.0, false);
    }
    
    //if a 'row' is selected
    if(currentlySelectedPad > 99)
    {
        mixSlider->sliderComponent()->setValue(1.0, false);
        frequencySlider->sliderComponent()->setValue(1000, false);
        bandwidthSlider->sliderComponent()->setValue(5.0, false);
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->sliderComponent()->setValue(1.0, false);
    }
    
}

void GuiBandPassFilter::mouseEnter (const MouseEvent &e)
{
    if (mixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Band-Pass Filter Mix Control. Sets the wet/dry ratio for Band-Pass filter for the selected pad/pads.");
    }
    else if (frequencySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Band-Pass Filter Centre Frequency Control. Sets the centre frequency for Band-Pass filter for the selected pad/pads.");
    }
    else if (bandwidthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Band-Pass Filter Bandwidth Control. Sets the bandwidth for Band-Pass filter for the selected pad/pads.");
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

void GuiBandPassFilter::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}