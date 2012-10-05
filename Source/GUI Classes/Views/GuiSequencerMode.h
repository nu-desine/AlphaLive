//
//  TestGuiSequencerMode.h
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

#ifndef H_TESTGUISEQUENCERMODE
#define H_TESTGUISEQUENCERMODE

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Custom Components/Sequencer/SequencerGrid.h"
#include "../../Functionality Classes/Sequencer Mode/ModeSequencer.h"
#include "../../Application/AbstractSubjectAndObserver.h"
#include "../Custom Components/General/ImageSlider.h"
#include "../Custom Components/Graphics/GuiCircleBackground.h"
#include "../Custom Components/General/ModeButton.h"
#include "../Custom Components/General/AlphaSlider.h"
#include "../../File and Settings/AppDocumentState.h"
#include "../Custom Components/General/SettingsButton.h"
#include "../Custom Components/General/GuiSwitch.h"
#include "../Custom Components/General/AlphaTextButton.h"
#include "../Custom Components/Effects/GuiFxDial.h"
#include "../Custom Components/General/AlphaPopUpButton.h"
#include "../Custom Components/General/AlphaRotarySlider.h"
#include "../Custom Components/General/SettingsButtonImage.h"
#include "../Custom Components/General/FileDropButton.h"


class MainComponent;


class GuiSequencerMode :    public Component,
                                public Button::Listener,
                                public Slider::Listener,
                                public ComboBox::Listener,
                                public Label::Listener,
                                public Observer //so this class can 'observe' modeSequencer
{
public:
    //==============================================================================
    GuiSequencerMode(ModeSequencer &ref, MainComponent &ref2, AppDocumentState &ref3);
    ~GuiSequencerMode();
    
    void resized();
    void paint (Graphics& g);
    
    //==============================================================================
    //derived Listener functions
    void comboBoxChanged (ComboBox* comboBox);
    void sliderDragStarted (Slider* slider);
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    void labelTextChanged (Label* labelThatHasChanged);	
    
    //override the Observer virtual update function
    bool update(const Subject& theChangedSubject);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    //==============================================================================
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    
    void setNoteLengthSliderRange (int maxValue);
    
    void updateDisplay();
    void hideComponents();
    void setDisplay (int settingsType); // 1 = trigger settings, 2 = pressure settings, 3 = sequence settings
    void setRotaryControlDisplay();
    
    void setParameterLabelText (String value);
    void setCurrentSequenceNumber();
    
    int getCurrentSequenceNumber();
    
private:
    
    Array<int> selectedPads;
    ModeSequencer &modeSequencerRef;
    MainComponent &mainComponentRef;
    AppDocumentState &appDocumentStateRef;
    
    int controlDisplayId; //used to set which rotary dial is display in the centre
    int sequenceLength; //this is controlled by the plus/minus buttons when in viewing sequencer settings
    int currentSequenceNumber; //this is controlled by the plus/minus buttons on the side
    
    SequencerGrid *sequencerGrid;
    GuiFxDial *fxDial;
    GuiCircleBackground *notSelected;
   
    ModeButton *triggerSettingsButton, *pressureSettingsButton, *sequenceSettingsButton, *quantiseButton;
    AlphaTextButton *nextSequenceButton, *previousSequenceButton;
    ModeButton *loopButton, *indestructibleButton, *finishLoopButton, *stickyButton, *linkButton;
	
	ModeButton *modeMidiButton, *modeSamplesButton;
  
    
	OwnedArray<AlphaTextButton> midiChannelButtons;
	OwnedArray<FileDropButton> audioRowButtons;
	
    OwnedArray<SettingsButtonImage> triggerModeButtons;
    OwnedArray<SettingsButtonImage> midiPressureModeButtons;

    SettingsButton *plusButton, *minusButton;
    GuiSwitch *pressureStatusButton;
    AlphaPopUpButton *popUpButton;
    DrawableButton *previewButton, *recordButton;

    AlphaRotarySlider *numberOfSequencesSlider, *midiPressureMinRangeSlider, *midiPressureMaxRangeSlider, *relativeTempoSlider, *noteLengthSlider, *audioGainSlider, *audioPanSlider, *audioAttackSlider;
    AlphaSlider *ccControllerSlider;
    
    Label *parameterLabel, *currentParameterLabel, *parameterHoverLabel;
    
     
};

#endif