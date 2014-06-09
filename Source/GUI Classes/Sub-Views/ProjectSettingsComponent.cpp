//
//  ProjectSettingsComponent.cpp
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

#include "ProjectSettingsComponent.h"
#include "../Views/MainComponent.h"
#include "../../File and Settings/AppSettings.h"
#include "../../Functionality Classes/Sequencer Mode/SequencerValues.h"
#include "../../Application/CommonInfoBoxText.h"
#include "../AlphaLiveLookandFeel.h"
#include "../../Application/CommandIDs.h"
#include "../../Application/Common.h"



ProjectSettingsComponent::ProjectSettingsComponent(MainComponent &ref, AlphaLiveEngine &ref2, AppDocumentState &ref3)
                                                                :   mainComponentRef(ref),
                                                                    alphaLiveEngineRef(ref2),
                                                                    appDocumentStateRef(ref3)
{
    //temporarily hide global osc feature/settings
    //globalOscComponent = new GlobalOscComponent(mainComponentRef, alphaLiveEngineRef);
    generalSettingsComponent = new GeneralProjSettingsComponent(mainComponentRef, appDocumentStateRef);
    hardwareSettingsComponent = new HardwareSettingsComponent (mainComponentRef, appDocumentStateRef);
    
    //create tabbed component and add tabs/child components
    addAndMakeVisible(tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));

    tabbedComponent->addTab(translate("Software Settings"),
                            AlphaTheme::getInstance()->tabColour,
                            generalSettingsComponent,
                            true);
    tabbedComponent->addTab(translate("Hardware Settings"),
                            AlphaTheme::getInstance()->tabColour,
                            hardwareSettingsComponent,
                            true);
    
    //temporarily hide global osc feature/settings
    //tabbedComponent->addTab(translate("Global OSC Settings"), AlphaTheme::getInstance()->foregroundColourDarker, globalOscComponent, true);
    
    addAndMakeVisible(closeButton = new TextButton());
    closeButton->setButtonText(translate("Close"));
    closeButton->addListener(this);
    closeButton->addMouseListener(this, true);
    
    //set this component to listen to itself
    addKeyListener(this);
}

ProjectSettingsComponent::~ProjectSettingsComponent()
{
    //delete globalOscComponent;
    deleteAllChildren();
    
    
}

void ProjectSettingsComponent::resized()
{
    tabbedComponent->setBounds(getWidth()/4, getHeight()/6, getWidth()/2, ((getHeight()/6)*4)-70);
    closeButton->setBounds((getWidth()/2)-20, ((getHeight()/6)*5)-68, 40, 35);
}

void ProjectSettingsComponent::paint (Graphics& g)
{
    g.setColour(AlphaTheme::getInstance()->backgroundColour.withAlpha(0.8f));
    g.fillRect(0, 0, getWidth(), getHeight());
    
    g.setColour(AlphaTheme::getInstance()->childBackgroundColourLighter.withAlpha(1.0f));
    g.fillRoundedRectangle(getWidth()/4, getHeight()/6, getWidth()/2, ((getHeight()/6)*4)-30, 10);
    
}

void ProjectSettingsComponent::visibilityChanged()
{
    //this is an easy way of updating the values/display of the components/views when needed
    
    if(isVisible())
    {
        hardwareSettingsComponent->updateDisplay();
        generalSettingsComponent->updateDisplay();
    }
}


void ProjectSettingsComponent::buttonClicked (Button *button)
{
    if (button == closeButton)
    {
        setVisible(false);
    }
}

void ProjectSettingsComponent::mouseEnter (const MouseEvent &e)
{
    if (closeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::closeButton));
    }
}

void ProjectSettingsComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}

bool ProjectSettingsComponent::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    if (key == KeyPress::escapeKey)
    {
        setVisible(false);
        
        return true;
    }
    else
        return false; //incase the keypress is shortcut that MainComponent needs to react to.
    
    
}

void ProjectSettingsComponent::updateDisplay()
{
    generalSettingsComponent->updateDisplay();
}

void ProjectSettingsComponent::setTabColour()
{
    //This can't be called in paint as it calls repaint and would cause loops and high CPU.
    for (int i = 0; i < tabbedComponent->getNumTabs(); i++)
        tabbedComponent->setTabBackgroundColour(i, AlphaTheme::getInstance()->tabColour);
}

