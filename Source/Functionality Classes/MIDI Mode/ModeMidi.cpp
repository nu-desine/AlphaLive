//
//  ModeMidi.cpp
//  AlphaLive
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

#include "ModeMidi.h"
#include "../../File and Settings/AppSettings.h"
#include "../Other/LayoutsAndScales.cpp"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]
#define PAD_SETTINGS_i AppSettings::Instance()->padSettings[i]

ModeMidi::ModeMidi(MidiOutput &midiOutput, AlphaLiveEngine &ref)
                :   midiOutputDevice(&midiOutput),
                    alphaLiveEngineRef(ref)
{
    for (int i = 0; i<= 47; i++)
    {
        
        channel[i] = PAD_SETTINGS_i->getMidiChannel();
        note[i] = PAD_SETTINGS_i->getMidiNote();
        velocity[i] = PAD_SETTINGS_i->getMidiVelocity();
        minRange[i] = PAD_SETTINGS_i->getMidiMinPressureRange();
        maxRange[i] = PAD_SETTINGS_i->getMidiMaxPressureRange();
        controllerNumber[i] = PAD_SETTINGS_i->getMidiCcController();
        pressureMode[i] = PAD_SETTINGS_i->getMidiPressureMode();
        triggerModeValue[i] = PAD_SETTINGS_i->getMidiTriggerMode();
        indestructible[i] = PAD_SETTINGS_i->getMidiIndestructible();
        sticky[i] = PAD_SETTINGS_i->getMidiSticky();
        pressureStatus[i] = PAD_SETTINGS_i->getMidiPressureStatus();
        noteStatus[i] = PAD_SETTINGS_i->getMidiNoteStatus();
        quantizeMode[i] = PAD_SETTINGS_i->getQuantizeMode();
        
        //not all members of the TriggerModeData struct are needed for MIDI mode
        triggerModeData[i].playingStatus = 0;
        //triggerModeData[i].pressureValue = 0;
        
        currentPlayingStatus[i] = 0;
        isCurrentlyPlaying[i] = false;
        prevPadValue[i] = 0;
        pressureValue[i] = 0;
    }
    
    broadcaster.addActionListener(this);
            
}


ModeMidi::~ModeMidi()
{

}

