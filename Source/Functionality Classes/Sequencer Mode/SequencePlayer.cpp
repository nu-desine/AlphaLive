//
//  SequencePlayer.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 18/10/2011.
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

#include "SequencePlayer.h"
#include "../../File and Settings/AppSettings.h"
#include "ModeSequencer.h"
#include "../../Application/Common.h"
#include "../../File and Settings/StoredSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]
#define PRESSURE_HOLDER modeSequencerRef.getAlphaLiveEngineRef().getMidiChannelPressureHolderPtr(midiChannel-1)

//=====================================================================================
//=Constructor=========================================================================
//=====================================================================================

SequencePlayer::SequencePlayer(int padNumber_, ModeSequencer &ref, TimeSliceThread* audioTransportSourceThread_) 
                                :   padNumber(padNumber_),
                                    modeSequencerRef(ref)
{
    sequenceIsRunning = sequenceFlaggedToStop = false;
    audioTransportSourceThread = audioTransportSourceThread_;
    
    //=======get setting values=========
    for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
    {
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
            {
                sequenceData[seq][row][column] = PAD_SETTINGS->getSequencerData(seq, row, column);
                recentlyAddedSequenceData[seq][row][column] = false;
            }
        }
    }
    
    relativeTempoMode = PAD_SETTINGS->getSequencerRelativeTempoMode();
    setTempo(AppSettings::Instance()->getGlobalTempo());
    
    numberOfSequences = PAD_SETTINGS->getSequencerNumberOfSequences();
    triggerMode = PAD_SETTINGS->getSequencerTriggerMode();
    shouldLoop = PAD_SETTINGS->getSequencerShouldLoop();
    indestructible = PAD_SETTINGS->getSequencerIndestructible();
    shouldFinishLoop = PAD_SETTINGS->getSequencerShouldFinishLoop();
    sticky = PAD_SETTINGS->getSequencerSticky();
    sequenceLength = PAD_SETTINGS->getSequencerLength();
    for (int row = 0; row <=NO_OF_ROWS-1; row++)
       midiNote[row] = PAD_SETTINGS->getSequencerMidiNote(row); 
    quantizeMode = PAD_SETTINGS->getQuantizeMode();
    dynamicMode = PAD_SETTINGS->getSequencerDynamicMode();
    
    midiChannel = PAD_SETTINGS->getSequencerMidiChannel();
    midiNoteLength  = PAD_SETTINGS->getSequencerMidiNoteLength();
    midiMinRange = PAD_SETTINGS->getSequencerMidiMinPressureRange();
    midiMaxRange = PAD_SETTINGS->getSequencerMidiMaxPressureRange();
    midiControllerNumber = PAD_SETTINGS->getSequencerMidiCcController();
    midiPressureMode = PAD_SETTINGS->getSequencerMidiPressureMode();
    midiPressureStatus = PAD_SETTINGS->getSequencerMidiPressureStatus();
    
    gain = gainPrev = PAD_SETTINGS->getSequencerGain();
    panLeft = panLeftPrev = PanControl::leftChanPan_(PAD_SETTINGS->getSequencerPan());
    panRight = panRightPrev = PanControl::rightChanPan_(PAD_SETTINGS->getSequencerPan());
    
    triggerModeData.playingStatus = 0;
    triggerModeData.moveToNextSeq = false;
    
    midiClockValue = AppSettings::Instance()->getMidiClockValue();
    midiClockMessageFilter = AppSettings::Instance()->getMidiClockMessageFilter();
    midiClockMessageCounter = 0;
    currentlySyncedToMidiClockMessages = false;
    
    //=====================================
    
    sequenceNumber = 0;
    prevSequenceNumber = 0;
    midiNoteOnCounter = 0;
    currentPlayingState = 0;
    
    for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
    {
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
            {
                //init midiNoteOffTime
                midiNoteOffTime[seq][row][column] = 0;
            }
        }
    }
    
    broadcaster.addActionListener(this);
    
    //init all effects to be null
    gainAndPan = nullptr;
    lowPassFilter = nullptr;
    highPassFilter = nullptr;
    bandPassFilter = nullptr;
    reverb = nullptr;
    delay = nullptr;
    flanger = nullptr;
    tremolo = nullptr;
	distortion = nullptr;
	bitcrusher = nullptr;
    
    //set effect to default 0, and then call set effect to create the effect object
    //This alg. prevents any crashes caused within prepareToPlay when trying to
    //set the sampleRate, where the effect object must exist
    effect = 0;
    setSamplesEffect(PAD_SETTINGS->getSequencerEffect());
    
    prevPadValue = pressureValue =  0;
    playingLastLoop = false;
    
    mode = 0;
    setMode(PAD_SETTINGS->getSequencerMode());
    
    //if the pad is currently set to be record enabled, add this pad to the recordingPad array
    if (PAD_SETTINGS->getSequencerRecordEnabled() == 1)
        modeSequencerRef.getAlphaLiveEngineRef().setRecordingSequencerPadsState(padNumber, 1);
    
    midiChannelPressureMode = StoredSettings::getInstance()->midiChannelPressureMode;
}

//=====================================================================================
//=Destructor==========================================================================
//=====================================================================================


SequencePlayer::~SequencePlayer()
{
    //if this pad is current set to record enabled, remove this pad from the recordingPads array
    if (PAD_SETTINGS->getSequencerRecordEnabled() == 1)
        modeSequencerRef.getAlphaLiveEngineRef().setRecordingSequencerPadsState(padNumber, 0);
    
    delete gainAndPan;
    delete lowPassFilter;
    delete highPassFilter;
    delete bandPassFilter;
    delete delay;
    delete reverb;
    delete flanger;
    delete tremolo;
	delete distortion;
	delete bitcrusher;
    
    stopSequence();
    //stopThreadAndReset();
    
    {
        const MessageManagerLock mmLock; //lock event thread so it is safe to make calls in the message thread
        modeSequencerRef.updatePadPlayingStatus(padNumber, 0);
    }
    
    if (mode == 2)
    {
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            delete sequenceAudioFilePlayer[row];
        }
    }
    
    audioMixer.removeAllInputs();
}

//=====================================================================================
//=Process input data stuff============================================================
//=====================================================================================

