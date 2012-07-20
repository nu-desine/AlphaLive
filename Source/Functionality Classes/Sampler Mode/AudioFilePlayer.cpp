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
#include "ModeLooper.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]


AudioFilePlayer::AudioFilePlayer(int looperPadNumber, ModeLooper &ref, TimeSliceThread* audioTransportSourceThread_)
                                    :   padNumber(looperPadNumber),
                                        modeLooperRef(ref)
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
    gain = gainPrev = PAD_SETTINGS->getLooperGain();
    panLeft = panLeftPrev = PanControl::leftChanPan_(PAD_SETTINGS->getLooperPan());
    panRight = panRightPrev = PanControl::rightChanPan_(PAD_SETTINGS->getLooperPan());
    playState = PAD_SETTINGS->getLooperPlayState();
    currentPlayingState = currentPressureValue = 0;
    effect = PAD_SETTINGS->getLooperEffect();
    quantizeMode = PAD_SETTINGS->getLooperQuantizeMode();
    channel = PAD_SETTINGS->getLooperChannel();
    
    playStateData.playingStatus = 0;
    playStateData.pressureValue = 0;
    playStateData.shouldLoop = true;
    
    prevPadValue = 0;
    
    /*
    //test playback manipulation stuff
    prevPressureRegion = 0;
    currentPositionInRegion = 0;
    fileStartPosition = 0;
    fileEndPosition = 0;
     */
    
    
    //call this here incase a loop has been 'dropped' onto a pad before this AudioFilePlayer instance actually exists,
    //which means that it wouldn't have been called from setLooperAudioFilePath().
    setAudioFile(PAD_SETTINGS->getLooperAudioFilePath());
    
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
    
    modeLooperRef.updatePadPlayingStatus(padNumber, 0);
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
    //this is needed incase audio ends on its own or is stopped via the 'exclusive mode' feature, in order to reset everything so it will trigger again properly
    if (fileSource.isPlaying() == false && currentPlayingState == 1 && prevPadValue == 0)
    {
        std::cout << "stream ended on its own!!";
        currentPlayingState = 0;
        playStates.reset();
    }
   
    //determime playstate and audio file transportation
    switch (playState) 
    {
        case 1:
            playStateData = playStates.playOnce(padValue);
            break;
        case 2:
            playStateData = playStates.playOnceContinuous(padValue);
            break;
        case 3:
            playStateData = playStates.loop(padValue);
            break;
        case 4:
            playStateData = playStates.togglePressOff(padValue);
            break;
        case 5:
            playStateData = playStates.toggleReleaseOff(padValue);
            break;
        case 6:
            playStateData = playStates.sticky(padValue);
            break;
        case 7:
            playStateData = playStates.latchMaxLatch(padValue);
            break;
        case 8:
            playStateData = playStates.latchPressLatch(padValue);
            break;
        case 9:
            playStateData = playStates.triggerLooped(padValue);
            break;
        case 10:
            playStateData = playStates.triggerNonLooped(padValue);
            break;
        case 21:
            playStateData = playStates.playOnceNonDestructive(padValue);
            break;
        case 22:
            playStateData = playStates.triggerLoopedNonDestructive(padValue);
            break;
        case 24:
            playStateData = playStates.triggerNonLoopedNonDestructive(padValue);
            break;
        case 25:
            playStateData = playStates.toggleNonDestructive(padValue);
            break;
        default:
            playStateData = playStates.loop(padValue);
            break;
    }
    
    
    //==========================================================================================
    if (quantizeMode == 1) //free
    {
        if (playStateData.playingStatus == 1) //play
        {
            playAudioFile();
        }
        
        else if (playStateData.playingStatus == 0) //stop
        {
            stopAudioFile();
            currentPlayingState = 0;
        }
    }
    //==========================================================================================
    else if (quantizeMode == 2) //quantized
    {
        if (playStateData.playingStatus == 1) //play
        {
            currentPlayingState = 2; //waiting to play
            
            //add this instance of AudioFilePlayer to the waitingPadLooper Array within
            //ModeLooper so that it is alerted of the next quantized point in time so
            //the loop can start playing
            modeLooperRef.addItemToWaitingPadLooper(this);
            broadcaster.sendActionMessage("WAITING TO PLAY");
            
            
        }
        
        else if (playStateData.playingStatus == 0) //stop
        {
            currentPlayingState = 3; // waiting to stop
            
            //add this instance of AudioFilePlayer to the waitingPadLooper Array within
            //ModeLooper so that it is alerted of the next quantized point in time so
            //the loop can stop playing
            modeLooperRef.addItemToWaitingPadLooper(this);
            broadcaster.sendActionMessage("WAITING TO STOP");
        }
    }
    //==========================================================================================
    
    
    prevPadValue = padValue;
    
    /*
    //playback manipulation test stuff
    int pressureRegion = 1 + (playStateData.pressureValue*(3.0/511.0));
    std::cout << pressureRegion << std::endl;
    
    if (pressureRegion != prevPressureRegion)
    {
        switch (pressureRegion)
        {
            case 1:
                fileStartPosition = 0.0;
                fileEndPosition = (fileSource.getLengthInSeconds()/4.0)*1.0;
                fileSource.setPosition(fileStartPosition + currentPositionInRegion);
                break;
            case 2:
                fileStartPosition = (fileSource.getLengthInSeconds()/4.0)*1.0;
                fileEndPosition = (fileSource.getLengthInSeconds()/4.0)*2.0;
                fileSource.setPosition(fileStartPosition + currentPositionInRegion);
                break;
            case 3:
                fileStartPosition = (fileSource.getLengthInSeconds()/4.0)*2.0;
                fileEndPosition = (fileSource.getLengthInSeconds()/4.0)*3.0;
                fileSource.setPosition(fileStartPosition + currentPositionInRegion);
                break;
            case 4:
                fileStartPosition = (fileSource.getLengthInSeconds()/4.0)*3.0;
                fileEndPosition = fileSource.getLengthInSeconds();
                fileSource.setPosition(fileStartPosition + currentPositionInRegion);
                break;
            default:
                break;
        }
    }
    
    prevPressureRegion = pressureRegion;
     */
    
    //determine what effect and parameter the pressure is controlling
    switch (effect)
    {
        case 1: //Gain and Pan
            gainAndPan->processAlphaTouch(playStateData.pressureValue);
            break;
        case 2: //LPF
            lowPassFilter->processAlphaTouch(playStateData.pressureValue);
            break;
        case 3: //HPF
            highPassFilter->processAlphaTouch(playStateData.pressureValue);
            break;
        case 4: //BPF
            bandPassFilter->processAlphaTouch(playStateData.pressureValue);
            break;
        case 6: //Delay
            delay->processAlphaTouch(playStateData.pressureValue);
            break;
        case 7: //Reverb
            reverb->processAlphaTouch(playStateData.pressureValue);
            break;
        case 9: //Flanger
            flanger->processAlphaTouch(playStateData.pressureValue);
            break;
        case 10: //Tremolo
            tremolo->processAlphaTouch(playStateData.pressureValue);
            break;
        default:
            break;
    }
    
  
    
}