void ModeMidi::convertToMidi(int padNumber, int padValue)
{
    //this if statement will be true when a pad is first pressed after it was previously turned off
    //via an 'exclusive group' situation. It is needed so that the pad can be reset correctly.
    if (currentPlayingStatus[padNumber] == 1 && isCurrentlyPlaying[padNumber] == false && prevPadValue[padNumber] == 0)
    {
        //reset triggerMode settings
        currentPlayingStatus[padNumber] = 0; 
        triggerModes[padNumber].reset();
    }
    
    //==========================================================================================
    // Trigger Mode stuff
    //==========================================================================================
    
    switch (triggerModeValue[padNumber]) 
    {
        case 1:
            triggerModeData[padNumber] = triggerModes[padNumber].hold(padValue);
            break;
        case 2:
            triggerModeData[padNumber] = triggerModes[padNumber].toggle(padValue);
            break;
        case 3:
            triggerModeData[padNumber] = triggerModes[padNumber].toggleRelease(padValue);
            break;
        case 4:
            triggerModeData[padNumber] = triggerModes[padNumber].latch(padValue);
            break;
        case 5:
            triggerModeData[padNumber] = triggerModes[padNumber].latchMax(padValue);
            break;
        case 6:
            triggerModeData[padNumber] = triggerModes[padNumber].trigger(padValue);
            break;
        default:
            triggerModeData[padNumber] = triggerModes[padNumber].toggle(padValue);
            break;
    }

    
    if (indestructible[padNumber] == 1)
    {
        //if set to indestructible...
        
        if (triggerModeData[padNumber].playingStatus == 0)
        {
            //...and triggerModeData signifies to stop midi, DON'T LET IT...MWAHAHAHA! 
            triggerModeData[padNumber].playingStatus = 2; //ignore
        }
        else if (triggerModeData[padNumber].playingStatus == 1 && currentPlayingStatus[padNumber] == 1 && triggerModeValue[padNumber] != 6)
        {
            //...and triggerModeData signifies to start midi, 
            //but note is already playing and triggerMode does not equal 'trigger'
            //Don't send a play command!
            triggerModeData[padNumber].playingStatus = 2; //ignore
        }
        
    }
    
    //==========================================================================================
    // Start/stop stuff
    //==========================================================================================

    
    if (noteStatus[padNumber] == true) //if pad note status is 'on'
    {
        //==========================================================================================
        if (quantizeMode[padNumber] == 0) //free
        {
            //Create 'note on' message
            if (triggerModeData[padNumber].playingStatus == 1) //play
            {
                noteOn(padNumber);
            }
            
            //Create 'note off' message
            else if (triggerModeData[padNumber].playingStatus == 0) //stop
            {
                noteOff(padNumber);
            }
        }
        
        //==========================================================================================
        else if (quantizeMode[padNumber] == 1) // quantized
        {
            //Create 'note on' message
            if (triggerModeData[padNumber].playingStatus == 1) //play
            {
                currentPlayingStatus[padNumber] = 2; //waiting to play
                //waitingPad.addIfNotAlreadyThere(padNumber);
                alphaLiveEngineRef.addPadToQueue(padNumber);
            
                guiPadWaitingPlayUpdater.add(padNumber);
                broadcaster.sendActionMessage("WAITING TO PLAY");
                
            }
            
            
            
            //Create 'note off' message
            else if (triggerModeData[padNumber].playingStatus == 0) //stop
            {
                currentPlayingStatus[padNumber] = 3; // waiting to stop
                //waitingPad.addIfNotAlreadyThere(padNumber);
                alphaLiveEngineRef.addPadToQueue(padNumber);
                
                guiPadWaitingStopUpdater.add(padNumber);
                broadcaster.sendActionMessage("WAITING TO STOP");
            
            }
        }
    }
    
    //if the incoming message is a 'press'
    if (prevPadValue[padNumber] == 0)
    {
        //if clock isn't currently running and autoStartClock is on, start clock
        //Is it too CPU heavy getting the following two variables from their sources each time? 
        //Should these variables be past into this class each time they change?
        if (alphaLiveEngineRef.getGlobalClock()->isThreadRunning() == false && AppSettings::Instance()->getAutoStartClock() == 1)
        {
            std::cout << "Clock Starting!" << std::endl;
            alphaLiveEngineRef.getGlobalClock()->startClock();
        }
    }
    
    
    //==========================================================================================
    // Pressure stuff
    //==========================================================================================
    
    //scale 0-511 to 0-127
    //as the value will be scaled down and into an int, padValue could be rounded down to 0 even when it isn't quite 0
    //therefore we must make sure that it is atleast at the value of 1 untill it is actually set to 0,
    //so it doesn't mess up how the sticky feature is handled

    float padValueFloat = padValue * (127.0 / 511.0);
    
    if (padValueFloat > 0 && padValueFloat < 1)
        padValue = 1;
    else
    padValue = padValueFloat;
    
    
    if (sticky[padNumber] == 1) //'on'
    {
        //modify pressure value
        if(prevPadValue[padNumber] == 0)
        {
            pressureValue[padNumber] = 0;
            
        }
        else
        {
            if(padValue > pressureValue[padNumber])
            {
                pressureValue[padNumber] = padValue;
            }
            //else if it is smaller, don't change the pressure value
        }
        
    }
    else // 'off'
    {
        pressureValue[padNumber] = padValue; 
    }
    
    //std::cout << "Pad Value: " << padValue << std::endl;
    //std::cout << "Prev Pad Value: " << prevPadValue[padNumber] << std::endl;
    //std::cout << "pressure val: " << pressureValue[padNumber] << std::endl;
    
    //create pressure MIDI data
    sendPressureData(padNumber);
    
    
    prevPadValue[padNumber] = padValue;
    
}




void ModeMidi::noteOn (int padNumber)
{
    if (currentPlayingStatus[padNumber] == 1)
    {
        //this will be called if the 'trigger' triggerMode is selected.
        //must send a note off message first otherwise MIDI notes will hang
        MidiMessage message = MidiMessage::noteOff(channel[padNumber], note[padNumber]);
        sendMidiMessage(message);
        isCurrentlyPlaying[padNumber] = false;
    }
    
    MidiMessage message = MidiMessage::noteOn(channel[padNumber], note[padNumber], (uint8)velocity[padNumber]);
    sendMidiMessage(message);
    isCurrentlyPlaying[padNumber] = true;
    
    
    
    //update pad GUI
    if (currentPlayingStatus[padNumber] != 1) //GUI won't need updating if this is false
    {
        guiPadOnUpdater.add(padNumber);
        broadcaster.sendActionMessage("ON");
    }
    
    //set pad to 'on'
    currentPlayingStatus[padNumber] = 1; 
    
    
    
    //Exclusive mode stuff
    if (PAD_SETTINGS->getExclusiveMode() == 1)
    {
        alphaLiveEngineRef.handleExclusiveMode(padNumber);
    }
    
}




