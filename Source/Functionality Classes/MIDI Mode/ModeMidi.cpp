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
#include "../../Application/Common.h"
#include "../../File and Settings/StoredSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]
#define PAD_SETTINGS_i AppSettings::Instance()->padSettings[i]
#define PRESSURE_HOLDER alphaLiveEngineRef.getMidiChannelPressureHolderPtr(currentChannel[padNumber])

ModeMidi::ModeMidi (AlphaLiveEngine &ref)
                : alphaLiveEngineRef(ref)
{
    for (int i = 0; i<= 47; i++)
    {
        channel[i] = currentChannel[i] = PAD_SETTINGS_i->getMidiChannel();
        note[i] = PAD_SETTINGS_i->getMidiNote();
        velocity[i] = PAD_SETTINGS_i->getStaticVelocity();
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
        autoMidiChannelStatus[i] = PAD_SETTINGS_i->getMidiDynamicChannelStatus();
        for (int chan = 0; chan < 16; chan++)
            autoMidiChannels[chan][i] = PAD_SETTINGS_i->getMidiDynamicChannels(chan);
        
        //not all members of the TriggerModeData struct are needed for MIDI mode
        triggerModeData[i].playingStatus = 0;
        
        currentPlayingStatus[i] = 0;
        isPlaying[i] = false;
        prevPadValue[i] = 0;
        pressureValue[i] = 0;
    }
    
    columnNumber = sequenceNumber = 0;
    midiChannelPressureMode = StoredSettings::getInstance()->midiChannelPressureMode;
    
    broadcaster.addActionListener(this);
}


ModeMidi::~ModeMidi()
{

}

void ModeMidi::getInputData(int padNumber, int padValue, int padVelocity)
{
    //this if statement will be true when a pad is first pressed after it was previously turned off
    //via an 'exclusive group' situation. It is needed so that the pad can be reset correctly.
    if (currentPlayingStatus[padNumber] == 1 && isPlaying[padNumber] == false && prevPadValue[padNumber] == 0)
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
            triggerModeData[padNumber] = triggerModes[padNumber].latch(padValue);
            break;
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
        else if (triggerModeData[padNumber].playingStatus == 1 && currentPlayingStatus[padNumber] == 1 && triggerModeValue[padNumber] != 4)
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
    
    //set velocity value...
    if (triggerModeData[padNumber].playingStatus == 1) //play
    {
        //should I be using local variables below instead of accessing PAD_SETTINGS each time?
        //Though as I'm only doing this if statement when the pad is first pressed it probably
        //won't be too CPU extensive here.
        if (PAD_SETTINGS->getVelocityCurve() == 4)
        {
            //static velocity
            velocity[padNumber] = PAD_SETTINGS->getStaticVelocity();
        }
        else
            velocity[padNumber] = padVelocity;
    }
    
    
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
            alphaLiveEngineRef.getGlobalClock()->startClock();
        }
    }
    
    
    //exclusive channel pressure stuff.
    //Do this here after the noteOn() function is called
    //incase dynamic channel mode is on and currentChannel is
    //changed.
    if (prevPadValue[padNumber] == 0) //pad pressed
    {
        if (midiChannelPressureMode == 1)
        {
            //assign the pressed pad number to the relevent variable in the
            //MidiChannelPressureHolder struct...
            
            if (pressureMode[padNumber] == 2) //channel aftertouch
            {
                PRESSURE_HOLDER->aftertouch = padNumber;
            }
            else if (pressureMode[padNumber] == 3) //mod wheel
            {
                PRESSURE_HOLDER->controlChange[1] = padNumber;
            }
            else if (pressureMode[padNumber] == 4) //CC data
            {
                PRESSURE_HOLDER->controlChange[controllerNumber[padNumber]] = padNumber;
            }
            else if (pressureMode[padNumber] == 5 || pressureMode[padNumber] == 6) //pitch bend
            {
                PRESSURE_HOLDER->pitchBend = padNumber;
            }
        }
    }
    
    //==========================================================================================
    // Pressure stuff
    //==========================================================================================
    
    // === sticky mode ===
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
    
    // === create pressure MIDI data ===
    sendPressureData(padNumber);
    
    
    
    
    //exclusive channel pressure stuff.
    //do this stuff here after pressure data has been sent.
    if (padValue == 0) //pad released
    {
        if (midiChannelPressureMode == 1)
        {
            //remove the released pad number from the relevent variable in the
            //MidiChannelPressureHolder struct if it exists there...
            
            if (pressureMode[padNumber] == 2) //channel aftertouch
            {
                if (PRESSURE_HOLDER->aftertouch == padNumber)
                {
                    PRESSURE_HOLDER->aftertouch = -1;
                }
            }
            else if (pressureMode[padNumber] == 3) //mod wheel
            {
                if (PRESSURE_HOLDER->controlChange[1] == padNumber)
                {
                    PRESSURE_HOLDER->controlChange[1] = -padNumber;
                }
            }
            else if (pressureMode[padNumber] == 4) //CC data
            {
                if (PRESSURE_HOLDER->controlChange[controllerNumber[padNumber]] == padNumber)
                {
                    PRESSURE_HOLDER->controlChange[controllerNumber[padNumber]] = -1;
                }
            }
            else if (pressureMode[padNumber] == 5 || pressureMode[padNumber] == 6) //pitch bend
            {
                if (PRESSURE_HOLDER->pitchBend == padNumber)
                {
                    PRESSURE_HOLDER->pitchBend = -1;
                }
            }
        }
    }

    prevPadValue[padNumber] = padValue;
}




