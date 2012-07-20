
//
//  PadSettings.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 15/09/2011.
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

#include "PadSettings.h"
#include "../Functionality Classes/AlphaLiveEngine.h"
#include "../Functionality Classes/Other/LayoutsAndScales.cpp"
#include "AppSettings.h"
#include "StoredSettings.h"

//==================================================================
//=Constructor======================================================
//==================================================================

PadSettings::PadSettings(int arrayIndex)
                            :   padNumber(arrayIndex)
                          
{
    alphaLiveEngineRef = nullptr;
    
    //------------default values--------------
    mode = 0; //or should default mode be midi (1) ?
    pressureSensitivityMode = 2; //standard
    prevMode = 0;
    currentState = 0;
    currentPlayingState = 0;
    currentValue = 0;
    
    //midi mode
    midiNote = 60;
    midiVelocity = 110;
    midiChannel = 1;
    midiMinPressureRange = 0;
    midiMaxPressureRange = 127;
    midiPressureMode = 1;
    midiTriggerMode = 2;
    midiPressureStatus = true;
    midiNoteStatus = true;
    //default CC layout
    midiCcController = Layouts::ccLayout[padNumber];
    midiExclusiveGroup = 1;
    midiQuantizeMode = 1;
    
    //looper mode
    //looperAudioFilePath = File::nonexistent;
    looperAudioFilePath = String::empty;
    looperTriggerMode = 2; 
    looperShouldLoop = 1;
    looperIndestructible = 0;
    looperShouldFinishLoop = 0;
    looperSticky = 0;
    looperEffect = 0;
    looperPan = 0.5;
    looperGain = 1.0;
    looperChannel = 1;
    looperQuantizeMode = 1;
    
    //----looper effects----
    //Gain and Pan
    looperFxGainPanGain = 0.5;
    looperFxGainPanPan = 0.5;
    looperFxGainPanAlphaTouch = 2;
    looperFxGainPanAtReverse = 0;
    looperFxGainPanAtIntensity = 1.0;
    //LPF
    looperFxLpfMix = 0.0;
    looperFxLpfFreq = 1000;
    looperFxLpfBandwidth = 5;
    looperFxLpfAlphaTouch = 2;
    looperFxLpfAtReverse = 0;
    looperFxLpfAtIntensity = 1.0;
    //HPF
    looperFxHpfMix = 0.0;
    looperFxHpfFreq = 1000;
    looperFxHpfBandwidth = 5;
    looperFxHpfAlphaTouch = 2;
    looperFxHpfAtReverse = 0;
    looperFxHpfAtIntensity = 1.0;
    //BPF
    looperFxBpfMix = 0.0;
    looperFxBpfFreq = 1000;
    looperFxBpfBandwidth = 5;
    looperFxBpfAlphaTouch = 2;
    looperFxBpfAtReverse = 0;
    looperFxBpfAtIntensity = 1.0;
    //Delay
    looperFxDelayMix = 0.0;
    looperFxDelayTime = 500.0;
    looperFxDelayFeedback = 0.5;
    looperFxDelayLpfFreq = 20000.0;
    looperFxDelayHpfFreq = 30.0;
    looperFxDelaySync = 1;
    looperFxDelayTimeMenu = 3;
    looperFxDelayAlphaTouch = 2;
    looperFxDelayAtReverse = 0;
    looperFxDelayAtIntensity = 1.0;
    //Reverb
    looperFxReverbMix = 0.0;
    looperFxReverbRoomSize = 0.5;
    looperFxReverbDamping = 0.5;
    looperFxReverbWidth = 0.5;
    looperFxReverbFreezeMode = 0.2;
    looperFxReverbAlphaTouch = 2;
    looperFxReverbAtReverse = 0;
    looperFxReverbAtIntensity = 1.0;
    //Flanger
    looperFxFlangerMix = 0.0;
    looperFxFlangerRate = 0.25;
    looperFxFlangerFeedback = 0.9;
    looperFxFlangerIntensity = 0.1;
    looperFxFlangerSync = 1;
    looperFxFlangerRateMenu = 4;
    looperFxFlangerAlphaTouch = 2;
    looperFxFlangerAtReverse = 0;
    looperFxFlangerAtIntensity = 1.0;
    //Tremolo
    looperFxTremoloDepth = 0.0;
    looperFxTremoloRate = 5.0;
    looperFxTremoloShape = 1;
    looperFxTremoloSync = 1;
    looperFxTremoloRateMenu = 3;
    looperFxTremoloAlphaTouch = 2;
    looperFxTremoloAtReverse = 0;
    looperFxTremoloAtIntensity = 1.0;
    
    //sequencer mode
    //init all indexes of the sequencerData 3D array to 0.
    for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
    {
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
            {
                sequencerData[seq][row][column] = 0;
            }
        }
    }
    
    for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
    {
        sequencerDataString[seq] = String::empty;
    }
    
    
    sequencerMode = 1; 
    sequencerNumberOfSequences = 8; 
    sequencerTriggerMode = 2; 
    sequencerChannel = 1;
    sequencerLength = 32;
    sequencerQuantizeMode = 1;
    sequencerRelativeTempoMode = 3; //regular time
    
    //set a default sequencer note midi layout of 60-72
    for (int i = 0, num = 60; i <= NO_OF_ROWS-1; i++)
    {
        sequencerMidiNote[i] = num;
        num ++;
    }
    
    sequencerMidiVelocity = 110;
    sequencerMidiChannel = 1;
    sequencerMidiNoteLength = 4;
    
    for (int i = 0; i <= NO_OF_ROWS-1; i++)
    {
        sequencerSamplesAudioFilePath[i] = String::empty;
    }
     
    sequencerPan = 0.5;
    sequencerGain = 1.0;
    
    //controller mode
    controllerControl = 1;
    controllerPresetNumber = 1;
    controllerOscIpAddress = "127.0.0.1";
    controllerOscPort = 5004;
    controllerMidiProgramChangeNumber = 1;
    controllerMidiProgramChangeChannel = 1;
    
}

//==================================================================
//=Destructor=======================================================
//==================================================================

PadSettings::~PadSettings()
{
    //delete alphaLiveEngineRef;
    
}

