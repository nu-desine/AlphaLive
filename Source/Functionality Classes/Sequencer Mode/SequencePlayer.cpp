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

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

//=====================================================================================
//=Constructor=========================================================================
//=====================================================================================

SequencePlayer::SequencePlayer(int padNumber_,MidiOutput &midiOutput, ModeSequencer &ref, TimeSliceThread* audioTransportSourceThread_) 
                                :   Thread("Sequencer " + String(padNumber)),
                                    padNumber(padNumber_),
                                    modeSequencerRef(ref),
                                    midiOutputDevice(&midiOutput)
{
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
    
    mode = PAD_SETTINGS->getSequencerMode();
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
    midiVelocity = PAD_SETTINGS->getSequencerMidiVelocity();
    midiNoteLength  = PAD_SETTINGS->getSequencerMidiNoteLength();
    midiMinRange = PAD_SETTINGS->getSequencerMidiMinPressureRange();
    midiMaxRange = PAD_SETTINGS->getSequencerMidiMaxPressureRange();
    midiControllerNumber = PAD_SETTINGS->getSequencerMidiCcController();
    midiPressureMode = PAD_SETTINGS->getSequencerMidiPressureMode();
    midiPressureStatus = PAD_SETTINGS->getSequencerMidiPressureStatus();
    
    gain = gainPrev = PAD_SETTINGS->getSequencerGain();
    panLeft = panLeftPrev = PanControl::leftChanPan_(PAD_SETTINGS->getSequencerPan());
    panRight = panRightPrev = PanControl::rightChanPan_(PAD_SETTINGS->getSequencerPan());
    
    recordEnabled = false;
    
    triggerModeData.playingStatus = 0;
    triggerModeData.moveToNextSeq = false;
    
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

    //audio stuff
    //init audio file player objects, 1 for each row
    //LOOK INTO DOING THIS STUFF ONLY WHEN MODE IS SET TO SAMPLES MODE?
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        sequenceAudioFilePlayer[row] = new SequenceAudioFilePlayer(padNumber, row, audioTransportSourceThread);
        audioMixer.addInputSource(sequenceAudioFilePlayer[row], false); //add as inputsource to audioMixer
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
    
    //set effect to default 0, and then call set effect to create the effect object
    //This alg. prevents any crashes caused within prepareToPlay when trying to
    //set the sampleRate, where the effect object must exist
    effect = 0;
    setSamplesEffect(PAD_SETTINGS->getSequencerEffect());
    
    prevPadValue = pressureValue =  0;
    playingLastLoop = false;
}

//=====================================================================================
//=Destructor==========================================================================
//=====================================================================================


SequencePlayer::~SequencePlayer()
{
    delete gainAndPan;
    delete lowPassFilter;
    delete highPassFilter;
    delete bandPassFilter;
    delete delay;
    delete reverb;
    delete flanger;
    delete tremolo;
    
    stopThread(timeInterval);
    //stopThreadAndReset();
    modeSequencerRef.updatePadPlayingStatus(padNumber, 0);
    
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        delete sequenceAudioFilePlayer[row];
    }
    
    audioMixer.removeAllInputs();
    audioPlayer.setSource(NULL);
     
}

//=====================================================================================
//=Process input data stuff============================================================
//=====================================================================================

