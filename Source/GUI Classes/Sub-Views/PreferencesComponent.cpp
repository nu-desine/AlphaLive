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
    #if JUCE_MAC || JUCE_LINUX
    audioAndMidiSettingsComponent = new AlphaAudioSettingsComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, false, false, true, false, alphaLiveEngineRef);
    #endif
    #if JUCE_WINDOWS
    if (alphaLiveEngineRef.getDeviceStatus() != 0)
    {
        //don't display the option to set a MIDI ouput device
        audioAndMidiSettingsComponent = new AlphaAudioSettingsComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, false, false, true, false, alphaLiveEngineRef);
    }
    else
    {
        //allow a MIDI output device to be set
        audioAndMidiSettingsComponent = new AlphaAudioSettingsComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, false, true, true, false, alphaLiveEngineRef);
    }
    #endif
    audioAndMidiSettingsComponent->addMouseListener(this, true);
    
    //GENERAL SETTINGS COMPONENT
    generalSettingsComponent = new GeneralSettingsComponent(mainComponentRef, alphaLiveEngineRef);
    
    
    //create tabbed component and add tabs/child components
    addAndMakeVisible(tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    tabbedComponent->addTab(translate("Audio Output Settings"), Colours::darkgrey, audioAndMidiSettingsComponent, true);
    tabbedComponent->addTab(translate("General Settings"), Colours::darkgrey, generalSettingsComponent, true);
    
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
    tabbedComponent->setBounds(getWidth()/4, getHeight()/6, getWidth()/2, ((getHeight()/6)*4)-70);
    
    closeButton->setBounds((getWidth()/2)-20, ((getHeight()/6)*5)-68, 40, 35);
}

