//
//  MainComponent.h
//  AlphaMIDI
//
//  Created by Liam Meredith-Lacey on 14/09/2011.
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
//==============================================================================
/*
    This is the main component of the application. Only one instance of this class is initialised (in MainWindow).
    This component should include most of the background and global graphics and components that are not mode-dependent.
    All graphics and components that are mode-dependent should be within their own class which that has a single instance which is initilised in this class.
 */
//==============================================================================

#ifndef H_MAINCOMPONENT
#define H_MAINCOMPONENT

#include "../../../JuceLibraryCode/JuceHeader.h"

#include "GuiMidiMode.h"
#include "GuiSamplerMode.h"
#include "GuiSequencerMode.h"
#include "GuiControllerMode.h"
#include "../../Functionality Classes/AlphaLiveEngine.h"
#include "../Custom Components/Pad/GuiPadLayout.h"
#include "../Custom Components/Graphics/GuiCircleBackground.h"
#include "../Custom Components/General/ModeButton.h"
#include "../Custom Components/General/GuiSwitch.h"
#include "../Custom Components/General/AlphaSlider.h"
#include "../Custom Components/General/AlphaRotarySlider.h"
#include "../Custom Components/General/AlphaTextButton.h"
#include "../Custom Components/Scene/SceneComponent.h"
#include "../../File and Settings/AppDocumentState.h"
#include "../../Application/AbstractSubjectAndObserver.h"
#include "../Sub-Views/GuiGlobalClock.h"
#include "../Sub-Views/AboutComponent.h"
#include "../Sub-Views/PreferencesComponent.h"
#include "../Sub-Views/ProjectSettingsComponent.h"
#include "../Custom Components/Toolbox/Toolbox.h"
#include "../Sub-Views/GuiGlobalPadSettings.h"
#include "../Custom Components/Circle Piano/GuiPiano.h"
#include "../AlphaLiveLookandFeel.h"

class MainComponent :   public Component,
                        public Button::Listener,
                        public Slider::Listener,
                        public ComboBox::Listener,
                        public Observer, //so this class can 'observe' appDocumentState
                        public ApplicationCommandTarget,
                        public Thread
{
public:
    //==============================================================================
    MainComponent(AlphaLiveEngine &ref, AppDocumentState &ref2, DocumentWindow* owner_); //need owner_ anymore??
    ~MainComponent();
    //==============================================================================
    void resized();
    void paint (Graphics &g);
    void buttonClicked (Button *button);
    void sliderValueChanged (Slider *slider);
    void comboBoxChanged (ComboBox *comboBox);
    void buttonStateChanged (Button* button);
	void sliderDragEnded (Slider *slider);
	void sliderDragStarted (Slider *slider);
    //==============================================================================
    
    //override the Observer virtual update function
    bool update(const Subject& theChangedSubject);
    
    void setCurrentlySelectedPad (Array <int> selectedPads_);
    void setToOffMode();
    void setToMidiMode();
    void setToSamplerMode();
    void setToSequencerMode();
    void setToControllerMode();
    void setGlobalPadSettingsDisplay();
    
    void run();
    void setInfoTextBoxText (String text);
    
    void mouseDown (const MouseEvent &e);
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    AlphaLiveEngine& getAlphaLiveEngineRef();
    AppDocumentState& getAppDocumentStateRef();
    
    GuiSamplerMode* getGuiSamplerMode();
    GuiSequencerMode* getGuiSequencerMode();
    GuiPiano* getGuiPiano();
    Toolbox* getToolbox();
	
    //==============================================================================
    //application command target stuff
	ApplicationCommandTarget* getNextCommandTarget();
	void getAllCommands (Array <CommandID>& commands);
	void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result);
	bool perform (const InvocationInfo& info);
    
    ApplicationCommandManager* getCommandManager(); //is this needed anymore?
    
    void setIsClockRunning (bool value);
    
    void setLocalisation();
    
    
private:
    //==============================================================================
    
    AlphaLiveLookandFeel alphaLiveLookAndFeel;
    
    Image backgroundImage, padsOff, padsOn, modeOff, padsBg;
    
    ModeButton *openButton, *saveButton;

    GuiMidiMode *guiMidiMode;
    GuiSamplerMode *guiSamplerMode;
    GuiSequencerMode *guiSequencerMode;
    GuiControllerMode *guiControllerMode;
    GuiGlobalPadSettings *guiGlobalPadSettings;
    
    AlphaLiveEngine &alphaLiveEngineRef;
    AppDocumentState &appDocumentStateRef;
    
    Array <int> selectedPads;
    
    SceneComponent *sceneComponent;
    
    TextEditor *infoTextBox;
    
    bool isInfoBoxEnabled;
    
    TextButton *killswitchButton;
    
    bool isClockRunning;
    
    LocalisedStrings *trans;
    
    DocumentWindow* owner;
    
    int noPadsSelected, noModeSelected;
    
    GuiPadLayout *guiPadLayout;
    AlphaRotarySlider *gainSlider, *panSlider;
    Label *gainPanValueLabel;

    //'rotate pad display' stuff
    Slider *padRotate;
	Image rotatedPadsImage;
	int rotateFlag;
	float offskew;
	float pivotX;
	float pivotY;
    
    //Mode buttons
    ModeButton *modeOffButton;
    ModeButton *modeSamplerButton;
	ModeButton *modeMidiButton;
	ModeButton *modeSequencerButton;
	ModeButton *modeControllerButton;
    ModeButton *globalSettingsButton;
    
    GuiGlobalClock *globalClock;
    GuiPiano *midiPiano;
    Toolbox *toolbox;
    
    //GuiSwitch *autoShowSettingsSwitch;

    AboutComponent *aboutComponent;
    PreferencesComponent *preferencesComponent;
    ProjectSettingsComponent *projectSettingsComponent;
    
    String infoBoxText;
    
    //==============================================================================
};


#endif // H_MAINCOMPONENT