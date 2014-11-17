//
//  Tremolo.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 13/07/2012.
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

#ifndef H_TREMOLO
#define H_TREMOLO


#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Oscillator.h"

class Tremolo
{
public:
	Tremolo(int padNumber_, int sampleRate_);
	~Tremolo();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill);
    //Function that takes in a pressure value and modulates the effect depending on the set AlphaTouch parameter
    void processAlphaTouch (int pressureValue);
    
    //Functions that set the effect parameters
    void setDepth (double value);
    void setRate (double value);
    void setShape (int value);
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value);
    void setAlphaTouchIntensity (double value);
    void setAlphaTouchReverse (int value);
    
    void setSampleRate (float value);
    
    void restart();
    
private:

    Oscillator *lfo;
    
    double depth, depthControlValue;
    double rate, rateControlValue;
    int shape, shapeControlValue;
    
    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    int padNumber;
    double sampleRate; // do i need this?
    
};

#endif //H_TREMOLO
