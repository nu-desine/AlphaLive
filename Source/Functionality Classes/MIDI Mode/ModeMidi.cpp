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
        velocity[i] = PAD_SETTINGS_i->getVelocity();
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
    
    columnNumber = sequenceNumber = 0;
    
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
        //case 3:
            //triggerModeData[padNumber] = triggerModes[padNumber].toggleRelease(padValue);
            //break;
        case 3:
            triggerModeData[padNumber] = triggerModes[padNumber].latch(padValue);
            break;
        //case 5:
           // triggerModeData[padNumber] = triggerModes[padNumber].latchMax(padValue);
           // break;
        case 4:
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
                currentPlayingStatus[padNumber] = 0; //set pad to 'off'
            }
        }
        
        //==========================================================================================
        else if (quantizeMode[padNumber] == 1) // quantized
        {
            //Create 'note on' message
            if (triggerModeData[padNumber].playingStatus == 1) //play
            {
                currentPlayingStatus[padNumber] = 2; //waiting to play
                alphaLiveEngineRef.addPadToQueue(padNumber);
            
                guiPadWaitingPlayUpdater.add(padNumber);
                broadcaster.sendActionMessage("WAITING TO PLAY");
                
            }
            
            
            
            //Create 'note off' message
            else if (triggerModeData[padNumber].playingStatus == 0) //stop
            {
                currentPlayingStatus[padNumber] = 3; // waiting to stop
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
    
    //NEW - recording into sequencer pads
    if (alphaLiveEngineRef.getRecordingPads().size() > 0)
    {
        for (int i = 0; i < alphaLiveEngineRef.getRecordingPads().size(); i++)
        {
            int recordingPad = alphaLiveEngineRef.getRecordingPads()[i];
            
            if (alphaLiveEngineRef.getModeSequencer()->getSequencePlayerInstance(recordingPad)->isThreadRunning()) //if currently playing
            {
                if (AppSettings::Instance()->padSettings[recordingPad]->getSequencerMode() == 1) //if MIDI mode
                {
                    if (channel[padNumber] == AppSettings::Instance()->padSettings[recordingPad]->getSequencerMidiChannel()) //if MIDI channels match
                    {
                        int seqNote[NO_OF_ROWS];
                        
                        for (int j = 0; j < NO_OF_ROWS; j++)
                        {
                            seqNote[j] = AppSettings::Instance()->padSettings[recordingPad]->getSequencerMidiNote(j);
                            
                            if (note[padNumber] == seqNote[j]) //if MIDI notes match
                            {
                                sequenceNumber = alphaLiveEngineRef.getModeSequencer()->getSequencePlayerInstance(recordingPad)->getSequenceNumber();
                                
                                //get the closest column number
                                Array <int> columnNumberData = alphaLiveEngineRef.getModeSequencer()->getSequencePlayerInstance(recordingPad)->getClosestColumnNumber();
                                columnNumber = columnNumberData[0];
                                int columnNumberType = columnNumberData[1];
                                
                                //When recording a note to a sequencer pad the note will play twice at this point - 
                                //from the played pad (this class) and from the recorded note in the sequence, which is note what we want.
                                //We want the new note to be played from this class only, and not the seq notes which will all be played at the same
                                //set length, amoung a few other unwanted behaviours.
                                //This is done using a new variable within SequencerPlayer called recentlyAddedSequenceData.
                                //Every time a note is recorded here it adds a 'true' to the array in the same location as the recorded note.
                                //Then in SequencerPlayer it won't play this new note due to this 'true' flag.
                                
                                if (columnNumberType == 1) //if the closest number is the current column number, add it to the recentAddedSequenceData Array so it isn't played
                                    alphaLiveEngineRef.getModeSequencer()->getSequencePlayerInstance(recordingPad)->setRecentlyAddedSequenceData(sequenceNumber, j, columnNumber, true);
                                
                                AppSettings::Instance()->padSettings[recordingPad]->setSequencerData(sequenceNumber, j, columnNumber, velocity[padNumber]);
                                
                                //if currently selected pad is the recording pad, update the grid gui.
                                //how should it be handled if multiple pads are selected? Do nothing?
                                if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1)
                                {
                                    if (AppSettings::Instance()->getCurrentlySelectedPad()[0] == recordingPad)
                                    {
                                        broadcaster.sendActionMessage("RECORD NOTE");
                                    }
                                }
                                
                            }
                        }
                    }
                }
            }
        }
    }
    
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
    
    //currentPlayingStatus[padNumber] = 0; //Don't call this here! Screws up exclusive mode stuff.
    
    
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
                
            case 4: //CC messages
                message = MidiMessage::controllerEvent(channel[padNumber], controllerNumber[padNumber], pressureValueScaled);
                break;
                
            case 2: //channel aftertouch
                message = MidiMessage::channelPressureChange(channel[padNumber], pressureValueScaled);
                break;
                
            case 5: //pitch bend up
                //convert 0-127 to 8191-16383
                pressureValueScaled = 8192 + (pressureValueScaled * (8191.0/127.0));
                message = MidiMessage::pitchWheel(channel[padNumber], pressureValueScaled);
                break;
                
            case 6: //pitch bend down
                //convert 0-127 to 8191-0
                pressureValueScaled = 8192 - (pressureValueScaled * (8191.0/127.0));
                message = MidiMessage::pitchWheel(channel[padNumber], pressureValueScaled);
                break;
                
            case 3: // mod wheel
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
    unsigned char dataToSend[4];
    memset(dataToSend,0,sizeof(dataToSend));
    
    uint8 *rawMidiMessage = midiMessage.getRawData();
    
    //std::cout << "raw midi message " << std::endl;
    //printf("%02hhx ", rawMidiMessage[0]);
    //printf("%02hhx ", rawMidiMessage[1]);
    //printf("%02hhx ", rawMidiMessage[2]);
    //printf("\n");
    
    dataToSend[0] = 0x06; //MIDI out HID report ID
    dataToSend[1] = rawMidiMessage[0]; //midi status byte
    dataToSend[2] = rawMidiMessage[1]; //midi data byte 1
    dataToSend[3] = rawMidiMessage[2]; //midi data byte 2
    
    alphaLiveEngineRef.sendControlReport(dataToSend);
    
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
    {
        noteOn(padNum);
    }
    else if (status == 3) //waiting to stop
    {
        noteOff(padNum);
        currentPlayingStatus[padNum] = 0; //set pad to 'off'
    }
}

