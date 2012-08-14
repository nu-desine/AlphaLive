//
//  AudioFilePlayer.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 29/09/2011.
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

#include "AudioFilePlayer.h"
#include "../../File and Settings/AppSettings.h"
#include "ModeSampler.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]


AudioFilePlayer::AudioFilePlayer(int samplerPadNumber, ModeSampler &ref, TimeSliceThread* audioTransportSourceThread_)
                                    :   padNumber(samplerPadNumber),
                                        modeSamplerRef(ref)
{
    audioTransportSourceThread = audioTransportSourceThread_;
    
    currentAudioFileSource = NULL;
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
    
    //init all effects to be null
    gainAndPan = nullptr;
    lowPassFilter = nullptr;
    highPassFilter = nullptr;
    bandPassFilter = nullptr;
    reverb = nullptr;
    delay = nullptr;
    flanger = nullptr;
    tremolo = nullptr;
    
    //grab the setting values (so that if this object is deleted and recreated, it will hold the previous settings)
    //do i need to enter shared memory here?
    //do ALL effect paramters need setting here? Can I do this in the dedicated effect classes? Would be neater that way
    gain = gainPrev = PAD_SETTINGS->getSamplerGain();
    panLeft = panLeftPrev = PanControl::leftChanPan_(PAD_SETTINGS->getSamplerPan());
    panRight = panRightPrev = PanControl::rightChanPan_(PAD_SETTINGS->getSamplerPan());
    triggerMode = PAD_SETTINGS->getSamplerTriggerMode();
    shouldLoop = PAD_SETTINGS->getSamplerShouldLoop();
    indestructible = PAD_SETTINGS->getSamplerIndestructible();
    shouldFinishLoop = PAD_SETTINGS->getSamplerShouldFinishLoop();
    sticky = PAD_SETTINGS->getSamplerSticky();
    currentPlayingState = currentPressureValue = 0;
    //set effect to default 0, and then call set effect to create the effect object
    //This alg. prevents any crashes caused within prepareToPlay when trying to
    //set the sampleRate, where the effect object must exist
    effect = 0;
    setEffect(PAD_SETTINGS->getSamplerEffect());
    quantizeMode = PAD_SETTINGS->getQuantizeMode();
    
    triggerModeData.playingStatus = 0;
    
    prevPadValue = pressureValue =  0;
    
    playingLastLoop = false;
    
    
    //call this here incase a loop has been 'dropped' onto a pad before this AudioFilePlayer instance actually exists,
    //which means that it wouldn't have been called from setSamplerAudioFilePath().
    setAudioFile(PAD_SETTINGS->getSamplerAudioFilePath());
    
    broadcaster.addActionListener(this);
}

AudioFilePlayer::~AudioFilePlayer()
{
    delete gainAndPan;
    delete lowPassFilter;
    delete highPassFilter;
    delete bandPassFilter;
    delete delay;
    delete reverb;
    delete flanger;
    delete tremolo;
    
    modeSamplerRef.updatePadPlayingStatus(padNumber, 0);
    fileSource.setSource(0);//unload the current file
	deleteAndZero(currentAudioFileSource);//delete the current file
}

bool AudioFilePlayer::getAudioTransportSourceStatus()
{
    if (fileSource.isPlaying() == true)
        return true;
    else
        return false;
    
}