void SequencePlayer::processInputData(int padValue)
{
    //this is needed incase audio ends on its own or is stopped via the 'exclusive mode' feature(?) , in order to reset everything so it will trigger again properly
    if (sequenceIsRunning == false && currentPlayingState == 1 && prevPadValue == 0)
    {
        currentPlayingState = 0;
        triggerModes.reset();
    }
    
    //==========================================================================================
    // Trigger Mode stuff
    //==========================================================================================
    
    switch (triggerMode) 
    {
        case 1:
            triggerModeData = triggerModes.hold(padValue);
            break;
        case 2:
            triggerModeData = triggerModes.toggle(padValue);
            break;
        case 3:
            triggerModeData = triggerModes.latch(padValue);
            break;
        case 4:
            triggerModeData = triggerModes.trigger(padValue);
            break;
        case 5:
            triggerModeData = triggerModes.cycle(padValue);
            break;
        case 6:
            triggerModeData = triggerModes.autoCycle(padValue);
            break;
        default:
            triggerModeData = triggerModes.toggle(padValue);
            break;
    }
    
    
    if (indestructible == 1)
    {
        //if set to indestructible...
        
        if (triggerModeData.playingStatus == 0)
        {
            //...and triggerModeData signifies to stop audio, DON'T LET IT...MWAHAHAHA! 
            triggerModeData.playingStatus = 2; //ignore
        }
        else if (triggerModeData.playingStatus == 1 && currentPlayingState == 1 && triggerMode != 4)
        {
            //...and triggerModeData signifies to start playing, 
            //but file is already playing and triggerMode does not equal 'trigger'
            //Don't send a play command!
            triggerModeData.playingStatus = 2; //ignore
        }
        
    }
    
    
    if (triggerModeData.playingStatus == 0 
        && shouldFinishLoop == 1 
        && indestructible == 0 
        && currentPlayingState == 1)
    {
        //if recieved a command to stop file but is set to finish current loop before stopping,
        //ignore note off message and set looping status to off
        triggerModeData.playingStatus = 2; //ignore
        shouldLoop = false;
        playingLastLoop = true;
        broadcaster.sendActionMessage("WAITING TO STOP");
        
        //what about if the user wants to cancel the finish loop command?
        //curently, if the user presses the pad again it will restart call
        //the below if else statement
    }
    else if (triggerModeData.playingStatus == 1 
             && playingLastLoop == true 
             && indestructible == 0 
             && currentPlayingState == 1)
    {
        playingLastLoop = false;
        shouldLoop = PAD_SETTINGS->getSequencerShouldLoop();
        triggerModeData.playingStatus = 1;
    }
    
    
    //for cycle trigger Mode
    if (triggerModeData.moveToNextSeq == true)
    {
        //if true, move to next seq
        sequenceNumber++;
        
        if (sequenceNumber == numberOfSequences) //if goes beyond the last sequence
        {
            if (shouldLoop == true)
            {
                sequenceNumber = 0; //if reached end of seqs and is set to loop, go back to the beginning
            }
            else if (shouldLoop == false)
            {
                stopSequence();
            }
        }
    }
    
    
    //==========================================================================================
    // Start/Stop stuff
    //==========================================================================================
    
    //if triggerModeData.playingStatus = 2, do nothing
    
    if (quantizeMode == 0) //free
    {
        if (triggerModeData.playingStatus == 1) //play
        {
            if (sequenceIsRunning == true)
            {
                //stopThread(timeInterval); //so that the 'trigger' play states work ()
                columnNumber = 0; //stopping and starting the thread causes a bigger time delay
                broadcaster.sendActionMessage("PLAYING ON");
            }
            
            sequenceNumber = 0;
            startSequence();
            currentPlayingState = 1;
            
            //EXCLUSIVE MODE STUFF 
            if (PAD_SETTINGS->getExclusiveMode() == 1)
            {
                broadcaster.sendActionMessage("EXCLUSIVE STOP");
            }
            
        }
        
        else if (triggerModeData.playingStatus == 0) //stop
        {
            currentPlayingState = 0;
            stopSequence();
            //stopThreadAndReset();
    
        }

    }
    
    //==========================================================================================
    else if (quantizeMode == 1) //quantized
    {
        if (triggerModeData.playingStatus == 1) //play
        {
            sequenceNumber = 0;
            currentPlayingState = 2; //waiting to play
            
            //add this objects padNumber to the queuedPads Array within AlphaLiveEngine
            //so that it is alerted of the next quantized point in time so
            //the sequence can start playing
            modeSequencerRef.getAlphaLiveEngineRef().addPadToQueue(padNumber);
            broadcaster.sendActionMessage("WAITING TO PLAY");
            
            
        }
        else if (triggerModeData.playingStatus == 0) //stop
        {
            currentPlayingState = 3; // waiting to stop
            
            //add this objects padNumber to the queuedPads Array within AlphaLiveEngine
            //so that it is alerted of the next quantized point in time so
            //the sequence can stop playing
            modeSequencerRef.getAlphaLiveEngineRef().addPadToQueue(padNumber);
            broadcaster.sendActionMessage("WAITING TO STOP");
        }
    }
    
    //exclusive channel pressure stuff.
    if (prevPadValue == 0) //pad pressed
    {
        if (midiChannelPressureMode == 1)
        {
            if (midiPressureStatus == true)
            {
                //assign the pressed pad number to the relevent variable in the
                //MidiChannelPressureHolder struct...
                
                if (midiPressureMode == 1) //channel aftertouch
                {
                    PRESSURE_HOLDER->aftertouch = padNumber;
                }
                else if (midiPressureMode == 2) //mod wheel
                {
                    PRESSURE_HOLDER->controlChange[1] = padNumber;
                }
                else if (midiPressureMode == 3) //CC data
                {
                    PRESSURE_HOLDER->controlChange[midiControllerNumber] = padNumber;
                }
                else if (midiPressureMode == 4 || midiPressureMode == 5) //pitch bend
                {
                    PRESSURE_HOLDER->pitchBend = padNumber;
                }
            }
        }
    }
    
    
    //==========================================================================================
      
    //==========================================================================================
    // Pressure stuff
    //==========================================================================================
        
    //=== sticky mode stuff ===
    
    if (sticky == 1) //'on'
    {
        //modify pressure value
        if(prevPadValue == 0)
        {
            pressureValue = 0;
        }
        else
        {
            if(padValue > pressureValue)
            {
                pressureValue = padValue;
            }
            //else if it is smaller, don't change the pressure value
        }
    }
    else // 'off'
    {
        pressureValue = padValue; 
    }
      
    
    //======create pressure data======
    
    if (mode == 1) //midi
    {
        sharedMemoryMidi.enter();
        
        //create midi pressure data
        sendMidiPressureData();
        
        sharedMemoryMidi.exit();
    }
    else if (mode == 2)
    {
        //determine what effect and parameter the pressure is controlling
        
        switch (effect)
        {
            case 1: //Gain and Pan
                gainAndPan->processAlphaTouch(pressureValue);
                break;
            case 2: //LPF
                lowPassFilter->processAlphaTouch(pressureValue);
                break;
            case 3: //HPF
                highPassFilter->processAlphaTouch(pressureValue);
                break;
            case 4: //BPF
                bandPassFilter->processAlphaTouch(pressureValue);
                break;
			case 5: //Distortion
                distortion->processAlphaTouch(pressureValue);
                break;
			case 6: //bitcrusher
                bitcrusher->processAlphaTouch(pressureValue);
                break;
            case 7: //Delay
                delay->processAlphaTouch(pressureValue);
                break;
            case 8: //Reverb
                reverb->processAlphaTouch(pressureValue);
                break;
            case 9: //Flanger
                flanger->processAlphaTouch(pressureValue);
                break;
            case 10: //Tremolo
                tremolo->processAlphaTouch(pressureValue);
                break;
            default:
                break;
        }
    }
    
    //exclusive channel pressure stuff.
    if (padValue == 0) //pad released
    {
        if (midiChannelPressureMode == 1)
        {
            if (midiPressureStatus == true)
            {
                //remove the released pad number from the relevent variable in the
                //MidiChannelPressureHolder struct if it exists there...
                
                if (midiPressureMode == 1) //channel aftertouch
                {
                    if (PRESSURE_HOLDER->aftertouch == padNumber)
                    {
                        PRESSURE_HOLDER->aftertouch = -1;
                    }
                }
                else if (midiPressureMode == 2) //mod wheel
                {
                    if (PRESSURE_HOLDER->controlChange[1] == padNumber)
                    {
                        PRESSURE_HOLDER->controlChange[1] = -padNumber;
                    }
                }
                else if (midiPressureMode == 3) //CC data
                {
                    if (PRESSURE_HOLDER->controlChange[midiControllerNumber] == padNumber)
                    {
                        PRESSURE_HOLDER->controlChange[midiControllerNumber] = -1;
                    }
                }
                else if (midiPressureMode == 4 || midiPressureMode == 5) //pitch bend
                {
                    if (PRESSURE_HOLDER->pitchBend == padNumber)
                    {
                        PRESSURE_HOLDER->pitchBend = -1;
                    }
                }
            }
        }
    }
    
    
    //==== control sequence arrangement with pressure====
    
    //Should this stuff be put BEFORE the sticky mode stuff
    //so that dynamic mode isn't affected by sticky mode?
    
    if (dynamicMode == 1)
    {
        if (triggerModeData.ignorePressure == false) //ignore for certain triggerModes
        {
            //scale 0-MAX_PRESSURE to 0-numberOfSequences
            
            sequenceNumber = scaleValue(pressureValue, 0, MAX_PRESSURE, 0, numberOfSequences);
            if (sequenceNumber > numberOfSequences-1)
                sequenceNumber  = numberOfSequences-1;
        }
    }
    
    
    //===============update sequencer grid GUI here!====================
    //if the this instance of SequencePlayer is the one belonging to the pad 
    //that is currently selected (and hence currently displayed)
    if (sequenceNumber != prevSequenceNumber) //so that an update will only be sent of the sequence number has changed!
    {
        if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1
            && sequenceNumber < numberOfSequences)
        {
            if (AppSettings::Instance()->getCurrentlySelectedPad()[0] == padNumber)
            {
                broadcaster.sendActionMessage("SEQ DISPLAY");
            }
        }
        
    }
    
    prevSequenceNumber = sequenceNumber;
    prevPadValue = padValue;
}



