//
//  Flanger.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 14/07/2012.
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
//

#ifndef H_FLANGER
#define H_FLANGER


#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Third Party Libraries/Juced/jucetice_CircularBuffer.h"
#include "../Oscillator.h"

class Flanger
{
public:
	Flanger(int padNumber_, int sampleRate_);
	~Flanger();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill);
    //Function that takes in a pressure value and modulates the effect depending on the set AlphaTouch parameter
    void processAlphaTouch (int pressureValue);
    
    //Functions that set the effect parameters
    void setMix (double value);
    void setFeedback (double value);
    void setRate (double value);
    void setIntensity (double value); //LFO depth
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value);
    void setAlphaTouchIntensity (double value);
    void setAlphaTouchReverse (int value);
    
    void setSampleRate (float value);
    
    void restart();
    void resetBuffers();
    
private:
    
    //Delay stuff
    CircularBuffer<float> circBufferLeft;
    CircularBuffer<float> circBufferRight;
    AudioSampleBuffer wetBuffer;
    
    Oscillator *lfo;
    
    double mix, mixPrev, mixControlValue;
    double intensity, intensityControlValue;
    double rate, rateControlValue;
    double feedback, feedbackControlValue;
    
    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    int padNumber;
    float sampleRate; // do i need this?
    
};

#endif //H_FLANGER