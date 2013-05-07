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
    triggerModeData.moveToNextSeq = false;
    triggerModeData.ignorePressure = false;
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

    triggerModeData.ignorePressure = false;
    triggerModeData.moveToNextSeq = false;
    
    return triggerModeData;
}

TriggerModeData TriggerModes::toggle (int padValue)
{
    //press to play
    //press again to stop
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0)
    {
        triggerModeData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        releaseCount = 1;
        triggerModeData.playingStatus = 2;
    }
    else if(currentPlayingStatus == 1 && padValue > 0 && releaseCount == 1 && pressCount >= 1) 
    {
        currentPlayingStatus = 0;
        triggerModeData.playingStatus = 0;
        pressCount = 0;
    }
    else if(currentPlayingStatus == 0 && padValue == 0 && releaseCount == 1 && pressCount == 0)
    {
        releaseCount = 0;
        triggerModeData.playingStatus = 2;
    }
    else
    {
        triggerModeData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1; 
    rePressCount = 0;
    
    triggerModeData.ignorePressure = false;
    triggerModeData.moveToNextSeq = false;
    
    return triggerModeData;
}

TriggerModeData TriggerModes::toggleRelease (int padValue) //not being used anymore
{
    //press to play
    //press again and release to stop
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0)
    {
        currentPlayingStatus = 1;
        triggerModeData.playingStatus = 1;
        pressCount = 1;
        
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && releaseCount == 0 && pressCount == 1)
    {
        releaseCount = 1;
        triggerModeData.playingStatus = 2;
        
    }
    else if(currentPlayingStatus == 1 && padValue > 0 && releaseCount == 1 && pressCount == 1)
    {
        pressCount = 2;
        triggerModeData.playingStatus = 2;
        
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount >= 2)
    {
        currentPlayingStatus = 0;
        triggerModeData.playingStatus = 0;
        releaseCount = 0;
        pressCount = 0;
    }
    else
    {
        triggerModeData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1;
    rePressCount = 0;
    
    triggerModeData.ignorePressure = false;
    triggerModeData.moveToNextSeq = false;
    
    return triggerModeData;
}

TriggerModeData TriggerModes::latch (int padValue)
{
    //press to play
    //press to max value and release to stop
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0) 
    {
        currentPlayingStatus = 1;
        triggerModeData.playingStatus = 1;
        pressCount = 1;
        maxPressCount = 1;
    }
    else if(padValue == MAX_PRESSURE && maxPressCount == 1)
    {
        maxPressCount = 0;
        triggerModeData.playingStatus = 2;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && maxPressCount == 0) 
    {
        currentPlayingStatus = 0;
        triggerModeData.playingStatus = 0;
        releaseCount = 0;
        pressCount = 0;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1) 
    {
        releaseCount = 1;
        triggerModeData.playingStatus = 2;
    }
    else
    {
        triggerModeData.playingStatus = 2; //ignore
    }
    
    rePressCount = 0;
    
    triggerModeData.ignorePressure = false;
    triggerModeData.moveToNextSeq = false;
    
    return triggerModeData;
}

TriggerModeData TriggerModes::latchMax (int padValue) //not being used anymore
{
    //press to play
    //release to stop
    //press to max value to 'latch' the pad so any further 
    //releases don't stop the pad until pressed to max value again
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0) 
    {
        currentPlayingStatus = 1;
        triggerModeData.playingStatus = 1;
        pressCount = 1;
        maxPressCount = 0;
    }
    else if(padValue == MAX_PRESSURE && maxPressCount == 0)
    {
        maxPressCount = 1;
        triggerModeData.playingStatus = 2;
    }
    else if(padValue == MAX_PRESSURE && maxPressCount == 1)
    {
        maxPressCount = 0;
        triggerModeData.playingStatus = 2;
    }
    else if (currentPlayingStatus == 1 && padValue == 0 && maxPressCount == 0)
    {
        currentPlayingStatus = 0;
        triggerModeData.playingStatus = 0;
        releaseCount = 0;
        pressCount = 0;
    }
    
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1) 
    {
        releaseCount = 1;
        triggerModeData.playingStatus = 2;
    }
    else
    {
        triggerModeData.playingStatus = 2; //ignore
    }
    
    rePressCount = 0;

    triggerModeData.ignorePressure = false;
    triggerModeData.moveToNextSeq = false;
    
    return triggerModeData;
}

