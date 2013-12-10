//
//  ProjectSettingsComponent.h
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

#ifndef H_PROJECTSETTINGSCOMPONENT
#define H_PROJECTSETTINGSCOMPONENT

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Functionality Classes/AlphaLiveEngine.h"
#include "../../File and Settings/AppDocumentState.h"


class MainComponent;
class GlobalOscComponent;
class GeneralProjSettingsComponent;

class ProjectSettingsComponent :    public Component,
                                    public Button::Listener,
                                    public KeyListener
{
public:
    ProjectSettingsComponent(MainComponent &ref, AlphaLiveEngine &ref2, AppDocumentState &ref3);
    ~ProjectSettingsComponent();
    
    void resized();
    void paint (Graphics& g);
    
    void buttonClicked (Button *button);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    bool keyPressed (const KeyPress &key, Component *originatingComponent);
    
    void updateDisplay();
    void setTabColour();
    
private:
    MainComponent &mainComponentRef;
    AlphaLiveEngine &alphaLiveEngineRef;
    AppDocumentState &appDocumentStateRef;
    
    TabbedComponent *tabbedComponent;
    GlobalOscComponent *globalOscComponent;
    GeneralProjSettingsComponent *generalSettingsComponent;
    
    TextButton *closeButton;
    
};

#endif //H_PROJECTSETTINGSCOMPONENT








#ifndef H_GLOBALOSCCOMPONENT
#define H_GLOBALOSCCOMPONENT

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Functionality Classes/AlphaLiveEngine.h"
#include "../Custom Components/General/AlphaSlider.h"

class MainComponent;

class GlobalOscComponent :  public Component,
                            public Button::Listener,
                            public Slider::Listener,
                            public Label::Listener
{
public:
    GlobalOscComponent(MainComponent &ref, AlphaLiveEngine &ref2);
    ~GlobalOscComponent();
    
    void resized();
    void paint (Graphics& g);
    
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    void labelTextChanged (Label* labelThatHasChanged);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
private:
    MainComponent &mainComponentRef;
    AlphaLiveEngine &alphaLiveEngineRef;
    
    TextButton *globalOscSwitch;
    Label *globalOscLabel, *oscIpAddressEditor;
    AlphaSlider *oscPortNumberSlider;
    
};

#endif //H_GLOBALOSCCOMPONENT











#ifndef H_GENERALPROJSETTINGSCOMPONENT
#define H_GENERALPROJSETTINGSCOMPONENT

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../File and Settings/AppDocumentState.h"
//#include "AlphaLiveEngine.h"
//#include "AlphaSlider.h"

class MainComponent;

class GeneralProjSettingsComponent :    public Component,
                                        public Button::Listener,
                                        public Slider::Listener,
                                        public ComboBox::Listener
{
public:
    GeneralProjSettingsComponent(MainComponent &ref, AppDocumentState &ref2);
    ~GeneralProjSettingsComponent();
    
    void resized();
    void paint (Graphics& g);
    
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    void comboBoxChanged (ComboBox *comboBox);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void updateDisplay();
    void setDisplay();
    
private:
    MainComponent &mainComponentRef;
    AppDocumentState &appDocumentStateRef;
    //AlphaLiveEngine &alphaLiveEngineRef;
    
    TextButton *copyExternalFilesSwitch;
    Label *copyExternalFilesLabel;
    
    ComboBox *midiClockMenu, *clockStartMessageMenu, *midiClockMessageFilterMenu;
    Label *midiClockLabel, *clockStartMessageLabel, *midiClockMessageFilterLabel;
    ComboBox *receiveMidiProgramChangeMessagesMenu;
    Label *receiveMidiProgramChangeMessagesLabel;
    
};

#endif //H_GENERALPROJSETTINGSCOMPONENT