//called from either the constructor, or setLooperAudioFilePath() in PadSettings
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
        currentPlayingState = 0;
    }
}


void AudioFilePlayer::playAudioFile()
{
    //set to whether the audio should loop, set from the playstate functions
    if (currentFile != File::nonexistent && currentAudioFileSource != NULL)
        currentAudioFileSource->setLooping(playStateData.shouldLoop);
    
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
     
    
    if (channel != 1) //if channel equals something above 1, it is in 'exclusive' mode
    {
        //stop currently playing loop of this channel and add this instance of AudioFilePlayer 
        //to the correct index (channel number-1) of currentExclusivePadLooper array within ModeLooper
        modeLooperRef.stopExclusivePadLooper(channel, this);
    }
    
    //update pad layout gui
    if (currentPlayingState != 1) //if currentlyPlayingStatus doesn't already equal 1
    {
        broadcaster.sendActionMessage("PLAYING");
        currentPlayingState = 1;
    }
    


}

void AudioFilePlayer::stopAudioFile()
{
    
    fileSource.stop();
    //don't need to setPosition to 0 after stopping AND before playing. Where's best to do it though?
    //fileSource.setPosition (0.0);
    
    broadcaster.sendActionMessage("OFF");
    
}

void AudioFilePlayer::actionListenerCallback (const String& message)
{
    if (message == "WAITING TO PLAY")
        modeLooperRef.updatePadPlayingStatus(padNumber, 2);
    
    else if (message == "WAITING TO STOP")
        modeLooperRef.updatePadPlayingStatus(padNumber, 3);
    
    else if (message == "PLAYING")
        modeLooperRef.updatePadPlayingStatus(padNumber, 1);
    
    else if (message == "OFF")
        modeLooperRef.updatePadPlayingStatus(padNumber, 0);
    
}