void ProjectSettingsComponent::selectTab (int tab)
{
    tabbedComponent->setCurrentTabIndex(tab);
}















GlobalOscComponent::GlobalOscComponent(MainComponent &ref, AlphaLiveEngine &ref2)
                                                    :   mainComponentRef(ref),
                                                        alphaLiveEngineRef(ref2)
{
    //GLOBAL OSC OUTPUT SETTINGS
    addAndMakeVisible(globalOscLabel = new Label());
    globalOscLabel->setText(translate("Global OSC Mode:"), dontSendNotification);
    globalOscLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    
    addAndMakeVisible(globalOscSwitch = new TextButton());
    globalOscSwitch->setButtonText(translate("Off"));
    globalOscSwitch->setClickingTogglesState(true);
    globalOscSwitch->setToggleState(false, dontSendNotification);
    globalOscSwitch->addListener(this);
    globalOscSwitch->addMouseListener(this, true);
    
    addAndMakeVisible(oscIpAddressEditor = new Label());
    oscIpAddressEditor->setText("127.0.0.1", dontSendNotification);
    oscIpAddressEditor->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourDarker);
    oscIpAddressEditor->setColour(Label::backgroundColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    oscIpAddressEditor->setJustificationType(Justification::centred);
    oscIpAddressEditor->setEditable(true);
    oscIpAddressEditor->addMouseListener(this, true);
    oscIpAddressEditor->addListener(this);
    oscIpAddressEditor->setVisible(false);
    //AT SOME POINT USE LABEL::CREATEEDITORCOMPONENT() TO SET INPUT RESTRICTIONS TO THE EDITOR
    
    addChildComponent(oscPortNumberSlider = new AlphaSlider());
    oscPortNumberSlider->setRange(0, 65535, 1);
    oscPortNumberSlider->addListener(this);
    oscPortNumberSlider->setValue(5004, dontSendNotification);
    oscPortNumberSlider->addMouseListener(this, true);
    
    
}

GlobalOscComponent::~GlobalOscComponent()
{
    deleteAllChildren();
}

void GlobalOscComponent::resized()
{
    globalOscLabel->setBounds(160, 10, 120, 20);
    globalOscSwitch->setBounds(290, 8, 40, 25);
    oscIpAddressEditor->setBounds(200, 40, 100, 20);
    oscPortNumberSlider->setBounds(224, 70, 48, 48);
}

void GlobalOscComponent::paint (Graphics& g)
{
    
}

void GlobalOscComponent::sliderValueChanged (Slider* slider)
{
    if (slider == oscPortNumberSlider)
    {
        alphaLiveEngineRef.setOscPortNumber(oscPortNumberSlider->getValue());
        //ideally, this should talk to AppSettings, which then talks to alphaLiveEngine
    }
}

void GlobalOscComponent::buttonClicked (Button* button)
{
    if (button == globalOscSwitch)
    {
        if(globalOscSwitch->getToggleStateValue()==true)
        {
            alphaLiveEngineRef.setIsDualOutputMode(true);
            //ideally, this should talk to AppSettings, which then talks to alphaLiveEngine
            
            oscIpAddressEditor->setVisible(true);
            oscPortNumberSlider->setVisible(true);
            
            globalOscSwitch->setButtonText(translate("On"));
        }
        else
        {
            alphaLiveEngineRef.setIsDualOutputMode(false);
            //ideally, this should talk to AppSettings, which then talks to alphaLiveEngine
            
            oscIpAddressEditor->setVisible(false);
            oscPortNumberSlider->setVisible(false);
            
            globalOscSwitch->setButtonText(translate("Off"));
        }
    }
}

void GlobalOscComponent::labelTextChanged (Label* labelThatHasChanged)
{
    if (labelThatHasChanged == oscIpAddressEditor)
    {
        alphaLiveEngineRef.setOscIpAddress(oscIpAddressEditor->getText());
        //ideally, this should talk to AppSettings, which then talks to alphaLiveEngine
    }
}

