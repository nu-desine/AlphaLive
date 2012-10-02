/*
 *  GuiPad.h
 *  sdaJuce
 *
 *  Created by Sam Davies on 19/09/2011.
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


#ifndef GUIPAD_H
#define GUIPAD_H

#include "../../../../JuceLibraryCode/JuceHeader.h"

class GuiPadLayout;

//Custom component for individual pad GUI, includes a button that references a listener in GuiPadLayout and drag and drop compatibility

class GuiPad :	public Component,
                public FileDragAndDropTarget
{
public:
    
    GuiPad(int padNum, GuiPadLayout &ref);
    ~GuiPad();
	
	void resized();
	void paint (Graphics& g);
	void modeChange(int value);
	bool hitTest (int x, int y);
	void toggleChange();
	void turnOff();
	void turnOn();
	Button* getButton();
	
    // These methods implement the FileDragAndDropTarget interface, and allow the component
    // to accept drag-and-drop of files..
    bool isInterestedInFileDrag (const StringArray& files);
    void fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/);
    void fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/);
    void fileDragExit (const StringArray& /*files*/);
    void filesDropped (const StringArray& files, int /*x*/, int /*y*/);
    
    void mouseDown (const MouseEvent &e);
    void mouseUp (const MouseEvent &e);
    void mouseDrag (const MouseEvent &e);
    
    void setGradient (int oscValue);
    void setPadPlayingState (int playingState);
    
    void setPadText();
    
    void copyPadSettings();
    void pastePadSettings();
    
	
	
    
private:
    //==============================================================================
    
	/*
     unsigned char alphaThreshold;
     int imageX, imageY, imageW, imageH;
     Image image;
     float normalOpacity, overOpacity, downOpacity, onOpacity;
	 Colour &normalColour, &overColour, &downColour;
	 */
    
    int padNumber;
	
	String message, padName;
    bool somethingIsBeingDraggedOver;
    
    ShapeButton *sb;
    int i;
	Image image;//, midiImage, loopImage, seqImage, controlImage;
	Path hitPath;
	
	GuiPadLayout &guiPadLayoutRef;
    
	Colour normalColour, overColour, downColour, overlayColour, modeColour, samplerColour, midiColour, sequencerColour, controllerColour;
	
    
    Colour padColour, padOuterColour, padInnerColour, oscGradientOuterColour, oscGradientInnerColour;
    float gradientOuterColourAlpha, gradientInnerColourAlpha, modeOpacity;
	
    int lastTime;
    
    bool isMouseInPlayMode; //when true, it means the mouse is currently being used to emulate a pad press
    bool shouldIgnoreValues;
    
    int playingStatus;  //1 when a looper or sequencer pad which is currently playing.
    //when true, the pad is highlighted to show that this pad is currently playing
    //2 when waiting to play, 3 when waiting to stop. These show different colours
    //0 when not playing. Nothing is displayed
    
    
};  


#endif //GUIPAD_H