void ModeMidi::noteOff (int padNumber)
{
    //what about reseting pressure data value in here?
    //e.g. if you use one pad to pitch bend, and then turn the pad off via exclusive mode
    //the pitch will still be bent and won't change until you touch the previous pad again.
    
    MidiMessage message = MidiMessage::noteOff(channel[padNumber], note[padNumber]);
    sendMidiMessage(message);
    isCurrentlyPlaying[padNumber] = false;
    
    //update pad GUI
    guiPadOffUpdater.add(padNumber);
    broadcaster.sendActionMessage("OFF");
    
    currentPlayingStatus[padNumber] = 0; //set pad to 'off'
    
}


void ModeMidi::sendPressureData (int padNumber)
{
    //scale 0-127 to minPressure-maxPressure
    //this has to be done here and not above with the 511 to 127 scaling,
    //as the minRange could be set higher than the maxRange, which would mean
    //the sticky feature wouldn't work how it's meant to. 
    //Also we need to use a new variable here to covert the midi data,
    //so that sticky will still work correctly in all situations
    int pressureValueScaled = minRange[padNumber] + (pressureValue[padNumber]  
                            * ((maxRange[padNumber] - minRange[padNumber]) / 127.0));
    
    
    //std::cout << "pressureValueScaled: " << pressureValueScaled << std::endl;
    
    if (pressureStatus[padNumber] == true) //if pad pressure status is 'off'
    {
        MidiMessage message;
        
        //create 'pressure' data
        switch (pressureMode[padNumber])
        {
            case 1: //poly aftertouch
                message = MidiMessage::aftertouchChange(channel[padNumber], note[padNumber], pressureValueScaled);
                break;
                
            case 2: //CC messages
                message = MidiMessage::controllerEvent(channel[padNumber], controllerNumber[padNumber], pressureValueScaled);
                break;
                
            case 3: //channel aftertouch
                message = MidiMessage::channelPressureChange(channel[padNumber], pressureValueScaled);
                break;
                
            case 4: //pitch bend up
                //convert 0-127 to 8191-16383
                pressureValueScaled = 8192 + (pressureValueScaled * (8191.0/127.0));
                message = MidiMessage::pitchWheel(channel[padNumber], pressureValueScaled);
                break;
                
            case 5: //pitch bend down
                //convert 0-127 to 8191-0
                pressureValueScaled = 8192 - (pressureValueScaled * (8191.0/127.0));
                message = MidiMessage::pitchWheel(channel[padNumber], pressureValueScaled);
                break;
                
            case 6: // mod wheel
                message = MidiMessage::controllerEvent(channel[padNumber], 1, pressureValueScaled);
                break;
                
            default: //poly aftertouch
                message = MidiMessage::aftertouchChange(channel[padNumber], note[padNumber], pressureValueScaled);
                break;
        }
        
        sendMidiMessage(message);
    }

}



//Output the MIDI messages
void ModeMidi::sendMidiMessage(MidiMessage midiMessage)
{
    if(midiOutputDevice)
		midiOutputDevice->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
	else
		std::cout << "No MIDI output selected\n";
     
}


void ModeMidi::setMidiOutputDevice (MidiOutput &midiOutput)
{
    midiOutputDevice = &midiOutput;
}


void ModeMidi::triggerQuantizationPoint (int padNum)
{
    //get currentPlayingStatus of pad
    int status = currentPlayingStatus[padNum];
    //call correct function based on currentPlayingStatus
    if (status == 2) //waiting to play
        noteOn(padNum);
    else if (status == 3) //waiting to stop
        noteOff(padNum);
    
    /*
    if (waitingPad.size() > 0)
    {
        for (int i = 0; i < waitingPad.size(); i++)
        {
            //get pad number of waitingPad
            int pad = waitingPad[i];
            //get currentPlayingStatus of said pad
            int status = currentPlayingStatus[pad];
            //call correct function based on currentPlayingStatus
            if (status == 2) //waiting to play
                noteOn(pad);
            else if (status == 3) //waiting to stop
                noteOff(pad);
                
        }
        
        //remove items from array so they no longer recieve alerts of
        //quantization points in time
        waitingPad.clear();
    }
     */
}

void ModeMidi::killPad (int padNum)
{
    //if playing, send call noteOff() to send note off MIDI message which also will update the GUI
    if (isCurrentlyPlaying[padNum] == true) //don't use currentPlayStatus like below as that
                                            //doesn't always define when a file is truely playing
                                        
    {
        noteOff(padNum);
        triggerModes[padNum].reset();
            
    }
    //if currently waiting to play or stop, just update the GUI and the relevent states
    else if (currentPlayingStatus[padNum] == 2 ||
             currentPlayingStatus[padNum] == 3)
    {
        //update GUI
        guiPadOffUpdater.add(padNum);
        broadcaster.sendActionMessage("OFF");
        //update states within this class
        currentPlayingStatus[padNum] = 0; //set pad to 'off'
        triggerModes[padNum].reset();
    }
}