void ModeMidi::noteOn (int padNumber)
{
    if (currentPlayingStatus[padNumber] == 1)
    {
        //this will be called if the 'trigger' triggerMode is selected.
        //must send a note off message first otherwise MIDI notes will hang
        MidiMessage message = MidiMessage::noteOff(currentChannel[padNumber], note[padNumber]);
        alphaLiveEngineRef.sendMidiMessage(message);
        isPlaying[padNumber] = false;
    }
    
    //if dynamic channel mode is on, only need to dynamically set the channel here when turning
    //a note on. The following pressure data and note off message must use the same channel.
    setCurrentChannel(padNumber);
    
    MidiMessage message = MidiMessage::noteOn(currentChannel[padNumber], note[padNumber], (uint8)velocity[padNumber]);
    alphaLiveEngineRef.sendMidiMessage(message);
    isPlaying[padNumber] = true;
    
    //NEW - recording into sequencer pads
    if (alphaLiveEngineRef.getRecordingPads().size() > 0)
    {
        for (int i = 0; i < alphaLiveEngineRef.getRecordingPads().size(); i++)
        {
            int recordingPad = alphaLiveEngineRef.getRecordingPads()[i];
            
            if (alphaLiveEngineRef.getModeSequencer()->getSequencePlayerInstance(recordingPad)->isCurrentlyPlaying()) //if currently playing
            {
                if (AppSettings::Instance()->padSettings[recordingPad]->getSequencerMode() == 1) //if MIDI mode
                {
                    if (currentChannel[padNumber] == AppSettings::Instance()->padSettings[recordingPad]->getSequencerMidiChannel()) //if MIDI channels match
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
    
    MidiMessage message = MidiMessage::noteOff(currentChannel[padNumber], note[padNumber]);
    alphaLiveEngineRef.sendMidiMessage(message);
    isPlaying[padNumber] = false;
    
    //update pad GUI
    guiPadOffUpdater.add(padNumber);
    broadcaster.sendActionMessage("OFF");
    
    //currentPlayingStatus[padNumber] = 0; //Don't call this here! Screws up exclusive mode stuff.
    
    
}


void ModeMidi::sendPressureData (int padNumber)
{
    //scale 0-MAX_PRESSURE to minPressure-maxPressure
    //this has to be done here and not above with the sticky mode code,
    //as the minRange could be set higher than the maxRange, which would mean
    //the sticky feature wouldn't work how it's meant to. 
    //Also we need to use a new variable here to covert the midi data,
    //so that sticky will still work correctly in all situations
    
    int pressureValueScaled = scaleValue (pressureValue[padNumber],
                                          0,
                                          MAX_PRESSURE,
                                          minRange[padNumber],
                                          maxRange[padNumber]);
    
    if (pressureStatus[padNumber] == true) //if pad pressure status is 'off'
    {
        MidiMessage message;
        
        //create 'pressure' data
        switch (pressureMode[padNumber])
        {
            case 1: //poly aftertouch
                message = MidiMessage::aftertouchChange(currentChannel[padNumber], note[padNumber], pressureValueScaled);
                alphaLiveEngineRef.sendMidiMessage(message);
                break;
                
            case 4: //CC messages
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->controlChange[controllerNumber[padNumber]] != padNumber)
                {
                    break;
                }
                else
                {
                    message = MidiMessage::controllerEvent(currentChannel[padNumber], controllerNumber[padNumber], pressureValueScaled);
                    alphaLiveEngineRef.sendMidiMessage(message);
                    break;
                }
                
            case 2: //channel aftertouch
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->aftertouch != padNumber)
                {
                    break;
                }
                else
                {
                    message = MidiMessage::channelPressureChange(currentChannel[padNumber], pressureValueScaled);
                    alphaLiveEngineRef.sendMidiMessage(message);
                    break;
                }
                
            case 5: //pitch bend up
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->pitchBend != padNumber)
                {
                    break;
                }
                else
                {
                    //convert 0-127 to 8192-16383
                    pressureValueScaled = scaleValue (pressureValueScaled, 0, 127, 8192, 16383);
                    message = MidiMessage::pitchWheel(currentChannel[padNumber], pressureValueScaled);
                    alphaLiveEngineRef.sendMidiMessage(message);
                    break;
                }
                
            case 6: //pitch bend down
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->pitchBend != padNumber)
                {
                    break;
                }
                else
                {
                    //convert 0-127 to 8192-0
                    pressureValueScaled = scaleValue (pressureValueScaled, 0, 127, 8192, 0);
                    message = MidiMessage::pitchWheel(currentChannel[padNumber], pressureValueScaled);
                    alphaLiveEngineRef.sendMidiMessage(message);
                    break;
                }
                
            case 3: // mod wheel
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->controlChange[1] != padNumber)
                {
                    break;
                }
                else
                {
                    message = MidiMessage::controllerEvent(currentChannel[padNumber], 1, pressureValueScaled);
                    alphaLiveEngineRef.sendMidiMessage(message);
                    break;
                }
                
            default: //poly aftertouch
                message = MidiMessage::aftertouchChange(currentChannel[padNumber], note[padNumber], pressureValueScaled);
                alphaLiveEngineRef.sendMidiMessage(message);
                break;
        }
        
    }

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
    if (isPlaying[padNum] == true) //don't use currentPlayStatus like below as that
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

