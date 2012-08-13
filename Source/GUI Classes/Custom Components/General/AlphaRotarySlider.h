/*
 *  AlphaRotarySlider.h
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

//would be good if this class was more user friendly for developers
//so that there's a more logical way to set the objects arguments in relation to its bounds

#ifndef H_ALPHAROTARYSLIDER
#define H_ALPHAROTARYSLIDER

#include "../../../../JuceLibraryCode/JuceHeader.h"

class AlphaRotarySlider : public Slider


{
public:
    AlphaRotarySlider(const float startAngleRadians,
					  const float endAngleRadians, const float _theWidth);
    ~AlphaRotarySlider();
	bool hitTest (int x, int y);
	
private:
	
	Path hitPath;
	float rotaryStart, rotaryEnd, theWidth;
	

};

#endif