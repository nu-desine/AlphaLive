//
//  AlphaSlider.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 31/10/2011.
//  Copyright 2011 nu desine.
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
// Class that hold the custom settings for the IncDecButton Slider used throughout the App.
// Allows certain aspects of multiple sliders to be updated from just here!


#ifndef H_ALPHASLIDER
#define H_ALPHASLIDER

#include "../../../../JuceLibraryCode/JuceHeader.h"

class AlphaSlider : public Slider,
					public Slider::Listener,
					public Label::Listener
	
{
public:
    AlphaSlider();
    ~AlphaSlider();
	//void resized();
	void paint(Graphics& g);
	
	void sliderValueChanged (Slider *slider);
	void labelTextChanged (Label* labelThatHasChanged);
	void sliderDragEnded (Slider *slider);

	
	bool hitTest (int x, int y);
	
	
	
private:
	
	Path thePath;
	Label *sliderValue;
	Colour arrowUpColour, arrowDownColour;
	int i, valueStore;
	
    
};

#endif