void SequencePlayer::processSequence(int padValue)
{
    //this is needed incase audio ends on its own or is stopped via the 'exclusive mode' feature(?) , in order to reset everything so it will trigger again properly
    if (isThreadRunning() == false && currentPlayingState == 1 && prevPadValue == 0)
    {
        std::cout << "stream ended on its own!!";
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
        //case 3:
            //triggerModeData = triggerModes.toggleRelease(padValue);
            //break;
        case 3:
            triggerModeData = triggerModes.latch(padValue);
            break;
        //case 5:
           // triggerModeData = triggerModes.latchMax(padValue);
           // break;
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
        else if (triggerModeData.playingStatus == 1 && currentPlayingState == 1 && triggerMode != /*6*/4)
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
        std::cout << "here..." << std::endl;
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
                //stopThreadAndReset(); //if reaches end of seqs and not set to loop, stop thread
                //stopThread(timeInterval);
                triggerModeData.playingStatus = 0;
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
            if (isThreadRunning() == true)
            {
                //stopThread(timeInterval); //so that the 'trigger' play states work ()
                columnNumber = 0; //stopping and starting the thread causes a bigger time delay
                broadcaster.sendActionMessage("PLAYING ON");
            }
            
            sequenceNumber = 0;
            startThread(8);
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
            stopThread(timeInterval);
            //stopThreadAndReset();
    
        }

    }
    
    //==========================================================================================
    else if (quantizeMode == 1) //quantized
    {
        if (triggerModeData.playingStatus == 1) //play
        {
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
    
    
    //==========================================================================================
      
    //==========================================================================================
    // Pressure stuff
    //==========================================================================================
    //the below algorithms will need to change once the pressure predominantly controls midi
    //continuous data or DSP effects, with the sequence number being a secondary control.
    
    if (mode == 1) //midi
    {
        //scale 0-511 to 0-127
        //as the value will be scaled down and into an int, padValue could be rounded down to 0 even when it isn't quite 0
        //therefore we must make sure that it is atleast at the value of 1 untill it is actually set to 0,
        //so it doesn't mess up how the sticky feature is handled
        
        float padValueFloat = padValue * (127.0 / 511.0);
        
        if (padValueFloat > 0 && padValueFloat < 1)
            padValue = 1;
        else
            padValue = padValueFloat;
        
    }
    //else if audio mode, no scaling required
        
    //sticky stuff
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
        //create midi pressure data
        sendMidiPressureData();
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
    
    
    //==== control sequence arrangement with pressure====
    if (dynamicMode == 1)
    {
        if (triggerModeData.ignorePressure == false) //ignore for certain triggerModes
        {
            //scale 0-511/0-127 to 0-numberOfSequences
            
            //current problem with sequencer mode which will mean sticky mode might not function properly!!! - 
            //a pressure value of 0 is sequence number 1, whereas in other modes a pressure value of 0 is 'off'
            //therefore when using the above scaling algorithm which makes sure 0 isn't set till it is an absolute
            //0, the first sequence will never be played.
            //============================================================================================
            //Actually, if padValue and prevPadValue are never scaled, sticky works!! so ignore the above!!!
            //============================================================================================
            
            
            if (mode == 1) //midi, scale from 127
            {
                sequenceNumber = pressureValue * (numberOfSequences/127.0);
                if (sequenceNumber > numberOfSequences-1)
                    sequenceNumber  = numberOfSequences-1;
            }
            if (mode == 2) //audio, scale from 511
            {
                sequenceNumber = pressureValue * (numberOfSequences/511.0);
                if (sequenceNumber > numberOfSequences-1)
                    sequenceNumber  = numberOfSequences-1;
            }
        }
    }
    
    
    //===============update sequencer grid GUI here!====================
    //if the this instance of SequencePlayer is the one belonging to the pad 
    //that is currently selected (and hence currently displayed)
    if (sequenceNumber != prevSequenceNumber) //so that an update will only be sent of the sequence number has changed!
    {
        if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1)
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
        if (isThreadRunning() == true)
        {
            //stopThread(timeInterval); //so that the 'trigger' play states work ()
            columnNumber = 0; //stopping and starting the thread causes a bigger time delay
            broadcaster.sendActionMessage("PLAYING ON");
        }
        
        startThread(8);
          
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
        broadcaster.sendActionMessage("STOP THREAD");
        
        //tell gui pad that the sequence has finished playing
        broadcaster.sendActionMessage("PLAYING OFF");
    }
}


//=====================================================================================
//=Thread stuff========================================================================
//=====================================================================================

