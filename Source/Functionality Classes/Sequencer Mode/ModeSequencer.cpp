//
//  ModeSequencer.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 25/09/2011.
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

#include "ModeSequencer.h"
#include "../../File and Settings/AppSettings.h"
#include "SequencerValues.h"
#include "../AlphaLiveEngine.h"

//#define PAD_SETTINGS padSettingsLooper[padNumber]
#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]


ModeSequencer::ModeSequencer(MidiOutput &midiOutput, AlphaLiveEngine &ref)
                             :  midiOutputDevice(&midiOutput),
                                alphaLiveEngineRef(ref)
{
    //init the sequenceplayer array to be empty
    //needs to be initialised as you cannot dynamcically add an object, say at index 30, if the indexes before it don't exist
    for (int i = 0; i <=47; i++)
    {
        padSequencer.insert(i, NULL);
        prevPadValue[i] = 0; //for the 'exclusive' mode
    }
    
    for (int i = 0; i <= 15; i++)
        currentExclusivePadSequencer.insert(i, NULL);
    
    tempo = AppSettings::Instance()->getGlobalTempo();
    
    audioTransportSourceThread = new TimeSliceThread("Sequencer Audio Thread");
    audioTransportSourceThread->startThread();
}


ModeSequencer::~ModeSequencer()
{
    audioMixer.removeAllInputs();
    
    currentExclusivePadSequencer.clear(false);
    padSequencer.clear(true);
    
    audioTransportSourceThread->stopThread(100);
    //delete audioTransportSourceThread;
    
    
}


//this function is called in AlphaLiveEngine whenever an OSC message is recieved
//who's pad is set to Sequencer mode
void ModeSequencer::getOscData(int padNumber, int padValue)
{
    //process the sequence
    //should i put this in an if statement to make sure the this index of padSequencer actually exists?
    //it should always do, but might be safe to play safe.
    padSequencer[padNumber]->processSequence(padValue);
    
    //if the incoming message is a 'press'
    if (prevPadValue[padNumber] == 0)
    {
        //if clock isn't currently running and autoStartClock is on, start clock
        //Is it too CPU heavy getting the following two variables from their sources each time? 
        //Should these variables be past into this class each time they change?
        if (alphaLiveEngineRef.getGlobalClock()->isThreadRunning() == false && AppSettings::Instance()->getAutoStartClock() == 1)
        {
            std::cout << "Clock Starting!" << std::endl;
            alphaLiveEngineRef.getGlobalClock()->startClock();
        }
    }
    
    prevPadValue[padNumber] = padValue;
}


void ModeSequencer::createSequencePlayer (int padNumber)
{
    std::cout << "Creating SequencePlayer for pad " << padNumber <<std::endl;
    padSequencer.remove(padNumber); //remove NULL object
    padSequencer.insert(padNumber, new SequencePlayer(padNumber, *midiOutputDevice, *this, audioTransportSourceThread)); 
    //add SequencePlayer object
    audioMixer.addInputSource(padSequencer[padNumber],false); //add as inputsource to audioMixer
}

void ModeSequencer::deleteSequencePlayer (int padNumber)
{
    //if deleted object is currently part of the currentExclusivePadSequencer array, remove it.
    if (currentExclusivePadSequencer.contains(padSequencer[padNumber]))
    {
        //get index of array
        int index = currentExclusivePadSequencer.indexOf(padSequencer[padNumber]);
        //remove seq object from array
        currentExclusivePadSequencer.remove(index, false);
        //fill index with NULL
        currentExclusivePadSequencer.insert(index, NULL);
    }
    
    //if deleted object is currently part of the waitingPadSequencer array, remove it.
    //DO I NEED TO DO THIS?
    if (waitingPadSequencer.contains(padSequencer[padNumber]))
    {
        //get index of array
        int index = waitingPadSequencer.indexOf(padSequencer[padNumber]);
        //remove seq object from array
        waitingPadSequencer.remove(index, false);
    }
    
    std::cout << "Deleting SequencerPlayer for pad " << padNumber <<std::endl;
    audioMixer.removeInputSource(padSequencer[padNumber]); //remove as input source
    padSequencer.remove(padNumber); //remove object from array
    padSequencer.insert(padNumber, NULL); //insert a NULL object
    

}


