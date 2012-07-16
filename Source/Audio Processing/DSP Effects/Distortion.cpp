//
//  Distortion.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 22/12/2011.
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

#include "Distortion.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

DistortionClass::DistortionClass(int padNumber_, float sampleRate_)
: padNumber(padNumber_)
{
    //===Pre-Filter===
    //filter
    preFilter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::BandPass2, 2> (1024);
    // sample rate
    paramsPreFilter[0] = sampleRate_;
    // cutoff frequency
    paramsPreFilter[1] = frequencyControlPreFilter = 300.0;
    // Q/Bandwidth (static)
    paramsPreFilter[2] = 5.0;
    
    //===Post-Filter===
    //filter
    postFilter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2> (1024);
    // sample rate
    paramsPostFilter[0] = sampleRate_;
    // cutoff frequency
    paramsPostFilter[1] = frequencyControlPreFilter = 1000.0;
    // Q/Bandwidth (static)
    paramsPostFilter[2] = 5.0;
    
    //Overdrive
    preGain = preGainControl = 2.0;
    
    //wet/dry mix ration
    mix = mixControl = 1.0;

    alphaTouchParam = 1;
    alphaTouchReverse = 0;
    alphaTouchIntensity = 1.0;
    
    postGain = 0.2;
}

DistortionClass::~DistortionClass()
{
	delete preFilter;
    delete postFilter;
}

//==============================================================================


void DistortionClass::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    //create a COPY of bufferToFill's buffer that will be used to hold processed audio data
    //bufferToFill.buffer will stay unprocessed
    AudioSampleBuffer wetBuffer(*bufferToFill.buffer);
    
    //===apply pre-filter===
    sharedMemory.enter();
    preFilter->setParams (paramsPreFilter);
    preFilter->process (wetBuffer.getNumSamples(), wetBuffer.getArrayOfChannels());
    sharedMemory.exit();
    
    //===apply pre-gain===
    sharedMemory.enter();
    wetBuffer.applyGain(0, wetBuffer.getNumSamples(), preGain);
    sharedMemory.exit();
    
    //===apply waveshaping function===
    //...
    //
    
    //===apply post-filter===
    sharedMemory.enter();
    postFilter->setParams(paramsPostFilter);
    postFilter->process(wetBuffer.getNumSamples(), wetBuffer.getArrayOfChannels());
    sharedMemory.exit();
    
    //==apply post-gain===
    sharedMemory.enter();
    wetBuffer.applyGain(0, wetBuffer.getNumSamples(), postGain);
    sharedMemory.exit();
    
    
    //set the relative gains of the processed and unprocessed buffers using the 'mix' value
    sharedMemory.enter();
    bufferToFill.buffer->applyGain(0, bufferToFill.buffer->getNumSamples(), 1-mix);
    wetBuffer.applyGain(0, wetBuffer.getNumSamples(), mix);
    
    //add the processed buffer to the unprcocessed buffer to create wet/dry mix
    bufferToFill.buffer->addFrom(0, 0, wetBuffer, 0, 0, wetBuffer.getNumSamples());
    if (bufferToFill.buffer->getNumChannels() == 2) 
        bufferToFill.buffer->addFrom(1, 0, wetBuffer, 1, 0, wetBuffer.getNumSamples());
    
    sharedMemory.exit();
}


void DistortionClass::processAlphaTouch (int pressureValue)
{
    /*
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
                params[2] = bandwidthControlValue + (pressureValue * (((16.0-bandwidthControlValue)*alphaTouchIntensity)/511.0));
            else
                params[2] = bandwidthControlValue - (pressureValue * (((16.0-(16.0 - bandwidthControlValue))*alphaTouchIntensity)/511.0));
            
            std::cout << params[2] << std::endl;
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
     */
}


void DistortionClass::setMix (float value)
{
    sharedMemory.enter();
    mix = mixControl = value;
    sharedMemory.exit();
}

void DistortionClass::setTone1 (float value)
{
    sharedMemory.enter();
    paramsPreFilter[1] = frequencyControlPreFilter = value;
    sharedMemory.exit();
}

void DistortionClass::setTone2 (float value)
{
    sharedMemory.enter();
    paramsPostFilter[1] = frequencyControlPostFilter = value;
    sharedMemory.exit();
}

void DistortionClass::setOverdrive (float value)
{
    sharedMemory.enter();
    preGain = preGainControl = value;
    sharedMemory.exit();
}


void DistortionClass::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void DistortionClass::setAlphaTouchIntensity (float value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void DistortionClass::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void DistortionClass::setSampleRate(float value)
{
    sharedMemory.enter();
    paramsPreFilter[0] = paramsPostFilter[0] = value;
    sharedMemory.exit();
}