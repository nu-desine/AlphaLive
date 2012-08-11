/*
 *  GuiPadRow.h
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

#ifndef GUIPADROW_H
#define GUIPADROW_H

#include "../../../../JuceLibraryCode/JuceHeader.h"

//creates a custom circular button for use in selecting pad rows

class GuiPadRow :	public Button

{
public:

	GuiPadRow(int pathWidth);
	~GuiPadRow();
	
	void resized();
	void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
	void clicked();
	bool hitTest (int x, int y);
	

private:
	
	Path hitPath, hitPathCover;
	Colour normalColour, overColour, downColour;
    int shapeWidth;

};

#endif //GUIPADROW_H