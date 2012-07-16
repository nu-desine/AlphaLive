//
//  GuiPopUpWindow.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 27/10/2011.
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

#include "GuiPopUpWindow.h"
#include "../../Binary Data/SequencerBinaryData.h"

GuiPopUpWindow::GuiPopUpWindow()
{
    //get image
    //image = ImageFileFormat::loadFrom(SequencerBinaryData::seqbg_png, SequencerBinaryData::seqbg_pngSize);
    image = ImageCache::getFromMemory(SequencerBinaryData::seqbg_png, SequencerBinaryData::seqbg_pngSize);
    //set threshold for hitTest
    alphaThreshold = 100.0f;
    
    imageW = image.getWidth();
	imageH = image.getHeight();
    imageX = 0;
    imageY = 0;
}

GuiPopUpWindow::~GuiPopUpWindow()
{
    
}

void GuiPopUpWindow::resized()
{
    
}

void GuiPopUpWindow::paint(Graphics &g)
{
    //draw image to context
    g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 0, image.getWidth(), image.getHeight()); 
}

bool GuiPopUpWindow::hitTest (int x, int y)
{
    if (alphaThreshold == 0)
        return true;
	
    Image im (image);
	
    return  im.isNull() 
            || (imageW > 0 && imageH > 0 && alphaThreshold < im.getPixelAt (((x - imageX) * im.getWidth()) / imageW,
            ((y - imageY) * im.getHeight()) / imageH).getAlpha());
    
}