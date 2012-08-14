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
    
}

bool GuiCircleBackground::hitTest (int x, int y)
{
	
	return hitPath.contains(x, y);
	
}