void GlobalOscComponent::mouseEnter (const MouseEvent &e)
{
    if (globalOscSwitch->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Global OSC Mode. This switch turns on Global OSC Mode which allows you to send Open Sound Control messages from each pad whilst still maintaining the functionality of their audio/MIDI modes."));
    }
    else if (oscIpAddressEditor->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::oscIpAddressEditor));
    }
    else if (oscPortNumberSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::oscportNumberSlider));
    }
}

void GlobalOscComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}








GeneralProjSettingsComponent::GeneralProjSettingsComponent(MainComponent &ref, AppDocumentState &ref2)
                                                :   mainComponentRef(ref),
                                                    appDocumentStateRef(ref2)
{
    addAndMakeVisible(generalGroupComponent = new GroupComponent("general group",
                                                                 translate("General")));
    addAndMakeVisible(copyExternalFilesSwitch = new TextButton());
    copyExternalFilesSwitch->setClickingTogglesState(true);
    copyExternalFilesSwitch->setToggleState(AppSettings::Instance()->getCopyExternalFiles(), dontSendNotification);
    if(copyExternalFilesSwitch->getToggleStateValue() == true)
        copyExternalFilesSwitch->setButtonText(translate("On"));
    else
        copyExternalFilesSwitch->setButtonText(translate("Off")); 
    
    copyExternalFilesSwitch->addListener(this);
    copyExternalFilesSwitch->addMouseListener(this, true);

    addAndMakeVisible(copyExternalFilesLabel = new Label());
    copyExternalFilesLabel->setText(translate("Copy External Files:"), dontSendNotification);
    
    addAndMakeVisible(midiGroupComponent = new GroupComponent("midi group",
                                                                 translate("MIDI")));
    
    addAndMakeVisible(midiClockMenu = new ComboBox());
    midiClockMenu->addItem(translate("Off"), 1);
    midiClockMenu->addItem(translate("Send MIDI Clock"), 2);
    midiClockMenu->addItem(translate("Sync to External MIDI Clock"), 3);
    midiClockMenu->addListener(this);
    midiClockMenu->addMouseListener(this, true);
    midiClockMenu->setSelectedId(AppSettings::Instance()->getMidiClockValue());
    
    addAndMakeVisible(midiClockLabel = new Label());
    midiClockLabel->setText(translate("MIDI Clock:"), dontSendNotification);
    
    addChildComponent(clockStartMessageMenu = new ComboBox());
    clockStartMessageMenu->addItem(translate("Send MIDI Clock Start Message"), 1);
    clockStartMessageMenu->addItem(translate("Send MIDI Clock Continue Message"), 2);
    clockStartMessageMenu->addListener(this);
    clockStartMessageMenu->addMouseListener(this, true);
    clockStartMessageMenu->setSelectedId(AppSettings::Instance()->getMidiClockStartMessage());
    
    addChildComponent(clockStartMessageLabel = new Label());
    clockStartMessageLabel->setText(translate("On Clock Start:"), dontSendNotification);
    
    addChildComponent(midiClockMessageFilterMenu = new ComboBox());
    midiClockMessageFilterMenu->addItem(translate("Use All Clock Messages"), 1);
    midiClockMessageFilterMenu->addItem(translate("Use only Start/Stop Messages"), 2);
    midiClockMessageFilterMenu->addListener(this);
    midiClockMessageFilterMenu->addMouseListener(this, true);
    midiClockMessageFilterMenu->setSelectedId(AppSettings::Instance()->getMidiClockMessageFilter());
    
    addChildComponent(midiClockMessageFilterLabel = new Label());
    midiClockMessageFilterLabel->setText(translate("Message Filter:"), dontSendNotification);
    
    addAndMakeVisible(receiveMidiProgramChangeMessagesMenu = new ComboBox());
    receiveMidiProgramChangeMessagesMenu->addItem(translate("Off"), 1);
    receiveMidiProgramChangeMessagesMenu->addItem(translate("On"), 2);
    receiveMidiProgramChangeMessagesMenu->addListener(this);
    receiveMidiProgramChangeMessagesMenu->addMouseListener(this, true);
    receiveMidiProgramChangeMessagesMenu->setSelectedId(AppSettings::Instance()->getReceiveMidiProgramChangeMessages());
    
    addAndMakeVisible(receiveMidiProgramChangeMessagesLabel = new Label());
    receiveMidiProgramChangeMessagesLabel->setText(translate("Receive Program Change Messages:"), dontSendNotification);
    
}

