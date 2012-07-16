//
//  SequencerGridPlayHead.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 20/10/2011.
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

#include "SequencerGridPlayHead.h"


SequencerGridPlayHead::SequencerGridPlayHead()
{
    linePosition = 0;
    
}

SequencerGridPlayHead::~SequencerGridPlayHead()
{
    
}

void SequencerGridPlayHead::resized()
{
    
}

void SequencerGridPlayHead::paint (Graphics &g)
{
  
    g.setColour(Colours::red);
    
    g.drawLine(linePosition, 0.0, linePosition, getHeight(), 5.0);
}

void SequencerGridPlayHead::setLinePostion (float position)
{
    linePosition = position;
    repaint();
}
