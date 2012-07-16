/*
 *  GuiPadLayout.h
 *  sdaJuce
 *
 *  Created by Sam Davies on 22/09/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#ifndef GUIPADLAYOUT_H
#define GUIPADLAYOUT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "GuiPad.h"
#include "GuiPadRow.h"
#include "AbstractSubjectAndObserver.h"
#include "AlphaLiveEngine.h"

class MainComponent;

//Component class for GUI pad arrangement. Inherits from button listener to monitor buttons clicked within the component

class GuiPadLayout :    public Component,
                        public Button::Listener,
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
	void turnOff(int o);
	void modeChange(int padNumber, int modeNumber);
	Image snapshot();
    
    //override the Observer virtual update function
    bool update(const Subject& theChangedSubject);
    
    //MainComponent& getMainComponent();
    void updateCurrentlySelectedPad (int padNumber);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    AlphaLiveEngine& getAlphaLiveEngine();
    MainComponent& getMainComponent();
    
    void setShouldDisplaySettings (bool value);
    
    void copyPadSettings();
    void pastePadSettings();
	
private:
	
    int currentlySelectedPad;
	OwnedArray<GuiPad> pads;
	
	Path hitPath, globalpath, row1path, row2path, row3path, row4path, row5path, row6path;
	int pStore;
	int rotateValue;
	float pivotX;
	float pivotY;
	int s;
	Colour normalColour, overColour, downColour;

	Image snap;
	GuiPadRow *globalSb, *row1Sb, *row2Sb, *row3Sb, *row4Sb, *row5Sb, *row6Sb;
	
	float sin1, sin2, sinN1, sinN2, sin3, sin4, sin5, sin6, sin7, sinN3, sinN4, sinN5, sinN6, sinN7, sin8, sin9, sin10, sin11, sinN8, sinN9, sinN10, sinN11;
	float cos1, cos2, cosN1, cosN2, cos3, cos4, cos5, cos6, cos7, cosN3, cosN4, cosN5, cosN6, cosN7, cos8, cos9, cos10, cos11, cosN8, cosN9, cosN10, cosN11;
	
	unsigned char alphaThreshold;
    
    //create a reference to the subject that you want this class to observe
    AlphaLiveEngine &mSubject;
    
    MainComponent &mainComponentRef;
    
    bool shouldDisplaySettings;
	

	
};


#endif //GUIPADLAYOUT_H