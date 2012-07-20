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
    
    for (int i = 0; i <=5; i++)
    {
        looperChannelMode[i] = 1;
        sequencerChannelMode[i] = 1;
    }
    
    copyExternalFiles = true;

    
    //=================================Pad Settings stuff==================================
    //'for' loop to create 48 objects of the PadSettings class, which are put into an array.
    //each object of the array holds the settings for a different pad
    //uses index values 0-47 (so the pad number for the user will be index+1)
    for (int i = 0; i <=47; i++)
    {
        padSettings.insert(i, new PadSettings(i));
    }
    
    copiedPadSettings = new PadSettings(NULL);
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
    for (int i = 0; i <=5; i++)
    {
        setLooperChannelMode(i, 1);
        setSequencerChannelMode(i, 1);
    }
    
    setQuantizationValue(3);
    setBeatsPerBar(4);
    setAutoStartClock(0);
    setCopyExternalFiles(true);
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
    //This is needed so that the looper mode (and probably the seq mode) are handled properly
    //as the audioFilePlayer class constructor must be called with the right pad number set beforehand, for example.
    int mode = padSettings[padNumber]->getMode();
    padSettings[padNumber]->setMode(0);
    padSettings[padNumber]->setMode(mode);
    
    
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
}

void AppSettings::setQuantizationValue(int value)
{
    quantizationValue = value;
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


void AppSettings::setLooperChannelMode (int channelNumber, int value)
{
    looperChannelMode[channelNumber] = value;
}
void AppSettings::setSequencerChannelMode(int channelNumber, int value)
{
    sequencerChannelMode[channelNumber] = value;
}

void AppSettings::setCopyExternalFiles (bool value)
{
    copyExternalFiles = value;
}



Array<int> AppSettings::getCurrentlySelectedPad()
{
    return selectedPads;
}

int AppSettings::getPadDisplayTextMode()
{
    return padDisplayTextMode;
}



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

int AppSettings::getLooperChannelMode (int channelNumber)
{
    return looperChannelMode[channelNumber];
}

int AppSettings::getSequencerChannelMode(int channelNumber)
{
    return sequencerChannelMode[channelNumber];
}

bool AppSettings::getCopyExternalFiles()
{
    return copyExternalFiles;
}