GeneralProjSettingsComponent::~GeneralProjSettingsComponent()
{
    deleteAllChildren();
}

void GeneralProjSettingsComponent::resized()
{
    generalGroupComponent->setBounds(20, 20, getWidth() - 40, 80);
    
    copyExternalFilesLabel->setBounds(160, 50, 120, 20);
    copyExternalFilesSwitch->setBounds(290, 48, 40, 25);
    
    midiGroupComponent->setBounds(20, 110, getWidth() - 40, 140);
    
    midiClockMenu->setBounds(200, 140, 210, 20);
    midiClockLabel->setBounds(60, 140, 120, 20);
    
    clockStartMessageMenu->setBounds(200, 170, 210, 20);
    clockStartMessageLabel->setBounds(60, 170, 120, 20);
    
    midiClockMessageFilterMenu->setBounds(200, 170, 210, 20);
    midiClockMessageFilterLabel->setBounds(60, 170, 120, 20);
    
    receiveMidiProgramChangeMessagesMenu->setBounds(290, 200, 120, 20);
    receiveMidiProgramChangeMessagesLabel->setBounds(60, 200, 210, 20);
    
}

void GeneralProjSettingsComponent::paint (Graphics& g)
{
//    copyExternalFilesLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
//    midiClockLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
//    clockStartMessageLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
//    midiClockMessageFilterLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
//    receiveMidiProgramChangeMessagesLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
    
//    generalGroupComponent->setColour(GroupComponent::textColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
//    generalGroupComponent->setColour(GroupComponent::outlineColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
//    midiGroupComponent->setColour(GroupComponent::textColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
//    midiGroupComponent->setColour(GroupComponent::outlineColourId, AlphaTheme::getInstance()->foregroundColourLighter.withAlpha(0.5f));
}

void GeneralProjSettingsComponent::sliderValueChanged (Slider* slider)
{
    
}

void GeneralProjSettingsComponent::buttonClicked (Button* button)
{
    //Never want to allow copyExternalFiles to be off and copyLibraryFiles to be on at the same time
    //this would cause problems when saving and loading. Its mainly due to bad programming on my behalf.
    //Ideally need to tidy up th algorithms on loading and saving and handling audio files
    
    
    if (button == copyExternalFilesSwitch)
    {
        if(copyExternalFilesSwitch->getToggleStateValue() == true)
        {
            bool shouldImport = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("Copy External Files"), translate("This command will allow all imported audio files to be copied into the projects directory. It will also instantly import any files currently included in the project that are stored externally. This may take a couple of seconds depending on the number and size of audio files."));
            
            if (shouldImport == true)
            {
                //check all pads of ALL scenes to see if set to Sampler Mode of Samples Sequencer Mode
                //if so, copy any audio files which aren't currently in the Projects Directory and
                //change the saved file path
                
                //SHOULD THIS DONE ONCE THE USER SAVES THE PROJECT AGAIN? for example, if the user does this
                //but then never saves the changes then all the audio files will be redundant within the
                //directory.
                
                appDocumentStateRef.importAudioFiles();
                
                AppSettings::Instance()->setCopyExternalFiles(true);
                copyExternalFilesSwitch->setButtonText(translate("On"));
            }
            else
            {
                copyExternalFilesSwitch->setToggleState(false, dontSendNotification);
            }
        }
        
        else
        {
            AppSettings::Instance()->setCopyExternalFiles(false);
            copyExternalFilesSwitch->setButtonText(translate("Off"));
        }
    }
    
}

void GeneralProjSettingsComponent::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == midiClockMenu)
    {
        AppSettings::Instance()->setMidiClockValue(comboBox->getSelectedId());
        
        setDisplay();
        
        //update the menu bar items status
        commandManager->commandStatusChanged();
        
        int status = mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->isThreadRunning();
        mainComponentRef.getGuiGlobalClock()->updateTransportButtonDisplay(status);
    }
    
    else if (comboBox == clockStartMessageMenu)
    {
        AppSettings::Instance()->setMidiClockStartMessage(comboBox->getSelectedId());
    }
    
    else if (comboBox == midiClockMessageFilterMenu)
    {
        AppSettings::Instance()->setMidiClockMessageFilter(comboBox->getSelectedId());
    }
    else if (comboBox == receiveMidiProgramChangeMessagesMenu)
    {
        AppSettings::Instance()->setReceiveMidiProgramChangeMessages(comboBox->getSelectedId()-1);
    }
}

