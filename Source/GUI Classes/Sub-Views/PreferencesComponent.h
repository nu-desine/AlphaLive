//
//  PreferencesComponent.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 22/05/2012.
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

#ifndef H_PREFERENCESCOMPONENT
#define H_PREFERENCESCOMPONENT

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Custom Components/General/AlphaAudioSettingsComponent.h"
#include "../../Functionality Classes/AlphaLiveEngine.h"
#include "../../File and Settings/StoredSettings.h"

class MainComponent;
class GeneralSettingsComponent;

class PreferencesComponent :    public Component,
                                public Button::Listener,
                                public KeyListener
{
public:
    PreferencesComponent(MainComponent &ref, AlphaLiveEngine &ref2);
    ~PreferencesComponent();
    
    void resized();
    void paint (Graphics& g);
    void visibilityChanged();
    
    void buttonClicked (Button *button);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    bool keyPressed (const KeyPress &key, Component *originatingComponent);
    
    void removeMidiOutputSelector();
    
private:
    MainComponent &mainComponentRef;
    AlphaLiveEngine &alphaLiveEngineRef;
    
    TabbedComponent *tabbedComponent;
    
    AlphaAudioSettingsComponent *audioAndMidiSettingsComponent;
    GeneralSettingsComponent *generalSettingsComponent;
    
    TextButton *closeButton;

};

#endif //H_PREFERENCESCOMPONENT






#ifndef H_GENERALSETTINGSCOMPONENT
#define H_GENERALSETTINGSCOMPONENT

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Functionality Classes/AlphaLiveEngine.h"

class MainComponent;

class GeneralSettingsComponent :    public Component,
                                    public ComboBox::Listener,
                                    public Button::Listener
{
public:
    GeneralSettingsComponent(MainComponent &ref, AlphaLiveEngine &ref2);
    ~GeneralSettingsComponent();
    
    void resized();
    void paint (Graphics& g);
    
    void buttonClicked (Button* button);
    void comboBoxChanged (ComboBox *comboBox);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void updateDisplay();
    
private:
    MainComponent &mainComponentRef;
    AlphaLiveEngine &alphaLiveEngineRef;

    
    Label *directoryLabel, *midiNoteDisplayTypeLabel, *launchTaskLabel;
    Label *killOnClockStopLabel, *cleanOnCloseLabel, *autoSaveScenesLabel;
    Label *deviceInterfaceLabel, *autoCheckUpdatesLabel;
    ComboBox *midiNoteDisplayTypeMenu, *launchTaskMenu, *appProjectDirChooser;
    TextButton *killOnClockStopButton, *cleanOnCloseButton, *autoSaveScenesButton;
    TextButton *autoCheckUpdatesButton;
    ComboBox *deviceInterfaceMenu;

};

#endif //H_GENERALSETTINGSCOMPONENT
