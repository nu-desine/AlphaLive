//
//  GlobalClock.h
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

/*
 This is a Thread class that handles the global clock for triggering loops and sequences (and probably more eventually).
 Preferably AlphaLiveEngine should handle the thread however it already contains a thread that listens for input messages (as it derives
 from HidComms) so it couldn't be implemented there (as far as I tried).
 There will be an instance of this class within AlphaLiveEngine - is this the best class relationship?
 */

#ifndef H_GLOBALCLOCK
#define H_GLOBALCLOCK

#include "../../JuceLibraryCode/JuceHeader.h"

class AlphaLiveEngine;
class MainComponent;

class GlobalClock :     public Thread,
                        public AudioSource,
                        public ActionListener
{
public:
    GlobalClock(AlphaLiveEngine &ref);
    ~GlobalClock();
    
    void actionListenerCallback (const String& message);
    
    //Thread callback function
    void run();
    
    void startClock();
    void stopClock();
    void processClock();
    
    void setMidiClockMessageTimestamp();
    
    //AudioSource Functions
	void prepareToPlay (int samplesPerBlockExpected,double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    void setTempo (float value);
    void setBeatsPerBar (int value);
    void setQuantizationValue (int value);
    void setMetronomeStatus (bool value);
    
    void setMidiClockValue (int value);
    void setMidiClockStartMessage (int value);
    void setMidiClockMessageFilter (int value);
    
    
    int getBeatNumber();
    int getBarNumber();
    
    void setMainComponent(MainComponent *mainComponent_);
    
    void sendLedClockMessage (uint8 messageType);
    
private:
    
    double currentTime;
    double tempo;
    double timeInterval;
    
    AlphaLiveEngine &alphaLiveEngineRef;
    
    int beatsPerBar;
    int microbeatNumber, beatNumber, barNumber;
    int quantizationValue;
    bool metronomeStatus;
    
    int midiClockValue, midiClockStartMessage, midiClockMessageFilter;
    
    //audio related
    MixerAudioSource audioMixer;
	AudioTransportSource tickFileSource, tockFileSource;
    ScopedPointer <AudioFormatReaderSource> tickAudioFormatReaderSource, tockAudioFormatReaderSource;
    ScopedPointer <TimeSliceThread> audioTransportSourceThread;
    
    ActionBroadcaster broadcaster;
    MainComponent *mainComponent;
    
    bool midiClockOutIsRunning, midiClockInIsRunning;
    double midiClockTimeInterval, midiClockCurrentTime;
    double prevMidiClockMessageTimestamp;
    Array <double> midiClockTempos;
    int midiClockMessageCounter;
    
    CriticalSection sharedMemory;
};

#endif //H_GLOBALCLOCK