void GeneralProjSettingsComponent::updateDisplay()
{
    //this function is called from ProjectSettingsComponent::visibilityChanged
    
    //set component values...
    
    copyExternalFilesSwitch->setToggleState(AppSettings::Instance()->getCopyExternalFiles(), dontSendNotification);
    if(copyExternalFilesSwitch->getToggleStateValue() == true)
        copyExternalFilesSwitch->setButtonText(translate("On"));
    else
        copyExternalFilesSwitch->setButtonText(translate("Off"));

    midiClockMenu->setSelectedId(AppSettings::Instance()->getMidiClockValue(), dontSendNotification);
    clockStartMessageMenu->setSelectedId(AppSettings::Instance()->getMidiClockStartMessage(), dontSendNotification);
    midiClockMessageFilterMenu->setSelectedId(AppSettings::Instance()->getMidiClockMessageFilter(), dontSendNotification);
    
    receiveMidiProgramChangeMessagesMenu->setSelectedId(AppSettings::Instance()->getReceiveMidiProgramChangeMessages() + 1, dontSendNotification);
    
    setDisplay();

}

void GeneralProjSettingsComponent::setDisplay()
{
    //set component visibility...
    
    clockStartMessageLabel->setVisible(false);
    clockStartMessageMenu->setVisible(false);
    midiClockMessageFilterLabel->setVisible(false);
    midiClockMessageFilterMenu->setVisible(false);
    
    int midiClockValue = midiClockMenu->getSelectedId();
    
    switch (midiClockValue)
    {
        case 2:
            clockStartMessageLabel->setVisible(true);
            clockStartMessageMenu->setVisible(true);
            break;
        case 3:
            midiClockMessageFilterLabel->setVisible(true);
            midiClockMessageFilterMenu->setVisible(true);
            break;
        default:
            break;
    }
}

void GeneralProjSettingsComponent::mouseEnter (const MouseEvent &e)
{
    if (copyExternalFilesSwitch->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Copy External Audio Files Options. By default when an external audio file is added to an AlphaLive project it is copied into the projects directory. Use this button to turn this option off. Be aware that with this set to off the project will not be able to link to any external audio files if moved onto another computer."));
    }
    else if (midiClockMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("MIDI Clock Mode Selector. Sets and displays the MIDI Clock functionality of the current AlphaLive project. AlphaLive can both send and receive MIDI Clock messages in order to synchronise this application with other MIDI applications and devices. Note that when set to sync with an external MIDI Clock, AlphaLive's global clock cannot be controlled directly via AlphaLive."));
    }
    else if (clockStartMessageMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sets and displays what type of MIDI Clock message is sent when AlphaLive's clock is started. A Clock Start message will start the sequence of the external MIDI software/device from the beginning, whereas a Clock Continue message will continue the sequence from its current position."));
    }
    else if (midiClockMessageFilterMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sets and displays what type of MIDI Clock messages are received and used by AlphaLive. By default any external clock that AlphaLive is synced to will start and stop AlphaLive's clock as well as keep it in sync and adjust its tempo. However if the option to only use Start/Stop messages is selected the external clock will only start and stop AlphaLive's clock."));
    }
    else if (receiveMidiProgramChangeMessagesMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("AlphaLive is able to receive MIDI Program Change Messages from external MIDI devices/software to trigger scene switching within AlphaLive. Send a Program Change message on any MIDI channel with a Program Change number in the range of 1 - 20 (or 0 - 19 if given 127 available programs instead of 128) to change to the corresponding scene number. Use this menu to disable this feature for the current project."));
    }

}

void GeneralProjSettingsComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}















