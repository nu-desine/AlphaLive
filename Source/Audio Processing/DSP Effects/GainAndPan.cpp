//
//  Gain&Pan.cpp
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

#include "GainAndPan.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

GainAndPan::GainAndPan(int padNumber_, float sampleRate_)
            : padNumber(padNumber_)
{
    gain = gainPrev =  gainControl = PAD_SETTINGS->getPadFxGainPanGain();
    pan = panPrev = panControl = PAD_SETTINGS->getPadFxGainPanPan();
    //pan.setPanControlValue(panControl);
    
    alphaTouchParam = PAD_SETTINGS->getPadFxGainPanAlphaTouch();
    alphaTouchReverse = PAD_SETTINGS->getPadFxGainPanAtReverse();
    alphaTouchIntensity = PAD_SETTINGS->getPadFxGainPanAtIntensity();
    
    sampleRate = 44100;
    
}

GainAndPan::~GainAndPan()
{
    
}

//Function that processes the audio
void GainAndPan::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    /*
    //get first pair of sample data from audio buffer
    float *pOutL = bufferToFill.buffer->getSampleData (0, bufferToFill.startSample);
    float *pOutR = bufferToFill.buffer->getSampleData (1, bufferToFill.startSample);
    
    sharedMemory.enter();
    //increment through each pair of samples (left channel and right channel) in the current block of the audio buffer
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        //pan and gain
        *pOutL = panObject.leftChanPan(*pOutL) * gain;
        *pOutR = panObject.rightChanPan(*pOutR) * gain;
        
        //move to next pair of samples
        pOutL++;
        pOutR++;
    }
    sharedMemory.exit();
     */
    
    //gain and pan
    sharedMemory.enter();
    
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
        if (i == 0) //left chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                PanControl::leftChanPan_(panPrev) * gainPrev,
                                                PanControl::leftChanPan_(pan) * gain);
        else if (i == 1) // right chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                PanControl::rightChanPan_(panPrev) * gainPrev,
                                                PanControl::rightChanPan_(pan) * gain);
    }
    
    gainPrev = gain;
    panPrev = pan;
    sharedMemory.exit();
    
}

//Function that takes in a pressure value and modulates the gain/pan depending on the set AlphaTouch parameter
void GainAndPan::processAlphaTouch (int pressureValue)
{
    
    sharedMemory.enter();
    switch (alphaTouchParam) 
    {
        case 2: //Gain
            if (alphaTouchReverse == false)
                gain = gainControl + (pressureValue * (((1.0-gainControl)*alphaTouchIntensity)/511.0));
            else
                gain = gainControl - (pressureValue * (((1.0-(1.0-gainControl))*alphaTouchIntensity)/511.0));
            
            std::cout << gain << std::endl;
            break;
            
        case 3: //Pan
            if (alphaTouchReverse == false)
                pan = panControl + (pressureValue * (((1.0-panControl)*alphaTouchIntensity)/511.0));
            else
                pan = panControl - (pressureValue * (((1.0-(1.0-panControl))*alphaTouchIntensity)/511.0));
            
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
     
}

void GainAndPan::setGain (double value)
{
    sharedMemory.enter();
    
    if (value <= 1.0)
    {
        gain = value * value * value;
        gainControl = gain;
    }
    else
        gain = gainControl = value;
    
    sharedMemory.exit();
    
}
void GainAndPan::setPan (double value)
{
    sharedMemory.enter();
    pan = panControl = value;
    sharedMemory.exit();
}

void GainAndPan::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}
void GainAndPan::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}
void GainAndPan::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse= value;
    sharedMemory.exit();
}

void GainAndPan::setSampleRate (float value)
{
    //don;t actually need this function of variable - pan or gain don't need to know the sample rate
    sampleRate = value;
}
