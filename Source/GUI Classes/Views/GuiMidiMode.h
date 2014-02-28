//
//  TestGuiMidiMode.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 25/09/2011.
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
//

#ifndef H_TESTGUIMIDIMODE
#define H_TESTGUIMIDIMODE

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Custom Components/General/ImageSlider.h"
#include "../Custom Components/General/GuiSwitch.h"
#include "../Custom Components/Graphics/GuiCircleBackground.h"
#include "../Custom Components/General/AlphaSlider.h"
#include "../Custom Components/General/AlphaRotarySlider.h"
#include "../Custom Components/General/AlphaTextButton.h"
#include "../Custom Components/General/SettingsButton.h"
#include "../Custom Components/General/ModeButton.h"
#include "../Custom Components/General/SettingsButtonImage.h"

class MainComponent;

class GuiMidiMode :     public Component,
                        public ComboBox::Listener,
                        public Slider::Listener,
                        public Button::Listener
{
public:
    //==============================================================================
    GuiMidiMode(MainComponent &ref);
    ~GuiMidiMode();
    //==============================================================================
    void resized();
    void paint (Graphics& g);
    
    //==============================================================================
    //derived Listener functions
    void comboBoxChanged (ComboBox* comboBox);
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    void changeView (int view);
    
    //void setScale();
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    
private:
    //==============================================================================
    void setDisplay(int settingsType); //sets whether the trigger (1) or pressure (2) settings are display. 
    
    Array<int> selectedPads;
    MainComponent &mainComponentRef;
    
	//GUI Components
    ModeButton *triggerSettingsButton, *pressureSettingsButton, *quantiseButton; //quantize button is in global settings too
    AlphaRotarySlider *pressureMinRangeSlider, *pressureMaxRangeSlider;
	AlphaSlider *ccControllerSlider;
    ModeButton *indestructibleButton, *stickyButton;
    GuiSwitch *pressureStatusButton, *noteStatusButton;
    Label *parameterHoverLabel;
    
    GuiCircleBackground *notSelected;
    
    AlphaTextButton *dynamicChannelButton;
    OwnedArray<AlphaTextButton> channelButtons;
    OwnedArray<SettingsButtonImage> triggerModeButtons;
    OwnedArray<SettingsButtonImage> pressureModeButtons;
    
};


#endif //H_TESTGUIMIDIMODE