//
//  GuiCircleBackground.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 26/10/2011.
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
//

#include "GuiCircleBackground.h"
#include "../../Binary Data/BinaryData.h"
#include "../../Binary Data/BinaryDataNew.h"
#include "../../AlphaLiveLookandFeel.h"

GuiCircleBackground::GuiCircleBackground()
{
    
    image = ImageCache::getFromMemory(BinaryDataNew::statusoff_png, BinaryDataNew::statusoff_pngSize);
    
	i = 0;
    
}

GuiCircleBackground::~GuiCircleBackground()
{
    
}

void GuiCircleBackground::resized()
{
    
	hitPath.addEllipse (683, 261, 324, 324);
    
}

void GuiCircleBackground::paint (Graphics &g)
{
    g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 0, image.getWidth(), image.getHeight());
	
	ColourGradient fillGradient(AlphaColours::nearlyblack,845 , 461, Colours::black, 845 , 383, false);
	g.setGradientFill(fillGradient);
	
	g.fillEllipse(802, 379, 86, 86);
	
	if (i == 1) {
		
		Path linkButtonBg;
		
		linkButtonBg.addCentredArc(805, 223, 72, 72, 0, (123 * (M_PI / 180)), (214 * (M_PI / 180)), true);
		linkButtonBg.addCentredArc(845, 423, 162, 162, 0, (330 * (M_PI / 180)), (367 * (M_PI / 180)), false);
														   
		
		g.setColour(Colours::black);
		g.fillPath(linkButtonBg, getTransform());
		g.fillEllipse(796,261, 46, 46);
		
		g.setColour(Colours::grey.withAlpha(0.3f));
		g.drawEllipse(800,265, 38, 38, 1.0f);
		
		linkButtonBg.clear();
		
	}
    
}

bool GuiCircleBackground::hitTest (int x, int y)
{
	
	return hitPath.contains(x, y);
	
}

void GuiCircleBackground::drawButtonBackground()
{
	
	i = 1;
	repaint();
}
