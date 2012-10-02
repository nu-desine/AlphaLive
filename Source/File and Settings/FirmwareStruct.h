//
//  FirmwareStruct.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 20/08/2012.
//  Copyright 2012 nu desine.
//

// This file contains 5 structs.

// The top three are mode specific structs.

// The 4th struct (padSettings) is for all the settings of a single 
// pad which includes the first 3 mode structs.

// The final struct (settings) is the main struct which includes
// 48 instances for the padSettings struct as well as global settings.


struct mode_midi
{
    int channel; //1-16
    int note; //0-127
    int minPressureRange; //0-127
    int maxPressureRange; //0-127
    enum triggerModes {standard = 1, toggle, latch, trigger};
    enum pressureModes {polyAT = 1, channelAT, modWheel, ccMessage, pitchBendUp, pitchBendDown};
    int ccNumber; //0-127
    bool indestructible;
    bool sticky;
    bool pressureStatus;
    bool noteStatus;
    
};



struct mode_sequencer
{
    enum modes {midi = 1, audio};    //Should the firmware 'drive' the audio sequences? Or should it
                                    //just view mode '2' as off?
    int sequenceData[8][12][32];    //0-127. [sequence][row][column]. 
                                    //0 = no note, 1-127 = note with given velocity
    int numberOfSequences; //1-8
    int sequenceLength //1-32
    enum triggerModes {standard = 1, toggle, latch, trigger, cycle, autoCycle};
    bool shouldLoop;
    bool shouldFinishLoop;
    bool indestructible;
    bool sticky;
    
    enum relativeTempo {quarterTime = -2, halfTime, regular, doubleTime, quadrupalTime};  
    // OR....
    double relativeTempo;
    
    bool dynamicMode;   //If true, the pressure is used to alter which sequence is playing
                        //as well as the default pressure functionality.
    
    int channel; //1-16
    int note[12]; //0-127. Note for each row
    int noteLength; //1-32
    int minPressureRange; //0-127
    int maxPressureRange; //0-127
    enum pressureModes {channelAT = 1, modWheel, ccMessage, pitchBendUp, pitchBendDown};
    int ccNumber; //0-127
    bool pressureStatus;
    
};



struct mode_controller
{
    enum modes {sceneChange = 1, midiProgramChange, sceneAndProgramChange, oscOut}; 
                //Firmware will only be needed for modes 
                //2 and 3 to send MIDI program change messages.
   
    int programChangeNumber; //0-127
    int channel; //1-16
};



struct pad_settings 
{
    struct mode_midi modeMidi;
    struct mode_sequencer modeSequencer;
    struct mode_controller modeController;
    
    enum modes {off = 0, midi, sampler, sequencer, controller}; 
    //Firmware will be needed for modes  1, 3 and 4
    
    bool exclusiveMode; //If true, the pad is given a group number (defined below)
                        //Exclusive mode means that only a single pad of said group
                        //can be playing at any time.
    int exclusiveGroup; //1-24
    bool quantizeMode;
    int pressureCurve;  //A menu of items to change the pressure mapping.
                        //Not sure on range yet.
    int velocityCurve;  //A menu of items to change the velocity mapping.
                        //Note sure on range yet.
    int staticVelocity; //0-127.  
    
};



struct settings
{
    struct pad_settings padSettings[48];
    
    float tempo; //60-360, maybe 20-360.
    int quantizationValue; //1-6 (1/4, 1/2, 1 beat, 1, 2, 4 bars)
    int beatsPerBar; //2-7
    bool autoStartClock; //If true, clock starts on first pad press.
    

};


/*
 Realtime data from firmware to software:
 - pressed pad number
 - pressed pad pressure value
 - pad state (on or off)
 - some form of timing/sync/tick data
 
 Realtime data from software to firmware:
 - Start pad
 - Stop pad
 - some form of timing/sync/tick data??
 */



