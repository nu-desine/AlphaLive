//
//  TriggerModes.cpp
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

#include "TriggerModes.h"

TriggerModes::TriggerModes()
{
    pressCount = releaseCount = maxPressCount = currentPlayingStatus = rePressCount = 0;
    
    triggerModeData.playingStatus = 2;
    triggerModeData.pressureValue = 0;
    triggerModeData.shouldLoop = true;
    triggerModeData.moveToNextSeq = false;
    triggerModeData.ignorePressure = false;
    triggerModeData.isLinearCycle = false;
    triggerModeData.isAutoCycle = false;
    
}


TriggerModes::~TriggerModes()
{
    
}


TriggerModeData TriggerModes::hold (int padValue)
{
    //press to play
    //release to stop
    
    if (currentPlayingStatus == 0 && padValue > 0)
    {
        triggerModeData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
        releaseCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        triggerModeData.playingStatus = 0;
        currentPlayingStatus = 0;
        pressCount = 0;
        releaseCount = 0;
    }
    else
    {
        triggerModeData.playingStatus = 2; //ignore
    }
    
    rePressCount = 0;
    maxPressCount = 1;
    
    //triggerModeData.pressureValue = padValue;    
    //triggerModeData.shouldLoop = true;
    triggerModeData.ignorePressure = false;
    triggerModeData.moveToNextSeq = false;
    triggerModeData.isLinearCycle = false;
    triggerModeData.isAutoCycle = false;
    
    return triggerModeData;
}

TriggerModeData TriggerModes::toggle (int padValue)
{
    
}

TriggerModeData TriggerModes::toggleRelease (int padValue)
{
    
}

TriggerModeData TriggerModes::latch (int padValue)
{
    
}

TriggerModeData TriggerModes::latchMax (int padValue)
{
    
}

TriggerModeData TriggerModes::Trigger (int padValue)
{
    
}

//sequencer-only trigger modes
TriggerModeData TriggerModes::cycle (int padValue)
{
    
}

TriggerModeData TriggerModes::autoCycle (int padValue)
{
    
}


void TriggerModes::reset()
{
    pressCount = 0;
    releaseCount = 0;
    maxPressCount = 0;
    rePressCount = 0;
    currentPlayingStatus = 0;
    
    triggerModeData.playingStatus = 0;
    triggerModeData.pressureValue = 0;
    triggerModeData.shouldLoop = true;
    triggerModeData.moveToNextSeq = false;
    triggerModeData.ignorePressure = false;
    triggerModeData.isLinearCycle = false;
    
}


