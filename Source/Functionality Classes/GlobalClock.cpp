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
#include "../GUI Classes/Binary Data/BinaryDataNew.h"
#include "../GUI Classes/Views/MainComponent.h"

GlobalClock::GlobalClock(AlphaLiveEngine &ref) 
            :   Thread("Global Clock"),
                alphaLiveEngineRef(ref)

                            
{
    tempo = AppSettings::Instance()->getGlobalTempo();
    timeInterval = (double(15000.0)/tempo);
    beatsPerBar = AppSettings::Instance()->getBeatsPerBar();
    quantizationValue = AppSettings::Instance()->getQuantizationValue();
    metronomeStatus = AppSettings::Instance()->getMetronomeStatus();
    
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
    
    AudioFormatReader* reader = wavFormat.createReaderFor(new MemoryInputStream (BinaryDataNew::tock_wav,
                                                                                 BinaryDataNew::tock_wavSize,
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
    
    AudioFormatReader* reader2 = wavFormat.createReaderFor(new MemoryInputStream (BinaryDataNew::tick_wav,
                                                                                 BinaryDataNew::tick_wavSize,
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


void GlobalClock::run()
{
    microbeatNumber = beatNumber = barNumber = 1;
    //tell GuiGlobalClock to update its display
    broadcaster.sendActionMessage("UPDATE CLOCK DISPLAY");
    
    //set the time that the thread loop will intially sleep for
    currentTime = Time::getMillisecondCounter() + int(timeInterval);
    //get the decimal value of timeInterval
    float timeIntervalDecimalOffset = timeInterval - int(timeInterval);
    //set the currently stored offset value (this will change each time the loop is iterated)
    float currentOffset = timeIntervalDecimalOffset;
    
     //====================thread loop starts here!====================================
     while( ! threadShouldExit())
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
                     
         //==================================================================
         //== SLEEP LOOP HERE!!! ============================================
         //==================================================================
         //sleep for int(timeInterval)
         Time::waitForMillisecondCounter(currentTime);
         
         //SHOULD THE BELOW SECTION OF CODE UP UNTIL currentTime = ... BE ABOVE THE ABOVE SLEEP CALL SO THAT THE REINITIALISATION OF
         //CURRENTTIME IS SET WITH A SMALLER POSSIBLE DELAY?
         //add the decimal offset of the time interval to the currently stored offset
         currentOffset = currentOffset + timeIntervalDecimalOffset;
         //get the offset that needs to be added. This will only make a difference if over than 1, otherwise it will equal 0.
         int currentOffsetToAdd = int(currentOffset);
         //change the value of currentTime so the next interation of the loop sleeps for the correct time,
         //possibly adding a value of 1 (currentlyOffsetToAdd) to make up for decimal values being ignored
         currentTime = currentTime + int(timeInterval) + currentOffsetToAdd;
         //if currentOffsetToAdd >= 1, remove from currentOffset as the value of 1 would have been added to currentTime
         if (currentOffsetToAdd >= 1)
             currentOffset = currentOffset-currentOffsetToAdd;
     
     }
    
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
}


void GlobalClock::startClock()
{
    //update the GUI of the start/stop button. 
    //This is only really neccassery when auto started from pressing a pad - check for that here?
    //Here i need to LOCK the message thread and directly call notifyObs() instead of through the AsyncUpdater
    //due to the weirdness where using the hardware to trigger the clock would cause the button to not
    //be updated, but using alt-click would work fine! why?
    broadcaster.sendActionMessage("UPDATE TRANSPORT BUTTON");
    
    //start the thread
    startThread(6);
    
    
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
    
    stopThread(200);
    
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
