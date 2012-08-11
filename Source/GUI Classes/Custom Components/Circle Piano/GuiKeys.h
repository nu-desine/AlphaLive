/*
 *  GuiPianoKeys.h
 *  AlphaLive
 *
 *  Created by Sam Davies on 08/06/2012.
 *  Copyright 2012 nu desine. All rights reserved.
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

#ifndef GUIKEYS_H
#define GUIKEYS_H

#include "../../../../JuceLibraryCode/JuceHeader.h"


class GuiKeys  : public Button
{
public:
	
    GuiKeys (const String& name,
			 const Colour& normalColour,
			 const Colour& overColour,
			 const Colour& downColour);
	
	
    ~GuiKeys();
	
	void setShape (const Path& newShape,
                   bool resizeNowToFitThisShape,
                   bool maintainShapeProportions,
                   bool hasDropShadow);
	
	
    void setColours (const Colour& normalColour,
                     const Colour& overColour,
                     const Colour& downColour);
	
	
    void setOutline (const Colour& outlineColour,
                     float outlineStrokeWidth);
	
	
    void paintButton (Graphics& g,
                      bool isMouseOverButton,
                      bool isButtonDown);
	
	bool hitTest (int x, int y);
	
private:
    //==============================================================================
    Colour normalColour, overColour, downColour, outlineColour;
    DropShadowEffect shadow;
    Path shape;
    bool maintainShapeProportions;
    float outlineWidth;
	
};


#endif   // GUIKEYS_H
