//
//  AlphaImageKnob.cpp
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

#include "AlphaImageKnob.h"
#include "../../Binary Data/ImageSliderBinaryData.h"



AlphaImageKnob::AlphaImageKnob(int labelDecimalPlaces, bool isPanStyle)
                                        :   noOfDecimalPlaces(labelDecimalPlaces)
{
    Image *image = new Image();
    
    if (isPanStyle == false)
    {
        //load regular image
        //*image = ImageFileFormat::loadFrom(ImageSliderBinaryData::gain2_png, ImageSliderBinaryData::gain2_pngSize);
        *image = ImageCache::getFromMemory(ImageSliderBinaryData::gain2_png, ImageSliderBinaryData::gain2_pngSize);
    }
    else
    {
        //load pan image
        //*image = ImageFileFormat::loadFrom(ImageSliderBinaryData::pan3_png, ImageSliderBinaryData::pan3_pngSize);
        *image = ImageCache::getFromMemory(ImageSliderBinaryData::pan3_png, ImageSliderBinaryData::pan3_pngSize);
    }
    
    addAndMakeVisible(knob = new ImageSlider(image, 55, true));
    knob->setSliderStyle(Slider::RotaryVerticalDrag);
    knob->addListener(this);
    knob->addMouseListener(this, true);
    
    addAndMakeVisible(knobValueLabel = new Label ("Label", String::empty));
    knobValueLabel->setJustificationType(Justification::centred);
    knobValueLabel->setInterceptsMouseClicks(false, false);
    knobValueLabel->setColour(Label::textColourId, Colours::black.withAlpha(0.7f));
    //Font newFont("Grixel Acme 7 Wide", 17, Font::plain);
    //knobValueLabel->setFont(newFont);
    knobValueLabel->setVisible(false);
    
}

AlphaImageKnob::~AlphaImageKnob()
{
    deleteAllChildren();
}

void AlphaImageKnob::resized()
{
    knob->setBounds(0, 0, getWidth(), getHeight());
    knobValueLabel->setBounds(0, 0, getWidth(), getHeight());
}


void AlphaImageKnob::sliderValueChanged (Slider *slider)
{
    if (slider == knob)
    {
        //rounding code taken from http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
        //double knobValueRounded = floorf(knob->getValue() * 100) /100;
        //std::cout << String(knobValueRounded) << std::endl;
        
        knobValueLabel->setText(String(knob->getValue(), noOfDecimalPlaces), false);
    }
}

//accessed by parent classes to set aspects of the slider child component of this component
Slider* AlphaImageKnob::sliderComponent()
{
    return knob;
}

/*
//accessed by parent classes to set aspects of the label child component of this component
Label* AlphaImageKnob::labelComponent()
{
    return  knobValueLabel;
}
 */

void AlphaImageKnob::mouseEnter (const MouseEvent &e)
{
    knobValueLabel->setText(String(knob->getValue(), noOfDecimalPlaces), false);
    knobValueLabel->setVisible(true);
    
}
void AlphaImageKnob::mouseExit (const MouseEvent &e)
{
    knobValueLabel->setVisible(false);
}

