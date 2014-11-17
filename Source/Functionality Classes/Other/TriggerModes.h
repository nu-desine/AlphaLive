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
    
    //special variables for the cycle and auto-cyle play states
    bool ignorePressure;
    bool moveToNextSeq;
    
};

#endif


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
    TriggerModeData toggleRelease (int padValue); //Not being used anymore
    TriggerModeData latch (int padValue);
    TriggerModeData latchMax (int padValue); //Not being used anymore
    TriggerModeData trigger (int padValue);
    
    //sequencer-only trigger modes
    TriggerModeData cycle (int padValue);
    TriggerModeData autoCycle (int padValue);
    
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