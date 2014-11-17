//
//  ImageSlider.h
//  sdaJuce
//
//  Created by Liam Meredith-Lacey on 30/08/2011.
//  Adapted from http://code.google.com/p/juced/source/browse/trunk/apps/tal-reverb/src/ImageSlider.h?r=145&spec=svn145 and http://www.rawmaterialsoftware.com/viewtopic.php?f=8&t=4169&hilit=filmstrip
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

#ifndef IMAGESLIDER_H
#define IMAGESLIDER_H

#include "../../../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 A class that creates a slider with a custom image on top.
 The class inherits from the Slider class. 
 Therefore you can apply all the same methods to this slider object as you can to a regular slider object.
 The image applied needs to be in a 'Film Strip' style - an image file containing a line of image 'frames' that differ in the way that the slider needs to animate when the user interacts with it.
 */

class ImageSlider : public Slider
{
public:
    
    //==============================================================================
    /** Creates a slider.
     
     @param image               the image to be applied to the slider
     @param numFrames           the number of frames contained within the image
     @param stripIsHorizontal   set to true if the frames in the image are aligned horizontally, false if vertically
     
     */
    ImageSlider(Image* image, const int numFrames, const bool stripIsHorizontal);
    
    /** Destructor. */
    ~ImageSlider();
    
    /** Overrides the components paint method. */
    void paint(Graphics& g);
    
    /** Returns the width of a frame. */
    int getFrameWidth() const; 

    /** Returns the height of a frame. */
    int getFrameHeight() const;
    
private:
    //==============================================================================
    Image* filmStrip;
    const int numFrames_;
    const bool isHorizontal_;
    int frameWidth, frameHeight;
};

#endif //IMAGESLIDER_H