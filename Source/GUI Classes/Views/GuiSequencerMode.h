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
#include "GuiSeqMidiMode.h"
#include "GuiSeqSamplesMode.h"
#include "../Custom Components/Sequencer/SequencerGrid.h"
#include "ModeSequencer.h"
#include "../../Application/AbstractSubjectAndObserver.h"
#include "../Custom Components/General/ImageSlider.h"
#include "../Custom Components/Graphics/GuiCircleBackground.h"
#include "../Custom Components/General/ModeButton.h"
#include "../Custom Components/Graphics/GuiPopUpWindow.h"
#include "../Custom Components/General/AlphaSlider.h"
#include "../../File and Settings/AppDocumentState.h"
#include "../Custom Components/General/SettingsButton.h"
#include "../Custom Components/General/GuiSwitch.h"
#include "../Custom Components/General/AlphaTextButton.h"
#include "../Custom Components/Effects/GuiFxDial.h"
#include "../Custom Components/General/AlphaPopUpButton.h"
#include "../Custom Components/General/AlphaRotarySlider.h"
#include "../Custom Components/General/SettingsButtonImage.h"

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
    void setCurrentSequenceNumber();
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    
    void hideComponents();
    void setDisplay (int settingsType); // 1 = trigger settings, 2 = pressure settings, 3 = sequence settings
    void setRotaryControlDisplay();
    
    void setToMidiMode();
    void setToSamplesMode();
    
    void setParameterLabelText (String value);
    
    
private:
    
    Array<int> selectedPads;
    ModeSequencer &modeSequencerRef;
    MainComponent &mainComponentRef;
    AppDocumentState &appDocumentStateRef;
    
    int controlDisplayId; //used to set which rotary dial is display in the centre
    
    //GuiSeqMidiMode *midiMode;
    //GuiSeqSamplesMode *samplesMode;
    SequencerGrid *sequencerGrid;
    
    //TextButton *clearButton, *clearAllButton, *saveSeqButton, *saveSeqSetButton, *loadSeqButton, *loadSeqSetButton;
    
    //AlphaSlider *currentSequenceNumberSlider, *sequenceLengthSlider; //*numberOfSequencesSlider;
    //ComboBox *triggerModeMenu, *relativeTempoMenu;
    //Label *velocityLabel;
    Label *parameterLabel, *currentParameterLabel;
    
    int sequenceLength; //this is controlled by the plus/minus buttons when in viewing sequencer settings
    int currentSequenceNumber;
    
    ModeButton *triggerSettingsButton, *pressureSettingsButton, *sequenceSettingsButton, *quantiseButton;
    AlphaTextButton *nextSequenceButton, *previousSequenceButton;
    ModeButton *loopButton, *indestructibleButton, *finishLoopButton, *stickyButton, *linkButton;
	
	ModeButton *modeMidiButton, *modeSamplesButton;
    
    AlphaRotarySlider *numberOfSequencesSlider, *relativeTempoSlider, *noteLengthSlider, *audioGainSlider, *audioPanSlider;
    
    //GuiCircleBackground *circleBackgroundLeft, *circleBackgroundRight;
    GuiCircleBackground *notSelected;
    
    GuiFxDial *fxDial;
    
    /*
    ModeButton *modeMidiButton, *modeSamplesButton;
    ModeButton *sequencerGridToggleButton;
    GuiPopUpWindow *popUpWindow;
     */
    
    OwnedArray<SettingsButtonImage> triggerModeButtons;
    OwnedArray<SettingsButton> midiPressureModeButtons;
    OwnedArray<AlphaTextButton> channelButtons;
	OwnedArray<AlphaTextButton> audioRowButtons;
    
    GuiSwitch *pressureStatusButton;
    
    SettingsButton *plusButton, *minusButton;
	DrawableButton *previewButton;
	
	AlphaSlider *ccControllerSlider;
	//Label *sequenceLength;
	AlphaPopUpButton *popUpButton;
    
};

#endif