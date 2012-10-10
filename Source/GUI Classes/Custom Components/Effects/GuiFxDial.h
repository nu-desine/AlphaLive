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
 The SettingsButtons array displays the GUI classes of each the effects with the following indexes:
 0 - Gain/Pan
 1 - LPF
 2 - HPF
 3 - BPF
 4 - Overdrive
 5 - Bitcrusher
 6 - Delay
 7 - Reverb
 8 - Flanger
 9 - Tremolo
 
 However in the functionality side the indexes are +1, so 0 can be used for 'no effect'
 */

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../General/SettingsButtonImage.h"
#include "Effects GUIs/FxGuiPanAndGain.h"
#include "Effects GUIs/FxGuiLowPassFilter.h"
#include "Effects GUIs/FxGuiHighPassFilter.h"
#include "Effects GUIs/FxGuiBandPassFilter.h"
#include "Effects GUIs/FxGuiDelay.h"
#include "Effects GUIs/FxGuiReverb.h"
#include "Effects GUIs/FxGuiFlanger.h"
#include "Effects GUIs/FxGuiTremolo.h"
#include "Effects GUIs/FxGuiDistortion.h"
#include "Effects GUIs/FxGuiBitcrusher.h"


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
	
    Array<int> selectedPads;
    MainComponent &mainComponentRef;
    Path hitPath;
    
	OwnedArray <SettingsButtonImage> fxButtons;
    
    //FX displays
    //should all the effect objects be put into a Juce Array and accessed through that throughout the class?
    //there seems to be a lot of 'if' checks to see which samplerEffect or buttons[] instance is there...
    //also theres lots of lists where all the effects need sending the same peice of data, which could be
    //done within a for loop if all the effects were in an array.
    //HANG ON, HOW DO YOU DO AN ARRAY OF DIFFERENT TYPES OF OBJECTS?
	//union structure with pointer?
    GuiGainAndPan *gainAndPan;
    GuiLowpassFilter *lowpassFilter;
    GuiHighPassFilter *highPassFilter;
    GuiBandPassFilter *bandPassFilter;
    GuiDelay *delay;
    GuiReverb *reverb;
    GuiFlanger *flanger;
    GuiTremolo *tremolo;
	GuiDistortion *distortion;
	GuiBitcrusher *bitcrusher;
    
	
	
	
};


#endif //GUIFXDIAL_H