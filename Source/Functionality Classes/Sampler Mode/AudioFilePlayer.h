//
//  AudioFilePlayer.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 29/09/2011.
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

#ifndef H_AUDIOFILEPLAYER
#define H_AUDIOFILEPLAYER

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Other/PlayStates.h"
#include "../../Audio Processing/DSP Effects/GainAndPan.h"
#include "../../Audio Processing/DSP Effects/LowpassFilter.h"
#include "../../Audio Processing/DSP Effects/HighPassFilter.h"
#include "../../Audio Processing/DSP Effects/BandPassFilter.h"
#include "../../Audio Processing/DSP Effects/Delay.h"
#include "../../Audio Processing/DSP Effects/Reverb.h"
#include "../../Audio Processing/DSP Effects/Flanger.h"
#include "../../Audio Processing/DSP Effects/Tremolo.h"
#include "../../Audio Processing/PanControl.h"


class ModeLooper;


class AudioFilePlayer : public AudioSource,
                        public ActionListener
{
public:
    AudioFilePlayer(int looperPadNumber, ModeLooper &ref, TimeSliceThread* audioTransportSourceThread_); //DO I NEED THE MODELOOPER REF ANYMORE?
    ~AudioFilePlayer();
    
    //AudioSource Functions
	void prepareToPlay (int samplesPerBlockExpected,double sampleRate);
	void releaseResources();
	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    void actionListenerCallback (const String& message);
    
    void processAudioFile(int padValue);
    void playAudioFile();
    void stopAudioFile();
    
    void setPlayState (int value);
    void setEffect (int value);
    void setQuantizeMode (int value);
    void setChannel (int value);
    
    int getCurrentPlayingState();
    bool getAudioTransportSourceStatus();
    
    void setAudioFile (File audioFile_);
    
    //audio signal processing stuff
    void setGain (float value);
    void setPan (float value);

    GainAndPan& getGainAndPan();
    LowpassFilter& getLowpassFilter();
    HighPassFilter& getHighPassFilter();
    BandPassFilter& getBandPassFilter();
    Delay& getDelay();
    ReverbClass& getReverb();
    Flanger& getFlanger();
    Tremolo& getTremolo();
    
    void setPlaybackPosition();
    
    //quantization stuff
    void triggerQuantizationPoint();
    
    void killAllAudio(); //kills things like delay effect tails

    
private:
	
	//audio related
	AudioTransportSource fileSource; //this controls the playback of a positionable audio stream, handling the starting/stopping and sample-rate conversion
	File currentFile;
    AudioFormatReaderSource* currentAudioFileSource;
    TimeSliceThread *audioTransportSourceThread; //this can't be a ScopedPointer as it will be equal to the scoped pointer within ModeLooper, so when deleting an instance of AudioFilePlayer it gets complicated as things will automatically try to be deleted when they might not exist anymore!
    
    CriticalSection sharedMemory;
    
    int padNumber;
    int prevPadValue;
    float sampleRate_;
    
    //create an object of the PlayStates Class and a variable of the PlayStateData struct
    PlayStates playStates;
    PlayStateData playStateData;
    
    ModeLooper &modeLooperRef;
    
    
    //settings
    int playState, effect;
    int currentPlayingState; //0 - off, 1 - playing, 2 - waiting to play, 3 - waiting to stop 
    int currentPressureValue;
    int quantizeMode;
    int channel;
    
    //audio signal processing stuff - now dynamically created and deleted when needed
    float gain, gainPrev, panLeft, panLeftPrev, panRight, panRightPrev;
    GainAndPan *gainAndPan;
    LowpassFilter *lowPassFilter;
    HighPassFilter *highPassFilter;
    BandPassFilter *bandPassFilter;
    Delay *delay;
    ReverbClass *reverb;
    Flanger *flanger;
    Tremolo *tremolo;
    
    ActionBroadcaster broadcaster;
    
    //playback manipulation stuff
    double fileStartPosition, fileEndPosition, currentPositionInRegion;
    int prevPressureRegion;
};

#endif