
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
    exclusiveMode = 0;
    exclusiveGroup = 1;
    quantizeMode = 0; //unquantized
    
    //midi mode
    midiNote = 60;
    midiVelocity = 110;
    midiChannel = 1;
    midiMinPressureRange = 0;
    midiMaxPressureRange = 127;
    midiPressureMode = 1;
    midiTriggerMode = 1;
    midiIndestructible = 0;
    midiSticky = 0;
    midiPressureStatus = true;
    midiNoteStatus = true;
    //default CC layout
    midiCcController = Layouts::ccLayout[padNumber];
    
    //sampler mode
    samplerAudioFilePath = String::empty;
    samplerTriggerMode = 2; 
    samplerShouldLoop = 1;
    samplerIndestructible = 0;
    samplerShouldFinishLoop = 0;
    samplerSticky = 0;
    samplerEffect = 0;
    samplerPan = 0.5;
    samplerGain = 0.9;
    samplerAttackTime = 0;
    samplerReleaseTime = 0;
    
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
    sequencerRelativeTempoMode = 0; //regular time
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
     
    sequencerEffect = 0;
    sequencerPan = 0.5;
    sequencerGain = 1.0;
    sequencerSamplesAttackTime = 0;
    
    //controller mode
    controllerControl = 1;
    controllerSceneNumber = 1;
    controllerOscIpAddress = "127.0.0.1";
    controllerOscPort = 5004;
    controllerMidiProgramChangeNumber = 1;
    controllerMidiProgramChangeChannel = 1;
    
    
    //----pad effects----
    //Gain and Pan
    padFxGainPanGain = 1.0;
    padFxGainPanPan = 0.5;
    padFxGainPanAlphaTouch = 2;
    padFxGainPanAtReverse = 1;
    padFxGainPanAtIntensity = 1.0;
    //LPF
    padFxLpfMix = 1.0;
    padFxLpfFreq = 500;
    padFxLpfBandwidth = 5;
    padFxLpfAlphaTouch = 3;
    padFxLpfAtReverse = 0;
    padFxLpfAtIntensity = 1.0;
    //HPF
    padFxHpfMix = 1.0;
    padFxHpfFreq = 500;
    padFxHpfBandwidth = 5;
    padFxHpfAlphaTouch = 3;
    padFxHpfAtReverse = 0;
    padFxHpfAtIntensity = 1.0;
    //BPF
    padFxBpfMix = 1.0;
    padFxBpfFreq = 500;
    padFxBpfBandwidth = 5;
    padFxBpfAlphaTouch = 3;
    padFxBpfAtReverse = 0;
    padFxBpfAtIntensity = 1.0;
    //Delay
    padFxDelayMix = 0.7;
    padFxDelayTime = 500.0;
    padFxDelayFeedback = 0.5;
    padFxDelayLpfFreq = 20000.0;
    padFxDelayHpfFreq = 30.0;
    padFxDelaySync = 1;
    padFxDelayTimeMenu = 4;
    padFxDelayAlphaTouch = 4;
    padFxDelayAtReverse = 0;
    padFxDelayAtIntensity = 1.0;
    //Reverb
    padFxReverbMix = 0.7;
    padFxReverbRoomSize = 0.25;
    padFxReverbDamping = 0.5;
    padFxReverbWidth = 0.5;
    padFxReverbFreezeMode = 0.2;
    padFxReverbAlphaTouch = 3;
    padFxReverbAtReverse = 0;
    padFxReverbAtIntensity = 1.0;
    //Flanger
    padFxFlangerMix = 0.7;
    padFxFlangerRate = 0.25;
    padFxFlangerFeedback = 0.9;
    padFxFlangerIntensity = 0.1;
    padFxFlangerSync = 1;
    padFxFlangerRateMenu = 4;
    padFxFlangerAlphaTouch = 5;
    padFxFlangerAtReverse = 0;
    padFxFlangerAtIntensity = 1.0;
    //Tremolo
    padFxTremoloDepth = 1.0;
    padFxTremoloRate = 5.0;
    padFxTremoloShape = 1;
    padFxTremoloSync = 1;
    padFxTremoloRateMenu = 3;
    padFxTremoloAlphaTouch = 2;
    padFxTremoloAtReverse = 1;
    padFxTremoloAtIntensity = 1.0;
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
        setMidiPressureMode(1);
        setMidiTriggerMode(1);
        setMidiIndestructible(0);
        setMidiSticky(0);
        setMidiPressureStatus (true);
        setMidiNoteStatus (true);
        setMidiCcController (Layouts::ccLayout[padNumber]);
    }
    
    if (whatToReset != 2)
    {
        setSamplerAudioFilePath (File::nonexistent);
        setSamplerTriggerMode (2);
        setSamplerShouldLoop(1);
        setSamplerIndestructible(0);
        setSamplerShouldFinishLoop(0);
        setSamplerSticky(0);
        setSamplerEffect (0);
        setSamplerPan (0.5);
        setSamplerGain (0.9);
        setSamplerAttackTime(0);
        setSamplerReleaseTime(0);
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
        setSequencerRelativeTempoMode(0);
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
        setSequencerEffect(0);
        setSequencerPan (0.5);
        setSequencerGain (1.0);
        setSequencerSamplesAttackTime (0);
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
    
    
    //what about reseting the FX values?
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
            //the below has to be called here and not within the sequencePlayer destructor
            //as on shutdown PadSettings is deleted before the sequencerPlayer objects.
            //Therefore the app would crash when shutting down as it would try to access
            //pad settings when it doesn't exist, but for some reason it seems to think it does exist.
            setSequencerRecordEnabled (false);
            
            //delete instance of SequencePlayer for pad 'padNumber'
            alphaLiveEngineRef->getModeSequencer()->deleteSequencePlayer(padNumber);
        }
        
        
        mode = value; //mode is set!
        
    }
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
            }
            
            value = audioFileCopy;
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

