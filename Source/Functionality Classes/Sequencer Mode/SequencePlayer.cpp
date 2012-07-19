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
            }
        }
    }
    
    relativeTempoMode = PAD_SETTINGS->getSequencerRelativeTempoMode();
    setTempo(AppSettings::Instance()->getGlobalTempo());
    
    mode = PAD_SETTINGS->getSequencerMode();
    numberOfSequences = PAD_SETTINGS->getSequencerNumberOfSequences();
    playState = PAD_SETTINGS->getSequencerPlayState();
    sequenceLength = PAD_SETTINGS->getSequencerLength();
    for (int row = 0; row <=NO_OF_ROWS-1; row++)
       midiNote[row] = PAD_SETTINGS->getSequencerMidiNote(row); 
    triggerMode = PAD_SETTINGS->getSequencerTriggerMode();
    channel = PAD_SETTINGS->getSequencerChannel();
    
    midiChannel = PAD_SETTINGS->getSequencerMidiChannel();
    midiVelocity = PAD_SETTINGS->getSequencerMidiVelocity();
    midiNoteLength  = PAD_SETTINGS->getSequencerMidiNoteLength();
    
    gain = gainPrev = PAD_SETTINGS->getSequencerGain();
    panLeft = panLeftPrev = PanControl::leftChanPan_(PAD_SETTINGS->getSequencerPan());
    panRight = panRightPrev = PanControl::rightChanPan_(PAD_SETTINGS->getSequencerPan());
    
    playStateData.playingStatus = 0;
    playStateData.pressureValue = 0;
    playStateData.shouldLoop = true;
    playStateData.moveToNextSeq = false;
    playStateData.ignorePressure = false;
    playStateData.isLinearCycle = false;
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
    //LOOK INTO DOING THIS STUFF ONLY WHEN MODE IS SET TO SAMPLES MODE
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        sequenceAudioFilePlayer[row] = new SequenceAudioFilePlayer(padNumber, row, audioTransportSourceThread);
        audioMixer.addInputSource(sequenceAudioFilePlayer[row], false); //add as inputsource to audioMixer
    }
    
    broadcaster.addActionListener(this);
    
    prevPadValue = 0;
}

//=====================================================================================
//=Destructor==========================================================================
//=====================================================================================


SequencePlayer::~SequencePlayer()
{
    stopThread(timeInterval);
    modeSequencerRef.updatePadPlayingStatus(padNumber, 0);
    
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        delete sequenceAudioFilePlayer[row];
    }
    
    audioMixer.removeAllInputs();
    audioPlayer.setSource(NULL);
}

//=====================================================================================
//=Process OSC message stuff===========================================================
//=====================================================================================

