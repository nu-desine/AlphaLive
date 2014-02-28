/*
 *  AlphaPopUpButton.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 09/08/2012.
 
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

#include "AlphaPopUpButton.h"
#include "../../AlphaLiveLookandFeel.h"

AlphaPopUpButton::AlphaPopUpButton()

{
	
	setClickingTogglesState(false);
	
 	
}

AlphaPopUpButton::~AlphaPopUpButton()
{
	
	//deleteAllChildren();
	
}

void AlphaPopUpButton::resized()
{
	
	hitPath.addEllipse (0.0f, 0.0f, getWidth(), getHeight());
	
	
}

void AlphaPopUpButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	g.fillPath(hitPath, getTransform());
	
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.5), AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			
			
            break;
        }
			
		case 1:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), (getHeight()*0.9), AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
		
			
            break;
        }
			
		case 2:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), (getHeight()*0.7), AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
			
			
            break;
        }
			
		case 3:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.5), AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			
			
            break;
        }
			
		case 4:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), (getHeight()*0.9), AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
			
			
            break;
        }
			
		case 5:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), (getHeight()*0.7), AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
		
			
            break;
        }
			
			
			
		default:
			break;
			
	}
	
	g.fillEllipse((getWidth()*0.15), (getHeight()*0.15), (getWidth()*0.7), (getHeight()*0.7));
	
	g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
	g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
	
	
	g.setFont (10 + AlphaTheme::getInstance()->fontSizeAddition);
    g.setColour (AlphaTheme::getInstance()->iconOnColour);
	
	
    g.drawFittedText (getButtonText(),
                      (getWidth()*0.15), (getHeight()*0.35), (getWidth()*0.7), (getHeight()*0.3),
                      Justification::centred, 2);
	
}


void AlphaPopUpButton::clicked()
{
	
}

bool AlphaPopUpButton::hitTest (int x, int y)
{
	
	return hitPath.contains(x, y);
	
}
