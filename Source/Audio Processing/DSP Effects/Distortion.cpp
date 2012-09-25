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

Distortion::Distortion(int padNumber_, float sampleRate_)
: padNumber(padNumber_)
{
    //===Pre-Filter1===
    //filter
    preFilter1 = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::HighPass, 2> (1024);
    // sample rate
    paramsPreFilter1[0] = sampleRate_;
    // cutoff frequency
    paramsPreFilter1[1] = 250.0;
    // Q/Bandwidth 
    paramsPreFilter1[2] = 2.0;
	
	//===Pre-Filter2===
    //filter
    preFilter2 = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2> (1024);
    // sample rate
    paramsPreFilter2[0] = sampleRate_;
    // cutoff frequency
    paramsPreFilter2[1] = 600.0;
    // Q/Bandwidth 
    paramsPreFilter2[2] = 2.0;
	
    //===Post-Filter===
    //filter
    postFilter1 = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::BandPass2, 2> (1024);
    // sample rate
    paramsPostFilter1[0] = sampleRate_;
    // centre frequency
    paramsPostFilter1[1] = 1000.0;
    // Q/Bandwidth (static)
    paramsPostFilter1[2] = 3.0;
    
	//===Post-Filter===
    //filter
    postFilter2 = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::BandPass2, 2> (1024);
    // sample rate
    paramsPostFilter2[0] = sampleRate_;
    // centre frequency
    paramsPostFilter2[1] = 1000.0;
    // Q/Bandwidth (static)
    paramsPostFilter2[2] = 3.0;
    
    
    //input gain
    inputGain = inputGainPrev = inputGainControl = PAD_SETTINGS->getPadFxDistortionInputGain();
	
	//drive value
	drive = PAD_SETTINGS->getPadFxDistortionDrive();
	
	//output gain
   	outputGainPrev = outputGain = PAD_SETTINGS->getPadFxDistortionOutputGain();
	
    //wet/dry mix ratio
    wetDryMixPrev = wetDryMix = wetDryMixControl = PAD_SETTINGS->getPadFxDistortionMix();
    
	postFilter1Cutoff = (PAD_SETTINGS->getPadFxDistortionTone() * 5000) + 250;
	postFilter2Cutoff = (PAD_SETTINGS->getPadFxDistortionTone() * 5000) + 50;

    alphaTouchParam = PAD_SETTINGS->getPadFxDistortionAlphaTouch();
    alphaTouchReverse = PAD_SETTINGS->getPadFxDistortionAtReverse();
    alphaTouchIntensity = PAD_SETTINGS->getPadFxDistortionAtIntensity();
	
	comboBoxID = PAD_SETTINGS->getPadFxDistortionTypeMenu();
}

Distortion::~Distortion()
{
	delete preFilter1;
	delete preFilter2;
    delete postFilter1;
	delete postFilter2;
}

//==============================================================================


