//
//  ModeSequencer.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 25/09/2011.
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

#ifndef H_MODESEQUENCER
#define H_MODESEQUENCER

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "SequencePlayer.h"

class AlphaLiveEngine;

class ModeSequencer :   public AudioSource,
                        public Subject //so this class can notify observers
{
public:
    ModeSequencer(MidiOutput &midiOutput, AlphaLiveEngine &ref);
    ~ModeSequencer();
    
    void getOscData(int padNumber, int padValue);
    
    void createSequencePlayer (int padNumber);
    void deleteSequencePlayer (int padNumber);
    
    void updateSequencerGridGui (int columnNumber_, int sequencerNumber_, int whatShouldUpdateFlag_);
    
    //these three functions are accessed by the observers of this class to update the GUI accordingly
    //getCurrentColumn number gets the current column number of the currently selected (and displayed) sequence (from the relevent SequencerPlayer instance),
    //which is then called by the SequenceGrid instance to 'move' the play head across the grid 
    int getCurrentColumnNumber();
    //getCurrentSequenceNumber gets the current sequence number of the currently selected (and displayed) sequence (from the relevent SequencerPlayer instance),
    //which is then called by GuiSequencerMode to display the sequence (in a sequencer set) that is currently being played
    int getCurrentSequenceNumber();
    //getWhatShouldUpdateFlag is used as a flag to tell the obeservers what to do, called by the observers
    //if it returns 1 the playhead is told to be updated, if it returns 2 the sequencer number is told to update
    //the gui update implementation update needs to be handled like this to reduce CPU load of things being updated when they don't need to be
    int getWhatShouldUpdateFlag();
    
    //used by PadSettings to access the SequencePlayer instances
    SequencePlayer* getSequencePlayerInstance (int padNumber);
    
    void setTempo (float value);
    
    void updatePadPlayingStatus(int padNumber, int playingStatus);
    
    void killAll();
    
    //AudioSource Functions
	void prepareToPlay (int samplesPerBlockExpected,double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    //quantization stuff
    void triggerQuantizationPoint();
    void addItemToWaitingPadSequencer (SequencePlayer* item);
    
    void stopExclusivePadSequencer (int channel, SequencePlayer* item);
    
    void setMidiOutputDevice (MidiOutput &midiOutput);
    
private:
    
    OwnedArray<SequencePlayer> padSequencer;
    
    OwnedArray<SequencePlayer> currentExclusivePadSequencer;//holds any AudioFilePlayer objects currently playing which are of a channel
                                                            //set to exclusive mode. Index 0 will hold channel 1, index 1 channel 2 etc...
                                                            //This will allow looper pads to be easily found and turned off when a new pad
                                                            //of that channel are triggered
    
    MidiOutput *midiOutputDevice; //this isn't really needed here!
    
    //audio related
	MixerAudioSource audioMixer;
    ScopedPointer <TimeSliceThread> audioTransportSourceThread;
    //TimeSliceThread *audioTransportSourceThread;
    CriticalSection sharedMemory;
    
    int prevPadValue[48];
    
    int columnNumber;
    int sequenceNumber;
    int whatShouldUpdateFlag;
    
    float tempo;
    
    AlphaLiveEngine &alphaLiveEngineRef;
    
    //quantization stuff
    OwnedArray<SequencePlayer> waitingPadSequencer;
};



#endif