//==================================================================


void PadSettings::setAlphaLiveEngineRef (AlphaLiveEngine *ref)
{
    alphaLiveEngineRef = ref;
}

void PadSettings::setPadNumber (int value)
{
    padNumber = value;
}


void PadSettings::resetData (int whatToReset)
{
    //this is going to be called when 'reset all' is clicked on the preset component
    //and when the 'delete preset' option is chosen for a single preset
    
    /*
     whatToReset is a flag variable that lets this the program know exactly what data needs to be reset.
     
     If this is called from the preset component 'reset all' button or the 'delete preset' drop down item,
     Everything must be reset including the mode.
     If this is called from the 'clean up project' button or from when a new project is loaded,
     only the data from the modes which this pad isn't set to needs to be reset back to the default values.
     
     whatToReset values:
     0 - reset everything
     1 - everything but MIDI mode
     2 - everything but Looper mode
     3 - everything but sequencer mode
     4 - everything but controller mode
     
     */
    
    if (whatToReset == 0)
    {
        setMode(0);
        setPressureSensitivityMode(2);
    }
    
    if (whatToReset != 1)
    {
        setMidiNote(60);
        setMidiVelocity(110);
        setMidiChannel(1);
        setMidiMinPressureRange(0);
        setMidiMaxPressureRange(127);
        setMidiPressureMode(1);
        setMidiTriggerMode(2);
        setMidiPressureStatus (true);
        setMidiNoteStatus (true);
        setMidiCcController (Layouts::ccLayout[padNumber]);
        setMidiExclusiveGroup(1);
        setMidiQuantizeMode(1);
    }
    
    if (whatToReset != 2)
    {
        setLooperAudioFilePath (File::nonexistent);
        setLooperTriggerMode (2);
        setLooperShouldLoop(1);
        setLooperIndestructible(0);
        setLooperShouldFinishLoop(0);
        setLooperSticky(0);
        setLooperEffect (0);
        setLooperPan (0.5);
        setLooperGain (1.0);
        setLooperChannel (1);
        setLooperQuantizeMode(1);
        
        //what about reseting the FX values?
    }
    
    if (whatToReset != 3)
    {
        setSequencerMode (1);
        for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
        {
            for (int row = 0; row <= NO_OF_ROWS-1; row++)
            {
                for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
                {
                    setSequencerData (seq, row, column, 0, false); //false as we want to clear seqDataString seperatly to reduce time
                }
            }
            clearSequencerDataString(seq);
        }
        setSequencerNumberOfSequences (8);
        setSequencerTriggerMode (2);
        setSequencerChannel (1);
        setSequencerLength (32);
        setSequencerQuantizeMode (1);
        setSequencerRelativeTempoMode(3);
        for (int i = 0, num = 60; i <= NO_OF_ROWS-1; i++)
        {
            setSequencerMidiNote (num, i);
            num ++;
        }
        setSequencerMidiVelocity (110);
        setSequencerMidiChannel (1);
        setSequencerMidiNoteLength (4);
        for (int i = 0; i <= NO_OF_ROWS-1; i++)
        {
            setSequencerSamplesAudioFilePath (File::nonexistent, i);
        }
        setSequencerPan (0.5);
        setSequencerGain (1.0);
    }
    
    if (whatToReset != 4)
    {
        setControllerControl(1);
        setControllerPresetNumber(1);
        setControllerOscIpAddress("127.0.0.1");
        setControllerOscPort(5004);
        setControllerMidiProgramChangeNumber(1);
        setControllerMidiProgramChangeChannel(1);
    }
}


#pragma mark Global mutator functions
//==================================================================
//=Global Methods===================================================
//==================================================================

void PadSettings::setMode(int value)
{
    //at this point, 'mode'  = previously selected mode, 'value' = new mode
    
    //THIS FUNCTION NEEDS TO BE TIDIED UP - NOT VERY MODULAR IN TERMS OF ADDING NEW MODES, OR IS IT?? HMMM...
    
    if (value != mode) //if mode is different to previously
    {
        if (value == 1)
        {
            //This needs to be called as when copying&pasting pad data the new values need to be set within
            //modeMidi
            alphaLiveEngineRef->getModeMidi()->setPadData(padNumber);
        }
        
        //do i need the != part in any of the statements below? (as I'm initally checking above)
        
        
        if (value == 2 && mode != 2) //to prevent Looper AudioFilePlayer objects being re-created
        {
            //create an instance of AudioFilePlayer for pad 'padNumber'
            alphaLiveEngineRef->getModeLooper()->createAudioFilePlayer(padNumber);
        }
        else if (mode == 2 && value != 2) //to prevent non-existent Looper AudioFilePlayer objects from being deleted
        {
            //delete instance of AudioFilePlayer for pad 'padNumber'
            alphaLiveEngineRef->getModeLooper()->deleteAudioFilePlayer(padNumber);
        }
        
        
        if (value == 3 && mode != 3) //to prevent Sequencer SequencePlayer objects being re-created
        {
            //create an instance of SequencePlayer for pad 'padNumber'
            alphaLiveEngineRef->getModeSequencer()->createSequencePlayer(padNumber);
        }
        else if (mode == 3 && value != 3) //to prevent non-existent Sequencer SequencePlayer objects from being deleted
        {
            //delete instance of SequencePlayer for pad 'padNumber'
            alphaLiveEngineRef->getModeSequencer()->deleteSequencePlayer(padNumber);
        }
        
        
        mode = value; //mode is set!
        
    }
}


void PadSettings::setCurrentState(int value)
{
    currentState = value;
}

void PadSettings::setCurrentPlayingState (int value)
{
    currentPlayingState = value;
}

void PadSettings::setCurrentValue (int value)
{
    currentValue = value;
}

void PadSettings::setPressureSensitivityMode (int value)
{
    pressureSensitivityMode = value;
}


#pragma mark Global accessor functions

int PadSettings::getMode()
{
    return mode;
}

int PadSettings::getCurrentState()
{
    return currentState;
}

int PadSettings::getCurrentValue()
{
    return currentValue;
}

