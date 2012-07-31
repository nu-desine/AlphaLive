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
   // audioPlayer.setSource(&audioMixer);
    
    for (int i = 0; i <= 15; i++)
        currentExclusivePadLooper.insert(i, NULL);
    
    audioTransportSourceThread = new TimeSliceThread("Looper Audio Thread");
    audioTransportSourceThread->startThread();
}


ModeLooper::~ModeLooper()
{
    audioMixer.removeAllInputs();
    
    currentExclusivePadLooper.clear(false);
    padLooper.clear(true);
    
    audioTransportSourceThread->stopThread(100);
    //delete audioTransportSourceThread;
    
}


//This function gets called everytime 'Looper mode' is set to a pad
//and creates an instance of AudioFilePlayer which handles the playing of audio files
void ModeLooper::createAudioFilePlayer (int padNumber)
{
    
    std::cout << "Creating AudioFilePlayer for pad " << padNumber <<std::endl;
    padLooper.remove(padNumber); //remove NULL object
    padLooper.insert(padNumber, new AudioFilePlayer(padNumber, *this, audioTransportSourceThread)); //add AudioFilePlayer object
    audioMixer.addInputSource(padLooper[padNumber],false); //add as inputsource to audioMixer
}



//This function gets called everytime a pad set to Looper mode is changes to a different mode
//and deletes the relevant instance of AudioFilePlayer 
void ModeLooper::deleteAudioFilePlayer (int padNumber)
{
    //if deleted object is currently part of the currentExclusivePadLooper array, remove it.
    if (currentExclusivePadLooper.contains(padLooper[padNumber]))
    {
        //get index of array
        int index = currentExclusivePadLooper.indexOf(padLooper[padNumber]);
        //remove seq object from array
        currentExclusivePadLooper.remove(index, false);
        //fill index with NULL
        currentExclusivePadLooper.insert(index, NULL);
    }
    
    //if deleted object is currently part of the waitingPadLooper array, remove it.
    //DO I NEED TO DO THIS?
    if (waitingPadLooper.contains(padLooper[padNumber]))
    {
        //get index of array
        int index = waitingPadLooper.indexOf(padLooper[padNumber]);
        //remove seq object from array
        waitingPadLooper.remove(index, false);
    }
    
    std::cout << "Deleting AudioFilePlayer for pad " << padNumber <<std::endl;
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

void ModeLooper::addItemToWaitingPadLooper (AudioFilePlayer* item)
{
    waitingPadLooper.addIfNotAlreadyThere(item);
}


void ModeLooper::triggerQuantizationPoint()
{
    if (waitingPadLooper.size() > 0)
    {
        for (int i = 0; i < waitingPadLooper.size(); i++)
        {
            //alert padLooper[i] of a quantization point in time
            waitingPadLooper[i]->triggerQuantizationPoint();
        }
    
        //remove items from array so they no longer recieve alerts of
        //quantization points in time
        waitingPadLooper.clear(false);
    }
}


void ModeLooper::stopExclusivePadLooper (int channel, AudioFilePlayer* item)
{
    if (currentExclusivePadLooper[channel-1] != NULL) //if it exists...
    {
        //stop previous loop of said channel if not the same as the new one
        if (currentExclusivePadLooper[channel-1] != item)
            currentExclusivePadLooper[channel-1]->stopAudioFile();
    }
    
    if (currentExclusivePadLooper[channel-1] != item)
    {
        //remove previous loop object from array
        currentExclusivePadLooper.remove(channel-1, false);
        //add new loop object to array
        currentExclusivePadLooper.insert(channel-1, item);
    }
    
    //should there be a method here for removing objects from this array when they have finshed playing? - prefereably yes! See MIDI mode
}

void ModeLooper::killPad (int padNum)
{
    if (padLooper[padNum] != NULL) //if it exists..
    {
        //should there be a check here to see if the pad is currently playing?
        
        padLooper[padNum]->stopAudioFile();
        //set playing state to 0?
        
        padLooper[padNum]->killAllAudio(); //to kill things like delay tails
    }
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

