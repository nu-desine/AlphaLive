//
//  EliteControls.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 19/11/2012.
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

#ifndef H_ELITECONTROLS
#define H_ELITECONTROLS

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../My IO Classes/OscOutput.h"

class AlphaLiveEngine;
class AppSettings;
class MainComponent;

class EliteControls
{
public:
    EliteControls (AlphaLiveEngine &ref);
    ~EliteControls();
    
    void setMainComponent(MainComponent *mainComponent_);
    
    void getInputData(int control, int value);
    
private:
    
    MainComponent *mainComponent;
    
    int eliteDialNumber, eliteButtonNumber;
    int eliteControlValue;
    
    OscOutput oscOutput;
    AlphaLiveEngine &alphaLiveEngineRef;
    
};

#endif //H_ELITECONTROLS