//
//  GlobalClock.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/02/2012.
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

#include "GlobalClock.h"
#include "../File and Settings/AppSettings.h"
#include "AlphaLiveEngine.h"
#include "../File and Settings/StoredSettings.h"
#include "../GUI Classes/Binary Data/MainBinaryData.h"
#include "../GUI Classes/Views/MainComponent.h"

GlobalClock::GlobalClock(AlphaLiveEngine &ref) 
            :   Thread("Global Clock"),
                alphaLiveEngineRef(ref)

                            
{
    setTempo (AppSettings::Instance()->getGlobalTempo()); //sets timeInterval and midiClockTimeInterval too
    beatsPerBar = AppSettings::Instance()->getBeatsPerBar();
    quantizationValue = AppSettings::Instance()->getQuantizationValue();
    metronomeStatus = AppSettings::Instance()->getMetronomeStatus();
    midiClockValue = AppSettings::Instance()->getMidiClockValue();
    midiClockStartMessage = AppSettings::Instance()->getMidiClockStartMessage();
    midiClockMessageFilter = AppSettings::Instance()->getMidiClockMessageFilter();
    
    midiClockOutIsRunning = false;
    
    for (int i = 0; i < 6; i++)
        midiClockTempos.insert(i, 0);
    
    //=================metronome stuff======================
    
    audioTransportSourceThread = new TimeSliceThread("Metronome Audio Thread");
    audioTransportSourceThread->startThread();
    tockAudioFormatReaderSource = NULL;
    tickAudioFormatReaderSource = NULL;
    tockFileSource.setPosition(0.0);
    tockFileSource.setSource (0);
    tickFileSource.setPosition(0.0);
    tickFileSource.setSource (0);
    
    WavAudioFormat wavFormat;
    
    AudioFormatReader* reader = wavFormat.createReaderFor(new MemoryInputStream (MainBinaryData::tock_wav,
                                                                                 MainBinaryData::tock_wavSize,
                                                                                 false), true);
    
    if (reader != 0)
    {
        tockAudioFormatReaderSource = new AudioFormatReaderSource (reader, true);
        
        //set read ahead buffer size
        int bufferSize = tockAudioFormatReaderSource->getTotalLength()/2;
        //restrict buffer size value
        if (bufferSize > 48000)
            bufferSize = 48000;
        else if (bufferSize < 8000)
            bufferSize = 8000;
        
        // ..and plug it into our transport source
        tockFileSource.setSource (tockAudioFormatReaderSource,
                              bufferSize, // tells it to buffer this many samples ahead
                              audioTransportSourceThread,
                              reader->sampleRate);
    }
    
    AudioFormatReader* reader2 = wavFormat.createReaderFor(new MemoryInputStream (MainBinaryData::tick_wav,
                                                                                 MainBinaryData::tick_wavSize,
                                                                                 false), true);
    
    if (reader2 != 0)
    {
        tickAudioFormatReaderSource = new AudioFormatReaderSource (reader2, true);
        
        //set read ahead buffer size
        int bufferSize = tickAudioFormatReaderSource->getTotalLength()/2;
        //restrict buffer size value
        if (bufferSize > 48000)
            bufferSize = 48000;
        else if (bufferSize < 8000)
            bufferSize = 8000;
        
        // ..and plug it into our transport source
        tickFileSource.setSource (tickAudioFormatReaderSource,
                                  bufferSize, // tells it to buffer this many samples ahead
                                  audioTransportSourceThread,
                                  reader2->sampleRate);
    }
    
    //mix audio sources together
    audioMixer.addInputSource(&tickFileSource, false); //add as inputsource to audioMixer
    audioMixer.addInputSource(&tockFileSource, false); //add as inputsource to audioMixer
    
    broadcaster.addActionListener(this);
}

GlobalClock::~GlobalClock()
{
    tockFileSource.setSource(0);
    tickFileSource.setSource(0);
    audioMixer.removeAllInputs();
    audioTransportSourceThread->stopThread(100);
    stopThread(timeInterval);
}

void GlobalClock::actionListenerCallback (const String& message)
{
    if (message == "UPDATE CLOCK DISPLAY")
    {
        mainComponent->getGuiGlobalClock()->updateClockDisplay(beatNumber, barNumber, beatsPerBar);
    }
    
    else if (message == "UPDATE TRANSPORT BUTTON")
    {
        mainComponent->getGuiGlobalClock()->updateTransportButtonDisplay(true);
    }
    
    else if (message == "UPDATE TRANSPORT BUTTON TO OFF")
    {
        mainComponent->getGuiGlobalClock()->updateTransportButtonDisplay(false);
    }
    else if (message == "UPDATE TEMPO")
    {
        mainComponent->getGuiGlobalClock()->updateTempoDisplay(tempo);
    }
}


