//
//  Delay.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/07/2012.
//  Copyright 2012 nu desine. All rights reserved.
//

#include "Delay.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]
#define MAX_BUFFER_SIZE 768000  //max number of samples needed, based on a tempo of 60bpm, 
                                //a delay time of a whole note, and a sample rate of 192000.

Delay::Delay(int padNumber_, int sampleRate_)
:   
    circBufferLeft(MAX_BUFFER_SIZE),
    circBufferRight(MAX_BUFFER_SIZE),
    wetBuffer(2, 44100), //buffer size only needs to be the size of bufferToFill below. how do i set that here?
    padNumber(padNumber_)
{
    delayTime = PAD_SETTINGS->getPadFxDelayTime();
    setSampleRate(sampleRate_); //this sets the delaySamples and delaySamplesControlValue default values too
    
    //Delay buffers
    circBufferLeft.reset();
    circBufferRight.reset();
    wetBuffer.clear();
    
    //=====LPF=====
    lpFilter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2> (1024);
    // sample rate
    paramsLpFilter[0] = sampleRate;
    // cutoff frequency
    paramsLpFilter[1] = lpfFrequencyControlValue = PAD_SETTINGS->getPadFxDelayLpfFreq();
    // Q/Bandwidth
    paramsLpFilter[2] = 1.0;
    
    //=====HPF=====
    hpFilter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::HighPass, 2> (1024);
    // sample rate
    paramsHpFilter[0] = sampleRate;
    // cutoff frequency
    paramsHpFilter[1] = hpfFrequencyControlValue = PAD_SETTINGS->getPadFxDelayHpfFreq();
    // Q/Bandwidth
    paramsHpFilter[2] = 1.0;
    
    wetMix = wetMixPrev = wetMixControlValue =  PAD_SETTINGS->getPadFxDelayMix();
    feedback = feedbackControlValue = PAD_SETTINGS->getPadFxDelayFeedback();
    
    alphaTouchParam = PAD_SETTINGS->getPadFxDelayAlphaTouch();
    alphaTouchReverse = PAD_SETTINGS->getPadFxDelayAtReverse();
    alphaTouchIntensity = PAD_SETTINGS->getPadFxDelayAtIntensity();
    
    maxSamplesRange = sampleRate * 4; //as a whole note at the lowest tempo of 60 equals 4 seconds
}

Delay::~Delay()
{
	delete lpFilter;
    delete hpFilter;
}

//==============================================================================


void Delay::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    //NOTES ON AUDIO SAMPLE BUFFER
    //as far as I can tell addFrom() adds data from one buffer to the other (+=), mixing the sound,
    //whereas copyFrom() replaces the current data with new data (=)
    
    //NOTES ABOUT THIS DELAY ALGORITHM
    //The code uses the following buffers:
    // - BufferToFill.buffer - the incoming/outgoing audio buffer (AudioSampleBuffer)
    // - wetBuffer - the processed audio buffer which holds the delayed signal (AudioSampleBuffer)
    // - circBufferLeft & circBufferRight - the buffer which stores the last 'n' amount of samples (CircularBuffer)
    // The algorithm creates a simple 'Recirculating echo' with a LPF and HPF applied to the delay signal.
    // The followings parameters are controllable - delay time, feedback, wet gain, LPF freq, HPF freq.
    // First, the output signal is set by adding the wet buffer to the ouput signal with a feedback value.
    // Next, the output signal is added to the circular buffers.
    // The wet/processed signal is then created by adding sample data from the circular buffers at a certain 'read' point
    // which determines the delay time.
    // A gain ramp is then applied to the processed signal.
    // Finally a LPF and HPF are applied to the processed signal.
    // The wetBuffer is needed as it is not as simple to apply a gain ramp and filtering to the circular buffers directly.
    
    
    sharedMemory.enter();
    //get first pair of sample data from incoming buffer
    float *pOutL = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    float *pOutR = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    //get the first pair of samples from the processed buffer
    float *pWetL = wetBuffer.getWritePointer (0, bufferToFill.startSample);
    float *pWetR = wetBuffer.getWritePointer (1, bufferToFill.startSample);
    
    //increment through each pair of samples
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        //set the output sample data by adding the wet buffer with a feedback value
		if (delayTime != 0) 
		{
			*pOutL = *pOutL + (*pWetL * feedback);
			*pOutR = *pOutR + (*pWetR * feedback);
		}
        
        //set the circular buffers sample data
        circBufferLeft.put(*pOutL);
        circBufferRight.put(*pOutR);
        
        //set the wet buffer sample data with a delay value
        *pWetL = circBufferLeft.get(delaySamples);
        *pWetR = circBufferRight.get(delaySamples);
        
        //move to next pair of samples
        pOutL++;
        pOutR++;
        pWetL++;
        pWetR++;
    }
    
    sharedMemory.exit();
    
    //set wet buffer gain
    sharedMemory.enter();
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
        wetBuffer.applyGainRamp (i,
                                 bufferToFill.startSample,
                                 bufferToFill.numSamples,
                                 wetMixPrev,
                                 wetMix);
        
    }
    wetMixPrev = wetMix;
    sharedMemory.exit();
    
    //Apply filtering to the wet buffer
    sharedMemory.enter();
    //apply low pass filter to the delay buffer
    lpFilter->setParams(paramsLpFilter);
    lpFilter->process(bufferToFill.numSamples, wetBuffer.getArrayOfWritePointers());
    //apply high pass filter to the delay buffer
    hpFilter->setParams(paramsHpFilter);
    hpFilter->process(bufferToFill.numSamples, wetBuffer.getArrayOfWritePointers());
    sharedMemory.exit();

}