/*
PlayStateData PlayStates::playOnce (int padValue)
{
    //play when pressed
    //stop when released
    //don't loop
    
    if (currentPlayingStatus == 0 && padValue > 0)
    {
        playStateData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
        releaseCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        playStateData.playingStatus = 0;
        currentPlayingStatus = 0;
        pressCount = 0;
        releaseCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    rePressCount = 0;
    maxPressCount = 1;
    
    playStateData.pressureValue = padValue;
    playStateData.shouldLoop = false;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    
    return playStateData;
}

PlayStateData PlayStates::playOnceContinuous (int padValue)
{
    //play when pressed
    //stop when pressed a second time
    //don't loop
    //if reaches end of loop, set currentPlayingStatus to off
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0) 
    {
        playStateData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        releaseCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue > 0 && pressCount == 1 && releaseCount == 1) 
    {
        playStateData.playingStatus = 0;
        currentPlayingStatus = 0;
        pressCount = 0;
    }
    else if(currentPlayingStatus == 0 && padValue == 0 && pressCount == 0 && releaseCount == 1)
    {
        releaseCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1;
    rePressCount = 0;
    
    playStateData.pressureValue = padValue;
    playStateData.shouldLoop = false;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
    
}


PlayStateData PlayStates::loop (int padValue)
{
    //play when pressed
    //stop when released
    //loop
    if (currentPlayingStatus == 0 && padValue > 0)
    {
        playStateData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
        releaseCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        playStateData.playingStatus = 0;
        currentPlayingStatus = 0;
        pressCount = 0;
        releaseCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    rePressCount = 0;
    maxPressCount = 1;
    
    playStateData.pressureValue = padValue;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}



PlayStateData PlayStates::togglePressOff (int padValue)
{
    //play when pressed
    //stop when pressed a second time
    //loop
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0)
    {
        playStateData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        releaseCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue > 0 && releaseCount == 1 && pressCount >= 1) 
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        pressCount = 0;
    }
    else if(currentPlayingStatus == 0 && padValue == 0 && releaseCount == 1 && pressCount == 0)
    {
        releaseCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1; 
    rePressCount = 0;
    
    playStateData.pressureValue = padValue;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
    
}

PlayStateData PlayStates::toggleReleaseOff (int padValue)
{
    //play when pressed
    //stop when released a second time
    //loop
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0)
    {
        currentPlayingStatus = 1;
        playStateData.playingStatus = 1;
        pressCount = 1;
        
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && releaseCount == 0 && pressCount == 1)
    {
        releaseCount = 1;
        
    }
    else if(currentPlayingStatus == 1 && padValue > 0 && releaseCount == 1 && pressCount == 1)
    {
        pressCount = 2;
        
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount >= 2)
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        releaseCount = 0;
        pressCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1;
    rePressCount = 0;
    
    playStateData.pressureValue = padValue;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
    
}

PlayStateData PlayStates::sticky (int padValue)
{
    //play when presssed
    //stop when pressed a second time
    //loop
    //pressure value sticks to highest entered value till released
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0)
    {
        playStateData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
    }
    
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        releaseCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue > 0 && pressCount >= 1 && releaseCount == 1) 
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        pressCount = 0;
    }
    else if(currentPlayingStatus == 0 && padValue == 0 && releaseCount == 1 && pressCount == 0)
    {
        releaseCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    
    //get modifed pressure value
    if(padValue == 0 && pressCount == 0)
    {
        playStateData.pressureValue = 0;
    }
    else
    {
        int prevPressure = playStateData.pressureValue;
        
        if(padValue < prevPressure)
            padValue = prevPressure; //don't change value
        
        playStateData.pressureValue = padValue;
    }
    
    maxPressCount = 1;
    rePressCount = 0;
    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}

PlayStateData PlayStates::latchMaxLatch (int padValue)
{
    //play when pressed
    //stop when released
    //loop
    //if pressure is set to max value, ignore any releases until pressed to max value again
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0) 
    {
        currentPlayingStatus = 1;
        playStateData.playingStatus = 1;
        pressCount = 1;
        maxPressCount = 0;
    }
    else if(padValue == 511 && maxPressCount == 0)
    {
        maxPressCount = 1;
    }
    else if(padValue == 511 && maxPressCount == 1)
    {
        maxPressCount = 0;
    }
    else if (currentPlayingStatus == 1 && padValue == 0 && maxPressCount == 0)
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        releaseCount = 0;
        pressCount = 0;
    }
    
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1) 
    {
        releaseCount = 1;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    
    rePressCount = 0;
    
    playStateData.pressureValue = padValue;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
    
    
}

PlayStateData PlayStates::latchPressLatch (int padValue)
{
    //play when pressed
    //stop when released
    //loop
    //ignore any releases until pressure is set to max value
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0) 
    {
        currentPlayingStatus = 1;
        playStateData.playingStatus = 1;
        pressCount = 1;
        maxPressCount = 1;
    }
    else if(padValue == 511 && maxPressCount == 1)
    {
        maxPressCount = 0;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && maxPressCount == 0) 
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        releaseCount = 0;
        pressCount = 0;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1) 
    {
        releaseCount = 1;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    rePressCount = 0;
    
    playStateData.pressureValue = padValue;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
    
}

PlayStateData PlayStates::triggerLooped (int padValue)
{
    //play when pressed
    //stop when pressed to max value (and then released?)
    //loop
    //retrigger when pressed again
    if (padValue > 0 && rePressCount == 0)
    {
        currentPlayingStatus = 1;
        playStateData.playingStatus = 1;
        pressCount = 1;
        rePressCount = 1;
        maxPressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1 && maxPressCount == 1)
    {
        releaseCount = 1;
        rePressCount = 0;
        
    }
    else if(padValue == 511)
    {
        maxPressCount = 0;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && maxPressCount == 0)
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        pressCount = 0;
        releaseCount = 0;
        rePressCount = 0;
        
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    
    playStateData.pressureValue = padValue;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}

PlayStateData PlayStates::triggerNonLooped (int padValue)
{
    //play when pressed
    //stop when pressed to max value (and then released?)
    //don't loop
    //retrigger when pressed again whilst playing
    
    playStateData = triggerLooped(padValue);
    
    playStateData.shouldLoop = false;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}




PlayStateData PlayStates::cycleLooped (int padValue)
{
    //play when pressed
    //loop
    //for each press after, move to the next sequence
    //when it reaches the last sequence, go back to the first sequence
    //stop when pressed to max value and then released
    //will be similar to trigger-looped, but instead of retriggering the sequence it will just move onto the next sequence
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0) 
    {
        currentPlayingStatus = 1;
        playStateData.playingStatus = 1;
        pressCount = 1;
        rePressCount = 1;
        maxPressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1 && maxPressCount == 1) 
    {
        releaseCount = 1;
        rePressCount = 0;
    }
    else if (currentPlayingStatus == 1 && padValue > 0 && rePressCount == 0 && releaseCount == 1) 
    {
        playStateData.moveToNextSeq = true;
        rePressCount = 1;
    }
    else if(padValue == 511)
    {
        maxPressCount = 0;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && maxPressCount == 0) 
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        pressCount = 0;
        releaseCount = 0;
        rePressCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
        playStateData.moveToNextSeq = false;
    }
    
    playStateData.pressureValue = 0;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = true;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}

PlayStateData PlayStates::cycleLinear (int padValue)
{
    //play when pressed
    //loop
    //for each press after, move to the next sequence
    //when it reaches the last sequence, stop
    //can also stop when pressed to max value and then released
    //will be similar to trigger-looped, but instead of retriggering the sequence it will just move onto the next sequence
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0) 
    {
        currentPlayingStatus = 1;
        playStateData.playingStatus = 1;
        pressCount = 1;
        rePressCount = 1;
        maxPressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1 && maxPressCount == 1) 
    {
        releaseCount = 1;
        rePressCount = 0;
    }
    else if (currentPlayingStatus == 1 && padValue > 0 && rePressCount == 0 && releaseCount == 1) 
    {
        playStateData.moveToNextSeq = true;
        rePressCount = 1;
    }
    else if(padValue == 511)
    {
        maxPressCount = 0;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && maxPressCount == 0) 
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        pressCount = 0;
        releaseCount = 0;
        rePressCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
        playStateData.moveToNextSeq = false;
    }
    
    playStateData.pressureValue = 0;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = true;
    playStateData.isLinearCycle = true;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}



PlayStateData PlayStates::autoCycleLooped (int padValue)
{
    //play when pressed
    //stop when pressed for a second time
    //when it reaches the the end of a sequence, automatically go to the next sequence
    //when it reaches the end of the last sequence, go back to beginning of the first sequence
    //will be similar to toggle pressoff, but automatically cycles through each sequence
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0)
    {
        playStateData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
    }
    
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        releaseCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue > 0 && pressCount >= 1 && releaseCount == 1) 
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        pressCount = 0;
    }
    else if(currentPlayingStatus == 0 && padValue == 0 && pressCount == 0 && releaseCount == 1)
    {
        releaseCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1; 
    rePressCount = 0;
    
    playStateData.pressureValue = 0;    
    playStateData.shouldLoop = false;
    playStateData.ignorePressure = true;
    playStateData.moveToNextSeq = false; //'move to next seq' command cannot be done here... must be done in SequencerPlayer
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = true;
    
    return playStateData;
}

PlayStateData PlayStates::autoCycleLinear (int padValue)
{
    //play when pressed
    //stop when pressed for a second time
    //when it reaches the the end of a sequence, automatically go to the next sequence
    //when it reaches the end of the last sequence, stop sequence
    //will be similar to toggle pressoff, but automatically cycles through each sequence
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0)
    {
        playStateData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
    }
    
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        releaseCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue > 0 && pressCount >= 1 && releaseCount == 1) 
    {
        currentPlayingStatus = 0;
        playStateData.playingStatus = 0;
        pressCount = 0;
    }
    else if(currentPlayingStatus == 0 && padValue == 0 && pressCount == 0 && releaseCount == 1)
    {
        releaseCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1; 
    rePressCount = 0;
    
    playStateData.pressureValue = 0;    
    playStateData.shouldLoop = false;
    playStateData.ignorePressure = true;
    playStateData.moveToNextSeq = false; //'move to next seq' command cannot be done here... must be done in SequencerPlayer
    playStateData.isLinearCycle = true;
    playStateData.isAutoCycle = true;
    
    return playStateData;
}


//new playstates
PlayStateData PlayStates::playOnceNonDestructive (int padValue)
{
    //play when pressed
    //can't be stopped by the user
    //don't loop
    //if reaches end of loop, set currentPlayingStatus to off
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0) 
    {
        playStateData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        releaseCount = 1;
    }
    else if(currentPlayingStatus == 0 && padValue == 0 && pressCount == 1 && releaseCount == 1)
    {
        pressCount = 0;
        releaseCount = 0;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1;
    rePressCount = 0;
    
    playStateData.pressureValue = padValue;
    playStateData.shouldLoop = false;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}


PlayStateData PlayStates::triggerLoopedNonDestructive (int padValue)
{
    //play when pressed
    //loop
    //retrigger when pressed again
    if (padValue > 0 && rePressCount == 0)
    {
        currentPlayingStatus = 1;
        playStateData.playingStatus = 1;
        pressCount = 1;
        rePressCount = 1;
        maxPressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1 && maxPressCount == 1)
    {
        releaseCount = 1;
        rePressCount = 0;
        
    }
    
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
  
    playStateData.pressureValue = padValue;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}


PlayStateData PlayStates::triggerNonLoopedNonDestructive (int padValue)
{
    //play when pressed
    //don't loop
    //retrigger when pressed again
    if (padValue > 0 && rePressCount == 0)
    {
        currentPlayingStatus = 1;
        playStateData.playingStatus = 1;
        pressCount = 1;
        rePressCount = 1;
        maxPressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1 && maxPressCount == 1)
    {
        releaseCount = 1;
        rePressCount = 0;
        
    }
    
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    
    playStateData.pressureValue = padValue;    
    playStateData.shouldLoop = false;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}



PlayStateData PlayStates::cycleLinearNonDestructive (int padValue)
{
    //play when pressed
    //loop
    //for each press after, move to the next sequence
    //when it reaches the last sequence, stop
    //will be similar to trigger-looped, but instead of retriggering the sequence it will just move onto the next sequence
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0) 
    {
        currentPlayingStatus = 1;
        playStateData.playingStatus = 1;
        pressCount = 1;
        rePressCount = 1;
        maxPressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1 && maxPressCount == 1) 
    {
        releaseCount = 1;
        rePressCount = 0;
    }
    else if (currentPlayingStatus == 1 && padValue > 0 && rePressCount == 0 && releaseCount == 1) 
    {
        playStateData.moveToNextSeq = true;
        rePressCount = 1;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
        playStateData.moveToNextSeq = false;
    }
    
    playStateData.pressureValue = 0;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = true;
    playStateData.isLinearCycle = true;
    playStateData.isAutoCycle = false;
    
    return playStateData;
}


PlayStateData PlayStates::toggleNonDestructive (int padValue)
{
    //play when pressed
    //loop
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0)
    {
        playStateData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        releaseCount = 1;
    }
    else
    {
        playStateData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1; 
    rePressCount = 0;
    
    playStateData.pressureValue = padValue;    
    playStateData.shouldLoop = true;
    playStateData.ignorePressure = false;
    playStateData.moveToNextSeq = false;
    playStateData.isLinearCycle = false;
    playStateData.isAutoCycle = false;
    
    return playStateData;
    
}
*/  
