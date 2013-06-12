//
//  GuiGlobalClock.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 09/02/2012.
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

#ifndef H_GUIGLOBALCLOCK
#define H_GUIGLOBALCLOCK

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Functionality Classes/AlphaLiveEngine.h"
#include "../Custom Components/General/AlphaSlider.h"
#include "../Custom Components/General/SettingsButton.h"
#include "../Custom Components/General/AlphaTextButton.h"
#include "../Custom Components/General/ModeButton.h"

class MainComponent;

class GuiGlobalClock    :   public Component,
                            public Slider::Listener,
                            public Button::Listener
{
public:
    GuiGlobalClock(MainComponent &ref, AlphaLiveEngine &ref2);
    ~GuiGlobalClock();
    
    void resized();
    void paint (Graphics& g);
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    
    void updateDisplay();
    
    void updateClockDisplay (int beatNumber, int barNumber, int beatsPerBar);
    void updateTransportButtonDisplay (bool status);
    void updateTempoDisplay (float value);
    
    void toggleTransportButtonOff();
    
    void triggerTransportButton();
    
    Slider* getTempoSlider();
    Button* getMetronomeButton();
    
private:
    MainComponent &mainComponentRef;
    AlphaLiveEngine &alphaLiveEngineRef;
    
    AlphaSlider *tempoSlider;
	
	int currentStepNumber;
    int beatsPerBar_;
	
	float countGap, countSeg, segStart, segEnd;
	
	Path barCount;
	
    AlphaTextButton *transportButton;
    
	OwnedArray <AlphaTextButton> beatsPerBarButtons;
    OwnedArray <SettingsButton> quantizationValueButtons;
    
    ModeButton *metronomeButton, *autoStartSwitch;
};

#endif //H_GUIGLOBALCLOCK