//
//  LFO.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 12/07/2012.
//  Copyright 2012 nu desine.
//

#include "Oscillator.h"
#include "../GUI Classes/Binary Data/MainBinaryData.h"

#ifndef M_PI
#define M_PI (3.1415926535897932)
#endif
#define TWOPI (2.0 * M_PI)

Oscillator::Oscillator (double sampleRate)
:			
	sampRate(sampleRate),
	squareBuffer(1, 22071),
	sawBuffer(1, 21984),
	sawDownBuffer(1, 21984)
{
	currentPhase = 0.0; 
	currentSample = 0.0;
	stepSize = 0;
	squareBuffer.clear();
	sawBuffer.clear();
	sawDownBuffer.clear();
	
	sharedMemory.enter();
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	
    ScopedPointer <AudioFormatReader> squareReader 
    (formatManager.createReaderFor(new MemoryInputStream (MainBinaryData::squarewave20500_wav,
                                                          MainBinaryData::squarewave20500_wavSize,
                                                          false)));
    
	squareReader->read(&squareBuffer, 0, squareReader->lengthInSamples, 0, true, false);
	
    squareNumSamples = squareReader->lengthInSamples;
	
	ScopedPointer <AudioFormatReader> sawReader 
    (formatManager.createReaderFor(new MemoryInputStream (MainBinaryData::sawwave20500_wav,
                                                          MainBinaryData::sawwave20500_wavSize,
                                                          false)));
    
	sawReader->read(&sawBuffer, 0, sawReader->lengthInSamples, 0, true, false);
	
	ScopedPointer <AudioFormatReader> sawDownReader
    (formatManager.createReaderFor(new MemoryInputStream (MainBinaryData::sawdownwave20500_wav,
                                                          MainBinaryData::sawdownwave20500_wavSize,
                                                          false)));
    
	sawDownReader->read(&sawDownBuffer, 0, sawDownReader->lengthInSamples, 0, true, false);
	
	sawNumSamples = sawReader->lengthInSamples; 
	
	sharedMemory.exit();
}

Oscillator::~Oscillator()
{

}

double Oscillator::process (double frequency, int waveShape)
{    
	sharedMemory.enter();
	
	double output;
    switch (waveShape)
    {
        case 1:
            output = processSin();
            break;
        case 2:
            output = processSqr();
            break;
        case 3:
            output = processTri();
            break;
        case 4:
            output = processSawUp();
            break;
        case 5:
            output = processSawDown();
            break;
        default:
            output = processSin();
            break;
            
    }
    
    //the below functions being in seperate if else statements
    //means that when switching between certain waveforms the
    //waveform won't still be in sync. Could do with interating
    //through all the values no matter what the waveform is
	if (waveShape == 1 || waveShape == 3) 
	{
		increment = frequency * (TWOPI / sampRate);
		currentPhase += increment;
		
		if (currentPhase >= TWOPI)
			currentPhase -= TWOPI;
		if (currentPhase < 0.0)
			currentPhase += TWOPI;
	}
	else if (waveShape == 2) 
	{
		stepSize = squareNumSamples * (frequency / sampRate);
		currentSample += stepSize;
		
		//std::cout << "In process current sample: " <<  << std::endl;
		
		if (currentSample >= squareNumSamples)
			currentSample -= squareNumSamples;
		if (currentSample < 0.0)
			currentSample += squareNumSamples;
		
	}
	else if (waveShape == 4 || waveShape == 5) 
	{
		stepSize = sawNumSamples * (frequency / sampRate);
		currentSample += stepSize;
		
		//std::cout << "In process current sample: " <<  << std::endl;
		
		if (currentSample >= sawNumSamples)
			currentSample -= sawNumSamples;
		if (currentSample < 0.0)
			currentSample += sawNumSamples;
		
	}
	
    return output;
    
	sharedMemory.exit();
    //A digital wave signal is in the range of +1 to -1.
    //To creating an LFO using this class, the ouput must be scaled and offset
    //afterwards into the desired range.
    //For modulating amplitude, this is usually in the range 0 to 1;
    //the following equation is an example of how to do this:
    //output = (output * 0.5) + 0.5
    //the 0.5 is equal to the new range divided by the old range.
    
}



double Oscillator::processSin()
{
    double output;
    
    output = sin(currentPhase);
    
    return output;
}

double Oscillator::processSqr()
{
	double output;
	
	sharedMemory.enter();
	output = *squareBuffer.getWritePointer(0, currentSample);
    sharedMemory.exit();

    return output;
}

double Oscillator::processTri()
{
    double output;
    
    //rectified sawtooth
    output = (2.0 * (currentPhase * (1.0 / TWOPI))) - 1.0;
    if (output < 0.0)
        output = -output;
    output = 2.0 * (output - 0.5);
    
    return output;
}

double Oscillator::processSawUp()
{
    double output;
    
    //output = (2.0 * (currentPhase * (1.0 / TWOPI))) - 1.0;
	
	sharedMemory.enter();
	output = *sawBuffer.getWritePointer(0, currentSample);
    sharedMemory.exit();
    
    return output;
}

double Oscillator::processSawDown()
{
    double output;
    
    //output = 1.0 - 2.0 * (currentPhase * (1.0 / TWOPI)); 
	
	sharedMemory.enter();
	output = *sawDownBuffer.getWritePointer(0, currentSample);
    sharedMemory.exit();
    
    return output;
}

void Oscillator::setSampleRate (double value)
{
    sampRate = value;
}


void Oscillator::restart()
{
    currentPhase = 0.0; 
    currentSample = 0;
}