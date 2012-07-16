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


GuiCircleBackground::GuiCircleBackground()
{
    //image = ImageFileFormat::loadFrom(BinaryData::circlebgthin_png, BinaryData::circlebgthin_pngSize);
    image = ImageCache::getFromMemory(BinaryData::circlebgthin_png, BinaryData::circlebgthin_pngSize);
    hitPath.addEllipse (0.0f, 0.0f, 230.0f, 230.0f);
    
}

GuiCircleBackground::~GuiCircleBackground()
{
    
}

void GuiCircleBackground::resized()
{
    hitPath.addEllipse(0.0f, 0.0f, getWidth(), getHeight());
    
}

void GuiCircleBackground::paint (Graphics &g)
{
    g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 0, image.getWidth(), image.getHeight());
    
}

bool GuiCircleBackground::hitTest (int x, int y)
{
    return hitPath.contains(x, y);
}