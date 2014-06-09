/*
 *  BandPassFilter.cpp
 *  AlphaLive
 *
 *  Created by Liam Lacey on 19/12/2011.
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

#include "BandPassFilter.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

BandPassFilter::BandPassFilter(int padNumber_, float sampleRate_)
: padNumber(padNumber_)
{
    filter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::BandPass1, 2> (1024);
    
    // sample rate
    params[0] = sampleRate_;
    // cutoff frequency
    params[1] = frequencyControlValue = PAD_SETTINGS->getPadFxBpfFreq();
    // Q/Bandwidth
    params[2] = bandwidthControlValue = PAD_SETTINGS->getPadFxBpfBandwidth();
    
    mix = mixPrev = mixControlValue =  PAD_SETTINGS->getPadFxBpfMix();
    
    alphaTouchParam = PAD_SETTINGS->getPadFxBpfAlphaTouch();
    alphaTouchReverse = PAD_SETTINGS->getPadFxBpfAtReverse();
    alphaTouchIntensity = PAD_SETTINGS->getPadFxBpfAtIntensity();
    
}

BandPassFilter::~BandPassFilter()
{
	delete filter;
}

//==============================================================================


void BandPassFilter::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    //create a COPY of bufferToFill's buffer that will be used to hold processed audio data
    //bufferToFill.buffer will stay unprocessed
    int numChans = bufferToFill.buffer->getNumChannels();
    int numSamps = bufferToFill.numSamples;
    
    AudioSampleBuffer wetBuffer(numChans, numSamps);
    
    for (int i = 0; i < numChans; i++)
        wetBuffer.copyFrom(i, 0, *bufferToFill.buffer, 1, 0, numSamps);
    
    //process filtering
    sharedMemory.enter();
    filter->setParams (params);
    filter->process (wetBuffer.getNumSamples(), wetBuffer.getArrayOfWritePointers());
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


void BandPassFilter::processAlphaTouch (int pressureValue)
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
            
        case 3: //Frequency
            if (alphaTouchReverse == false)
                params[1] = frequencyControlValue + (pressureValue * (((20000.0-frequencyControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                params[1] = frequencyControlValue - (pressureValue * (((19970.0-(20000.0 - frequencyControlValue))*alphaTouchIntensity)/MAX_PRESSURE)); 
            
            //std::cout << params[1] << std::endl;
            break;
            
        case 4: //Bandwidth
            if (alphaTouchReverse == false)
                params[2] = bandwidthControlValue + (pressureValue * (((100.0-bandwidthControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                params[2] = bandwidthControlValue - (pressureValue * (((99.0-(100.0 - bandwidthControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << params[2] << std::endl;
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
}


void BandPassFilter::setMix (double value)
{
    sharedMemory.enter();
    mix = mixControlValue = value * value * value;
    sharedMemory.exit();
}

void BandPassFilter::setFrequency (double value)
{
    sharedMemory.enter();
    params[1] = frequencyControlValue = value;
    sharedMemory.exit();
}

void BandPassFilter::setBandwidth(double value)
{
    sharedMemory.enter();
    params[2] = bandwidthControlValue = value;
    sharedMemory.exit();
}



void BandPassFilter::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void BandPassFilter::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void BandPassFilter::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void BandPassFilter::setSampleRate(float value)
{
    sharedMemory.enter();
    params[0] = value;
    sharedMemory.exit();
}