void ModeSequencer::addItemToWaitingPadSequencer (SequencePlayer* item)
{
    waitingPadSequencer.addIfNotAlreadyThere(item);
}


void ModeSequencer::triggerQuantizationPoint()
{
    if (waitingPadSequencer.size() > 0)
    {
        for (int i = 0; i < waitingPadSequencer.size(); i++)
        {
            //alert padLooper[i] of a quantization point in time
            waitingPadSequencer[i]->triggerQuantizationPoint();
        }
        
        //remove items from array so they no longer recieve alerts of
        //quantization points in time
        waitingPadSequencer.clear(false);
    }
}


void ModeSequencer::stopExclusivePadSequencer (int channel, SequencePlayer* item)
{
    if (currentExclusivePadSequencer[channel-1] != NULL) //if the sequennce object exists...
    {
        //stop previous sequence of said channel if not the same as the new one
        if (currentExclusivePadSequencer[channel-1] != item)
        {
            currentExclusivePadSequencer[channel-1]->stopThread(currentExclusivePadSequencer[channel-1]->getTimeInterval());
            //should i be setting the playing status of said pad to 0 here?
            //or is that what's being handled at the top of processSequence()
            //within SequencePlayer?
        }
        
    }
    
    if (currentExclusivePadSequencer[channel-1] != item)
    {
        //remove previous seq object from array
        currentExclusivePadSequencer.remove(channel-1, false);
        //add new seq object to array
        currentExclusivePadSequencer.insert(channel-1, item);
    }
    
    //should there be a method here for removing objects from this array when they have finshed playing?
}

void ModeSequencer::killPad (int padNum)
{
    //for (int i = 0; i <= 47; i++)
    //{
        if (padSequencer[padNum] != NULL) //if it exists..
        {
            padSequencer[padNum]->stopThread(padSequencer[padNum]->getTimeInterval());
            //set playing state to 0?
        }
    //}
}


void ModeSequencer::updatePadPlayingStatus(int padNumber, int playingStatus)
{
    alphaLiveEngineRef.updatePadPlayingStatus(padNumber, playingStatus);
}



SequencePlayer* ModeSequencer::getSequencePlayerInstance (int padNumber)
{
    return padSequencer[padNumber];
}

void ModeSequencer::setTempo(float value)
{
    for (int i = 0; i <= 47; i++)
    {
        //if the padSequencer instance exists, set its tempo
        if (padSequencer[i] != NULL)
        {
            padSequencer[i]->setTempo(value);
        }
    }
}


//notify observer class - SequencerGrid
void ModeSequencer::updateSequencerGridGui (int columnNumber_, int sequenceNumber_, int whatShouldUpdateFlag_)
{
    columnNumber = columnNumber_;
    sequenceNumber = sequenceNumber_;
    whatShouldUpdateFlag = whatShouldUpdateFlag_;
    
    notifyObs();
}

int ModeSequencer::getCurrentColumnNumber()
{
    return columnNumber;
}

int ModeSequencer::getCurrentSequenceNumber()
{
    return sequenceNumber;
}

int ModeSequencer::getWhatShouldUpdateFlag()
{
    return whatShouldUpdateFlag;
}

void ModeSequencer::setMidiOutputDevice (MidiOutput &midiOutput)
{
    midiOutputDevice = &midiOutput;
    
    for (int i = 0; i <= 47; i++)
    {
        if (padSequencer[i] != NULL)
        {
            padSequencer[i]->setMidiOutputDevice (*midiOutputDevice);
        }
    }
}

void ModeSequencer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    audioMixer.getNextAudioBlock(bufferToFill);
}

void ModeSequencer::prepareToPlay (int samplesPerBlockExpected,double sampleRate)
{
    audioMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void ModeSequencer::releaseResources()
{
    audioMixer.releaseResources();
}

