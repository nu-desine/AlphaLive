//
//  Delay.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/07/2012.
//  Copyright 2012 nu desine.
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

//NOTES ABOUT THE DELAY EFFECT:
//Currently, if you change the tempo, you need to change or reset the delay note length
//before it will sync to the new tempo.

#ifndef H_DELAY
#define H_DELAY


#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Third Party Libraries/DspFilters/Dsp.h"
#include "../../Third Party Libraries/Juced/jucetice_CircularBuffer.h"

class Delay
{
public:
	Delay(int padNumber_, int sampleRate_);
	~Delay();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill);
    //Function that takes in a pressure value and modulates the effect depending on the set AlphaTouch parameter
    void processAlphaTouch (int pressureValue);
    
    //Functions that set the effect parameters
    void setWetMix (double value);
    void setDelayTime (double value);
    void setFeedback (double value);
    void setLpfFrequency (double value);
    void setHpfFrequency (double value);
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value);
    void setAlphaTouchIntensity (double value);
    void setAlphaTouchReverse (int value);
    
    void setSampleRate (float value);
    
    void resetBuffers();
    
private:
    
    //Delay stuff
    CircularBuffer<float> circBufferLeft;
    CircularBuffer<float> circBufferRight;
    AudioSampleBuffer wetBuffer;
    
    //The filters
    Dsp::Filter *lpFilter;
    Dsp::Filter *hpFilter;
    Dsp::Params paramsLpFilter;
    Dsp::Params paramsHpFilter;
    
    double wetMix, wetMixPrev, wetMixControlValue;
    double lpfFrequencyControlValue, hpfFrequencyControlValue;
    double feedback, feedbackControlValue;
    int delaySamples, delaySamplesControlValue;
    
    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    int padNumber, sampleRate;
    float maxSamplesRange, delayTime;

    
};

#endif //H_DELAY