void AudioFilePlayer::processAudioFile(int padValue)
{
    if (currentFile != File::nonexistent && currentAudioFileSource != NULL)
    {
        //this is needed incase audio ends on its own or is stopped via the 'exclusive mode' feature, in order to reset everything so it will trigger again properly
        if (fileSource.isPlaying() == false && currentPlayingState == 1 && prevPadValue == 0)
        {
            std::cout << "stream ended on its own!!";
            currentPlayingState = 0;
            triggerModes.reset();
        }
        
        //==========================================================================================
        // Trigger Mode stuff
        //==========================================================================================
        switch (triggerMode) 
        {
            case 1:
                triggerModeData = triggerModes.hold(padValue);
                break;
            case 2:
                triggerModeData = triggerModes.toggle(padValue);
                break;
            case 3:
                triggerModeData = triggerModes.toggleRelease(padValue);
                break;
            case 4:
                triggerModeData = triggerModes.latch(padValue);
                break;
            case 5:
                triggerModeData = triggerModes.latchMax(padValue);
                break;
            case 6:
                triggerModeData = triggerModes.trigger(padValue);
                break;
            default:
                triggerModeData = triggerModes.toggle(padValue);
                break;
        }
        
        
        
        
        if (triggerModeData.playingStatus == 0 
            && shouldFinishLoop == 1 
            && indestructible == 0 
            && currentPlayingState == 1)
        {
            //if recieved a command to stop file but is set to finish current loop before stopping,
            //ignore note off message and set looping status to off
            triggerModeData.playingStatus = 2; //ignore
            currentAudioFileSource->setLooping(false);
            playingLastLoop = true;
            broadcaster.sendActionMessage("WAITING TO STOP");
            
            //what about if the user wants to cancel the finish loop command?
            //curently, if the user presses the pad again it will restart call
            //the below if else statement
        }
        else if (triggerModeData.playingStatus == 1 
                 && playingLastLoop == true 
                 && indestructible == 0 
                 && currentPlayingState == 1)
        {
            currentAudioFileSource->setLooping(shouldLoop);
            playingLastLoop = false;
            triggerModeData.playingStatus = 1;
        }
        
        if (playingLastLoop == false)
        {
            currentAudioFileSource->setLooping(shouldLoop);
        }
        
        
        
        
        
        
        
        if (indestructible == 1)
        {
            //if set to indestructible...
            
            if (triggerModeData.playingStatus == 0)
            {
                //...and triggerModeData signifies to stop audio, DON'T LET IT...MWAHAHAHA! 
                triggerModeData.playingStatus = 2; //ignore
            }
            else if (triggerModeData.playingStatus == 1 && currentPlayingState == 1 && triggerMode != 6)
            {
                //...and triggerModeData signifies to start playing, 
                //but file is already playing and triggerMode does not equal 'trigger'
                //Don't send a play command!
                triggerModeData.playingStatus = 2; //ignore
            }
            
        }
        
        //==========================================================================================
        // Start/Stop stuff
        //==========================================================================================
        if (quantizeMode == 0) //free
        {
            if (triggerModeData.playingStatus == 1) //play
            {
                playAudioFile();
            }
            
            else if (triggerModeData.playingStatus == 0) //stop
            {
                stopAudioFile();
                //currentPlayingState = 0; //now done within stopAudioFile()
            }
        }
        //==========================================================================================
        else if (quantizeMode == 1) //quantized
        {
            if (triggerModeData.playingStatus == 1) //play
            {
                currentPlayingState = 2; //waiting to play
                
                //add this objects padNumber to the queuedPads Array within AlphaLiveEngine
                //so that it is alerted of the next quantized point in time so
                //the sample can start playing
                modeSamplerRef.getAlphaLiveEngineRef().addPadToQueue(padNumber);
                broadcaster.sendActionMessage("WAITING TO PLAY");
                
                
            }
            
            else if (triggerModeData.playingStatus == 0) //stop
            {
                currentPlayingState = 3; // waiting to stop
                
                //add this objects padNumber to the queuedPads Array within AlphaLiveEngine
                //so that it is alerted of the next quantized point in time so
                //the sample can stop playing
                modeSamplerRef.getAlphaLiveEngineRef().addPadToQueue(padNumber);
                broadcaster.sendActionMessage("WAITING TO STOP");
            }
        }
        //=========================================================================================
        
        //==========================================================================================
        // Pressure stuff
        //==========================================================================================
        
        if (sticky == 1) //'on'
        {
            //modify pressure value
            if(prevPadValue == 0)
            {
                pressureValue = 0;
                
            }
            else
            {
                if(padValue > pressureValue)
                {
                    pressureValue = padValue;
                }
                //else if it is smaller, don't change the pressure value
            }
            
        }
        else // 'off'
        {
            pressureValue = padValue; 
        }
        
        
        
        //determine what effect and parameter the pressure is controlling
        
        switch (effect)
        {
            case 1: //Gain and Pan
                gainAndPan->processAlphaTouch(pressureValue);
                break;
            case 2: //LPF
                lowPassFilter->processAlphaTouch(pressureValue);
                break;
            case 3: //HPF
                highPassFilter->processAlphaTouch(pressureValue);
                break;
            case 4: //BPF
                bandPassFilter->processAlphaTouch(pressureValue);
                break;
            case 7: //Delay
                delay->processAlphaTouch(pressureValue);
                break;
            case 8: //Reverb
                reverb->processAlphaTouch(pressureValue);
                break;
            case 9: //Flanger
                flanger->processAlphaTouch(pressureValue);
                break;
            case 10: //Tremolo
                tremolo->processAlphaTouch(pressureValue);
                break;
            default:
                break;
        }
        
        prevPadValue = padValue;
        
    }
}

