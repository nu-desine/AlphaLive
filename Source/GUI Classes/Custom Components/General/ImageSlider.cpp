//
//  ImageSlider.cpp
//  sdaJuce
//
//  Created by Liam Meredith-Lacey on 30/08/2011.
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

#include "ImageSlider.h"

ImageSlider::ImageSlider(Image* image, const int numFrames, const bool stripIsHorizontal)
                    :   Slider(" ImageSlider"),
                        filmStrip(image),
                        numFrames_(numFrames),
                        isHorizontal_(stripIsHorizontal) 
//init lists can be used to init class variables without the need of a 'mutator' method. Examine this class as an example.
{
    if(filmStrip) 
    {
        setTextBoxStyle(NoTextBox, 0, 0, 0);
        
        if(isHorizontal_) 
        {
            frameHeight = filmStrip->getHeight();
            frameWidth = filmStrip->getWidth() / numFrames_;
        } 
        else 
        {
            frameHeight = filmStrip->getHeight() / numFrames_;
            frameWidth = filmStrip->getWidth();
        }
    }
}




ImageSlider::~ImageSlider() 
{ 
    delete filmStrip; 
}



void ImageSlider::paint(Graphics& g)
{
    if(filmStrip) 
    {
        int value = (getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (numFrames_ - 1);
        
        if(isHorizontal_) 
        {
            g.drawImage(*filmStrip, 0, 0, getWidth(), getHeight(), value * frameWidth, 0, frameWidth, frameHeight);
        } else 
        {
            g.drawImage(*filmStrip, 0, 0, getWidth(), getHeight(), 0, value * frameHeight, frameWidth, frameHeight);
        }
    }
    
}

int ImageSlider::getFrameWidth() const 
{ 
    return filmStrip ? frameWidth  : 100; 
}


int ImageSlider::getFrameHeight() const 
{ 
    return filmStrip ? frameHeight : 24;  
}

