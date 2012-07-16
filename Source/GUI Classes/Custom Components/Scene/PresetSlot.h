//
//  PresetSlot.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 31/10/2011.
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
#ifndef H_PRESETSLOT
#define H_PRESETSLOT

//represets a single slot/hole in the preset component.

#include "../JuceLibraryCode/JuceHeader.h"

class PresetComponent;

class PresetSlot :  public Component
                    
{
public:
    PresetSlot(int slotNumber_, PresetComponent &ref);
    ~PresetSlot();
    
    void mouseDown (const MouseEvent &e);
    void mouseEnter	(const MouseEvent & e);
    void mouseExit	(const MouseEvent & e);
    
    void resized();
    void paint (Graphics &g);
    
    void setStatus (int value);
    int getStatus();
    
    void savePreset();
    void loadPreset();
    void clearPreset();
    
private:
    
    
    /*
    Status holds a slots status:
     - 0 = empty
     - 1 = Filled but not selected
     - 2 = Selected
    */
    int status;
    bool mouseIsOver;
    
    int slotNumber;
    String slotNumberString;
    
    PresetComponent &presetComponentRef;
    
};


#endif