void SequencePlayer::triggerQuantizationPoint()
{
    if (currentPlayingState == 2) //waiting to play
    {
        if (sequenceIsRunning == true)
        {
            //stopThread(timeInterval); //so that the 'trigger' play states work ()
            columnNumber = 0; //stopping and starting the thread causes a bigger time delay
            broadcaster.sendActionMessage("PLAYING ON");
        }
        
        startSequence();
          
        currentPlayingState = 1;
        
        //EXCLUSIVE MODE STUFF 
        if (PAD_SETTINGS->getExclusiveMode() == 1)
        {
            broadcaster.sendActionMessage("EXCLUSIVE STOP");
        }
        
    }
    
    else if (currentPlayingState == 3) //waiting to stop
    {
        //done via actionMessage so that there is no build up of delays caused by the stopThread() time value
        sequenceFlaggedToStop = true;
        broadcaster.sendActionMessage("STOP SEQ AND RESET");
        
        //tell gui pad that the sequence has finished playing
        broadcaster.sendActionMessage("PLAYING OFF");
    }
}






void SequencePlayer::startSequence()
{
    currentTime = Time::getMillisecondCounterHiRes();
    columnNumber = 0; //counter variable
    
    sharedMemoryMidiClock.enter();
    midiClockMessageCounter = 6;
    sharedMemoryMidiClock.exit();
    
    if (mode == 2)
    {
        //set the state of certain effects
        if (effect == 9) //flanger
            flanger->restart(); //so the lfo is re-started when the file is started
        else if (effect == 10) //tremolo
            tremolo->restart(); //so the lfo is re-started when the file is started
    }
    
    //initally reset sequence display
    if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1)
    {
        if (AppSettings::Instance()->getCurrentlySelectedPad()[0] == padNumber)
        {
            prevSequenceNumber = sequenceNumber;
            broadcaster.sendActionMessage("SEQ DISPLAY");
        }
    }
    
    //tell the pad gui that the sequence is playing
    broadcaster.sendActionMessage("PLAYING ON"); 
    
    sequenceFlaggedToStop = false;
    sequenceIsRunning = true;
    
    //add this pads SequencerPlayer object to the running sequencer array
    //so that processSequence() will be called repeatedly 
    modeSequencerRef.editRunningSequencersArray(1, padNumber);
    
}