//called from either the constructor, or setSamplerAudioFilePath() in PadSettings
void AudioFilePlayer::setAudioFile (File audioFile_)
{
    if (audioFile_ != File::nonexistent)
    {
        //passes in pads audio file
        File audioFile (audioFile_);
        
        //if the audio file is different from the previous one, stop and load in the new file
        if (audioFile != currentFile)
        {
            // unload the previous file source and delete it..
            broadcaster.sendActionMessage("OFF");
            fileSource.stop();
            fileSource.setPosition(0.0);
            fileSource.setSource (0);
            deleteAndZero (currentAudioFileSource);
            
            // create a new file source from the file..
            // get a format manager and set it up with the basic types (wav, ogg and aiff).
            AudioFormatManager formatManager;
            formatManager.registerBasicFormats();
            
            AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
            
            if (reader != 0)
            {
                currentFile = audioFile;
                currentAudioFileSource = new AudioFormatReaderSource (reader, true);
                
                //set read ahead buffer size
                int bufferSize = currentAudioFileSource->getTotalLength()/2;
                //restrict buffer size value
                if (bufferSize > 48000)
                    bufferSize = 48000;
                else if (bufferSize < 8000)
                    bufferSize = 8000;
                
                
                // ..and plug it into our transport source
                fileSource.setSource (currentAudioFileSource,
                                      bufferSize, // tells it to buffer this many samples ahead
                                      audioTransportSourceThread,
                                      reader->sampleRate);
                
                
            }
        }
    }

}

void AudioFilePlayer::triggerQuantizationPoint()
{
    if (currentPlayingState == 2) //waiting to play
    {
        playAudioFile();
        currentPlayingState = 1;
    }
    else if (currentPlayingState == 3) //waiting to stop
    {
        stopAudioFile();
        //currentPlayingState = 0; //now done within stopAudioFile()
    }
}


void AudioFilePlayer::playAudioFile()
{
    //This is now in processAudioFile() - too much too much CPU usage?
    //if (currentFile != File::nonexistent && currentAudioFileSource != NULL)
        //currentAudioFileSource->setLooping(shouldLoop);
    
    //set the state of certain effects
    if (effect == 9) //flanger
        flanger->restart(); //so the lfo is re-started when the file is started
    else if (effect == 10) //tremolo
        tremolo->restart(); //so the lfo is re-started when the file is started
    
    //start audio file
    fileSource.setPosition (0.0);
    fileSource.start();
    
    /*
    AudioSampleBuffer buffer(2, 512);
    AudioSourceChannelInfo info;
    info.buffer = &buffer;
    info.startSample = 0;
    info.numSamples = 512;
    
    fileSource.getNextAudioBlock(info);
    
    for (int i = 0; i < 2; ++i)
    {
        info.buffer->applyGainRamp (i,
                                    info.startSample,
                                    info.numSamples,
                                    0,
                                    gain);
    }
     */
     
    
    if (PAD_SETTINGS->getExclusiveMode() == 1)
    {
        modeSamplerRef.getAlphaLiveEngineRef().handleExclusiveMode(padNumber);
    }
    
    
    //update pad layout gui
    if (currentPlayingState != 1 || shouldFinishLoop == true) //if currentlyPlayingStatus doesn't already equal 1
    {
        broadcaster.sendActionMessage("PLAYING");
        currentPlayingState = 1;
    }
    


}

