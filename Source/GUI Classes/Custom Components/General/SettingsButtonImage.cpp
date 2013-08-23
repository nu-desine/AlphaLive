/*
 *  SettingsButtonImage.cpp
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

#include "SettingsButtonImage.h"
#include "../../AlphaLiveLookandFeel.h"

SettingsButtonImage::SettingsButtonImage (Image* _theImage, float _startRadians, float _endRadians, float _theWidth, float _imageWidth)
                                        :   Button ("SettingsButtonImage"),
                                            theImage(_theImage)

{
	setClickingTogglesState(true);
    
	startRadians = _startRadians;
	endRadians = _endRadians;
	theWidth = _theWidth;
	
	imageWidth = _imageWidth;
	
	normalOpacity = 1.0f;
    normalOverlay = Colours::transparentBlack;
    overOpacity   = 0.5f;
    overOverlay   = Colours::white;
    downOpacity   = 0.5f;
    downOverlay   = Colours::white;
	onOpacity   = 0.9f;
    onOverlay   = AlphaColours::colour1;
}

SettingsButtonImage::~SettingsButtonImage()
{
	delete theImage;	
}


//==============================================================================
void SettingsButtonImage::resized()
{
	thePath.addPieSegment(0, 0, getWidth(), getHeight(), startRadians, endRadians, theWidth);
}


void SettingsButtonImage::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	
	
	
	float halfAngle = endRadians - startRadians;
	halfAngle = halfAngle * 0.5;
	halfAngle = halfAngle + startRadians;
	
	float sinX = sin(halfAngle);
	float cosY = -cos(halfAngle);
	float midRadius = (getWidth() * 0.5) * (theWidth + ((1 - theWidth) * 0.5));
	//float xCo = (getWidth() * 0.5) + (midRadius * sinX);
	//float yCo =	(getHeight() * 0.5) + (midRadius * cosY);
											
	//float imageAngle = halfAngle - (startRadians + (M_PI / (4 *(M_PI / 180)))
	float maxImageWidth = ((getWidth() * 0.5) * (1 - theWidth)) * 0.7;
	float imageScale = (maxImageWidth * imageWidth) / theImage->getWidth();

	//float imageSinX = sin(imageAngle);
	//float imageCosY = -cos(imageAngle);
										
	float imageRadius = midRadius + ((maxImageWidth * 0.5) * imageWidth);
										
	float xImageCo = (getWidth() * 0.5) + (imageRadius * sinX);
											
	float yImageCo = (getHeight() * 0.5) + (imageRadius * cosY);
												
	
	AffineTransform rotateImage(AffineTransform::scale(imageScale, imageScale).
								AffineTransform::translated((xImageCo - ((maxImageWidth * 0.5) * imageWidth)), 
															yImageCo).
								AffineTransform::rotated(halfAngle, xImageCo, yImageCo));
	

	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
			ColourGradient fillGradient(Colours::black, (getWidth() * 0.5), (getHeight() * 0.5), AlphaColours::colour3, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.5f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(Colours::white.withAlpha(0.5f));
			g.drawImageTransformed(*theImage, rotateImage, true);
			
            break;
        }
			
		case 1:
        {
			ColourGradient fillGradient(Colours::black, (getWidth()*0.5), (getHeight()*0.5), AlphaColours::colour3, 0, 0, true);			
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.7f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(Colours::white.withAlpha(0.8f));

			g.drawImageTransformed(*theImage, rotateImage, true);
			
            break;
        }
			
		case 2:
        {
			ColourGradient fillGradient(AlphaColours::colour3, (getWidth()*0.5), (getHeight()*0.5), Colours::black, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.7f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(Colours::white.withAlpha(0.8f));

			g.drawImageTransformed(*theImage, rotateImage, true);
			
            break;
        }
			
		case 3:
        {
			ColourGradient fillGradient(AlphaColours::colour3, (getWidth()*0.5), (getHeight()*0.5), Colours::black, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.4f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(AlphaColours::colour1);
			g.drawImageTransformed(*theImage, rotateImage, true);
			
            break;
        }
			
		case 4:
        {
			ColourGradient fillGradient(AlphaColours::colour3, (getWidth()*0.5), (getHeight()*0.5), Colours::black, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.4f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(AlphaColours::colour1);
			
			g.drawImageTransformed(*theImage, rotateImage, true);
			
            break;
        }
			
		case 5:
        {
			ColourGradient fillGradient(AlphaColours::colour3, (getWidth()*0.5), (getHeight()*0.5), Colours::black, 0, 0, true);
			g.setGradientFill(fillGradient);
			g.fillPath(thePath, getTransform());
			
			g.setColour(Colours::darkgrey.withAlpha(0.4f));
			g.strokePath (thePath, 1.0f, getTransform());
			
			g.setColour(AlphaColours::colour1);
			
			g.drawImageTransformed(*theImage, rotateImage, true);
			
            break;
        }
			
		default:
			break;
    }
}



void SettingsButtonImage::clicked()
{
	
	
}

void SettingsButtonImage::buttonStateChanged()
{
	
	
}

bool SettingsButtonImage::hitTest (int x, int y)
{
    
	return thePath.contains(x, y);
	
}
