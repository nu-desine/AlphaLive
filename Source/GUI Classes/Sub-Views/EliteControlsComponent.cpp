//
//  EliteControlsComponent.cpp
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

#include "EliteControlsComponent.h"
#include "../Views/MainComponent.h"
#include "../AlphaLiveLookandFeel.h"

enum
{
    DIAL_1 = 1,
    DIAL_2,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
};

EliteControlsComponent::EliteControlsComponent(MainComponent &ref)
        : mainComponentRef(ref)
{
    Path dialShape, buttonShape;
    dialShape.addEllipse (0, 0, 50, 50);
    buttonShape.addRectangle(0, 0, 30, 30);
    
    addAndMakeVisible (dial1Button = new ShapeButton("Dial 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    dial1Button->setShape(dialShape, true, true, false);
    dial1Button->addListener(this);
    dial1Button->addMouseListener(this, true);
    addAndMakeVisible (dial2Button = new ShapeButton("Dial 2", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    dial2Button->setShape(dialShape, true, true, false);
    dial2Button->addListener(this);
    dial2Button->addMouseListener(this, true);
    
    addAndMakeVisible (button1Button = new ShapeButton("Button 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    button1Button->setShape(buttonShape, true, true, false);
    button1Button->addListener(this);
    button1Button->addMouseListener(this, true);
    addAndMakeVisible (button2Button = new ShapeButton("Button 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    button2Button->setShape(buttonShape, true, true, false);
    button2Button->addListener(this);
    button2Button->addMouseListener(this, true);
    addAndMakeVisible (button3Button = new ShapeButton("Button 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    button3Button->setShape(buttonShape, true, true, false);
    button3Button->addListener(this);
    button3Button->addMouseListener(this, true);
    
}

EliteControlsComponent::~EliteControlsComponent()
{

}

void EliteControlsComponent::resized()
{
    dial1Button->setBounds(4, 0, 30, 30);
    dial2Button->setBounds(45, 0, 30, 30);
    
    button1Button->setBounds(0, 40, 20, 20);
    button2Button->setBounds(30, 40, 20, 20);
    button3Button->setBounds(60, 40, 20, 20);
}

void EliteControlsComponent::paint (Graphics& g)
{
    
}

void EliteControlsComponent::buttonClicked (Button* button)
{
    int clickedControl = 0;
    
    if (button == dial1Button)
        clickedControl = DIAL_1;
    else if (button == dial2Button)
        clickedControl = DIAL_2;
    else if (button == button1Button)
        clickedControl = BUTTON_1;
    else if (button == button2Button)
        clickedControl = BUTTON_2;
    else if (button == button3Button)
        clickedControl = BUTTON_3;

    
    mainComponentRef.setEliteControlsSettingsDisplay(clickedControl);
    
}

//void EliteControlsComponent::updateDisplay()
//{
//    
//}

void EliteControlsComponent::mouseEnter (const MouseEvent &e)
{
    
}

void EliteControlsComponent::mouseExit (const MouseEvent &e)
{
    
}