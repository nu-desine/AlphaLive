//
//  ModeController.cpp
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

#include "ModeController.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]


ModeController::ModeController()
                    : midiOutputDevice(NULL)
{
    for (int i = 0; i <= 47; i++)
    {
        prevPadValue[i] = 0;
    }

}

ModeController::~ModeController()
{
    
}


//this function is called from AlphaLiveEngine whenever an OSC message is recieved
//who's pad is set to Controller mode
void ModeController::getOscData(int pad, int value)
{
    padNumber = pad;
    padValue = value;
    
    if (PAD_SETTINGS->getControllerControl() == 2) //scene switcher
    {
        changeScene(); 
    }
    else if (PAD_SETTINGS->getControllerControl() == 3) //osc controller
    {
        //would be nice if this option wasn't in the controller mode and was in the OSC routing class, as there is a very slight delay. would it make a noticeable difference?
        //or is the delay down to the code in the OscOutput class?
        //actually, is there a delay compared to the reaction in other modes???
        
        //should i bet getting the ip addresses and port numbers for each pad beforehand (is it to cpu heavy accessing PadSettings everytime there is an incoming message?)?
        
        //scale the range back down to 0-127
        padValue = padValue * (127.0/511.0);
        
        oscOutput.transmitThruMessage(padNumber+1, padValue, PAD_SETTINGS->getControllerOscIpAddress(), PAD_SETTINGS->getControllerOscPort());
    }
    
    else if (PAD_SETTINGS->getControllerControl() == 4) //MIDI program change
    {
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            MidiMessage message = MidiMessage::programChange(PAD_SETTINGS->getControllerMidiProgramChangeChannel(), PAD_SETTINGS->getControllerMidiProgramChangeNumber());
            sendMidiMessage(message);
        }
    
    }
    
    else if (PAD_SETTINGS->getControllerControl() == 5) //MIDI program change & scene switcher
    {
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            MidiMessage message = MidiMessage::programChange(PAD_SETTINGS->getControllerMidiProgramChangeChannel(), PAD_SETTINGS->getControllerMidiProgramChangeNumber());
            sendMidiMessage(message);
            
            changeScene(); 
        }
        
       
    }

     prevPadValue[padNumber] = padValue;
}


//called when pad is set to the scene switcher control
void ModeController::changeScene()
{
    if (prevPadValue[padNumber] == 0 && padValue > 0)
    {
        triggerAsyncUpdate();
        //if there is a 'two clicks' error... it's probably being caused here!
        //how can i fix this?
        
    }

}


//Output the MIDI messages
void ModeController::sendMidiMessage(MidiMessage midiMessage)
{
    if(midiOutputDevice)
		midiOutputDevice->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
	else
		std::cout << "No MIDI output selected\n";
    
}


void ModeController::setMidiOutputDevice (MidiOutput &midiOutput)
{
    midiOutputDevice = &midiOutput;
}


void ModeController::handleAsyncUpdate()
{
    notifyObs();
}

int ModeController::getPadNumber()
{
    return padNumber;
}
