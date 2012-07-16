//
//  GuiCirclePianoOneOctave.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 09/01/2012.
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

#ifndef GUICIRCLEPIANOONEOCTAVE_H
#define GUICIRCLEPIANOONEOCTAVE_H

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "GuiPianoKeys.h"
#include "../Effects/GuiFxButtons.h"

class GuiCirclePianoOneOctave : public Component,
                                public Button::Listener,
                                public ComboBox::Listener

{
public:
	
	GuiCirclePianoOneOctave();
	~GuiCirclePianoOneOctave();
	
	void resized();
	void paint (Graphics& g);
	bool hitTest (int x, int y);
	void buttonClicked (Button *button);
    void comboBoxChanged (ComboBox* comboBox);
    
    void setLayout();
    
    void setCurrentlySelectedPad (int padNumber);
    void updateDisplay();
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
	
private:
    
    ComboBox *layoutMenu;
    Label *keyLabel;
	
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
	
    int octaveValue;
    int keyValue;
    int note;
    
    int currentlySelectedPad;
	
};


#endif //GUICIRCLEPIANOONEOCTAVE_H
