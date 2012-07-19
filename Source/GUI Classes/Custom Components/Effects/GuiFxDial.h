/*
 *  GuiFxDial.h
 *  sdaJuce
 *
 *  Created by Sam Davies on 19/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#ifndef GUIFXDIAL_H
#define GUIFXDIAL_H

/*
 The buttons array 'show' the Fx controls for each effect with the following indexes:
 0 - Off
 1 - Gain/Pan
 2 - LPF
 3 - HPF
 4 - BPF
 5 - Distortion
 6 - Delay
 7 - Reverb
 8 - Chorus
 9 - Flanger
 10 - Tremolo
 11 - Vibrato
 12 - Pitchshifter
 */

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "GuiFxButtons.h"
#include "../General/AlphaImageKnob.h"
#include "Effects GUIs/FxGuiPanAndGain.h"
#include "Effects GUIs/FxGuiLowPassFilter.h"
#include "Effects GUIs/FxGuiHighPassFilter.h"
#include "Effects GUIs/FxGuiBandPassFilter.h"
#include "Effects GUIs/FxGuiDelay.h"
#include "Effects GUIs/FxGuiReverb.h"
#include "Effects GUIs/FxGuiFlanger.h"
#include "Effects GUIs/FxGuiTremolo.h"

class MainComponent;

class GuiFxDial :   public Component,
                    public Button::Listener,
                    public Slider::Listener
{
public:
	GuiFxDial(MainComponent &ref);
	~GuiFxDial();
	
	void resized();
	void paint (Graphics& g);
	bool hitTest (int x, int y);
	void buttonClicked (Button *button);
    void sliderValueChanged (Slider *slider);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    void hideAllFx();
	
private:
	
	OwnedArray<Image> keyOffIm;
	OwnedArray<Image> keyOverIm;
	OwnedArray<Image> keyDownIm;
	OwnedArray<Image> bgIm;
	
	OwnedArray<GuiFxButtons> buttons;
	
	Image image;
	Image im;
	
	
	Path hitPath;
    
    //int currentlySelectedPad;
    Array<int> selectedPads;
    
    MainComponent &mainComponentRef;
    
    //FX displays
    //should all the effect objects be put into a Juce Array and accessed through that throughout the class?
    //there seems to be a lot of 'if' checks to see which looperEffect or buttons[] instance is there...
    //also theres lots of lists where all the effects need sending the same peice of data, which could be
    //done within a for loop if all the effects were in an array.
    //HANG ON, HOW DO YOU DO AN ARRAY OF DIFFERENT TYPES OF OBJECTS?
    GuiGainAndPan *gainAndPan;
    GuiLowpassFilter *lowpassFilter;
    GuiHighPassFilter *highPassFilter;
    GuiBandPassFilter *bandPassFilter;
    GuiDelay *delay;
    GuiReverb *reverb;
    GuiFlanger *flanger;
    GuiTremolo *tremolo;
    
	
	
	
};


#endif //GUIFXDIAL_H