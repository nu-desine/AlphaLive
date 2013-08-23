/*
 *  GuiSwitch.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 25/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "GuiSwitch.h"
#include "../../Binary Data/BinaryDataNew.h"
#include "../../AlphaLiveLookandFeel.h"

GuiSwitch::GuiSwitch()
{
	setClickingTogglesState(true);
	theImage = ImageCache::getFromMemory(BinaryDataNew::onoff_png, BinaryDataNew::onoff_pngSize);
}

GuiSwitch::~GuiSwitch()
{
	deleteAllChildren();
}

void GuiSwitch::resized()
{
	
	hitPath.addEllipse (0.0f, 0.0f, getWidth(), getHeight());
	
	
}

void GuiSwitch::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	
	g.setColour(Colours::black);
	g.fillPath(hitPath, getTransform());
	
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            
			ColourGradient fillGradient(Colours::black, (getWidth()*0.5), (getHeight()*0.6), AlphaColours::getInstance()->colour3, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);

            break;
        }
			
		case 1:
        {
            
			ColourGradient fillGradient(AlphaColours::getInstance()->colour3, (getWidth()*0.5), (getHeight()*0.9), Colours::black, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);

            break;
        }
			
		case 2:
        {
            
			ColourGradient fillGradient(AlphaColours::getInstance()->colour3, (getWidth()*0.5), (getHeight()*0.7), Colours::black, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);

            break;
        }
			
		case 3:
        {
            
			ColourGradient fillGradient(AlphaColours::getInstance()->colour1, (getWidth()*0.5), (getHeight()*0.5), AlphaColours::getInstance()->colour2, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			
            break;
        }
			
		case 4:
        {
            
			ColourGradient fillGradient(AlphaColours::getInstance()->colour2, (getWidth()*0.5), (getHeight()*0.9), AlphaColours::getInstance()->colour1, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
			
            break;
        }
			
		case 5:
        {
            
			ColourGradient fillGradient(AlphaColours::getInstance()->colour2, (getWidth()*0.5), (getHeight()*0.7), AlphaColours::getInstance()->colour1, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
			
            break;
        }
			
			
			
		default:
			break;
			
	}
	
	g.fillEllipse((getWidth()*0.15), (getHeight()*0.15), (getWidth()*0.7), (getHeight()*0.7));
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
	
	g.setColour(Colours::white);
	g.drawImage (theImage,
				 (getWidth()*0.22), (getHeight()*0.22), (getWidth()*0.6), (getHeight()*0.6),
				 0, 0, theImage.getWidth(), theImage.getHeight(), true);
}


void GuiSwitch::clicked()
{
	
}

bool GuiSwitch::hitTest (int x, int y)
{
	
	return hitPath.contains(x, y);
	
}

