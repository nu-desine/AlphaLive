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
#include "../../Binary Data/BinaryData.h"
#include "../../AlphaLiveLookandFeel.h"


//==============================================================================

ModeButton::ModeButton (Image* modeSymbol)
:   Button ("ModeButton"),
symbol(modeSymbol)
{
	setClickingTogglesState(true);
	
	
	normalOpacity = 1.0f;
    normalOverlay = Colours::transparentBlack;
    overOpacity   = 0.5f;
    overOverlay   = Colours::white;
    downOpacity   = 0.5f;
    downOverlay   = Colours::white;
	onOpacity   = 0.9f;
    onOverlay   = AlphaColours::blue;
	
}

ModeButton::~ModeButton()
{
    delete symbol;
	//deleteAllChildren();
	
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
            
			
			ColourGradient fillGradient(Colours::black, (getWidth()*0.5), (getHeight()*0.6), AlphaColours::nearlyblack, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
            
			
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, false);
            break;
        }
			
		case 1:
        {
            
            ColourGradient fillGradient(Colours::black, (getWidth()*0.5), (getHeight()*0.6), AlphaColours::nearlyblack, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(overOverlay);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, false);
			g.setOpacity(overOpacity);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, true);
            
            break;
        }
			
		case 2:
        {
            
            ColourGradient fillGradient(AlphaColours::nearlyblack, (getWidth()*0.5), (getHeight()*0.9), Colours::black, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(downOverlay);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, false);
			g.setOpacity(downOpacity);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, true);
			
            break;
        }
			
		case 3:
        {
            
			ColourGradient fillGradient(AlphaColours::nearlyblack, (getWidth()*0.5), (getHeight()*0.9), Colours::black, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(onOverlay);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, false);
			
			g.setOpacity(onOpacity);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, true);
			
			
            break;
        }
			
		case 4:
        {
            
			ColourGradient fillGradient(AlphaColours::nearlyblack, (getWidth()*0.5), (getHeight()*0.9), Colours::black, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(onOverlay);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, false);
			g.setOpacity(onOpacity);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, true);
			
            break;
        }
			
		case 5:
        {
            
            ColourGradient fillGradient(AlphaColours::nearlyblack, (getWidth()*0.5), (getHeight()*0.9), Colours::black, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, getWidth(), getHeight());
			
			ColourGradient highlightGradient(Colours::transparentBlack, (getWidth()*0.5), (getHeight()*0.8), Colours::silver.withAlpha(0.3f), (getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((getWidth()* 0.03), (getHeight()* 0.03), (getWidth()*0.94), (getHeight()*0.94), (getHeight()*0.025));
			
			g.setColour(onOverlay);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, false);
			g.setOpacity(onOpacity);
			g.drawImage (* symbol,
						 0, 0, getWidth(), getHeight(),
                         0, 0, 50, 50, true);
			
            break;
        }
			
		default:
			break;
    }
    
	
}



void ModeButton::clicked()
{
    //[UserCode_clicked] -- Add your code here...
    //[/UserCode_clicked]
	
    
}

void ModeButton::buttonStateChanged()
{
    //
    
}

bool ModeButton::hitTest (int x, int y)
{
    
	return hitPath.contains(x, y);
	
}





