//
//  LFO.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 12/07/2012.
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

#ifndef H_OSCILLATOR
#define H_OSCILLATOR

//#include <iostream.h>
#include <math.h>
#include "../../JuceLibraryCode/JuceHeader.h"

class Oscillator
{
public:
    Oscillator (double sampleRate);
    ~Oscillator();
    
    double process (double frequency, int waveShape);
    void setSampleRate (double sampleRate);
    void restart();
    
private:
    double processSin();
    double processSqr();
    double processTri();
    double processSawUp();
    double processSawDown();
	
	CriticalSection sharedMemory;
    
    double currentPhase, increment, sampRate;
	
	double numSamples, stepSize, currentSample;
	
	AudioSampleBuffer squareBuffer;
	
	//buffer file source
	File audioFile;
    
};

#endif //H_OSCILLATOR