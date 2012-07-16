/*
 *  GuiSwitch.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 25/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "GuiSwitch.h"
#include "../../Binary Data/BinaryData.h"

GuiSwitch::GuiSwitch() : Button ("GuiSwitch")

{
	
	setClickingTogglesState(true);
    //offIm = ImageFileFormat::loadFrom (BinaryData::switchoff_png, BinaryData::switchoff_pngSize);
    offIm = ImageCache::getFromMemory(BinaryData::switchoff_png, BinaryData::switchoff_pngSize);
    //onIm = ImageFileFormat::loadFrom (BinaryData::switchon_png, BinaryData::switchon_pngSize);
    onIm = ImageCache::getFromMemory(BinaryData::switchon_png, BinaryData::switchon_pngSize);
	
}

GuiSwitch::~GuiSwitch()
{
	
	
	
}

void GuiSwitch::resized()
{
	
	hitPath.addEllipse (0.0f, 0.0f, getWidth(), getHeight());
	
	
}

void GuiSwitch::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	
	hitPath.addEllipse (0.0f, 0.0f, getWidth(), getHeight());
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            
			g.drawImage (offIm,
                         0, 0, getWidth(), getHeight(),
                         0, 0, offIm.getWidth(), offIm.getHeight(), false);
			
            break;
        }
			
		case 1:
        {
            
			g.drawImage (offIm,
                         0, 0, getWidth(), getHeight(),
                         0, 0, offIm.getWidth(), offIm.getHeight(), false);
			
            break;
        }
			
		case 2:
        {
            
			g.drawImage (onIm,
                         0, 0, getWidth(), getHeight(),
                         0, 0, offIm.getWidth(), offIm.getHeight(), false);
			
            break;
        }
			
		case 3:
        {
            
			g.drawImage (onIm,
                         0, 0, getWidth(), getHeight(),
                         0, 0, offIm.getWidth(), offIm.getHeight(), false);
			
            break;
        }
			
		case 4:
        {
            
			g.drawImage (onIm,
                         0, 0, getWidth(), getHeight(),
                         0, 0, offIm.getWidth(), offIm.getHeight(), false);
			
            break;
        }
			
		case 5:
        {
            
			g.drawImage (offIm,
                         0, 0, getWidth(), getHeight(),
                         0, 0, offIm.getWidth(), offIm.getHeight(), false);
			
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