int PadSettings::getCurrentPlayingState()
{
    return currentPlayingState;
}
int PadSettings::getPressureSensitivityMode()
{
    return pressureSensitivityMode;
}

#pragma mark MIDI mode mutator functions
//==================================================================
//=MIDI Mode Methods================================================
//==================================================================

void PadSettings::setMidiNote(int value)
{
    midiNote = value;
    
    //when applying scales or layouts, midi note numbers could go out the range of
    //0-119 (the range of the circular piano). Therefore the values need to be set
    //so they can't go out of this range otherwise the app will crash when it trys
    //to display the note on the circular piano.
    if (midiNote < 0)
        midiNote = 0;
    if (midiNote > 119)
        midiNote = 119;
    
    alphaLiveEngineRef->getModeMidi()->setNote(midiNote, padNumber);
}
void PadSettings::setMidiVelocity(int value)
{
    midiVelocity = value;
    alphaLiveEngineRef->getModeMidi()->setVelocity(value, padNumber);
}
void PadSettings::setMidiChannel(int value)
{
    midiChannel = value;
    alphaLiveEngineRef->getModeMidi()->setChannel(value, padNumber);
}
void PadSettings::setMidiMinPressureRange(int value)
{
    midiMinPressureRange = value;
    alphaLiveEngineRef->getModeMidi()->setMinRange(value, padNumber);
}
void PadSettings::setMidiMaxPressureRange(int value)
{
    midiMaxPressureRange = value;
    alphaLiveEngineRef->getModeMidi()->setMaxRange(value, padNumber);
}
void PadSettings::setMidiPressureMode(int value)
{
    midiPressureMode = value;
    alphaLiveEngineRef->getModeMidi()->setPressureMode(value, padNumber);
}
void PadSettings::setMidiTriggerMode(int value)
{
    midiTriggerMode = value;
    alphaLiveEngineRef->getModeMidi()->setTriggerModeValue(value, padNumber);
}
void PadSettings::setMidiPressureStatus (bool value)
{
    midiPressureStatus = value;
    alphaLiveEngineRef->getModeMidi()->setPressureStatus(value, padNumber);
}
void PadSettings::setMidiNoteStatus (bool value)
{
    midiNoteStatus = value;
    alphaLiveEngineRef->getModeMidi()->setNoteStatus(value, padNumber);
}
void PadSettings::setMidiCcController (int value)
{
    midiCcController = value;
    alphaLiveEngineRef->getModeMidi()->setControllerNumber(value, padNumber);
}
void PadSettings::setMidiExclusiveGroup (int value)
{
    midiExclusiveGroup = value;
    alphaLiveEngineRef->getModeMidi()->setExclusiveGroup(value, padNumber);
}
void PadSettings::setMidiQuantizeMode (int value)
{
    midiQuantizeMode = value;
    alphaLiveEngineRef->getModeMidi()->setQuantizeMode(value, padNumber);
}

#pragma mark MIDI mode accessor functions

int PadSettings::getMidiNote()
{
    return midiNote;
}
int PadSettings::getMidiVelocity()
{
    return midiVelocity;
}
int PadSettings::getMidiChannel()
{
    return midiChannel;
}
int PadSettings::getMidiMinPressureRange()
{
    return midiMinPressureRange;
}
int PadSettings::getMidiMaxPressureRange()
{
    return midiMaxPressureRange;
}
int PadSettings::getMidiPressureMode()
{
    return midiPressureMode;
}
int PadSettings::getMidiTriggerMode()
{
    return midiTriggerMode;
}
bool PadSettings::getMidiPressureStatus()
{
    return midiPressureStatus;
}
bool PadSettings::getMidiNoteStatus()
{
    return midiNoteStatus;
}
int PadSettings::getMidiCcController()
{
    return midiCcController;
}
int PadSettings::getMidiExclusiveGroup()
{
    return midiExclusiveGroup;
}
int PadSettings::getMidiQuantizeMode()
{
    return midiQuantizeMode;
}



#pragma mark Looper mode mutator functions
//==================================================================
//=Looper Mode Methods==============================================
//==================================================================

void PadSettings::setLooperAudioFilePath(File value)
{
    /*
     If the default option is set, this function creates a copy of the imported audio file in the working directory, 
     and sets the new file path as the loopers audio file location.
     The algorithm used below ensures that there will be no obvious duplicates of audio files,
     but it also ensures that imported files with an existing name might not necessarily be a duplicate!
     
     */
    
    if (value != File::nonexistent)
    {
        if (AppSettings::Instance()->getCopyExternalFiles() == true)
        {
            //create a new file for the imported audio file in the working directory but only if it doesn't already exist!!
            File audioFileCopy (File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + value.getFileName());
            
            if (audioFileCopy.existsAsFile() == false) //if it doesn't yet exist
            {
                //copy the added audio file into the newly created file
                value.copyFileTo(audioFileCopy);
            }
            
            else if (audioFileCopy.existsAsFile() == true) //if it already exists 
            {
                Array<File> matchingFilesArray;
                String fileWildCard (audioFileCopy.getFileNameWithoutExtension()+"*");
                bool importedFileNeedsCopying = true;
                
                //Find all possible duplicates of the imported file using fileWildCard
                //and add reference of all possible files to matchingFilesArray.
                File::getCurrentWorkingDirectory().findChildFiles(matchingFilesArray, 2, false, fileWildCard);
                
                for (int i = 0; i < matchingFilesArray.size(); i++)
                {
                    if (value.hasIdenticalContentTo(matchingFilesArray[i]) == true)
                    {
                        //if it finds a duplicate, flag that the file doesn't need copying
                        importedFileNeedsCopying = false;
                        //set the file
                        audioFileCopy = matchingFilesArray[i];
                        //break out of the for loop once a duplicate has been found to prevent unnecessary checks
                        break;
                    }
                }
                
                if (importedFileNeedsCopying == true) 
                {
                    //if no duplicate was found...
                    //... copy the added file with an appended name
                    audioFileCopy = audioFileCopy.getNonexistentSibling();
                    value.copyFileTo(audioFileCopy); 
                }
                
                value = audioFileCopy;
            }
        }
        
        //OLD - store just the FILE NAME of the audio file.
        //looperAudioFilePath = value.getFileName();
        //NEW - store the full path of the audio file, incase the file isn't copied into the working directory
        looperAudioFilePath = value.getFullPathName();
    }
    else
        looperAudioFilePath = String::empty; 
    
    //set the audio file within the correct AudioFilePlayer Instance if it exists
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setAudioFile(value);
    }
}


