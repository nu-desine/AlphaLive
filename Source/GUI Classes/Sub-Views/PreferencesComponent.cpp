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


PreferencesComponent::PreferencesComponent(MainComponent &ref, AlphaLiveEngine &ref2)
                                            :   mainComponentRef(ref),
                                                alphaLiveEngineRef(ref2)
{
    
    //AUDIO OUTPUT SETTINGS
    #if JUCE_MAC || JUCE_LINUX
    audioAndMidiSettingsComponent = new AlphaAudioSettingsComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, false, false, true, false, alphaLiveEngineRef);
    #endif
    #if JUCE_WINDOWS
    audioAndMidiSettingsComponent = new AlphaAudioSettingsComponent(alphaLiveEngineRef.getAudioDeviceManager(), 0, 0, 0, 2, false, true, true, false, alphaLiveEngineRef);
    #endif
    audioAndMidiSettingsComponent->addMouseListener(this, true);
    
    //GENERAL SETTINGS COMPONENT
    generalSettingsComponent = new GeneralSettingsComponent(mainComponentRef, alphaLiveEngineRef);
    
    
    //create tabbed component and add tabs/child components
    addAndMakeVisible(tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    tabbedComponent->addTab("Audio and MIDI Settings", Colours::darkgrey, audioAndMidiSettingsComponent, true);
    tabbedComponent->addTab("General Settings", Colours::darkgrey, generalSettingsComponent, true);
    
    addAndMakeVisible(closeButton = new TextButton());
    closeButton->setButtonText("Close");
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
    
    closeButton->setBounds((getWidth()/2)-35, ((getHeight()/6)*5)-60, 70, 20);
}

void PreferencesComponent::paint (Graphics& g)
{
    g.setColour(Colours::black.withAlpha(0.8f));
    g.fillRect(0, 0, getWidth(), getHeight());
    
    g.setColour(Colours::grey.withAlpha(0.9f));
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

void PreferencesComponent::mouseEnter (const MouseEvent &e)
{
    if (closeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Click to go back to the application's main interface. Pressing ESC also triggers this command.");
    }
    else if (audioAndMidiSettingsComponent->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText ("Audio Output Settings. Sets and displays the global audio output settings of AlphaLive. Output device, sample rate, and audio buffer size can be set along with a button that allows a test signal to be played.");
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

    
    appProjectDirChooser = new FilenameComponent ("app project directory",
                                                  StoredSettings::getInstance()->appProjectDir.getParentDirectory(),
                                                  false, true, false,
                                                  String::empty,
                                                  String::empty,
                                                  "Choose the AlphaLive Project Directory");
	appProjectDirChooser->addListener (this);					
	///appProjectDirChooser->setBrowseButtonText ("Browse...");
	appProjectDirChooser->setMaxNumberOfRecentFiles(0);
	addAndMakeVisible (appProjectDirChooser);
    appProjectDirChooser->addMouseListener(this, true);
    
    addAndMakeVisible(directoryLabel = new Label("directory label", "Projects Directory:"));
    directoryLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    addAndMakeVisible(midiNoteDisplayTypeLabel = new Label ("midi display label", "Note Display Type:"));
    midiNoteDisplayTypeLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    addAndMakeVisible(midiNoteDisplayTypeMenu = new ComboBox());
    midiNoteDisplayTypeMenu->addItem("MIDI Note Number", 1);
    midiNoteDisplayTypeMenu->addItem("MIDI Note Name", 2);
    midiNoteDisplayTypeMenu->addListener(this);
    midiNoteDisplayTypeMenu->addMouseListener(this, true);
    midiNoteDisplayTypeMenu->setSelectedId(StoredSettings::getInstance()->midiNoteDisplayType, true);
    
    addAndMakeVisible(launchTaskLabel = new Label ("Launch Task Label", "On Launch:"));
    launchTaskLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    addAndMakeVisible(launchTaskMenu = new ComboBox());
    launchTaskMenu->addItem("Open new project", 1);
    launchTaskMenu->addItem("Open last project", 2);
    launchTaskMenu->addListener(this);
    launchTaskMenu->addMouseListener(this, true);
    launchTaskMenu->setSelectedId(StoredSettings::getInstance()->launchTask, true);
    
    
    addAndMakeVisible(killOnClockStopButton = new TextButton());
    killOnClockStopButton->setClickingTogglesState(true);
    killOnClockStopButton->setToggleState(StoredSettings::getInstance()->killOnClockStop, false);
    if(killOnClockStopButton->getToggleStateValue() == true)
        killOnClockStopButton->setButtonText("On");
    else
        killOnClockStopButton->setButtonText("Off"); 
    
    killOnClockStopButton->addListener(this);
    killOnClockStopButton->addMouseListener(this, true);
    
    addAndMakeVisible(killOnClockStopLabel = new Label());
    killOnClockStopLabel->setText("Kill Pads On Clock Stop:", false);
    killOnClockStopLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    
}

GeneralSettingsComponent::~GeneralSettingsComponent()
{
    
    deleteAllChildren();
    
}

void GeneralSettingsComponent::resized()
{
    
    appProjectDirChooser->setBounds(200, 50, 210, 20);
    directoryLabel->setBounds(60, 50, 120, 20);
    
    midiNoteDisplayTypeMenu->setBounds(200, 90, 210, 20);
    midiNoteDisplayTypeLabel->setBounds(60, 90, 120, 20);
    
    launchTaskMenu->setBounds(200, 130, 210, 20);
    launchTaskLabel->setBounds(60, 130, 120, 20);
    
    killOnClockStopButton->setBounds(230, 170, 180, 20);
    killOnClockStopLabel->setBounds(60, 170, 150, 20);
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
            killOnClockStopButton->setButtonText("On");
        }
        
        else
        {
            StoredSettings::getInstance()->killOnClockStop = 0;
            killOnClockStopButton->setButtonText("Off");
        }
    }
}

void GeneralSettingsComponent::comboBoxChanged (ComboBox *comboBox)
{
    
}

void GeneralSettingsComponent::filenameComponentChanged (FilenameComponent* filenameComponent)
{
    if (filenameComponent == appProjectDirChooser)
    {
        if (StoredSettings::getInstance()->appProjectDir.getParentDirectory() != appProjectDirChooser->getCurrentFile())
        {
            //create new directory based on the users selection
            File newProjectDirectory = (appProjectDirChooser->getCurrentFile().getFullPathName() + "/AlphaLive Projects");
            //move old directory to the new one
            StoredSettings::getInstance()->appProjectDir.moveFileTo(newProjectDirectory);
            
            StoredSettings::getInstance()->appProjectDir = newProjectDirectory;
            StoredSettings::getInstance()->flush();
        }
    }
}

void GeneralSettingsComponent::mouseEnter (const MouseEvent &e)
{
    
    if (appProjectDirChooser->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("AlphaLive Projects Directory Chooser. This control allows you to change the location of the applications projects folder. The previous folder and its contents will be moved to the new location.");
    }
    else if (midiNoteDisplayTypeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("MIDI note display type selector. Use this menu to configure how MIDI notes are displayed on the circular pianos.");
    }
    else if (launchTaskMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Application Launch Task selector. Use this menu to configure what happens when the application is lauched.");
    }
    else if (killOnClockStopButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("If this option is set to 'On' any playing pads will be stopped when the clock is stopped.");
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
    
}

