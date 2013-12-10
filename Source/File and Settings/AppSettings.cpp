//
//  AppSettings.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 22/09/2011.
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

#include "AppSettings.h"

AppSettings* AppSettings::pInstance = 0;

AppSettings* AppSettings::Instance()
{
    if (pInstance == 0) //is this is the first call
    {
        //create sole instance
        pInstance = new AppSettings();
    }
    
    //address of sole instance
    return pInstance;
}


AppSettings::AppSettings()
{
    //default values
    //currentlySelectedPad = 99; //what should this default be?
    padDisplayTextMode = 1;
    
    globalPan = 0.5;
    globalGain = 1.0;
    
    globalTempo = 120.0;
    quantizationValue = 3; //1 Bar
    beatsPerBar = 4;
    autoStartClock = 0; //off
    metronomeStatus = false;
    
    //==== project settings ====
    for (int i = 0; i < 20; i++)
        sceneName[i] = "Scene " + String(i+1);
    
    copyExternalFiles = true;
    midiClockValue = 1;
    midiClockStartMessage = 1;
    midiClockMessageFilter = 1;
    
    receiveMidiProgramChangeMessages = true;
    
    hardwareLedMode = 0;
    hardwareLedStatus = 1;
    hardwareLedPressureStatus = 1;
    hardwareLedClockStatus = 0;
    
    // ==== elite controls settings ====
    eliteDial[0].control = 1;
    eliteDial[1].control = 2;
    eliteButton[0].control = 1;
    eliteButton[1].control = 6;
    eliteButton[2].control = 2;
    
    for (int i = 0; i < 2; i++)
    {
        eliteDial[i].status = 1;
        eliteDial[i].prevValue = 0.0;
        eliteDial[i].midiControllerType = 0;
        eliteDial[i].midiCcNumber = 12;
        eliteDial[i].midiChannel = 1;
        eliteDial[i].midiMinRange = 0;
        eliteDial[i].midiMaxRange = 127;
        eliteDial[i].oscPortNumber = 5004;
        eliteDial[i].oscMinRange = 0;
        eliteDial[i].oscMaxRange = 511;
        eliteDial[i].oscIpAddress = "127.0.0.1";
        eliteDial[i].oscStepValue = 1.0;
    }
    for (int i = 0; i < 3; i++)
    {
        eliteButton[i].status = 1;
        eliteButton[i].sceneMode = 1;
        eliteButton[i].sceneNumber = 2;
        eliteButton[i].midiCcNumber = 13;
        eliteButton[i].midiChannel = 1;
        eliteButton[i].midiOffNumber = 0;
        eliteButton[i].midiOnNumber = 127;
        eliteButton[i].oscPortNumber = 5004;
        eliteButton[i].oscOffNumber = 0;
        eliteButton[i].oscOnNumber = 1;
        eliteButton[i].oscIpAddress = "127.0.0.1";
    }

    //=================================Pad Settings stuff==================================
    //'for' loop to create 48 objects of the PadSettings class, which are put into an array.
    //each object of the array holds the settings for a different pad
    //uses index values 0-47 (so the pad number for the user will be index+1)
    for (int i = 0; i <=47; i++)
    {
        padSettings.insert(i, new PadSettings(i));
    }
    
    copiedPadSettings = new PadSettings(NULL);
    
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
            copiedSequencerData[row][column] = 0;
    }
    
    lastAudioSampleDirectory = File::getSpecialLocation (File::userMusicDirectory);
}


void AppSettings::setAlphaLiveEngineRef (AlphaLiveEngine *ref)
{
    alphaLiveEngineRef = ref;
    
    //pass the OSC routing ref into all the padSettings instances
    for (int i = 0; i <=47; i++)
    {
        padSettings[i]->setAlphaLiveEngineRef(alphaLiveEngineRef);
    }
    
    copiedPadSettings->setAlphaLiveEngineRef(alphaLiveEngineRef);
}