void PadSettings::setLooperTriggerMode(int value)
{
    looperTriggerMode = value;
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setTriggerMode(looperTriggerMode);
    }
}

void PadSettings::setLooperShouldLoop (int value)
{
    looperShouldLoop = value;
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setShouldLoop(looperShouldLoop);
    }
    
}
void PadSettings::setLooperIndestructible (int value)
{
    looperIndestructible = value;
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setIndestructible(looperIndestructible);
    }
}
void PadSettings::setLooperShouldFinishLoop (int value)
{
    looperShouldFinishLoop = value;
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setShouldFinishLoop(looperShouldFinishLoop);
    }
}

void PadSettings::setLooperSticky (int value)
{
    looperSticky = value;
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setSticky(looperSticky);
    }
}

void PadSettings::setLooperEffect(int value)
{
    //std::cout << "Pad " << padNumber << " set with effect " << value << std::endl;
    looperEffect = value;
    
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setEffect(looperEffect);
    }
    
}
void PadSettings::setLooperPan(float value)
{
    looperPan = value;
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setPan(looperPan);
    }
}
void PadSettings::setLooperGain(float value)
{
    looperGain = value;
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setGain(looperGain);
    }
}
void PadSettings::setLooperChannel(int value)
{
    looperChannel = value;
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setChannel(looperChannel);
}
void PadSettings::setLooperQuantizeMode(int value)
{
    looperQuantizeMode = value;
    if (alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->setQuantizeMode(looperQuantizeMode);
    }
}

#pragma mark Looper mode accessor functions

File PadSettings::getLooperAudioFilePath()
{
    if (looperAudioFilePath != String::empty)
    {
        File audioFile = looperAudioFilePath;
        
        if (audioFile.existsAsFile() == false) //if for some reason it doesn't exist
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File not found!", "\"" + looperAudioFilePath + "\"" + " for Pad " + String(padNumber+1) + " could not be found.");
            //IN NEXT VERSION USE A YES/NO ALERTWINDOW THAT ASKS IF YOU WOULD LIKE TO MANUALLY LOCATE THE AUDIO FILE.
            //THIS WILL BRING UP A FILEBROWSERWINDOW AND THE CHOSEN FILE WILL THEN BE MOVED TO THE CURRENT WORKING DIRECTORY AND
            //SAVED INTO THE looperAudioFilePath STRING IN THE CORRECT FORMAT.
            
            //set the relevent variables to nonexistent/empty
            audioFile = File::nonexistent;
            looperAudioFilePath = String::empty;
        }
        
        return audioFile;
    }
    
    else
        return File::nonexistent;
    
}


int PadSettings::getLooperTriggerMode()
{
    return looperTriggerMode;
}

int PadSettings::getLooperShouldLoop()
{
    return looperShouldLoop;
}
int PadSettings::getLooperIndestructible()
{
    return looperIndestructible;
}
int PadSettings::getLooperShouldFinishLoop()
{
    return looperShouldFinishLoop;
}
int PadSettings::getLooperSticky()
{
    return looperSticky;
}

int PadSettings::getLooperEffect()
{
    return looperEffect;
}
float PadSettings::getLooperPan()
{
    return looperPan;
}
float PadSettings::getLooperGain()
{
    return looperGain;
}
int PadSettings::getLooperChannel()
{
    return looperChannel;
}
int PadSettings::getLooperQuantizeMode()
{
    return looperQuantizeMode;
}


#pragma mark Looper FX functions
//---LOOPER EFFECTS---
#pragma mark Gain/Pan FX
//Gain&Pan
void PadSettings::setLooperFxGainPanGain (float value)
{
    looperFxGainPanGain = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setGain(value);
}
void PadSettings::setLooperFxGainPanPan (float value)
{
    looperFxGainPanPan = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setPan(value);
}
void PadSettings::setLooperFxGainPanAlphaTouch (int value)
{
    looperFxGainPanAlphaTouch = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchParam(value);
}
void PadSettings::setLooperFxGainPanAtReverse (int value)
{
    looperFxGainPanAtReverse = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchReverse(value);
}
void PadSettings::setLooperFxGainPanAtIntensity (float value)
{
    looperFxGainPanAtIntensity = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchIntensity(value);
}
float PadSettings::getLooperFxGainPanGain()
{
    return looperFxGainPanGain;
}
float PadSettings::getLooperFxGainPanPan()
{
    return looperFxGainPanPan;
}
int PadSettings::getLooperFxGainPanAlphaTouch()
{
    return looperFxGainPanAlphaTouch;
}
int PadSettings::getLooperFxGainPanAtReverse()
{
    return looperFxGainPanAtReverse;
}
float PadSettings::getLooperFxGainPanAtIntensity()
{
    return looperFxGainPanAtIntensity;
}

#pragma mark LPF FX
//LPF
void PadSettings::setLooperFxLpfMix (float value)
{
    looperFxLpfMix = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setMix(looperFxLpfMix);
}
void PadSettings::setLooperFxLpfFreq (float value)
{
    looperFxLpfFreq = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setFrequency(looperFxLpfFreq);
}
void PadSettings::setLooperFxLpfBandwidth (float value)
{
    looperFxLpfBandwidth = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setBandwidth(looperFxLpfBandwidth);
}
void PadSettings::setLooperFxLpfAlphaTouch (int value)
{
    looperFxLpfAlphaTouch = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchParam(looperFxLpfAlphaTouch);
}
void PadSettings::setLooperFxLpfAtReverse (int value)
{
    looperFxLpfAtReverse = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchReverse(looperFxLpfAtReverse);
    
}
void PadSettings::setLooperFxLpfAtIntensity (float value)
{
    looperFxLpfAtIntensity = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchIntensity(looperFxLpfAtIntensity);
}

