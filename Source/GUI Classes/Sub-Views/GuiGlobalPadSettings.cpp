/*
 *  GuiGlobalPadSettings.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 20/06/2012.
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
 */

#include "GuiGlobalPadSettings.h"
#include "../../File and Settings/AppSettings.h"
#include "../Views/MainComponent.h"
#include "../Binary Data/BinaryDataNew.h"
#include "../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)

GuiGlobalPadSettings::GuiGlobalPadSettings(MainComponent &ref)
: mainComponentRef(ref)

{
	Image *quantiseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::quantiseicon_png, BinaryDataNew::quantiseicon_pngSize));
	addAndMakeVisible(quantiseButton = new ModeButton(quantiseIcon));
	quantiseButton->setClickingTogglesState(true);
	quantiseButton->setToggleState(false, false);	
	quantiseButton->addListener(this);
	quantiseButton->addMouseListener(this, true);
    
	addAndMakeVisible(exclusiveModeButton = new TextButton("Exc Mode"));
    exclusiveModeButton->addListener(this);
    exclusiveModeButton->addMouseListener(this, true);
    exclusiveModeButton->setClickingTogglesState(true);
    exclusiveModeButton->setToggleState(0, false);
    
    addChildComponent(exclusiveGroupSlider = new AlphaSlider());
    exclusiveGroupSlider->setRange(1, 24, 1);
    exclusiveGroupSlider->addListener(this);
    exclusiveGroupSlider->setValue(1, false);
    exclusiveGroupSlider->addMouseListener(this, true);
    
    addAndMakeVisible(velocitySlider = new AlphaSlider());
    velocitySlider->setRange(0, 127, 1);
    velocitySlider->addListener(this);
    velocitySlider->setValue(110, false);
    velocitySlider->addMouseListener(this, true);
    
    addAndMakeVisible(pressureSensitivityMenu = new ComboBox());
    pressureSensitivityMenu->addListener(this);
    pressureSensitivityMenu->addMouseListener(this, true);
    pressureSensitivityMenu->addItem(translate("Exponential"), 1);
    pressureSensitivityMenu->addItem(translate("Linear"), 2);
    pressureSensitivityMenu->addItem(translate("Logarithmic"), 3);
    pressureSensitivityMenu->setSelectedId(2);
	
}

GuiGlobalPadSettings::~GuiGlobalPadSettings()
{
	
	deleteAllChildren();
}

void GuiGlobalPadSettings::resized()
{
    quantiseButton->setBounds(828, 270, 32, 32); //should this be in the same position as the other quantise buttons to prevent confusion?
	exclusiveModeButton->setBounds(823, 320, 42, 42);
    exclusiveGroupSlider->setBounds(823, 370, 42, 42);
    
    velocitySlider->setBounds(823, 500, 42, 42);
	pressureSensitivityMenu->setBounds(802, 550, 87, 20);
	
}

void GuiGlobalPadSettings::paint (Graphics& g)
{
	
	
}

void GuiGlobalPadSettings::setCurrentlySelectedPad (Array<int> selectedPads_)
{
	
	selectedPads = selectedPads_;
}

void GuiGlobalPadSettings::buttonClicked (Button* button)
{
    if (button == exclusiveModeButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setExclusiveMode(button->getToggleState());
        }
        
        if (button->getToggleState() == true)
            exclusiveGroupSlider->setVisible(true);
        else
            exclusiveGroupSlider->setVisible(false); 
    }
    
    else if(button == quantiseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setQuantizeMode(button->getToggleState());
        }
    }
        
}

void GuiGlobalPadSettings::sliderValueChanged (Slider* slider)
{
    if (slider == exclusiveGroupSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setExclusiveGroup(slider->getValue());
        }
    }
    else if (slider == velocitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setVelocity(slider->getValue());
        }
    }
}

void GuiGlobalPadSettings::comboBoxChanged (ComboBox* comboBox)
{
    if (comboBox == pressureSensitivityMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPressureSensitivityMode(pressureSensitivityMenu->getSelectedId());
        }
    }
}

void GuiGlobalPadSettings::updateDisplay()
{
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        pressureSensitivityMenu->setSelectedId(PAD_SETTINGS->getPressureSensitivityMode(), true);
        exclusiveModeButton->setToggleState(PAD_SETTINGS->getExclusiveMode(), false);
        exclusiveGroupSlider->setComponentValue(PAD_SETTINGS->getExclusiveGroup());
        quantiseButton->setToggleState(PAD_SETTINGS->getQuantizeMode(), false);
        velocitySlider->setComponentValue(PAD_SETTINGS->getVelocity());
        
    }
    else if(MULTI_PADS)
    {
        
        //==================================================================================================
        int quantiseMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getQuantizeMode();
        //loop through all selected pads expect for the first one
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            //if setting of this pad does NOT match setting of last pad, set default and break
            if (PAD_SETTINGS->getQuantizeMode() != quantiseMode_)
            {
                quantiseButton->setToggleState(0, false);
                break;
            }
            //if this is the last 'natural' iteraction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
                quantiseButton->setToggleState(quantiseMode_, false);
        }
        //==================================================================================================
        int exclusiveMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getExclusiveMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getExclusiveMode() != exclusiveMode_)
            {
                exclusiveModeButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                exclusiveModeButton->setToggleState(exclusiveMode_, false);
        }
        
        //==================================================================================================
        int exclusiveGroup_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getExclusiveGroup();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getExclusiveGroup() != exclusiveGroup_)
            {
                exclusiveGroupSlider->setComponentValue(-999);
                break;
            }
            if (i == selectedPads.size()-1)
                exclusiveGroupSlider->setComponentValue(exclusiveGroup_);
        }
        
        //==================================================================================================
        int pressureSensitivity_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getPressureSensitivityMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getPressureSensitivityMode() != pressureSensitivity_)
            {
                pressureSensitivityMenu->setSelectedId(2, true);
                break;
            }
            if (i == selectedPads.size()-1)
                pressureSensitivityMenu->setSelectedId(pressureSensitivity_, true);
        }
        
        //==================================================================================================
        int velocity_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getVelocity();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getVelocity() != velocity_)
            {
                velocitySlider->setComponentValue(-999);
                break;
            }
            if (i == selectedPads.size()-1)
                velocitySlider->setComponentValue(velocity_);
        }
        
    }
    
    if (exclusiveModeButton->getToggleState() == true)
        exclusiveGroupSlider->setVisible(true);
    else
        exclusiveGroupSlider->setVisible(false);
}


void GuiGlobalPadSettings::mouseEnter (const MouseEvent &e)
{
    if (exclusiveModeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Exclusive Mode Button. If this button is set to 'on' the selected pads will be set to 'Exclusive Mode' - only a single pad can be playing at any time for each exclusive group."));
    }
    else if (exclusiveGroupSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Exclusive Group Selector. Sets and displays the exclusive group number for the selected pads."));
        
    }
    else if (pressureSensitivityMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Pressure Curve Menu. Use this menu to select the curve for the pressure mapping."));
    }
    else if (quantiseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::quantizeButton));
    }
    else if (velocitySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Static Velocity Selector. Sets and displays the static velocity for a MIDI note or sample."));
    }
}

void GuiGlobalPadSettings::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}

