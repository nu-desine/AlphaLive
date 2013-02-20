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

/*
    HOW OSC IS ROUTED CORRECTLY THROUGH THE PROGRAM.
    When an OSC message is received in this program, it needs to do two things.
    Firstly it needs to trigger the audio/MIDI data attached to that pad,
    and secondly it needs show the OSC input visual by updating the corresponding
    GUI pad.
 
    OSC is received in the program using the class AlphaLiveEngine which inherits from the
    abstract base class OscInput. OscInput listens for OSC messages on it's own thread
    and whenver it receives an OSC message it calls the pure virtual oscCallBack function
    within AlphaLiveEngine.
 
    Within oscCallBack firstly it sets the values of the received pad number and pad value 
    to private member variables of AlphaLiveEngine. Secondly it sends a ActionBroadcaster message
    to call the actionListenerCallback function, which tells the program to update something
    in the GUI in the Message Thread when it has finished in the current (OSC) Thread.
    Wthout using this ActionListener implementation the GUI would be updated in the
    wrong thread, calling an Assertion error at runtime. Lastly, the program checks
    what mode is set to the received pad using the AppSettings Singleton object,
    and routes the OSC message to the correct mode's class in order to create the desired
    audio or MIDI data.
 
    Passing the OSC data to the mode classes is straight forward; the pad number and value  
    are simply passed into the correct objects as function arguments. However using the OSC messages
    to update the GUI is a lot more complex. The Observer Design Pattern has been used 
    so that each time an OSC message is received the GUI pad layout object is notified and
    told to update the corresponding GUI pad child object appearence. Using the Subject base class (within the 
    AbstractSubjectAndObserver file) AlphaLiveEngine is set as the 'subject' class, and the
    GUI pad layout object is set as the 'observer' using the Observer abstract base class.
    The GUI pad layout observer object then needs to be linked to the alphaLiveEngine subject object 
    by initialising it with a reference to the alphaLiveEngine object. As the alphaLiveEngine object
    is created within Main but the GUI pad objects are a few levels down the program
    hierarchy, a reference to alphaLiveEngine needs to be passed down through each object
    until it gets to the MainComponent which creates the GUI pad layout object.
    
    When an OSC message is received in AlphaLiveEngine (the 'subject') it calls oscCallBack, which intern 
    calls actionListenerCallback, which is used to call notifyObs. This function calls the
    update() function in the GUI pad layout class (the 'observer') which then gets the received pad
    number and value and updates the corresponding Gui pad object accordingly.
 
    (Previously, the actual Gui pad objects were the observers and within their update() function
    it would check if that particular instance matched the received pad number and if so update
    its appearance. This method was discarded as it meant everytime an OSC message was received 
    all the 48 gui pad objects would be notified but only 1 would be updated. The new method means
    that only 1 update() function will be called, reducing potential CPU usage)
    
 
 */

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
    
    void setMidiOutputDevice (int deviceIndex);
    MidiOutput* getMidiOutputDevice();
    
    void setGain (float value);
    void setPan (float value);
    
    int getGuiUpdateFlag();
    void setGuiUpdateFlag (int value);
    int getPadNumberForPlayingStatus();
    int getPlayingStatus();
    
    void updatePadPlayingStatus (int padNumber, int status);
    
    void setIsDualOutputMode (bool value);
    void setOscIpAddress (String value);
    void setOscPortNumber (int value);
    
    //global clock/quantize stuff
    GlobalClock* getGlobalClock(); //used by AppSettings to set the tempo
    void triggerQuantizationPoint(); 
    void addPadToQueue (int padNum);
    
    //exclusive mode stuff
    void handleExclusiveMode (int padNum);
    
    void killAll();
    
    void setRecordingSequencerPadsState (int padNum, int state);
    Array<int> getRecordingPads();
        
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
    CriticalSection sharedMemory, sharedMemoryGui;
    
    //audio stuff for mixing the modeSampler and modeSequencer objects, which are AudioSources
    MixerAudioSource audioMixer;
    AudioSourcePlayer audioPlayer;
    
    float gain, gainPrev, panLeft, panLeftPrev, panRight, panRightPrev;
    
    int pendingUpdatePadValue;
    
    //guiUpdateFlag is accessed by the observer class (GuiPadLayout) so it knows what part of the GUI it should be updating.
    // 0 = pad gradient/pressure (default)
    // 1 = pad playing status signifier
    int guiUpdateFlag;
    
    int padNumberForPlayingStatus;
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
    
};

#endif // H_ALPHALIVEENGINE