//
//  SequencePlayer.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 18/10/2011.
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

#ifndef H_SEQUENCEPLAYER
#define H_SEQUENCEPLAYER

//how should I handle playing the seperate audio files in here?

#include "../JuceLibraryCode/JuceHeader.h"
#include "SequencerValues.h"
#include "../Other/TriggerModes.h"
#include "SequenceAudioFilePlayer.h"
#include "../../Application/AbstractSubjectAndObserver.h"
#include "../../Audio Processing/PanControl.h"
#include "../../Audio Processing/DSP Effects/GainAndPan.h"
#include "../../Audio Processing/DSP Effects/LowpassFilter.h"
#include "../../Audio Processing/DSP Effects/HighPassFilter.h"
#include "../../Audio Processing/DSP Effects/BandPassFilter.h"
#include "../../Audio Processing/DSP Effects/Delay.h"
#include "../../Audio Processing/DSP Effects/Reverb.h"
#include "../../Audio Processing/DSP Effects/Flanger.h"
#include "../../Audio Processing/DSP Effects/Tremolo.h"
#include "../../Audio Processing/PanControl.h"

class ModeSequencer;

class SequencePlayer :  public Thread,
                        public AudioSource,
                        public ActionListener
{
public:
    SequencePlayer(int padNumber_, MidiOutput &midiOutput, ModeSequencer &ref, TimeSliceThread *audioTransportSourceThread_);
    ~SequencePlayer();
    
    void processSequence(int padValue);
    void actionListenerCallback (const String& message);
    
    //Midi stuff
    void triggerMidiMessage(int rowNumber, int velocity);
    void triggerMidiNoteOffMessage (int rowNumber);
    void sendMidiPressureData();
    void sendMidiMessage(MidiMessage midiMessage);
    
    
    //Audio stuff
    void triggerAudioMessage(int rowNumber, int velocity);
	void prepareToPlay (int samplesPerBlockExpected,double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    void setMidiOutputDevice (MidiOutput &midiOutput);
    
    
    //Thread callback function
    void run();
    
    void stopThreadAndReset();
    
    int getCurrentPlayingState();
    
    //settings stuff
    void setTempo (float value);
    void setSequenceData (int seq, int row, int column, int value);
    void setMode (int value);
    void setNumberOfSequences (int value);
    void setTriggerMode (int value);
    void setShouldLoop (int value);
    void setIndestructible (int value);
    void setShouldFinishLoop (int value);
    void setSticky (int value);
    void setSequenceLength (int value);
    void setQuantizeMode (int value);
    void setDynamicMode (int value);
    
    void setMidiNote (int row, int value);
    void setMidiChannel (int value);
    void setMidiVelocity (int value); //NO LONGER NEED THIS
    void setMidiNoteLength (int value);
    void setMidiMinRange (int value);
    void setMidiMaxRange (int value);
    void setMidiControllerNumber (int value);
    void setMidiPressureMode (int value);
    void setMidiPressureStatus (bool value);
    
    void setSamplesEffect (int value);
    void setSamplesAudioFile (int row, File sample);
    void setSamplesGain (float value);
    void setSamplesPan (float value);
    
    void setRelativeTempoMode (int value);
    double getTimeInterval();
    
    void setRecordEnabled (bool value);
    
    //called when previewing sequence
    void setSequenceNumber (int value);
    
    //called when recording from other pads
    int getSequenceNumber();
    int getColumnNumber();
    
    //quantization stuff
    void triggerQuantizationPoint();
    
    GainAndPan& getGainAndPan();
    LowpassFilter& getLowpassFilter();
    HighPassFilter& getHighPassFilter();
    BandPassFilter& getBandPassFilter();
    Delay& getDelay();
    ReverbClass& getReverb();
    Flanger& getFlanger();
    Tremolo& getTremolo();

    
private:
    int padNumber;
    int prevPadValue, pressureValue;
    int columnNumber;
    
    double currentTime;
    int currentPlayingState;
    
    ActionBroadcaster broadcaster;
    ModeSequencer &modeSequencerRef;
    
    //create an object of the TriggerModes Class and a variable of the TriggerModeData struct
    TriggerModes triggerModes;
    TriggerModeData triggerModeData;
    
    //midi stuff
    MidiOutput *midiOutputDevice;
	
	//audio related
	SequenceAudioFilePlayer *sequenceAudioFilePlayer[NO_OF_ROWS];
    CriticalSection sharedMemory;
    TimeSliceThread *audioTransportSourceThread;
    GainAndPan *gainAndPan;
    LowpassFilter *lowPassFilter;
    HighPassFilter *highPassFilter;
    BandPassFilter *bandPassFilter;
    Delay *delay;
    ReverbClass *reverb;
    Flanger *flanger;
    Tremolo *tremolo;
    float sampleRate_;
    
    //audio stuff for mixing the SequenceAudioFilePlayer objects
	MixerAudioSource audioMixer;
	AudioSourcePlayer audioPlayer;
    
    int midiNoteOffTime[NO_OF_SEQS][NO_OF_ROWS][NO_OF_COLUMNS];
    int midiNoteOnCounter;
    
    int sequenceNumber; // holds the sequence number determined by the pressure - 0-7
    int prevSequenceNumber;

    //settings stuff
    double tempo;
    int relativeTempoMode;
    double timeInterval;
    int sequenceData[NO_OF_SEQS][NO_OF_ROWS][NO_OF_COLUMNS];
    int mode;
    int numberOfSequences;
    int triggerMode;
    int shouldLoop, indestructible, shouldFinishLoop, sticky;
    int sequenceLength;
    int quantizeMode;
    int dynamicMode;
    
    int midiNote[NO_OF_ROWS];
    int midiChannel;
    int midiVelocity; //NO LONGER NEED THIS
    int midiNoteLength;
    int midiMinRange;
    int midiMaxRange;
    int midiControllerNumber;
    int midiPressureMode;
    bool midiPressureStatus;
    
    float gain, gainPrev, panLeft, panLeftPrev, panRight, panRightPrev;
    int effect;
    bool recordEnabled;
    
    bool playingLastLoop;
};


#endif