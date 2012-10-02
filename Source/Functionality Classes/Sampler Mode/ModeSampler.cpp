//
//  ModeSampler.cpp
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

#include "ModeSampler.h"
#include "../../File and Settings/AppSettings.h"
#include "../AlphaLiveEngine.h"

//#define PAD_SETTINGS padSettingsSampler[padNumber]
#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]


ModeSampler::ModeSampler(AlphaLiveEngine &ref)
                    :   alphaLiveEngineRef(ref)
                        
{
    //init the audiofileplayer array to be empty
    //needs to be initialised as you cannot dynamcically add an object, say at index 30, if the indexes before it don't exist
    for (int i = 0; i <=47; i++)
    {
        padSampler.insert(i, NULL);
        prevPadValue[i] = 0;
    }
    
    audioTransportSourceThread = new TimeSliceThread("Sampler Audio Thread");
    audioTransportSourceThread->startThread();
}


ModeSampler::~ModeSampler()
{
    audioMixer.removeAllInputs();
    padSampler.clear(true);
    
    audioTransportSourceThread->stopThread(100);
    //delete audioTransportSourceThread;
    
}


//This function gets called everytime 'Sampler mode' is set to a pad
//and creates an instance of AudioFilePlayer which handles the playing of audio files
void ModeSampler::createAudioFilePlayer (int padNumber)
{
    padSampler.remove(padNumber); //remove NULL object
    padSampler.insert(padNumber, new AudioFilePlayer(padNumber, *this, audioTransportSourceThread)); //add AudioFilePlayer object
    audioMixer.addInputSource(padSampler[padNumber],false); //add as inputsource to audioMixer
}



//This function gets called everytime a pad set to Sampler mode is changes to a different mode
//and deletes the relevant instance of AudioFilePlayer 
void ModeSampler::deleteAudioFilePlayer (int padNumber)
{
    audioMixer.removeInputSource(padSampler[padNumber]); //remove as input source
    padSampler.remove(padNumber); //remove object from array
    padSampler.insert(padNumber, NULL); //insert a NULL object
    
}

void ModeSampler::updatePadPlayingStatus(int padNumber, int playingStatus)
{
    alphaLiveEngineRef.updatePadPlayingStatus(padNumber, playingStatus);
}

AudioFilePlayer* ModeSampler::getAudioFilePlayerInstance (int padNumber)
{
    return padSampler[padNumber];
}


//this function is called in AlphaLiveEngine whenever an OSC message is recieved
//who's pad is set to Sampler mode
void ModeSampler::getOscData(int padNumber, int padValue)
{
    
    //process the audio file
    //should i put this in an if statement to make sure the this index of padSequencer actually exists?
    //it should always do, but might be safe to play safe.
    padSampler[padNumber]->processAudioFile(padValue);
    
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

void ModeSampler::triggerQuantizationPoint (int padNum)
{
    if (padSampler[padNum]!= NULL)
    {
        //alert padSampler[padNum] of a quantization point in time
        padSampler[padNum]->triggerQuantizationPoint();
    }
    
}


void ModeSampler::killPad (int padNum)
{
    if (padSampler[padNum] != NULL) //if it exists..
    {
        //should there be a check here to see if the pad is currently playing?
        padSampler[padNum]->killAllAudio(); //to kill things like delay tails
        padSampler[padNum]->stopAudioFile(true);
        padSampler[padNum]->resetTriggerMode();
    }
}

void ModeSampler::stopPrevExclusivePad (int padNum)
{
    padSampler[padNum]->stopAudioFile(false);
}

AlphaLiveEngine& ModeSampler::getAlphaLiveEngineRef()
{
    return alphaLiveEngineRef;
}

void ModeSampler::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    audioMixer.getNextAudioBlock(bufferToFill);
}

void ModeSampler::prepareToPlay (int samplesPerBlockExpected,double sampleRate)
{
    audioMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void ModeSampler::releaseResources()
{
    audioMixer.releaseResources();
}