void PadSettings::setSamplerAttackTime (double value)
{
    samplerAttackTime = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setAttackTime(value);
    }
}

void PadSettings::setSamplerReleaseTime (double value)
{
    samplerReleaseTime = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setReleaseTime(value);
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
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, translate("File not found!"), samplerAudioFilePath + " " + translate("could not be found."));
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

float PadSettings::getSamplerPan()
{
    return samplerPan;
}
float PadSettings::getSamplerGain()
{
    return samplerGain;
}

double PadSettings::getSamplerAttackTime()
{
    return samplerAttackTime;
}

double PadSettings::getSamplerReleaseTime()
{
    return samplerReleaseTime;
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
            }
            
            value = audioFileCopy;
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

void PadSettings::setSequencerEffect(int value)
{
    sequencerEffect = value;
    
    
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSamplesEffect(sequencerEffect);
    }
     
    
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

void PadSettings::setSequencerSamplesAttackTime (double value)
{
    sequencerSamplesAttackTime = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSamplesAttackTime(value);
    }
}

void PadSettings::setSequencerRecordEnabled (bool value)
{
    
    sequencerRecordEnabled = value;
    alphaLiveEngineRef->setRecordingSequencerPadsState(padNumber, value);
    
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setRecordEnabled(value);
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
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, translate("File not found!"), sequencerSamplesAudioFilePath[rowNumber] + " " + translate("could not be found."));
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

int PadSettings::getSequencerEffect()
{
    return sequencerEffect;
}

float PadSettings::getSequencerPan()
{
    return sequencerPan;
}
float PadSettings::getSequencerGain()
{
    return sequencerGain;
}

double PadSettings::getSequencerSamplesAttackTime()
{
    return sequencerSamplesAttackTime;
}