void GlobalClock::startClock()
{
    
    microbeatNumber = beatNumber = barNumber = 1;
    currentTime = midiClockCurrentTime = Time::getMillisecondCounterHiRes();
    midiClockMessageCounter = 6; //so that 'processClock()' will be called instantly
    prevMidiClockMessageTimestamp = Time::getMillisecondCounterHiRes();
    
    //MIDI Clock stuff
    if (midiClockValue == 2) //send MIDI Clock
    {
        if (midiClockStartMessage == 1) //'Start' message
        {
            MidiMessage message = MidiMessage::midiStart();
            alphaLiveEngineRef.sendMidiMessage(message);
        }
        else if (midiClockStartMessage == 2) //'Continue' message
        {
            MidiMessage message = MidiMessage::midiContinue();
            alphaLiveEngineRef.sendMidiMessage(message);
        }
        
        midiClockOutIsRunning = true;
    }
    
    //start the thread
    startThread(6);
    
    //update the GUI of the start/stop button. 
    //This is only really neccassery when auto started from pressing a pad - check for that here?
    //Here i need to LOCK the message thread and directly call notifyObs() instead of through the AsyncUpdater
    //due to the weirdness where using the hardware to trigger the clock would cause the button to not
    //be updated, but using alt-click would work fine! why?
    broadcaster.sendActionMessage("UPDATE TRANSPORT BUTTON");
    
    //tell GuiGlobalClock to update its display
    broadcaster.sendActionMessage("UPDATE CLOCK DISPLAY");
    
}

void GlobalClock::run()
{
    while ( ! threadShouldExit())
    {
        //When not syncing to an external MIDI clock,
        //or when syncing to an external clock but only for the start/stop controls
        if (midiClockValue != 3 || (midiClockValue == 3 && midiClockMessageFilter == 2))
        {
            //process the internal clock display and quantisation points
            
            if (Time::getMillisecondCounterHiRes() >= currentTime)
            {
                currentTime = currentTime + timeInterval;
                
                processClock();
            }
            
            //Sending MIDI Clock stuff
            if (midiClockOutIsRunning)
            {
                if (Time::getMillisecondCounterHiRes() >= midiClockCurrentTime)
                {
                    midiClockCurrentTime += midiClockTimeInterval;
                    
                    MidiMessage message = MidiMessage::midiClock();
                    alphaLiveEngineRef.sendMidiMessage(message);
                }
            }
        }
        
        //When syncing to an external MIDI clock and not filtering out clock messages
        else
        {
            sharedMemory.enter();
            
            if (midiClockMessageCounter >= 6)
            {
                midiClockMessageCounter = 0;
                
                processClock();
            }
            
            sharedMemory.exit();
        }
        
        
        
        //sleep the thread for 1ms
        wait(1); 
    }
}

void GlobalClock::processClock()
{

        //=====get current microbeat, beat, and bar numbers============
        if (microbeatNumber == 5) //1 beat
        {
            microbeatNumber = 1;
            beatNumber++;
            //tell GuiGlobalClock to update its beat Number display. 
            //Bar number updates according to beat number within GuiGlobalClock
            broadcaster.sendActionMessage("UPDATE CLOCK DISPLAY");
            
        }
        if (beatNumber >= beatsPerBar+1) //1 bar
        {
            beatNumber = 1;
            barNumber++;
        }
        if (barNumber == 5) 
        {
            barNumber = 1;
        }
        
        //=====send clock info to alphaLiveEngine and beyond based on quantization value=========
        switch (quantizationValue) 
        {
            case 1: //4 bars
                if (barNumber == 1 && beatNumber == 1 && microbeatNumber == 1)
                {
                    alphaLiveEngineRef.triggerQuantizationPoint();
                }
                break;
            case 2: //2 bars
                if ((barNumber == 1 || barNumber == 3) && beatNumber == 1 && microbeatNumber == 1)
                {
                    alphaLiveEngineRef.triggerQuantizationPoint();
                }
                break;
            case 3: //1 bar (default)
                if (beatNumber == 1 && microbeatNumber == 1)
                {
                    alphaLiveEngineRef.triggerQuantizationPoint();
                }
                break;
            case 5: //1 beat
                if (microbeatNumber == 1)
                {
                    alphaLiveEngineRef.triggerQuantizationPoint();
                }
                break;
            case 6: //half beat
                if (microbeatNumber == 1 || microbeatNumber == 3)
                {
                    alphaLiveEngineRef.triggerQuantizationPoint();
                }
                break;
            case 7: //quarter beat
                //trigger everytime
                alphaLiveEngineRef.triggerQuantizationPoint();
                break;
            default: //1 bar
                if (beatNumber == 1)
                {
                    alphaLiveEngineRef.triggerQuantizationPoint();
                }
                break;
        }
        
        //trigger metronome
        if (metronomeStatus == true)
        {
            if (beatNumber == 1 && microbeatNumber == 1) //the start of a bar
            {
                tickFileSource.setPosition (0.0);
                tickFileSource.start();
            }
            else if (microbeatNumber == 1) //the start of a beat
            {
                tockFileSource.setPosition (0.0);
                tockFileSource.start();
            }
        }
        
        //==================================================================
        
        //increment microbeat
        microbeatNumber++;

}