float PadSettings::getLooperFxLpfMix()
{
    return looperFxLpfMix;
}
float PadSettings::getLooperFxLpfFreq()
{
    return looperFxLpfFreq;
}
float PadSettings::getLooperFxLpfBandwidth()
{
    return looperFxLpfBandwidth;
}
int PadSettings::getLooperFxLpfAlphaTouch()
{
    return looperFxLpfAlphaTouch;
}
int PadSettings::getLooperFxLpfAtReverse()
{
    return looperFxLpfAtReverse;
}
float PadSettings::getLooperFxLpfAtIntensity()
{
    return looperFxLpfAtIntensity;
}

#pragma mark HPF FX
//HPF
void PadSettings::setLooperFxHpfMix (float value)
{
    looperFxHpfMix = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setMix(looperFxHpfMix);
}
void PadSettings::setLooperFxHpfFreq (float value)
{
    looperFxHpfFreq = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setFrequency(looperFxHpfFreq);
}
void PadSettings::setLooperFxHpfBandwidth (float value)
{
    looperFxHpfBandwidth = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setBandwidth(looperFxHpfBandwidth);
}
void PadSettings::setLooperFxHpfAlphaTouch (int value)
{
    looperFxHpfAlphaTouch = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchParam(looperFxHpfAlphaTouch);
}
void PadSettings::setLooperFxHpfAtReverse (int value)
{
    looperFxHpfAtReverse = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchReverse(looperFxHpfAtReverse);
    
}
void PadSettings::setLooperFxHpfAtIntensity (float value)
{
    looperFxHpfAtIntensity = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchIntensity(looperFxHpfAtIntensity);
}

float PadSettings::getLooperFxHpfMix()
{
    return looperFxHpfMix;
}
float PadSettings::getLooperFxHpfFreq()
{
    return looperFxHpfFreq;
}
float PadSettings::getLooperFxHpfBandwidth()
{
    return looperFxHpfBandwidth;
}
int PadSettings::getLooperFxHpfAlphaTouch()
{
    return looperFxHpfAlphaTouch;
}
int PadSettings::getLooperFxHpfAtReverse()
{
    return looperFxHpfAtReverse;
}
float PadSettings::getLooperFxHpfAtIntensity()
{
    return looperFxHpfAtIntensity;
}

#pragma mark BPF FX
//BPF
void PadSettings::setLooperFxBpfMix (float value)
{
    looperFxBpfMix = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setMix(looperFxBpfMix);
}
void PadSettings::setLooperFxBpfFreq (float value)
{
    looperFxBpfFreq = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setFrequency(looperFxBpfFreq);
}
void PadSettings::setLooperFxBpfBandwidth (float value)
{
    looperFxBpfBandwidth = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setBandwidth(looperFxBpfBandwidth);
}
void PadSettings::setLooperFxBpfAlphaTouch (int value)
{
    looperFxBpfAlphaTouch = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchParam(looperFxBpfAlphaTouch);
}
void PadSettings::setLooperFxBpfAtReverse (int value)
{
    looperFxBpfAtReverse = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchReverse(looperFxBpfAtReverse);
    
}
void PadSettings::setLooperFxBpfAtIntensity (float value)
{
    looperFxBpfAtIntensity = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchIntensity(looperFxBpfAtIntensity);
}

float PadSettings::getLooperFxBpfMix()
{
    return looperFxBpfMix;
}
float PadSettings::getLooperFxBpfFreq()
{
    return looperFxBpfFreq;
}
float PadSettings::getLooperFxBpfBandwidth()
{
    return looperFxBpfBandwidth;
}
int PadSettings::getLooperFxBpfAlphaTouch()
{
    return looperFxBpfAlphaTouch;
}
int PadSettings::getLooperFxBpfAtReverse()
{
    return looperFxBpfAtReverse;
}
float PadSettings::getLooperFxBpfAtIntensity()
{
    return looperFxBpfAtIntensity;
}

#pragma mark Delay FX
//Delay
void PadSettings::setLooperFxDelayMix (float value)
{
    looperFxDelayMix = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getDelay().setWetMix(value);
}
void PadSettings::setLooperFxDelayTime (float value)
{
    looperFxDelayTime = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getDelay().setDelayTime(value);
}
void PadSettings::setLooperFxDelayFeedback (float value)
{
    looperFxDelayFeedback = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getDelay().setFeedback(value);
}
void PadSettings::setLooperFxDelayLpfFreq (float value)
{
    looperFxDelayLpfFreq = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getDelay().setLpfFrequency(value);
}
void PadSettings::setLooperFxDelayHpfFreq (float value)
{
    looperFxDelayHpfFreq = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getDelay().setHpfFrequency(value);
}
void PadSettings::setLooperFxDelaySync (int value)
{
    looperFxDelaySync = value;
}
void PadSettings::setLooperFxDelayTimeMenu (int value)
{
    looperFxDelayTimeMenu = value;
}
void PadSettings::setLooperFxDelayAlphaTouch (int value)
{
    looperFxDelayAlphaTouch = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getDelay().setAlphaTouchParam(value);
}
void PadSettings::setLooperFxDelayAtReverse (int value)
{
    looperFxDelayAtReverse = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getDelay().setAlphaTouchReverse(value);
}
void PadSettings::setLooperFxDelayAtIntensity (float value)
{
    looperFxDelayAtIntensity = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getDelay().setAlphaTouchIntensity(value);
}
float PadSettings::getLooperFxDelayMix()
{
    return looperFxDelayMix;
}
float PadSettings::getLooperFxDelayTime()
{
    return looperFxDelayTime;
}
float PadSettings::getLooperFxDelayFeedback()
{
    return looperFxDelayFeedback;
}
float PadSettings::getLooperFxDelayLpfFreq()
{
    return looperFxDelayLpfFreq;
}
float PadSettings::getLooperFxDelayHpfFreq()
{
    return looperFxDelayHpfFreq;
}
int PadSettings::getLooperFxDelaySync()
{
    return looperFxDelaySync;
}
int PadSettings::getLooperFxDelayTimeMenu()
{
    return looperFxDelayTimeMenu;
}
int PadSettings::getLooperFxDelayAlphaTouch()
{
    return looperFxDelayAlphaTouch;
}
int PadSettings::getLooperFxDelayAtReverse()
{
    return looperFxDelayAtReverse;
}
float PadSettings::getLooperFxDelayAtIntensity()
{
    return looperFxDelayAtIntensity;
}

