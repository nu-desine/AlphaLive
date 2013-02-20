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
	distortion = nullptr;
	bitcrusher = nullptr;
    
    //grab the setting values (so that if this object is deleted and recreated, it will hold the previous settings)
    //do i need to enter shared memory here?
    
    gain = staticGain = PAD_SETTINGS->getSamplerGain(); //should this be cubed?
    panLeft = PanControl::leftChanPan_(PAD_SETTINGS->getSamplerPan());
    panRight = PanControl::rightChanPan_(PAD_SETTINGS->getSamplerPan());
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
    attackTime = PAD_SETTINGS->getSamplerAttackTime();
    releaseTime = PAD_SETTINGS->getSamplerReleaseTime();
    
    triggerModeData.playingStatus = 0;
    prevPadValue = pressureValue =  0;
    playingLastLoop = false;
    
    attackSamples = attackTime * sampleRate_;
    releaseSamples = releaseTime * sampleRate_;
    isInAttack = isInRelease = false;
    attackPosition = releasePosition = 0;
    attRelGainL = attRelGainR = prevGainL = prevGainR = 0;
    velocity = 127;
    
    //call this here incase a loop has been 'dropped' onto a pad before this AudioFilePlayer instance actually exists,
    //which means that it wouldn't have been called from setSamplerAudioFilePath().
    setAudioFile(PAD_SETTINGS->getSamplerAudioFilePath());
    
    columnNumber = sequenceNumber = 0;
    
    
    
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
	delete distortion;
	delete bitcrusher;
    
    {
        const MessageManagerLock mmLock; //lock event thread so it is safe to make calls in the message thread
        modeSamplerRef.updatePadPlayingStatus(padNumber, 0);
    }
    
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

