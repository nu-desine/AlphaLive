//
//  Gain&Pan.h
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

#ifndef H_GAINPAN
#define H_GAINPAN

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../PanControl.h"
#include "../Oscillator.h"

class GainAndPan
{
public:
	GainAndPan(int padNumber_, float sampleRate_);
	~GainAndPan();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill);
    //Function that takes in a pressure value and modulates the audio depending on the set AlphaTouch parameter
    void processAlphaTouch (int pressureValue);
    
    //functions to set effect parameters
    void setGain (double value);
    void setPan (double value);
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value);
    void setAlphaTouchIntensity (double value);
    void setAlphaTouchReverse (int value);
    
    void setSampleRate (float value);
    
private:

    //we need the ...Control variables as the actual gain or pan won't always be
    //equal to the value of their GUI control as the pressure could be used to
    //modulate the value
    double gain, gainPrev, gainControl;
    double pan, panPrev, panControl;

    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    int padNumber;
    int sampleRate;
    
};

#endif //H_GAINPAN