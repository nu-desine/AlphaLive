/*
 *  SettingsButton.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 13/06/2012.
 //  Copyright 2012 nu desine.
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
 */

#include "SettingsButton.h"
#include "../../AlphaLiveLookandFeel.h"

SettingsButton::SettingsButton (const String& buttonName,float _startRadians, float _endRadians, float _theWidth, float _textRotation, float _textAngle, float _textRadius)
: Button ("SettingsButton"),
theText(buttonName)


{
    
	
	setClickingTogglesState(true);
	
	startRadians = _startRadians;
	endRadians = _endRadians;
	theWidth = _theWidth;
	
	textRotation = _textRotation;
	textAngle = _textAngle;
	textRadius = _textRadius;

	
	normalOpacity = 1.0f;
    normalOverlay = Colours::transparentBlack;
    overOpacity   = 0.5f;
    overOverlay   = Colours::white;
    downOpacity   = 0.5f;
    downOverlay   = Colours::white;
	onOpacity   = 0.9f;
    onOverlay   = AlphaColours::blue;
	
	
	
	
}

SettingsButton::~SettingsButton()
{
    
	
}



//==============================================================================
void SettingsButton::resized()
{
	thePath.addPieSegment(0, 0, getWidth(), getHeight(), startRadians, endRadians, theWidth);
}


void SettingsButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{

	float halfAngle = endRadians - startRadians;
	halfAngle = halfAngle * textAngle;
	halfAngle = halfAngle + startRadians;
	
	float sinX = sin(halfAngle);
	float cosY = -cos(halfAngle);
	
	float midRadius = (getWidth() * 0.5) * (theWidth + textRadius);
	//midRadius = midRadius + (((getWidth() * 0.5) * (1 - theWidth)) * textRadius);
	
	float xCo = (getWidth() * 0.5) + (midRadius * sinX);
	
	float yCo =	(getHeight() * 0.5) + (midRadius * cosY);	
	
	float textAngle = halfAngle - (textRotation *(M_PI / 180));
	
	AffineTransform rotateText(AffineTransform::rotation(textAngle)
							   .AffineTransform::translated(xCo, yCo)
							   .AffineTransform::scaled(0.8, 0.8,(getWidth()*0.5),(getHeight()*0.5)));
	
	
	
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            
			//ColourGradient fillGradient(Colours::black, (thePath.getBounds().getSmallestIntegerContainer().getX() * 0.5), (thePath.getBounds().getSmallestIntegerContainer().getY() * 0.5), Colours::nearlyblack, 0, 0, true);

			ColourGradient fillGradient(Colours::black, (getWidth() * 0.5), (getHeight() * 0.5), AlphaColours::nearlyblack, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.5f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.drawTextAsPath(theText, rotateText);
			
			
            break;
        }
			
		case 1:
        {
            
			ColourGradient fillGradient(Colours::black, (getWidth()*0.5), (getHeight()*0.5), AlphaColours::nearlyblack, 0, 0, true);			
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.7f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(Colours::darkgrey);
			g.drawTextAsPath(theText, rotateText);

			
            break;
        }
			
		case 2:
        {
			
			ColourGradient fillGradient(AlphaColours::nearlyblack, (getWidth()*0.5), (getHeight()*0.5), Colours::black, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.7f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(Colours::darkgrey);
			g.drawTextAsPath(theText, rotateText);

		
			
            break;
        }
			
		case 3:
        {
			
			ColourGradient fillGradient(AlphaColours::nearlyblack, (getWidth()*0.5), (getHeight()*0.5), Colours::black, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.4f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(AlphaColours::blue);
			g.drawTextAsPath(theText, rotateText);

			
			
            break;
        }
			
		case 4:
        {
			
			ColourGradient fillGradient(AlphaColours::nearlyblack, (getWidth()*0.5), (getHeight()*0.5), Colours::black, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.4f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(AlphaColours::blue);

			g.drawTextAsPath(theText, rotateText);


		
			
            break;
        }
			
		case 5:
        {
			
			ColourGradient fillGradient(AlphaColours::nearlyblack, (getWidth()*0.5), (getHeight()*0.5), Colours::black, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());

			g.setColour(Colours::darkgrey.withAlpha(0.4f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(AlphaColours::blue);

			g.drawTextAsPath(theText, rotateText);


		
			
            break;
        }
			
		default:
			break;
    }
	
	
}



void SettingsButton::clicked()
{
  
	
}

void SettingsButton::buttonStateChanged()
{
	
	
}

bool SettingsButton::hitTest (int x, int y)
{
    
	return thePath.contains(x, y);
	
}

