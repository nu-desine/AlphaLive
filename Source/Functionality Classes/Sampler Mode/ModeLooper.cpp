//
//  ModeLooper.cpp
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

#include "ModeLooper.h"
#include "../../File and Settings/AppSettings.h"
#include "../AlphaLiveEngine.h"

//#define PAD_SETTINGS padSettingsLooper[padNumber]
#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]


ModeLooper::ModeLooper(AlphaLiveEngine &ref)
                    :   alphaLiveEngineRef(ref)
                        
{
    //init the audiofileplayer array to be empty
    //needs to be initialised as you cannot dynamcically add an object, say at index 30, if the indexes before it don't exist
    for (int i = 0; i <=47; i++)
    {
        padLooper.insert(i, NULL);
        prevPadValue[i] = 0;
    }
    
    audioTransportSourceThread = new TimeSliceThread("Looper Audio Thread");
    audioTransportSourceThread->startThread();
}


ModeLooper::~ModeLooper()
{
    audioMixer.removeAllInputs();
    padLooper.clear(true);
    
    audioTransportSourceThread->stopThread(100);
    //delete audioTransportSourceThread;
    
}


//This function gets called everytime 'Looper mode' is set to a pad
//and creates an instance of AudioFilePlayer which handles the playing of audio files
void ModeLooper::createAudioFilePlayer (int padNumber)
{
    padLooper.remove(padNumber); //remove NULL object
    padLooper.insert(padNumber, new AudioFilePlayer(padNumber, *this, audioTransportSourceThread)); //add AudioFilePlayer object
    audioMixer.addInputSource(padLooper[padNumber],false); //add as inputsource to audioMixer
}



//This function gets called everytime a pad set to Looper mode is changes to a different mode
//and deletes the relevant instance of AudioFilePlayer 
void ModeLooper::deleteAudioFilePlayer (int padNumber)
{
    audioMixer.removeInputSource(padLooper[padNumber]); //remove as input source
    padLooper.remove(padNumber); //remove object from array
    padLooper.insert(padNumber, NULL); //insert a NULL object
    
}

void ModeLooper::updatePadPlayingStatus(int padNumber, int playingStatus)
{
    alphaLiveEngineRef.updatePadPlayingStatus(padNumber, playingStatus);
}

AudioFilePlayer* ModeLooper::getAudioFilePlayerInstance (int padNumber)
{
    return padLooper[padNumber];
}


//this function is called in AlphaLiveEngine whenever an OSC message is recieved
//who's pad is set to Looper mode
void ModeLooper::getOscData(int padNumber, int padValue)
{
    
    //process the audio file
    //should i put this in an if statement to make sure the this index of padSequencer actually exists?
    //it should always do, but might be safe to play safe.
    padLooper[padNumber]->processAudioFile(padValue);
    
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

void ModeLooper::triggerQuantizationPoint (int padNum)
{
    if (padLooper[padNum]!= NULL)
    {
        //alert padLooper[padNum] of a quantization point in time
        padLooper[padNum]->triggerQuantizationPoint();
    }
    
}


void ModeLooper::killPad (int padNum)
{
    if (padLooper[padNum] != NULL) //if it exists..
    {
        //should there be a check here to see if the pad is currently playing?
        
        padLooper[padNum]->stopAudioFile();
        padLooper[padNum]->killAllAudio(); //to kill things like delay tails
        padLooper[padNum]->resetTriggerMode();
    }
}

AlphaLiveEngine& ModeLooper::getAlphaLiveEngineRef()
{
    return alphaLiveEngineRef;
}

void ModeLooper::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    audioMixer.getNextAudioBlock(bufferToFill);
}

void ModeLooper::prepareToPlay (int samplesPerBlockExpected,double sampleRate)
{
    audioMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void ModeLooper::releaseResources()
{
    audioMixer.releaseResources();
}