void AudioFilePlayer::stopAudioFile()
{
    
    fileSource.stop();
    
    playingLastLoop = false;
    broadcaster.sendActionMessage("OFF");
    
    currentPlayingState = 0;
    //triggerModes.reset(); //can't do this hear as this will cause instant retriggering
                            //of files if stopAudioFile() is called from a pad press.
    
}

void AudioFilePlayer::actionListenerCallback (const String& message)
{
    if (message == "WAITING TO PLAY")
        modeSamplerRef.updatePadPlayingStatus(padNumber, 2);
    
    else if (message == "WAITING TO STOP")
        modeSamplerRef.updatePadPlayingStatus(padNumber, 3);
    
    else if (message == "PLAYING")
        modeSamplerRef.updatePadPlayingStatus(padNumber, 1);
    
    else if (message == "OFF")
        modeSamplerRef.updatePadPlayingStatus(padNumber, 0);
    
}


void AudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // DO AS LITTLE IN HERE AS POSSIBLE. DON'T ATTEMPT TO CALL TO MANY FUNCTIONS
    //do not call anything from AppSettings in here. Only call member variables of this class,
    //otherwise it is to CPU heavy!
    
    fileSource.getNextAudioBlock(bufferToFill);
    
    //how can I set it so that effects will only be applied when playing, but without causing clicks at the start and the end of sound? Need something like a look ahead and release time.
    sharedMemory.enter();
    if (/*fileSource.isPlaying() == true &&*/ effect != 0)
    {
        
        switch (effect) 
        {
            case 1: //Gain and Pan
                gainAndPan->processAudio(bufferToFill);
                break;
            case 2: //LPF
                lowPassFilter->processAudio(bufferToFill);
                break;
            case 3://HPF
                highPassFilter->processAudio(bufferToFill);
                break;
            case 4: //BPF
                bandPassFilter->processAudio(bufferToFill);
                break;
            case 7: //Delay
                delay->processAudio(bufferToFill);
                break;
            case 8: //Reverb
                reverb->processAudio(bufferToFill);
                break;
            case 9: //Flanger
                flanger->processAudio(bufferToFill);
                break;
            case 10: //Tremolo
                tremolo->processAudio(bufferToFill);
                break;
            default:
                break;
        }
         
    }
    sharedMemory.exit();
     
    //gain and pan
    sharedMemory.enter();
    
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
        if (i == 0) //left chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                panLeftPrev * gainPrev,
                                                panLeft * gain);
        else if (i == 1) // right chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                panRightPrev * gainPrev,
                                                panRight * gain);
    }
    
    gainPrev = gain;
    panLeftPrev = panLeft;
    panRightPrev = panRight;
    sharedMemory.exit();
     
     
    if (fileSource.hasStreamFinished() == true) //if the audio file has ended on its own, automatically update the pad GUI
        //NOTE - hasStreamFinshed could be used above where I've been manually checking if the stream has ended on its own
    {
        //can't directly call modeSamplerRef.updatePadPlayingStatus(padNumber, false) to update the pad GUI as
        //we are currently in ther audio thread here and calling it causes some form of objective C leak. Therefore we
        //are using a Async action broadcaster so that it doesn't cause any time-critical pauses in the thread which i think
        broadcaster.sendActionMessage("OFF");
        
        playingLastLoop = false;
    }

}

void AudioFilePlayer::prepareToPlay (int samplesPerBlockExpected,double sampleRate)
{
    fileSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    sampleRate_ = sampleRate;
    
    
    switch (effect)
    {
        case 1:
            gainAndPan->setSampleRate(sampleRate);
            break;
        case 2:
            lowPassFilter->setSampleRate(sampleRate);
            break;
        case 3:
            highPassFilter->setSampleRate(sampleRate);
            break;
        case 4:
            bandPassFilter->setSampleRate(sampleRate);
            break;
        case 7:
            delay->setSampleRate(sampleRate);
            break;
        case 8:
            reverb->setSampleRate(sampleRate);
            break;
        case 9:
            flanger->setSampleRate(sampleRate);
            break;
        case 10:
            tremolo->setSampleRate(sampleRate);
            break;
        default:
            break;
    }
  
        
}
void AudioFilePlayer::releaseResources()
{
    fileSource.releaseResources();
}