void AppSettings::resetData()
{
    setGlobalPan(0.5);
    setGlobalGain(1.0);
    setGlobalTempo(120.0);
    
    setQuantizationValue(3);
    setBeatsPerBar(4);
    setAutoStartClock(0);
    
    setCopyExternalFiles(true);
    setMidiClockValue(1);
    setMidiClockStartMessage(1);
    setMidiClockMessageFilter(1);
    
    setReceiveMidiProgramChangeMessages(true);
    
    //elite controls stuff
    eliteDial[0].control = 1;
    eliteDial[1].control = 2;
    eliteButton[0].control = 1;
    eliteButton[1].control = 6;
    eliteButton[2].control = 2;
    for (int i = 0; i < 2; i++)
    {
        eliteDial[i].status = 1;
        eliteDial[i].prevValue = 0.0;
        eliteDial[i].midiControllerType = 0;
        eliteDial[i].midiCcNumber = 12;
        eliteDial[i].midiChannel = 1;
        eliteDial[i].midiMinRange = 0;
        eliteDial[i].midiMaxRange = 127;
        eliteDial[i].oscPortNumber = 5004;
        eliteDial[i].oscMinRange = 0;
        eliteDial[i].oscMaxRange = 511;
        eliteDial[i].oscIpAddress = "127.0.0.1";
        eliteDial[i].oscStepValue = 1.0;
    }
    for (int i = 0; i < 3; i++)
    {
        eliteButton[i].status = 1;
        eliteButton[i].sceneMode = 1;
        eliteButton[i].sceneNumber = 2;
        eliteButton[i].midiCcNumber = 13;
        eliteButton[i].midiChannel = 1;
        eliteButton[i].midiOffNumber = 0;
        eliteButton[i].midiOnNumber = 127;
        eliteButton[i].oscPortNumber = 5004;
        eliteButton[i].oscOffNumber = 0;
        eliteButton[i].oscOnNumber = 1;
        eliteButton[i].oscIpAddress = "127.0.0.1";
    }
}

void AppSettings::resetProjectSettingsData()
{
    setCopyExternalFiles(true);
    
    for (int i = 0; i < 20; i++)
        setSceneName(i, "Scene " + String(i+1));
    
    setHardwareLedMode(0);
    setHardwareLedStatus(1);
    setHardwareLedPressureStatus(1);
    setHardwareLedClockStatus(0);
}

void AppSettings::copyPadSettings (int padNumber)
{
    //copy the selected padSettings object
    *copiedPadSettings = *padSettings[padNumber];
}

void AppSettings::pastePadSettings (int padNumber)
{
    //set the contents of the pad's padSettings to the content of copiedPadSettings
    *padSettings[padNumber] = *copiedPadSettings;
    //set the pad number of this 'new' version
    padSettings[padNumber]->setPadNumber(padNumber);
    
    //get mode, then reset mode before applying it again.
    //This is needed so that the sampler mode (and probably the seq mode) are handled properly
    //as the audioFilePlayer class constructor must be called with the right pad number set beforehand, for example.
    int mode = padSettings[padNumber]->getMode();
    padSettings[padNumber]->setMode(0);
    padSettings[padNumber]->setMode(mode);
    
    
}

void AppSettings::copySequenceData (int value, int row, int column)
{
    copiedSequencerData[row][column] = value;
}

int AppSettings::pasteSequenceData (int row, int column)
{
    return copiedSequencerData[row][column];
}

//=====================================================================

void AppSettings::setCurrentlySelectedPad(Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}
void AppSettings::setPadDisplayTextMode(int value)
{
    padDisplayTextMode = value;
}

#pragma mark scene mutator functions

void AppSettings::setGlobalPan (float value)
{
    globalPan = value;
    alphaLiveEngineRef->setPan(globalPan);
}

void AppSettings::setGlobalGain (float value)
{
    globalGain = value;
    alphaLiveEngineRef->setGain(globalGain);
}



void AppSettings::setGlobalTempo (float value)
{
    globalTempo = value;
    
    //send the tempo to all relevent places
    alphaLiveEngineRef->getModeSequencer()->setTempo(globalTempo);
    alphaLiveEngineRef->getGlobalClock()->setTempo(globalTempo);
    for (int i = 0; i < 48; i++)
        padSettings[i]->setTempo(globalTempo);
}