void Delay::processAlphaTouch (int pressureValue)
{
    sharedMemory.enter();
    switch (alphaTouchParam) 
    {
        case 2: //Wet Mix
            if (alphaTouchReverse == false)
                wetMix = wetMixControlValue + (pressureValue * (((1.0-wetMixControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                wetMix = wetMixControlValue - (pressureValue * (((1.0-(1.0-wetMixControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << wetMix << std::endl;
            break;
            
        case 3: //delay time/samples
            
            if (alphaTouchReverse == false)
                delaySamples = delaySamplesControlValue + (pressureValue * (((maxSamplesRange-delaySamplesControlValue)*alphaTouchIntensity)/MAX_PRESSURE)); 
            else
                delaySamples = delaySamplesControlValue - (pressureValue * (((maxSamplesRange-(maxSamplesRange-delaySamplesControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << delaySamples << std::endl;
            break;
            
        case 4: //Feedback
            if (alphaTouchReverse == false)
                feedback = feedbackControlValue + (pressureValue * (((1.0-feedbackControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                feedback = feedbackControlValue - (pressureValue * (((1.0-(1.0-feedbackControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << feedback << std::endl;
            break;
            
        case 5: //LPF Frequency
            if (alphaTouchReverse == false)
                paramsLpFilter[1] = lpfFrequencyControlValue + (pressureValue * (((20000.0-lpfFrequencyControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                paramsLpFilter[1] = lpfFrequencyControlValue - (pressureValue * (((19970.0-(20000.0 - lpfFrequencyControlValue))*alphaTouchIntensity)/MAX_PRESSURE)); 
            
            //std::cout << paramsLpFilter[1] << std::endl;
            break;
            
        case 6: //HPF Frequency
            if (alphaTouchReverse == false)
                paramsHpFilter[1] = hpfFrequencyControlValue + (pressureValue * (((20000.0-hpfFrequencyControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                paramsHpFilter[1] = hpfFrequencyControlValue - (pressureValue * (((19970.0-(20000.0 - hpfFrequencyControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << paramsHpFilter[1] << std::endl;
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
}


void Delay::setWetMix (double value)
{
    sharedMemory.enter();
    wetMix = wetMixControlValue = value * value * value;
    sharedMemory.exit();
}

void Delay::setDelayTime (double value)
{
    sharedMemory.enter();
    delayTime = value;
    //algorithm that converts delay time to number of samples
    delaySamples = delaySamplesControlValue = sampleRate * (value/1000.0);
    sharedMemory.exit();
}

void Delay::setFeedback (double value)
{
    sharedMemory.enter();
    feedback = feedbackControlValue = value;
    sharedMemory.exit();
}

void Delay::setLpfFrequency (double value)
{
    sharedMemory.enter();
    paramsLpFilter[1] = lpfFrequencyControlValue = value;
    sharedMemory.exit();
}

void Delay::setHpfFrequency (double value)
{
    sharedMemory.enter();
    paramsHpFilter[1] = hpfFrequencyControlValue = value;
    sharedMemory.exit();
}


void Delay::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void Delay::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void Delay::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void Delay::setSampleRate(float value)
{
    sharedMemory.enter();
    paramsLpFilter[0] = paramsHpFilter[0] = sampleRate = value;
    maxSamplesRange = sampleRate * 4; //as a whole note at the lowest tempo of 60 equals 4 seconds
    setDelayTime(delayTime);    //re-sets the delayTime and delaySamples variables so that the delay 
                                //is correct with the new sample rate 
    
    //std::cout << "Sample Rate = " << sampleRate << std::endl;
    //std::cout << "Delay Time = " << delayTime << std::endl;
    //std::cout << "Delay Samples = " << delaySamples << std::endl;
    
    sharedMemory.exit();
}

void Delay::resetBuffers()
{
    circBufferLeft.reset();
    circBufferRight.reset();
    wetBuffer.clear();
}