/*
 *  GuiPiano.h
 *  AlphaLive
 *
 *  Created by Sam Davies on 07/06/2012.
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

#ifndef GUIPIANO_H
#define GUIPIANO_H

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "GuiKeys.h"



class GuiPiano : public Component,
public Button::Listener

{
public:
	
	GuiPiano();
	
	~GuiPiano();
	
	void resized();
	void paint (Graphics& g);
	void buttonClicked (Button *button);
    
    void setNote();
   // bool hitTest (int x, int y);
    void mouseDown (const MouseEvent &e);
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    
    void setNoteDisplayType (int value);
    void setNoteLabelText (int noteNumber);
    
    void setActive (bool value);
    void setKeyDisplay (int keyNumber, bool isOn);
	
private:
    Array <int> selectedPads;
    
	Label *midiNoteLabel;
    int noteDisplayType; //0 = MIDI Note Number, 1 = MIDI Note Name
    
	OwnedArray<Path> keysPath;
	OwnedArray<GuiKeys> keys;
	Array <int> selectedKeys;
    
	Path hitPath;
	
	String keyNumber;
	
    int noteNumber; //holds the current MIDI note number and key index number
    
	float keyWidth, keySegWhite, keyGapWhite, keySegBlack, keyGapBlack, segStartWhite, segStartBlack, segEndWhite, segEndBlack;
    
    bool recentlyUpdated;
};


#endif //GUIPIANO_H

