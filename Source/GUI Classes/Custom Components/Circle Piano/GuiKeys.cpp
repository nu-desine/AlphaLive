/*
 *  GuiPianoKeys.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 08/06/2012.
 *  Copyright 2012 nu desine. All rights reserved.
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

#include "GuiKeys.h"

GuiKeys::GuiKeys (const String& text_,
				  const Colour& normalColour_,
				  const Colour& overColour_,
				  const Colour& downColour_)
: Button (text_),
normalColour (normalColour_),
overColour (overColour_),
downColour (downColour_),
maintainShapeProportions (false),
outlineWidth (0.0f)
{
}

GuiKeys::~GuiKeys()
{
	
	//deleteAllChildren();
	
}

void GuiKeys::setColours (const Colour& newNormalColour,
						  const Colour& newOverColour,
						  const Colour& newDownColour)
{
    normalColour = newNormalColour;
    overColour = newOverColour;
    downColour = newDownColour;
}

void GuiKeys::setOutline (const Colour& newOutlineColour,
						  const float newOutlineWidth)
{
    outlineColour = newOutlineColour;
    outlineWidth = newOutlineWidth;
}

void GuiKeys::setShape (const Path& newShape,
						const bool resizeNowToFitThisShape,
						const bool maintainShapeProportions_,
						const bool hasShadow)
{
    shape = newShape;
    maintainShapeProportions = maintainShapeProportions_;
	
    //shadow.setShadowProperties (3.0f, 0.5f, 0, 0); // FIX THIS
    
    
    setComponentEffect ((hasShadow) ? &shadow : 0);
	
    if (resizeNowToFitThisShape)
    {
        Rectangle<float> newBounds (shape.getBounds());
		
        if (hasShadow)
            newBounds.expand (4.0f, 4.0f);
		
        shape.applyTransform (AffineTransform::translation (-newBounds.getX(), -newBounds.getY()));
		
        setSize (1 + (int) (newBounds.getWidth() + outlineWidth),
                 1 + (int) (newBounds.getHeight() + outlineWidth));
    }
}

void GuiKeys::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    if (! isEnabled())
    {
        isMouseOverButton = false;
        isButtonDown = false;
    }
	
    g.setColour ((isButtonDown) ? downColour
				 : (isMouseOverButton) ? overColour
				 : normalColour);
	
    int w = getWidth();
    int h = getHeight();
	
    if (getComponentEffect() != nullptr)
    {
        w -= 4;
        h -= 4;
    }
	
    const float offset = (outlineWidth * 0.5f) + (isButtonDown ? 1.5f : 0.0f);
	
    const AffineTransform trans (shape.getTransformToScaleToFit (offset, offset,
                                                                 w - offset - outlineWidth,
                                                                 h - offset - outlineWidth,
                                                                 maintainShapeProportions));
    g.fillPath (shape, trans);
	
    if (outlineWidth > 0.0f)
    {
        g.setColour (outlineColour);
        g.strokePath (shape, PathStrokeType (outlineWidth), trans);
    }
}

bool GuiKeys::hitTest (int x, int y)
{
	
	return shape.contains(x, y);
}

