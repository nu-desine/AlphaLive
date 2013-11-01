//
//  AlphaLiveEngine.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 19/09/2011.
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

#ifndef H_ALPHALIVEENGINE
#define H_ALPHALIVEENGINE

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../My IO Classes/HidComms.h"
#include "MIDI Mode/ModeMidi.h"
#include "Sampler Mode/ModeSampler.h"
#include "Sequencer Mode/ModeSequencer.h"
#include "Controller Mode/ModeController.h"
#include "../My IO Classes/OscOutput.h"
#include "GlobalClock.h"
#include "../Audio Processing/PanControl.h"
#include "EliteControls.h"

class MainComponent;

class AlphaLiveEngine :     public AudioIODeviceCallback, //so this class handles the audio output
                            public HidComms,
                            public ActionListener

{
public:
    AlphaLiveEngine();
    ~AlphaLiveEngine();
    
    void hidInputCallback (int pad, int value, int velocity);
    void setDeviceType (int type); //1 - AlphaSphere, 2 - AlphaSphere elite
    void removeMidiOut();
    void updateFirmware();
    void setFirmwareUpdateStatus (bool status);
    void setDeviceStatus();
    void setFirmwareDetails (String version, String serial);
    
    void actionListenerCallback (const String& message);
    
    int getRecievedPad();
    int getRecievedValue();
    void playPadFromMouse (int pad, int value);
    
    MainComponent* getMainComponent();
    void setMainComponent(MainComponent *mainComponent_);
    
    AudioDeviceManager& getAudioDeviceManager();
    ModeMidi* getModeMidi(); //used by PadSettings to access the settings accessors
    ModeSampler* getModeSampler(); //used by PadSettings to access the ModeSampler instance
    ModeSequencer* getModeSequencer();  //used by PadSettings to access the ModeSequencer instance
                                        //also used to set the SequencerGrid instance as an observer to ModeSequencer
    ModeController* getModeController(); //used by SceneComponent to set the SceneComponent as an observer to ModeController
    
    void audioDeviceAboutToStart(AudioIODevice* device);
	void audioDeviceStopped();
	void audioDeviceIOCallback (const float** inputChannelData,
                                int totalNumInputChannels,
                                float** outputChannelData,
                                int totalNumOutputChannels,
                                int numSamples);
    
    
    void sendMidiMessage (MidiMessage midiMessage);
    void setMidiOutputDevice (int deviceIndex);
    
    void setGain (float value);
    void setPan (float value);
    
    void updatePadPlayingStatus (int padNumber, int status);
    
    void setIsDualOutputMode (bool value);
    void setOscIpAddress (String value);
    void setOscPortNumber (int value);
    
    //global clock/quantize stuff
    GlobalClock* getGlobalClock(); //used by AppSettings to set the tempo
    void triggerQuantizationPoint(); 
    void addPadToQueue (int padNum);
    void killQueuedExclusivePads (int padNum);
    
    //exclusive mode stuff
    void handleExclusiveMode (int padNum);
    
    void killAll();
    
    void setRecordingSequencerPadsState (int padNum, int state);
    Array<int> getRecordingPads();
    
    String getFirmwareVersion();
    String getDeviceSerial();
    
    bool getMidiChannelStatus (int channel);
    Array<int> getPreviouslyUsedMidiChannels();
    
    void latchPressureValue (int padNum, bool shouldLatch);
        
private:
    
    ModeMidi *modeMidi;
    ModeSampler *modeSampler;
    ModeSequencer *modeSequencer;
    ModeController *modeController;
    EliteControls *eliteControls;
    
    int recievedPad;
    float recievedValue;
    float recievedVelocity;
    
    //midi output device
    MidiOutput *midiOutputDevice; //is this actually needed?
    
    //audio related
	AudioDeviceManager audioDeviceManager;	// this wraps the actual audio device
    CriticalSection sharedMemory, sharedMemoryGui, sharedMemoryMidi;
    
    //audio stuff for mixing the modeSampler and modeSequencer objects, which are AudioSources
    MixerAudioSource audioMixer;
    AudioSourcePlayer audioPlayer;
    
    float gain, gainPrev, panLeft, panLeftPrev, panRight, panRightPrev;

    int playingStatus;
    
    OscOutput oscOutput;
    bool isDualOutputMode;
    String oscIpAddress;
    int oscPortNumber;
    
    //global clock/quantize stuff
    GlobalClock *globalClock;
    Array <int> queuedPads;
    
    Array <int> recordingPads;
    
    //if a pads exclusive mode is set to 1 (on), its padNumber needs to be added to an array
    //of an index relevant to the pads exclusive group number (index 0 = group 1, index 1 = group 2, 
    //index 2 = group 3 etc...). This will allow pads to be easily found and turned off when a new pad
    //of that group is triggered
    int currentExclusivePad[23];
    
    MainComponent *mainComponent;
    
    bool shouldUpdateFirmware;
    ActionBroadcaster broadcaster;
    
    Array <int> padPressureGuiQueue;
    int padPressure[48];
    int padVelocity[48];
    Array <int> eliteControlGuiQueue;
    int eliteControlValue[5];
    
    String firmwareVersion, deviceSerial;
    bool hasDisplayedNoMidiDeviceWarning;
    
    //auto MIDI Channel stuff
    //this stuff needs to be here and not within the MIDI Mode class
    //as we don't want auto channels to interfere with MIDI from Sequencer Mode
    bool isMidiChannelActive[16];
    Array<int> previouslyUsedMidiChannels;  //this array will always hold all 16 channels but
                                            //its order will changing depending on the order
                                            //of active channels - the oldest channel will always
                                            //be at the start of the array with latest played channel
                                            //being at the end.
    
    int minPressureValue[48];  //Used to store the start/unpressed pressure value.
                            //By default and for most of the time each index will equal 0.
                            //The values can only changed when pads pressure is 'latched'.
    
};

#endif // H_ALPHALIVEENGINE