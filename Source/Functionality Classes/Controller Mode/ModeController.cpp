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
#include "../../GUI Classes/Views/MainComponent.h"
#include "../../Application/CommandIDs.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]


ModeController::ModeController(AlphaLiveEngine &ref)
                    :   alphaLiveEngineRef(ref)
{
    for (int i = 0; i <= 47; i++)
    {
        prevPadValue[i] = 0;
        pressureLatchModeStatus[i] = false;
    }

     broadcaster.addActionListener(this);
}

ModeController::~ModeController()
{
    
}


//this function is called from AlphaLiveEngine whenever an OSC message is recieved
//who's pad is set to Controller mode
void ModeController::getInputData(int pad, int value, int velocity)
{
    padNumber = pad;
    padValue = value;
    if (PAD_SETTINGS->getVelocityCurve() == 4)
        velocity = PAD_SETTINGS->getStaticVelocity();
    
    if (PAD_SETTINGS->getControllerControl() == 1) //scene switcher
    {
        changeScene(); 
    }
    else if (PAD_SETTINGS->getControllerControl() == 4) //osc controller
    {
        //should i bet getting the ip addresses and port numbers for each pad beforehand (is it to cpu heavy accessing PadSettings everytime there is an incoming message?)?
        oscOutput.transmitPadMessage(padNumber+1, padValue, velocity, PAD_SETTINGS->getControllerOscIpAddress(), PAD_SETTINGS->getControllerOscPort());
    }
    
    else if (PAD_SETTINGS->getControllerControl() == 2) //MIDI program change
    {
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            MidiMessage message = MidiMessage::programChange(PAD_SETTINGS->getControllerMidiProgramChangeChannel(), PAD_SETTINGS->getControllerMidiProgramChangeNumber());
            alphaLiveEngineRef.sendMidiMessage(message);
        }
    }
    
    else if (PAD_SETTINGS->getControllerControl() == 3) //MIDI program change & scene switcher
    {
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            MidiMessage message = MidiMessage::programChange(PAD_SETTINGS->getControllerMidiProgramChangeChannel(), PAD_SETTINGS->getControllerMidiProgramChangeNumber());
            alphaLiveEngineRef.sendMidiMessage(message);
            
            changeScene(); 
        }
    }
    
    if (PAD_SETTINGS->getControllerControl() == 5) //killswitch
    {
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            broadcaster.sendActionMessage("KILLSWITCH");
        }
    }
    
    if (PAD_SETTINGS->getControllerControl() == 6) //pressure latch mode
    {
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            if (pressureLatchModeStatus[padNumber] == false)
            {
                //latching a pad
                int padToLatch = PAD_SETTINGS->getControllerPressureLatchPadNumber();
                alphaLiveEngineRef.latchPressureValue(padToLatch, true);
                pressureLatchModeStatus[padNumber] = true;
            }
            else
            {
                //unlatching a pad
                int padToLatch = PAD_SETTINGS->getControllerPressureLatchPadNumber();
                alphaLiveEngineRef.latchPressureValue(padToLatch, false);
                pressureLatchModeStatus[padNumber] = false;
            }
            
        }
    }

     prevPadValue[padNumber] = padValue;
}

//called when pad is set to the scene switcher control
void ModeController::changeScene()
{
    if (prevPadValue[padNumber] == 0 && padValue > 0)
    {
        mainComponent->getSceneComponent()->selectSlot(PAD_SETTINGS->getControllerSceneNumber() - 1);
        
        //if there is a 'two clicks' error... it's probably being caused here!
        //how can i fix this?
    }
}

void ModeController::actionListenerCallback (const String& message)
{
    if (message == "KILLSWITCH")
    {
        mainComponent->perform(CommandIDs::KillSwitch);
    }
}

int ModeController::getPadNumber()
{
    return padNumber;
}

void ModeController::setMainComponent(MainComponent *mainComponent_)
{
    mainComponent = mainComponent_;
}
