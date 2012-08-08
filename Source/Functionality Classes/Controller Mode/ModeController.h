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
#include "../../Application/AbstractSubjectAndObserver.h"
#include "../../My IO Classes/OscOutput.h"

class ModeController :  public Subject, //so this class can be observed by the scene component
                        public AsyncUpdater
{

public:
    ModeController();
    ~ModeController();
    
    void setMidiOutputDevice (MidiOutput &midiOutput);
    void sendMidiMessage(MidiMessage midiMessage);
    
    void getOscData(int pad, int value);
    
    void changeScene();
    
    //void actionListenerCallback (const String& message);
    void handleAsyncUpdate();
    
    int getPadNumber();

private:
    
    int padNumber;
    int padValue;
    
    //scene switcher stuff
    int prevPadValue[48];
    
    OscOutput oscOutput;
    MidiOutput *midiOutputDevice;
    
};


#endif