#pragma mark Reverb FX
//Reverb
void PadSettings::setLooperFxReverbMix (float value)
{
    looperFxReverbMix = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getReverb().setMix(value);
}
void PadSettings::setLooperFxReverbRoomSize (float value)
{
    looperFxReverbRoomSize = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getReverb().setRoomSize(value);
}
void PadSettings::setLooperFxReverbDamping (float value)
{
    looperFxReverbDamping = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getReverb().setDamping(value);
}
void PadSettings::setLooperFxReverbWidth (float value)
{
    looperFxReverbWidth = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getReverb().setWidth(value);
}
void PadSettings::setLooperFxReverbFreezeMode (float value)
{
    looperFxReverbFreezeMode = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getReverb().setFreezeMode(value);
}
void PadSettings::setLooperFxReverbAlphaTouch (int value)
{
    looperFxReverbAlphaTouch = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getReverb().setAlphaTouchParam(value);
}
void PadSettings::setLooperFxReverbAtReverse (int value)
{
    looperFxReverbAtReverse = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getReverb().setAlphaTouchReverse(value);
}
void PadSettings::setLooperFxReverbAtIntensity (float value)
{
    looperFxReverbAtIntensity = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getReverb().setAlphaTouchIntensity(value);
}
float PadSettings::getLooperFxReverbMix()
{
    return looperFxReverbMix;
}
float PadSettings::getLooperFxReverbRoomSize()
{
    return looperFxReverbRoomSize;
}
float PadSettings::getLooperFxReverbDamping()
{
    return looperFxReverbDamping;
}
float PadSettings::getLooperFxReverbWidth()
{
    return looperFxReverbWidth;
}
float PadSettings::getLooperFxReverbFreezeMode()
{
    return looperFxReverbFreezeMode;
}
int PadSettings::getLooperFxReverbAlphaTouch()
{
    return looperFxReverbAlphaTouch;
}
int PadSettings::getLooperFxReverbAtReverse()
{
    return looperFxReverbAtReverse;
}
float PadSettings::getLooperFxReverbAtIntensity()
{
    return looperFxReverbAtIntensity;
}

#pragma mark Flanger
void PadSettings::setLooperFxFlangerMix (double value)
{
    looperFxFlangerMix = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getFlanger().setMix(value);
}
void PadSettings::setLooperFxFlangerRate(double value)
{
    looperFxFlangerRate = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getFlanger().setRate(value);
}
void PadSettings::setLooperFxFlangerFeedback (double value)
{
    looperFxFlangerFeedback = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getFlanger().setFeedback(value);
}
void PadSettings::setLooperFxFlangerIntensity (double value)
{
    looperFxFlangerIntensity = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getFlanger().setIntensity(value);
}
void PadSettings::setLooperFxFlangerSync (int value)
{
    looperFxFlangerSync = value;
}
void PadSettings::setLooperFxFlangerRateMenu (int value)
{
    looperFxFlangerRateMenu = value;
}
void PadSettings::setLooperFxFlangerAlphaTouch (int value)
{
    looperFxFlangerAlphaTouch = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getFlanger().setAlphaTouchParam(value);
}
void PadSettings::setLooperFxFlangerAtReverse (int value)
{
    looperFxFlangerAtReverse = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getFlanger().setAlphaTouchReverse(value);
}
void PadSettings::setLooperFxFlangerAtIntensity (double value)
{
    looperFxFlangerAtIntensity = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getFlanger().setAlphaTouchIntensity(value);
}
double PadSettings::getLooperFxFlangerMix()
{
    return looperFxFlangerMix;
}
double PadSettings::getLooperFxFlangerRate()
{
    return looperFxFlangerRate;
}
double PadSettings::getLooperFxFlangerFeedback()
{
    return looperFxFlangerFeedback;
}
double PadSettings::getLooperFxFlangerIntensity()
{
    return looperFxFlangerIntensity;
}
int PadSettings::getLooperFxFlangerSync()
{
    return looperFxFlangerSync;
}
int PadSettings::getLooperFxFlangerRateMenu()
{
    return looperFxFlangerRateMenu;
}
int PadSettings::getLooperFxFlangerAlphaTouch()
{
    return looperFxFlangerAlphaTouch;
}
int PadSettings::getLooperFxFlangerAtReverse()
{
    return looperFxFlangerAtReverse;
}
double PadSettings::getLooperFxFlangerAtIntensity()
{
    return looperFxFlangerAtIntensity;
}

#pragma mark Tremolo
void PadSettings::setLooperFxTremoloDepth (double value)
{
    looperFxTremoloDepth = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getTremolo().setDepth(value);
}
void PadSettings::setLooperFxTremoloRate (double value)
{
    looperFxTremoloRate = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getTremolo().setRate(value);
}
void PadSettings::setLooperFxTremoloShape (int value)
{
    looperFxTremoloShape = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getTremolo().setShape(value);
}
void PadSettings::setLooperFxTremoloSync (int value)
{
    looperFxTremoloSync = value;
}
void PadSettings::setLooperFxTremoloRateMenu (int value)
{
    looperFxTremoloRateMenu = value;
}
void PadSettings::setLooperFxTremoloAlphaTouch (int value)
{
    looperFxTremoloAlphaTouch = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getTremolo().setAlphaTouchParam(value);
}
void PadSettings::setLooperFxTremoloAtReverse (int value)
{
    looperFxTremoloAtReverse = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getTremolo().setAlphaTouchReverse(value);
}
void PadSettings::setLooperFxTremoloAtIntensity (double value)
{
    looperFxTremoloAtIntensity = value;
    alphaLiveEngineRef->getModeLooper()->getAudioFilePlayerInstance(padNumber)->getTremolo().setAlphaTouchIntensity(value);
}
double PadSettings::getLooperFxTremoloDepth()
{
    return looperFxTremoloDepth;
}
double PadSettings::getLooperFxTremoloRate()
{
    return looperFxTremoloRate;
}
int PadSettings::getLooperFxTremoloShape()
{
    return looperFxTremoloShape;
}
int PadSettings::getLooperFxTremoloSync()
{
    return looperFxTremoloSync;
}
int PadSettings::getLooperFxTremoloRateMenu()
{
    return looperFxTremoloRateMenu;
}
int PadSettings::getLooperFxTremoloAlphaTouch()
{
    return looperFxTremoloAlphaTouch;
}
int PadSettings::getLooperFxTremoloAtReverse()
{
    return looperFxTremoloAtReverse;
}
double PadSettings::getLooperFxTremoloAtIntensity()
{
    return looperFxTremoloAtIntensity;
}



