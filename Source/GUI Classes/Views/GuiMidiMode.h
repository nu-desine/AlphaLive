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
#include "../Custom Components/Circle Piano/GuiCirclePiano.h"
#include "../Custom Components/Circle Piano/GuiCirclePianoForScales.h"
#include "../Custom Components/Circle Piano/GuiCirclePianoOneOctave.h"
#include "../Custom Components/General/GuiSwitch.h"
#include "../Custom Components/Graphics/GuiCircleBackground.h"
#include "../Custom Components/Graphics/GuiSpeaker.h"
#include "../Custom Components/General/AlphaSlider.h"
#include "../Custom Components/General/AlphaImageKnob.h"

class MainComponent;

class GuiMidiMode : public Component,
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
    
    void setScale();
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    
    
private:
    //==============================================================================
	//GUI Components
    ComboBox *pressureModeMenu, *triggerModeMenu, *quantizeModeMenu;
    AlphaSlider *pressureMinRangeSlider, *pressureMaxRangeSlider, *ccControllerSlider, *exclusiveGroupSlider;
    TextButton *indestructibleButton, *stickyButton;
    
    //int currentlySelectedPad;
    Array<int> selectedPads;
    
    MainComponent &mainComponentRef;
    
    //==============================================================================
    //New GUI stuff!
    ImageSlider *channelSlider;
    AlphaImageKnob *velocitySlider;
    GuiCirclePiano *circlePiano;
    GuiCirclePianoForScales *circlePianoForScales;
    GuiCirclePianoOneOctave *circlePianoOneOctave;
    GuiSwitch *pressureStatusButton, *noteStatusButton;
    GuiCircleBackground *circleBackgroundLeft, *circleBackgroundRight;
    GuiSpeaker *speakerLeft, *speakerRight;
    
    
};


#endif //H_TESTGUIMIDIMODE