bool ModeMidi::isCurrentlyPlaying (int padNum)
{
    return isPlaying[padNum];
}

void ModeMidi::setCurrentChannel (int padNum)
{
    //static MIDI channel
    if (autoMidiChannelStatus[padNum] == false)
    {
        currentChannel[padNum] = channel[padNum];
    }
    
    //Dynamic MIDI channel
    else if (autoMidiChannelStatus[padNum] == true)
    {
        Array<int> previouslyUsedChannels = alphaLiveEngineRef.getPreviouslyUsedMidiChannels();
        Array <int> availableChannels;
        
        for (int i = 0; i < 16; i++)
        {
            //Find all channels that this pad could be set to that aren't currently active
            
            if (autoMidiChannels[i][padNum] == true && alphaLiveEngineRef.getMidiChannelStatus(i) == false)
            {
                availableChannels.add(i);
            }
        }
        
        if (availableChannels.size() > 0)
        {
            //Find the oldest value in previousUsedChannels (oldest at the start)
            //that is also in available channels, and set the current channel to that
            
            for (int i = 0; i < previouslyUsedChannels.size(); i++)
            {
                if (availableChannels.contains(previouslyUsedChannels[i]))
                {
                    //this if statement should always be true before it reaches the
                    //end of the loop. Should I put a check incase it doesn't for some
                    //strange reason, and what should I do in this situation?
                    
                    currentChannel[padNum] = previouslyUsedChannels[i] + 1;
                    //std::cout << "Current channel set to: " << currentChannel[padNum] << std::endl;
                    
                    break;
                }
            }
   
        }
        else
        {
            //All channels this pad could be set to are currently active.
            //The best thing to do here would probably be to
            //set it to the oldest active pad.
            
            availableChannels.clearQuick();
            
            for (int i = 0; i < 16; i++)
            {
                //Find all channels that this pad could be set to
                
                if (autoMidiChannels[i][padNum] == true)
                {
                    availableChannels.add(i);
                }
            }
            
            //Find the oldest value in previousUsedChannels (oldest at the start)
            //that is also in available channels, and set the current channel to that
            
            for (int i = 0; i < previouslyUsedChannels.size(); i++)
            {
                if (availableChannels.contains(previouslyUsedChannels[i]))
                {
                    //this if statement should always be true before it reaches the
                    //end of the loop. Should I put a check incase it doesn't for some
                    //strange reason, and what should I do in this situation?
                    
                    currentChannel[padNum] = previouslyUsedChannels[i] + 1;
                    //std::cout << "No free channels - Current channel set to: " << currentChannel[padNum] << std::endl;
                    
                    break;
                }
            }
            
        }
    }
}

void ModeMidi::setChannel (int value, int pad)
{
    //if new channel is different from current channel, stop MIDI note if currently playing to prevent hanging notes
    if (currentChannel[pad] != value && isPlaying[pad] == true)
    {
        noteOff(pad);
        triggerModes[pad].reset();
        currentPlayingStatus[pad] = 0; //set pad to 'off'
    }
    
    channel[pad] = value;
    currentChannel[pad] = value;
}

void ModeMidi::setNote (int value, int pad)
{
    //if new note is different from current note, stop MIDI note if currently playing to prevent hanging notes
    if (note[pad] != value && isPlaying[pad] == true)
    {
        noteOff(pad);
        triggerModes[pad].reset();
        currentPlayingStatus[pad] = 0; //set pad to 'off'
    }
    
    note[pad] = value;
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
    if (noteStatus[pad] != value && isPlaying[pad] == true)
    {
        noteOff(pad);
        triggerModes[pad].reset();
        currentPlayingStatus[pad] = 0; //set pad to 'off'
    }

    noteStatus[pad] = value;
    
    //If note status has been changed to off, we need to make sure that the channel is set
    //correctly, as at this point it could have been set to another channel using the
    //dynamic channel mode.
    if (value == false)
    {
        currentChannel[pad] = channel[pad];
    }
}

void ModeMidi::setQuantizeMode (int value, int pad)
{
    quantizeMode[pad] = value;
}

void ModeMidi::setAutoMidiChannelStatus (bool value, int pad)
{
    autoMidiChannelStatus[pad] = value;
}

void ModeMidi::setAutoMidiChannels (int channel, bool status, int pad)
{
    autoMidiChannels[channel][pad] = status;
}

void ModeMidi::setMidiChannelPressureMode (int value)
{
    midiChannelPressureMode = value;
}

