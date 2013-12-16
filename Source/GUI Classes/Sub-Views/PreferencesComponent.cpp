//
//  PreferencesComponent.cpp
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

#include "PreferencesComponent.h"
#include "../Views/MainComponent.h"
#include "../../Application/CommonInfoBoxText.h"
#include "../AlphaLiveLookandFeel.h"


PreferencesComponent::PreferencesComponent(MainComponent &ref, AlphaLiveEngine &ref2)
                                            :   mainComponentRef(ref),
                                                alphaLiveEngineRef(ref2)
{
    //AUDIO OUTPUT SETTINGS
    initAudioSettingsComponent();
    //GENERAL SETTINGS COMPONENT
    generalSettingsComponent = new GeneralSettingsComponent(mainComponentRef, alphaLiveEngineRef);
    //HARDWARE PREFERENCES COMPONENT
    hardwarePreferencesComponent = new HardwarePreferencesComponent (mainComponentRef, alphaLiveEngineRef);
    
    //create tabbed component and add tabs/child components
    addAndMakeVisible(tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    
    tabbedComponent->addTab(translate("General Settings"),
                            AlphaTheme::getInstance()->foregroundColourDarker,
                            generalSettingsComponent,
                            true);
    tabbedComponent->addTab(translate("Hardware Settings"),
                            AlphaTheme::getInstance()->foregroundColourDarker,
                            hardwarePreferencesComponent,
                            true);
    //make sure the audio component is always the last tab, as this moves when the theme is changed
    tabbedComponent->addTab(translate("Audio Output Settings"),
                            AlphaTheme::getInstance()->foregroundColourDarker,
                            audioAndMidiSettingsComponent,
                            true);
    
    
    addAndMakeVisible(closeButton = new TextButton());
    closeButton->setButtonText(translate("Close"));
    closeButton->addListener(this);
    closeButton->addMouseListener(this, true);
    
    //set this component to listen to itself
    addKeyListener(this);
}

PreferencesComponent::~PreferencesComponent()
{
    deleteAllChildren();
    
}

void PreferencesComponent::resized()
{
    tabbedComponent->setBounds(getWidth()/4, getHeight()/10, getWidth()/2, ((getHeight()/10)*8)-70);
    
    closeButton->setBounds((getWidth()/2)-20, ((getHeight()/10)*9)-68, 40, 35);
}

void PreferencesComponent::paint (Graphics& g)
{
    g.setColour(AlphaTheme::getInstance()->backgroundColour.withAlpha(0.8f));
    g.fillRect(0, 0, getWidth(), getHeight());
    
    g.setColour(AlphaTheme::getInstance()->childBackgroundColourLighter.withAlpha(1.0f));
    g.fillRoundedRectangle(getWidth()/4, getHeight()/10, getWidth()/2, ((getHeight()/10)*8)-30, 10);
    
}

void PreferencesComponent::visibilityChanged()
{
    //this is an easy way of updating the values/display of the components/views when needed
    
    if(isVisible())
    {
        generalSettingsComponent->updateDisplay();
    }
}


void PreferencesComponent::buttonClicked (Button *button)
{
    if (button == closeButton)
    {
        setVisible(false);
    }
}

void PreferencesComponent::initAudioSettingsComponent()
{
    #if JUCE_MAC || JUCE_LINUX
    audioAndMidiSettingsComponent = new AudioDeviceSelectorComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, false, false, true, false);
    #endif
    #if JUCE_WINDOWS
    if (alphaLiveEngineRef.getDeviceStatus() != 0)
    {
        //don't display the option to set a MIDI ouput device
        audioAndMidiSettingsComponent = new AudioDeviceSelectorComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, false, false, true, false);
    }
    else
    {
        //allow MIDI output and input devices to be set
        audioAndMidiSettingsComponent = new AudioDeviceSelectorComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, true, true, true, false);
    }
    #endif
    audioAndMidiSettingsComponent->addMouseListener(this, true);
}

void PreferencesComponent::removeMidiOutputSelector()
{
    #if JUCE_WINDOWS
    delete audioAndMidiSettingsComponent;
    tabbedComponent->removeTab(1);
    
    //don't display the option to set a MIDI ouput device
    audioAndMidiSettingsComponent = new AudioDeviceSelectorComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, false, false, true, false);
    tabbedComponent->addTab(translate("Audio Output Settings"), AlphaTheme::getInstance()->foregroundColourDarker, audioAndMidiSettingsComponent, true);
    
    #endif //JUCE_WINDOWS
}