void AppSettings::setQuantizationValue(int value)
{
    quantizationValue = value;
    alphaLiveEngineRef->getGlobalClock()->setQuantizationValue(value);
}

void AppSettings::setBeatsPerBar (int value)
{
    beatsPerBar = value;
    alphaLiveEngineRef->getGlobalClock()->setBeatsPerBar(beatsPerBar);
}
void AppSettings::setAutoStartClock (int value)
{
    autoStartClock = value;
}
void AppSettings::setMetronomeStatus (bool value)
{
    metronomeStatus = value;
    alphaLiveEngineRef->getGlobalClock()->setMetronomeStatus(value);
}

#pragma mark project settings mutator functions

void AppSettings::setCopyExternalFiles (bool value)
{
    copyExternalFiles = value;
}
void AppSettings::setMidiClockValue (int value)
{
    midiClockValue = value;
    alphaLiveEngineRef->getGlobalClock()->setMidiClockValue(value);
    alphaLiveEngineRef->setMidiClockValue(value);
    
    for (int i = 0; i < 48; i++)
    {
        if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(i) != nullptr)
            alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(i)->setMidiClockValue(value);
    }
}
void AppSettings::setMidiClockStartMessage (int value)
{
    midiClockStartMessage = value;
    alphaLiveEngineRef->getGlobalClock()->setMidiClockStartMessage(value);
}
void AppSettings::setMidiClockMessageFilter (int value)
{
    midiClockMessageFilter = value;
    alphaLiveEngineRef->getGlobalClock()->setMidiClockMessageFilter(value);
    alphaLiveEngineRef->setMidiClockMessageFilter(value);
    
    for (int i = 0; i < 48; i++)
    {
        if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(i) != nullptr)
            alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(i)->setMidiClockMessageFilter(value);
    }
}

void AppSettings::setReceiveMidiProgramChangeMessages(bool value)
{
    receiveMidiProgramChangeMessages = value;
    alphaLiveEngineRef->setReceiveMidiProgramChangeMessages(value);
}

void AppSettings::setSceneName(int sceneNumber, String value)
{
    sceneName[sceneNumber] = value;
}

void AppSettings::setHardwareLedMode (int value)
{
    hardwareLedMode = value;
    
    //send setting value to hardware
    alphaLiveEngineRef->setLedSettings(4, value);
}
void AppSettings::setHardwareLedStatus (int value)
{
    hardwareLedStatus = value;
    
    //send setting value to hardware
    alphaLiveEngineRef->setLedSettings(1, value);
}
void AppSettings::setHardwareLedPressureStatus (int value)
{
    hardwareLedPressureStatus = value;
    
    //send setting value to hardware
    alphaLiveEngineRef->setLedSettings(2, value);
}
void AppSettings::setHardwareLedClockStatus (int value)
{
    hardwareLedClockStatus = value;
    
    //send setting value to hardware
    alphaLiveEngineRef->setLedSettings(3, value);
}


Array<int> AppSettings::getCurrentlySelectedPad()
{
    return selectedPads;
}

int AppSettings::getPadDisplayTextMode()
{
    return padDisplayTextMode;
}

#pragma mark scene accessor functions

float AppSettings::getGlobalPan()
{
    return globalPan;
}
float AppSettings::getGlobalGain()
{
    return globalGain;
}


float AppSettings::getGlobalTempo()
{
    return globalTempo;
}
int AppSettings::getQuantizationValue()
{
    return quantizationValue;
}
int AppSettings::getBeatsPerBar()
{
    return beatsPerBar;
}
int AppSettings::getAutoStartClock()
{
    return autoStartClock;
}
bool AppSettings::getMetronomeStatus()
{
    return metronomeStatus;
}

#pragma mark project settings accessor functions

bool AppSettings::getCopyExternalFiles()
{
    return copyExternalFiles;
}
int AppSettings::getMidiClockValue()
{
    return midiClockValue;
}
int AppSettings::getMidiClockStartMessage()
{
    return midiClockStartMessage;
}
int AppSettings::getMidiClockMessageFilter()
{
    return midiClockMessageFilter;
}

