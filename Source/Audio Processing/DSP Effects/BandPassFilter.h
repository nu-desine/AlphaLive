/*
 *  BandPassFilter.h
 *  AlphaLive
 *
 *  Created by Liam Lacey on 20/12/2011.
 *  Copyright 2011 nu desine.
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


#ifndef H_BANDPASSFILTER
#define H_BANDPASSFILTER


#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Third Party Libraries/DspFilters/Dsp.h"

class BandPassFilter
{
public:
	BandPassFilter(int padNumber_, float sampleRate_);
	~BandPassFilter();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill);
    //Function that takes in a pressure value and modulates the filter depending on the set AlphaTouch parameter
    void processAlphaTouch (int pressureValue);
    
    //Functions that set the effect parameters
    void setFrequency (double value);
    void setMix (double value);
    void setBandwidth( double value);
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value);
    void setAlphaTouchIntensity (double value);
    void setAlphaTouchReverse (int value);
    
    void setSampleRate (float value);
    
private:
    
    //The filter
    Dsp::Filter *filter;
    Dsp::Params params;
    
    double mix, mixPrev, mixControlValue;
    double frequencyControlValue;
    double bandwidthControlValue;
    
    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    int padNumber;
    
};

#endif //H_HIGHPASSFILTER