/*
 *  SettingsButtonImage.h
 *  AlphaLive
 *
 *  Created by Sam Davies on 13/06/2012.
 //  Copyright 2012 nu desine.
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

#ifndef SETTINGSBUTTONIMAGE_H
#define SETTINGSBUTTONIMAGE_H

#include "../../../../JuceLibraryCode/JuceHeader.h"

class SettingsButtonImage :	public Button
{
public:
    SettingsButtonImage(Image* _theImage, float _startRadians, float _endRadians, float _theWidth, float _imageWidth);
    ~SettingsButtonImage();

	void resized();
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
    void clicked();
    void buttonStateChanged();
	bool hitTest (int x, int y);

private:
    
	float normalOpacity, overOpacity, downOpacity, onOpacity;
    Colour normalOverlay, overOverlay, downOverlay, onOverlay;
	
	Image *theImage;
	
	float startRadians, endRadians, theWidth, imageWidth;
	
	int i;
	
	Path thePath;	
};    


#endif //SETTINGSBUTTONIMAGE_H