void PreferencesComponent::mouseEnter (const MouseEvent &e)
{
    if (closeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::closeButton));
    }
    else if (audioAndMidiSettingsComponent->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText (translate("Audio Output Settings. Allows you to set the global audio output settings for AlphaLive. Output device, output channels, sample rate, and audio buffer size can be set from here. The 'test' button can be used to play a test tone."));
    }
}

void PreferencesComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}

bool PreferencesComponent::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    if (key == KeyPress::escapeKey)
    {
        setVisible(false);
        return true;
    }
    else
        return false; //incase the keypress is shortcut that MainComponent needs to react to.
    
}

void PreferencesComponent::setTabColour()
{
    //This can't be called in paint as it calls repaint and would cause loops and high CPU.
    for (int i = 0; i < tabbedComponent->getNumTabs(); i++)
        tabbedComponent->setTabBackgroundColour(i, AlphaTheme::getInstance()->foregroundColourDarker);
}

void PreferencesComponent::redrawAudioSettingsComponent()
{
    //this function is called when changing the theme to easily change the colour scheme
    
    tabbedComponent->removeTab(tabbedComponent->getNumTabs() - 1);
    
    initAudioSettingsComponent();

    tabbedComponent->addTab(translate("Audio Output Settings"), AlphaTheme::getInstance()->foregroundColourDarker, audioAndMidiSettingsComponent, true);
}

void PreferencesComponent::selectHardwareTab()
{
    tabbedComponent->setCurrentTabIndex(1);
}