void SequencePlayer::processSequence(int padValue)
{
    //this is needed incase audio ends on its own or is stopped via the 'exclusive mode' feature , in order to reset everything so it will trigger again properly
    if (isThreadRunning() == false && currentPlayingState == 1 && prevPadValue == 0)
    {
        std::cout << "stream ended on its own!!";
        currentPlayingState = 0;
        playStates.reset();
    }
    
    switch (playState) 
    {
        case 1:
            playStateData = playStates.playOnce(padValue);
            break;
        case 2:
            playStateData = playStates.playOnceContinuous(padValue);
            break;
        case 3:
            playStateData = playStates.loop(padValue);
            break;
        case 4:
            playStateData = playStates.togglePressOff(padValue);
            break;
        case 5:
            playStateData = playStates.toggleReleaseOff(padValue);
            break;
        case 6:
            playStateData = playStates.sticky(padValue);
            break;
        case 7:
            playStateData = playStates.latchMaxLatch(padValue);
            break;
        case 8:
            playStateData = playStates.latchPressLatch(padValue);
            break;
        case 9: 
            playStateData = playStates.triggerLooped(padValue);
            break;
        case 10:
            playStateData = playStates.triggerNonLooped(padValue);
            break;
        case 11:
            playStateData = playStates.cycleLooped(padValue);
            break;
        case 12:
            playStateData = playStates.cycleLinear(padValue);
            break;
        case 13:
            playStateData = playStates.autoCycleLooped(padValue);
            break;
        case 14:
            playStateData = playStates.autoCycleLinear(padValue);
            break;
        case 21:
            playStateData = playStates.playOnceNonDestructive(padValue);
            break;
        case 22:
            playStateData = playStates.triggerLoopedNonDestructive(padValue);
            break;
        case 23:
            playStateData = playStates.cycleLinearNonDestructive(padValue);
            break;
        case 24:
            playStateData = playStates.triggerNonLoopedNonDestructive(padValue);
            break;
        case 25:
            playStateData = playStates.toggleNonDestructive(padValue);
            break;
        default:
            playStateData = playStates.loop(padValue);
            break;
    }
    
    //CONTROL PRESSURE STUFF WITH playStateData.pressureValue NOT padValue AS STICKY MODE WOULD MODIFY THE VALUE
    
    //===========convert pressure to steps here!===============
    if (playStateData.ignorePressure == false) //ignore for certain playstates
    {
        sequenceNumber = playStateData.pressureValue * (numberOfSequences/511.0);
        if (sequenceNumber > numberOfSequences-1)
            sequenceNumber  = numberOfSequences-1;
    }
    
    //for cycle playstates
    if (playStateData.moveToNextSeq == true)
    {
        //if true, move to next seq
        sequenceNumber++;
        
        if (sequenceNumber == numberOfSequences) //if goes beyond the last sequence
        {
            if (playStateData.isLinearCycle == false)
            {
                sequenceNumber = 0; //if reached end of seqs and play state isn't a linear cycle, go back to the beginning
            }
            else if (playStateData.isLinearCycle == true)
            {
                //stopThreadAndReset(); //if reached end of seqs and play state IS a linear cycle, stop thread
                stopThread(timeInterval);
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
            
                prevSequenceNumber = sequenceNumber;
                broadcaster.sendActionMessage("SEQ DISPLAY");
            }
        }
    
    }
    
    //==========================================================================================
    //====================set the transportation of the thread here!============================
    
    //if playStateData.playingStatus = 2, do nothing
    
    if (triggerMode == 1) //free
    {
        if (playStateData.playingStatus == 1) //play
        {
            if (isThreadRunning() == true)
                stopThread(timeInterval); //so that the 'trigger' play states work ()
                //should I be stopping the thread or should i just do: columnNumber = 0; ????
            
            startThread();
            currentPlayingState = 1;
            
            //EXCLUSIVE MODE STUFF 
            if (channel != 1) //if channel/group is 2 or above
            {
                broadcaster.sendActionMessage("EXCLUSIVE STOP");
            }
            
        }
        
        else if (playStateData.playingStatus == 0) //stop
        {
            currentPlayingState = 0;
            stopThread(timeInterval);
    
        }

    }
    
    //==========================================================================================
    else if (triggerMode == 2) //quantised
    {
        if (playStateData.playingStatus == 1) //play
        {
            currentPlayingState = 2; //waiting to play
            
            //add this instance of SequencePlayer to the waitingPadSequencer Array within
            //ModeSequencer so that it is alerted of the next quantised point in time so
            //the sequence can start playing
            modeSequencerRef.addItemToWaitingPadSequencer(this);
            broadcaster.sendActionMessage("WAITING TO PLAY");
            
            
        }
        else if (playStateData.playingStatus == 0) //stop
        {
            currentPlayingState = 3; // waiting to stop
            
            //add this instance of SequencePlayer to the waitingPadSequencer Array within
            //ModeSequencer so that it is alerted of the next quantised point in time so
            //the sequence can stop playing
            modeSequencerRef.addItemToWaitingPadSequencer(this);
            broadcaster.sendActionMessage("WAITING TO STOP");
        }
    }
    //==========================================================================================
        
    
    prevPadValue = padValue;
}

