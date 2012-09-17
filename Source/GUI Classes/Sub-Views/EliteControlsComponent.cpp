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


EliteControlsComponent::EliteControlsComponent(MainComponent &ref)
        : mainComponentRef(ref)
{
    Path dialShape, buttonShape;
    dialShape.addEllipse (0, 0, 50, 50);
    buttonShape.addRectangle(0, 0, 30, 30);
    
    addAndMakeVisible (dial1Button = new ShapeButton("Dial 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    dial1Button->setShape(dialShape, true, true, false);
    addAndMakeVisible (dial2Button = new ShapeButton("Dial 2", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    dial2Button->setShape(dialShape, true, true, false);
    
    addAndMakeVisible (button1Button = new ShapeButton("Button 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    button1Button->setShape(buttonShape, true, true, false);
    addAndMakeVisible (button2Button = new ShapeButton("Button 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    button2Button->setShape(buttonShape, true, true, false);
    addAndMakeVisible (button3Button = new ShapeButton("Button 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    button3Button->setShape(buttonShape, true, true, false);
    
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