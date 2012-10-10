//
//  Bitcrusher.cpp
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

#include "Bitcrusher.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

Bitcrusher::Bitcrusher(int padNumber_, float sampleRate_)
:   padNumber(padNumber_)
{
	//===Smoothing-Filter===
    //filter
    smoothingFilter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2> (1024);
    // sample rate
    paramsSmoothingFilter[0] = sampleRate_;
    // centre frequency
    paramsSmoothingFilter[1] = PAD_SETTINGS->getPadFxBitcrusherSmoothing() * 10000;
    // Q/Bandwidth (static)
    paramsSmoothingFilter[2] = 2.0;
	
	//input gain
    inputGain = inputGainPrev = inputGainControl = PAD_SETTINGS->getPadFxBitcrusherInputGain();
	
	//crush value
	crush = crushControlValue = PAD_SETTINGS->getPadFxBitcrusherCrush();
	
	downsample = downsampleControl = PAD_SETTINGS->getPadFxBitcrusherDownsample();
	
    //wet/dry mix ratio
    wetDryMixPrev = wetDryMix = wetDryMixControl = PAD_SETTINGS->getPadFxBitcrusherWetDryMix();
    
    alphaTouchParam = PAD_SETTINGS->getPadFxBitcrusherAlphaTouch();
    alphaTouchReverse = PAD_SETTINGS->getPadFxBitcrusherAtReverse();
    alphaTouchIntensity = PAD_SETTINGS->getPadFxBitcrusherAtIntensity();

    sampleL = 0.0;
    sampleR = 0.0;
    counter = downsample;
}

Bitcrusher::~Bitcrusher()
{
    delete smoothingFilter;
}

//==============================================================================


void Bitcrusher::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    //==============================================================================
    //======================AUDIO PROCESSING CODE GOES IN HERE======================
    //bufferToFill.buffer is the audio buffer you will want to use.
    //Use the effectParam array of floats as the variables within the DSP code that you want
    //controllable by the GUI. The slider numbers match the index numbers of the array
    //e.g. Slider 0 = effectParam[0], Slider 1 = effectParam[1] and so on.
    //==============================================================================
    
	sharedMemory.enter();
    //get audio channel data
	pIn[0] = bufferToFill.buffer->getArrayOfChannels()[0] + bufferToFill.startSample;
	pIn[1] = bufferToFill.buffer->getArrayOfChannels()[1] + bufferToFill.startSample;
    
	AudioSampleBuffer dryBuffer(pIn, bufferToFill.buffer->getNumChannels(),bufferToFill.numSamples);
	AudioSampleBuffer wetBuffer(bufferToFill.buffer->getNumChannels(),bufferToFill.numSamples);
	sharedMemory.exit();
	
	wetBuffer.copyFrom(0, 0, dryBuffer, 0, 0, wetBuffer.getNumSamples());
	wetBuffer.copyFrom(1, 0, dryBuffer, 1, 0, wetBuffer.getNumSamples());
	
	sharedMemory.enter();
    //===apply pre-gain===
	for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
	{
		wetBuffer.applyGainRamp(i, 0, wetBuffer.getNumSamples(), inputGain, inputGainPrev);	
	}	
	
	sharedMemory.exit();
	inputGainPrev = inputGain;
	
	leftChannel = rightChannel = 0;
	
	//link wetBuffer to wet pointer variables
	leftChannel = wetBuffer.getSampleData(0); 
	rightChannel = wetBuffer.getSampleData(1); 
	
	sharedMemory.enter();
    //increment through each pair of samples (left channel and right channel) in the current block of the audio buffer
    for (int i = 0; i < wetBuffer.getNumSamples(); ++i)
    {		
		if (--counter == 0) 
		{
			sampleL = *leftChannel;
			sampleR = *rightChannel;
			
			counter = downsample;
		}
		
		int sL = sampleL * 32768, sR = sampleR * 32768, index = 16 - crush;
		
		sL = sL >> index;
		sR = sR >> index;
		sL = sL << index;
		sR = sR << index;
		
		*leftChannel = float(sL) / 32768.0;
		*rightChannel = float(sR) / 32768.0;
		
		*leftChannel *= 0.6;
		*rightChannel *= 0.6;
		
		//move to next pair of samples
		leftChannel++;
		rightChannel++;
    }
	sharedMemory.exit();
	
	sharedMemory.enter();
    //===apply smoothing-filter1===
    smoothingFilter->setParams(paramsSmoothingFilter);
    smoothingFilter->process(wetBuffer.getNumSamples(), wetBuffer.getArrayOfChannels());
	sharedMemory.exit();
	
    //set the relative gains of the processed and unprocessed buffers using the 'mix' value
	dryBuffer.applyGainRamp(0, 0, bufferToFill.buffer->getNumSamples(), (1.0-wetDryMix), (1.0-wetDryMixPrev));
	dryBuffer.applyGainRamp(1, 0, bufferToFill.buffer->getNumSamples(), (1.0-wetDryMix), (1.0-wetDryMixPrev));
	
	wetBuffer.applyGainRamp(0, 0, wetBuffer.getNumSamples(), wetDryMix, wetDryMixPrev);
	wetBuffer.applyGainRamp(1, 0, wetBuffer.getNumSamples(), wetDryMix, wetDryMixPrev);
	
	wetDryMixPrev = wetDryMix;
	
	sharedMemory.enter();
	//add the processed buffer to the unprcocessed buffer to create wet/dry mix
	bufferToFill.buffer->addFrom(0, 0, wetBuffer, 0, 0, wetBuffer.getNumSamples());
    bufferToFill.buffer->addFrom(1, 0, wetBuffer, 1, 0, wetBuffer.getNumSamples());
    sharedMemory.exit();
}


