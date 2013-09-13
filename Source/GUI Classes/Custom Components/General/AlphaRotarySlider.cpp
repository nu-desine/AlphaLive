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
    
    addChildComponent (textBox = new TextEditor());
    textBox->setColour(TextEditor::textColourId, Colours::darkgrey);
    textBox->setColour(TextEditor::backgroundColourId, Colours::lightgrey);
    textBox->setColour(TextEditor::focusedOutlineColourId, AlphaTheme::getInstance()->childBackgroundColour);
    textBox->setOpaque(true);
    textBox->setSelectAllWhenFocused (true);
    textBox->addListener(this);
    
    //set this component to listen to itself
    addKeyListener(this);
	
}

AlphaRotarySlider::~AlphaRotarySlider()
{
	deleteAllChildren();
}


void AlphaRotarySlider::textEditorReturnKeyPressed (TextEditor &textEditor)
{
    if (&textEditor == textBox)
    {
        this->setValue(textBox->getText().getDoubleValue(), sendNotification);
        hideTextBox();
    }
    
}

void AlphaRotarySlider::textEditorFocusLost (TextEditor &textEditor)
{
    if (&textEditor == textBox)
    {
        hideTextBox();
    }
}
					
bool AlphaRotarySlider::hitTest (int x, int y)
{
	return hitPath.contains(x, y);
}

void AlphaRotarySlider::mouseDown(const MouseEvent &e)
{
	
    //if right click, display text box
	if (e.mods.isPopupMenu() == true)
    {
        hitPath.clear();
        hitPath.addRectangle(0, 0, getWidth(), getHeight());
        
        //set bounds of text box so it doesn't go out of bounds of the overall component
        int xPos = e.x;
        int yPos = e.y;
        int width = 50;
        int height = 20;
        
        if (xPos + width > getWidth())
            xPos = getWidth() - width;
        if (yPos + height > getHeight())
            yPos = getHeight() - height;
            
        textBox->setBounds(xPos, yPos, width, height);
        
        textBox->setText(String(this->getValue()), false);
        textBox->setInputRestrictions(0, "1234567890.-");
        textBox->setVisible(true);
        
        //set this whole component to the front so that the text box will be in full view.
        //Also this call with give the component and the text box keyboard focus that will
        //subsequently highlight the text box text.
        this->toFront(true);
    }
    //if regular click and the text box is currently visible, hide the text box
    else if (e.mods.isLeftButtonDown() && textBox->isVisible() == true)
    {
        hideTextBox();
    }
    //else, send mouse command to the Slider aspect of this component
    else
    {
        Slider::mouseDown(e);
    }
}

bool AlphaRotarySlider::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    if (key == KeyPress::returnKey)
    {
        hideTextBox();
        return true;
    }
    else
        return false; //incase the keypress is a shortcut that the parent needs to react to.
    
}


void AlphaRotarySlider::hideTextBox()
{
    if (textBox->isVisible() == true)
    {
        textBox->setVisible(false);
        
        hitPath.clear();
        hitPath.addPieSegment (0, 0, theWidth, theWidth, rotaryStart, rotaryEnd, ((1 - 0.2) + (0.2 * (theWidth * 0.5) * 0.003)));
    }
}