void ModeMidi::actionListenerCallback (const String& message)
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
    
    else if (message == "WAITING TO PLAY")
    {
        alphaLiveEngineRef.updatePadPlayingStatus(guiPadWaitingPlayUpdater.getLast(), 2);
        guiPadWaitingPlayUpdater.removeLast();
    }
    
    else if (message == "WAITING TO STOP")
    {
        alphaLiveEngineRef.updatePadPlayingStatus(guiPadWaitingStopUpdater.getLast(), 3);
        guiPadWaitingStopUpdater.removeLast();
    }
    
}

void ModeMidi::setPadData (int padNumber)
{
    setChannel (PAD_SETTINGS->getMidiChannel(), padNumber);
    setNote (PAD_SETTINGS->getMidiNote(), padNumber);
    setVelocity (PAD_SETTINGS->getMidiVelocity(), padNumber);
    setMinRange (PAD_SETTINGS->getMidiMinPressureRange(), padNumber);
    setMaxRange (PAD_SETTINGS->getMidiMaxPressureRange(), padNumber);
    setControllerNumber (PAD_SETTINGS->getMidiCcController(), padNumber);
    setPressureMode (PAD_SETTINGS->getMidiPressureMode(), padNumber);
    setTriggerModeValue (PAD_SETTINGS->getMidiTriggerMode(), padNumber);
    setIndestructible (PAD_SETTINGS->getMidiIndestructible(), padNumber);
    setSticky (PAD_SETTINGS->getMidiSticky(), padNumber);
    setPressureStatus (PAD_SETTINGS->getMidiPressureStatus(), padNumber);
    setNoteStatus (PAD_SETTINGS->getMidiNoteStatus(), padNumber);
    //setExclusiveGroup (PAD_SETTINGS->getMidiExclusiveGroup(), padNumber);
    setQuantizeMode (PAD_SETTINGS->getQuantizeMode(), padNumber);
    
}


void ModeMidi::setChannel (int value, int pad)
{
    //if new channel is different from current channel, stop MIDI note if currently playing to prevent hanging notes
    if (channel[pad] != value && isCurrentlyPlaying[pad] == true)
    {
        noteOff(pad);
        triggerModes[pad].reset();
    }
    
    channel[pad] = value;
}

void ModeMidi::setNote (int value, int pad)
{
    //if new note is different from current note, stop MIDI note if currently playing to prevent hanging notes
    if (note[pad] != value && isCurrentlyPlaying[pad] == true)
    {
        noteOff(pad);
        triggerModes[pad].reset();
    }
    
    note[pad] = value;
}

void ModeMidi::setVelocity (int value, int pad)
{
    velocity[pad] = value;
}

void ModeMidi::setMinRange (int value, int pad)
{
    minRange[pad] = value;
}

void ModeMidi::setMaxRange (int value, int pad)
{
    maxRange[pad] = value;
}

void ModeMidi::setControllerNumber (int value, int pad)
{
    //is CC number has changed and pressure is currently > 0, reset the value of the current pressure data
    if (controllerNumber[pad] != value && pressureValue[pad] > 0)
    {
       pressureValue[pad] = 0; 
        sendPressureData(pad);
    }
    
    controllerNumber[pad] = value;
}

void ModeMidi::setPressureMode (int value, int pad)
{
    //if pressure mode has changed and pressure is currently > 0, reset the value of the current pressure data
    if (pressureMode[pad] != value && pressureValue[pad] > 0)
    {
        pressureValue[pad] = 0; 
        sendPressureData(pad);
    }
    
    pressureMode[pad] = value;
}

void ModeMidi::setTriggerModeValue (int value, int pad)
{
    triggerModeValue[pad] = value;
}

void ModeMidi::setIndestructible (int value, int pad)
{
    indestructible[pad] = value;
}

void ModeMidi::setSticky (int value, int pad)
{
    sticky[pad] = value;
}

void ModeMidi::setPressureStatus (bool value, int pad)
{
    //if pressure status has changed and pressure is currently > 0, reset the value of the current pressure data
    if (pressureStatus[pad] != value && pressureValue[pad] > 0)
    {
        pressureValue[pad] = 0; 
        sendPressureData(pad);
    }
    
    pressureStatus[pad] = value;
}

void ModeMidi::setNoteStatus (bool value, int pad)
{
    //if note status has changed, stop MIDI note if currently playing to prevent hanging notes
    if (noteStatus[pad] != value && isCurrentlyPlaying[pad] == true)
    {
        noteOff(pad);
        triggerModes[pad].reset();
    }

    noteStatus[pad] = value;
}

void ModeMidi::setQuantizeMode (int value, int pad)
{
    quantizeMode[pad] = value;
}