//called everytime a sequence is played/started
void SequencePlayer::run()
{
    //===================init stuff before thread loop here!===========================
    
    //THREAD TIMING
    //Time::waitForMillisecondCounter() takes in an int value, however the value of timeInterval will very likely
    //have a decimal point value (tempo value dependant). Therefore the decimal value would be ignored each time and
    //cause the timing of the thread/sequence to drift out of time.
    //Therefore an algorithm has been used to store any offset decimal values, and everytime the collection of values
    //equal or greater (though it should never be greater) than 1, 1ms is added to the intervalTime value to make up
    //for the 'ignored' decimal values. The code is commented below and at the bottom of the thread loop.
    //There's probably a way around this - have a look at the Time class - maybe use ticks not ms. 
    //In that case, what would I use instead of Time::waitForMillisecondCounter()?
    
    //set the time that the thread loop will intially sleep for
    currentTime = Time::getMillisecondCounter() + int(timeInterval);
    //get the decimal value of timeInterval
    float timeIntervalDecimalOffset = timeInterval - int(timeInterval);
    //set the currently stored offset value (this will change each time the loop is iterated)
    float currentOffset = timeIntervalDecimalOffset;

    columnNumber = 0; //counter variable
    //will this be ok being here?
    //sequenceNumber = 0;
    
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
    
    //====================thread loop starts here!====================================
    while( ! threadShouldExit() && columnNumber < sequenceLength && sequenceNumber < numberOfSequences)
    {
        //in the above while loop statement, the following paramaters cause the thread to end:
        // - threadShouldExit() becomes true when stopThread is called
        // - columnNumber becomes greater than sequenceLength when shouldLoop is set to false from the current play state. see bottom of this loop.
        // - sequenceNumber becomes greater than numberOfSequences when triggerModeData.isLinearCycle is set to true from the autocyclelinear triggerMode. see bottom of this while loop
        
        
        //PREVIOUSLY, waitForMillisecondCounter() WAS CALLED HERE, AND currentTime ABOVE WAS SET TO JUST Time::getMillisecondCounter().
        //HOWEVER NOW IT IS CALLED AT THE END OF THE WHILE LOOP AND THE INTIAL VALUE OF currentTime HAS BEEN CHANGED.
        //THIS HAS BEEN DONE AS BEFORE THERE WAS A BUG IN THAT WHEN THE THREAD WAS TOLD TO EXIT IT WOULD CUT OFF THE LAST NOTE BEING PLAYED
        //HOWEVER NOW THIS BUG DOES NOT OCCUR AS THE THREAD PAUSES AFTER THE SOUND HAS BEEN TRIGGER IN THE SAME INTERATION OF THE LOOP, NOT BEFORE.
        //IT ALSO SEEMS TO MAKE THE TRIGGER TriggerMode BEHAVE BETTER IN TERMS OF TIMING
        
        
        if (mode == 1) //midi mode selected
        {
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
        if (triggerMode == /*8*/ 6)
        {
            
            if (columnNumber == sequenceLength && playingLastLoop == true)
                signalThreadShouldExit(); //ends the 'last' loop
            
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
                if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1)
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
            (columnNumber == sequenceLength && triggerMode == /*7*/ 5 && playingLastLoop == false))
            columnNumber = 0;
        
        
        //===============update sequencer grid GUI here!====================
        //if the this instance of SequencePlayer is the one belonging to the pad 
        //that is currently selected (and hence currently displayed)
        if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1)
        {
            if (AppSettings::Instance()->getCurrentlySelectedPad()[0] == padNumber)
                broadcaster.sendActionMessage("PLAYHEAD");
        }
        
        //==================================================================
        //== SLEEP LOOP HERE!!! ============================================
        //==================================================================
        //sleep for int(timeInterval)
        Time::waitForMillisecondCounter(currentTime);
        
        //SHOULD THE BELOW SECTION OF CODE UP UNTIL currentTime = ... BE ABOVE THE ABOVE SLEEP CALL SO THAT THE REINITIALISATION OF
        //CURRENTTIME IS SET WITH A SMALLER POSSIBLE DELAY?
        
        //add the decimal offset of the time interval to the currently stored offset
        currentOffset = currentOffset + timeIntervalDecimalOffset;
        //get the offset that needs to be added. This will only make a difference if over than 1, otherwise it will equal 0.
        int currentOffsetToAdd = int(currentOffset);
        //change the value of currentTime so the next interation of the loop sleeps for the correct time,
        //possibly adding a value of 1 (currentlyOffsetToAdd) to make up for decimal values being ignored
        
        
        currentTime = currentTime + int(timeInterval) + currentOffsetToAdd;
        //if currentOffsetToAdd >= 1, remove from currentOffset as the value of 1 would have been added to currentTime
        if (currentOffsetToAdd >= 1)
        {
            currentOffset = currentOffset-currentOffsetToAdd;
        }
        
    }
    
    //IF PROGRAM GETS TO THIS POINT THREAD WILL EXIT
    
    //===============trigger midi notes off here!====================
    //ideally, when the sequence is told to stop, any notes still on should be turned off within the length of time they were set to be on for.
    //That is what I have tried to do with the sendActionMessage call below, but it is still very tempermental.
    //So for now, when the sequencer is told to stop, all hanging midi notes are instantly turned off.
    //This means sequences can be retriggered quickily without any problems.
    //However using a piano roll implementation for MIDI sequences will more-than-likely solve this problem.
    
    //broadcaster.sendActionMessage("END MIDI SEQ");
    
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
    
    
    playingLastLoop = false;
    shouldLoop = PAD_SETTINGS->getSequencerShouldLoop();
    
    //tell gui pad that the sequence has finished playing
    broadcaster.sendActionMessage("PLAYING OFF");
    
}


void SequencePlayer::stopThreadAndReset()
{
    stopThread(timeInterval*2);

    triggerModes.reset();
    currentPlayingState = 0;
    
    if (isThreadRunning() == false)
    {
        //this stuff as also at the bottom of the run() function, which won't be called
        //if the thread isn't running
        playingLastLoop = false;
        shouldLoop = PAD_SETTINGS->getSequencerShouldLoop();
        //tell gui pad that the sequence has finished playing
        broadcaster.sendActionMessage("PLAYING OFF"); 
    }
    
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
        sendMidiMessage(message);
    }
}

