//
//  SequenceAudioFilePlayer.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 19/10/2011.
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

#include "SequenceAudioFilePlayer.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]
#define START_RAMP_LENGTH 100.0

SequenceAudioFilePlayer::SequenceAudioFilePlayer(int padNumber_, int rowNumber_, TimeSliceThread *audioTransportSourceThread_)
                                                :   padNumber(padNumber_),
                                                    rowNumber(rowNumber_)
{
    audioTransportSourceThread = audioTransportSourceThread_;
    
    currentAudioFileSource = NULL;
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
    
    currentFile = File::nonexistent;
    setPolyphony(PAD_SETTINGS->getSequencerSamplesPolyphony());
    setAudioFile(PAD_SETTINGS->getSequencerSamplesAudioFilePath(rowNumber));
    attackTime = PAD_SETTINGS->getSequencerSamplesAttackTime();
    
    attackSamples = attackTime * sampleRate_;
    isInAttack = isInStartRamp = false;
    attackPosition = startRampPosition = 0;
    
    nextFileSourceIndex = 0;
    
}

SequenceAudioFilePlayer::~SequenceAudioFilePlayer()
{
    for (int i = 0; i < polyphony; i++)
        fileSource[i]->setSource(0);//unload the current file
    
    audioMixer.removeAllInputs();
    fileSource.clear(true);
    
	deleteAndZero(currentAudioFileSource);//delete the current file
    
}



//called from either the constructor, or setSequencerSamplesAudioFilePath() in PadSettings
void SequenceAudioFilePlayer::setAudioFile (File audioFile_)
{
    if (audioFile_ != File::nonexistent)
    {
        //passes in pads audio file
        File audioFile (audioFile_);
        
        //if the audio file is different from the previous one, stop and load in the new file
        if (audioFile != currentFile)
        {
            for (int i = 0; i < polyphony; i++)
            {
                fileSource[i]->stop();
                fileSource[i]->setPosition(0.0);
                fileSource[i]->setSource (0);
            }
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
                
                //add the audio file to the fileSource array
                for (int i = 0; i < polyphony; i++)
                {
                    addtoFileSourceArray(i, reader);
                }
                
            }
        } 
    }
}

void SequenceAudioFilePlayer::addtoFileSourceArray (int arrayIndex, AudioFormatReader* reader_)
{
    //set read ahead buffer size
    int bufferSize = currentAudioFileSource->getTotalLength()/2;
    //restrict buffer size value
    if (bufferSize > 48000)
        bufferSize = 48000;
    else if (bufferSize < 8000)
        bufferSize = 8000;
    
    // ..and plug it into our transport source
    fileSource[arrayIndex]->setSource (currentAudioFileSource,
                                       bufferSize, // tells it to buffer this many samples ahead
                                       audioTransportSourceThread,
                                       reader_->sampleRate);
}


void SequenceAudioFilePlayer::playAudioFile (float gain)
{
    if (attackTime > 0)
    {
        isInAttack = true;
        attackPosition = 0;
    }
    else
    {
        isInStartRamp = true;
        startRampPosition = 0;
    }
    
    if (nextFileSourceIndex >= polyphony)
        nextFileSourceIndex = 0;
    
    fileSource[nextFileSourceIndex]->setGain(gain); //ideal the gain should be mapped logorythmitically (?)
    //there is a bug here in that when the gain is changed to a low value, it doesn't seem to change in time
    //most likely this is a juce bug that would have been fixed in the latest version?
    
    fileSource[nextFileSourceIndex]->setPosition (0.0);
    fileSource[nextFileSourceIndex]->start();
    
    //iterate to next index of the FileSource array
    nextFileSourceIndex++;
}


void SequenceAudioFilePlayer::stopAudioFile()
{
    for (int i = 0; i < polyphony; i++)
        fileSource[i]->stop();
    
    nextFileSourceIndex = 0;
}



void SequenceAudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    audioMixer.getNextAudioBlock(bufferToFill);
    
    if (isInAttack == true)
    {
        //====== set attack and release =======
        
        sharedMemory.enter();
        
        //get first pair of sample data from audio buffer
        float *pOutL = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        float *pOutR = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
        
        //increment through each pair of samples (left channel and right channel) in the current block of the audio buffer
        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            if (isInAttack)
            {
                //ramp up to 1.0
                
                double currentGainL = attackPosition * (1.0/attackSamples);
                double currentGainR = attackPosition * (1.0/attackSamples);
                
                if (currentGainL <= 1)
                    *pOutL = *pOutL * (currentGainL * currentGainL * currentGainL);
                else
                    *pOutL = *pOutL * currentGainL;
                
                if (currentGainR <= 1)
                    *pOutR = *pOutR * (currentGainR * currentGainR * currentGainR);
                else
                    *pOutR = *pOutR * currentGainR;
                
                attackPosition++;
                
                //move to next pair of samples
                pOutL++;
                pOutR++;
                
                if (attackPosition >= attackSamples)
                {
                    isInAttack = false;
                }
            }
            else
            {
                break;
            }
        }
        
        sharedMemory.exit();
    }
    
    else if (isInStartRamp)
    {
        //==== apply very short gain ramp from zero to prevent potential click at sample start ====
        
        sharedMemory.enter();
        
        //get first pair of sample data from audio buffer
        float *pOutL = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        float *pOutR = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
        
        //increment through each pair of samples (left channel and right channel) in the current block of the audio buffer
        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            //ramp up to 1.0 over START_RAMP_LENGTH samples...
            
            //get a gain value based on the position in the ramp
            float gain_ = startRampPosition * (1.0/START_RAMP_LENGTH);
            
            //apply the gain to the current samples
            if (gain_ <= 1.0)
            {
                *pOutL = *pOutL * gain_;
                *pOutR = *pOutR * gain_;
            }
            
            //increment position
            startRampPosition++;
            
            //move to next pair of samples
            pOutL++;
            pOutR++;
            
            if (startRampPosition >= START_RAMP_LENGTH)
            {
                isInStartRamp = false;
            }
        }
    
        sharedMemory.exit();
    }
}

void SequenceAudioFilePlayer::prepareToPlay (int samplesPerBlockExpected,double sampleRate)
{
    audioMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    sampleRate_ = sampleRate;
    attackSamples = attackTime * sampleRate_;
}

void SequenceAudioFilePlayer::releaseResources()
{
    audioMixer.releaseResources();
}


void SequenceAudioFilePlayer::setAttackTime (double value)
{
    attackTime = value;
    attackSamples = attackTime * sampleRate_;
}

void SequenceAudioFilePlayer::setPolyphony (int value)
{
    int arraySize = fileSource.size();
    
    if (value > arraySize)
    {
        AudioFormatManager formatManager;
        formatManager.registerBasicFormats();
        ScopedPointer <AudioFormatReader> reader (formatManager.createReaderFor (currentFile));
        
        for (int i = 0; i < (value - arraySize); i++)
        {
            //add element
            fileSource.add (new AudioTransportSource());
            //add new element as an input source to audioMixer
            audioMixer.addInputSource(fileSource.getLast(), false);
            
            //apply audio file to new element
            if (currentFile != File::nonexistent)
            {
                addtoFileSourceArray (fileSource.size()-1, reader);
            }
        }
        
        //set AFTER elements have been created to prevent
        //any potential crashes.
        polyphony = value;
    }
    else if (value < arraySize)
    {
        //set BEFORE elements have been deleted to prevent
        //any potential crashes.
        polyphony = value;
        
        for (int i = 0; i < (arraySize - value); i++)
        {
            //remove elements
            audioMixer.removeInputSource(fileSource.getLast());
            fileSource.removeLast();
        }
    }
    
}

