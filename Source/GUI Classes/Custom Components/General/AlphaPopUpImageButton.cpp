/*
 *  AlphaPopUpImageButton.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 15/11/2012.
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
 //  along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 */

#include "AlphaPopUpImageButton.h"
#include "../../AlphaLiveLookandFeel.h"

AlphaPopUpImageButton::AlphaPopUpImageButton()

{
	
	setClickingTogglesState(false);
	theImage = new Image();
	
 	
}

AlphaPopUpImageButton::~AlphaPopUpImageButton()
{
	
	delete theImage;
	
}

void AlphaPopUpImageButton::resized()
{
	
	hitPath.addEllipse (0.0f, 0.0f, getWidth(), getHeight());
	
	
}

void AlphaPopUpImageButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	
	g.setColour(Colours::black);
	g.fillPath(hitPath, getTransform());
	
	float imageAlpha = 1.0f;
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            
			ColourGradient fillGradient(AlphaColours::blue, (getWidth()*0.5), (getHeight()*0.5), AlphaColours::lightblue, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			imageAlpha = 0.7f;
			
			
            break;
        }
			
		case 1:
        {
            
			ColourGradient fillGradient(AlphaColours::lightblue, (getWidth()*0.5), (getHeight()*0.9), AlphaColours::blue, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
			imageAlpha = 1.0f;
			
			
            break;
        }
			
		case 2:
        {
            
			ColourGradient fillGradient(AlphaColours::lightblue, (getWidth()*0.5), (getHeight()*0.7), AlphaColours::blue, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
			imageAlpha = 1.0f;
			
            break;
        }
			
		case 3:
        {
            
			ColourGradient fillGradient(AlphaColours::blue, (getWidth()*0.5), (getHeight()*0.5), AlphaColours::lightblue, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			imageAlpha = 1.0f;
			
			
            break;
        }
			
		case 4:
        {
            
			ColourGradient fillGradient(AlphaColours::lightblue, (getWidth()*0.5), (getHeight()*0.9), AlphaColours::blue, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
			imageAlpha = 1.0f;
			
			
            break;
        }
			
		case 5:
        {
            
			ColourGradient fillGradient(AlphaColours::lightblue, (getWidth()*0.5), (getHeight()*0.7), AlphaColours::blue, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
			imageAlpha = 1.0f;
			
			
            break;
        }
			
			
			
		default:
			break;
			
	}
	
	g.fillEllipse((getWidth()*0.15), (getHeight()*0.15), (getWidth()*0.7), (getHeight()*0.7));
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
	
	
	g.setFont (10);
    g.setColour (Colours::white.withAlpha(imageAlpha));

	if(theImage->isValid ())
	{
	g.drawImage (*theImage,
				0, 0, getWidth(), getHeight(),
				 0, 0, theImage->getWidth(), theImage->getHeight(), true);
	}
}


void AlphaPopUpImageButton::clicked()
{
	
}

void AlphaPopUpImageButton::setImage(Image *_theImage)
{
	
	theImage = _theImage;
	repaint();
	
}

bool AlphaPopUpImageButton::hitTest (int x, int y)
{
	
	return hitPath.contains(x, y);
	
}