bool AppSettings::getReceiveMidiProgramChangeMessages()
{
    return receiveMidiProgramChangeMessages;
}

String AppSettings::getSceneName (int sceneNumber)
{
    return sceneName[sceneNumber];
}

int AppSettings::getHardwareLedMode()
{
    return hardwareLedMode;
}
int AppSettings::getHardwareLedStatus()
{
    return hardwareLedStatus;
}
int AppSettings::getHardwareLedPressureStatus()
{
    return hardwareLedPressureStatus;
}
int AppSettings::getHardwareLedClockStatus()
{
    return hardwareLedClockStatus;
}


void AppSettings::setLastAudioSampleDirectory (File directory)
{
    lastAudioSampleDirectory = directory;
}

File AppSettings::getLastAudioSampleDirectory()
{
    return lastAudioSampleDirectory;
}

#pragma mark elite control settings mutator functions

//Elite controls stuff
void AppSettings::setEliteDialPrevValue(double value, int dialNumber)
{
    eliteDial[dialNumber].prevValue = value;
}
void AppSettings::setEliteDialControl(int value, int dialNumber)
{
    eliteDial[dialNumber].control = value;
}
void AppSettings::setEliteDialStatus(int value, int dialNumber)
{
    eliteDial[dialNumber].status = value;
}

void AppSettings::setEliteDialMidiControllerType(int value, int dialNumber)
{
    eliteDial[dialNumber].midiControllerType = value;
}
void AppSettings::setEliteDialMidiCcNumber(int value, int dialNumber)
{
    eliteDial[dialNumber].midiCcNumber = value;
}
void AppSettings::setEliteDialMidiChannel(int value, int dialNumber)
{
    eliteDial[dialNumber].midiChannel = value;
}
void AppSettings::setEliteDialMidiMinRange(int value, int dialNumber)
{
    eliteDial[dialNumber].midiMinRange = value;
}
void AppSettings::setEliteDialMidiMaxRange(int value, int dialNumber)
{
    eliteDial[dialNumber].midiMaxRange = value;
}
void AppSettings::setEliteDialOscPortNumber(int value, int dialNumber)
{
    eliteDial[dialNumber].oscPortNumber = value;
}
void AppSettings::setEliteDialOscIpAddress(String value, int dialNumber)
{
    eliteDial[dialNumber].oscIpAddress = value;
}
void AppSettings::setEliteDialOscMinRange(double value, int dialNumber)
{
    eliteDial[dialNumber].oscMinRange = value;
}
void AppSettings::setEliteDialOscMaxRange(double value, int dialNumber)
{
    eliteDial[dialNumber].oscMaxRange = value;
}
void AppSettings::setEliteDialOscStepValue(double value, int dialNumber)
{
    eliteDial[dialNumber].oscStepValue = value;
}

#pragma mark elite control settings accessor functions

double AppSettings::getEliteDialPrevValue (int dialNumber)
{
    return eliteDial[dialNumber].prevValue;
}
int AppSettings::getEliteDialControl (int dialNumber)
{
    return eliteDial[dialNumber].control;
}
int AppSettings::getEliteDialStatus(int dialNumber)
{
    return eliteDial[dialNumber].status;
}

int AppSettings::getEliteDialMidiControllerType(int dialNumber)
{
    return eliteDial[dialNumber].midiControllerType;
}
int AppSettings::getEliteDialMidiCcNumber(int dialNumber)
{
    return eliteDial[dialNumber].midiCcNumber;
}
int AppSettings::getEliteDialMidiChannel(int dialNumber)
{
    return eliteDial[dialNumber].midiChannel;
}
int AppSettings::getEliteDialMidiMinRange(int dialNumber)
{
    return eliteDial[dialNumber].midiMinRange;
}
int AppSettings::getEliteDialMidiMaxRange(int dialNumber)
{
    return eliteDial[dialNumber].midiMaxRange;
}
int AppSettings::getEliteDialOscPortNumber(int dialNumber)
{
    return eliteDial[dialNumber].oscPortNumber;
}
String AppSettings::getEliteDialOscIpAddress(int dialNumber)
{
    return eliteDial[dialNumber].oscIpAddress;
}
double AppSettings::getEliteDialOscMinRange(int dialNumber)
{
    return eliteDial[dialNumber].oscMinRange;
}
double AppSettings::getEliteDialOscMaxRange(int dialNumber)
{
    return eliteDial[dialNumber].oscMaxRange;
}
double AppSettings::getEliteDialOscStepValue(int dialNumber)
{
    return eliteDial[dialNumber].oscStepValue;
}

