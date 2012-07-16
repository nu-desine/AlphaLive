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
    
    
private:
    
    int padNumber, rowNumber;
    
    //audio related
	AudioTransportSource fileSource;	// this controls the playback of a positionable audio stream, handling the starting/stopping and sample-rate conversion
	File currentFile;
    AudioFormatReaderSource* currentAudioFileSource;
    TimeSliceThread *audioTransportSourceThread;

};



#endif