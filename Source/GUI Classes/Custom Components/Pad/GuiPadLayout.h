/*
 *  GuiPadLayout.h
 *  sdaJuce
 *
 *  Created by Sam Davies on 22/09/2011.
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

#ifndef GUIPADLAYOUT_H
#define GUIPADLAYOUT_H

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "GuiPad.h"
#include "GuiPadRow.h"
#include "../../../Application/AbstractSubjectAndObserver.h"
#include "../../../Functionality Classes/AlphaLiveEngine.h"
#include "../../Views/MainComponent.h" //how come I can include this here and don't have to use a forward declaration?
//class MainComponent;

//Component class for GUI pad arrangement. Inherits from button listener to monitor buttons clicked within the component

class GuiPadLayout :    public Component,
                        public Button::Listener,
                        public KeyListener,
                        public Observer //so this class can 'observe' alphaLiveEngine

{
public:
	//take in two paramaters:
    // - a reference to the AlphaLiveEngine instance so that it can be attached as this classes 'subject'
    // - a reference to the MainComponent class so that calls to its functions can be made from here
	GuiPadLayout(AlphaLiveEngine &subject, MainComponent &ref);
	~GuiPadLayout();
	
	void resized();
	void paint (Graphics& g);
	bool hitTest (int x, int y);
	void buttonClicked (Button *button);
	void rotated (int val);
    
    void turnOn(int pad);
	void turnOff(int pad);
	void modeChange(int padNumber, int modeNumber);
	Image snapshot();
    
    //override the Observer virtual update function
    bool update(const Subject& theChangedSubject);
    void updateCurrentlySelectedPad();
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    AlphaLiveEngine& getAlphaLiveEngine();
    MainComponent& getMainComponent();
    
    void setShouldDisplaySettings (bool value);
    void deselectAllPads();
    
    //access by the menu bar/command manager in MainComponent
    //which will only be available when a single pad is selected
    void copyPadSettings();
    void pastePadSettings();
    
    bool keyPressed (const KeyPress &key, Component *originatingComponent);
	
private:
	
    //create a reference to the subject that you want this class to observe
    AlphaLiveEngine &mSubject;
    MainComponent &mainComponentRef;
    
    Array <int> selectedPads;
	OwnedArray<GuiPad> pads;
	
	Path hitPath, globalpath, row1path, row2path, row3path, row4path, row5path, row6path;
	int pStore;
	int rotateValue;
	float pivotX;
	float pivotY;
	int s;
	Colour normalColour, overColour, downColour;

	Image snap;
	GuiPadRow *globalSb, *row1Sb, *row2Sb, *row3Sb, *row4Sb, *row5Sb, *row6Sb, *row7Sb;
	
	float sin1, sin2, sinN1, sinN2, sin3, sin4, sin5, sin6, sin7, sinN3, sinN4, sinN5, sinN6, sinN7, sin8, sin9, sin10, sin11, sinN8, sinN9, sinN10, sinN11;
	float cos1, cos2, cosN1, cosN2, cos3, cos4, cos5, cos6, cos7, cosN3, cosN4, cosN5, cosN6, cosN7, cos8, cos9, cos10, cos11, cosN8, cosN9, cosN10, cosN11;
	
	unsigned char alphaThreshold;
    
    
    
    bool shouldDisplaySettings;
	

	
};


#endif //GUIPADLAYOUT_H