TriggerModeData TriggerModes::trigger (int padValue)
{
    //press to play
    //press again to restart the loop
    //press to max value and release to stop
    
    if (padValue > 0 && rePressCount == 0)
    {
        currentPlayingStatus = 1;
        triggerModeData.playingStatus = 1;
        pressCount = 1;
        rePressCount = 1;
        maxPressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1 && maxPressCount == 1)
    {
        releaseCount = 1;
        rePressCount = 0;
        triggerModeData.playingStatus = 2;
        
    }
    else if(padValue == MAX_PRESSURE)
    {
        maxPressCount = 0;
        triggerModeData.playingStatus = 2;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && maxPressCount == 0)
    {
        currentPlayingStatus = 0;
        triggerModeData.playingStatus = 0;
        pressCount = 0;
        releaseCount = 0;
        rePressCount = 0;
        
    }
    else
    {
        triggerModeData.playingStatus = 2; //ignore
    }
    
    triggerModeData.ignorePressure = false;
    triggerModeData.moveToNextSeq = false;
    
    return triggerModeData;
}



//===================================================================
//==================sequencer-only trigger modes=====================
//===================================================================



TriggerModeData TriggerModes::cycle (int padValue)
{
    //press to play and loop
    //for each press after, move to the next sequence
    //press to max value and release to stop
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0) 
    {
        currentPlayingStatus = 1;
        triggerModeData.playingStatus = 1;
        pressCount = 1;
        rePressCount = 1;
        maxPressCount = 1;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && pressCount == 1 && maxPressCount == 1) 
    {
        releaseCount = 1;
        rePressCount = 0;
        triggerModeData.playingStatus = 2;
    }
    else if (currentPlayingStatus == 1 && padValue > 0 && rePressCount == 0 && releaseCount == 1) 
    {
        triggerModeData.moveToNextSeq = true;
        rePressCount = 1;
        triggerModeData.playingStatus = 2;
    }
    else if(padValue == MAX_PRESSURE)
    {
        maxPressCount = 0;
        triggerModeData.playingStatus = 2;
    }
    else if(currentPlayingStatus == 1 && padValue == 0 && maxPressCount == 0) 
    {
        currentPlayingStatus = 0;
        triggerModeData.playingStatus = 0;
        pressCount = 0;
        releaseCount = 0;
        rePressCount = 0;
    }
    else
    {
        triggerModeData.playingStatus = 2; //ignore
        triggerModeData.moveToNextSeq = false;
    }

    triggerModeData.ignorePressure = true;
    
    return triggerModeData;
}

TriggerModeData TriggerModes::autoCycle (int padValue)
{
    //press to play
    //when sequence reaches end, it moves on to the next sequence
    //press to max value and release to stop
    
    if (currentPlayingStatus == 0 && padValue > 0 && pressCount == 0 && releaseCount == 0)
    {
        triggerModeData.playingStatus = 1;
        currentPlayingStatus = 1;
        pressCount = 1;
    }
    
    else if(currentPlayingStatus == 1 && padValue == 0)
    {
        releaseCount = 1;
        triggerModeData.playingStatus = 2;
    }
    else if(currentPlayingStatus == 1 && padValue > 0 && pressCount >= 1 && releaseCount == 1) 
    {
        currentPlayingStatus = 0;
        triggerModeData.playingStatus = 0;
        pressCount = 0;
    }
    else if(currentPlayingStatus == 0 && padValue == 0 && pressCount == 0 && releaseCount == 1)
    {
        releaseCount = 0;
        triggerModeData.playingStatus = 2;
    }
    else
    {
        triggerModeData.playingStatus = 2; //ignore
    }
    
    maxPressCount = 1; 
    rePressCount = 0;
    
    triggerModeData.ignorePressure = true;
    //'move to next seq' command cannot be done here for auto-cycle... must be done in SequencerPlayer
    triggerModeData.moveToNextSeq = false; 
    
    return triggerModeData;
}


void TriggerModes::reset()
{
    pressCount = 0;
    releaseCount = 0;
    maxPressCount = 0;
    rePressCount = 0;
    currentPlayingStatus = 0;
    
    triggerModeData.playingStatus = 0;
    triggerModeData.moveToNextSeq = false;
    triggerModeData.ignorePressure = false;
    
}