void SequencePlayer::processSequence()
{
    //sequenceFlaggedToStop is needed as in certain sitations (such as at the end of a non-looping sequence)
    //the sequence is triggered to stop asyncronously, however this means that procesSequence() could be
    //called in between calling sendActionMessage and the actual action being carried out, which is not what
    //we want.
    
    //detmine whether the sequence needs to be processed at this point in time depending
    //on current time and/or MIDI Clock status
    bool processSeq = false;
    
    //When not syncing to an external MIDI clock messages
    if (!currentlySyncedToMidiClockMessages)
    {
        if (Time::getMillisecondCounterHiRes() >= currentTime && sequenceFlaggedToStop == false)
        {
            processSeq = true;
        }
    }
    //when synced to MIDI clock messages
    else
    {
        sharedMemoryMidiClock.enter();
        
        if (midiClockMessageCounter >= 6)
        {
            processSeq = true;
            midiClockMessageCounter = 0;
        }
        
        sharedMemoryMidiClock.exit();
    }
    
    
    if (processSeq)
    {
        if (columnNumber >= sequenceLength || sequenceNumber >= numberOfSequences)
        {
            //The following paramaters cause the sequence to end:
            // - columnNumber becomes greater than sequenceLength when shouldLoop is set to false from the current trigger mode.
            // - sequenceNumber becomes greater than numberOfSequences when triggerModeData.isLinearCycle is set to true from the autocyclelinear triggerMode.
            
            sequenceFlaggedToStop = true;
            broadcaster.sendActionMessage("STOP SEQ");
        }
        else
        {
            //play next sequence point....
            
            currentTime = currentTime + timeInterval;
            
            if (mode == 1) //midi mode selected
            {
                sharedMemoryMidi.enter();
                
                //cycle through each row to look for any note messages
                for (int rowNumber = 0; rowNumber <= NO_OF_ROWS-1; rowNumber++)
                {
                    //look for note-off messages before note-on messages,
                    //as otherwise if you set the note length to be max length,
                    //the note will be turned off straight away due to how note length
                    //is implemented below
                    
                    //check for any midi notes that need to be turned off
                    //need to check ALL sequences, not just current one
                    for (int seq = 0; seq <= numberOfSequences-1; seq++)
                    {
                        if (midiNoteOffTime[seq][rowNumber][columnNumber] == 1)
                        {
                            triggerMidiNoteOffMessage(rowNumber);
                            
                            //set a flag that this particular note in the array will be now off
                            midiNoteOffTime[seq][rowNumber][columnNumber] = 0;
                            midiNoteOnCounter--;
                        }
                    }
                    
                    //then check for any midi note-on messages
                    if (sequenceData[sequenceNumber][rowNumber][columnNumber] >= 1 && 
                        recentlyAddedSequenceData[sequenceNumber][rowNumber][columnNumber] == false) //if 'on' and not a 'recently recorded' notes
                    {
                        int velocity = sequenceData[sequenceNumber][rowNumber][columnNumber];
                        //trigger note-on message
                        triggerMidiMessage(rowNumber, velocity);
                        
                        //set a flag that this particular note in the array will be on and
                        //will need to be 'turned off' at after a certain amount of time
                        //set the 'columnNumber' step number that the note will need to be turned off
                        int noteOffStep = columnNumber+midiNoteLength;
                        
                        //if the step number is greater than the number of columns,
                        //set it to 'overlap' into the next columnNumber cycle
                        if (noteOffStep >= sequenceLength)
                        {
                            noteOffStep = noteOffStep - sequenceLength;
                        }
                        midiNoteOffTime[sequenceNumber][rowNumber][noteOffStep] = 1;
                        midiNoteOnCounter++;
                        
                    }
                    else if (recentlyAddedSequenceData[sequenceNumber][rowNumber][columnNumber] == true)
                    {
                        recentlyAddedSequenceData[sequenceNumber][rowNumber][columnNumber] = false;
                    }
                }
                
                sharedMemoryMidi.exit();
            }
            
            //=========================================================================
            //=Samples stuff - everything else is for the midi mode!===================
            //=========================================================================
            else //samples mode selected
            {
                //cycle through each row to look for any note on messages
                for (int rowNumber = 0; rowNumber <= NO_OF_ROWS-1; rowNumber++)
                {
                    if (sequenceData[sequenceNumber][rowNumber][columnNumber] >= 1 &&
                        recentlyAddedSequenceData[sequenceNumber][rowNumber][columnNumber] == false) //if 'on' and not a recently recorded note
                    {
                        int velocity = sequenceData[sequenceNumber][rowNumber][columnNumber];
                        triggerAudioMessage(rowNumber, velocity);
                    }
                    else if (recentlyAddedSequenceData[sequenceNumber][rowNumber][columnNumber] == true)
                    {
                        recentlyAddedSequenceData[sequenceNumber][rowNumber][columnNumber] = false;
                    }
                }
            }
            
            //=========================================================================
            
            columnNumber++;
            
            
            //for autocycle trigger Mode
            if (triggerMode == 6)
            {
                
                if (columnNumber == sequenceLength && playingLastLoop == true)
                {
                    sequenceFlaggedToStop = true;
                    broadcaster.sendActionMessage("STOP SEQ"); //ends the 'last' loop
                }
                
                else if (columnNumber == sequenceLength) //if goes beyond the last column of the sequence
                {
                    sequenceNumber++;
                    columnNumber = 0; //reset columnNumber here otherwise thread loop will exit and seq will stop
                    
                    if (sequenceNumber == numberOfSequences) //if goes beyond the last sequence
                    {
                        if (shouldLoop == true)
                            sequenceNumber = 0; //if reached end of seqs and is set to loop, go back to the beginning
                        
                        //else, sequencer number will greater than the number of sequences, so the while loop will not run next time
                    }
                    
                    //update gui (if this pad's gui is currently being displayed)
                    if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1
                        && sequenceNumber < numberOfSequences)
                    {
                        if (AppSettings::Instance()->getCurrentlySelectedPad()[0] == padNumber)
                            broadcaster.sendActionMessage("SEQ DISPLAY");
                    }
                }
                
            }
            
            
            //if the counter variable reaches the end, and the current play state is set to loop or triggerMode
            //is 'cycle' and it is not currently playing the 'last loop',
            //restart the counter so that the sequence loops
            //otherwise the while loop will exit and the thread will stop on it's own
            if ((columnNumber == sequenceLength && shouldLoop == true) || 
                (columnNumber == sequenceLength && triggerMode == 5 && playingLastLoop == false))
                columnNumber = 0;
            
            
            //===============update sequencer grid GUI here!====================
            //if the this instance of SequencePlayer is the one belonging to the pad 
            //that is currently selected (and hence currently displayed)
            if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1)
            {
                if (AppSettings::Instance()->getCurrentlySelectedPad()[0] == padNumber)
                    broadcaster.sendActionMessage("PLAYHEAD");
            }
        }
    }
    
}