void Bitcrusher::processAlphaTouch (int pressureValue)
{
    sharedMemory.enter();
    switch (alphaTouchParam) 
    {
		case 2: //Input gain
            if (alphaTouchReverse == false)
            {
                inputGain = inputGainControl + (pressureValue * (((1.0-inputGainControl)*alphaTouchIntensity)/511.0));
			}
            else
            {
                inputGain = inputGainControl - (pressureValue * (((1.0-(1.0-inputGainControl))*alphaTouchIntensity)/511.0));
			}
            
            inputGain = inputGain * inputGain * inputGain;
			
			std::cout << inputGain << std::endl;
			break;
			
		case 3: //downsample
            if (alphaTouchReverse == false)
            {
                downsample = roundToInt (downsampleControl + (pressureValue * (((16.0 - downsampleControl)*alphaTouchIntensity)/511.0)));
            }
			else
            {
                downsample = roundToInt (downsampleControl - (pressureValue * (((15.0-(16.0-downsampleControl))*alphaTouchIntensity)/511.0)));
            }
            
			std::cout << downsample << std::endl;
			break;
			
        case 4: //crush
            if (alphaTouchReverse == false)
            {
                crush = roundToInt (crushControlValue + (pressureValue * (((8.0 - crushControlValue)*alphaTouchIntensity)/511.0)));
            }
			else
            {
                crush = roundToInt (crushControlValue - (pressureValue * (((7.0 - (8.0 - crushControlValue))*alphaTouchIntensity)/511.0)));
            }
            
			std::cout << crush << std::endl;
			
			break;
			
		case 5: //Smoothing
            if (alphaTouchReverse == false)
            {
                paramsSmoothingFilter[1] = 10000 * (smoothingControl + (pressureValue * (((1.0-smoothingControl)*alphaTouchIntensity)/511.0)));
            }
			else
                paramsSmoothingFilter[1] = 10000 * (smoothingControl - (pressureValue * (((0.9-(1.0-smoothingControl))*alphaTouchIntensity)/511.0)));
           
			std::cout << paramsSmoothingFilter[1] << std::endl;
			break;
			
        case 6: //Mix
            if (alphaTouchReverse == false)
            {
                wetDryMix = wetDryMixControl + (pressureValue * (((1.0-wetDryMixControl)*alphaTouchIntensity)/511.0));
            }
			else
            {
                wetDryMix = wetDryMixControl - (pressureValue * (((1.0-(1.0-wetDryMixControl))*alphaTouchIntensity)/511.0));
			}
			
            wetDryMix = wetDryMix * wetDryMix * wetDryMix;
            
			std::cout << wetDryMix << std::endl;
			break;

        default:
            break;
    }
    sharedMemory.exit();
	
}

void Bitcrusher::setInputGain (double value)
{
	std::cout << value << std::endl;
	value = value*value*value;
	
    sharedMemory.enter();
    inputGain = inputGainControl = value;
    sharedMemory.exit();
}

void Bitcrusher::setCrush(int value)
{
	std::cout << value << std::endl;
	
	sharedMemory.enter();
    crush = crushControlValue = value;
    sharedMemory.exit();
}

void Bitcrusher::setDownsample(int value)
{
	std::cout << value << std::endl;
	
	sharedMemory.enter();
	downsample = downsampleControl = value;
	sharedMemory.exit();
}

void Bitcrusher::setMix (double value)
{
	std::cout << value << std::endl;
	value = value*value*value;
	
    sharedMemory.enter();
    wetDryMix = wetDryMixPrev = wetDryMixControl = value;
    sharedMemory.exit();
}

void Bitcrusher::setSmoothing (double value)
{
	std::cout << value << std::endl;
	value = value*value*value;
	
    sharedMemory.enter();
	paramsSmoothingFilter[1] = value * 10000;
    sharedMemory.exit();
}

void Bitcrusher::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void Bitcrusher::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void Bitcrusher::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void Bitcrusher::setSampleRate(float value)
{
    sharedMemory.enter();
    paramsSmoothingFilter[0] = value;
    sharedMemory.exit();
}
