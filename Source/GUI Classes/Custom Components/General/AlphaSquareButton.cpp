/*
 *  AlphaSquareButton.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 08/11/2012.
 *  Copyright 2012 nu desine. All rights reserved.
 
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

#include "AlphaSquareButton.h"
#include "../../AlphaLiveLookandFeel.h"

AlphaSquareButton::AlphaSquareButton(float _theCornerRadius)
: Button ("AlphaSquareButton")

{
	
	theCornerRadius = _theCornerRadius;
	setClickingTogglesState(true);
	
 	
}

AlphaSquareButton::~AlphaSquareButton()
{
	
	//deleteAllChildren();
	
}

void AlphaSquareButton::resized()
{
	
	hitPath.addRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
	
	
}

void AlphaSquareButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            
			g.setColour(Colours::black);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient fillGradient(Colours::black, (getWidth()*0.5), (getHeight()*0.6), Colours::white.withAlpha(0.1f), (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawRoundedRectangle ((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), theCornerRadius, (getHeight()*0.025));
			
			
			
            break;
        }
			
		case 1:
        {
            
			g.setColour(Colours::black);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
            ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.9), Colours::black, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawRoundedRectangle ((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), theCornerRadius, (getHeight()*0.025));
			
		
            break;
        }
			
		case 2:
        {
			g.setColour(Colours::black);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
            ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.4), Colours::black, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawRoundedRectangle ((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), theCornerRadius, (getHeight()*0.025));
			
			
            break;
        }
			
		case 3:
        {
			
			g.setColour(AlphaColours::blue);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.7), AlphaColours::blue, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawRoundedRectangle ((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), theCornerRadius, (getHeight()*0.025));
			
			
            break;
        }
			
		case 4:
        {
			g.setColour(AlphaColours::blue);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.7), AlphaColours::blue, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawRoundedRectangle ((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), theCornerRadius, (getHeight()*0.025));
			
			
            break;
        }
			
		case 5:
        {
			g.setColour(AlphaColours::blue);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.7), AlphaColours::blue, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillRoundedRectangle (0.0f, 0.0f, getWidth(), getHeight(), theCornerRadius);
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawRoundedRectangle ((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), theCornerRadius, (getHeight()*0.025));
	
			
            break;
        }
			
		default:
			break;
    }	 
	
	/*g.setColour(Colours::white.withAlpha(0.8f));
	 g.drawImage (* symbol,
	 0, 0, getWidth(), getHeight(),
	 0, 0, symbol->getWidth(), symbol->getHeight(), true);*/
	
}


void AlphaSquareButton::clicked()
{
	
}

bool AlphaSquareButton::hitTest (int x, int y)
{
	
	return hitPath.contains(x, y);
	
}