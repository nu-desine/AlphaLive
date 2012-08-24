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



ProjectSettingsComponent::ProjectSettingsComponent(MainComponent &ref, AlphaLiveEngine &ref2, AppDocumentState &ref3)
                                                                :   mainComponentRef(ref),
                                                                    alphaLiveEngineRef(ref2),
                                                                    appDocumentStateRef(ref3)
{
    globalOscComponent = new GlobalOscComponent(mainComponentRef, alphaLiveEngineRef);
    generalSettingsComponent = new GeneralProjSettingsComponent(mainComponentRef, appDocumentStateRef);
    
    //create tabbed component and add tabs/child components
    addAndMakeVisible(tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    tabbedComponent->addTab(translate("General Settings"), Colours::darkgrey, generalSettingsComponent, true);
    tabbedComponent->addTab(translate("Global OSC Settings"), Colours::darkgrey, globalOscComponent, true);
    
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
    closeButton->setBounds((getWidth()/2)-35, ((getHeight()/6)*5)-60, 70, 20);
}

void ProjectSettingsComponent::paint (Graphics& g)
{
    g.setColour(Colours::black.withAlpha(0.8f));
    g.fillRect(0, 0, getWidth(), getHeight());
    
    g.setColour(Colours::grey.withAlpha(0.9f));
    g.fillRoundedRectangle(getWidth()/4, getHeight()/6, getWidth()/2, ((getHeight()/6)*4)-30, 10);
    
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















GlobalOscComponent::GlobalOscComponent(MainComponent &ref, AlphaLiveEngine &ref2)
                                                    :   mainComponentRef(ref),
                                                        alphaLiveEngineRef(ref2)
{
    //GLOBAL OSC OUTPUT SETTINGS
    addAndMakeVisible(globalOscLabel = new Label());
    globalOscLabel->setText(translate("Global OSC Mode:"), false);
    globalOscLabel->setColour(Label::textColourId, Colours::lightgrey);
    
    addAndMakeVisible(globalOscSwitch = new TextButton());
    globalOscSwitch->setButtonText(translate("Off"));
    globalOscSwitch->setClickingTogglesState(true);
    globalOscSwitch->setToggleState(false, false);
    globalOscSwitch->addListener(this);
    globalOscSwitch->addMouseListener(this, true);
    
    addAndMakeVisible(oscIpAddressEditor = new Label());
    oscIpAddressEditor->setText("127.0.0.1", false);
    oscIpAddressEditor->setColour(Label::textColourId, Colours::darkgrey);
    oscIpAddressEditor->setColour(Label::backgroundColourId, Colours::lightgrey);
    oscIpAddressEditor->setJustificationType(Justification::centred);
    oscIpAddressEditor->setEditable(true);
    oscIpAddressEditor->addMouseListener(this, true);
    oscIpAddressEditor->addListener(this);
    oscIpAddressEditor->setVisible(false);
    //AT SOME POINT USE LABEL::CREATEEDITORCOMPONENT() TO SET INPUT RESTRICTIONS TO THE EDITOR
    
    addAndMakeVisible(oscPortNumberSlider = new AlphaSlider());
    oscPortNumberSlider->setRange(0, 65535, 1);
    oscPortNumberSlider->addListener(this);
    oscPortNumberSlider->setValue(5004, false);
    oscPortNumberSlider->addMouseListener(this, true);
    oscPortNumberSlider->setVisible(false);
    
    
}

GlobalOscComponent::~GlobalOscComponent()
{
    deleteAllChildren();
}

void GlobalOscComponent::resized()
{
    globalOscLabel->setBounds(160, 10, 120, 20);
    globalOscSwitch->setBounds(290, 10, 50, 20);
    oscIpAddressEditor->setBounds(200, 40, 100, 20);
    oscPortNumberSlider->setBounds(200, 70, 100, 20);
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
        mainComponentRef.setInfoTextBoxText(translate("Global OSC Mode. Use this switch to turn on Global OSC Mode which allows you to send out Open Sound Control messages from each pad whilst still maintaining the functionality of their audio/MIDI modes."));
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
    addAndMakeVisible(copyExternalFilesSwitch = new TextButton());
    copyExternalFilesSwitch->setClickingTogglesState(true);
    copyExternalFilesSwitch->setToggleState(AppSettings::Instance()->getCopyExternalFiles(), false);
    if(copyExternalFilesSwitch->getToggleStateValue() == true)
        copyExternalFilesSwitch->setButtonText(translate("On"));
    else
        copyExternalFilesSwitch->setButtonText(translate("Off")); 
    
    copyExternalFilesSwitch->addListener(this);
    copyExternalFilesSwitch->addMouseListener(this, true);
    
    addAndMakeVisible(copyExternalFilesLabel = new Label());
    copyExternalFilesLabel->setText("Copy External Files:", false);
    copyExternalFilesLabel->setColour(Label::textColourId, Colours::lightgrey);
    
}

GeneralProjSettingsComponent::~GeneralProjSettingsComponent()
{
    deleteAllChildren();
}

void GeneralProjSettingsComponent::resized()
{
    copyExternalFilesLabel->setBounds(160, 10, 120, 20);
    copyExternalFilesSwitch->setBounds(290, 10, 50, 20);
    
}

void GeneralProjSettingsComponent::paint (Graphics& g)
{
    
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
                copyExternalFilesSwitch->setToggleState(false, false);
            }
        }
        
        else
        {
            AppSettings::Instance()->setCopyExternalFiles(false);
            copyExternalFilesSwitch->setButtonText(translate("Off"));
        }
    }
    
}

void GeneralProjSettingsComponent::updateDisplay()
{
    copyExternalFilesSwitch->setToggleState(AppSettings::Instance()->getCopyExternalFiles(), false);
    if(copyExternalFilesSwitch->getToggleStateValue() == true)
        copyExternalFilesSwitch->setButtonText(translate("On"));
    else
        copyExternalFilesSwitch->setButtonText(translate("Off"));
}

void GeneralProjSettingsComponent::mouseEnter (const MouseEvent &e)
{
    if (copyExternalFilesSwitch->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Copy External Audio Files Options. By default when an external audio file is added to an AlphaLive project it is copied into the projects directory. Use this button to turn this option off. Be aware that with this set to off the project will not be able to link to any external audio files if moved onto another computer."));
    }

}

void GeneralProjSettingsComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}


