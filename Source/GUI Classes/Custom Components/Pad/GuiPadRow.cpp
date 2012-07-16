/*
 *  GuiPadRow.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 14/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "GuiPadRow.h"


GuiPadRow::GuiPadRow() : Button ("GuiPadRow")
{
	normalColour = Colours::transparentBlack;
	overColour = Colours::white.withAlpha(0.5f);
	downColour = Colours::white.withAlpha(0.6f);
    


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
    hitPathCover.addEllipse(30.0f, 30.0f, getWidth()-60, getHeight()-60);
    
}


void GuiPadRow::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	
	//hitPath.addEllipse (0.0f, 0.0f, getWidth(), getHeight());
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
			g.setColour(normalColour);
			//g.fillEllipse(0, 0, getWidth(), getHeight());
            g.drawEllipse(15, 15, getWidth()-30, getHeight()-30, 30);
            break;
			
		case 1:
			g.setColour(overColour);
			//g.fillEllipse(0, 0, getWidth(), getHeight());
            g.drawEllipse(15, 15, getWidth()-30, getHeight()-30, 30);
            break;
			
		case 2:
			g.setColour(downColour);
			//g.fillEllipse(0, 0, getWidth(), getHeight());
            g.drawEllipse(15, 15, getWidth()-30, getHeight()-30, 30);
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