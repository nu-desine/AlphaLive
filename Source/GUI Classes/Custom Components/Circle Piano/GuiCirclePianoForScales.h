//
//  GuiCirclePianoForScales.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/01/2012.
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
//
/*
 How transposition/octave switching currently works in the code: - see GuiCirclePiano.h
 */
#ifndef GUICIRCLEPIANOFORSCALES_H
#define GUICIRCLEPIANOFORSCALES_H

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "GuiPianoKeys.h"
#include "../Effects/GuiFxButtons.h"

class GuiCirclePianoForScales : public Component,
                                public Button::Listener,
                                public ComboBox::Listener

{
public:
	
	GuiCirclePianoForScales();
	
	~GuiCirclePianoForScales();
	
	void resized();
	void paint (Graphics& g);
	bool hitTest (int x, int y);
	void buttonClicked (Button *button);
    void comboBoxChanged (ComboBox *comboBox);
    
    void setScale();
    
    void mouseDown (const MouseEvent &e);
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (int padNumber);
    void updateDisplay();
    
    void setNoteLabelText (int noteNumber);
	
private:
    ComboBox *scaleMenu;
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
    int rootNote; //holds the current MIDI root note number of the scale
    
    int currentlySelectedPad;
};


#endif //GUICIRCLEPIANOFORSCALES_H