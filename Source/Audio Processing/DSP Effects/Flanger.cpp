//
//  Flanger.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 14/07/2012.
//  Copyright 2012 nu desine.
//
#include "Flanger.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]
#define MAX_BUFFER_SIZE 2000  //max number of samples needed?

Flanger::Flanger(int padNumber_, int sampleRate_)
:   circBufferLeft(MAX_BUFFER_SIZE),
    circBufferRight(MAX_BUFFER_SIZE),
    wetBuffer(2, 44100), //buffer size only needs to be the size of bufferToFill below. how do i set that here?
    padNumber(padNumber_)
{
    lfo = new Oscillator (sampleRate_);
    
    //delay stuff
    circBufferLeft.reset();
    circBufferRight.reset();
    wetBuffer.clear();
    
    //SET DEAFULT SETTINGS HERE FROM PAD_SETTINGS
    mix = mixPrev = mixControlValue = PAD_SETTINGS->getPadFxFlangerMix();
    rate = rateControlValue = PAD_SETTINGS->getPadFxFlangerRate();
    feedback = feedbackControlValue = PAD_SETTINGS->getPadFxFlangerFeedback();
    intensity = intensityControlValue = PAD_SETTINGS->getPadFxFlangerIntensity();
    
    alphaTouchParam = PAD_SETTINGS->getPadFxFlangerAlphaTouch();
    alphaTouchReverse = PAD_SETTINGS->getPadFxFlangerAtReverse();
    alphaTouchIntensity = PAD_SETTINGS->getPadFxFlangerAtIntensity();
    
}

Flanger::~Flanger()
{
	delete lfo;
}

//==============================================================================


void Flanger::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    
    sharedMemory.enter();
    //float feedback = effectParam[2];
    //double depth = effectParam[3]; //intensity
    //float rate = effectParam[4] * (10.0/1.0);
    //float mix = effectParam[0];
    float delaySamples;
    
    //get first pair of sample data from incoming buffer
    float *pOutL = bufferToFill.buffer->getSampleData (0, bufferToFill.startSample);
    float *pOutR = bufferToFill.buffer->getSampleData (1, bufferToFill.startSample);
    //get the first pair of samples from the processed buffer
    float *pWetL = wetBuffer.getSampleData (0, bufferToFill.startSample);
    float *pWetR = wetBuffer.getSampleData (1, bufferToFill.startSample);
    
    //increment through each pair of samples
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        //set the output sample data by adding the wet buffer with a feedback value
        *pOutL = *pOutL + (*pWetL * feedback);
        *pOutR = *pOutR + (*pWetR * feedback);
        
        //set the circular buffers sample data
        circBufferLeft.put(*pOutL);
        circBufferRight.put(*pOutR);
        
        //create the modulating delay time based on the osc
        delaySamples = lfo->process(rate, 3);
        //scale and offset the osc from +1 - -1 to 500 - 0 so it can be used as an LFO
        delaySamples = (delaySamples * (intensity * 250.0)) + (intensity * 250.0);
        
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
                                 mixPrev,
                                 mix);
        
    }
    mixPrev = mix;
    sharedMemory.exit();
    
}


void Flanger::processAlphaTouch (int pressureValue)
{
    
    
    
    sharedMemory.enter();
    switch (alphaTouchParam) 
    {
        case 2: //Mix
            if (alphaTouchReverse == false)
                mix = mixControlValue + (pressureValue * (((1.0-mixControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                mix = mixControlValue - (pressureValue * (((1.0-(1.0-mixControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            std::cout << mix << std::endl;
            break;
            
        case 3: //rate
            
            if (alphaTouchReverse == false)
                rate = rateControlValue + (pressureValue * (((20.0-rateControlValue)*alphaTouchIntensity)/MAX_PRESSURE)); 
            else
                rate = rateControlValue - (pressureValue * (((20.0-(20.0-rateControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            std::cout << rate << std::endl;
            break;
            
        case 4: //feedback
            if (alphaTouchReverse == false)
                feedback = feedbackControlValue + (pressureValue * (((1.0-feedbackControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                feedback = feedbackControlValue - (pressureValue * (((1.0-(1.0-feedbackControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            std::cout << feedback << std::endl;
            break;
            
        case 5: //intensity
            if (alphaTouchReverse == false)
                intensity = intensityControlValue + (pressureValue * (((1.0-intensityControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                intensity = intensityControlValue - (pressureValue * (((1.0-(1.0-intensityControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            std::cout << intensity << std::endl;
            break;
            
            
        default:
            break;
    }
    sharedMemory.exit();
     
     
    
}

void Flanger::restart()
{
    //restarts the lfo's cycle. This is called when the audio file is first triggered.
    //would be good to have the option to sync the lfo to the global clock too.
    lfo->restart();
}

void Flanger::resetBuffers()
{
    circBufferLeft.reset();
    circBufferRight.reset();
    wetBuffer.clear();
}

void Flanger::setMix (double value)
{
    sharedMemory.enter();
    mix = mixControlValue =  value;
    sharedMemory.exit();
}

void Flanger::setRate (double value)
{
    sharedMemory.enter();
    rate = rateControlValue = value;
    sharedMemory.exit();
}

void Flanger::setFeedback (double value)
{
    sharedMemory.enter();
    feedback = feedbackControlValue = value;
    sharedMemory.exit();
}

void Flanger::setIntensity (double value)
{
    sharedMemory.enter();
    intensity = intensityControlValue = value;
    sharedMemory.exit();
}


void Flanger::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void Flanger::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void Flanger::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void Flanger::setSampleRate(float value)
{
    sharedMemory.enter();
    sampleRate = value;
    lfo->setSampleRate(value);
    sharedMemory.exit();
}
