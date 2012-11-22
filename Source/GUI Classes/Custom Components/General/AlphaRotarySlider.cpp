/*
 *  AlphaRotarySlider.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 11/07/2012.
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
 */

#include "AlphaRotarySlider.h"

AlphaRotarySlider::AlphaRotarySlider(const float startAngleRadians,
									  const float endAngleRadians, const float _theWidth)
{
	setSliderStyle(Slider::Rotary);
	setTextBoxStyle(Slider::NoTextBox, false, 80, 40);
    
	rotaryStart = startAngleRadians;
	rotaryEnd = endAngleRadians;
	theWidth = _theWidth;
	hitPath.addPieSegment (0, 0, theWidth, theWidth, rotaryStart, rotaryEnd, ((1 - 0.2) + (0.2 * (theWidth * 0.5) * 0.003)));
    
    addChildComponent (textBox = new Label());
    textBox->setColour(Label::textColourId, Colours::grey);
    textBox->setColour(Label::backgroundColourId, Colours::lightgrey);
    textBox->setOpaque(true);
    textBox->setJustificationType(Justification::centred);
    textBox->setEditable(true);
    textBox->addListener(this);
    
    //set this component to listen to itself
    addKeyListener(this);
	
}

AlphaRotarySlider::~AlphaRotarySlider()
{
	deleteAllChildren();
}


void AlphaRotarySlider::labelTextChanged (Label* labelThatHasChanged)
{
    if (labelThatHasChanged == textBox)
    {
        
        keyPressed(KeyPress(KeyPress::returnKey), this);
    }
}
					
bool AlphaRotarySlider::hitTest (int x, int y)
{
	return hitPath.contains(x, y);
}

void AlphaRotarySlider::mouseDown(const MouseEvent &e)
{
	
	if (e.mods.isPopupMenu() == true)
    {
        std::cout << "popping" << std::endl;
        
        hitPath.clear();
        hitPath.addRectangle(0, 0, getWidth(), getHeight());
        
        textBox->setBounds(e.x, e.y, 50, 20);
        textBox->setText(String(this->getValue()), false);
        textBox->showEditor();
        //textBox->getCurrentTextEditor ()->setInputRestrictions(0, "1234567890.");
        textBox->setVisible(true);
        
        grabKeyboardFocus();
    }
    else if (e.mods.isLeftButtonDown() && textBox->isVisible() == true)
    {
        keyPressed(KeyPress(KeyPress::returnKey), this);
    }
    else
    {
        Slider::mouseDown(e);
    }
}

bool AlphaRotarySlider::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    std::cout << "keypressed!" << std::endl;
    
    
    if (key == KeyPress::returnKey)
    {
        std::cout << "return key pressed" << std::endl;
        if (textBox->isVisible() == true)
        {
            textBox->setVisible(false);
            
            hitPath.clear();
            hitPath.addPieSegment (0, 0, theWidth, theWidth, rotaryStart, rotaryEnd, ((1 - 0.2) + (0.2 * (theWidth * 0.5) * 0.003)));
            
            return true;
        }
        else
            return false;
        
    }
    else
        return false; //incase the keypress is a shortcut that the parent needs to react to.
    
}