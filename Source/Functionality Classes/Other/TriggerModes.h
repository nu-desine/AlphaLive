//
//  TriggerModes.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 20/07/2012.
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

#ifndef H_TRIGGERMODEDATA
#define H_TRIGGERMODEDATA

//struct that is used by trigger modes to return the necessary data
struct TriggerModeData 
{
    int playingStatus; // 0 = stop, 1 = play, 2 = ignore
    int pressureValue; //do we need this now? as sticky isn't now contain with trigger modes the incoming pressure will always be the outgoing pressure
    bool shouldLoop; //don't need this now as looping is defined with trigger modes
    
    //special variables for the cycle and auto-cyle play states
    bool ignorePressure; //is this variable needed? in each playstate where pressure is ignored, the pressure value is set to 0 each time - is this adequate enough?
    bool moveToNextSeq;
    bool isLinearCycle;
    bool isAutoCycle;
    
};

#endif

//================================================================================
//=TRIGGER MODES==================================================================
//================================================================================
/*  Each of the following functions alters the currentState variable in AppSettings
 of the padNumber parameter in question by using padValue parameter. The bool returned
 determines whether the play state causes the audio to loop.
 
 To start audio, set the currentState varialble to 1, or set to 0 to stop the audio.
 
 HOW TO ADD A TRIGGER MODES TO THE APPLICATION:
 [add instruuctions here...]
 
 */
//================================================================================

#ifndef H_TRIGGERMODES
#define H_TRIGGERMODES

#include "../../../JuceLibraryCode/JuceHeader.h"

class TriggerModes
{
public:
    TriggerModes();
    ~TriggerModes();
    
    TriggerModeData hold (int padValue);
    TriggerModeData toggle (int padValue);
    TriggerModeData toggleRelease (int padValue);
    TriggerModeData latch (int padValue);
    TriggerModeData latchMax (int padValue);
    TriggerModeData Trigger (int padValue);
    
    //sequencer-only trigger modes
    TriggerModeData cycle (int padValue);
    TriggerModeData autoCycle (int padValue);
    
    /*
    PlayStateData playOnce (int padValue);
    PlayStateData playOnceContinuous (int padValue);
    PlayStateData loop (int padValue);
    PlayStateData togglePressOff (int padValue);
    PlayStateData toggleReleaseOff (int padValue);
    PlayStateData sticky (int padValue);
    PlayStateData latchMaxLatch (int padValue);
    PlayStateData latchPressLatch (int padValue);
    PlayStateData triggerLooped (int padValue);
    PlayStateData triggerNonLooped (int padValue);
    
    //Sequencer-only play states
    PlayStateData cycleLooped (int padValue);
    PlayStateData cycleLinear (int padValue);
    PlayStateData autoCycleLooped (int padValue);
    PlayStateData autoCycleLinear (int padValue);
    
    //new playstates
    PlayStateData playOnceNonDestructive (int padValue);
    PlayStateData triggerLoopedNonDestructive (int padValue);
    PlayStateData triggerNonLoopedNonDestructive (int padValue);
    PlayStateData cycleLinearNonDestructive (int padValue);
    PlayStateData toggleNonDestructive (int padValue);
     */
    
    void reset();
    
    
private:
    int pressCount; //not really COUNTING presses - what is it doing? 1 after intiall press?
    int releaseCount; //not really COUNTING releases - what is it doing? 1 after intial release?
    int maxPressCount; //1 when looping and waiting for a max press to stop
    int rePressCount; //what is this actually being used for? 1 when pressed and 0 when released?
    
    int currentPlayingStatus;
    TriggerModeData triggerModeData;
    
};

#endif