void SequencePlayer::stopSequence()
{
    //===============trigger midi notes off here!====================
    //ideally, when the sequence is told to stop, any notes still on should be turned off within the length of time they were set to be on for.
    //That is what I have tried to do with the sendActionMessage call below, but it is still very tempermental.
    //So for now, when the sequencer is told to stop, all hanging midi notes are instantly turned off.
    //This means sequences can be retriggered quickily without any problems.
    //However using a piano roll implementation for MIDI sequences will more-than-likely solve this problem.
    
    //broadcaster.sendActionMessage("END MIDI SEQ");
    
    sharedMemoryMidi.enter();
    
    //instantly turn off any hanging notes
    for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
    {
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
            {
                if (midiNoteOffTime[seq][row][column] == 1)
                {
                    triggerMidiNoteOffMessage(row);
                    //set a flag that this particular note in the array will be now off
                    midiNoteOffTime[seq][row][column] = 0;
                }
            }
        }
    }
    
    sharedMemoryMidi.exit();
    
    
    playingLastLoop = false;
    shouldLoop = PAD_SETTINGS->getSequencerShouldLoop();
    
    //Update sequencer grid
    //if the this instance of SequencePlayer is the one belonging to the pad 
    //that is currently selected (and hence currently displayed)
    if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1)
    {
        columnNumber = -1;
        
        if (AppSettings::Instance()->getCurrentlySelectedPad()[0] == padNumber)
            broadcaster.sendActionMessage("PLAYHEAD");
    }
    
    //tell gui pad that the sequence has finished playing
    broadcaster.sendActionMessage("PLAYING OFF");
    
    modeSequencerRef.editRunningSequencersArray(0, padNumber);
    
    sequenceIsRunning = false;
}





void SequencePlayer::stopSequenceAndReset()
{
    stopSequence();

    triggerModes.reset();
    currentPlayingState = 0;
    
//    if (sequenceIsRunning == false)
//    {
//        //this stuff as also at the bottom of the run() function, which won't be called
//        //if the thread isn't running
//        playingLastLoop = false;
//        shouldLoop = PAD_SETTINGS->getSequencerShouldLoop();
//        //tell gui pad that the sequence has finished playing
//        broadcaster.sendActionMessage("PLAYING OFF"); 
//    }
    
    if (mode == 2)
    {
        if (effect == 7) //delay
            delay->resetBuffers();
        else if (effect == 9) //flanger
            flanger->resetBuffers();
    }
    
}




//=====================================================================================
//=Create a note stuff=================================================================
//=====================================================================================


//called every time a midi message needs triggering
void SequencePlayer::triggerMidiMessage(int rowNumber, int velocity)
{
    if (midiNote[rowNumber] >= 0)
    {
        MidiMessage message = MidiMessage::noteOn(midiChannel, midiNote[rowNumber], (uint8)velocity);
        modeSequencerRef.getAlphaLiveEngineRef().sendMidiMessage(message);
    }
}

void SequencePlayer::triggerMidiNoteOffMessage (int rowNumber)
{
    if (midiNote[rowNumber] >= 0)
    {
        MidiMessage message = MidiMessage::noteOff(midiChannel, midiNote[rowNumber]);
        modeSequencerRef.getAlphaLiveEngineRef().sendMidiMessage(message);
    }
}

void SequencePlayer::sendMidiPressureData()
{
    //scale 0-MAX_PRESSURE to minPressure-maxPressure
    //this has to be done here and not above with the sticky mode code,
    //as the minRange could be set higher than the maxRange, which would mean
    //the sticky feature wouldn't work how it's meant to. 
    //Also we need to use a new variable here to covert the midi data,
    //so that sticky will still work correctly in all situations
    
    int pressureValueScaled = scaleValue (pressureValue,
                                          0,
                                          MAX_PRESSURE,
                                          midiMinRange,
                                          midiMaxRange);
    
    if (midiPressureStatus == true) //if pad pressure status is 'off'
    {
        MidiMessage message;
        
        //create 'pressure' data
        switch (midiPressureMode)
        {
            case 1: //channel aftertouch
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->aftertouch != padNumber)
                {
                    break;
                }
                else
                {
                    message = MidiMessage::channelPressureChange(midiChannel, pressureValueScaled);
                    modeSequencerRef.getAlphaLiveEngineRef().sendMidiMessage(message);
                    break;
                }
                
            case 2: // mod wheel
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->controlChange[1] != padNumber)
                {
                    break;
                }
                else
                {
                    message = MidiMessage::controllerEvent(midiChannel, 1, pressureValueScaled);
                    modeSequencerRef.getAlphaLiveEngineRef().sendMidiMessage(message);
                    break;
                }
                
            case 3: //CC messages
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->controlChange[midiControllerNumber] != padNumber)
                {
                    break;
                }
                else
                {
                    message = MidiMessage::controllerEvent(midiChannel, midiControllerNumber, pressureValueScaled);
                    modeSequencerRef.getAlphaLiveEngineRef().sendMidiMessage(message);
                    break;
                }
                
            case 4: //pitch bend up
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->pitchBend != padNumber)
                {
                    break;
                }
                else
                {
                    //convert 0-127 to 8191-16383
                    pressureValueScaled = 8192 + (pressureValueScaled * (8191.0/127.0));
                    message = MidiMessage::pitchWheel(midiChannel, pressureValueScaled);
                    modeSequencerRef.getAlphaLiveEngineRef().sendMidiMessage(message);
                    break;
                }
                
            case 5: //pitch bend down
                
                if (midiChannelPressureMode == 1 && PRESSURE_HOLDER->pitchBend != padNumber)
                {
                    break;
                }
                else
                {
                    //convert 0-127 to 8191-0
                    pressureValueScaled = 8192 - (pressureValueScaled * (8191.0/127.0));
                    message = MidiMessage::pitchWheel(midiChannel, pressureValueScaled);
                    modeSequencerRef.getAlphaLiveEngineRef().sendMidiMessage(message);
                    break;

                }
                
            default:
                message = MidiMessage::channelPressureChange(midiChannel, pressureValueScaled);
                modeSequencerRef.getAlphaLiveEngineRef().sendMidiMessage(message);
                break;
        }
        
    }
    
}