//==================================================================
//=Sequencer Mode Methods===========================================
//==================================================================
#pragma mark Sequencer mode mutator functions

void PadSettings::setSequencerMode(int value)
{
    sequencerMode = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMode(value);
    }
}




void PadSettings::setSequencerData(int sequenceNumber, int rowNumber, int columnNumber, int value, bool shouldUpdateSeqDataString)
{
    
    sequencerData[sequenceNumber][rowNumber][columnNumber] = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSequenceData(sequenceNumber, rowNumber, columnNumber, value);
    }
    
    //this check is needed as when using the clear/clear all buttons it triggers this function 100's of times per sequence, which is too much for seqDataToString()
    //the equivilent of seqDataToString() is done using clearSequencerDataString() which is called from the button click of clear/clearall
    if (shouldUpdateSeqDataString == true)
    {
        seqDataToString();
    }
}



void PadSettings::setSequencerNumberOfSequences(int value)
{
    sequencerNumberOfSequences = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setNumberOfSequences(value);
    }
}
void PadSettings::setSequencerTriggerMode(int value)
{
    sequencerTriggerMode = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setTriggerMode(value);
    }
}
void PadSettings::setSequencerChannel(int value)
{
    sequencerChannel = value; 
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setChannel(sequencerChannel);
}
void PadSettings::setSequencerLength(int value)
{
    sequencerLength = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSequenceLength(value);
    }
}
void PadSettings::setSequencerQuantizeMode(int value)
{
    sequencerQuantizeMode = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setQuantizeMode(sequencerQuantizeMode);
}
void PadSettings::setSequencerRelativeTempoMode (int value)
{
    sequencerRelativeTempoMode = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setRelativeTempoMode(sequencerRelativeTempoMode);
    }
}


//this function puts this pads sequencer data into 8 strings, one for each sequence. These strings are then used to store the sequence data into the xml files
void PadSettings::seqDataToString()
{
    for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
    {
        sequencerDataString[seq] = String::empty;
        
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
            {
                sequencerDataString[seq] = sequencerDataString[seq] + String(getSequencerData(seq, row, column)) + " ";
            }
       }
        
    }

}

//called when a seq is loaded from file
void PadSettings::stringToSeqData(String savedString, int seqNumber)
{
    // code take from http://www.rawmaterialsoftware.com/viewtopic.php?f=2&t=6933&hilit=split+string
    
    StringArray tokens;
    tokens.addTokens(savedString, " ", String::empty);
    
    int i = 0; //counts the index tokens
    
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
        {
            setSequencerData(seqNumber, row, column, tokens[i].getIntValue(), false);
            i++; //go to next token index
        }
    }
    //put the savedString into the sequencerDataString
    sequencerDataString[seqNumber] = savedString;
}


//called when a seq of the OLD 0-1 format is loaded from file
void PadSettings::stringToSeqDataFormatConversion(String savedString, int seqNumber)
{
    // code take from http://www.rawmaterialsoftware.com/viewtopic.php?f=2&t=6933&hilit=split+string
    
    StringArray tokens;
    tokens.addTokens(savedString, " ", String::empty);
    
    int i = 0; //counts the index tokens
    
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
        {
            int status = 0;
            if (tokens[i].getIntValue() >= 1) //old format 'note' message
            {
                //convert to 110, as the previous '1' will just create a note with a VERY low velocity
                status = 110;
            }
            
            setSequencerData(seqNumber, row, column, status, false);
            i++; //go to next token index
        }
    }
    //amend the seq data string with the new values
    seqDataToString();
    sequencerDataString[seqNumber] = savedString;
}



String PadSettings::getSequencerDataString (int seqNumber)
{
    return sequencerDataString[seqNumber];
}


void PadSettings::clearSequencerDataString (int seqNumber)
{
    //make it a series or 0's. Doing String::empty causes problems somewhere down the line in certain situations. OR DOES IT? check?
     sequencerDataString[seqNumber] = "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ";
    
}


void PadSettings::setSequencerMidiNote(int value, int rowNumber)
{
    sequencerMidiNote[rowNumber] = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiNote(rowNumber, value);
    }
}
void PadSettings::setSequencerMidiVelocity(int value)
{
    sequencerMidiVelocity = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiVelocity(value);
    }
}
void PadSettings::setSequencerMidiChannel(int value)
{
    sequencerMidiChannel = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiChannel(value);
    }
}
void PadSettings::setSequencerMidiNoteLength(int value)
{
    sequencerMidiNoteLength = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiNoteLength(value);
    }
}