bool PadSettings::getSequencerRecordEnabled()
{
    return sequencerRecordEnabled;
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
int PadSettings::getControllerSceneNumber()
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


#pragma mark Pad Effect functions
//---PAD EFFECTS---
#pragma mark Gain/Pan FX
//Gain&Pan
void PadSettings::setPadFxGainPanGain (float value)
{
    padFxGainPanGain = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setGain(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getGainAndPan().setGain(value);
}
void PadSettings::setPadFxGainPanPan (float value)
{
    padFxGainPanPan = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setPan(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getGainAndPan().setPan(value);
}
void PadSettings::setPadFxGainPanAlphaTouch (int value)
{
    padFxGainPanAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchParam(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchParam(value);
}
void PadSettings::setPadFxGainPanAtReverse (int value)
{
    padFxGainPanAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchReverse(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchReverse(value);
}
void PadSettings::setPadFxGainPanAtIntensity (float value)
{
    padFxGainPanAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchIntensity(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getGainAndPan().setAlphaTouchIntensity(value);
}
float PadSettings::getPadFxGainPanGain()
{
    return padFxGainPanGain;
}
float PadSettings::getPadFxGainPanPan()
{
    return padFxGainPanPan;
}
int PadSettings::getPadFxGainPanAlphaTouch()
{
    return padFxGainPanAlphaTouch;
}
int PadSettings::getPadFxGainPanAtReverse()
{
    return padFxGainPanAtReverse;
}
float PadSettings::getPadFxGainPanAtIntensity()
{
    return padFxGainPanAtIntensity;
}

#pragma mark LPF FX
//LPF
void PadSettings::setPadFxLpfMix (float value)
{
    padFxLpfMix = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setMix(padFxLpfMix);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getLowpassFilter().setMix(padFxLpfMix);
}
void PadSettings::setPadFxLpfFreq (float value)
{
    padFxLpfFreq = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setFrequency(padFxLpfFreq);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getLowpassFilter().setFrequency(padFxLpfFreq);
}
void PadSettings::setPadFxLpfBandwidth (float value)
{
    padFxLpfBandwidth = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setBandwidth(padFxLpfBandwidth);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getLowpassFilter().setBandwidth(padFxLpfBandwidth);
}
void PadSettings::setPadFxLpfAlphaTouch (int value)
{
    padFxLpfAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchParam(padFxLpfAlphaTouch);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchParam(padFxLpfAlphaTouch);
}
void PadSettings::setPadFxLpfAtReverse (int value)
{
    padFxLpfAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchReverse(padFxLpfAtReverse);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchReverse(padFxLpfAtReverse);
    
}
void PadSettings::setPadFxLpfAtIntensity (float value)
{
    padFxLpfAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchIntensity(padFxLpfAtIntensity);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getLowpassFilter().setAlphaTouchIntensity(padFxLpfAtIntensity);
}

float PadSettings::getPadFxLpfMix()
{
    return padFxLpfMix;
}
float PadSettings::getPadFxLpfFreq()
{
    return padFxLpfFreq;
}
float PadSettings::getPadFxLpfBandwidth()
{
    return padFxLpfBandwidth;
}
int PadSettings::getPadFxLpfAlphaTouch()
{
    return padFxLpfAlphaTouch;
}
int PadSettings::getPadFxLpfAtReverse()
{
    return padFxLpfAtReverse;
}
float PadSettings::getPadFxLpfAtIntensity()
{
    return padFxLpfAtIntensity;
}

#pragma mark HPF FX
//HPF
void PadSettings::setPadFxHpfMix (float value)
{
    padFxHpfMix = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setMix(padFxHpfMix);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getHighPassFilter().setMix(padFxHpfMix);
}
void PadSettings::setPadFxHpfFreq (float value)
{
    padFxHpfFreq = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setFrequency(padFxHpfFreq);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getHighPassFilter().setFrequency(padFxHpfFreq);
}
void PadSettings::setPadFxHpfBandwidth (float value)
{
    padFxHpfBandwidth = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setBandwidth(padFxHpfBandwidth);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getHighPassFilter().setBandwidth(padFxHpfBandwidth);
}
void PadSettings::setPadFxHpfAlphaTouch (int value)
{
    padFxHpfAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchParam(padFxHpfAlphaTouch);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchParam(padFxHpfAlphaTouch);
}
void PadSettings::setPadFxHpfAtReverse (int value)
{
    padFxHpfAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchReverse(padFxHpfAtReverse);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchReverse(padFxHpfAtReverse);
    
}
void PadSettings::setPadFxHpfAtIntensity (float value)
{
    padFxHpfAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchIntensity(padFxHpfAtIntensity);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getHighPassFilter().setAlphaTouchIntensity(padFxHpfAtIntensity);
}

float PadSettings::getPadFxHpfMix()
{
    return padFxHpfMix;
}
float PadSettings::getPadFxHpfFreq()
{
    return padFxHpfFreq;
}
float PadSettings::getPadFxHpfBandwidth()
{
    return padFxHpfBandwidth;
}
int PadSettings::getPadFxHpfAlphaTouch()
{
    return padFxHpfAlphaTouch;
}
int PadSettings::getPadFxHpfAtReverse()
{
    return padFxHpfAtReverse;
}
float PadSettings::getPadFxHpfAtIntensity()
{
    return padFxHpfAtIntensity;
}

#pragma mark BPF FX
//BPF
void PadSettings::setPadFxBpfMix (float value)
{
    padFxBpfMix = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setMix(padFxBpfMix);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBandPassFilter().setMix(padFxBpfMix);
}
void PadSettings::setPadFxBpfFreq (float value)
{
    padFxBpfFreq = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setFrequency(padFxBpfFreq);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBandPassFilter().setFrequency(padFxBpfFreq);
}
void PadSettings::setPadFxBpfBandwidth (float value)
{
    padFxBpfBandwidth = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setBandwidth(padFxBpfBandwidth);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBandPassFilter().setBandwidth(padFxBpfBandwidth);
}
void PadSettings::setPadFxBpfAlphaTouch (int value)
{
    padFxBpfAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchParam(padFxBpfAlphaTouch);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchParam(padFxBpfAlphaTouch);
}
void PadSettings::setPadFxBpfAtReverse (int value)
{
    padFxBpfAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchReverse(padFxBpfAtReverse);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchReverse(padFxBpfAtReverse);
    
}
void PadSettings::setPadFxBpfAtIntensity (float value)
{
    padFxBpfAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchIntensity(padFxBpfAtIntensity);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBandPassFilter().setAlphaTouchIntensity(padFxBpfAtIntensity);
}

float PadSettings::getPadFxBpfMix()
{
    return padFxBpfMix;
}
float PadSettings::getPadFxBpfFreq()
{
    return padFxBpfFreq;
}
float PadSettings::getPadFxBpfBandwidth()
{
    return padFxBpfBandwidth;
}
int PadSettings::getPadFxBpfAlphaTouch()
{
    return padFxBpfAlphaTouch;
}
int PadSettings::getPadFxBpfAtReverse()
{
    return padFxBpfAtReverse;
}
float PadSettings::getPadFxBpfAtIntensity()
{
    return padFxBpfAtIntensity;
}

#pragma mark Delay FX
//Delay
void PadSettings::setPadFxDelayMix (float value)
{
    padFxDelayMix = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setWetMix(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDelay().setWetMix(value);
}
void PadSettings::setPadFxDelayTime (float value)
{
    padFxDelayTime = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setDelayTime(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDelay().setDelayTime(value);
}
void PadSettings::setPadFxDelayFeedback (float value)
{
    padFxDelayFeedback = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setFeedback(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDelay().setFeedback(value);
}
void PadSettings::setPadFxDelayLpfFreq (float value)
{
    padFxDelayLpfFreq = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setLpfFrequency(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDelay().setLpfFrequency(value);
}
void PadSettings::setPadFxDelayHpfFreq (float value)
{
    padFxDelayHpfFreq = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setHpfFrequency(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDelay().setHpfFrequency(value);
}
void PadSettings::setPadFxDelaySync (int value)
{
    padFxDelaySync = value;
}
void PadSettings::setPadFxDelayTimeMenu (int value)
{
    padFxDelayTimeMenu = value;
}
void PadSettings::setPadFxDelayAlphaTouch (int value)
{
    padFxDelayAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setAlphaTouchParam(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDelay().setAlphaTouchParam(value);
}
void PadSettings::setPadFxDelayAtReverse (int value)
{
    padFxDelayAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setAlphaTouchReverse(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDelay().setAlphaTouchReverse(value);
}
void PadSettings::setPadFxDelayAtIntensity (float value)
{
    padFxDelayAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDelay().setAlphaTouchIntensity(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDelay().setAlphaTouchIntensity(value);
}
float PadSettings::getPadFxDelayMix()
{
    return padFxDelayMix;
}
float PadSettings::getPadFxDelayTime()
{
    return padFxDelayTime;
}
float PadSettings::getPadFxDelayFeedback()
{
    return padFxDelayFeedback;
}
float PadSettings::getPadFxDelayLpfFreq()
{
    return padFxDelayLpfFreq;
}
float PadSettings::getPadFxDelayHpfFreq()
{
    return padFxDelayHpfFreq;
}
int PadSettings::getPadFxDelaySync()
{
    return padFxDelaySync;
}
int PadSettings::getPadFxDelayTimeMenu()
{
    return padFxDelayTimeMenu;
}
int PadSettings::getPadFxDelayAlphaTouch()
{
    return padFxDelayAlphaTouch;
}
int PadSettings::getPadFxDelayAtReverse()
{
    return padFxDelayAtReverse;
}
float PadSettings::getPadFxDelayAtIntensity()
{
    return padFxDelayAtIntensity;
}

#pragma mark Reverb FX
//Reverb
void PadSettings::setPadFxReverbMix (float value)
{
    padFxReverbMix = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setMix(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getReverb().setMix(value);
}
void PadSettings::setPadFxReverbRoomSize (float value)
{
    padFxReverbRoomSize = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setRoomSize(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getReverb().setRoomSize(value);
}
void PadSettings::setPadFxReverbDamping (float value)
{
    padFxReverbDamping = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setDamping(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getReverb().setDamping(value);
}
void PadSettings::setPadFxReverbWidth (float value)
{
    padFxReverbWidth = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setWidth(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getReverb().setWidth(value);
}
void PadSettings::setPadFxReverbFreezeMode (float value)
{
    padFxReverbFreezeMode = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setFreezeMode(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getReverb().setFreezeMode(value);
}
void PadSettings::setPadFxReverbAlphaTouch (int value)
{
    padFxReverbAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setAlphaTouchParam(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getReverb().setAlphaTouchParam(value);
}
void PadSettings::setPadFxReverbAtReverse (int value)
{
    padFxReverbAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setAlphaTouchReverse(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getReverb().setAlphaTouchReverse(value);
}
void PadSettings::setPadFxReverbAtIntensity (float value)
{
    padFxReverbAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getReverb().setAlphaTouchIntensity(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getReverb().setAlphaTouchIntensity(value);
}
float PadSettings::getPadFxReverbMix()
{
    return padFxReverbMix;
}
float PadSettings::getPadFxReverbRoomSize()
{
    return padFxReverbRoomSize;
}
float PadSettings::getPadFxReverbDamping()
{
    return padFxReverbDamping;
}
float PadSettings::getPadFxReverbWidth()
{
    return padFxReverbWidth;
}
float PadSettings::getPadFxReverbFreezeMode()
{
    return padFxReverbFreezeMode;
}
int PadSettings::getPadFxReverbAlphaTouch()
{
    return padFxReverbAlphaTouch;
}
int PadSettings::getPadFxReverbAtReverse()
{
    return padFxReverbAtReverse;
}
float PadSettings::getPadFxReverbAtIntensity()
{
    return padFxReverbAtIntensity;
}

#pragma mark Flanger
void PadSettings::setPadFxFlangerMix (double value)
{
    padFxFlangerMix = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setMix(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getFlanger().setMix(value);
    
}
void PadSettings::setPadFxFlangerRate(double value)
{
    padFxFlangerRate = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setRate(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getFlanger().setRate(value);
}
void PadSettings::setPadFxFlangerFeedback (double value)
{
    padFxFlangerFeedback = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setFeedback(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getFlanger().setFeedback(value);
}
void PadSettings::setPadFxFlangerIntensity (double value)
{
    padFxFlangerIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setIntensity(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getFlanger().setIntensity(value);
}
void PadSettings::setPadFxFlangerSync (int value)
{
    padFxFlangerSync = value;
}
void PadSettings::setPadFxFlangerRateMenu (int value)
{
    padFxFlangerRateMenu = value;
}
void PadSettings::setPadFxFlangerAlphaTouch (int value)
{
    padFxFlangerAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setAlphaTouchParam(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getFlanger().setAlphaTouchParam(value);
}
void PadSettings::setPadFxFlangerAtReverse (int value)
{
    padFxFlangerAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setAlphaTouchReverse(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getFlanger().setAlphaTouchReverse(value);
}
void PadSettings::setPadFxFlangerAtIntensity (double value)
{
    padFxFlangerAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getFlanger().setAlphaTouchIntensity(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getFlanger().setAlphaTouchIntensity(value);
}
double PadSettings::getPadFxFlangerMix()
{
    return padFxFlangerMix;
}
double PadSettings::getPadFxFlangerRate()
{
    return padFxFlangerRate;
}
double PadSettings::getPadFxFlangerFeedback()
{
    return padFxFlangerFeedback;
}
double PadSettings::getPadFxFlangerIntensity()
{
    return padFxFlangerIntensity;
}
int PadSettings::getPadFxFlangerSync()
{
    return padFxFlangerSync;
}
int PadSettings::getPadFxFlangerRateMenu()
{
    return padFxFlangerRateMenu;
}
int PadSettings::getPadFxFlangerAlphaTouch()
{
    return padFxFlangerAlphaTouch;
}
int PadSettings::getPadFxFlangerAtReverse()
{
    return padFxFlangerAtReverse;
}
double PadSettings::getPadFxFlangerAtIntensity()
{
    return padFxFlangerAtIntensity;
}

#pragma mark Tremolo
void PadSettings::setPadFxTremoloDepth (double value)
{
    padFxTremoloDepth = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setDepth(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getTremolo().setDepth(value);
}
void PadSettings::setPadFxTremoloRate (double value)
{
    padFxTremoloRate = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setRate(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getTremolo().setRate(value);
}
void PadSettings::setPadFxTremoloShape (int value)
{
    padFxTremoloShape = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setShape(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getTremolo().setShape(value);
}
void PadSettings::setPadFxTremoloSync (int value)
{
    padFxTremoloSync = value;
}
void PadSettings::setPadFxTremoloRateMenu (int value)
{
    padFxTremoloRateMenu = value;
}
void PadSettings::setPadFxTremoloAlphaTouch (int value)
{
    padFxTremoloAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setAlphaTouchParam(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getTremolo().setAlphaTouchParam(value);
}
void PadSettings::setPadFxTremoloAtReverse (int value)
{
    padFxTremoloAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setAlphaTouchReverse(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getTremolo().setAlphaTouchReverse(value);
}
void PadSettings::setPadFxTremoloAtIntensity (double value)
{
    padFxTremoloAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getTremolo().setAlphaTouchIntensity(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getTremolo().setAlphaTouchIntensity(value);
}
double PadSettings::getPadFxTremoloDepth()
{
    return padFxTremoloDepth;
}
double PadSettings::getPadFxTremoloRate()
{
    return padFxTremoloRate;
}
int PadSettings::getPadFxTremoloShape()
{
    return padFxTremoloShape;
}
int PadSettings::getPadFxTremoloSync()
{
    return padFxTremoloSync;
}
int PadSettings::getPadFxTremoloRateMenu()
{
    return padFxTremoloRateMenu;
}
int PadSettings::getPadFxTremoloAlphaTouch()
{
    return padFxTremoloAlphaTouch;
}
int PadSettings::getPadFxTremoloAtReverse()
{
    return padFxTremoloAtReverse;
}
double PadSettings::getPadFxTremoloAtIntensity()
{
    return padFxTremoloAtIntensity;
}

