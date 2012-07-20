//
//  LFO.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 12/07/2012.
//  Copyright 2012 nu desine.
//

#include "Oscillator.h"

#ifndef M_PI
#define M_PI (3.1415926535897932)
#endif
#define TWOPI (2.0 * M_PI)

Oscillator::Oscillator (double sampleRate)
                    :  sampRate(sampleRate)
{
    currentPhase = 0.0; 
}

Oscillator::~Oscillator()
{
    
}

double Oscillator::process (double frequency, int waveShape)
{
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
    
    increment = frequency * (TWOPI / sampRate);
    currentPhase += increment;
    
    if (currentPhase >= TWOPI)
        currentPhase -= TWOPI;
    if (currentPhase < 0.0)
        currentPhase += TWOPI;
    
    return output;
    
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
    
    if (currentPhase <= M_PI)
        output = 1.0;
    else
        output = -1.0;
    
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
    
    output = (2.0 * (currentPhase * (1.0 / TWOPI))) - 1.0;
    
    return output;
}

double Oscillator::processSawDown()
{
    double output;
    
    output = 1.0 - 2.0 * (currentPhase * (1.0 / TWOPI)); 
    
    return output;
}

void Oscillator::setSampleRate (double value)
{
    sampRate = value;
}


void Oscillator::restart()
{
    currentPhase = 0.0; 
}