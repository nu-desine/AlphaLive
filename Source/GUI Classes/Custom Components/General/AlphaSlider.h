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
// This class is messy. Ideally it shouldn't inherit from slider;
// it should be a component which comprises of a label, 2 buttons (arrows) and an invisible slider. Possibly.
//It should then have a function called setValue which mimcs the sliders function, but sets the slider and label value.
//the class should appear to be a Juce::slider as much as possible, therefore 'reimplementing' the most popular slider functions which in turn call the slider functions.
//it could just have a getSlider(*slider) function that will in turn allow acces to the slider functions, but would not be very user friendly.
//However if this class isn't a slider any parent classes with need as access to the slider component to add a listener to it.
// For now I have added a setComponentValue() function to set the slider value as well as the label. This must be called instead of calling the Slider function setValue()
// externally without sending a notification, otherwise the label won't be updated. However if you call setValue(value, true) it should be ok, though we don't call this much.
//also there seems to be high CPU usage and eventual lagging when using the slider part lots. why? something to do with what it is repainting?
//thats why it might be better to have arrow buttons that you can paint individually instead of painting the whole component.

//more problems with this class:

//setting the number of decimal places when setting the label text from a float/double (mainly in setComponentValue()).
//ideally you'll want to get the step size of the slider (getInterval()) and work out how many decimal place to display from that.
//I've added a quick fix to setComponentValue() but it's not the best way of handling it.

//if you enter non-numerical text into the label it won't be accepted, however if you enter non-numerical text aswell as numbers, it will be. Not cool, not cool.

//what would be cool is if you select mutiple pads and alphaslider needs to display a default value, it should display '-' instead.
//what it be possible to force it to display that but not allow the user to enter it?





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
    void setComponentValue (double value);

	
	bool hitTest (int x, int y);
	
	
	
private:
	
    Label *sliderValueLabel;
	Path thePath;
	Colour arrowUpColour, arrowDownColour;
	int i, valueStore;
	
    
};

#endif