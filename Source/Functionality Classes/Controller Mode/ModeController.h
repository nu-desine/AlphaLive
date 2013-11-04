//
//  ModeController.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 21/11/2011.
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

#ifndef H_MODECONTROLLER
#define H_MODECONTROLLER

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../My IO Classes/OscOutput.h"

class AlphaLiveEngine;
class MainComponent;

class ModeController :  public ActionListener
{

public:
    ModeController(AlphaLiveEngine &ref);
    ~ModeController();
    
    void getInputData(int pad, int value, int velocity);
    
    void changeScene();
    
    void actionListenerCallback (const String& message);
    
    int getPadNumber();
    
    void setMainComponent(MainComponent *mainComponent_);

private:
    
    int padNumber;
    int padValue;
    
    int prevPadValue[48];
    bool pressureLatchModeStatus[48]; //stores whether each pad is currently latching another pad
    
    OscOutput oscOutput;
    AlphaLiveEngine &alphaLiveEngineRef;
    
    MainComponent *mainComponent;
    
    ActionBroadcaster broadcaster;
};


#endif