HardwareSettingsComponent::HardwareSettingsComponent(MainComponent &ref, AppDocumentState &ref2)
                                                        :   mainComponentRef(ref),
                                                            appDocumentStateRef(ref2)
{
    addAndMakeVisible(ledModeLabel = new Label());
    ledModeLabel->setText(translate("LED MIDI CC Control Mode:"), dontSendNotification);
    addAndMakeVisible(ledModeButton = new TextButton());
    ledModeButton->setClickingTogglesState(true);
    ledModeButton->setToggleState(AppSettings::Instance()->getHardwareLedMode(), dontSendNotification);
    if (ledModeButton->getToggleState() == true)
        ledModeButton->setButtonText(translate("On"));
    else
        ledModeButton->setButtonText(translate("Off"));
    ledModeButton->addListener(this);
    ledModeButton->addMouseListener(this, true);
    
    addAndMakeVisible(ledStatusLabel = new Label());
    ledStatusLabel->setText(translate("LED Status:"), dontSendNotification);
    addAndMakeVisible(ledStatusButton = new TextButton());
    ledStatusButton->setClickingTogglesState(true);
    ledStatusButton->setToggleState(AppSettings::Instance()->getHardwareLedStatus(), dontSendNotification);
    if (ledStatusButton->getToggleState() == true)
        ledStatusButton->setButtonText(translate("On"));
    else
        ledStatusButton->setButtonText(translate("Off"));
    ledStatusButton->addListener(this);
    ledStatusButton->addMouseListener(this, true);
    
    addAndMakeVisible(ledPressureStatusLabel = new Label());
    ledPressureStatusLabel->setText(translate("LED Pressure Interaction:"), dontSendNotification);
    addAndMakeVisible(ledPressureStatusButton = new TextButton());
    ledPressureStatusButton->setClickingTogglesState(true);
    ledPressureStatusButton->setToggleState(AppSettings::Instance()->getHardwareLedPressureStatus(), dontSendNotification);
    if (ledPressureStatusButton->getToggleState() == true)
        ledPressureStatusButton->setButtonText(translate("On"));
    else
        ledPressureStatusButton->setButtonText(translate("Off"));
    ledPressureStatusButton->addListener(this);
    ledPressureStatusButton->addMouseListener(this, true);
    
    addAndMakeVisible(ledClockStatusLabel = new Label());
    ledClockStatusLabel->setText(translate("LED Clock Interaction:"), dontSendNotification);
    addAndMakeVisible(ledClockStatusButton = new TextButton());
    ledClockStatusButton->setClickingTogglesState(true);
    ledClockStatusButton->setToggleState(AppSettings::Instance()->getHardwareLedClockStatus(), dontSendNotification);
    if (ledClockStatusButton->getToggleState() == true)
        ledClockStatusButton->setButtonText(translate("On"));
    else
        ledClockStatusButton->setButtonText(translate("Off"));
    ledClockStatusButton->addListener(this);
    ledClockStatusButton->addMouseListener(this, true);

}

HardwareSettingsComponent::~HardwareSettingsComponent()
{
    deleteAllChildren();
}

void HardwareSettingsComponent::resized()
{
    ledModeButton->setBounds(280+50, 18, 40, 25);
    ledModeLabel->setBounds(60+50, 20, 200, 20);
    
    ledStatusButton->setBounds(280+50, 58, 40, 25);
    ledStatusLabel->setBounds(60+50, 60, 200, 20);
    
    ledPressureStatusButton->setBounds(280+50, 98, 40, 25);
    ledPressureStatusLabel->setBounds(60+50, 100, 200, 20);
    
    ledClockStatusButton->setBounds(280+50, 138, 40, 25);
    ledClockStatusLabel->setBounds(60+50, 140, 200, 20);
}

void HardwareSettingsComponent::paint (Graphics& g)
{
//    ledModeLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
//    ledStatusLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
//    ledPressureStatusLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
//    ledClockStatusLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->foregroundColourLighter);
}

void HardwareSettingsComponent::buttonClicked (Button* button)
{
    if (button == ledModeButton)
    {
        JUCEApplication::getInstance()->perform(CommandIDs::EnableLedMidiMode);
        
        if (button->getToggleState() == true)
            button->setButtonText(translate("On"));
        else
            button->setButtonText(translate("Off"));
        
        setDisplay();
    }
    
    else if (button == ledStatusButton)
    {
        JUCEApplication::getInstance()->perform(CommandIDs::EnableLed);
        
        if (button->getToggleState() == true)
            button->setButtonText(translate("On"));
        else
            button->setButtonText(translate("Off"));
        
        setDisplay();
    }
    
    else if (button == ledPressureStatusButton)
    {
        JUCEApplication::getInstance()->perform(CommandIDs::EnableLedPressure);
        
        if (button->getToggleState() == true)
            button->setButtonText(translate("On"));
        else
            button->setButtonText(translate("Off"));
    }
    
    else if (button == ledClockStatusButton)
    {
        JUCEApplication::getInstance()->perform(CommandIDs::EnableLedClock);
        
        if (button->getToggleState() == true)
            button->setButtonText(translate("On"));
        else
            button->setButtonText(translate("Off"));
    }
    
}