GeneralSettingsComponent::GeneralSettingsComponent(MainComponent &ref, AlphaLiveEngine &ref2)
                                                                :   mainComponentRef(ref),
                                                                    alphaLiveEngineRef(ref2)
{
    //To add a non-standard character to a label, combobox, etc..
    // you must wrap the string like this - CharPointer_UTF8 ("日本の").
    
    addAndMakeVisible(fileGroup = new GroupComponent("file group", translate("File")));
    addAndMakeVisible(displayGroup = new GroupComponent("display group", translate("Display")));
    addAndMakeVisible(audioMidiGroup = new GroupComponent("audio/midi group", translate("Audio/MIDI")));
    
    addAndMakeVisible(deviceInterfaceMenu = new ComboBox());
    deviceInterfaceMenu->addItem(translate("AlphaSphere"), 1);
    deviceInterfaceMenu->addItem(translate("AlphaSphere elite"), 2);
    deviceInterfaceMenu->addListener(this);
    deviceInterfaceMenu->addMouseListener(this, true);
    deviceInterfaceMenu->setSelectedId(StoredSettings::getInstance()->deviceType, true);
    
    addAndMakeVisible(deviceInterfaceLabel = new Label("device label", translate("Device Interface:")));
    
    addAndMakeVisible(appProjectDirChooser = new ComboBox());
    appProjectDirChooser->addItem(StoredSettings::getInstance()->appProjectDir.getFullPathName(), 1);
    appProjectDirChooser->addSeparator();
    appProjectDirChooser->addItem(translate("Set Directory..."), 2);
    appProjectDirChooser->addListener(this);
    appProjectDirChooser->addMouseListener(this, true);
    appProjectDirChooser->setSelectedId(1, true);
    
    addAndMakeVisible(directoryLabel = new Label("directory label", translate("Projects Directory:")));
    
    addAndMakeVisible(midiNoteDisplayTypeLabel = new Label ("midi display label", translate("Note Display Type:")));
    
    addAndMakeVisible(midiNoteDisplayTypeMenu = new ComboBox());
    midiNoteDisplayTypeMenu->addItem(translate("MIDI Note Number"), 1);
    midiNoteDisplayTypeMenu->addItem(translate("MIDI Note Name"), 2);
    midiNoteDisplayTypeMenu->addListener(this);
    midiNoteDisplayTypeMenu->addMouseListener(this, true);
    midiNoteDisplayTypeMenu->setSelectedId(StoredSettings::getInstance()->midiNoteDisplayType, true);
    
    addAndMakeVisible(launchTaskLabel = new Label ("Launch Task Label", translate("On Launch:")));
    
    addAndMakeVisible(launchTaskMenu = new ComboBox());
    launchTaskMenu->addItem(translate("Open new project"), 1);
    launchTaskMenu->addItem(translate("Open last project"), 2);
    launchTaskMenu->addListener(this);
    launchTaskMenu->addMouseListener(this, true);
    launchTaskMenu->setSelectedId(StoredSettings::getInstance()->launchTask, true);
    
    
    addAndMakeVisible(killOnClockStopButton = new TextButton());
    killOnClockStopButton->setClickingTogglesState(true);
    killOnClockStopButton->setToggleState(StoredSettings::getInstance()->killOnClockStop-1, false);
    if(killOnClockStopButton->getToggleStateValue() == true)
        killOnClockStopButton->setButtonText(translate("On"));
    else
        killOnClockStopButton->setButtonText(translate("Off")); 
    
    killOnClockStopButton->addListener(this);
    killOnClockStopButton->addMouseListener(this, true);
    
    addAndMakeVisible(killOnClockStopLabel = new Label());
    killOnClockStopLabel->setText(translate("Stop All Sound On Clock Stop:"), dontSendNotification);
    
    
    addAndMakeVisible(cleanOnCloseButton = new TextButton());
    cleanOnCloseButton->setClickingTogglesState(true);
    cleanOnCloseButton->setToggleState(StoredSettings::getInstance()->cleanOnClose-1, false);
    if(cleanOnCloseButton->getToggleStateValue() == true)
        cleanOnCloseButton->setButtonText(translate("On"));
    else
        cleanOnCloseButton->setButtonText(translate("Off")); 
    
    cleanOnCloseButton->addListener(this);
    cleanOnCloseButton->addMouseListener(this, true);
    
    addAndMakeVisible(cleanOnCloseLabel = new Label());
    cleanOnCloseLabel->setText(translate("Clean Project On Close:"), dontSendNotification);
    
    
    addAndMakeVisible(autoSaveScenesButton = new TextButton());
    autoSaveScenesButton->setClickingTogglesState(true);
    autoSaveScenesButton->setToggleState(StoredSettings::getInstance()->autoSaveScenes-1, false);
    if(autoSaveScenesButton->getToggleStateValue() == true)
        autoSaveScenesButton->setButtonText(translate("On"));
    else
        autoSaveScenesButton->setButtonText(translate("Off")); 
    
    autoSaveScenesButton->addListener(this);
    autoSaveScenesButton->addMouseListener(this, true);
    
    addAndMakeVisible(autoSaveScenesLabel = new Label());
    autoSaveScenesLabel->setText(translate("Auto-save Scenes:"), dontSendNotification);

    addAndMakeVisible(autoCheckUpdatesButton = new TextButton());
    autoCheckUpdatesButton->setClickingTogglesState(true);
    autoCheckUpdatesButton->setToggleState(StoredSettings::getInstance()->autoCheckUpdates-1, false);
    if(autoCheckUpdatesButton->getToggleStateValue() == true)
        autoCheckUpdatesButton->setButtonText(translate("On"));
    else
        autoCheckUpdatesButton->setButtonText(translate("Off")); 
    autoCheckUpdatesButton->addListener(this);
    autoCheckUpdatesButton->addMouseListener(this, true);
    
    addAndMakeVisible(autoCheckUpdatesLabel = new Label());
    autoCheckUpdatesLabel->setText(translate("Autocheck for Updates:"), dontSendNotification);
    
    addAndMakeVisible(interfaceThemeMenu = new ComboBox());
    interfaceThemeMenu->addItem(translate("Classic"), 1);
    interfaceThemeMenu->addListener(this);
    interfaceThemeMenu->addMouseListener(this, true);
    
    addAndMakeVisible(interfaceThemeLabel = new Label());
    interfaceThemeLabel->setText(translate("Interface Theme:"), dontSendNotification);
    
    // ===== Add the Materia theme if the user has the Materia sample pack =====
    
    File currentAppParentDir = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory();
    
    File materiaDir (currentAppParentDir.getFullPathName() +
                     File::separatorString +
                     "Library" +
                     File::separatorString +
                     "Audio Library" +
                     File::separatorString +
                     "Materia");
    
    if (materiaDir.exists())
        interfaceThemeMenu->addItem("Materia", 101);
    
    interfaceThemeMenu->setSelectedId(StoredSettings::getInstance()->interfaceTheme, true);
    
    addAndMakeVisible(padContentDisplayLabel = new Label());
    padContentDisplayLabel->setText(translate("Pad Content Display:"), dontSendNotification);
    
    addAndMakeVisible(padContentDisplayMenu = new ComboBox());
    padContentDisplayMenu->addItem(translate("Pad numbers only"), 1);
    padContentDisplayMenu->addItem(translate("All pad contents"), 2);
    padContentDisplayMenu->addListener(this);
    padContentDisplayMenu->addMouseListener(this, true);
    padContentDisplayMenu->setSelectedId(StoredSettings::getInstance()->padContentDisplay);
    /*
     Eventually it would be good if the user could completely customise what contents
     they can see on a pad. This could be done by instead of using a combobox, the user
     is just given a button that pops up a calloutbox that includes a bunch of tick boxes
     for each available setting (e.g. MIDI Mode - note, channel; Sampler Mode - sample, 
     effect etc...). 
     How should we handle what happens when there's too much info to fit on a pad?
     Should we warn the user here if they've selected 'too many' settings, or just
     make the settings really small or use ellipsis?
     */
    
    addAndMakeVisible(midiChannelPressureModeLabel = new Label());
    midiChannelPressureModeLabel->setText(translate("MIDI Channel Pressure Mode:"), dontSendNotification);
    
    addAndMakeVisible(midiChannelPressureModeMenu = new ComboBox());
    midiChannelPressureModeMenu->addItem(translate("All pads have control"), 1);
    midiChannelPressureModeMenu->addItem(translate("Newest pad has control only"), 2);
    midiChannelPressureModeMenu->addListener(this);
    midiChannelPressureModeMenu->addMouseListener(this, true);
    
}

