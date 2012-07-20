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
#include "../Custom Components/GuiBeatIndicator.h"
#include "../../Application/AbstractSubjectAndObserver.h"
#include "../Custom Components/General/GuiSwitch.h"

class MainComponent;

class GuiGlobalClock    :   public Component,
                            public Slider::Listener,
                            public Button::Listener,
                            public ComboBox::Listener,
                            public Observer //so this class can observe GlobalClock
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
    void comboBoxChanged (ComboBox* comboBox);
    
    void updateDisplay();
    
    //override the Observer virtual update function
    bool update(const Subject& theChangedSubject);
    
    void toggleTransportButtonOff();
    
    void triggerTransportButton();
    
private:
    MainComponent &mainComponentRef;
    AlphaLiveEngine &alphaLiveEngineRef;
    
    Label *barNumberLabel, *beatNumberLabel;
    AlphaSlider *tempoSlider, *beatsPerBarSlider;
    ComboBox *quantizationMenu;
    TextButton *transportButton;
    GuiSwitch *autoStartSwitch;
    
    GuiBeatIndicator *beatIndicator;
    ComponentAnimator *animator;
    
    
    
};

#endif //H_GUIGLOBALCLOCK