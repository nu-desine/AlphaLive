//
//  SequenceAudioFilePlayer.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 19/10/2011.
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

#ifndef H_SEQUENCEAUDIOFILEPLAYER
#define H_SEQUENCEAUDIOFILEPLAYER

#include "../../../JuceLibraryCode/JuceHeader.h"

class SequenceAudioFilePlayer : public AudioSource 
{
public:
    SequenceAudioFilePlayer(int padNumber_, int rowNumber_, TimeSliceThread *audioTransportSourceThread_);
    ~SequenceAudioFilePlayer();
    
    //AudioSource Functions
	void prepareToPlay (int samplesPerBlockExpected,double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    void playAudioFile (float gain);
    void stopAudioFile();
    
    void setAudioFile (File audioFile_);
    void addtoFileSourceArray (int arrayIndex, AudioFormatReader* reader_);
    
    void setAttackTime (double value);
    void setPolyphony (int value);
    
    
private:
    
    int padNumber, rowNumber;
    
    //audio related
	MixerAudioSource audioMixer;
	OwnedArray <AudioTransportSource> fileSource; //controls the playback of a positionable audio streams
	File currentFile;
    AudioFormatReaderSource* currentAudioFileSource;
    TimeSliceThread *audioTransportSourceThread;
    
    CriticalSection sharedMemory;
    
    double sampleRate_;
    
    double attackTime;
    int polyphony;
    
    bool isInAttack;
    int attackSamples;
    int attackPosition;
    bool isInStartRamp; //signifies that the audio file has just started and a short gain
                        //ramp needs to be applied to prevent potential clicking. This is
                        //only used if the user hasn't set a custom attack time.
    int startRampPosition;
    
    int nextFileSourceIndex;
 
};



#endif