GeneralSettingsComponent::~GeneralSettingsComponent()
{
    
    deleteAllChildren();
    
}

void GeneralSettingsComponent::resized()
{
    
    fileGroup->setBounds(20, 10, getWidth() - 40, 180);
    
    appProjectDirChooser->setBounds(200, 30, 210, 20);
    directoryLabel->setBounds(60, 30, 120, 20);
    
    launchTaskMenu->setBounds(200, 60, 210, 20);
    launchTaskLabel->setBounds(60, 60, 120, 20);
    
    cleanOnCloseButton->setBounds(280, 88, 40, 25);
    cleanOnCloseLabel->setBounds(60, 90, 200, 20);
    
    autoSaveScenesButton->setBounds(280, 118, 40, 25);
    autoSaveScenesLabel->setBounds(60, 120, 200, 20);
    
    autoCheckUpdatesButton->setBounds(280, 148, 40, 25);
    autoCheckUpdatesLabel->setBounds(60, 150, 200, 20);
    
    
    displayGroup->setBounds(20, 200, getWidth() - 40, 150);
    
    interfaceThemeMenu->setBounds(200, 220, 210, 20);
    interfaceThemeLabel->setBounds(60, 220, 120, 20);

    deviceInterfaceMenu->setBounds(200, 250, 210, 20);
    deviceInterfaceLabel->setBounds(60, 250, 120, 20);
    
    midiNoteDisplayTypeMenu->setBounds(200, 280, 210, 20);
    midiNoteDisplayTypeLabel->setBounds(60, 280, 120, 20);
    
    padContentDisplayMenu->setBounds(240, 310, 170, 20);
    padContentDisplayLabel->setBounds(60, 310, 160, 20);
    
    
    audioMidiGroup->setBounds(20, 360, getWidth() - 40, 90);
    
    midiChannelPressureModeMenu->setBounds(270, 380, 140, 20);
    midiChannelPressureModeLabel->setBounds(60, 380, 190, 20);
    
    killOnClockStopButton->setBounds(280, 410, 40, 25);
    killOnClockStopLabel->setBounds(60, 410, 200, 20);
    
    
    }

