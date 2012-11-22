//
//  Tremolo.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 13/07/2012.
//  Copyright 2012 nu desine. All rights reserved.
//

#include "Tremolo.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

Tremolo::Tremolo(int padNumber_, int sampleRate_)
:   padNumber(padNumber_)
{
    lfo = new Oscillator (sampleRate_);
    
    //setSampleRate(sampleRate_); //do actually i need to do this here?
    
    depth = depthControlValue =  PAD_SETTINGS->getPadFxTremoloDepth();
    rate = rateControlValue = (AppSettings::Instance()->getGlobalTempo()/60.0) * 1; //sets the default value to 1/4
    shape = shapeControlValue = PAD_SETTINGS->getPadFxTremoloShape();
    
    alphaTouchParam = PAD_SETTINGS->getPadFxTremoloAlphaTouch();
    alphaTouchReverse = PAD_SETTINGS->getPadFxTremoloAtReverse();
    alphaTouchIntensity = PAD_SETTINGS->getPadFxTremoloAtIntensity();
    
}

Tremolo::~Tremolo()
{
	delete lfo;
}

//==============================================================================


void Tremolo::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    
    sharedMemory.enter();
    
    //double rate = effectParam[0] * (20.0/1.0);
    //double depth = effectParam[1];
    //depth = pow(depth, 0.3); //use this to give finer controller over the higher depths. The lower the number exponent the finer.
    //int waveShape = 1.0 + (effectParam[2] * (4.0/1.0));
    float lfoValue;
    
    //get first pair of sample data from incoming buffer
    float *pOutL = bufferToFill.buffer->getSampleData (0, bufferToFill.startSample);
    float *pOutR = bufferToFill.buffer->getSampleData (1, bufferToFill.startSample);
    
    //increment through each pair of samples
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        //changing the depth below needs to be done with a gain ramp for smoothness
        
        //process LFO
        lfoValue = lfo->process(rate, shape);
        //scale and offset output so the osc can be used as an LFO
        lfoValue = (lfoValue * (depth/2.0)) + (1 - (depth/2.0)); 
        //apply LFO object to sample data
        *pOutL = *pOutL * lfoValue;
        *pOutR = *pOutR * lfoValue;
        
        //move to next pair of samples
        pOutL++;
        pOutR++;
        
    }
    
    sharedMemory.exit();
    
}


void Tremolo::processAlphaTouch (int pressureValue)
{
    
    sharedMemory.enter();
    switch (alphaTouchParam) 
    {
        case 2: //Depth
            if (alphaTouchReverse == false)
                depth = depthControlValue + (pressureValue * (((1.0-depthControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
            else
                depth = depthControlValue - (pressureValue * (((1.0-(1.0-depthControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            std::cout << depth << std::endl;
            break;
            
        case 3: //rate
            
            if (alphaTouchReverse == false)
                rate = rateControlValue + (pressureValue * (((50.0-rateControlValue)*alphaTouchIntensity)/MAX_PRESSURE)); 
            else
                rate = rateControlValue - (pressureValue * (((50.0-(50.0-rateControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            std::cout << rate << std::endl;
            break;
            
        case 4: //shape
            if (alphaTouchReverse == false)
                shape = roundToInt (shapeControlValue + (pressureValue * (((5.0-shapeControlValue)*alphaTouchIntensity)/MAX_PRESSURE)));
            else
                shape = roundToInt (shapeControlValue - (pressureValue * (((4.0-(5.0-shapeControlValue))*alphaTouchIntensity)/MAX_PRESSURE))); 
            
            //the above equations don't seem to work. why?
            
            std::cout << shape << std::endl;
            break;
            
            
        default:
            break;
    }
    sharedMemory.exit();
     
}

void Tremolo::restart()
{
    //restarts the lfo's cycle. This is called when the audio file is first triggered.
    //would be good to have the option to sync the lfo to the global clock too.
    lfo->restart();
}

void Tremolo::setDepth (double value)
{
    sharedMemory.enter();
    depth = depthControlValue =  value;
    sharedMemory.exit();
}

void Tremolo::setRate (double value)
{
    sharedMemory.enter();
    rate = rateControlValue = value;
    sharedMemory.exit();
}

void Tremolo::setShape (int value)
{
    sharedMemory.enter();
    shape = shapeControlValue = value;
    sharedMemory.exit();
}


void Tremolo::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void Tremolo::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void Tremolo::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void Tremolo::setSampleRate(float value)
{
    sharedMemory.enter();
    sampleRate = value;
    lfo->setSampleRate(value);
    sharedMemory.exit();
}

