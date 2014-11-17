/*
 *  GuiPadRow.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 14/10/2011.
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
 */

#include "GuiPadRow.h"


GuiPadRow::GuiPadRow(int pathWidth) : Button ("GuiPadRow"),
                                        shapeWidth(pathWidth)
{
	normalColour = Colours::transparentBlack;
	overColour = Colours::white.withAlpha(0.2f);
	downColour = Colours::white.withAlpha(0.3f);
}

GuiPadRow::~GuiPadRow()
{
}

void GuiPadRow::resized()
{
    //use two INVISIBLE circles (paths) to create the true 'hitPath' for the hitTest method,
    //which will theorectically be the outline/edge of the bigger circle (hitPath).
    //This is done by using a smaller circle (hitPathCover) to 'cover' all but the edge
    //of the bigger circle.
    hitPath.addEllipse (0.0f, 0.0f, getWidth(), getHeight());
    //the position of this circle needs to make the hitPath circle the same size as
    //the visible circles drawn within the paintButton() function below
    hitPathCover.addEllipse(shapeWidth*0.5, shapeWidth*0.5, getWidth()-shapeWidth, getHeight()-shapeWidth);
    
}


void GuiPadRow::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
			g.setColour(normalColour);
			g.drawEllipse(3, 3, getWidth()-6, getHeight()-6, 6);
            break;
			
		case 1:
			g.setColour(overColour);
            g.drawEllipse(3, 3, getWidth()-6, getHeight()-6, 6);
            break;
			
		case 2:
			g.setColour(downColour);
			g.drawEllipse(3, 3, getWidth()-6, getHeight()-6, 6);  
			break;
            
		default:
			break;
            
	}
	
	
}

void GuiPadRow::clicked()
{
	
}

bool GuiPadRow::hitTest (int x, int y)
{

    bool overHitPathCover = hitPathCover.contains(x, y);
    bool overHitPath = hitPath.contains(x, y);

    if (overHitPathCover == false && overHitPath == true)
        return true;
    else 
        return false;
	
}