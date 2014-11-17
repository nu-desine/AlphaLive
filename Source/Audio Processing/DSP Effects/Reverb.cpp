//
//  Reverb.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 21/12/2011.
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

#include "Reverb.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

ReverbClass::ReverbClass(int padNumber_, float sampleRate_)
                        : padNumber(padNumber_)
{
    mix = mixPrev = mixControlValue = PAD_SETTINGS->getPadFxReverbMix();
    params.roomSize = roomSizeControl = PAD_SETTINGS->getPadFxReverbRoomSize();
    params.damping = dampingControl =  PAD_SETTINGS->getPadFxReverbDamping();
    params.wetLevel = 1.0; //static - mix is controlled by my own algorithm
    params.dryLevel = 0; //same as above...
    params.width = widthControl =  PAD_SETTINGS->getPadFxReverbWidth();
    params.freezeMode = freezeModeControl =  PAD_SETTINGS->getPadFxReverbFreezeMode();
    
    alphaTouchParam = PAD_SETTINGS->getPadFxReverbAlphaTouch();
    alphaTouchReverse = PAD_SETTINGS->getPadFxReverbAtReverse();
    alphaTouchIntensity = PAD_SETTINGS->getPadFxReverbAtIntensity();
    
    reverb.setSampleRate(sampleRate_);
}

ReverbClass::~ReverbClass()
{
    
}

//Function that processes the audio
void ReverbClass::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    //create a COPY of bufferToFill's buffer that will be used to hold processed audio data
    //bufferToFill.buffer will stay unprocessed
    int numChans = bufferToFill.buffer->getNumChannels();
    int numSamps = bufferToFill.numSamples;
    
    AudioSampleBuffer wetBuffer(numChans, numSamps);
    
    for (int i = 0; i < numChans; i++)
        wetBuffer.copyFrom(i, 0, *bufferToFill.buffer, 1, 0, numSamps);
    
    //get audio channel data
    float *channels[2];
    channels[0] = wetBuffer.getArrayOfWritePointers()[0] + bufferToFill.startSample;
    channels[1] = wetBuffer.getArrayOfWritePointers()[1] + bufferToFill.startSample;
    
    //process reverb
    sharedMemory.enter();
    reverb.setParameters(params);
    reverb.processStereo(channels[0], channels[1], wetBuffer.getNumSamples());
    sharedMemory.exit();
    
    //set the relative gains of the processed and unprocessed buffers using the 'mix' value
    sharedMemory.enter();
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
        bufferToFill.buffer->applyGainRamp (i,
                                            bufferToFill.startSample,
                                            bufferToFill.numSamples,
                                            1.0 - mixPrev,
                                            1.0 - mix);
        wetBuffer.applyGainRamp (i,
                                 0,
                                 wetBuffer.getNumSamples(),
                                 mixPrev,
                                 mix);
        
    }
    
    mixPrev = mix;
    
    //add the processed buffer to the unprcocessed buffer to create wet/dry mix
    bufferToFill.buffer->addFrom(0, 0, wetBuffer, 0, 0, wetBuffer.getNumSamples());
    if (bufferToFill.buffer->getNumChannels() == 2) 
        bufferToFill.buffer->addFrom(1, 0, wetBuffer, 1, 0, wetBuffer.getNumSamples());
    
    sharedMemory.exit();

    
}

//Function that takes in a pressure value and modulates the reverb depending on the set AlphaTouch parameter
void ReverbClass::processAlphaTouch (int pressureValue)
{
    sharedMemory.enter();
    switch (alphaTouchParam) 
    {
        case 2: //Mix
            if (alphaTouchReverse == false)
                mix = mixControlValue + (pressureValue * (((1.0-mixControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                mix = mixControlValue - (pressureValue * (((1.0-(1.0-mixControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << mix << std::endl;
            break;
            
        case 3: //Room Size
            if (alphaTouchReverse == false)
                params.roomSize = roomSizeControl + (pressureValue * (((1.0-roomSizeControl)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                params.roomSize = roomSizeControl - (pressureValue * (((1.0-(1.0-roomSizeControl))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << params.roomSize << std::endl;
            break;
            
        case 4: //Damping
            if (alphaTouchReverse == false)
                params.damping = dampingControl + (pressureValue * (((1.0-dampingControl)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                params.damping = dampingControl - (pressureValue * (((1.0-(1.0-dampingControl))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << params.damping << std::endl;
            break;
            
        case 5: //Width
            if (alphaTouchReverse == false)
                params.width = widthControl + (pressureValue * (((1.0-widthControl)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                params.width = widthControl - (pressureValue * (((1.0-(1.0-widthControl))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << params.width << std::endl;
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
}

void ReverbClass::setMix (float value)
{
    sharedMemory.enter();
    mix = mixControlValue = value * value * value;
    sharedMemory.exit();
    
}
void ReverbClass::setRoomSize (float value)
{
    sharedMemory.enter();
    params.roomSize = roomSizeControl = value;
    sharedMemory.exit();
}
void ReverbClass::setDamping (float value)
{
    sharedMemory.enter();
    params.damping =  dampingControl = value;
    sharedMemory.exit();
}
void ReverbClass::setWidth (float value)
{
    sharedMemory.enter();
    params.width = widthControl = value;
    sharedMemory.exit();
}
void ReverbClass::setFreezeMode (float value)
{
    sharedMemory.enter();
    params.freezeMode =  freezeModeControl = value;
    sharedMemory.exit();
}

void ReverbClass::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}
void ReverbClass::setAlphaTouchIntensity (float value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}
void ReverbClass::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse= value;
    sharedMemory.exit();
}

void ReverbClass::setSampleRate (float value)
{
    reverb.setSampleRate(value);
}
