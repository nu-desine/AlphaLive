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

//Current issues with this class:

//Setting the number of decimal places when setting the label text from a float/double (mainly in setValue()).
//ideally you'll want to get the step size of the slider (getInterval()) and work out how many decimal place to display from that.
//I've added a quick fix to setValue() but it's not the best way of handling it.


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
	void paint(Graphics& g);
    void resized();
	
	void sliderValueChanged (Slider *slider);
	void labelTextChanged (Label* labelThatHasChanged);
	void sliderDragEnded (Slider *slider);
    
    void mouseDown(const MouseEvent &e);
	bool hitTest (int x, int y);
    
    void setValue (double value, int sendNotification = false);
	
private:
	
    Label *sliderValueLabel;
	Path thePath, upButtonPath, downButtonPath;
	Colour arrowUpColour, arrowDownColour;
	int i, valueStore;
	
};

#endif