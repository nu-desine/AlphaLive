//
//  Reverb.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 21/12/2011.
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

#ifndef H_REVERB
#define H_REVERB

//how should I handle the wet/dry mix for reverb?
//Currently I have done it exactly the same as with the filter - processing a copy of the input buffer
//and then mixing this copy back in with the original unprocessed buffer to create a wet/dry mix
//However the Juce Reverb Class as seperate Dry and Wet paramaters that could be used to change the mix.
//This would probably be easier - will it change the interface and how 'modular' the effect is to the rest of the application?

//IS THIS EFFECT CURRENTLY JUST IN MONO? CHECK IT OUT AND MAKE IT STEREO!

#include "../../../JuceLibraryCode/JuceHeader.h"

class ReverbClass
{
public:
	ReverbClass(int padNumber_, float sampleRate_);
	~ReverbClass();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill);
    //Function that takes in a pressure value and modulates the reverb depending on the set AlphaTouch parameter
    void processAlphaTouch (int pressureValue);
    
    //functions to set effect parameters
    void setMix (float value);
    void setRoomSize (float value);
    void setDamping (float value);
    void setWidth (float value);
    void setFreezeMode (float value);
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value);
    void setAlphaTouchIntensity (float value);
    void setAlphaTouchReverse (int value);
    
    void setSampleRate (float value);
    
private:
    
    //the reverb DSP object
    Reverb reverb;
    Reverb::Parameters params;
    
    double mix, mixPrev, mixControlValue;
    double roomSizeControl;
    double dampingControl;
    double widthControl;
    double freezeModeControl;
    
    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    int padNumber;
    
};

#endif //H_REVERB