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
        
        int padNumber = i;
        control[i] = PAD_SETTINGS->getControllerControl();
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
    
    
    
    if (control[padNumber] == 1) //scene switcher
    {
        changeScene(); 
    }
    
    else if (control[padNumber] == 4) //osc controller
    {
        //should i bet getting the ip addresses and port numbers for each pad beforehand (is it to cpu heavy accessing PadSettings everytime there is an incoming message?)?
        oscOutput.transmitPadMessage(padNumber+1, padValue, velocity, PAD_SETTINGS->getControllerOscIpAddress(), PAD_SETTINGS->getControllerOscPort());
    }
    
    else if (control[padNumber] == 2) //MIDI program change
    {
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            MidiMessage message = MidiMessage::programChange(PAD_SETTINGS->getControllerMidiProgramChangeChannel(), PAD_SETTINGS->getControllerMidiProgramChangeNumber());
            alphaLiveEngineRef.sendMidiMessage(message);
        }
    }
    
    else if (control[padNumber] == 3) //MIDI program change & scene switcher
    {
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            MidiMessage message = MidiMessage::programChange(PAD_SETTINGS->getControllerMidiProgramChangeChannel(), PAD_SETTINGS->getControllerMidiProgramChangeNumber());
            alphaLiveEngineRef.sendMidiMessage(message);
            
            changeScene(); 
        }
    }
    
    if (control[padNumber] == 5) //killswitch
    {
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            broadcaster.sendActionMessage("KILLSWITCH");
        }
    }
    
    if (control[padNumber] == 6) //pressure latch mode
    {
        //Remember that padNumber here is the Controller pad number,
        //NOT the number of the pad that is being latched. The pad number for
        //the latched pad is called padToLatch set below.
        
        if (prevPadValue[padNumber] == 0 && padValue > 0)
        {
            if (pressureLatchModeStatus[padNumber] == false)
            {
                //latching a pad
                int padToLatch = PAD_SETTINGS->getControllerPressureLatchPadNumber();
                alphaLiveEngineRef.latchPressureValue(padToLatch, true);
                pressureLatchModeStatus[padNumber] = true;
                
                //update the pad GUI
                guiPadOnUpdater.add(padNumber);
                broadcaster.sendActionMessage("ON");
            }
            else
            {
                //unlatching a pad
                unlatchPad (padNumber);
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
        
        //the 'two press' bug is being caused by prevPadValue[padNumber] not being
        //reset to 0, as once this function is called the getInputData() function
        //won't be called again if this pad is being switched to a different mode.
    }
}

void ModeController::actionListenerCallback (const String& message)
{
    if (message == "ON")
    {
        alphaLiveEngineRef.updatePadPlayingStatus(guiPadOnUpdater.getLast(), 1);
        guiPadOnUpdater.removeLast();
    }
    
    else if (message == "OFF")
    {
        alphaLiveEngineRef.updatePadPlayingStatus(guiPadOffUpdater.getLast(), 0);
        guiPadOffUpdater.removeLast();
    }
    
    else if (message == "KILLSWITCH")
    {
        mainComponent->perform(CommandIDs::KillSwitch);
    }
}

void ModeController::killPad (int padNum)
{
    if (pressureLatchModeStatus[padNum] == true)
    {
        //update the pad GUI
        unlatchPad(padNum);
    }
}

void ModeController::unlatchPad (int padNum)
{
    //Remember that padNum here is the Controller pad number,
    //NOT the number of the pad that is being latched. The pad number for
    //the latched pad is called padToLatch set below.
    
    std::cout << "unlatching " << PAD_SETTINGS->getControllerPressureLatchPadNumber() << " using pad " << padNum << std::endl;
    
    //unlatch the defined pad
    int padToLatch = PAD_SETTINGS->getControllerPressureLatchPadNumber();
    alphaLiveEngineRef.latchPressureValue(padToLatch, false);
    
    //set that THIS pad is no longer latching a pad
    pressureLatchModeStatus[padNum] = false;
    
    //update THIS pad GUI
    guiPadOffUpdater.add(padNum);
    broadcaster.sendActionMessage("OFF");
}

int ModeController::getPadNumber()
{
    return padNumber;
}

void ModeController::setMainComponent(MainComponent *mainComponent_)
{
    mainComponent = mainComponent_;
}

void ModeController::setControl(int value, int padNum)
{
    if (control[padNum] != value)
    {
        if (pressureLatchModeStatus[padNum] == true)
        {
            unlatchPad(padNum);
        }
        
        control[padNum] = value;
    }
}
