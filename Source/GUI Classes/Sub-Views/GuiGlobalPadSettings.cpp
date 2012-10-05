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

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)

GuiGlobalPadSettings::GuiGlobalPadSettings(MainComponent &ref)
: mainComponentRef(ref)

{
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
    exclusiveGroupSlider->setVisible(false);
    
    addAndMakeVisible(pressureSensitivityMenu = new ComboBox());
    pressureSensitivityMenu->addListener(this);
    pressureSensitivityMenu->addMouseListener(this, true);
    pressureSensitivityMenu->addItem(translate("Non-Sensitive"), 1);
    pressureSensitivityMenu->addItem(translate("Standard"), 2);
    pressureSensitivityMenu->addItem(translate("Sensitive"), 3);
    pressureSensitivityMenu->setSelectedId(2);
	
}

GuiGlobalPadSettings::~GuiGlobalPadSettings()
{
	
	deleteAllChildren();
}

void GuiGlobalPadSettings::resized()
{
	exclusiveModeButton->setBounds(823, 270, 42, 42);
    exclusiveGroupSlider->setBounds(823, 320, 42, 42);
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
        
    }
    else if(MULTI_PADS)
    {
        /*
        pressureSensitivityMenu->setSelectedId(2, true);
        exclusiveModeButton->setToggleState(false, false);
        exclusiveGroupSlider->setComponentValue(1);
         */
        
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
        mainComponentRef.setInfoTextBoxText(translate("Pressure Curve Menu. Use this menu to select the curve for the pressure."));
    }
}

void GuiGlobalPadSettings::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}

