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

ModeMidi::ModeMidi(MidiOutput &midiOutput, AlphaLiveEngine &ref)
                :   midiOutputDevice(&midiOutput),
                    alphaLiveEngineRef(ref)
{
    for (int i = 0; i<= 47; i++)
    {
        channel[i] = 1;
        note[i] = 60;
        velocity[i] = 110;
        minRange[i] = 0;
        maxRange[i] = 127;
        controllerNumber[i] = Layouts::ccLayout[i];
        pressureMode[i] = 1;
        triggerModeValue[i] = 1;
        pressureStatus[i] = true;
        noteStatus[i] = true;
        exclusiveGroup[i] = 0;
        quantizeMode[i] = 1;
        
        //not all members of the TriggerModeData struct are needed for MIDI mode
        triggerModeData[i].playingStatus = 0;
        triggerModeData[i].pressureValue = 0;
        
        currentPlayingStatus[i] = 0;
        isCurrentlyPlaying[i] = false;
        prevPadValue[i] = 0;
    }
    
    for (int i = 0; i <= 15; i++)
        currentExclusivePad.insert(i, 100); //'100' here is used to signify an 'empty/NULL' value
    
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
    //determime triggerMode
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

    
    
    
    //scale 0-511 to minPressure-maxPressure
    triggerModeData[padNumber].pressureValue = minRange[padNumber] + (triggerModeData[padNumber].pressureValue * ((maxRange[padNumber] - minRange[padNumber]) / 511.0));
    

    
    if (noteStatus[padNumber] == true) //if pad note status is 'on'
    {
        //==========================================================================================
        if (quantizeMode[padNumber] == 1) //free
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
        else if (quantizeMode[padNumber] == 2) // quantized
        {
            //Create 'note on' message
            if (triggerModeData[padNumber].playingStatus == 1) //play
            {
                currentPlayingStatus[padNumber] = 2; //waiting to play
                waitingPad.addIfNotAlreadyThere(padNumber);
                
                if (triggerModeValue[padNumber] != 1) //if not on 'standard' triggerMode
                {
                    guiPadWaitingPlayUpdater.add(padNumber);
                    broadcaster.sendActionMessage("WAITING TO PLAY");
                }
            }
            
            //Create 'note off' message
            else if (triggerModeData[padNumber].playingStatus == 0) //stop
            {
                currentPlayingStatus[padNumber] = 3; // waiting to stop
                waitingPad.addIfNotAlreadyThere(padNumber);
                
                if (triggerModeValue[padNumber] != 1) //if not on 'standard' triggerMode
                {
                    guiPadWaitingStopUpdater.add(padNumber);
                    broadcaster.sendActionMessage("WAITING TO STOP");
                }
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

    
    prevPadValue[padNumber] = padValue;
    
    sendPressureData(padNumber);
    
    
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
    if (triggerModeValue[padNumber] != 1) //if not on 'standard' triggerMode
    {
        if (currentPlayingStatus[padNumber] != 1) //GUI won't need updating if this is false
        {
            guiPadOnUpdater.add(padNumber);
            broadcaster.sendActionMessage("ON");
        }
    }
    
    //set pad to 'on'
    currentPlayingStatus[padNumber] = 1; 
    
    
    
    
    //Exclusive mode stuff
    if (exclusiveGroup[padNumber] != 0) //if exclusive group equals something above 1, it is in 'exclusive' mode
    {
        //get the previously triggered pad of the same group
        int prevPad = currentExclusivePad[exclusiveGroup[padNumber]];
        
        if (currentExclusivePad[exclusiveGroup[padNumber]] != 100) //if it exists...
        {
            //if it isn't the same as the current pad...
            if (prevPad != padNumber)
            {
                //get the MIDI note and MIDI channel of the pad number stored in the array
                int prevNote = AppSettings::Instance()->padSettings[prevPad]->getMidiNote();
                int prevChannel = AppSettings::Instance()->padSettings[prevPad]->getMidiChannel();
                
                //stop prev note of this group
                MidiMessage message = MidiMessage::noteOff(prevChannel, prevNote);
                sendMidiMessage(message);
                isCurrentlyPlaying[prevPad] = false;
                
                //update GUI
                if (triggerModeValue[padNumber] != 1) //if not on 'standard' triggerMode
                {
                    //update pad GUI
                    guiPadOffUpdater.add(prevPad);
                    broadcaster.sendActionMessage("OFF");
                }
                
            }
            
        }
        
        //if it isn't the same as the current pad...
        if (prevPad != padNumber)
        {
            //remove prev pad from exclusive group array
            currentExclusivePad.remove(exclusiveGroup[padNumber]);
            //add current pad to the exclusive group array
            currentExclusivePad.insert(exclusiveGroup[padNumber], padNumber);
        }
    }
}




void ModeMidi::noteOff (int padNumber)
{
    MidiMessage message = MidiMessage::noteOff(channel[padNumber], note[padNumber]);
    sendMidiMessage(message);
    isCurrentlyPlaying[padNumber] = false;
    
    if (triggerModeValue[padNumber] != 1) //if not on 'standard' triggerMode
    {
        //update pad GUI
        guiPadOffUpdater.add(padNumber);
        broadcaster.sendActionMessage("OFF");
    }
    
    currentPlayingStatus[padNumber] = 0; //set pad to 'off'
    
    //Exclusive mode stuff - need to reset the relevent index of the exclusive array
    //so that the above note off message won't be repeated the next time a pad of the same group
    //is triggered.
    if (exclusiveGroup[padNumber] != 0) //if exclusive group equals something above 1, it is in 'exclusive' mode
    {
        //remove pad from exclusive group array
        currentExclusivePad.remove(exclusiveGroup[padNumber]);
        //insert the default 'NULL' value
        currentExclusivePad.insert(exclusiveGroup[padNumber], 100);
    }
}


void ModeMidi::sendPressureData (int padNumber)
{
    
    if (pressureStatus[padNumber] == true) //if pad pressure status is 'off'
    {
        MidiMessage message;
        
        //create 'pressure' data
        switch (pressureMode[padNumber])
        {
            case 1: //poly aftertouch
                message = MidiMessage::aftertouchChange(channel[padNumber], note[padNumber], triggerModeData[padNumber].pressureValue);
                break;
                
            case 2: //CC messages
                message = MidiMessage::controllerEvent(channel[padNumber], controllerNumber[padNumber], triggerModeData[padNumber].pressureValue);
                break;
                
            case 3: //channel aftertouch
                message = MidiMessage::channelPressureChange(channel[padNumber], triggerModeData[padNumber].pressureValue);
                break;
                
            case 4: //pitch bend up
                //convert 0-127 to 8191-16383
                triggerModeData[padNumber].pressureValue = 8192 + (triggerModeData[padNumber].pressureValue * (8191.0/127.0));
                message = MidiMessage::pitchWheel(channel[padNumber], triggerModeData[padNumber].pressureValue);
                break;
                
            case 5: //pitch bend down
                //convert 0-127 to 8191-0
                triggerModeData[padNumber].pressureValue = 8192 - (triggerModeData[padNumber].pressureValue * (8191.0/127.0));
                message = MidiMessage::pitchWheel(channel[padNumber], triggerModeData[padNumber].pressureValue);
                break;
                
            case 6: // mod wheel
                message = MidiMessage::controllerEvent(channel[padNumber], 1, triggerModeData[padNumber].pressureValue);
                break;
                
            default: //poly aftertouch
                message = MidiMessage::aftertouchChange(channel[padNumber], note[padNumber], triggerModeData[padNumber].pressureValue);
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


void ModeMidi::triggerQuantizationPoint()
{
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
}

void ModeMidi::killAll()
{
    for (int i = 0; i <= 47; i++)
    {
        if (isCurrentlyPlaying[i] == true)
        {
            noteOff(i);
            triggerModes[i].reset();
            
        }
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
    setPressureStatus (PAD_SETTINGS->getMidiPressureStatus(), padNumber);
    setNoteStatus (PAD_SETTINGS->getMidiNoteStatus(), padNumber);
    setExclusiveGroup (PAD_SETTINGS->getMidiExclusiveGroup(), padNumber);
    setQuantizeMode (PAD_SETTINGS->getMidiQuantizeMode(), padNumber);
    
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
    if (controllerNumber[pad] != value && triggerModeData[pad].pressureValue > 0)
    {
        triggerModeData[pad].pressureValue = 0; 
        sendPressureData(pad);
    }
    
    controllerNumber[pad] = value;
}

void ModeMidi::setPressureMode (int value, int pad)
{
    //if pressure mode has changed and pressure is currently > 0, reset the value of the current pressure data
    if (pressureMode[pad] != value && triggerModeData[pad].pressureValue > 0)
    {
        triggerModeData[pad].pressureValue = 0; 
        sendPressureData(pad);
    }
    
    pressureMode[pad] = value;
}

void ModeMidi::setTriggerModeValue (int value, int pad)
{
    triggerModeValue[pad] = value;
}

void ModeMidi::setPressureStatus (bool value, int pad)
{
    //if pressure status has changed and pressure is currently > 0, reset the value of the current pressure data
    if (pressureStatus[pad] != value && triggerModeData[pad].pressureValue > 0)
    {
        triggerModeData[pad].pressureValue = 0; 
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

void ModeMidi::setExclusiveGroup (int value, int pad)
{
    exclusiveGroup[pad] = value-1;
}

void ModeMidi::setQuantizeMode (int value, int pad)
{
    quantizeMode[pad] = value;
}