void AudioFilePlayer::killAllAudio()
{
    if (effect == 7) //delay
        delay->resetBuffers();
    else if (effect == 9) //flanger
        flanger->resetBuffers();
    
}

void AudioFilePlayer::resetTriggerMode()
{
    //called from killPad(). Can't be called within stopAudioFile as this will cause instant retriggering
    //of files if stopAudioFile() is called from a pad press.
    triggerModes.reset();
}

//=========================================================================================

int AudioFilePlayer::getCurrentPlayingState()
{
    return currentPlayingState;
}
void AudioFilePlayer::setTriggerMode (int value)
{
    triggerMode = value;
}
void AudioFilePlayer::setEffect(int value)
{
    sharedMemory.enter();
    
    if (effect != value) //if the effect is being changed
    {
        //first, delete the current effect...
        switch (effect)
        {
            case 1:
                delete gainAndPan;
                gainAndPan = nullptr;
                break;
            case 2:
                delete lowPassFilter;
                lowPassFilter = nullptr;
                break;
            case 3:
                delete highPassFilter;
                highPassFilter = nullptr;
                break;
            case 4:
                delete bandPassFilter;
                bandPassFilter = nullptr;
                break;
            case 7:
                delete delay;
                delay = nullptr;
                break;
            case 8:
                delete reverb;
                reverb = nullptr;
                break;
            case 9:
                delete flanger;
                flanger = nullptr;
                break;
            case 10:
                delete tremolo;
                tremolo = nullptr;
                break;
            default:
                break;
        }
    
        //Next, create the new effect...
        switch (value)
        {
            case 1:
                gainAndPan = new GainAndPan (padNumber, sampleRate_);
                break;
            case 2:
                lowPassFilter = new LowpassFilter (padNumber, sampleRate_);
                break;
            case 3:
                highPassFilter = new HighPassFilter (padNumber, sampleRate_);
                break;
            case 4:
                bandPassFilter = new BandPassFilter (padNumber, sampleRate_);
                break;
            case 7:
                delay = new Delay (padNumber, sampleRate_);
                break;
            case 8:
                reverb = new ReverbClass (padNumber, sampleRate_);
                break;
            case 9:
                flanger = new Flanger (padNumber, sampleRate_);
                break;
            case 10:
                tremolo = new Tremolo (padNumber, sampleRate_);
                break;
            default:
                break;
        }
    
        //Finally, set the new effect to be the current effect
        effect = value;
    }
    sharedMemory.exit();

}



void AudioFilePlayer::setQuantizeMode (int value)
{
    quantizeMode = value;
}

void AudioFilePlayer::setGain (float value)
{
    sharedMemory.enter();
    
    if (value <= 1.0)
        gain = value * value * value;
    else
        gain = value;
    
    sharedMemory.exit();
}
void AudioFilePlayer::setPan (float value)
{
    sharedMemory.enter();
    panLeft = PanControl::leftChanPan_(value);
    panRight = PanControl::rightChanPan_(value);
    sharedMemory.exit();
}

void AudioFilePlayer::setShouldLoop (int value)
{
    shouldLoop = value;
}
void AudioFilePlayer::setIndestructible (int value)
{
    indestructible = value;
}
void AudioFilePlayer::setShouldFinishLoop (int value)
{
    shouldFinishLoop = value;
}

void AudioFilePlayer::setSticky (int value)
{
    sticky = value;
}

//========================================================================================
GainAndPan& AudioFilePlayer::getGainAndPan()
{
    return *gainAndPan;
}
LowpassFilter& AudioFilePlayer::getLowpassFilter()
{
    return *lowPassFilter;
}

HighPassFilter& AudioFilePlayer::getHighPassFilter()
{
    return *highPassFilter;
}

BandPassFilter& AudioFilePlayer::getBandPassFilter()
{
    return *bandPassFilter;
}

Delay& AudioFilePlayer::getDelay()
{
    return *delay;
}

ReverbClass& AudioFilePlayer::getReverb()
{
    return *reverb;
}

Flanger& AudioFilePlayer::getFlanger()
{
    return *flanger;
}

Tremolo& AudioFilePlayer::getTremolo()
{
    return *tremolo;
}

