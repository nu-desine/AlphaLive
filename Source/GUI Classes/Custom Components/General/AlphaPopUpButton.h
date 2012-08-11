/*
 *  AlphaPopUpButton.h
 *  AlphaLive
 *
 *  Created by Sam Davies on 09/08/2012.
 
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

#ifndef ALPHAPOPUPBUTTON_H
#define ALPHAPOPUPBUTTON_H

#include "../../../../JuceLibraryCode/JuceHeader.h"


class AlphaPopUpButton :	public TextButton

{
public:
	
	AlphaPopUpButton();
	~AlphaPopUpButton();
	
	void resized();
	void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
	void clicked();
	bool hitTest (int x, int y);
	
	
private:
	
	Path hitPath;
	
	
};

#endif //ALPHAPOPUPBUTTON_H