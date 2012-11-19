//
//  EliteControls.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 19/11/2012.
//  Copyright 2012 nu desine. All rights reserved.
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

#include "EliteControls.h"
#include "../File and Settings/AppSettings.h"

EliteControls::EliteControls(MidiOutput &midiOutput, AlphaLiveEngine &ref)
                            :   midiOutputDevice(&midiOutput),
                                alphaLiveEngineRef(ref)
{
    
}

EliteControls::~EliteControls()
{
    
}

void EliteControls::getInputData(int control, int value)
{
    
}

void EliteControls::sendMidiMessage(MidiMessage midiMessage)
{
    if (alphaLiveEngineRef.hasOpenedHidDevice() == true)
    {
        unsigned char dataToSend[5];
        
        uint8 *rawMidiMessage = midiMessage.getRawData();
        
        //std::cout << "raw midi message " << std::endl;
        //printf("%02hhx ", rawMidiMessage[0]);
        //printf("%02hhx ", rawMidiMessage[1]);
        //printf("%02hhx ", rawMidiMessage[2]);
        //printf("\n");
        
        dataToSend[0] = 0x00;   //if no reportID's are defined in the descriptor,
                                //must send 0x00. First byte MUST be the report ID.
        dataToSend[1] = MIDI_OUT_COMMAND_ID;
        dataToSend[2] = rawMidiMessage[0]; //midi status byte
        dataToSend[3] = rawMidiMessage[1]; //midi data byte 1
        dataToSend[4] = rawMidiMessage[2]; //midi data byte 2
        
        alphaLiveEngineRef.sendHidControlReport(dataToSend);
    }
    else
    {
        if(midiOutputDevice)
            midiOutputDevice->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
        else
            std::cout << "No MIDI output selected\n";
    }
}


void EliteControls::setMidiOutputDevice (MidiOutput &midiOutput)
{
    midiOutputDevice = &midiOutput;
}