void AudioFilePlayer::setPlaybackPosition()
{
    //test playback manipulation stuff
    
    //ideally stuff in here should be in it's own class
    /*
    if (fileSource.getCurrentPosition() >= fileSource.getLengthInSeconds()/32)
    {
        double difference = fileSource.getCurrentPosition() - fileSource.getLengthInSeconds()/32;
        fileSource.setPosition(0.0 + difference);
    }
     */
    
    if (fileSource.getCurrentPosition() >= fileEndPosition)
    {
        double difference = fileSource.getCurrentPosition() - fileEndPosition;
        fileSource.setPosition(fileStartPosition + difference);
    }
    
    /*
    if (fileSource.getCurrentPosition() < fileStartPosition)
    {
        //double difference = fileSource.getCurrentPosition() - fileEndPosition;
        fileSource.setPosition(fileStartPosition);
    }
     */
    
    currentPositionInRegion = fileSource.getCurrentPosition() - fileStartPosition;
    //std::cout << currentPositionInRegion << std::endl;
    
}


void AudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // DO AS LITTLE IN HERE AS POSSIBLE. DON'T ATTEMPT TO CALL TO MANY FUNCTIONS
    //do not call anything from AppSettings in here. Only call member variables of this class,
    //otherwise it is to CPU heavy!
    
    fileSource.getNextAudioBlock(bufferToFill);
    
    /*
    sharedMemory.enter();
    setPlaybackPosition();
    sharedMemory.exit();
     */
    
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
            case 6: //Delay
                delay->processAudio(bufferToFill);
                break;
            case 7: //Reverb
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
    
    //OLD METHOD OF SETTING GAIN AND PAN - created clicks/pops
    //when changing the vales and playing audio and the same time
    /*
    //get first pair of sample data from audio buffer
    float *pOutL = bufferToFill.buffer->getSampleData (0, bufferToFill.startSample);
    float *pOutR = bufferToFill.buffer->getSampleData (1, bufferToFill.startSample);
    
    sharedMemory.enter();
    //increment through each pair of samples (left channel and right channel) in the current block of the audio buffer
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        //pan
        *pOutL = panControl.leftChanPan(*pOutL) * gainNext;
        *pOutR = panControl.rightChanPan(*pOutR) * gainNext;
        
        //move to next pair of samples
        pOutL++;
        pOutR++;
    }
    sharedMemory.exit();
     */
     
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
        //can't directly call modeLooperRef.updatePadPlayingStatus(padNumber, false) to update the pad GUI as
        //we are currently in ther audio thread here and calling it causes some form of objective C leak. Therefore we
        //are using a Async action broadcaster so that it doesn't cause any time-critical pauses in the thread which i think
        broadcaster.sendActionMessage("OFF");
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
        case 6:
            delay->setSampleRate(sampleRate);
            break;
        case 7:
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
    if (effect == 6) //delay
        delay->resetBuffers();
    else if (effect == 9) //tremolo
        flanger->resetBuffers();
    
}

//=========================================================================================

int AudioFilePlayer::getCurrentPlayingState()
{
    return currentPlayingState;
}
void AudioFilePlayer::setPlayState (int value)
{
    playState = value;
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
            case 6:
                delete delay;
                delay = nullptr;
                break;
            case 7:
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
            case 6:
                delay = new Delay (padNumber, sampleRate_);
                break;
            case 7:
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
void AudioFilePlayer::setChannel (int value)
{
    channel = value;
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

