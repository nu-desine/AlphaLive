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
	
	this->setSliderStyle(Slider::Rotary);
	this->setTextBoxStyle(Slider::NoTextBox, false, 80, 40);
	rotaryStart = startAngleRadians;
	rotaryEnd = endAngleRadians;
	theWidth = _theWidth;
	
	hitPath.addPieSegment (0, 0, theWidth, theWidth, rotaryStart, rotaryEnd, ((1 - 0.2) + (0.2 * (theWidth * 0.5) * 0.006)));
	
}

AlphaRotarySlider::~AlphaRotarySlider()
{
	
	//deleteAllChildren();
    
}

					
bool AlphaRotarySlider::hitTest (int x, int y)
{

	return hitPath.contains(x, y);
	
}