//called every time an audio file needs playing
void SequencePlayer::triggerAudioMessage(int rowNumber, int velocity)
{
    sequenceAudioFilePlayer[rowNumber]->playAudioFile(velocity*(1.0/127.0));
}



//=====================================================================================
//=AudioSource Stuff===================================================================
//=====================================================================================

void SequencePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // DO AS LITTLE IN HERE AS POSSIBLE. DON'T ATTEMPT TO CALL TO MANY FUNCTIONS
    //do not call anything from AppSettings in here. Only call member variables of this class,
    //otherwise it is to CPU heavy!
    
    audioMixer.getNextAudioBlock(bufferToFill);
    
    sharedMemory.enter();
    if (/*fileSource.isPlaying() == true &&*/ effect != 0)
    {
        
        switch (effect) 
        {
            case 1: //Gain and Pan
                gainAndPan->processAudio(bufferToFill);
                break;
            case 2: //LPF
                lowPassFilter->processAudio(bufferToFill);
                break;
            case 3://HPF
                highPassFilter->processAudio(bufferToFill);
                break;
            case 4: //BPF
                bandPassFilter->processAudio(bufferToFill);
                break;
			case 5: //Distortion
                distortion->processAudio(bufferToFill);
                break;
			case 6: //Bitcrusher
                bitcrusher->processAudio(bufferToFill);
                break;
            case 7: //Delay
                delay->processAudio(bufferToFill);
                break;
            case 8: //Reverb
                reverb->processAudio(bufferToFill);
                break;
            case 9: //Flanger
                flanger->processAudio(bufferToFill);
                break;
            case 10: //Tremolo
                tremolo->processAudio(bufferToFill);
                break;
            default:
                break;
        }
        
    }
    sharedMemory.exit();
    
    //gain and pan
    sharedMemory.enter();
    
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
        if (i == 0) //left chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                panLeftPrev * gainPrev,
                                                panLeft * gain);
        else if (i == 1) // right chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                panRightPrev * gainPrev,
                                                panRight * gain);
    }
    
    gainPrev = gain;
    panLeftPrev = panLeft;
    panRightPrev = panRight;
    sharedMemory.exit();
    
}

void SequencePlayer::prepareToPlay (int samplesPerBlockExpected,double sampleRate)
{
    audioMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    sampleRate_ = sampleRate;
    
    switch (effect)
    {
        case 1:
            gainAndPan->setSampleRate(sampleRate);
            break;
        case 2:
            lowPassFilter->setSampleRate(sampleRate);
            break;
        case 3:
            highPassFilter->setSampleRate(sampleRate);
            break;
        case 4:
            bandPassFilter->setSampleRate(sampleRate);
            break;
		case 5:
            distortion->setSampleRate(sampleRate);
            break;	
		case 6:
            bitcrusher->setSampleRate(sampleRate);
            break;
        case 7:
            delay->setSampleRate(sampleRate);
            break;
        case 8:
            reverb->setSampleRate(sampleRate);
            break;
        case 9:
            flanger->setSampleRate(sampleRate);
            break;
        case 10:
            tremolo->setSampleRate(sampleRate);
            break;
        default:
            break;
    }
}

void SequencePlayer::releaseResources()
{
    audioMixer.releaseResources();
}

//=====================================================================================
//=actionListenerCallback==============================================================
//=====================================================================================


void SequencePlayer::actionListenerCallback (const String& message)
{
    //update playhead gui
    if (message == "PLAYHEAD")
        modeSequencerRef.updateSequencerGridGui (columnNumber, sequenceNumber, 1);
    
    //update sequence display gui
    else if (message == "SEQ DISPLAY")
        modeSequencerRef.updateSequencerGridGui (columnNumber, sequenceNumber, 2);
    
    //update gui playing signifier
    else if (message == "PLAYING ON")
        modeSequencerRef.updatePadPlayingStatus(padNumber, 1);
    
    //update gui playing signifier
    else if (message == "PLAYING OFF")
        modeSequencerRef.updatePadPlayingStatus(padNumber, 0);
    
    else if (message == "WAITING TO PLAY")
        modeSequencerRef.updatePadPlayingStatus(padNumber, 2);
    
    else if (message == "WAITING TO STOP")
        modeSequencerRef.updatePadPlayingStatus(padNumber, 3);
    
    else if (message == "EXCLUSIVE STOP")
    {
        //this is called async so that there is no noticable time delay caused when
        //doing this via quantization mode (which is caused by the time value
        //paramater within the stopThread() function.
        modeSequencerRef.getAlphaLiveEngineRef().handleExclusiveMode(padNumber);

    }
    
    else if (message == "STOP SEQ")
    {
        stopSequence();
    }
    
    else if (message == "STOP SEQ AND RESET")
    {
        stopSequence();
        currentPlayingState = 0;
        //stopThreadAndReset();
    }
        

    /*
    //turn off 'hanging' midi notes
    if (message == "END MIDI SEQ")
    {
        //can't use currentTime and ColumnNumber as they may be being used above again. So trasnfer the values into new variables
        float currentTimeEnd = currentTime;
        int columnNumberEnd = columnNumber;
        while (midiNoteOnCounter > 0)//while there are notes on
        {
            Time::waitForMillisecondCounter(currentTimeEnd);
            currentTimeEnd += TIME_INTERVAL;
            
            //cycle through each row to look for any note off messages
            for (int rowNumber = 0; rowNumber <= NO_OF_ROWS-1; rowNumber++)
            {
                for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
                {
                    if (midiNoteOffTime[seq][rowNumber][columnNumberEnd] == 1)
                    {
                        triggerMidiNoteOffMessage(rowNumber);
                        
                        //set a flag that this particular note in the array will be now off
                        midiNoteOffTime[seq][rowNumber][columnNumberEnd] = 0;
                        midiNoteOnCounter--;
                    }
                }
            }
            columnNumberEnd++;
            //if the counter variable reaches the end and the current play state is set to loop, restart the counter.
            //otherwise the while loop will exit 
            if (columnNumberEnd == sequenceLength && shouldLoop == true)
                columnNumberEnd = 0;
        }
        
        std::cout << "All seq notes turned off!" << std::endl;
    }
     */
    
}
 

