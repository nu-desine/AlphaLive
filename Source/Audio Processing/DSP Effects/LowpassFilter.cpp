/*
 *  LowpassFilter.cpp
 *  AlphaLive
 *
 *  Created by Liam Lacey on 15/12/2011.
 *  Copyright 2010 nu desine.
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
 *
 */

#include "LowpassFilter.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

LowpassFilter::LowpassFilter(int padNumber_, float sampleRate_)
                            : padNumber(padNumber_)
{
    filter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2> (1024);
    
    // sample rate
    params[0] = sampleRate_;
    // cutoff frequency
    params[1] = frequencyControlValue = PAD_SETTINGS->getPadFxLpfFreq();
    // Q/Bandwidth
    params[2] = bandwidthControlValue = PAD_SETTINGS->getPadFxLpfBandwidth();
    
    mix = mixPrev = mixControlValue =  PAD_SETTINGS->getPadFxLpfMix();
    
    alphaTouchParam = PAD_SETTINGS->getPadFxLpfAlphaTouch();
    alphaTouchReverse = PAD_SETTINGS->getPadFxLpfAtReverse();
    alphaTouchIntensity = PAD_SETTINGS->getPadFxLpfAtIntensity();

}

LowpassFilter::~LowpassFilter()
{
	delete filter;
}

//==============================================================================


void LowpassFilter::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    /*
    //get audio channel data
    float *channels[2];
    channels[0] = bufferToFill.buffer->getArrayOfChannels()[0] + bufferToFill.startSample;
    channels[1] = bufferToFill.buffer->getArrayOfChannels()[1] + bufferToFill.startSample;
    //create audio buffers
    AudioSampleBuffer wetBuffer (channels, bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
     */
    
    //create a COPY of bufferToFill's buffer that will be used to hold processed audio data
    //bufferToFill.buffer will stay unprocessed
    AudioSampleBuffer wetBuffer(*bufferToFill.buffer);
    
    //process filtering
    sharedMemory.enter();
    filter->setParams (params);
    filter->process (wetBuffer.getNumSamples(), wetBuffer.getArrayOfChannels());
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


void LowpassFilter::processAlphaTouch (int pressureValue)
{
    sharedMemory.enter();
    switch (alphaTouchParam) 
    {
        case 2: //Mix
            if (alphaTouchReverse == false)
                mix = mixControlValue + (pressureValue * (((1.0-mixControlValue)*alphaTouchIntensity)/511.0));
            else
                mix = mixControlValue - (pressureValue * (((1.0-(1.0-mixControlValue))*alphaTouchIntensity)/511.0));
            
            std::cout << mix << std::endl;
            break;
        
        case 3: //Frequency
            if (alphaTouchReverse == false)
                params[1] = frequencyControlValue + (pressureValue * (((20000.0-frequencyControlValue)*alphaTouchIntensity)/511.0));
            else
               params[1] = frequencyControlValue - (pressureValue * (((19970.0-(20000.0 - frequencyControlValue))*alphaTouchIntensity)/511.0)); 
            
            std::cout << params[1] << std::endl;
            break;
        
        case 4: //Bandwidth
            if (alphaTouchReverse == false)
                params[2] = bandwidthControlValue + (pressureValue * (((100.0-bandwidthControlValue)*alphaTouchIntensity)/511.0));
            else
                params[2] = bandwidthControlValue - (pressureValue * (((99.0-(100.0 - bandwidthControlValue))*alphaTouchIntensity)/511.0));
            
            std::cout << params[2] << std::endl;
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
}


void LowpassFilter::setMix (float value)
{
    sharedMemory.enter();
    mix = mixControlValue = value * value * value;
    sharedMemory.exit();
}

void LowpassFilter::setFrequency (float value)
{
    sharedMemory.enter();
    params[1] = frequencyControlValue = value;
    sharedMemory.exit();
}

void LowpassFilter::setBandwidth(float value)
{
    sharedMemory.enter();
    params[2] = bandwidthControlValue = value;
    sharedMemory.exit();
}



void LowpassFilter::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void LowpassFilter::setAlphaTouchIntensity (float value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void LowpassFilter::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void LowpassFilter::setSampleRate(float value)
{
    sharedMemory.enter();
    params[0] = value;
    sharedMemory.exit();
}