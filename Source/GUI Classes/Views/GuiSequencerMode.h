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

class MainComponent;


class GuiSequencerMode :    public Component,
                                public Button::Listener,
                                public Slider::Listener,
                                public ComboBox::Listener,
                                public Observer //so this class can 'observe' modeSequencer
{
public:
    //==============================================================================
    GuiSequencerMode(ModeSequencer &ref, MainComponent &ref2, AppDocumentState &ref3);
    ~GuiSequencerMode();
    
    void resized();
    
    //==============================================================================
    //derived Listener functions
    void comboBoxChanged (ComboBox* comboBox);
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    //bool hitTest (int x, int y);
    
    //override the Observer virtual update function
    bool update(const Subject& theChangedSubject);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    //==============================================================================
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    
    void setToMidiMode();
    void setToSamplesMode();
    
    void setVelocityLabelText (String velocity);
    
    
private:
    
    GuiSeqMidiMode *midiMode;
    GuiSeqSamplesMode *samplesMode;
    SequencerGrid *sequencerGrid;
    
    TextButton *clearButton, *clearAllButton, *saveSeqButton, *saveSeqSetButton, *loadSeqButton, *loadSeqSetButton;
    AlphaSlider *currentSequenceNumberSlider, *numberOfSequencesSlider, *sequenceLengthSlider;
    ComboBox *triggerModeMenu, *quantizeModeMenu, *relativeTempoMenu;
    Label *velocityLabel;
    TextButton *loopButton, *indestructibleButton, *finishLoopButton, *stickyButton, *linkButton;
    
    //int currentlySelectedPad;
    Array<int> selectedPads;
    
    ModeSequencer &modeSequencerRef;
    MainComponent &mainComponentRef;
    AppDocumentState &appDocumentStateRef;
    
    //New GUI stuff!
    GuiCircleBackground *circleBackgroundLeft, *circleBackgroundRight;
    ModeButton *modeMidiButton, *modeSamplesButton;
    ModeButton *sequencerGridToggleButton;
    GuiPopUpWindow *popUpWindow;
    
    //int lastTime;
    
};

#endif