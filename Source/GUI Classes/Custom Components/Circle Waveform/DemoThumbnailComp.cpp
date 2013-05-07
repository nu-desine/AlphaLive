//
//  DemoThumbnailComp.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 08/12/2011.
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

#include "DemoThumbnailComp.h"


DemoThumbnailComp::DemoThumbnailComp() :   thumbnailCache (5),
thumbnail (512, formatManager, thumbnailCache)
{
    startTime = endTime = 0;
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener (this);
}

DemoThumbnailComp::~DemoThumbnailComp()
{
    thumbnail.removeChangeListener (this);
}

void DemoThumbnailComp::setFile (const File& file)
{
    thumbnail.setSource (new FileInputSource (file));
    startTime = 0;
    endTime = thumbnail.getTotalLength();
}

void DemoThumbnailComp::setZoomFactor (double amount)
{
    if (thumbnail.getTotalLength() > 0)
    {
        double timeDisplayed = jmax (0.001, (thumbnail.getTotalLength() - startTime) * (1.0 - jlimit (0.0, 1.0, amount)));
        endTime = startTime + timeDisplayed;
        repaint();
    }
}


void DemoThumbnailComp::paint (Graphics& g)
{
    g.fillAll (Colours::transparentBlack);
    
    g.setColour (Colours::white.withAlpha(0.5f));
    
    if (thumbnail.getTotalLength() > 0)
    {
        thumbnail.drawChannel (g, getLocalBounds().reduced (2, 2),
                               startTime, endTime, 0, 1.0f);
        
        
    }
    
    //HOW SHOULD WE REPRESENT THAT THERE IS NO AUDIOFILE CURRENTLY SELECTED?
    /*
     else
     {
     g.setFont (14.0f);
     g.drawFittedText ("(No audio file selected)", 0, 0, getWidth(), getHeight(),
     Justification::centred, 2);
     }
     */
}

void DemoThumbnailComp::changeListenerCallback (ChangeBroadcaster*)
{
    // this method is called by the thumbnail when it has changed, so we should repaint it..
    repaint();
}