void Distortion::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    //==============================================================================
    //======================AUDIO PROCESSING CODE GOES IN HERE======================
    //bufferToFill.buffer is the audio buffer you will want to use.
    //Use the effectParam array of floats as the variables within the DSP code that you want
    //controllable by the GUI. The slider numbers match the index numbers of the array
    //e.g. Slider 0 = effectParam[0], Slider 1 = effectParam[1] and so on.
    //==============================================================================
    
    //get audio channel data
	pIn[0] = bufferToFill.buffer->getArrayOfChannels()[0] + bufferToFill.startSample;
	pIn[1] = bufferToFill.buffer->getArrayOfChannels()[1] + bufferToFill.startSample;
    
	AudioSampleBuffer dryBuffer(pIn, bufferToFill.buffer->getNumChannels(),bufferToFill.numSamples);
	AudioSampleBuffer wetBuffer(bufferToFill.buffer->getNumChannels(),bufferToFill.numSamples);
	AudioSampleBuffer wetBufferSplit(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
	
	wetBuffer.copyFrom(0, 0, dryBuffer, 0, 0, wetBuffer.getNumSamples());
	wetBuffer.copyFrom(1, 0, dryBuffer, 1, 0, wetBuffer.getNumSamples());
	
    //===apply pre-filter1 (Hipass)===
    sharedMemory.enter();
    paramsPreFilter1[1] = preFilter1Cutoff; //pre filter freq
    preFilter1->setParams (paramsPreFilter1);
    preFilter1->process (wetBuffer.getNumSamples(), wetBuffer.getArrayOfChannels());
    sharedMemory.exit();
    
	//===apply pre-filter2 (Lowpass)===
	sharedMemory.enter();
    paramsPreFilter2[1] = preFilter2Cutoff; //pre filter freq
    preFilter2->setParams (paramsPreFilter2);
    preFilter2->process (wetBuffer.getNumSamples(), wetBuffer.getArrayOfChannels());
    sharedMemory.exit();
    
	
    //===apply pre-gain===
	sharedMemory.enter();
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
		
		//SOFT
		if (comboBoxID == 1) 
		{
			*leftChannel *= drive;
			*rightChannel *= drive;
			
			
			*leftChannel = *leftChannel / (fabsf(*leftChannel) + .7f);
			*rightChannel = *rightChannel / (fabsf(*rightChannel) + .7f);
		}
		
		//HARD
		if (comboBoxID == 2) 
		{
			*leftChannel *= drive;
			*rightChannel *= drive;
			
			*leftChannel = (*leftChannel * fabs(*leftChannel) + *leftChannel) / (*leftChannel * *leftChannel + 1.f);
			*rightChannel = (*rightChannel * fabs(*rightChannel) + *rightChannel) / (*rightChannel * *rightChannel + 1.f);
		}
		
		//HF0
		if (comboBoxID == 3) 
		{
			*leftChannel *= drive;
			*rightChannel *= drive;
			
			*leftChannel = *leftChannel / (*leftChannel * *leftChannel + .7f);
			*rightChannel = *rightChannel / (*rightChannel * *rightChannel + .7f);
		}
		
		//HF1
		if (comboBoxID == 4) 
		{
			*leftChannel *= drive;
			*rightChannel *= drive;
			
			*leftChannel = (*leftChannel - *leftChannel * fabsf(*leftChannel)) / (*leftChannel * *leftChannel + .6f);
			*rightChannel = (*rightChannel - *rightChannel * fabsf(*rightChannel)) / (*rightChannel * *rightChannel + .6f);
		}
		
		//HF2
		if (comboBoxID == 5) 
		{
			*leftChannel *= drive;
			*rightChannel *= drive;
			
			*leftChannel = (*leftChannel - *leftChannel * fabsf(*leftChannel)) / (*leftChannel * *leftChannel * fabsf(*leftChannel) + .6f);
			*rightChannel = (*rightChannel - *rightChannel * fabsf(*rightChannel)) / (*rightChannel * *rightChannel * fabsf(*rightChannel) + .6f);
		}
		
		//DIGITAL
		if (comboBoxID == 9) 
		{
			*leftChannel *= drive;
			*rightChannel *= drive;
			
			*leftChannel = fast_clamp_mul2(*leftChannel , -.5f, .5f);
			*rightChannel = fast_clamp_mul2( *rightChannel , -.5f, .5f);
		}
		
		*leftChannel *= 1.5;
		*rightChannel *= 1.5;
		
		//move to next pair of samples
		leftChannel++;
		rightChannel++;
    }
    sharedMemory.exit();
	
	wetBufferSplit.copyFrom(0, 0, wetBuffer, 0, 0, wetBuffer.getNumSamples());
	wetBufferSplit.copyFrom(1, 0, wetBuffer, 1, 0, wetBuffer.getNumSamples());
	
    //===apply post-filter1===
    sharedMemory.enter();
    paramsPostFilter1[1] = postFilter1Cutoff; //post filter freq
    paramsPostFilter1[2] = postFilter1Bandwidth; //pre filter bandwidth 
    postFilter1->setParams(paramsPostFilter1);
    postFilter1->process(wetBuffer.getNumSamples(), wetBuffer.getArrayOfChannels());
    sharedMemory.exit();
	
	//===apply post-filter1===
    sharedMemory.enter();
    paramsPostFilter2[1] = postFilter2Cutoff; //post filter freq
    paramsPostFilter2[2] = postFilter2Bandwidth; //pre filter bandwidth 
    postFilter2->setParams(paramsPostFilter2);
    postFilter2->process(wetBufferSplit.getNumSamples(), wetBufferSplit.getArrayOfChannels());
    sharedMemory.exit();
    
	//mixes the seperate filtered signals
	wetBuffer.addFrom(0, 0, wetBufferSplit, 0, 0, wetBufferSplit.getNumSamples());
    wetBuffer.addFrom(1, 0, wetBufferSplit, 1, 0, wetBufferSplit.getNumSamples());
	
    //==apply post-gain===
	sharedMemory.enter();
	for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
		wetBuffer.applyGainRamp(i, 0, wetBuffer.getNumSamples(), outputGain, outputGainPrev);	
	}	
	sharedMemory.exit();
	
	outputGainPrev = outputGain;
	
    //set the relative gains of the processed and unprocessed buffers using the 'mix' value
    sharedMemory.enter();
	
	dryBuffer.applyGainRamp(0, 0, bufferToFill.buffer->getNumSamples(), (1.0-wetDryMix), (1.0-wetDryMixPrev));
	dryBuffer.applyGainRamp(1, 0, bufferToFill.buffer->getNumSamples(), (1.0-wetDryMix), (1.0-wetDryMixPrev));
	
	wetBuffer.applyGainRamp(0, 0, wetBuffer.getNumSamples(), wetDryMix, wetDryMixPrev);
	wetBuffer.applyGainRamp(1, 0, wetBuffer.getNumSamples(), wetDryMix, wetDryMixPrev);
	
	wetDryMixPrev = wetDryMix;
    sharedMemory.exit();
	
	//add the processed buffer to the unprcocessed buffer to create wet/dry mix
	bufferToFill.buffer->addFrom(0, 0, wetBuffer, 0, 0, wetBuffer.getNumSamples());
    bufferToFill.buffer->addFrom(1, 0, wetBuffer, 1, 0, wetBuffer.getNumSamples());
  
}


