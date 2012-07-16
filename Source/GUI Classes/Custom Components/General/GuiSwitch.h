/*
 *  GuiSwitch.h
 *  sdaJuce
 *
 *  Created by Sam Davies on 25/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */
#ifndef GUISWITCH_H
#define GUISWITCH_H

#include "../../../../JuceLibraryCode/JuceHeader.h"


class GuiSwitch :	public Button

{
public:
	
	GuiSwitch();
	~GuiSwitch();
	
	void resized();
	void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
	void clicked();
	bool hitTest (int x, int y);
	
	
private:
	
	Path hitPath;
	Image offIm, onIm;
	
};

#endif //GUISWITCH_H
