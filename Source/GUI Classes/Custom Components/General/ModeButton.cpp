/*
 *  ModeButton.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 14/09/2011.
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
 *
 */

#include "ModeButton.h"
#include "../../AlphaLiveLookandFeel.h"


//==============================================================================

ModeButton::ModeButton (Image* modeSymbol)
:   Button ("ModeButton"),
    symbol(modeSymbol)
{
	setClickingTogglesState(true);
	imageRotationAngle =pivotXOffset_ = pivotYOffset_ = 0;
}

ModeButton::~ModeButton()
{
    delete symbol;	
}

//==============================================================================
void ModeButton::resized()
{
    hitPath.addEllipse (0.0f, 0.0f, getWidth(), getHeight());
}

void ModeButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
			g.setColour(AlphaTheme::getInstance()->backgroundColour);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient fillGradient(AlphaTheme::getInstance()->backgroundColour, (getWidth()*0.5), (getHeight()*0.6), Colours::white.withAlpha(0.1f), (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(Colours::white.withAlpha(0.8f));
			
            break;
        }
			
		case 1:
        {
			g.setColour(AlphaTheme::getInstance()->backgroundColour);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
            ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.9), AlphaTheme::getInstance()->backgroundColour, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			
			g.setColour(Colours::white.withAlpha(1.0f));
			
            break;
        }
			
		case 2:
        {
			g.setColour(AlphaTheme::getInstance()->backgroundColour);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
            ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.4), AlphaTheme::getInstance()->backgroundColour, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(Colours::white.withAlpha(1.0f));
			
            break;
        }
			
		case 3:
        {
			g.setColour(AlphaTheme::getInstance()->mainColour);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.7), AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(Colours::white.withAlpha(1.0f));
			
            break;
        }
			
		case 4:
        {
			g.setColour(AlphaTheme::getInstance()->mainColour);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.7), AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(Colours::white.withAlpha(1.0f));
			
            break;
        }
			
		case 5:
        {
			g.setColour(AlphaTheme::getInstance()->mainColour);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (getWidth()*0.5), (getHeight()*0.7), AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(Colours::white.withAlpha(1.0f));
			
            break;
        }
			
		default:
			break;
    }
    
    
//    g.drawImage (* symbol,
//                 0, 0, getWidth(), getHeight(),
//                 0, 0, symbol->getWidth(), symbol->getHeight(), true);
    
    g.drawImageTransformed(symbol->rescaled(getWidth(), 
                                            getHeight()), 
                           AffineTransform::rotation(imageRotationAngle, 
                                                     (getWidth() / 2) + pivotXOffset_, 
                                                     (getHeight() / 2) + pivotYOffset_));
}

bool ModeButton::hitTest (int x, int y)
{
	return hitPath.contains(x, y);
}

void ModeButton::rotateImage (float angleInRadians, float pivotXOffset, float pivotYOffset)
{
    //rotates the buttons image around the centre point of the button
    
    imageRotationAngle = angleInRadians;
    pivotXOffset_ = pivotXOffset;
    pivotYOffset_ = pivotYOffset;
    
    repaint();
}