void GlobalClock::stopClock()
{
    if (StoredSettings::getInstance()->killOnClockStop == 2)
    {
        for (int i = 0; i <= 47; i++)
        {
            alphaLiveEngineRef.getModeSampler()->killPad(i);
            alphaLiveEngineRef.getModeSequencer()->killPad(i);
            alphaLiveEngineRef.getModeMidi()->killPad(i);
        }
    }
    
    stopThread(100);
    
    //MIDI Clock stuff
    if (midiClockOutIsRunning)
    {
        MidiMessage message = MidiMessage::midiStop();
        alphaLiveEngineRef.sendMidiMessage(message);
        
        midiClockOutIsRunning = false;
    }
    
    broadcaster.sendActionMessage("UPDATE TRANSPORT BUTTON TO OFF");
    
}

void GlobalClock::setMidiClockMessageTimestamp()
{
    //need to improve how things work here so the set tempo is more accurate.
    //also, when the clock is being 'driven' by Midi clock messages, sequences
    //should also be driven this way, somehow...
    
    sharedMemory.enter();
    
    midiClockTempos.set(midiClockMessageCounter, 2500 / (Time::getMillisecondCounterHiRes() - prevMidiClockMessageTimestamp));
    prevMidiClockMessageTimestamp = Time::getMillisecondCounterHiRes();
    midiClockMessageCounter++;
    
    if (midiClockMessageCounter >= 6)
    {
        //order the array
        for (int i = midiClockTempos.size()-1; i >= 0; i--)
        {
            if (midiClockTempos[i] < midiClockTempos[i-1])
            {
                  midiClockTempos.swap(i, i-1);
            }
        }
        
        //set tempo based on the median of the midiClockTempos array
        double newTempo = midiClockTempos[midiClockTempos.size()/2];
        
        //round tempo to 1 decimal place
        newTempo *= 10;
        newTempo = ceil(newTempo);
        newTempo = newTempo/10;
        
        //its probably worth changing the tempo less frequently than everytime a clock message is recieved,
        //and set the tempo to the most common value found here.
        
        AppSettings::Instance()->setGlobalTempo(newTempo);
        broadcaster.sendActionMessage("UPDATE TEMPO");
    }
    
    sharedMemory.exit();
}

void GlobalClock::prepareToPlay (int samplesPerBlockExpected,double sampleRate)
{
    audioMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void GlobalClock::releaseResources()
{
    audioMixer.releaseResources();
}
void GlobalClock::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
     audioMixer.getNextAudioBlock(bufferToFill);
}

void GlobalClock::setTempo (float value)
{
    tempo = value;
    timeInterval = double(15000.0)/tempo;
    
    midiClockTimeInterval = timeInterval / 6.0;
}
void GlobalClock::setBeatsPerBar (int value)
{
    beatsPerBar = value;
}
void GlobalClock::setQuantizationValue(int value)
{
    quantizationValue = value;
}
void GlobalClock::setMetronomeStatus(bool value)
{
    metronomeStatus = value;
}
void GlobalClock::setMidiClockValue (int value)
{
    midiClockValue = value;
}
void GlobalClock::setMidiClockStartMessage (int value)
{
    midiClockStartMessage = value;
}
void GlobalClock::setMidiClockMessageFilter (int value)
{
    midiClockMessageFilter = value;
}

int GlobalClock::getBeatNumber()
{
    return beatNumber;
}
int GlobalClock::getBarNumber()
{
    return barNumber;
}

void GlobalClock::setMainComponent(MainComponent *mainComponent_)
{
    mainComponent = mainComponent_;
}
