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

//#define PAD_SETTINGS padSettingsSampler[padNumber]
#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]


ModeSequencer::ModeSequencer(AlphaLiveEngine &ref)
                             :  Thread("sequencerThread"), 
                                alphaLiveEngineRef(ref)
{
    //init the sequenceplayer array to be empty
    //needs to be initialised as you cannot dynamcically add an object, say at index 30, if the indexes before it don't exist
    for (int i = 0; i <=47; i++)
    {
        padSequencer.insert(i, NULL);
        prevPadValue[i] = 0; //for the 'exclusive' mode
    }
    
    tempo = AppSettings::Instance()->getGlobalTempo();
    
    audioTransportSourceThread = new TimeSliceThread("Sequencer Audio Thread");
    audioTransportSourceThread->startThread();
    
    lastPreviewedSequencePadNum = 100; //no sequence default value
}


ModeSequencer::~ModeSequencer()
{
    audioMixer.removeAllInputs();
    padSequencer.clear(true);
    
    audioTransportSourceThread->stopThread(100);
    //delete audioTransportSourceThread;
    
    
}


//this function is called in AlphaLiveEngine whenever an OSC message is recieved
//who's pad is set to Sequencer mode
void ModeSequencer::getInputData(int padNumber, int padValue)
{
    //process the sequence
    //should i put this in an if statement to make sure the this index of padSequencer actually exists?
    //it should always do, but might be safe to play safe.
    padSequencer[padNumber]->processInputData(padValue);
    
    //if the incoming message is a 'press'
    if (prevPadValue[padNumber] == 0)
    {
        //if clock isn't currently running and autoStartClock is on, start clock
        //Is it too CPU heavy getting the following two variables from their sources each time? 
        //Should these variables be past into this class each time they change?
        if (alphaLiveEngineRef.getGlobalClock()->isThreadRunning() == false && AppSettings::Instance()->getAutoStartClock() == 1)
        {
            alphaLiveEngineRef.getGlobalClock()->startClock();
        }
    }
    
    prevPadValue[padNumber] = padValue;
}

void ModeSequencer::editRunningSequencersArray (int action, int padNumber)
{
    if (action == 1)
    {
        //if this is the first object added to the array, start the thread
        if (runningSequencers.size() == 0)
            startThread(7);
            
        //add padSequencer object to array
        runningSequencers.addIfNotAlreadyThere(padSequencer[padNumber]);
    }
    else if (action == 0)
    {
        //remove padSequencer object from array
        runningSequencers.removeFirstMatchingValue(padSequencer[padNumber]);
        
        if (runningSequencers.size() == 0)
        {
            //int setGlobalTimeInterval;
            stopThread(100);
        }
        
        
    }
}

void ModeSequencer::run()
{
    //std::cout << "Global Sequencer Thread Starting!" << std::endl;
    
    while( ! threadShouldExit())
    {
        for (int i = 0; i < runningSequencers.size(); i++)
        {
            runningSequencers[i]->processSequence();
        }
        
        wait(1); 
    }
    
    //std::cout << "Global Sequencer Thread Stopped!" << std::endl;
}


void ModeSequencer::createSequencePlayer (int padNumber)
{
    padSequencer.remove(padNumber); //remove NULL object
    padSequencer.insert(padNumber, new SequencePlayer(padNumber, *this, audioTransportSourceThread)); 
    //add SequencePlayer object
    audioMixer.addInputSource(padSequencer[padNumber],false); //add as inputsource to audioMixer
}

void ModeSequencer::deleteSequencePlayer (int padNumber)
{
    //this needs to be removed from here
    editRunningSequencersArray(0, padNumber);
    
    audioMixer.removeInputSource(padSequencer[padNumber]); //remove as input source
    padSequencer.remove(padNumber); //remove object from array
    padSequencer.insert(padNumber, NULL); //insert a NULL object
}


void ModeSequencer::triggerQuantizationPoint (int padNum)
{
    if (padSequencer[padNum]!= NULL)
    {
        //alert padSequencer[padNum] of a quantization point in time
        padSequencer[padNum]->triggerQuantizationPoint();
    }

}


void ModeSequencer::killPad (int padNum)
{
    if (padSequencer[padNum] != NULL) //if it exists..
    {
        //should there be a check here to see if the pad is currently playing?
        padSequencer[padNum]->stopSequenceAndReset();
    }
}

void ModeSequencer::stopPrevExclusivePad (int padNum)
{
    padSequencer[padNum]->stopSequence();
}

void ModeSequencer::setPreviewSequenceNumber (int padNum, int sequenceNumber)
{
    padSequencer[padNum]->setSequenceNumber(sequenceNumber);
}

void ModeSequencer::previewSequence (int padNum, int status)
{
    //what should I do here?
    //should there be a dedicated class/object similar to sequencerplayer
    //that just plays a sequence completetly seperate from the sequenceplayer
    //objects for each pad?
    //or not...?
    if (status == 1)
        padSequencer[padNum]->processInputData(1);
    else
        padSequencer[padNum]->stopSequenceAndReset();
    
    lastPreviewedSequencePadNum = padNum;
    
}

void ModeSequencer::stopLastPreviewedSequence()
{
    if (padSequencer[lastPreviewedSequencePadNum] != nullptr &&
        padSequencer[lastPreviewedSequencePadNum]->isCurrentlyPlaying() == true)
    {
        padSequencer[lastPreviewedSequencePadNum]->stopSequenceAndReset();
        lastPreviewedSequencePadNum = 100; //no sequence default value
    }
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

AlphaLiveEngine& ModeSequencer::getAlphaLiveEngineRef()
{
    return alphaLiveEngineRef;
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

