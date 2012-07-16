/*
 *  GuiCirclePiano.h
 *  AlphaLive
 *
 *  Created by Sam Davies and Liam Lacey on 28/09/2011.
 *  Copyright 2011 nu desine.
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
 *
 */

/*
How transposition/octave switching currently works in the code:
 There are 5 buttons for selecting which octave to view, which have a correspond transposeValue variable -
 - buttons[0] (-36), transposeValue - 0, note range 0-47
 - buttons[1] (-24), transposeValue - 1, note range 12-59
 - buttons[2] (0),   transposeValue - 3, note range 36-83 [DEFAULT]
 - buttons[3] (+24), transposeValue - 5, note range 60-107
 - buttons[4] (+36), transposeValue - 6, note range 72-119
 
 This layout covers almost the whole range of midi notes (though I think NI Kontakt doesn't include notes 120+ either,
 so this may be standard?) and it provides enough overlap for the scales within GuiCirclePianoForScales.cpp
 to be displayed. '-12' and '+12' aren't needed as those ranges are fully covered by the other octave ranges, but
 '-24' and '+24' are needed as the most scales go over the range of a single octave; if only -/+36 was available 
 it would be possible to create a scale that couldn't be displayed fully on the piano as there would only be a single
 octave of overlap between the different octave ranges.
 */


#ifndef GUICIRCLEPIANO_H
#define GUICIRCLEPIANO_H

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "GuiPianoKeys.h"
#include "../Effects/GuiFxButtons.h"

class GuiCirclePiano : public Component,
					   public Button::Listener

{
public:
	
	GuiCirclePiano();
	
	~GuiCirclePiano();
	
	void resized();
	void paint (Graphics& g);
	bool hitTest (int x, int y);
	void buttonClicked (Button *button);
    
    void setNote();
    
    void mouseDown (const MouseEvent &e);
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (int padNumber);
    void updateDisplay();
    
    void setNoteLabelText (int noteNumber);
	
private:
	Label *midiNoteLabel;
    int noteDisplayType; //Do i need this variable anymore?
    
	OwnedArray<Image> keyOffIm;
	OwnedArray<Image> keyOverIm;
	OwnedArray<Image> keyDownIm;
	OwnedArray<Image> keyOnIm;
	
	OwnedArray<Image> transposeOffIm;
	OwnedArray<Image> transposeOverIm;
	OwnedArray<Image> transposeDownIm;
	
	OwnedArray<GuiPianoKeys> keys;
	OwnedArray<GuiFxButtons> buttons;
	
	Image image;
	Path hitPath;
	
	
    int transposeValue; //holds the index number of the currently selected transpose/octave button 
    int keyValue; //holds the index number of the currently selected piano key button
    int note; //holds the current MIDI note number
    
    int currentlySelectedPad;
    
};


#endif //GUICIRCLEPIANO_H