void SequencePlayer::triggerMidiNoteOffMessage (int rowNumber)
{
    if (midiNote[rowNumber] >= 0)
    {
        MidiMessage message = MidiMessage::noteOff(midiChannel, midiNote[rowNumber]);
        sendMidiMessage(message);
    }
}

void SequencePlayer::sendMidiPressureData()
{
    //scale 0-127 to midiMinPressure-midiMaxPressure
    //this has to be done here and not above with the 511 to 127 scaling,
    //as the minRange could be set higher than the maxRange, which would mean
    //the sticky feature wouldn't work how it's meant to. 
    //Also we need to use a new variable here to covert the midi data,
    //so that sticky will still work correctly in all situations
    int pressureValueScaled = midiMinRange + (pressureValue * ((midiMaxRange - midiMinRange) / 127.0));
    
    if (midiPressureStatus == true) //if pad pressure status is 'off'
    {
        MidiMessage message;
        
        //create 'pressure' data
        switch (midiPressureMode)
        {
            case 1: //channel aftertouch
                message = MidiMessage::channelPressureChange(midiChannel, pressureValueScaled);
                break;
            case 2: // mod wheel
                message = MidiMessage::controllerEvent(midiChannel, 1, pressureValueScaled);
                break;
            case 3: //CC messages
                message = MidiMessage::controllerEvent(midiChannel, midiControllerNumber, pressureValueScaled);
                break;
            case 4: //pitch bend up
                //convert 0-127 to 8191-16383
                pressureValueScaled = 8192 + (pressureValueScaled * (8191.0/127.0));
                message = MidiMessage::pitchWheel(midiChannel, pressureValueScaled);
                break;
            case 5: //pitch bend down
                //convert 0-127 to 8191-0
                pressureValueScaled = 8192 - (pressureValueScaled * (8191.0/127.0));
                message = MidiMessage::pitchWheel(midiChannel, pressureValueScaled);
                break;
            default: 
                message = MidiMessage::channelPressureChange(midiChannel, pressureValueScaled);
                break;
        }
        
        sendMidiMessage(message);
    }
    
}

//Output any MIDI messages
void SequencePlayer::sendMidiMessage(MidiMessage midiMessage)
{
    if(midiOutputDevice)
		midiOutputDevice->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
	else
		std::cout << "No MIDI output selected\n";
}





void SequencePlayer::setMidiOutputDevice (MidiOutput &midiOutput)
{
    midiOutputDevice = &midiOutput;
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
    
    else if (message == "STOP THREAD")
    {
        stopThread(timeInterval*2);
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
    //if previous mode was MIDI, prevent any hanging midi notes
    if (mode == 1 && mode != value && isThreadRunning() == true)
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

    if (midiNote[row] != value && isThreadRunning() == true)
    {
        triggerMidiNoteOffMessage(row);
    }
    
    midiNote[row] = value;
}

void SequencePlayer::setMidiChannel (int value)
{
    //if new channel is different from current channel, 
    //stop MIDI notes if seq is currently playing to prevent hanging notes
    
    if (midiChannel != value && isThreadRunning() == true)
    {
        for (int i = 0; i < NO_OF_ROWS; i++)
        {
            triggerMidiNoteOffMessage(i);
        }
    }
    
    midiChannel = value;
}

void SequencePlayer::setMidiVelocity (int value)
{
    midiVelocity = value;
}
void SequencePlayer::setMidiNoteLength (int value)
{
    midiNoteLength = value;
}
void SequencePlayer::setMidiMinRange (int value)
{
    midiMinRange = value;
}

void SequencePlayer::setMidiMaxRange (int value)
{
    midiMaxRange = value;
}

void SequencePlayer::setMidiControllerNumber (int value)
{
    //is CC number has changed and pressure is currently > 0, reset the value of the current pressure data
    if (midiControllerNumber != value && pressureValue > 0)
    {
        pressureValue = 0; 
        sendMidiPressureData();
    }
    
    midiControllerNumber = value;
}

void SequencePlayer::setMidiPressureMode (int value)
{
    //if pressure mode has changed and pressure is currently > 0, reset the value of the current pressure data
    if (midiPressureMode != value && pressureValue > 0)
    {
        pressureValue = 0; 
        sendMidiPressureData();
    }
    
    midiPressureMode = value;
}


void SequencePlayer::setMidiPressureStatus (bool value)
{
    //if pressure status has changed and pressure is currently > 0, reset the value of the current pressure data
    if (midiPressureStatus != value && pressureValue > 0)
    {
        pressureValue = 0; 
        sendMidiPressureData();
    }
    
    midiPressureStatus = value;
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

void SequencePlayer::setRecordEnabled (bool value)
{
    recordEnabled = value;
}



double SequencePlayer::getTimeInterval()
{
    return timeInterval;
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