void GeneralSettingsComponent::paint (Graphics& g)
{
    deviceInterfaceLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    directoryLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    midiNoteDisplayTypeLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    launchTaskLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    killOnClockStopLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    cleanOnCloseLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    autoSaveScenesLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    autoCheckUpdatesLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    interfaceThemeLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    padContentDisplayMenu->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    padContentDisplayLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    midiChannelPressureModeMenu->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    midiChannelPressureModeLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    
    fileGroup->setColour(GroupComponent::textColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
    fileGroup->setColour(GroupComponent::outlineColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
    displayGroup->setColour(GroupComponent::textColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
    displayGroup->setColour(GroupComponent::outlineColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
    audioMidiGroup->setColour(GroupComponent::textColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
    audioMidiGroup->setColour(GroupComponent::outlineColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
}

void GeneralSettingsComponent::buttonClicked (Button* button)
{
    if (button == killOnClockStopButton)
    {
        if(killOnClockStopButton->getToggleStateValue() == true)
        {
            StoredSettings::getInstance()->killOnClockStop = 2;
            killOnClockStopButton->setButtonText(translate("On"));
        }
        
        else
        {
            StoredSettings::getInstance()->killOnClockStop = 1;
            killOnClockStopButton->setButtonText(translate("Off"));
        }
        
        StoredSettings::getInstance()->flush();
    }
    
    
    else if (button == cleanOnCloseButton)
    {
        if(button->getToggleStateValue() == true)
        {
            StoredSettings::getInstance()->cleanOnClose = 2;
            button->setButtonText(translate("On"));
        }
        
        else
        {
            StoredSettings::getInstance()->cleanOnClose = 1;
            button->setButtonText(translate("Off"));
        }
        
        StoredSettings::getInstance()->flush();
    }
    
    else if (button == autoSaveScenesButton)
    {
        if(button->getToggleStateValue() == true)
        {
            StoredSettings::getInstance()->autoSaveScenes = 2;
            button->setButtonText(translate("On"));
        }
        
        else
        {
            StoredSettings::getInstance()->autoSaveScenes = 1;
            button->setButtonText(translate("Off"));
        }
        
        StoredSettings::getInstance()->flush();
    }
    
    else if (button == autoCheckUpdatesButton)
    {
        if(button->getToggleStateValue() == true)
        {
            StoredSettings::getInstance()->autoCheckUpdates = 2;
            button->setButtonText(translate("On"));
        }
        
        else
        {
            StoredSettings::getInstance()->autoCheckUpdates = 1;
            button->setButtonText(translate("Off"));
        }
        
        StoredSettings::getInstance()->flush();
    }
    
}

void GeneralSettingsComponent::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == midiNoteDisplayTypeMenu)
    {
        StoredSettings::getInstance()->midiNoteDisplayType = comboBox->getSelectedId();
        StoredSettings::getInstance()->flush();
        
        if (StoredSettings::getInstance()->getInstance()->padContentDisplay == 2)
        {
            for (int i = 0; i < 48; i++)
                mainComponentRef.getGuiPadLayout()->setPadDisplay(i);
        }
    }
    
    else if (comboBox == launchTaskMenu)
    {
        StoredSettings::getInstance()->launchTask = comboBox->getSelectedId();
        StoredSettings::getInstance()->flush();
    }
    
    else if (comboBox == deviceInterfaceMenu)
    {
        StoredSettings::getInstance()->deviceType = comboBox->getSelectedId();
        StoredSettings::getInstance()->flush();
        
        mainComponentRef.editInterfaceFromDeviceConnectivity(2);
    }
    
    else if (comboBox == appProjectDirChooser)
    {
        if (comboBox->getSelectedId() == 2) //set directory
        {
            FileChooser myChooser (translate("Please select a directory to move the AlphaLive Projects directory to..."),
                                   StoredSettings::getInstance()->appProjectDir.getParentDirectory());
            
            if (myChooser.browseForDirectory() == true)
            {
                File selectedDir (myChooser.getResult());
                
                if (StoredSettings::getInstance()->appProjectDir.getParentDirectory() != selectedDir &&
                    StoredSettings::getInstance()->appProjectDir != selectedDir)
                {
                    //create new directory based on the users selection
                    File newProjectDirectory = (selectedDir.getFullPathName() + File::separatorString + "AlphaLive Projects");
                    //move old directory to the new one
                    StoredSettings::getInstance()->appProjectDir.moveFileTo(newProjectDirectory);
                    //Add to stored settings
                    StoredSettings::getInstance()->appProjectDir = newProjectDirectory;
                    StoredSettings::getInstance()->flush();
                    //set menu item 1 to  new directory
                    appProjectDirChooser->changeItemText(1, StoredSettings::getInstance()->appProjectDir.getFullPathName());
                }
            }
            
            comboBox->setSelectedId(1, true);
        }
    }
    
    else if (comboBox == interfaceThemeMenu)
    {
        StoredSettings::getInstance()->interfaceTheme = comboBox->getSelectedId();
        StoredSettings::getInstance()->flush();
        
        mainComponentRef.changeLookAndFeel();
    }
    
    if (comboBox == padContentDisplayMenu)
    {
        StoredSettings::getInstance()->padContentDisplay = comboBox->getSelectedId();
        StoredSettings::getInstance()->flush();
        
        for (int i = 0; i < 48; i++)
            mainComponentRef.getGuiPadLayout()->setPadDisplay(i);
    }
    
}


void GeneralSettingsComponent::mouseEnter (const MouseEvent &e)
{
    if (deviceInterfaceMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("AlphaSphere Device Interface Selector. This allows you to manually change the AlphaLive user interface to comply with a particular AlphaSphere model. When an AlphaSphere is connected to AlphaLive it will automatically display the correct interface."));
    }
    else if (appProjectDirChooser->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("AlphaLive Projects Directory selector. This allows you to change the location of the AlphaLive's projects folder. The current folder and its contents will be moved to the new location."));
    }
    else if (midiNoteDisplayTypeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("MIDI note display type selector. This menu is used to configure how MIDI notes are displayed within the application."));
    }
    else if (launchTaskMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Application Launch Task selector. This menu is used to configure what happens when the application is launched."));
    }
    else if (killOnClockStopButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("If this option is set to 'on' any playing pads will be stopped when the clock is stopped."));
    }
    else if (cleanOnCloseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("'Cleaning' a project is the process of searching through the projects Audio Files directory and removing any unused audio files. When set to 'on' the project will automatically be cleaned when the application is shut down. This will prevent a build-up of unused data."));
    }
    else if (autoSaveScenesButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("By default ('on') when you switch between scenes it will automatically save the recently changed settings to the scene slot locally (not to file). If this option is set to 'off' you will need to shift-click the current scene slot before switching scenes otherwise any changes to the scenes settings will be lost."));
    }
    else if (autoCheckUpdatesButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("When this option is set to 'on' the application will automatically check online for software updates when the application is launched. You can manually check for updates using the option in the 'Help' item on the menu bar."));
    }
    else if (interfaceThemeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("AlphaLive interface theme selector. This allows you to change the overall interface skin and colour scheme of the application."));
    }
    else if (padContentDisplayMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Pad Contents Display select. This allows you to change the text of the pads on the pad layout display. If you selected 'All pad contents' the values of the main settings are shown using the following common abbreviations: Ch - channel, N - note, P - pressure, M - Mode. See the Reference Manual for the full list of abbreviations."));
    }
}

void GeneralSettingsComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}

void GeneralSettingsComponent::updateDisplay()
{
    //this function is called from PreferencesComponent::visibilityChanged
    
    midiNoteDisplayTypeMenu->setSelectedId(StoredSettings::getInstance()->midiNoteDisplayType, true);
    deviceInterfaceMenu->setSelectedId(StoredSettings::getInstance()->deviceType, true);
    
}










HardwarePreferencesComponent::HardwarePreferencesComponent(MainComponent &ref, AlphaLiveEngine &ref2)
                                                            :   mainComponentRef(ref),
                                                                alphaLiveEngineRef(ref2)
{
    addAndMakeVisible(ledColourSchemeLabel = new Label());
    ledColourSchemeLabel->setText(translate("LED Colour Scheme:"), dontSendNotification);
    
    addAndMakeVisible(ledColourSchemeMenu = new ComboBox());
    ledColourSchemeMenu->addListener(this);
    ledColourSchemeMenu->addMouseListener(this, true);
    
    createLedColourSchemes();
    
    //Set the order of the colour schemes to be displayed as follows:
    // 1. 'Default'
    // 2. All other schemes in alphabetically order
    // 3. 'Custom'
    
    StringArray names;
    for (int i = 0; i < ledColourScheme.size(); i++)
        names.add(ledColourScheme[i]->name);
    names.sort(true);
    names.move(names.indexOf("Default"), 0);
    names.move(names.indexOf("Custom"), names.size() - 1);
    
    //add all schemes to the combobox
    for (int i = 0; i < names.size(); i++)
    {
        //to add new colour schemes to the application, see 'createLedColourScheme()'
        ledColourSchemeMenu->addItem (names[i], i + 1);
    }
    
    //select the matching item from what is stored in prefs
    //We do not use the stored scheme to set the colours here.
    //This is just displayed for continuity.
    //The actual LED colours and button colours are set below.
    for (int i = 0; i <= names.size(); i++)
    {
        if (names[i] == StoredSettings::getInstance()->hardwareLedColourScheme)
        {
            ledColourSchemeMenu->setSelectedId(i + 1);
            break;
        }
    }
    
    addAndMakeVisible(ledColourLabel[0] = new Label());
    ledColourLabel[0]->setText(translate("Minimum Pressure LED Colour:"), dontSendNotification);
    addAndMakeVisible(ledColourLabel[1] = new Label());
    ledColourLabel[1]->setText(translate("Mid Pressure LED Colour:"), dontSendNotification);
    addAndMakeVisible(ledColourLabel[2] = new Label());
    ledColourLabel[2]->setText(translate("Maximum Pressure LED Colour:"), dontSendNotification);
    
    Path rect;
    rect.addRectangle(0, 0, 10, 10);
    
    for (int i = 0; i < 3; i++)
    {
        Colour colour = StoredSettings::getInstance()->hardwareLedColour[i];
        
        addAndMakeVisible(ledColourButton[i] = new ShapeButton("LED colour button " + String(i),
                                                               colour,
                                                               colour,
                                                               colour));
        ledColourButton[i]->setShape(rect, true, false, false);
        ledColourButton[i]->addListener(this);
        ledColourButton[i]->addMouseListener(this, true);
    }
}

