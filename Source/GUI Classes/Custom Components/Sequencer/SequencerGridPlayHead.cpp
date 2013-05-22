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
    setLinePostion(0);
    
}

SequencerGridPlayHead::~SequencerGridPlayHead()
{
    
}

void SequencerGridPlayHead::resized()
{
    
}

void SequencerGridPlayHead::paint (Graphics &g)
{
    g.setColour(Colours::grey.withAlpha(0.3f));
	g.fillPath(playheadPath, getTransform());
}

void SequencerGridPlayHead::setLinePostion (float position)
{
    //we deal with two Paths here:
    // - playheadPath - the path of the actual playhead that is display
    // - paintPath - the path of the area that needs to be repainted when
    // updating the display of the playhead
    
    playheadPath.clear();
    Path paintPath;
	
    //add the previous playhead position to paintPath
	float theAngle = ((2 * M_PI) / (SEQ_HORIZONTAL_STEPS-1)) * linePosition;
	float theAngleEnd = ((2 * M_PI) / (SEQ_HORIZONTAL_STEPS-1)) * (linePosition-1);
	paintPath.addPieSegment(0, 0, getWidth(), getHeight(), theAngleEnd, theAngle, 0.3f);
    
    //set the new linePosition
    linePosition = position;
    
    //add the new playhead position the both the playheadPath and paintPath
    theAngle = ((2 * M_PI) / (SEQ_HORIZONTAL_STEPS-1)) * linePosition;
	theAngleEnd = ((2 * M_PI) / (SEQ_HORIZONTAL_STEPS-1)) * (linePosition-1);
	playheadPath.addPieSegment(0, 0, getWidth(), getHeight(), theAngleEnd, theAngle, 0.3f);
    paintPath.addPieSegment(0, 0, getWidth(), getHeight(), theAngleEnd, theAngle, 0.3f);
    
    //repaint using the paintPath bounds
    repaint(paintPath.getBounds().getSmallestIntegerContainer());
}