void HardwareSettingsComponent::comboBoxChanged (ComboBox *comboBox)
{
    
}


void HardwareSettingsComponent::mouseEnter (const MouseEvent &e)
{
    if (ledModeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Use this button to enable LED MIDI CC Control Mode for the current project. This mode allows the LED to be controlled using external MIDI Control Change messages, set to Channel 16, being sent to the AlphaSphere. Use CC 21 to control the red value, CC 22 to control the green value, and CC 23 to control the blue value, where a value of '0' is 'off' and '127' is 'full brightness'. You can also enable this mode externally using CC 20 - a value of '0' disables the mode while anything else enables it. Please note that this mode disables pressure and clock LED control."));
    }
    else if (ledStatusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Use this button to enable/disable the LED for the current project. When this is disabled it will disable pressure and clock LED control."));
    }
    else if (ledPressureStatusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Use this button to enable/disable the LED pressure interaction for the current project. When this is disabled the LED will stay at a static colour."));
    }
    else if (ledClockStatusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Use this button to enable/disable the LED clock interaction for the current project. When this is enabled you can use either the AlphaLive clock or an external MIDI clock to animate the LED with synchronised fade-outs. Please not that the LED can only sync to a single clock source at any time."));
    }
}

void HardwareSettingsComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}

void HardwareSettingsComponent::updateDisplay()
{
    //this function is called from ProjectSettingsComponent::visibilityChanged
    
    ledModeButton->setToggleState(AppSettings::Instance()->getHardwareLedMode(), dontSendNotification);
    if (ledModeButton->getToggleState() == true)
        ledModeButton->setButtonText(translate("On"));
    else
        ledModeButton->setButtonText(translate("Off"));
    
    ledStatusButton->setToggleState(AppSettings::Instance()->getHardwareLedStatus(), dontSendNotification);
    if (ledStatusButton->getToggleState() == true)
        ledStatusButton->setButtonText(translate("On"));
    else
        ledStatusButton->setButtonText(translate("Off"));
    
    ledPressureStatusButton->setToggleState(AppSettings::Instance()->getHardwareLedPressureStatus(), dontSendNotification);
    if (ledPressureStatusButton->getToggleState() == true)
        ledPressureStatusButton->setButtonText(translate("On"));
    else
        ledPressureStatusButton->setButtonText(translate("Off"));
    
    ledClockStatusButton->setToggleState(AppSettings::Instance()->getHardwareLedClockStatus(), dontSendNotification);
    if (ledClockStatusButton->getToggleState() == true)
        ledClockStatusButton->setButtonText(translate("On"));
    else
        ledClockStatusButton->setButtonText(translate("Off"));
    
    setDisplay();
}

void HardwareSettingsComponent::setDisplay()
{
    //sets which buttons are enabled/dissabled
    
    ledStatusButton->setEnabled(false);
    ledStatusButton->setAlpha(0.4);
    ledPressureStatusButton->setEnabled(false);
    ledPressureStatusButton->setAlpha(0.4);
    ledClockStatusButton->setEnabled(false);
    ledClockStatusButton->setAlpha(0.4);
    
    if (AppSettings::Instance()->getHardwareLedMode() == 0)
    {
        if (AppSettings::Instance()->getHardwareLedStatus() == 0)
        {
            ledStatusButton->setEnabled(true);
            ledStatusButton->setAlpha(1.0);
        }
        else
        {
            ledStatusButton->setEnabled(true);
            ledStatusButton->setAlpha(1.0);
            ledPressureStatusButton->setEnabled(true);
            ledPressureStatusButton->setAlpha(1.0);
            ledClockStatusButton->setEnabled(true);
            ledClockStatusButton->setAlpha(1.0);
        }
    }
    
}