HardwarePreferencesComponent::~HardwarePreferencesComponent()
{
    ledColourScheme.clear();
    deleteAllChildren();
}

void HardwarePreferencesComponent::resized()
{
    ledColourSchemeLabel->setBounds(90, 20, 150, 20);
    ledColourSchemeMenu->setBounds(250, 20, 140, 20);
    
    ledColourLabel[0]->setBounds(60+30, 60+20, 200, 20);
    ledColourButton[0]->setBounds(280+30, 60+18, 80, 25);
    
    ledColourLabel[1]->setBounds(60+30, 60+60, 200, 20);
    ledColourButton[1]->setBounds(280+30, 60+58, 80, 25);

    ledColourLabel[2]->setBounds(60+30, 60+100, 200, 20);
    ledColourButton[2]->setBounds(280+30, 60+98, 80, 25);


}

void HardwarePreferencesComponent::paint (Graphics& g)
{
    for (int i = 0; i < 3; i++)
    {
        Colour colour = StoredSettings::getInstance()->hardwareLedColour[i];
        ledColourButton[i]->setColours(colour, colour, colour);
    }
    
    ledColourSchemeLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    ledColourLabel[0]->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    ledColourLabel[1]->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    ledColourLabel[2]->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
}

void HardwarePreferencesComponent::buttonClicked (Button* button)
{
    for (int i = 0; i < 3; i++)
    {
        if (button == ledColourButton[i])
        {
            ColourSelector* colourSelector = new ColourSelector(14);
            colourSelector->setName("cs" + String (i));
            colourSelector->setCurrentColour (StoredSettings::getInstance()->hardwareLedColour[i]);
            colourSelector->addChangeListener(this);
            colourSelector->setSize (300, 400);

            CallOutBox::launchAsynchronously (colourSelector, button->getScreenBounds(), nullptr);
        }
    }
 
}

void HardwarePreferencesComponent::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == ledColourSchemeMenu)
    {
        //if it does not equal custom...
        if (comboBox->getSelectedId() != comboBox->getNumItems())
        {
            int schemeArrayIndex;
            
            //find the ledColourScheme object with the matching name
            for (int i = 0; i < ledColourScheme.size(); i++)
            {
                if (ledColourScheme[i]->name == comboBox->getText())
                {
                    schemeArrayIndex = i;
                    break;
                }
            }
            
            //apply the colours to the LED and the buttons
            for (int i = 0; i < 3; i++)
            {
                Colour colour = ledColourScheme[schemeArrayIndex]->colour[i];
                
                alphaLiveEngineRef.setLedColour(i, colour);
                StoredSettings::getInstance()->hardwareLedColour[i] = colour;
                ledColourButton[i]->repaint();
            }
            
            StoredSettings::getInstance()->hardwareLedColourScheme = ledColourScheme[schemeArrayIndex]->name;
            StoredSettings::getInstance()->flush();
        }
    }
}

void HardwarePreferencesComponent::changeListenerCallback (ChangeBroadcaster *source)
{
    ColourSelector* cs = dynamic_cast <ColourSelector*> (source);
    Colour colour = cs->getCurrentColour();
    
    for (int i = 0; i < 3; i++)
    {
        if (cs->getName() == "cs" + String (i))
        {
            alphaLiveEngineRef.setLedColour(i, colour);
            
            StoredSettings::getInstance()->hardwareLedColour[i] = colour;
            
            ledColourButton[i]->repaint();
        }
    }
    
    //Set the colour scheme menu to "Custom"
    if (ledColourSchemeMenu->getSelectedId() != ledColourSchemeMenu->getNumItems())
        ledColourSchemeMenu->setSelectedId(ledColourSchemeMenu->getNumItems(), true);
}