void ModeMidi::killPad (int padNum)
{
    //if playing, send call noteOff() to send note off MIDI message which also will update the GUI
    if (isCurrentlyPlaying[padNum] == true) //don't use currentPlayStatus like below as that
                                            //doesn't always define when a file is truely playing
                                        
    {
        noteOff(padNum);
        triggerModes[padNum].reset();
        currentPlayingStatus[padNum] = 0; //set pad to 'off'
            
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

void ModeMidi::stopPrevExclusivePad (int padNum)
{
   noteOff(padNum);
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
    
    else if (message == "RECORD NOTE")
    {
        //optimise the below so we're only calling/updating what needs to be done!
        //first, update the display of the sequence grid which gets the stored
        //sequence data from PadSettings and puts it into the local sequenceData. This
        //could be optimised so that it is only getting the data from the current seq,
        //as thats all that will be changed here.
        alphaLiveEngineRef.getModeSequencer()->updateSequencerGridGui (columnNumber, sequenceNumber, 3);
        //next set the currently sequence display, which sets the status's of the grid points
        alphaLiveEngineRef.getModeSequencer()->updateSequencerGridGui (columnNumber, sequenceNumber, 2);
    }
    
}

void ModeMidi::setPadData (int padNumber)
{
    setChannel (PAD_SETTINGS->getMidiChannel(), padNumber);
    setNote (PAD_SETTINGS->getMidiNote(), padNumber);
    setVelocity (PAD_SETTINGS->getVelocity(), padNumber);
    setMinRange (PAD_SETTINGS->getMidiMinPressureRange(), padNumber);
    setMaxRange (PAD_SETTINGS->getMidiMaxPressureRange(), padNumber);
    setControllerNumber (PAD_SETTINGS->getMidiCcController(), padNumber);
    setPressureMode (PAD_SETTINGS->getMidiPressureMode(), padNumber);
    setTriggerModeValue (PAD_SETTINGS->getMidiTriggerMode(), padNumber);
    setIndestructible (PAD_SETTINGS->getMidiIndestructible(), padNumber);
    setSticky (PAD_SETTINGS->getMidiSticky(), padNumber);
    setPressureStatus (PAD_SETTINGS->getMidiPressureStatus(), padNumber);
    setNoteStatus (PAD_SETTINGS->getMidiNoteStatus(), padNumber);
    setQuantizeMode (PAD_SETTINGS->getQuantizeMode(), padNumber);
    
}


void ModeMidi::setChannel (int value, int pad)
{
    //if new channel is different from current channel, stop MIDI note if currently playing to prevent hanging notes
    if (channel[pad] != value && isCurrentlyPlaying[pad] == true)
    {
        noteOff(pad);
        triggerModes[pad].reset();
        currentPlayingStatus[pad] = 0; //set pad to 'off'
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
        currentPlayingStatus[pad] = 0; //set pad to 'off'
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
        currentPlayingStatus[pad] = 0; //set pad to 'off'
    }

    noteStatus[pad] = value;
}

void ModeMidi::setQuantizeMode (int value, int pad)
{
    quantizeMode[pad] = value;
}

