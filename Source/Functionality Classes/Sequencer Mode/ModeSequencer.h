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
                        public Subject, //so this class can notify observers
                        public Thread
{
public:
    ModeSequencer (AlphaLiveEngine &ref);
    ~ModeSequencer();
    
    void getInputData(int padNumber, int padValue);
    
    //Thread callback function
    void run();
    void editRunningSequencersArray (int action, int padNumber); //Action: 1 - add object, 0 - delete object
    
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
    
    void killPad (int padNum);
    void stopPrevExclusivePad (int padNum);
    
    //preview sequence stuff (called directly from guiSequencerMode)
    void previewSequence (int padNum, int status);
    void setPreviewSequenceNumber (int padNum, int sequenceNumber);
    void stopLastPreviewedSequence();
    
    //AudioSource Functions
	void prepareToPlay (int samplesPerBlockExpected,double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    //quantization stuff
    void triggerQuantizationPoint (int padNum);
    
    AlphaLiveEngine& getAlphaLiveEngineRef();
    
private:
    
    OwnedArray<SequencePlayer> padSequencer;
    Array <SequencePlayer*> runningSequencers;
    
    //audio related
	MixerAudioSource audioMixer;
    ScopedPointer <TimeSliceThread> audioTransportSourceThread;
    CriticalSection sharedMemory;
    
    int prevPadValue[48];
    
    int columnNumber;
    int sequenceNumber;
    int whatShouldUpdateFlag;
    
    float tempo;
    
    int lastPreviewedSequencePadNum;
    
    AlphaLiveEngine &alphaLiveEngineRef;
};



#endif