int SequencePlayer::getCurrentPlayingState()
{
    return currentPlayingState;
}

bool SequencePlayer::isCurrentlyPlaying()
{
    return sequenceIsRunning;
}

void SequencePlayer::setSequenceNumber (int value)
{
    sequenceNumber = value;
}

int SequencePlayer::getSequenceNumber()
{
    return sequenceNumber;
}

Array<int> SequencePlayer::getClosestColumnNumber()
{
    //get the exact system time and compare it to currentTime
    //if the exact time is closer to current time, return the current column number
    //if the exact time is closer to currentTime - timeInterval, return the last column
    //is this the right algorithm?
    
    int exactTime = Time::getMillisecondCounter();
    int timeFromLastColumn = exactTime - (currentTime - timeInterval); 
    int timeTillNextColumn = currentTime - exactTime;
    
    //index 0 stores closest column number, 
    //index 1 stores whether this is the last column or the current/next column
    Array <int> closestColumnData; 
    
    if (timeFromLastColumn <= timeTillNextColumn) //closer to last column
    {
        int lastColumnNumber = columnNumber - 1;
        if (lastColumnNumber < 0)
            lastColumnNumber = sequenceLength - 1;
        
        closestColumnData.insert(0, lastColumnNumber);
        closestColumnData.insert(1, 0);
        
        //std::cout << "returning last column number..." << std::endl;
    }
    else //timeTillNextColumn < timeFromLastColumn, closer to next column
    {
        //sstd::cout << "returning current column number..." << std::endl;
        
        closestColumnData.insert(0, columnNumber);
        closestColumnData.insert(1, 1);
    }
    
    //std::cout << std::endl;
    
    return closestColumnData;
}

//=====================================================================================
//=Settings Stuff======================================================================
//=====================================================================================


void SequencePlayer::setSamplesAudioFile (int row, File sample)
{
    sequenceAudioFilePlayer[row]->setAudioFile(sample);
}

void SequencePlayer::setRelativeTempoMode (int value)
{
    sharedMemory.enter();
    relativeTempoMode = value;
    sharedMemory.exit();
    setTempo(AppSettings::Instance()->getGlobalTempo());
    
}

void SequencePlayer::setTempo(float value)
{
    sharedMemory.enter();
    
    switch (relativeTempoMode)
    {
    case -2: //quarter time
        tempo = value/4.0;
        break;
        
    case -1: //half time
        tempo = value/2.0;
        break;
    
    case 0: //regular time
        tempo = value;
        break;
        
    case 1: //double time
        tempo = value*2.0;
        break;
        
    case 2: //quadrupal time
        tempo = value*4.0;
        break;
        
    default: //regular time
        tempo = value;
        break;
    }
    
    timeInterval = double(15000.0)/tempo;
    
    sharedMemory.exit();
}

void SequencePlayer::setSequenceData(int seq, int row, int column, int value)
{
    sequenceData[seq][row][column] = value;
}

void SequencePlayer::setMode(int value)
{
    //if new mode is set to samples mode, create the SequenceAudioFilePlayer objects
    if (value == 2 && mode != value)
    {
        //audio stuff
        //init audio file player objects, 1 for each row
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            sequenceAudioFilePlayer[row] = new SequenceAudioFilePlayer(padNumber, row, audioTransportSourceThread);
            audioMixer.addInputSource(sequenceAudioFilePlayer[row], false); //add as inputsource to audioMixer
        }
    }
    //if was previously set to samples mode but new mode isn't, remove the SequenceAudioFilePlayer objects
    else if (mode == 2 && value != 2)
    {
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            delete sequenceAudioFilePlayer[row];
        }
        
        audioMixer.removeAllInputs();
    }
    
    
    //if previous mode was MIDI, prevent any hanging midi notes
    if (mode == 1 && mode != value && sequenceIsRunning == true)
    {
        for (int i = 0; i < NO_OF_ROWS; i++)
        {
            triggerMidiNoteOffMessage(i);
        }
    }
    
    mode = value;
}

void SequencePlayer::setRecentlyAddedSequenceData (int sequenceNumber, int rowNumber, int columnNumber, bool value)
{
    recentlyAddedSequenceData[sequenceNumber][rowNumber][columnNumber] = value;
}

void SequencePlayer::setNumberOfSequences (int value)
{
    numberOfSequences = value;
}
void SequencePlayer::setTriggerMode (int value)
{
    triggerMode = value;
}
void SequencePlayer::setShouldLoop (int value)
{
    if (playingLastLoop == false)
        shouldLoop = value;
}
void SequencePlayer::setIndestructible (int value)
{
    indestructible = value;
}
void SequencePlayer::setShouldFinishLoop (int value)
{
    shouldFinishLoop = value;
}
void SequencePlayer::setSticky (int value)
{
    sticky = value;
}

void SequencePlayer::setSequenceLength (int value)
{
    sequenceLength = value;
}

void SequencePlayer::setQuantizeMode (int value)
{
    quantizeMode = value;
}

void SequencePlayer::setDynamicMode (int value)
{
    dynamicMode = value;
}

void SequencePlayer::setMidiNote (int row, int value)
{
    sharedMemoryMidi.enter();
    
    if (midiNote[row] != value && sequenceIsRunning == true)
    {
        triggerMidiNoteOffMessage(row);
    }
    
    midiNote[row] = value;
    
    sharedMemoryMidi.exit();
}

void SequencePlayer::setMidiChannel (int value)
{
    sharedMemoryMidi.enter();
    
    //if new channel is different from current channel, 
    //stop MIDI notes if seq is currently playing to prevent hanging notes
    
    if (midiChannel != value && sequenceIsRunning == true)
    {
        for (int i = 0; i < NO_OF_ROWS; i++)
        {
            triggerMidiNoteOffMessage(i);
        }
    }
    
    midiChannel = value;
    
    sharedMemoryMidi.exit();
}
void SequencePlayer::setMidiNoteLength (int value)
{
    sharedMemoryMidi.enter();
    midiNoteLength = value;
    sharedMemoryMidi.exit();
}
void SequencePlayer::setMidiMinRange (int value)
{
    sharedMemoryMidi.enter();
    midiMinRange = value;
    sharedMemoryMidi.exit();
}

