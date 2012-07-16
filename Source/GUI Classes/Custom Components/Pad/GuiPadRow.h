/*
 *  GuiPadRow.h
 *  sdaJuce
 *
 *  Created by Sam Davies on 14/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#ifndef GUIPADROW_H
#define GUIPADROW_H

#include "../JuceLibraryCode/JuceHeader.h"

//creates a custom circular button for use in selecting pad rows

class GuiPadRow :	public Button

{
public:

	GuiPadRow();
	~GuiPadRow();
	
	void resized();
	void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
	void clicked();
	bool hitTest (int x, int y);
	

private:
	
	Path hitPath, hitPathCover;
	Colour normalColour, overColour, downColour;

};

#endif //GUIPADROW_H