void AppSettings::setEliteButtonControl(int value, int buttonNumber)
{
    eliteButton[buttonNumber].control = value;
}
void AppSettings::setEliteButtonStatus(int value, int buttonNumber)
{
    eliteButton[buttonNumber].status = value;
}

void AppSettings::setEliteButtonSceneMode(int value, int buttonNumber)
{
    eliteButton[buttonNumber].sceneMode = value;
}
void AppSettings::setEliteButtonSceneNumber(int value, int buttonNumber)
{
    eliteButton[buttonNumber].sceneNumber = value;
}
void AppSettings::setEliteButtonMidiCcNumber(int value, int buttonNumber)
{
    eliteButton[buttonNumber].midiCcNumber = value;
}
void AppSettings::setEliteButtonMidiChannel(int value, int buttonNumber)
{
    eliteButton[buttonNumber].midiChannel = value;
}
void AppSettings::setEliteButtonMidiOffNumber(int value, int buttonNumber)
{
    eliteButton[buttonNumber].midiOffNumber = value;
}
void AppSettings::setEliteButtonMidiOnNumber(int value, int buttonNumber)
{
    eliteButton[buttonNumber].midiOnNumber = value;
}
void AppSettings::setEliteButtonOscPortNumber(int value, int buttonNumber)
{
    eliteButton[buttonNumber].oscPortNumber = value;
}
void AppSettings::setEliteButtonOscIpAddress(String value, int buttonNumber)
{
    eliteButton[buttonNumber].oscIpAddress = value;
}
void AppSettings::setEliteButtonOscOffNumber(double value, int buttonNumber)
{
    eliteButton[buttonNumber].oscOffNumber = value;
}
void AppSettings::setEliteButtonOscOnNumber(double value, int buttonNumber)
{
    eliteButton[buttonNumber].oscOnNumber = value;
}

int AppSettings::getEliteButtonControl (int buttonNumber)
{
    return eliteButton[buttonNumber].control;
}
int AppSettings::getEliteButtonStatus(int buttonNumber)
{
    return eliteButton[buttonNumber].status;
}
int AppSettings::getEliteButtonSceneMode(int buttonNumber)
{
    return eliteButton[buttonNumber].sceneMode;
}
int AppSettings::getEliteButtonSceneNumber(int buttonNumber)
{
    return eliteButton[buttonNumber].sceneNumber;
}
int AppSettings::getEliteButtonMidiCcNumber(int buttonNumber)
{
    return eliteButton[buttonNumber].midiCcNumber;
}
int AppSettings::getEliteButtonMidiChannel(int buttonNumber)
{
    return eliteButton[buttonNumber].midiChannel;
}
int AppSettings::getEliteButtonMidiOffNumber(int buttonNumber)
{
    return eliteButton[buttonNumber].midiOffNumber;
}
int AppSettings::getEliteButtonMidiOnNumber(int buttonNumber)
{
    return eliteButton[buttonNumber].midiOnNumber;
}
int AppSettings::getEliteButtonOscPortNumber(int buttonNumber)
{
    return eliteButton[buttonNumber].oscPortNumber;
}
String AppSettings::getEliteButtonOscIpAddress(int buttonNumber)
{
    return eliteButton[buttonNumber].oscIpAddress;
}
double AppSettings::getEliteButtonOscOffNumber(int buttonNumber)
{
    return eliteButton[buttonNumber].oscOffNumber;
}
double AppSettings::getEliteButtonOscOnNumber(int buttonNumber)
{
    return eliteButton[buttonNumber].oscOnNumber;
}