void SequencePlayer::setMidiMaxRange (int value)
{
    sharedMemoryMidi.enter();
    midiMaxRange = value;
    sharedMemoryMidi.exit();
}

void SequencePlayer::setMidiControllerNumber (int value)
{
    sharedMemoryMidi.enter();
    
    //is CC number has changed and pressure is currently > 0, reset the value of the current pressure data
    if (midiControllerNumber != value && pressureValue > 0)
    {
        pressureValue = 0; 
        sendMidiPressureData();
    }
    
    midiControllerNumber = value;
    
    sharedMemoryMidi.exit();
}

void SequencePlayer::setMidiPressureMode (int value)
{
    sharedMemoryMidi.enter();
    
    //if pressure mode has changed and pressure is currently > 0, reset the value of the current pressure data
    if (midiPressureMode != value && pressureValue > 0)
    {
        pressureValue = 0; 
        sendMidiPressureData();
    }
    
    midiPressureMode = value;
    
    sharedMemoryMidi.exit();
}


void SequencePlayer::setMidiPressureStatus (bool value)
{
    sharedMemoryMidi.enter();
    
    //if pressure status has changed and pressure is currently > 0, reset the value of the current pressure data
    if (midiPressureStatus != value && pressureValue > 0)
    {
        pressureValue = 0; 
        sendMidiPressureData();
    }
    
    midiPressureStatus = value;
    
    sharedMemoryMidi.exit();
}


void SequencePlayer::setSamplesEffect(int value)
{
    sharedMemory.enter();
    
    if (effect != value) //if the effect is being changed
    {
        //first, delete the current effect...
        switch (effect)
        {
            case 1:
                delete gainAndPan;
                gainAndPan = nullptr;
                break;
            case 2:
                delete lowPassFilter;
                lowPassFilter = nullptr;
                break;
            case 3:
                delete highPassFilter;
                highPassFilter = nullptr;
                break;
            case 4:
                delete bandPassFilter;
                bandPassFilter = nullptr;
                break;
			case 5:
                delete distortion;
                distortion = nullptr;
                break;
			case 6:
                delete bitcrusher;
                bitcrusher = nullptr;
                break;
            case 7:
                delete delay;
                delay = nullptr;
                break;
            case 8:
                delete reverb;
                reverb = nullptr;
                break;
            case 9:
                delete flanger;
                flanger = nullptr;
                break;
            case 10:
                delete tremolo;
                tremolo = nullptr;
                break;
            default:
                break;
        }
        
        //Next, create the new effect...
        switch (value)
        {
            case 1:
                gainAndPan = new GainAndPan (padNumber, sampleRate_);
                break;
            case 2:
                lowPassFilter = new LowpassFilter (padNumber, sampleRate_);
                break;
            case 3:
                highPassFilter = new HighPassFilter (padNumber, sampleRate_);
                break;
            case 4:
                bandPassFilter = new BandPassFilter (padNumber, sampleRate_);
                break;
			case 5:
                distortion = new Distortion (padNumber, sampleRate_);
                break;
			case 6:
                bitcrusher = new Bitcrusher (padNumber, sampleRate_);
                break;
            case 7:
                delay = new Delay (padNumber, sampleRate_);
                break;
            case 8:
                reverb = new ReverbClass (padNumber, sampleRate_);
                break;
            case 9:
                flanger = new Flanger (padNumber, sampleRate_);
                break;
            case 10:
                tremolo = new Tremolo (padNumber, sampleRate_);
                break;
            default:
                break;
        }
        
        //Finally, set the new effect to be the current effect
        effect = value;
    }
    sharedMemory.exit();
    
}


void SequencePlayer::setSamplesGain (float value)
{
    sharedMemory.enter();
    
    if (value <= 1.0)
        gain = value * value * value;
    else
        gain = value;
    
    sharedMemory.exit();
}
void SequencePlayer::setSamplesPan (float value)
{
    sharedMemory.enter();
    panLeft = PanControl::leftChanPan_(value);
    panRight = PanControl::rightChanPan_(value);
    sharedMemory.exit();
}

void SequencePlayer::setSamplesAttackTime (double value)
{
    for (int i = 0; i < NO_OF_ROWS; i++)
    {
        sequenceAudioFilePlayer[i]->setAttackTime(value);
    }
}

void SequencePlayer::setSamplesPolyphony (int value)
{
    for (int i = 0; i < NO_OF_ROWS; i++)
    {
        sequenceAudioFilePlayer[i]->setPolyphony(value);
    }
}

double SequencePlayer::getTimeInterval()
{
    return timeInterval;
}

void SequencePlayer::setMidiClockMessageTimestamp()
{
    sharedMemoryMidiClock.enter();
    midiClockMessageCounter++;
    sharedMemoryMidiClock.exit();
}

void SequencePlayer::setCurrentlySyncedToMidiClockMessages (bool value)
{
    currentlySyncedToMidiClockMessages = value;
}

void SequencePlayer::setMidiClockValue (int value)
{
    midiClockValue = value;
}

void SequencePlayer::setMidiClockMessageFilter (int value)
{
    midiClockMessageFilter = value;
}


//========================================================================================
GainAndPan& SequencePlayer::getGainAndPan()
{
    return *gainAndPan;
}
LowpassFilter& SequencePlayer::getLowpassFilter()
{
    return *lowPassFilter;
}

HighPassFilter& SequencePlayer::getHighPassFilter()
{
    return *highPassFilter;
}

BandPassFilter& SequencePlayer::getBandPassFilter()
{
    return *bandPassFilter;
}

Delay& SequencePlayer::getDelay()
{
    return *delay;
}

ReverbClass& SequencePlayer::getReverb()
{
    return *reverb;
}

Flanger& SequencePlayer::getFlanger()
{
    return *flanger;
}

Tremolo& SequencePlayer::getTremolo()
{
    return *tremolo;
}

Distortion& SequencePlayer::getDistortion()
{
	return *distortion;
}

Bitcrusher& SequencePlayer::getBitcrusher()
{
	return *bitcrusher;
}

void SequencePlayer::setMidiChannelPressureMode (int value)
{
    midiChannelPressureMode = value;
}
