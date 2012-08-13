
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
    exclusiveMode = 0;
    exclusiveGroup = 1;
    quantizeMode = 0; //unquantized
    
    //midi mode
    midiNote = 60;
    midiVelocity = 110;
    midiChannel = 1;
    midiMinPressureRange = 0;
    midiMaxPressureRange = 127;
    midiPressureMode = 0;
    midiTriggerMode = 0;
    midiIndestructible = 0;
    midiSticky = 0;
    midiPressureStatus = true;
    midiNoteStatus = true;
    //default CC layout
    midiCcController = Layouts::ccLayout[padNumber];
    
    //sampler mode
    samplerAudioFilePath = String::empty;
    samplerTriggerMode = 1; 
    samplerShouldLoop = 1;
    samplerIndestructible = 0;
    samplerShouldFinishLoop = 0;
    samplerSticky = 0;
    samplerEffect = 0;
    samplerPressureStatus = 0;
    samplerPan = 0.5;
    samplerGain = 1.0;
    
    //----sampler effects----
    //Gain and Pan
    samplerFxGainPanGain = 0.5;
    samplerFxGainPanPan = 0.5;
    samplerFxGainPanAlphaTouch = 2;
    samplerFxGainPanAtReverse = 0;
    samplerFxGainPanAtIntensity = 1.0;
    //LPF
    samplerFxLpfMix = 0.0;
    samplerFxLpfFreq = 1000;
    samplerFxLpfBandwidth = 5;
    samplerFxLpfAlphaTouch = 2;
    samplerFxLpfAtReverse = 0;
    samplerFxLpfAtIntensity = 1.0;
    //HPF
    samplerFxHpfMix = 0.0;
    samplerFxHpfFreq = 1000;
    samplerFxHpfBandwidth = 5;
    samplerFxHpfAlphaTouch = 2;
    samplerFxHpfAtReverse = 0;
    samplerFxHpfAtIntensity = 1.0;
    //BPF
    samplerFxBpfMix = 0.0;
    samplerFxBpfFreq = 1000;
    samplerFxBpfBandwidth = 5;
    samplerFxBpfAlphaTouch = 2;
    samplerFxBpfAtReverse = 0;
    samplerFxBpfAtIntensity = 1.0;
    //Delay
    samplerFxDelayMix = 0.0;
    samplerFxDelayTime = 500.0;
    samplerFxDelayFeedback = 0.5;
    samplerFxDelayLpfFreq = 20000.0;
    samplerFxDelayHpfFreq = 30.0;
    samplerFxDelaySync = 1;
    samplerFxDelayTimeMenu = 3;
    samplerFxDelayAlphaTouch = 2;
    samplerFxDelayAtReverse = 0;
    samplerFxDelayAtIntensity = 1.0;
    //Reverb
    samplerFxReverbMix = 0.0;
    samplerFxReverbRoomSize = 0.5;
    samplerFxReverbDamping = 0.5;
    samplerFxReverbWidth = 0.5;
    samplerFxReverbFreezeMode = 0.2;
    samplerFxReverbAlphaTouch = 2;
    samplerFxReverbAtReverse = 0;
    samplerFxReverbAtIntensity = 1.0;
    //Flanger
    samplerFxFlangerMix = 0.0;
    samplerFxFlangerRate = 0.25;
    samplerFxFlangerFeedback = 0.9;
    samplerFxFlangerIntensity = 0.1;
    samplerFxFlangerSync = 1;
    samplerFxFlangerRateMenu = 4;
    samplerFxFlangerAlphaTouch = 2;
    samplerFxFlangerAtReverse = 0;
    samplerFxFlangerAtIntensity = 1.0;
    //Tremolo
    samplerFxTremoloDepth = 0.0;
    samplerFxTremoloRate = 5.0;
    samplerFxTremoloShape = 1;
    samplerFxTremoloSync = 1;
    samplerFxTremoloRateMenu = 3;
    samplerFxTremoloAlphaTouch = 2;
    samplerFxTremoloAtReverse = 0;
    samplerFxTremoloAtIntensity = 1.0;
    
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
    sequencerNumberOfSequences = 1; 
    sequencerTriggerMode = 2; 
    sequencerShouldLoop = 1;
    sequencerIndestructible = 0;
    sequencerShouldFinishLoop = 0;
    sequencerSticky = 0;
    sequencerLength = 32;
    sequencerRelativeTempoMode = 3; //regular time
    sequencerDynamicMode = 0;
    
    //set a default sequencer note midi layout of 60-72
    for (int i = 0, num = 60; i <= NO_OF_ROWS-1; i++)
    {
        sequencerMidiNote[i] = num;
        num ++;
    }
    
    sequencerMidiVelocity = 110;
    sequencerMidiChannel = 1;
    sequencerMidiNoteLength = 4;
    sequencerMidiMinPressureRange = 0;
    sequencerMidiMaxPressureRange = 127;
    sequencerMidiPressureMode = 1;
    sequencerMidiCcController = Layouts::ccLayout[padNumber];
    sequencerMidiPressureStatus = true;
    
    for (int i = 0; i <= NO_OF_ROWS-1; i++)
    {
        sequencerSamplesAudioFilePath[i] = String::empty;
    }
     
    sequencerPan = 0.5;
    sequencerGain = 1.0;
    
    //controller mode
    controllerControl = 1;
    controllerSceneNumber = 1;
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
    //this is going to be called when 'reset all' is clicked on the scene component
    //and when the 'delete scene' option is chosen for a single scene
    
    /*
     whatToReset is a flag variable that lets this the program know exactly what data needs to be reset.
     
     If this is called from the scene component 'reset all' button or the 'delete scene' drop down item,
     Everything must be reset including the mode.
     If this is called from the 'clean up project' button or from when a new project is loaded,
     only the data from the modes which this pad isn't set to needs to be reset back to the default values.
     
     whatToReset values:
     0 - reset everything
     1 - everything but MIDI mode
     2 - everything but Sampler mode
     3 - everything but sequencer mode
     4 - everything but controller mode
     
     */
    
    if (whatToReset == 0)
    {
        setMode(0);
        setPressureSensitivityMode(2);
        setExclusiveMode(0);
        setExclusiveGroup(1);
        setQuantizeMode(0);
    }
    
    if (whatToReset != 1)
    {
        setMidiNote(60);
        setMidiVelocity(110);
        setMidiChannel(1);
        setMidiMinPressureRange(0);
        setMidiMaxPressureRange(127);
        setMidiPressureMode(0);
        setMidiTriggerMode(0);
        setMidiIndestructible(0);
        setMidiSticky(0);
        setMidiPressureStatus (true);
        setMidiNoteStatus (true);
        setMidiCcController (Layouts::ccLayout[padNumber]);
    }
    
    if (whatToReset != 2)
    {
        setSamplerAudioFilePath (File::nonexistent);
        setSamplerTriggerMode (1);
        setSamplerShouldLoop(1);
        setSamplerIndestructible(0);
        setSamplerShouldFinishLoop(0);
        setSamplerSticky(0);
        setSamplerEffect (0);
        setSamplerPressureStatus(false);
        setSamplerPan (0.5);
        setSamplerGain (1.0);
        
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
        setSequencerNumberOfSequences (1);
        setSequencerTriggerMode (2);
        setSequencerShouldLoop(1);
        setSequencerIndestructible(0);
        setSequencerShouldFinishLoop(0);
        setSequencerSticky(0);
        setSequencerLength (32);
        setSequencerRelativeTempoMode(3);
        setSequencerDynamicMode(0);
        
        for (int i = 0, num = 60; i <= NO_OF_ROWS-1; i++)
        {
            setSequencerMidiNote (num, i);
            num ++;
        }
        setSequencerMidiVelocity (110);
        setSequencerMidiChannel (1);
        setSequencerMidiNoteLength (4);
        setSequencerMidiMinPressureRange (0);
        setSequencerMidiMaxPressureRange (127);
        setSequencerMidiPressureMode (1);
        setSequencerMidiPressureStatus (true);
        setSequencerMidiCcController (Layouts::ccLayout[padNumber]);
        
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
        setControllerSceneNumber(1);
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
        
        
        if (value == 2 && mode != 2) //to prevent Sampler AudioFilePlayer objects being re-created
        {
            //create an instance of AudioFilePlayer for pad 'padNumber'
            alphaLiveEngineRef->getModeSampler()->createAudioFilePlayer(padNumber);
        }
        else if (mode == 2 && value != 2) //to prevent non-existent Sampler AudioFilePlayer objects from being deleted
        {
            //delete instance of AudioFilePlayer for pad 'padNumber'
            alphaLiveEngineRef->getModeSampler()->deleteAudioFilePlayer(padNumber);
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

void PadSettings::setExclusiveMode (int value)
{
    exclusiveMode = value;
}

void PadSettings::setExclusiveGroup (int value)
{
    exclusiveGroup = value;
}
void PadSettings::setQuantizeMode (int value)
{
    quantizeMode = value;
    
    alphaLiveEngineRef->getModeMidi()->setQuantizeMode(value, padNumber);
    
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setQuantizeMode(value);
    }
    
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setQuantizeMode(value);
    }
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
int PadSettings::getExclusiveMode()
{
    return exclusiveMode;
}
int PadSettings::getExclusiveGroup()
{
    return exclusiveGroup;
}
int PadSettings::getQuantizeMode()
{
    return quantizeMode;
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
void PadSettings::setMidiIndestructible (int value)
{
    midiIndestructible = value;
    alphaLiveEngineRef->getModeMidi()->setIndestructible(value, padNumber);
}
void PadSettings::setMidiSticky (int value)
{
    midiSticky = value;
    alphaLiveEngineRef->getModeMidi()->setSticky(value, padNumber);
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
int PadSettings::getMidiIndestructible()
{
    return midiIndestructible;
}
int PadSettings::getMidiSticky()
{
    return midiSticky;
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



#pragma mark Sampler mode mutator functions
//==================================================================
//=Sampler Mode Methods==============================================
//==================================================================

void PadSettings::setSamplerAudioFilePath(File value)
{
    /*
     If the default option is set, this function creates a copy of the imported audio file in the working directory, 
     and sets the new file path as the samplers audio file location.
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
        //samplerAudioFilePath = value.getFileName();
        //NEW - store the full path of the audio file, incase the file isn't copied into the working directory
        samplerAudioFilePath = value.getFullPathName();
    }
    else
        samplerAudioFilePath = String::empty; 
    
    //set the audio file within the correct AudioFilePlayer Instance if it exists
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setAudioFile(value);
    }
}


void PadSettings::setSamplerTriggerMode(int value)
{
    samplerTriggerMode = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setTriggerMode(samplerTriggerMode);
    }
}

void PadSettings::setSamplerShouldLoop (int value)
{
    samplerShouldLoop = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setShouldLoop(samplerShouldLoop);
    }
    
}
void PadSettings::setSamplerIndestructible (int value)
{
    samplerIndestructible = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setIndestructible(samplerIndestructible);
    }
}
void PadSettings::setSamplerShouldFinishLoop (int value)
{
    samplerShouldFinishLoop = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setShouldFinishLoop(samplerShouldFinishLoop);
    }
}

void PadSettings::setSamplerSticky (int value)
{
    samplerSticky = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setSticky(samplerSticky);
    }
}

void PadSettings::setSamplerEffect(int value)
{
    //std::cout << "Pad " << padNumber << " set with effect " << value << std::endl;
    samplerEffect = value;
    
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setEffect(samplerEffect);
    }
    
}
void PadSettings::setSamplerPressureStatus(bool value)
{

    samplerPressureStatus = value;
    
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setPressureStatus(value);
    }
    
}
void PadSettings::setSamplerPan(float value)
{
    samplerPan = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setPan(samplerPan);
    }
}
void PadSettings::setSamplerGain(float value)
{
    samplerGain = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setGain(samplerGain);
    }
}

#pragma mark Sampler mode accessor functions

File PadSettings::getSamplerAudioFilePath()
{
    if (samplerAudioFilePath != String::empty)
    {
        File audioFile = samplerAudioFilePath;
        
        if (audioFile.existsAsFile() == false) //if for some reason it doesn't exist
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File not found!", "\"" + samplerAudioFilePath + "\"" + " for Pad " + String(padNumber+1) + " could not be found.");
            //IN NEXT VERSION USE A YES/NO ALERTWINDOW THAT ASKS IF YOU WOULD LIKE TO MANUALLY LOCATE THE AUDIO FILE.
            //THIS WILL BRING UP A FILEBROWSERWINDOW AND THE CHOSEN FILE WILL THEN BE MOVED TO THE CURRENT WORKING DIRECTORY AND
            //SAVED INTO THE samplerAudioFilePath STRING IN THE CORRECT FORMAT.
            
            //set the relevent variables to nonexistent/empty
            audioFile = File::nonexistent;
            samplerAudioFilePath = String::empty;
        }
        
        return audioFile;
    }
    
    else
        return File::nonexistent;
    
}


int PadSettings::getSamplerTriggerMode()
{
    return samplerTriggerMode;
}

int PadSettings::getSamplerShouldLoop()
{
    return samplerShouldLoop;
}
int PadSettings::getSamplerIndestructible()
{
    return samplerIndestructible;
}
int PadSettings::getSamplerShouldFinishLoop()
{
    return samplerShouldFinishLoop;
}
int PadSettings::getSamplerSticky()
{
    return samplerSticky;
}

int PadSettings::getSamplerEffect()
{
    return samplerEffect;
}
bool PadSettings::getSamplerPressureStatus()
{
    return samplerPressureStatus;
}
float PadSettings::getSamplerPan()
{
    return samplerPan;
}
float PadSettings::getSamplerGain()
{
    return samplerGain;
}


#pragma mark Sampler FX functions
//---SAMPLER EFFECTS---
#pragma mark Gain/Pan FX
//Gain&Pan
void PadSettings::setSamplerFxGainPanGain (float value)
{
    samplerFxGainPanGain = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setGain(value);
}
void PadSettings::setSamplerFxGainPanPan (float value)
{
    samplerFxGainPanPan = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setPan(value);
}
void PadSettings::setSamplerFxGainPanAlphaTouch (int value)
{
    samplerFxGainPanAlphaTouch = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchParam(value);
}
void PadSettings::setSamplerFxGainPanAtReverse (int value)
{
    samplerFxGainPanAtReverse = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchReverse(value);
}
void PadSettings::setSamplerFxGainPanAtIntensity (float value)
{
    samplerFxGainPanAtIntensity = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchIntensity(value);
}
float PadSettings::getSamplerFxGainPanGain()
{
    return samplerFxGainPanGain;
}
float PadSettings::getSamplerFxGainPanPan()
{
    return samplerFxGainPanPan;
}
int PadSettings::getSamplerFxGainPanAlphaTouch()
{
    return samplerFxGainPanAlphaTouch;
}
int PadSettings::getSamplerFxGainPanAtReverse()
{
    return samplerFxGainPanAtReverse;
}
float PadSettings::getSamplerFxGainPanAtIntensity()
{
    return samplerFxGainPanAtIntensity;
}

#pragma mark LPF FX
//LPF
void PadSettings::setSamplerFxLpfMix (float value)
{
    samplerFxLpfMix = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setMix(samplerFxLpfMix);
}
void PadSettings::setSamplerFxLpfFreq (float value)
{
    samplerFxLpfFreq = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setFrequency(samplerFxLpfFreq);
}
void PadSettings::setSamplerFxLpfBandwidth (float value)
{
    samplerFxLpfBandwidth = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setBandwidth(samplerFxLpfBandwidth);
}
void PadSettings::setSamplerFxLpfAlphaTouch (int value)
{
    samplerFxLpfAlphaTouch = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchParam(samplerFxLpfAlphaTouch);
}
void PadSettings::setSamplerFxLpfAtReverse (int value)
{
    samplerFxLpfAtReverse = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchReverse(samplerFxLpfAtReverse);
    
}
void PadSettings::setSamplerFxLpfAtIntensity (float value)
{
    samplerFxLpfAtIntensity = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchIntensity(samplerFxLpfAtIntensity);
}

float PadSettings::getSamplerFxLpfMix()
{
    return samplerFxLpfMix;
}
float PadSettings::getSamplerFxLpfFreq()
{
    return samplerFxLpfFreq;
}
float PadSettings::getSamplerFxLpfBandwidth()
{
    return samplerFxLpfBandwidth;
}
int PadSettings::getSamplerFxLpfAlphaTouch()
{
    return samplerFxLpfAlphaTouch;
}
int PadSettings::getSamplerFxLpfAtReverse()
{
    return samplerFxLpfAtReverse;
}
float PadSettings::getSamplerFxLpfAtIntensity()
{
    return samplerFxLpfAtIntensity;
}

#pragma mark HPF FX
//HPF
void PadSettings::setSamplerFxHpfMix (float value)
{
    samplerFxHpfMix = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setMix(samplerFxHpfMix);
}
void PadSettings::setSamplerFxHpfFreq (float value)
{
    samplerFxHpfFreq = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setFrequency(samplerFxHpfFreq);
}
void PadSettings::setSamplerFxHpfBandwidth (float value)
{
    samplerFxHpfBandwidth = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setBandwidth(samplerFxHpfBandwidth);
}
void PadSettings::setSamplerFxHpfAlphaTouch (int value)
{
    samplerFxHpfAlphaTouch = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchParam(samplerFxHpfAlphaTouch);
}
void PadSettings::setSamplerFxHpfAtReverse (int value)
{
    samplerFxHpfAtReverse = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchReverse(samplerFxHpfAtReverse);
    
}
void PadSettings::setSamplerFxHpfAtIntensity (float value)
{
    samplerFxHpfAtIntensity = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchIntensity(samplerFxHpfAtIntensity);
}

float PadSettings::getSamplerFxHpfMix()
{
    return samplerFxHpfMix;
}
float PadSettings::getSamplerFxHpfFreq()
{
    return samplerFxHpfFreq;
}
float PadSettings::getSamplerFxHpfBandwidth()
{
    return samplerFxHpfBandwidth;
}
int PadSettings::getSamplerFxHpfAlphaTouch()
{
    return samplerFxHpfAlphaTouch;
}
int PadSettings::getSamplerFxHpfAtReverse()
{
    return samplerFxHpfAtReverse;
}
float PadSettings::getSamplerFxHpfAtIntensity()
{
    return samplerFxHpfAtIntensity;
}

#pragma mark BPF FX
//BPF
void PadSettings::setSamplerFxBpfMix (float value)
{
    samplerFxBpfMix = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setMix(samplerFxBpfMix);
}
void PadSettings::setSamplerFxBpfFreq (float value)
{
    samplerFxBpfFreq = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setFrequency(samplerFxBpfFreq);
}
void PadSettings::setSamplerFxBpfBandwidth (float value)
{
    samplerFxBpfBandwidth = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setBandwidth(samplerFxBpfBandwidth);
}
void PadSettings::setSamplerFxBpfAlphaTouch (int value)
{
    samplerFxBpfAlphaTouch = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchParam(samplerFxBpfAlphaTouch);
}
void PadSettings::setSamplerFxBpfAtReverse (int value)
{
    samplerFxBpfAtReverse = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchReverse(samplerFxBpfAtReverse);
    
}
void PadSettings::setSamplerFxBpfAtIntensity (float value)
{
    samplerFxBpfAtIntensity = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchIntensity(samplerFxBpfAtIntensity);
}

float PadSettings::getSamplerFxBpfMix()
{
    return samplerFxBpfMix;
}
float PadSettings::getSamplerFxBpfFreq()
{
    return samplerFxBpfFreq;
}
float PadSettings::getSamplerFxBpfBandwidth()
{
    return samplerFxBpfBandwidth;
}
int PadSettings::getSamplerFxBpfAlphaTouch()
{
    return samplerFxBpfAlphaTouch;
}
int PadSettings::getSamplerFxBpfAtReverse()
{
    return samplerFxBpfAtReverse;
}
float PadSettings::getSamplerFxBpfAtIntensity()
{
    return samplerFxBpfAtIntensity;
}

#pragma mark Delay FX
//Delay
void PadSettings::setSamplerFxDelayMix (float value)
{
    samplerFxDelayMix = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setWetMix(value);
}
void PadSettings::setSamplerFxDelayTime (float value)
{
    samplerFxDelayTime = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setDelayTime(value);
}
void PadSettings::setSamplerFxDelayFeedback (float value)
{
    samplerFxDelayFeedback = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setFeedback(value);
}
void PadSettings::setSamplerFxDelayLpfFreq (float value)
{
    samplerFxDelayLpfFreq = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setLpfFrequency(value);
}
void PadSettings::setSamplerFxDelayHpfFreq (float value)
{
    samplerFxDelayHpfFreq = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setHpfFrequency(value);
}
void PadSettings::setSamplerFxDelaySync (int value)
{
    samplerFxDelaySync = value;
}
void PadSettings::setSamplerFxDelayTimeMenu (int value)
{
    samplerFxDelayTimeMenu = value;
}
void PadSettings::setSamplerFxDelayAlphaTouch (int value)
{
    samplerFxDelayAlphaTouch = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setAlphaTouchParam(value);
}
void PadSettings::setSamplerFxDelayAtReverse (int value)
{
    samplerFxDelayAtReverse = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setAlphaTouchReverse(value);
}
void PadSettings::setSamplerFxDelayAtIntensity (float value)
{
    samplerFxDelayAtIntensity = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setAlphaTouchIntensity(value);
}
float PadSettings::getSamplerFxDelayMix()
{
    return samplerFxDelayMix;
}
float PadSettings::getSamplerFxDelayTime()
{
    return samplerFxDelayTime;
}
float PadSettings::getSamplerFxDelayFeedback()
{
    return samplerFxDelayFeedback;
}
float PadSettings::getSamplerFxDelayLpfFreq()
{
    return samplerFxDelayLpfFreq;
}
float PadSettings::getSamplerFxDelayHpfFreq()
{
    return samplerFxDelayHpfFreq;
}
int PadSettings::getSamplerFxDelaySync()
{
    return samplerFxDelaySync;
}
int PadSettings::getSamplerFxDelayTimeMenu()
{
    return samplerFxDelayTimeMenu;
}
int PadSettings::getSamplerFxDelayAlphaTouch()
{
    return samplerFxDelayAlphaTouch;
}
int PadSettings::getSamplerFxDelayAtReverse()
{
    return samplerFxDelayAtReverse;
}
float PadSettings::getSamplerFxDelayAtIntensity()
{
    return samplerFxDelayAtIntensity;
}

#pragma mark Reverb FX
//Reverb
void PadSettings::setSamplerFxReverbMix (float value)
{
    samplerFxReverbMix = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setMix(value);
}
void PadSettings::setSamplerFxReverbRoomSize (float value)
{
    samplerFxReverbRoomSize = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setRoomSize(value);
}
void PadSettings::setSamplerFxReverbDamping (float value)
{
    samplerFxReverbDamping = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setDamping(value);
}
void PadSettings::setSamplerFxReverbWidth (float value)
{
    samplerFxReverbWidth = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setWidth(value);
}
void PadSettings::setSamplerFxReverbFreezeMode (float value)
{
    samplerFxReverbFreezeMode = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setFreezeMode(value);
}
void PadSettings::setSamplerFxReverbAlphaTouch (int value)
{
    samplerFxReverbAlphaTouch = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setAlphaTouchParam(value);
}
void PadSettings::setSamplerFxReverbAtReverse (int value)
{
    samplerFxReverbAtReverse = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setAlphaTouchReverse(value);
}
void PadSettings::setSamplerFxReverbAtIntensity (float value)
{
    samplerFxReverbAtIntensity = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setAlphaTouchIntensity(value);
}
float PadSettings::getSamplerFxReverbMix()
{
    return samplerFxReverbMix;
}
float PadSettings::getSamplerFxReverbRoomSize()
{
    return samplerFxReverbRoomSize;
}
float PadSettings::getSamplerFxReverbDamping()
{
    return samplerFxReverbDamping;
}
float PadSettings::getSamplerFxReverbWidth()
{
    return samplerFxReverbWidth;
}
float PadSettings::getSamplerFxReverbFreezeMode()
{
    return samplerFxReverbFreezeMode;
}
int PadSettings::getSamplerFxReverbAlphaTouch()
{
    return samplerFxReverbAlphaTouch;
}
int PadSettings::getSamplerFxReverbAtReverse()
{
    return samplerFxReverbAtReverse;
}
float PadSettings::getSamplerFxReverbAtIntensity()
{
    return samplerFxReverbAtIntensity;
}

#pragma mark Flanger
void PadSettings::setSamplerFxFlangerMix (double value)
{
    samplerFxFlangerMix = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setMix(value);
}
void PadSettings::setSamplerFxFlangerRate(double value)
{
    samplerFxFlangerRate = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setRate(value);
}
void PadSettings::setSamplerFxFlangerFeedback (double value)
{
    samplerFxFlangerFeedback = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setFeedback(value);
}
void PadSettings::setSamplerFxFlangerIntensity (double value)
{
    samplerFxFlangerIntensity = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setIntensity(value);
}
void PadSettings::setSamplerFxFlangerSync (int value)
{
    samplerFxFlangerSync = value;
}
void PadSettings::setSamplerFxFlangerRateMenu (int value)
{
    samplerFxFlangerRateMenu = value;
}
void PadSettings::setSamplerFxFlangerAlphaTouch (int value)
{
    samplerFxFlangerAlphaTouch = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setAlphaTouchParam(value);
}
void PadSettings::setSamplerFxFlangerAtReverse (int value)
{
    samplerFxFlangerAtReverse = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setAlphaTouchReverse(value);
}
void PadSettings::setSamplerFxFlangerAtIntensity (double value)
{
    samplerFxFlangerAtIntensity = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setAlphaTouchIntensity(value);
}
double PadSettings::getSamplerFxFlangerMix()
{
    return samplerFxFlangerMix;
}
double PadSettings::getSamplerFxFlangerRate()
{
    return samplerFxFlangerRate;
}
double PadSettings::getSamplerFxFlangerFeedback()
{
    return samplerFxFlangerFeedback;
}
double PadSettings::getSamplerFxFlangerIntensity()
{
    return samplerFxFlangerIntensity;
}
int PadSettings::getSamplerFxFlangerSync()
{
    return samplerFxFlangerSync;
}
int PadSettings::getSamplerFxFlangerRateMenu()
{
    return samplerFxFlangerRateMenu;
}
int PadSettings::getSamplerFxFlangerAlphaTouch()
{
    return samplerFxFlangerAlphaTouch;
}
int PadSettings::getSamplerFxFlangerAtReverse()
{
    return samplerFxFlangerAtReverse;
}
double PadSettings::getSamplerFxFlangerAtIntensity()
{
    return samplerFxFlangerAtIntensity;
}

#pragma mark Tremolo
void PadSettings::setSamplerFxTremoloDepth (double value)
{
    samplerFxTremoloDepth = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setDepth(value);
}
void PadSettings::setSamplerFxTremoloRate (double value)
{
    samplerFxTremoloRate = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setRate(value);
}
void PadSettings::setSamplerFxTremoloShape (int value)
{
    samplerFxTremoloShape = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setShape(value);
}
void PadSettings::setSamplerFxTremoloSync (int value)
{
    samplerFxTremoloSync = value;
}
void PadSettings::setSamplerFxTremoloRateMenu (int value)
{
    samplerFxTremoloRateMenu = value;
}
void PadSettings::setSamplerFxTremoloAlphaTouch (int value)
{
    samplerFxTremoloAlphaTouch = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setAlphaTouchParam(value);
}
void PadSettings::setSamplerFxTremoloAtReverse (int value)
{
    samplerFxTremoloAtReverse = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setAlphaTouchReverse(value);
}
void PadSettings::setSamplerFxTremoloAtIntensity (double value)
{
    samplerFxTremoloAtIntensity = value;
    alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setAlphaTouchIntensity(value);
}
double PadSettings::getSamplerFxTremoloDepth()
{
    return samplerFxTremoloDepth;
}
double PadSettings::getSamplerFxTremoloRate()
{
    return samplerFxTremoloRate;
}
int PadSettings::getSamplerFxTremoloShape()
{
    return samplerFxTremoloShape;
}
int PadSettings::getSamplerFxTremoloSync()
{
    return samplerFxTremoloSync;
}
int PadSettings::getSamplerFxTremoloRateMenu()
{
    return samplerFxTremoloRateMenu;
}
int PadSettings::getSamplerFxTremoloAlphaTouch()
{
    return samplerFxTremoloAlphaTouch;
}
int PadSettings::getSamplerFxTremoloAtReverse()
{
    return samplerFxTremoloAtReverse;
}
double PadSettings::getSamplerFxTremoloAtIntensity()
{
    return samplerFxTremoloAtIntensity;
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
void PadSettings::setSequencerShouldLoop (int value)
{
    sequencerShouldLoop = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setShouldLoop(value);
    }
    
}
void PadSettings::setSequencerIndestructible (int value)
{
    sequencerIndestructible = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setIndestructible(value);
    }
}
void PadSettings::setSequencerShouldFinishLoop (int value)
{
    sequencerShouldFinishLoop = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setShouldFinishLoop(value);
    }
}

void PadSettings::setSequencerSticky (int value)
{
    sequencerSticky = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSticky(value);
    }
}

void PadSettings::setSequencerLength(int value)
{
    sequencerLength = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSequenceLength(value);
    }
}

void PadSettings::setSequencerRelativeTempoMode (int value)
{
    sequencerRelativeTempoMode = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setRelativeTempoMode(sequencerRelativeTempoMode);
    }
}
void PadSettings::setSequencerDynamicMode (int value)
{
    sequencerDynamicMode = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setDynamicMode(value);
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

void PadSettings::setSequencerMidiMinPressureRange (int value)
{
    sequencerMidiMinPressureRange = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiMinRange(value);
    }
}
void PadSettings::setSequencerMidiMaxPressureRange (int value)
{
    sequencerMidiMaxPressureRange = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiMaxRange(value);
    }
}
void PadSettings::setSequencerMidiPressureMode (int value)
{
    sequencerMidiPressureMode = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiPressureMode(value);
    }
}
void PadSettings::setSequencerMidiPressureStatus (bool value)
{
    sequencerMidiPressureStatus = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiPressureStatus(value);
    }
}
void PadSettings::setSequencerMidiCcController (int value)
{
    sequencerMidiCcController = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiControllerNumber(value);
    }
}




void PadSettings::setSequencerSamplesAudioFilePath(File value, int rowNumber)
{
    /*
     If the default option is set, this function creates a copy of the imported audio file in the working directory, 
     and sets the new file path as the samplers audio file location.
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
int PadSettings::getSequencerShouldLoop()
{
    return sequencerShouldLoop;
}
int PadSettings::getSequencerIndestructible()
{
    return sequencerIndestructible;
}
int PadSettings::getSequencerShouldFinishLoop()
{
    return sequencerShouldFinishLoop;
}
int PadSettings::getSequencerSticky()
{
    return sequencerSticky;
}

int PadSettings::getSequencerLength()
{
    return sequencerLength;
}

int PadSettings::getSequencerRelativeTempoMode()
{
    return sequencerRelativeTempoMode;
}
int PadSettings::getSequencerDynamicMode()
{
    return sequencerDynamicMode;
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
int PadSettings::getSequencerMidiMinPressureRange()
{
    return sequencerMidiMinPressureRange;
}
int PadSettings::getSequencerMidiMaxPressureRange()
{
    return sequencerMidiMaxPressureRange;
}
int PadSettings::getSequencerMidiPressureMode()
{
    return sequencerMidiPressureMode;
}
bool PadSettings::getSequencerMidiPressureStatus()
{
    return sequencerMidiPressureStatus;
}
int PadSettings::getSequencerMidiCcController()
{
    return sequencerMidiCcController;
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
            //SAVED INTO THE samplerAudioFilePath STRING IN THE CORRECT FORMAT.
            
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
void PadSettings::setControllerSceneNumber (int value)
{
    controllerSceneNumber = value;
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
    return controllerSceneNumber;
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







