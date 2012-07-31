//
//  ModeMidi.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 22/09/2011.
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

#ifndef H_MODEMIDI
#define H_MODEMIDI

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Other/TriggerModes.h"

class AlphaLiveEngine;

class ModeMidi :    public ActionListener
{

public:
    ModeMidi(MidiOutput &midiOutput, AlphaLiveEngine &ref);
    ~ModeMidi();
    
    void convertToMidi(int padNumber, int padValue);
    void sendMidiMessage(MidiMessage midiMessage);
    
    void noteOn (int padNumber);
    void noteOff (int padNumber);
    void sendPressureData (int padNumber);
    
    void triggerQuantizationPoint();
    
    void actionListenerCallback (const String& message);
    
    void killPad (int padNum);
    
    void setMidiOutputDevice (MidiOutput &midiOutput);
    void setPadData (int padNumber);
    
    //settings
    void setChannel (int value, int pad);
    void setNote (int value, int pad);
    void setVelocity (int value, int pad);
    void setMinRange (int value, int pad);
    void setMaxRange (int value, int pad);
    void setControllerNumber (int value, int pad);
    void setPressureMode (int value, int pad);
    void setTriggerModeValue (int value, int pad);
    void setIndestructible (int value, int pad);
    void setSticky (int value, int pad);
    void setPressureStatus (bool value, int pad);
    void setNoteStatus (bool value, int pad);
    void setQuantizeMode (int value, int pad);

private:
    
    MidiOutput *midiOutputDevice;
    
    bool isCurrentlyPlaying[48];    //set whenever a MIDI note on/off message is triggered within the code
    int currentPlayingStatus[48];   //set when a MIDI note is turned on or off directly by the user, but not when
                                    //a midi note is turned off via exclusive mode
    int prevPadValue[48];
    int pressureValue[48];
    
    //settings
    int channel[48];
    int note[48];
    int velocity[48];
    int minRange[48];
    int maxRange[48];
    int controllerNumber[48];
    int pressureMode[48];
    int triggerModeValue[48];
    int indestructible[48];
    int sticky[48];
    bool pressureStatus[48];
    bool noteStatus[48];
    int quantizeMode[48];
    TriggerModeData triggerModeData[48];
    TriggerModes triggerModes[48];
    
    ActionBroadcaster broadcaster;
    AlphaLiveEngine &alphaLiveEngineRef;
    
    //As we're updating the pad GUI playing status using an actionListenerCallback
    //the actual command to update the GUI is done at a later time to when the MIDI note on/off
    //command is called. Therefore we need a couple of int arrays to hold the pad numbers of any
    //pads that need their GUI's updating, as if multiple pads are pressed/released at the same time
    //the actionListenerCallback probably won't be called right after each individual pad press/release,
    //meaning that we can't just get the last pad number that was triggered.
    //Each time a pad is pressed/released the pad number is added to the end of the relevant array,
    //and then within actionListnerCallback it sends a command to update the pad number
    //at the last index of the relevant array, and then removes this item.
    Array<int> guiPadOnUpdater;
    Array<int> guiPadOffUpdater;
    Array<int> guiPadWaitingPlayUpdater;
    Array<int> guiPadWaitingStopUpdater;
    
    //quantization stuff
    Array<int> waitingPad;
    
};


#endif //H_MODEMIDI
