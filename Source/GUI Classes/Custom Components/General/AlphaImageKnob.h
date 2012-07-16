//
//  AlphaImageKnob.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 17/11/2011.
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

/*
 This class is a component which contains an ImageSlider with the standard knob film strip image
 with a Label on the top which displays the value of the slider.
 The label is only visible when the mouse is over the component.
 */

#ifndef H_ALPHAIMAGEKNOB
#define H_ALPHAIMAGEKNOB

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "ImageSlider.h"
//#include <math.h>

class AlphaImageKnob :  public Component,
                        public Slider::Listener
{
public:
    AlphaImageKnob(int labelDecimalPlaces, bool isPanStyle=false); //set to true if need to use the 'pan' style image rather than the regualr image
    ~AlphaImageKnob();
    
    Slider* sliderComponent();
    //Label* labelComponent();
    
    void resized();
    
    void sliderValueChanged (Slider *slider);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
private:
    
    ImageSlider *knob;
    Label *knobValueLabel;
    
    int noOfDecimalPlaces;
    
    
};



#endif