void PreferencesComponent::paint (Graphics& g)
{
    g.setColour(Colours::black.withAlpha(0.8f));
    g.fillRect(0, 0, getWidth(), getHeight());
    
    g.setColour(AlphaColours::verydarkgrey.withAlpha(1.0f));
    g.fillRoundedRectangle(getWidth()/4, getHeight()/6, getWidth()/2, ((getHeight()/6)*4)-30, 10);
    
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

void PreferencesComponent::removeMidiOutputSelector()
{
    #if JUCE_WINDOWS
    delete audioAndMidiSettingsComponent;
    tabbedComponent->removeTab(0);
    
    //don't display the option to set a MIDI ouput device
    audioAndMidiSettingsComponent = new AlphaAudioSettingsComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, false, false, true, false, alphaLiveEngineRef);
    tabbedComponent->addTab(translate("Audio Output Settings"), Colours::darkgrey, audioAndMidiSettingsComponent, true);
    
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














GeneralSettingsComponent::GeneralSettingsComponent(MainComponent &ref, AlphaLiveEngine &ref2)
                                                                :   mainComponentRef(ref),
                                                                    alphaLiveEngineRef(ref2)
{
    //To add a non-standard character to a label, combobox, etc..
    // you must wrap the string like this - CharPointer_UTF8 ("日本の").
    
    addAndMakeVisible(deviceInterfaceMenu = new ComboBox());
    deviceInterfaceMenu->addItem(translate("AlphaSphere"), 1);
    deviceInterfaceMenu->addItem(translate("AlphaSphere elite"), 2);
    deviceInterfaceMenu->addListener(this);
    deviceInterfaceMenu->addMouseListener(this, true);
    deviceInterfaceMenu->setSelectedId(StoredSettings::getInstance()->deviceType, true);
    
    addAndMakeVisible(deviceInterfaceLabel = new Label("device label", translate("Device Interface:")));
    deviceInterfaceLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    addAndMakeVisible(appProjectDirChooser = new ComboBox());
    appProjectDirChooser->addItem(StoredSettings::getInstance()->appProjectDir.getFullPathName(), 1);
    appProjectDirChooser->addSeparator();
    appProjectDirChooser->addItem(translate("Set Directory..."), 2);
    appProjectDirChooser->addListener(this);
    appProjectDirChooser->addMouseListener(this, true);
    appProjectDirChooser->setSelectedId(1, true);
    
    addAndMakeVisible(directoryLabel = new Label("directory label", translate("Projects Directory:")));
    directoryLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    addAndMakeVisible(midiNoteDisplayTypeLabel = new Label ("midi display label", translate("Note Display Type:")));
    midiNoteDisplayTypeLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    addAndMakeVisible(midiNoteDisplayTypeMenu = new ComboBox());
    midiNoteDisplayTypeMenu->addItem(translate("MIDI Note Number"), 1);
    midiNoteDisplayTypeMenu->addItem(translate("MIDI Note Name"), 2);
    midiNoteDisplayTypeMenu->addListener(this);
    midiNoteDisplayTypeMenu->addMouseListener(this, true);
    midiNoteDisplayTypeMenu->setSelectedId(StoredSettings::getInstance()->midiNoteDisplayType, true);
    
    addAndMakeVisible(launchTaskLabel = new Label ("Launch Task Label", translate("On Launch:")));
    launchTaskLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    addAndMakeVisible(launchTaskMenu = new ComboBox());
    launchTaskMenu->addItem(translate("Open new project"), 1);
    launchTaskMenu->addItem(translate("Open last project"), 2);
    launchTaskMenu->addListener(this);
    launchTaskMenu->addMouseListener(this, true);
    launchTaskMenu->setSelectedId(StoredSettings::getInstance()->launchTask, true);
    
    
    addAndMakeVisible(killOnClockStopButton = new TextButton());
    killOnClockStopButton->setClickingTogglesState(true);
    killOnClockStopButton->setToggleState(StoredSettings::getInstance()->killOnClockStop, false);
    if(killOnClockStopButton->getToggleStateValue() == true)
        killOnClockStopButton->setButtonText(translate("On"));
    else
        killOnClockStopButton->setButtonText(translate("Off")); 
    
    killOnClockStopButton->addListener(this);
    killOnClockStopButton->addMouseListener(this, true);
    
    addAndMakeVisible(killOnClockStopLabel = new Label());
    killOnClockStopLabel->setText(translate("Stop All Sound On Clock Stop:"), false);
    killOnClockStopLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    
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
    cleanOnCloseLabel->setText(translate("Clean Project On Close:"), false);
    cleanOnCloseLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    
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
    autoSaveScenesLabel->setText(translate("Auto-save Scenes:"), false);
    autoSaveScenesLabel->setColour(Label::textColourId, Colours::lightgrey);
    

    
}

GeneralSettingsComponent::~GeneralSettingsComponent()
{
    
    deleteAllChildren();
    
}

void GeneralSettingsComponent::resized()
{
    deviceInterfaceMenu->setBounds(200, 10, 210, 20);
    deviceInterfaceLabel->setBounds(60, 10, 120, 20);
    
    appProjectDirChooser->setBounds(200, 50, 210, 20);
    directoryLabel->setBounds(60, 50, 120, 20);
    
    midiNoteDisplayTypeMenu->setBounds(200, 90, 210, 20);
    midiNoteDisplayTypeLabel->setBounds(60, 90, 120, 20);
    
    launchTaskMenu->setBounds(200, 130, 210, 20);
    launchTaskLabel->setBounds(60, 130, 120, 20);
    
    killOnClockStopButton->setBounds(230, 168, 40, 25);
    killOnClockStopLabel->setBounds(60, 170, 150, 20);
    
    cleanOnCloseButton->setBounds(230, 208, 40, 25);
    cleanOnCloseLabel->setBounds(60, 210, 150, 20);
    
    autoSaveScenesButton->setBounds(230, 248, 40, 25);
    autoSaveScenesLabel->setBounds(60, 250, 150, 20);
}

void GeneralSettingsComponent::paint (Graphics& g)
{
    
}

void GeneralSettingsComponent::buttonClicked (Button* button)
{
    if (button == killOnClockStopButton)
    {
        if(killOnClockStopButton->getToggleStateValue() == true)
        {
            StoredSettings::getInstance()->killOnClockStop = 1;
            killOnClockStopButton->setButtonText(translate("On"));
        }
        
        else
        {
            StoredSettings::getInstance()->killOnClockStop = 0;
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
    
}

void GeneralSettingsComponent::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == midiNoteDisplayTypeMenu)
    {
        StoredSettings::getInstance()->midiNoteDisplayType = comboBox->getSelectedId();
        StoredSettings::getInstance()->flush();
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
                    //set menu item 1 to new directory
                    appProjectDirChooser->changeItemText(1, StoredSettings::getInstance()->appProjectDir.getFullPathName());
                }
            }
            
            comboBox->setSelectedId(1, true);
        }
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

