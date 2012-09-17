//
//  GuiEliteControlsSettings.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 17/09/2012.
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

#include "GuiEliteControlsSettings.h"
#include "../../File and Settings/AppSettings.h"
#include "../Views/MainComponent.h"


GuiEliteControlsSettings::GuiEliteControlsSettings(MainComponent &ref)
                                                : mainComponentRef(ref)
{
    addAndMakeVisible(dialsMenu = new ComboBox());
    
}

GuiEliteControlsSettings::~GuiEliteControlsSettings()
{
    
}

void GuiEliteControlsSettings::resized()
{
    dialsMenu->setBounds(802, 379, 100, 20);
}

void GuiEliteControlsSettings::paint (Graphics& g)
{
    
}

void GuiEliteControlsSettings::buttonClicked (Button* button)
{
    
}

void GuiEliteControlsSettings::sliderValueChanged (Slider* slider)
{
    
}

void GuiEliteControlsSettings::comboBoxChanged (ComboBox* comboBox)
{
    
}

void GuiEliteControlsSettings::setDisplay (int controlNumber)
{
    
}

void GuiEliteControlsSettings::mouseEnter (const MouseEvent &e)
{
    
}

void GuiEliteControlsSettings::mouseExit (const MouseEvent &e)
{
    
}