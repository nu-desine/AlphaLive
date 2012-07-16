//
//  GlobalClock.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/02/2012.
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

/*
 This is a Thread class that handles the global clock for triggering loops and sequences (and probably more eventually).
 Preferably AlphaLiveEngine should handle the thread however it already contains a thread that listens for OSC messages (as it derives
 from OscInput) so it couldn't be implemented there (as far as I tried).
 There will be an instance of this class within AlphaLiveEngine - is this the best class relationship?
 */

#ifndef H_GLOBALCLOCK
#define H_GLOBALCLOCK

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Application/AbstractSubjectAndObserver.h"

class AlphaLiveEngine;

class GlobalClock :     public Thread,
                        public Subject, //so this class can notify the observer class (GuiGlobalClock)
                        public AsyncUpdater
{
public:
    GlobalClock(AlphaLiveEngine &ref);
    ~GlobalClock();
    void handleAsyncUpdate();
    //Thread callback function
    void run();
    
    void startClock();
    void stopClock();
    
    void setTempo (float value);
    void setBeatsPerBar (int value);
    
    int getBeatNumber();
    int getBarNumber();
    
    int getGuiUpdateFlag();
    
    
private:
    
    double currentTime;
    double tempo;
    double timeInterval;
    
    AlphaLiveEngine &alphaLiveEngineRef;
    
    int beatsPerBar;
    int microbeatNumber, beatNumber, barNumber;
    int guiUpdateFlag; //1 - beat indictor, 2 - Toggle start button on
    
    
};

#endif //H_GLOBALCLOCK