void AudioFilePlayer::processAudioFile(int padValue, int padVelocity)
{
    if (currentFile != File::nonexistent && currentAudioFileSource != NULL)
    {
        //this is needed incase audio ends on its own or is stopped via the 'exclusive mode' feature, in order to reset everything so it will trigger again properly
        if ((fileSource.isPlaying() == false && currentPlayingState == 1 && prevPadValue == 0) ||
            (isInRelease == true && currentPlayingState == 1 && prevPadValue == 0))
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
            //case 3:
                //triggerModeData = triggerModes.toggleRelease(padValue);
                //break;
            case 3:
                triggerModeData = triggerModes.latch(padValue);
                break;
            //case 5:
             //   triggerModeData = triggerModes.latchMax(padValue);
             //   break;
            case 4:
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
            else if (triggerModeData.playingStatus == 1 && currentPlayingState == 1 && triggerMode != /*6*/4)
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
        
        //set velocity value...
        if (triggerModeData.playingStatus == 1) //play
        {
            //should I be using local variables below instead of accessing PAD_SETTINGS each time?
            //Though as I'm only doing this if statement when the pad is first pressed it probably
            //won't be too CPU extensive here.
            
            velocity = padVelocity;
            
            if (PAD_SETTINGS->getVelocityCurve() != 4)
            {
                // not static velocity
                gain = staticGain * (velocity*(1.0/127.0));
            }
            else
            {
                //static velocity - just use Sampler mode gain value
                gain = staticGain;
                velocity = 110; //this should actually be equal to some value based on staticGain
            }
                
        }
        
        
        
        if (quantizeMode == 0) //free
        {
            if (triggerModeData.playingStatus == 1) //play
            {
                playAudioFile();
            }
            
            else if (triggerModeData.playingStatus == 0) //stop
            {
                stopAudioFile(false);
                currentPlayingState = 0;    //needs to be done here and not within stopAudioFile,
                                            //incase stopAudioFile is called from stopPrevExclusivePad,
                                            //where the triggerModeData won't be reset.
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
			case 5: //Distortion
				distortion->processAlphaTouch(pressureValue);
				break;
			case 6: //Bitcrusher
				bitcrusher->processAlphaTouch(pressureValue);
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
        //currentPlayingState = 1;
    }
    else if (currentPlayingState == 3) //waiting to stop
    {
        stopAudioFile(false);
        currentPlayingState = 0;    //needs to be done here and not within stopAudioFile,
                                    //incase stopAudioFile is called from stopPrevExclusivePad,
                                    //where the triggerModeData won't be reset.
    }
}


void AudioFilePlayer::playAudioFile()
{
    //set the state of certain effects
    if (effect == 9) //flanger
        flanger->restart(); //so the lfo is re-started when the file is started
    else if (effect == 10) //tremolo
        tremolo->restart(); //so the lfo is re-started when the file is started
    
    if (attackTime > 0)
    {
        attackPosition = 0;
        isInAttack = true;
    }
    
    //start audio file
    fileSource.setPosition (0.0);
    fileSource.start();
    
    
    //NEW - recording into sequencer pads
    if (modeSamplerRef.getAlphaLiveEngineRef().getRecordingPads().size() > 0)
    {
        for (int i = 0; i < modeSamplerRef.getAlphaLiveEngineRef().getRecordingPads().size(); i++)
        {
            int recordingPad = modeSamplerRef.getAlphaLiveEngineRef().getRecordingPads()[i];
            
            if (modeSamplerRef.getAlphaLiveEngineRef().getModeSequencer()->getSequencePlayerInstance(recordingPad)->isThreadRunning()) //if currently playing
            {
                if (AppSettings::Instance()->padSettings[recordingPad]->getSequencerMode() == 2) //if samples mode
                {
                    File seqFile[NO_OF_ROWS];
                    
                    for (int j = 0; j < NO_OF_ROWS; j++)
                    {
                        seqFile[j] = AppSettings::Instance()->padSettings[recordingPad]->getSequencerSamplesAudioFilePath(j);
                        
                        if (currentFile == seqFile[j]) //if audio file path matches
                        {
                            sequenceNumber = modeSamplerRef.getAlphaLiveEngineRef().getModeSequencer()->getSequencePlayerInstance(recordingPad)->getSequenceNumber();
                            
                            //get the closest column number
                            Array <int> columnNumberData = modeSamplerRef.getAlphaLiveEngineRef().getModeSequencer()->getSequencePlayerInstance(recordingPad)->getClosestColumnNumber();
                            columnNumber = columnNumberData[0];
                            int columnNumberType = columnNumberData[1];
                            
                            //When recording a note to a sequencer pad the note will play twice at this point - 
                            //from the played pad (this class) and from the recorded note in the sequence, which is note what we want.
                            //We want the new note to be played from this class only, and not the seq notes which will all be played at the same
                            //set length, amoung a few other unwanted behaviours.
                            //This is done using a new variable within SequencerPlayer called recentlyAddedSequenceData.
                            //Every time a note is recorded here it adds a 'true' to the array in the same location as the recorded note.
                            //Then in SequencerPlayer it won't play this new note due to this 'true' flag.
                            
                            if (columnNumberType == 1) //if the closest number is the current column number, add it to the recentAddedSequenceData Array so it isn't played
                                modeSamplerRef.getAlphaLiveEngineRef().getModeSequencer()->getSequencePlayerInstance(recordingPad)->setRecentlyAddedSequenceData(sequenceNumber, j, columnNumber, true);
                            
                            AppSettings::Instance()->padSettings[recordingPad]->setSequencerData(sequenceNumber, j, columnNumber, velocity);
                            
                            
                            //if currently selected pad is the recording pad, update the grid gui.
                            //how should it be handled if multiple pads are selected? Do nothing?
                            if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1)
                            {
                                if (AppSettings::Instance()->getCurrentlySelectedPad()[0] == recordingPad)
                                {
                                    broadcaster.sendActionMessage("RECORD NOTE");
                                }
                            }
                            
                        }
                    }
                }
            }
        }
    }

     
    
    if (PAD_SETTINGS->getExclusiveMode() == 1)
    {
        modeSamplerRef.getAlphaLiveEngineRef().handleExclusiveMode(padNumber);
    }
    
    
    //update pad layout gui
    if (currentPlayingState != 1 || shouldFinishLoop == true || isInRelease == true) //if currentlyPlayingStatus doesn't already equal 1
    {
        broadcaster.sendActionMessage("PLAYING");
        currentPlayingState = 1;
    }
    
    isInRelease = false;

}

void AudioFilePlayer::stopAudioFile (bool shouldStopInstantly)
{
    // shouldStopInstantly will be true when called from killPads()
    
    if (releaseTime > 0 && shouldStopInstantly == false && fileSource.isPlaying() == true)
    {
        if (isInRelease == false)   //to prevent cases where a pad may be in release state but then
                                    //another pad tries to stop the pad (hence, start the release again)
                                    //via exclusive mode. It would probably make more sense to remove
                                    //a pad from the exclusive array when called to stop.
        {
            if (attackTime == 0)
            {
                //if there was no attack, the below variables will not be set correctly
                //whic will cause an incorrect release.
                attRelGainL = attRelGainR = 1.0;
            }
            
            releasePosition = 0;
            isInRelease = true;
            
            broadcaster.sendActionMessage("WAITING TO STOP");
        }
    }
    else
    {
        fileSource.stop();
    
        playingLastLoop = false;
        broadcaster.sendActionMessage("OFF");
    }
    
      isInAttack = false;
    
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
    
    else if (message == "RECORD NOTE")
    {
        //optimise the below so we're only calling/updating what needs to be done!
        //first, update the display of the sequence grid which gets the stored
        //sequence data from PadSettings and puts it into the local sequenceData. This
        //could be optimised so that it is only getting the data from the current seq,
        //as thats all that will be changed here.
        modeSamplerRef.getAlphaLiveEngineRef().getModeSequencer()->updateSequencerGridGui (columnNumber, sequenceNumber, 3);
        //next set the currently sequence display, which sets the status's of the grid points
        modeSamplerRef.getAlphaLiveEngineRef().getModeSequencer()->updateSequencerGridGui (columnNumber, sequenceNumber, 2);

    }
    
    else if (message == "STOP AFTER RELEASE")
    {
        fileSource.stop();
        playingLastLoop = false;
        modeSamplerRef.updatePadPlayingStatus(padNumber, 0);
    
        isInRelease = false;
    }
    
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
			case 5: //Distortion
				distortion->processAudio(bufferToFill);
				break;
			case 6: //Bitcrusher
				bitcrusher->processAudio(bufferToFill);
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
    
    
    if (isInAttack == true || isInRelease == true)
    {
        //====== set attack and release =======
        
        sharedMemory.enter();
        
        //get first pair of sample data from audio buffer
        float *pOutL = bufferToFill.buffer->getSampleData (0, bufferToFill.startSample);
        float *pOutR = bufferToFill.buffer->getSampleData (1, bufferToFill.startSample);
        
        //increment through each pair of samples (left channel and right channel) in the current block of the audio buffer
        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            if (isInAttack)
            {
                //ramp up to 1.0
                attRelGainL = attackPosition * (1.0/attackSamples);
                attRelGainR = attackPosition * (1.0/attackSamples);
                
                if (attRelGainL <= 1)
                    *pOutL = *pOutL * (attRelGainL * attRelGainL * attRelGainL);
                else
                    *pOutL = *pOutL * attRelGainL;
                
                if (attRelGainR <= 1)
                    *pOutR = *pOutR * (attRelGainR * attRelGainR * attRelGainR);
                else
                    *pOutR = *pOutR * attRelGainR;
                
                attackPosition++;
                
                //move to next pair of samples
                pOutL++;
                pOutR++;
                
                if (attackPosition >= attackSamples)
                {
                    isInAttack = false;
                }
            }
            else if (isInRelease)
            {
                //ramp down from current gains
                
                //one thing that could be improved here is if the release is triggered before
                //the attack is finished, the release time should be shorter - should be
                //as long as the length of the attack until it was disturbed.
                //this will involve finding out how many samples the attack at left to process,
                //and then shortening the release time/samples by that much.
                
                double relGainL = attRelGainL - (releasePosition * (attRelGainL/releaseSamples));
                double relGainR = attRelGainR - (releasePosition * (attRelGainR/releaseSamples));
                
                if (relGainL >= 0)
                {
                    if (relGainL <= 1)
                        *pOutL = *pOutL * (relGainL * relGainL * relGainL);
                    else
                        *pOutL = *pOutL * relGainL;
                }
                if (relGainR >= 0)
                {
                    if (relGainR <= 1)
                        *pOutR = *pOutR * (relGainR * relGainR * relGainR);
                    else
                        *pOutR = *pOutR * relGainR;
                }
                
                releasePosition++;
                
                //move to next pair of samples
                pOutL++;
                pOutR++;
                
                if (releasePosition == releaseSamples-1)
                {
                    //set gains to 0.
                    //This needs to be done to avoid a click after the release.
                    //I think this is due the samples within the next call to
                    //getNextAudioBlock being processed 'incorrectly' due to the stop audio file command
                    //happening asyncronosly. Not entirely sure, but this way works.
                    attRelGainL = attRelGainR = 0;
                    
                    //stop file and stuff
                    broadcaster.sendActionMessage("STOP AFTER RELEASE");
                }
            }
            else
            {
                break;
            }
            
        }
        
        sharedMemory.exit();
    }

    
    //===== set gain and pan======
    sharedMemory.enter();
  
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
        if (i == 0) //left chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                prevGainL,
                                                panLeft * gain);
        else if (i == 1) // right chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                prevGainR,
                                                panRight * gain);
    }
    
    prevGainL = panLeft * gain;
    prevGainR = panRight * gain;
    
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
    attackSamples = attackTime * sampleRate_;
    releaseSamples = releaseTime * sampleRate_;
    
    
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
		case 5:
			distortion->setSampleRate(sampleRate);
			break;
		case 6:
			bitcrusher->setSampleRate(sampleRate);
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
bool AudioFilePlayer::isCurrentlyPlaying()
{
    return fileSource.isPlaying();
}

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
			case 5:
				delete distortion;
				distortion = nullptr;
				break;
			case 6:
				delete bitcrusher;
				bitcrusher = nullptr;
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
			case 5:
                distortion = new Distortion (padNumber, sampleRate_);
                break;
			case 6:
                bitcrusher = new Bitcrusher (padNumber, sampleRate_);
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
        staticGain = value * value * value;
    else
        staticGain = value;
    
    //set gain value here in case the sample is currently playing
    if (PAD_SETTINGS->getVelocityCurve() != 4)
    {
        // not static velocity
        gain = staticGain * (velocity*(1.0/127.0));
    }
    else
    {
        gain = staticGain;
    }
    
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

void AudioFilePlayer::setAttackTime (double value)
{
    //do we actually needs the attackTime variable? Won't attackSamples be enough here?
    sharedMemory.enter();
    attackTime = value;
    attackSamples = attackTime * sampleRate_;
    sharedMemory.exit();
}

void AudioFilePlayer::setReleaseTime (double value)
{
    //do we actually needs the releaseTime variable? Won't releaseSamples be enough here?
    sharedMemory.enter();
    releaseTime = value;
    releaseSamples = releaseTime * sampleRate_;
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

Distortion& AudioFilePlayer::getDistortion()
{
	return *distortion;
}

Bitcrusher& AudioFilePlayer::getBitcrusher()
{
	return *bitcrusher;
}
