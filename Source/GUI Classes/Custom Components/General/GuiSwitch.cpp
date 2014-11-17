/*
 *  GuiSwitch.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 25/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "GuiSwitch.h"
#include "../../Binary Data/MainBinaryData.h"
#include "../../AlphaLiveLookandFeel.h"

GuiSwitch::GuiSwitch()
{
	setClickingTogglesState(true);
	theImage = ImageCache::getFromMemory(MainBinaryData::onoff_png, MainBinaryData::onoff_pngSize);
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
	
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	g.fillPath(hitPath, getTransform());
	
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->backgroundColour, (getWidth()*0.5), (getHeight()*0.6), AlphaTheme::getInstance()->childBackgroundColour, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
            g.fillEllipse((getWidth()*0.15), (getHeight()*0.15), (getWidth()*0.7), (getHeight()*0.7));
            
            g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
            g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
            g.setColour(AlphaTheme::getInstance()->iconColour);
            g.drawImage (theImage,
                         (getWidth()*0.22), (getHeight()*0.22), (getWidth()*0.6), (getHeight()*0.6),
                         0, 0, theImage.getWidth(), theImage.getHeight(), true);

            break;
        }
			
		case 1:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->childBackgroundColour, (getWidth()*0.5), (getHeight()*0.9), AlphaTheme::getInstance()->backgroundColour, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
            g.fillEllipse((getWidth()*0.15), (getHeight()*0.15), (getWidth()*0.7), (getHeight()*0.7));
            
            g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
            g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
            g.setColour(AlphaTheme::getInstance()->iconColour);
            g.drawImage (theImage,
                         (getWidth()*0.22), (getHeight()*0.22), (getWidth()*0.6), (getHeight()*0.6),
                         0, 0, theImage.getWidth(), theImage.getHeight(), true);

            break;
        }
			
		case 2:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->childBackgroundColour, (getWidth()*0.5), (getHeight()*0.7), AlphaTheme::getInstance()->backgroundColour, (getWidth()*0.5), (getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
            g.fillEllipse((getWidth()*0.15), (getHeight()*0.15), (getWidth()*0.7), (getHeight()*0.7));
            
            g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
            g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
            g.setColour(AlphaTheme::getInstance()->iconColour);
            g.drawImage (theImage,
                         (getWidth()*0.22), (getHeight()*0.22), (getWidth()*0.6), (getHeight()*0.6),
                         0, 0, theImage.getWidth(), theImage.getHeight(), true);

            break;
        }
			
		case 3:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.5), AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
            g.fillEllipse((getWidth()*0.15), (getHeight()*0.15), (getWidth()*0.7), (getHeight()*0.7));
            
            g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
            g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
            g.setColour(AlphaTheme::getInstance()->iconOnColour);
            g.drawImage (theImage,
                         (getWidth()*0.22), (getHeight()*0.22), (getWidth()*0.6), (getHeight()*0.6),
                         0, 0, theImage.getWidth(), theImage.getHeight(), true);
			
            break;
        }
			
		case 4:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), (getHeight()*0.9), AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
            g.fillEllipse((getWidth()*0.15), (getHeight()*0.15), (getWidth()*0.7), (getHeight()*0.7));
            
            g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
            g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
            g.setColour(AlphaTheme::getInstance()->iconOnColour);
            g.drawImage (theImage,
                         (getWidth()*0.22), (getHeight()*0.22), (getWidth()*0.6), (getHeight()*0.6),
                         0, 0, theImage.getWidth(), theImage.getHeight(), true);
			
            break;
        }
			
		case 5:
        {
            
			ColourGradient fillGradient(AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), (getHeight()*0.7), AlphaTheme::getInstance()->mainColour, (getWidth()*0.5), (getHeight()*0.5), false);
			g.setGradientFill(fillGradient);
            g.fillEllipse((getWidth()*0.15), (getHeight()*0.15), (getWidth()*0.7), (getHeight()*0.7));
            
            g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
            g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
            g.setColour(AlphaTheme::getInstance()->iconOnColour);
            g.drawImage (theImage,
                         (getWidth()*0.22), (getHeight()*0.22), (getWidth()*0.6), (getHeight()*0.6),
                         0, 0, theImage.getWidth(), theImage.getHeight(), true);
			
            break;
        }
			
			
			
		default:
			break;
			
	}
	

	

}


void GuiSwitch::clicked()
{
	
}

bool GuiSwitch::hitTest (int x, int y)
{
	
	return hitPath.contains(x, y);
	
}

