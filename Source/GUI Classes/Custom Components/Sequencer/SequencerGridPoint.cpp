//
//  SequencerGridPoint.cpp
//  sdaJuce
//
//  Created by Liam Meredith-Lacey on 14/10/2011.
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

#include "SequencerGridPoint.h"
#include "SequencerGrid.h"
#include "../../AlphaLiveLookandFeel.h"

SequencerGridPoint::SequencerGridPoint(int rowNumber, int columnNumber, SequencerGrid &ref)
                                        :   row(rowNumber),
                                            column(columnNumber),
                                            parent(ref)
{
    status = 0;
    shouldChangeVelocity = false;
	
	endRadians = (((2 * M_PI) / (SEQ_HORIZONTAL_STEPS-1)) * (column + 1)) - (0.5 * (M_PI / 180));
	startRadians = (((2 * M_PI) / (SEQ_HORIZONTAL_STEPS-1)) * column) + (0.5 * (M_PI / 180));
    
}

SequencerGridPoint::~SequencerGridPoint()
{

}

void SequencerGridPoint::resized()
{
    
    
}

void SequencerGridPoint::paint (Graphics &g)
{
    thePath.clear();
    
    if (status == 0)
        g.setColour(Colours::transparentWhite); //off
    else //1-127
    {
        /*ColourGradient seqGradient(Colours::white.withAlpha(float(status*(1.0/127.0))), (getWidth()*0.5),(getHeight()*0.5), Colours::lightgrey.withAlpha(float(status*(1.0/127.0))), (getWidth()*0.8),(getHeight()*0.8), true);
         g.setGradientFill(seqGradient);
         //g.fillEllipse((getWidth()*0.05), (getHeight()*0.05), (getWidth()*0.9), (getHeight()*0.9));
         //g.setColour(Colours::black); //on*/
		
        float alphaValue = float(status*(1.0/127.0));
        if (alphaValue < 0)
            alphaValue = 0;
        else if (alphaValue > 1.0)
            alphaValue = 1;
        
		g.setColour(AlphaColours::getInstance()->colour1.withAlpha(alphaValue));
    }
	
    
	theWidth = getWidth() - 16;
	theWidth = theWidth / getWidth();
	
	
	thePath.addPieSegment(0, 0, getWidth(), getHeight(), startRadians, endRadians, theWidth);
    
    g.fillPath(thePath, getTransform());
    
}

void SequencerGridPoint::mouseDown (const MouseEvent &e)
{
    if (e.mods.isAltDown() == true) //alt-click to change velocity
    {
        shouldChangeVelocity = true;
        initialVelocity = status;
    }
    
    else //normal click to create/delete a note
    {
        if (status == 0)
        {
            status = 110; //on with an intial velocity of 110 - should this be changeable?
            parent.setVelocityLabelText(String(status));
        }
        else
        {
            status = 0; //off
            parent.setVelocityLabelText(String::empty);
        }
        
        //send data
        parent.gridClicked(row, column, status);
    }
    
    //update display
    repaint();
    
}

void SequencerGridPoint::mouseDrag (const MouseEvent &e)
{
    if (shouldChangeVelocity == true)
    {
        status = initialVelocity - e.getDistanceFromDragStartY();
        
        if (status > 127)
            status = 127;
        else if (status < 0)
            status = 0;
        
        repaint();
        parent.setVelocityLabelText(String(status));
    }
    
}


void SequencerGridPoint::mouseUp (const MouseEvent &e)
{
    if (shouldChangeVelocity == true)
    {
        //update velocity
        parent.gridClicked(row, column, status);
        
        repaint();
        shouldChangeVelocity = false;
    }
    
}


int SequencerGridPoint::getStatus()
{
    return status;
}

void SequencerGridPoint::setStatus (int value)
{
    status = value;
    
    repaint();
}


void SequencerGridPoint::mouseEnter (const MouseEvent &e)
{
    if (status > 0) //if there is a note present
    {
        parent.setVelocityLabelText(String(status));
    }
    
}

void SequencerGridPoint::mouseExit (const MouseEvent &e)
{
    parent.setVelocityLabelText(String::empty);
}

bool SequencerGridPoint::hitTest (int x, int y)
{
	return thePath.contains(x, y);
}