void PadSettings::setSequencerSamplesAudioFilePath(File value, int rowNumber)
{
    /*
     If the default option is set, this function creates a copy of the imported audio file in the working directory, 
     and sets the new file path as the loopers audio file location.
     The algorithm used below ensures that there will be no obvious duplicates of audio files,
     but it also ensures that imported files with an existing name might not necessarily be a duplicate!
     
     */
    
    if (value != File::nonexistent)
    {
        if (AppSettings::Instance()->getCopyExternalFiles() == true)
        {
            //create a new file for the imported audio file in the working directory but only if it doesn't already exist!!
            File audioFileCopy (File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + value.getFileName());
            
            if (audioFileCopy.existsAsFile() == false) //if it doesn't yet exist
            {
                //copy the added audio file into the newly created file
                value.copyFileTo(audioFileCopy);
            }
            
            else if (audioFileCopy.existsAsFile() == true) //if it already exists 
            {
                Array<File> matchingFilesArray;
                String fileWildCard (audioFileCopy.getFileNameWithoutExtension()+"*");
                bool importedFileNeedsCopying = true;
                
                //Find all possible duplicates of the imported file using fileWildCard
                //and add reference of all possible files to matchingFilesArray.
                File::getCurrentWorkingDirectory().findChildFiles(matchingFilesArray, 2, false, fileWildCard);
                
                for (int i = 0; i < matchingFilesArray.size(); i++)
                {
                    if (value.hasIdenticalContentTo(matchingFilesArray[i]) == true)
                    {
                        //if it finds a duplicate, flag that the file doesn't need copying
                        importedFileNeedsCopying = false;
                        //set the file
                        audioFileCopy = matchingFilesArray[i];
                        //break out of the for loop once a duplicate has been found to prevent unnecessary checks
                        break;
                    }
                }
                
                if (importedFileNeedsCopying == true) 
                {
                    //if no duplicate was found...
                    //... copy the added file with an appended name
                    audioFileCopy = audioFileCopy.getNonexistentSibling();
                    value.copyFileTo(audioFileCopy); 
                }
                
                value = audioFileCopy;
            }
            
        }
        
        //OLD - store just the FILE NAME of the audio file.
        //sequencerSamplesAudioFilePath[rowNumber] = audioFileCopy.getFileName();
        //NEW - store the full path of the audio file, incase the file isn't copied into the working directory
        sequencerSamplesAudioFilePath[rowNumber] = value.getFullPathName();
    }
    else
        sequencerSamplesAudioFilePath[rowNumber] = String::empty;
    
    
    //if (alphaLiveEngineRef != nullptr)  //THIS IF STATEMENT IS ONLY TEMP WHILE DEFAULT SAMPLES ARE LOADED IN FROM CONSTRUCTOR
    //{
    //set the audio file within the correct SequencerAudioFilePlayer Instance if it exists
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSamplesAudioFile(rowNumber, value);
    }
    //}
}



void PadSettings::setSequencerPan(float value)
{
    sequencerPan = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSamplesPan(value);
    }
}
void PadSettings::setSequencerGain(float value)
{
    sequencerGain = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSamplesGain(value);
    }
}


#pragma mark Sequencer mode accessor functions

int PadSettings::getSequencerMode()
{
    return sequencerMode;
}
int PadSettings::getSequencerData(int sequenceNumber, int rowNumber, int columnNumber)
{
    return sequencerData[sequenceNumber][rowNumber][columnNumber];
}
int PadSettings::getSequencerNumberOfSequences()
{
    return sequencerNumberOfSequences;
}
int PadSettings::getSequencerTriggerMode()
{
    return sequencerTriggerMode;
}
int PadSettings::getSequencerChannel()
{
    return sequencerChannel;
}
int PadSettings::getSequencerLength()
{
    return sequencerLength;
}
int PadSettings::getSequencerQuantizeMode()
{
    return sequencerQuantizeMode;
}
int PadSettings::getSequencerRelativeTempoMode()
{
    return sequencerRelativeTempoMode;
}



int PadSettings::getSequencerMidiNote(int rowNumber)
{
    return sequencerMidiNote[rowNumber];
}
int PadSettings::getSequencerMidiVelocity()
{
    return sequencerMidiVelocity;
}
int PadSettings::getSequencerMidiChannel()
{
    return sequencerMidiChannel;
}
int PadSettings::getSequencerMidiNoteLength()
{
    return sequencerMidiNoteLength;
}


File PadSettings::getSequencerSamplesAudioFilePath(int rowNumber)
{
    if (sequencerSamplesAudioFilePath[rowNumber] != String::empty)
    {
        File audioFile = sequencerSamplesAudioFilePath[rowNumber];
        
        if (audioFile.existsAsFile() == false) //if for some reason it doesn't exist
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File not found!", "\"" + sequencerSamplesAudioFilePath[rowNumber] + "\"" + " for Pad " + String(padNumber+1) + " could not be found.");
            //IN NEXT VERSION USE A YES/NO ALERTWINDOW THAT ASKS IF YOU WOULD LIKE TO MANUALLY LOCATE THE AUDIO FILE.
            //THIS WILL BRING UP A FILEBROWSERWINDOW AND THE CHOSEN FILE WILL THEN BE MOVED TO THE CURRENT WORKING DIRECTORY AND
            //SAVED INTO THE looperAudioFilePath STRING IN THE CORRECT FORMAT.
            
            //set the relevent variables to nonexistent/empty
            audioFile = File::nonexistent;
            sequencerSamplesAudioFilePath[rowNumber] = String::empty;
        }

        return audioFile;
    }
    
    else
        return File::nonexistent;
    
}


float PadSettings::getSequencerPan()
{
    return sequencerPan;
}
float PadSettings::getSequencerGain()
{
    return sequencerGain;
}


//==================================================================
//=Controller Mode Methods==========================================
//==================================================================
#pragma mark Controller mode mutator functions

void PadSettings::setControllerControl (int value)
{
    controllerControl = value;
}
void PadSettings::setControllerPresetNumber (int value)
{
    controllerPresetNumber = value;
}
void PadSettings::setControllerOscIpAddress (String value)
{
    controllerOscIpAddress = value;
}
void PadSettings::setControllerOscPort (int value)
{
    controllerOscPort = value;
}
void PadSettings::setControllerMidiProgramChangeNumber (int value)
{
    controllerMidiProgramChangeNumber = value;
}
void PadSettings::setControllerMidiProgramChangeChannel (int value)
{
    controllerMidiProgramChangeChannel = value;
}

#pragma mark Sequencer mode accessor functions

int PadSettings::getControllerControl()
{
    return controllerControl;
}
int PadSettings::getControllerPresentNumber()
{
    return controllerPresetNumber;
}
String PadSettings::getControllerOscIpAddress()
{
    return controllerOscIpAddress;
}
int PadSettings::getControllerOscPort()
{
    return controllerOscPort;
}
int PadSettings::getControllerMidiProgramChangeNumber()
{
    return controllerMidiProgramChangeNumber;
}
int PadSettings::getControllerMidiProgramChangeChannel()
{
    return controllerMidiProgramChangeChannel;
}