void Distortion::processAlphaTouch (int pressureValue)
{
    sharedMemory.enter();
    switch (alphaTouchParam) 
    {
        case 2: //Mix
            if (alphaTouchReverse == false)
                wetDryMix = wetDryMixControl + (pressureValue * (((1.0-wetDryMixControl)*alphaTouchIntensity)/511.0));
            else
                wetDryMix = wetDryMixControl - (pressureValue * (((1.0-(1.0-wetDryMixControl))*alphaTouchIntensity)/511.0));
            
            std::cout << wetDryMix << std::endl;
            break;
            
        case 3: //Tone
            if (alphaTouchReverse == false){
                postFilter1Cutoff = toneControl + (pressureValue * (((20000.0-toneControl)*alphaTouchIntensity)/511.0));
				postFilter2Cutoff = toneControl + (pressureValue * (((20000.0-toneControl)*alphaTouchIntensity)/511.0));
			}
            else{
                postFilter1Cutoff = toneControl - (pressureValue * (((19970.0-(20000.0 - toneControl))*alphaTouchIntensity)/511.0)); 
				postFilter2Cutoff = toneControl - (pressureValue * (((19970.0-(20000.0 - toneControl))*alphaTouchIntensity)/511.0));
            }
			
            std::cout << postFilter1Cutoff << "&" << postFilter2Cutoff << std::endl;
            break;
            
        case 4: //Drive
            if (alphaTouchReverse == false)
                drive = driveControlValue + (pressureValue * (((16.0-driveControlValue)*alphaTouchIntensity)/511.0));
            else
                drive = driveControlValue - (pressureValue * (((16.0-(16.0 - driveControlValue))*alphaTouchIntensity)/511.0));
            
            std::cout << drive << std::endl;
            break;
			
		case 5: //Input gain
            if (alphaTouchReverse == false)
                inputGain = inputGainControl + (pressureValue * (((16.0-inputGainControl)*alphaTouchIntensity)/511.0));
            else
                inputGain = inputGainControl - (pressureValue * (((16.0-(16.0 - inputGainControl))*alphaTouchIntensity)/511.0));
            
            std::cout << inputGain << std::endl;
            break;
			
		case 6: //Output gain
            if (alphaTouchReverse == false)
                outputGain = outputGainControl + (pressureValue * (((16.0-outputGainControl)*alphaTouchIntensity)/511.0));
            else
                outputGain = outputGainControl - (pressureValue * (((16.0-(16.0 - outputGainControl))*alphaTouchIntensity)/511.0));
            
            std::cout << outputGain << std::endl;
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
     
}

void Distortion::setInputGain (double value)
{
    sharedMemory.enter();
	value = value*value*value;
    inputGain = inputGainControl = value;
    sharedMemory.exit();
}

void Distortion::setDrive(double value)
{
	sharedMemory.enter();
    drive = driveControlValue = value;
	drive = 1.f + (22.f * drive);
    sharedMemory.exit();
}

void Distortion::setOutputGain (double value)
{
    sharedMemory.enter();
	value = value*value*value;
    outputGain = value;
    sharedMemory.exit();
}

void Distortion::setMix (double value)
{
    sharedMemory.enter();
	value = value*value*value;
    wetDryMix = wetDryMixPrev = wetDryMixControl = value;
    sharedMemory.exit();
}

void Distortion::setTone (double value)
{
    sharedMemory.enter();
	value = value*value*value;
	postFilter1Cutoff = (value * 5000) + 250;
	postFilter2Cutoff = (value * 5000) + 50;
    sharedMemory.exit();
}

void Distortion::setDistortionTypeMenu(int value)
{
	sharedMemory.enter();
	comboBoxID = value;
	sharedMemory.exit();
}

void Distortion::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void Distortion::setAlphaTouchIntensity (float value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void Distortion::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void Distortion::setSampleRate(float value)
{
    sharedMemory.enter();
    paramsPreFilter[0] = paramsPostFilter[0] = value;
    sharedMemory.exit();
}