void SequencePlayer::triggerQuantisationPoint()
{
    if (currentPlayingState == 2) //waiting to play
    {
        if (isThreadRunning() == true)
            stopThread(timeInterval); //so that the 'trigger' play states work ()
        
        startThread();
        currentPlayingState = 1;
        
        //EXCLUSIVE MODE STUFF 
        if (channel != 1) //if channel is 2 or above
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
    sequenceNumber = 0;
    
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
        // - columnNumber becomes greater than sequenceLength when playStateData.shouldLoop is set to false from the current play state. see bottom of this loop.
        // - sequenceNumber becomes greater than numberOfSequences when playStateData.isLinearCycle is set to true from the autocyclelinear playstate. see bottom of this while loop
        
        
        //PREVIOUSLY, waitForMillisecondCounter() WAS CALLED HERE, AND currentTime ABOVE WAS SET TO JUST Time::getMillisecondCounter().
        //HOWEVER NOW IT IS CALLED AT THE END OF THE WHILE LOOP AND THE INTIAL VALUE OF currentTime HAS BEEN CHANGED.
        //THIS HAS BEEN DONE AS BEFORE THERE WAS A BUG IN THAT WHEN THE THREAD WAS TOLD TO EXIT IT WOULD CUT OFF THE LAST NOTE BEING PLAYED
        //HOWEVER NOW THIS BUG DOES NOT OCCUR AS THE THREAD PAUSES AFTER THE SOUND HAS BEEN TRIGGER IN THE SAME INTERATION OF THE LOOP, NOT BEFORE.
        //IT ALSO SEEMS TO MAKE THE TRIGGER PLAYSTATE BEHAVE BETTER IN TERMS OF TIMING
        
        
        if (mode == 1) //midi mode selected
        {
            //cycle through each row to look for any note messages
            for (int rowNumber = 0; rowNumber <= NO_OF_ROWS-1; rowNumber++)
            {
                //look for note-off messages before note-on messages,
                //as otherwise if you set the note length to be max length,
                //the note will be turned off straight away duw to how note length
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
                if (sequenceData[sequenceNumber][rowNumber][columnNumber] >= 1) //if 'on'
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
                if (sequenceData[sequenceNumber][rowNumber][columnNumber] >= 1) //if 'on'
                {
                    int velocity = sequenceData[sequenceNumber][rowNumber][columnNumber];
                    triggerAudioMessage(rowNumber, velocity);
                }
            }
        }
        
        //=========================================================================
        
        
        
        columnNumber++;
        //if the counter variable reaches the end and the current play state iss set to loop, restart the counter.
        //otherwise the while loop will exit and the read will stop on it's own
        if (columnNumber == sequenceLength && playStateData.shouldLoop == true)
            columnNumber = 0;
        
        
        
        //for autocycle playstates
        if (playStateData.isAutoCycle == true)
        {
            if (columnNumber == sequenceLength) //if goes beyond the last column of the sequence
            {
                sequenceNumber++;
                columnNumber = 0; //reset columnNumber here otherwise thread loop will exit and seq will stop
                
                if (sequenceNumber == numberOfSequences) //if goes beyond the last sequence
                {
                    if (playStateData.isLinearCycle == false)
                        sequenceNumber = 0; //if reached end of seqs and play state isn't a linear cycle, go back to the beginning
                    
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
    
    //tell gui pad that the sequence has finished playing
    broadcaster.sendActionMessage("PLAYING OFF");
    
}


//DO I NEED THIS FUNCTION ANYMORE??
void SequencePlayer::stopThreadAndReset()
{
    stopThread(timeInterval);
    
    //this call can't be made in bottom of run() (when the thread will be finshed)
    //as it would interupt the trigger play states
    //playStates.reset();
    currentPlayingState = 0;
    std::cout << "stopping sequence!" << std::endl;
}



//=====================================================================================
//=Create a note stuff=================================================================
//=====================================================================================


//called every time a midi message needs triggering
void SequencePlayer::triggerMidiMessage(int rowNumber, int velocity)
{
    MidiMessage message = MidiMessage::noteOn(midiChannel, midiNote[rowNumber], (uint8)velocity);
    sendMidiMessage(message);
}

void SequencePlayer::triggerMidiNoteOffMessage (int rowNumber)
{
    MidiMessage message = MidiMessage::noteOff(midiChannel, midiNote[rowNumber]);
    sendMidiMessage(message);
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
    
    /*
    //OLD GAIN AND PAN ALGORITHM
    //get sample data from audio buffer
    float *pOutL = bufferToFill.buffer->getSampleData (0, bufferToFill.startSample);
    float *pOutR = bufferToFill.buffer->getSampleData (1, bufferToFill.startSample);
    sharedMemory.enter();
    //should 'sharedMemory' be entered on the other side where stuff like gain is actually set? YES
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        //gain
        *pOutL = *pOutL * samplesGain;
        *pOutR = *pOutR * samplesGain
        //pan
        *pOutL = panControl.leftChanPan(*pOutL);
        *pOutR = panControl.rightChanPan(*pOutR);
        
        pOutL++;
        pOutR++;
    }
    sharedMemory.exit();
     */
    
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
        //stop currently playing sequence of this channel,
        //and add this instance of SequencePlayer to the correct index (channel number-1) 
        //of currentExclusivePadSequencer array within ModeSequencer
        //so that this sequence can be stopped via exclusive mode next time
        
        //this is called async so that there is no noticable time delay caused when
        //doing this via quantisation mode (which is caused by the time value
        //paramater within the stopThread() function.
        modeSequencerRef.stopExclusivePadSequencer(channel, this);
    }
    
    else if (message == "STOP THREAD")
    {
        stopThread(timeInterval);
        currentPlayingState = 0;
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
            if (columnNumberEnd == sequenceLength && playStateData.shouldLoop == true)
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
    case 1: //quarter time
        tempo = value/4.0;
        break;
        
    case 2: //half time
        tempo = value/2.0;
        break;
    
        case 3: //regular time
        tempo = value;
        break;
        
    case 4: //double time
        tempo = value*2.0;
        break;
        
    case 5: //quadrupal time
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
    mode = value;
}

void SequencePlayer::setNumberOfSequences (int value)
{
    numberOfSequences = value;
}
void SequencePlayer::setPlayState (int value)
{
    playState = value;
}
void SequencePlayer::setSequenceLength (int value)
{
    sequenceLength = value;
}

void SequencePlayer::setTriggerMode (int value)
{
    triggerMode = value;
}
void SequencePlayer::setChannel (int value)
{
    channel = value;
}

void SequencePlayer::setMidiNote (int row, int value)
{
    midiNote[row] = value;
}
void SequencePlayer::setMidiChannel (int value)
{
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



double SequencePlayer::getTimeInterval()
{
    return timeInterval;
}