void HardwarePreferencesComponent::mouseEnter (const MouseEvent &e)
{
    if (ledColourSchemeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("LED Colour Scheme Selector. This menu provides you with a set of colour schemes that can be applied to the AlphaSphere's LED. The LED will transition between three colours as more pressure is applied to the pads. You can also create a custom colour scheme using the three buttons below."));
    }
    
    else if (ledColourButton[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Minimum Pressure LED Colour Selector. Click this button to display a colour picker to choose a custom colour for the LED when no pads are being pressed."));
    }
    else if (ledColourButton[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Mid Pressure LED Colour Selector. Click this button to display a colour picker to choose a custom colour for the LED when a single pad is presssed to it's full depth. To see the colour change in realtime, hold a pad at it's full depth while chosing the colour."));
    }
    else if (ledColourButton[2]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Maximum Pressure LED Colour Selector. Click this button to display a colour picker to choose a custom colour for the LED when a two or more pads are pressed to their full depth. To see the colour change in realtime, hold two pads at their full depths while chosing the colour."));
    }
}

void HardwarePreferencesComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}

void HardwarePreferencesComponent::updateDisplay()
{
    //this function is called from PreferencesComponent::visibilityChanged
}

void HardwarePreferencesComponent::createLedColourSchemes()
{
    int currentIndex;
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Default";
    ledColourScheme[currentIndex]->colour[0] = Colour(0, 0, 255);
    ledColourScheme[currentIndex]->colour[1] = Colour(0, 255, 0);
    ledColourScheme[currentIndex]->colour[2] = Colour(255, 0, 0);
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Custom";
    
    //=================================================
    //create schemes here....
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Fire";
    ledColourScheme[currentIndex]->colour[0] = Colour(0xffffff00);
    ledColourScheme[currentIndex]->colour[1] = Colour(0xffff7500);
    ledColourScheme[currentIndex]->colour[2] = Colour(0xffff0000);
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Ice";
    ledColourScheme[currentIndex]->colour[0] = Colour(0xff0000ff);
    ledColourScheme[currentIndex]->colour[1] = Colour(0xff00d8ff);
    ledColourScheme[currentIndex]->colour[2] = Colour(0xff7effff);
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Christmas";
    ledColourScheme[currentIndex]->colour[0] = Colour(0xffffffff);
    ledColourScheme[currentIndex]->colour[1] = Colour(0xff00ff00);
    ledColourScheme[currentIndex]->colour[2] = Colour(0xffff0000);
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Monochrome";
    ledColourScheme[currentIndex]->colour[0] = Colour(0xff000000);
    ledColourScheme[currentIndex]->colour[1] = Colour(0xff222222);
    ledColourScheme[currentIndex]->colour[2] = Colour(0xffffffff);
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Water";
    ledColourScheme[currentIndex]->colour[0] = Colour(0xff034e25);
    ledColourScheme[currentIndex]->colour[1] = Colour(0xff0000ff);
    ledColourScheme[currentIndex]->colour[2] = Colour(0xff00e3ff);
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Midnight";
    ledColourScheme[currentIndex]->colour[0] = Colour(0xff240046);
    ledColourScheme[currentIndex]->colour[1] = Colour(0xffa900ff);
    ledColourScheme[currentIndex]->colour[2] = Colour(0xff3a00ff);
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Dub";
    ledColourScheme[currentIndex]->colour[0] = Colour(0xff00ff00);
    ledColourScheme[currentIndex]->colour[1] = Colour(0xffffff00);
    ledColourScheme[currentIndex]->colour[2] = Colour(0xffff0000);
    
    ledColourScheme.add(new LedColourScheme());
    currentIndex = ledColourScheme.size() - 1;
    ledColourScheme[currentIndex]->name = "Pink Power";
    ledColourScheme[currentIndex]->colour[0] = Colour(0xffe100ff);
    ledColourScheme[currentIndex]->colour[1] = Colour(0xff8400ff);
    ledColourScheme[currentIndex]->colour[2] = Colour(0xff4a00ff);
    
    
    //=================================================
}

