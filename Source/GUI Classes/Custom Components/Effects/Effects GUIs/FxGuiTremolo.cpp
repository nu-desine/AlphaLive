//
//  FxGuiTremolo.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 13/07/2012.
//  Copyright 2012 nu desine.
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

#include "FxGuiTremolo.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiTremolo::GuiTremolo(MainComponent &ref)
:           mainComponentRef(ref)
{
    addAndMakeVisible(depthSlider = new AlphaImageKnob(2));
    depthSlider->sliderComponent()->setRange(0.0, 1.0);
    depthSlider->sliderComponent()->setValue(0.7, false);
    depthSlider->sliderComponent()->addListener(this);
    depthSlider->sliderComponent()->addMouseListener(this, true);
    
    addAndMakeVisible(rateSlider = new AlphaImageKnob(2));
    rateSlider->sliderComponent()->setRange(0.0, 50.0);
    rateSlider->sliderComponent()->setValue(5.0, false);
    rateSlider->sliderComponent()->addListener(this);
    rateSlider->sliderComponent()->addMouseListener(this, true);
    rateSlider->setVisible(false);
    
    addAndMakeVisible(rateMenu = new ComboBox());
    rateMenu->addListener(this);
    rateMenu->addMouseListener(this, true);
    rateMenu->addItem("1/1", 1);
    rateMenu->addItem("1/2", 2);
    rateMenu->addItem("1/4", 3);
    rateMenu->addItem("1/8", 4);
    rateMenu->addItem("1/16", 5);
    rateMenu->addItem("1/32", 6);
    rateMenu->addItem("1/64", 7);
    //what about other values, such as the d's and t's in Logic's tremolo?
    rateMenu->setSelectedId(3, true);
    
    addAndMakeVisible(syncButton = new TextButton("sync", "Tempo Sync"));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);
    
    addAndMakeVisible(shapeMenu = new ComboBox());
    shapeMenu->addListener(this);
    shapeMenu->addMouseListener(this, true);
    shapeMenu->addItem("Sine", 1);
    shapeMenu->addItem("Square", 2);
    shapeMenu->addItem("Triangle", 3);
    shapeMenu->addItem("Rising Sawtooth", 4);
    shapeMenu->addItem("Falling Sawtooth", 5);
    shapeMenu->setSelectedId(1, true);
    
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem("Off", 1);
    alphaTouchMenu->addItem("Depth/Mix", 2);
    alphaTouchMenu->addItem("Rate", 3);
    alphaTouchMenu->addItem("Wave Shape", 4);
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

GuiTremolo::~GuiTremolo()
{
    deleteAllChildren();
}



void GuiTremolo::resized()
{
    depthSlider->setBounds(getWidth()/2-(40/2), 40, 40, 40);
    rateSlider->setBounds((getWidth()/3)-35, 70, 40, 40);
    rateMenu->setBounds((getWidth()/3)-35, 85, 50, 15);
    syncButton->setBounds(((getWidth()/2)-(35/2))-3, 85, 40, 15);
    shapeMenu->setBounds(((getWidth()/3)*2)-10, 85, 50, 15);
    
    alphaTouchMenu->setBounds(65, 125, 100, 15);
    reverseButton->setBounds(68, 155, 40, 20);
    intensitySlider->setBounds(120, 145, 40, 40);
}



void GuiTremolo::sliderValueChanged (Slider *slider)
{
    if (slider == depthSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxTremoloDepth(depthSlider->sliderComponent()->getValue());
        }
        
    }
    
    else if (slider == rateSlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxTremoloRate(rateSlider->sliderComponent()->getValue());
        }
        
    }
    
    
    else if (slider == intensitySlider->sliderComponent())
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxTremoloAtIntensity(intensitySlider->sliderComponent()->getValue());
        }
        
    }
    
    
    
}

void GuiTremolo::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxTremoloAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
    }
    
    else if (comboBox == shapeMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxTremoloShape(shapeMenu->getSelectedId());
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
                
            case 1: // 1/1 - 4 beats
                lfoRate = bps * 0.25;
                break;
            case 2: // 1/2 - 2 beats
                lfoRate = bps * 0.5;
                break;
            case 3: // 1/4 - 1 beat
                lfoRate = bps * 1.0;
                break;
            case 4: // 1/8 - half beat
                lfoRate = bps * 2.0;
                break;
            case 5: // 1/16 - quarter beat
                lfoRate = bps * 4.0;
                break;
            case 6: // 1/32 - 8th beat
                lfoRate = bps * 8.0;
                break;
            case 7: // 1/64 - 16th beat
                lfoRate = bps * 16.0;
                break;
            default: // 1/4 - 1 beat
                lfoRate = bps * 1.0;
                break;
                 
        }
        
        rateSlider->sliderComponent()->setValue(lfoRate, false);
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxTremoloRate(lfoRate);
            PAD_SETTINGS->setSamplerFxTremoloRateMenu(rateMenu->getSelectedId());
        }
        
    }
    
}


void GuiTremolo::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxTremoloAtReverse(reverseButton->getToggleState());
        }
        
    }
    
    else if (button == syncButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerFxTremoloSync(syncButton->getToggleState());
        }
        
        rateSlider->setVisible(false);
        rateMenu->setVisible(false);
        
        if (syncButton->getToggleState() == 0) //off
            rateSlider->setVisible(true);
        else //on
            rateMenu->setVisible(true);
        
    }
}

void GuiTremolo::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiTremolo::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    rateSlider->setVisible(false);
    rateMenu->setVisible(false);
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        depthSlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxTremoloDepth(), false);
        rateSlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxTremoloRate(), false);
        rateMenu->setSelectedId(PAD_SETTINGS->getSamplerFxTremoloRateMenu(), true);
        syncButton->setToggleState(PAD_SETTINGS->getSamplerFxTremoloSync(), false);
        shapeMenu->setSelectedId(PAD_SETTINGS->getSamplerFxTremoloShape(), true);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getSamplerFxTremoloAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getSamplerFxTremoloAtReverse(), false);
        intensitySlider->sliderComponent()->setValue(PAD_SETTINGS->getSamplerFxTremoloAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        depthSlider->sliderComponent()->setValue(0.7, false);
        rateSlider->sliderComponent()->setValue(5.0, false);
        rateMenu->setSelectedId(3, true);
        syncButton->setToggleState(true, false);
        shapeMenu->setSelectedId(2, true);
        
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->sliderComponent()->setValue(1.0, false);
    }
    
    
    if (syncButton->getToggleState() == 0) //off
        rateSlider->setVisible(true);
    else //on
        rateMenu->setVisible(true);
    
}

//need this?? not currently being used
void GuiTremolo::setTempo (float value)
{
    tempo = value;
    //OR
    tempo = AppSettings::Instance()->getGlobalTempo();
}

void GuiTremolo::mouseEnter (const MouseEvent &e)
{
    if (depthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Depth/Mix Control. Sets the depth/mix of the LFO for the selected pad/pads.");
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
    else if (shapeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Waveshape Menu. Sets the LFO waveshape for the selected pad/pads.");
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

void GuiTremolo::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}
