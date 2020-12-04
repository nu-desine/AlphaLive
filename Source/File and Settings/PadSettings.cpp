
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
    pressureCurve = 2; //Linear
    exclusiveMode = 0;
    exclusiveGroup = 1;
    quantizeMode = 0; //unquantized
    
    //hard code top row of pads to be static 
    if (padNumber < 40)
        velocityCurve = 3; //Log
    else
        velocityCurve = 4; //Static
    
    staticVelocity = 100;
    velocityMinRange = 0;
    velocityMaxRange = 127;
    
    //midi mode
    midiNote = 60;
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
    
    midiDynamicChannelStatus = false;
    for (int i = 0; i < 4; i++)
        midiDynamicChannels[i] = true;
    for (int i = 4; i < 16; i++)
        midiDynamicChannels[i] = false;
    
    //sampler mode
    samplerAudioFilePath = String();
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
    samplerPolyphony = 1;
    
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
        sequencerDataString[seq] = String();
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
    
    sequencerMidiChannel = 1;
    sequencerMidiNoteLength = 4;
    sequencerMidiMinPressureRange = 0;
    sequencerMidiMaxPressureRange = 127;
    sequencerMidiPressureMode = 1;
    sequencerMidiCcController = Layouts::ccLayout[padNumber];
    sequencerMidiPressureStatus = true;
    
    for (int i = 0; i <= NO_OF_ROWS-1; i++)
    {
        sequencerSamplesAudioFilePath[i] = String();
    }
     
    sequencerEffect = 0;
    sequencerPan = 0.5;
    sequencerGain = 1.0;
    sequencerSamplesAttackTime = 0;
    sequencerSamplesPolyphony = 4;
    
    sequencerRecordEnabled = false;
    
    //controller mode
    controllerControl = 1;
    controllerSceneNumber = 1;
    controllerOscIpAddress = "127.0.0.1";
    controllerOscPort = 5004;
    controllerMidiProgramChangeNumber = 1;
    controllerMidiProgramChangeChannel = 1;
    controllerPressureLatchPadNumber = 0;
    if (padNumber == 0)
        controllerPressureLatchPadNumber = 1;
    controllerLedControl = 1;
    
    
    //----pad effects----
    double tempo = 120; //default tempo
    
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
    padFxDelayTime = 30000.0/tempo;
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
    padFxFlangerRate = (tempo/60.0) * 0.25;
    padFxFlangerFeedback = 0.9;
    padFxFlangerIntensity = 0.1;
    padFxFlangerSync = 1;
    padFxFlangerRateMenu = 4;
    padFxFlangerAlphaTouch = 5;
    padFxFlangerAtReverse = 0;
    padFxFlangerAtIntensity = 1.0;
    //Tremolo
    padFxTremoloDepth = 1.0;
    padFxTremoloRate = (tempo/60.0) * 1.0;
    padFxTremoloShape = 1;
    padFxTremoloSync = 1;
    padFxTremoloRateMenu = 3;
    padFxTremoloAlphaTouch = 2;
    padFxTremoloAtReverse = 1;
    padFxTremoloAtIntensity = 1.0;
	//Distortion
	padFxDistortionInputGain = 1.0;
	padFxDistortionDrive = 50;
	padFxDistortionTone = 0.8;
	padFxDistortionWetDryMix = 0.5;
	padFxDistortionType = 1;
	padFxDistortionAlphaTouch = 3;
	padFxDistortionAtReverse = 1;
	padFxDistortionAtIntensity = 1.0;
	//Bitcrusher
	padFxBitcrusherInputGain = 1;
	padFxBitcrusherDownsample = 16;
	padFxBitcrusherCrush = 2;
	padFxBitcrusherSmoothing = 0.92;
	padFxBitcrusherWetDryMix = 0.64;
	padFxBitcrusherAlphaTouch = 2;
	padFxBitcrusherAtReverse = 0;
	padFxBitcrusherAtIntensity = 1.0;
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
        setPressureCurve(2);
        setExclusiveMode(0);
        setExclusiveGroup(1);
        setQuantizeMode(0);
        
        //hard code top row of pads to be static 
        if (padNumber < 40)
            setVelocityCurve(3);
        else
            setVelocityCurve(4);
        
        setStaticVelocity(100);
        setVelocityMinRange(0);
        setVelocityMaxRange(127);
    }
    
    if (whatToReset != 1)
    {
        setMidiNote(60);
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
        
        setMidiDynamicChannelStatus(false);
        for (int i = 0; i < 4; i++)
            setMidiDynamicChannels(i, true);
        for (int i = 4; i < 16; i++)
            setMidiDynamicChannels(i, false);
    }
    
    if (whatToReset != 2)
    {
        setSamplerAudioFilePath (File());
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
        setSamplerPolyphony (1);
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
        setSequencerMidiChannel (1);
        setSequencerMidiNoteLength (4);
        setSequencerMidiMinPressureRange (0);
        setSequencerMidiMaxPressureRange (127);
        setSequencerMidiPressureMode (1);
        setSequencerMidiPressureStatus (true);
        setSequencerMidiCcController (Layouts::ccLayout[padNumber]);
        
        for (int i = 0; i <= NO_OF_ROWS-1; i++)
        {
            setSequencerSamplesAudioFilePath (File(), i);
        }
        setSequencerEffect(0);
        setSequencerPan (0.5);
        setSequencerGain (1.0);
        setSequencerSamplesAttackTime (0);
        setSequencerSamplesPolyphony (4);
        
        setSequencerRecordEnabled(false);
    }
    
    if (whatToReset != 4)
    {
        setControllerControl(1);
        setControllerSceneNumber(1);
        setControllerOscIpAddress("127.0.0.1");
        setControllerOscPort(5004);
        setControllerMidiProgramChangeNumber(1);
        setControllerMidiProgramChangeChannel(1);
        setControllerPressureLatchPadNumber(0);
        if (padNumber == 0)
            setControllerPressureLatchPadNumber(1);
        setControllerLedControl(1);
    }
    
    
    //what about reseting the FX values?
}

void PadSettings::changeGuiPadText()
{
    if (StoredSettings::getInstance()->padContentDisplay == 2)
    {
        alphaLiveEngineRef->changeGuiPadText(padNumber);
    }
}

void PadSettings::setTempo (double value)
{
    //set the rate of any tempo-relative effects
    
    //essentially the below functions are called as they can grab the new tempo
    //and reset the effect rate if they need to (if they are currently set to sync)
    
    if (mode == 2)
    {
        if (samplerEffect == 7)
            setPadFxDelayTimeMenu(padFxDelayTimeMenu);
        else if (samplerEffect == 9)
            setPadFxFlangerRateMenu(padFxFlangerRateMenu);
        else if (samplerEffect == 10)
            setPadFxTremoloRateMenu(padFxTremoloRateMenu);
        
    }
    else if (mode == 3 && sequencerMode == 2)
    {
        if (sequencerEffect == 7)
            setPadFxDelayTimeMenu(padFxDelayTimeMenu);
        else if (sequencerEffect == 9)
            setPadFxFlangerRateMenu(padFxFlangerRateMenu);
        else if (sequencerEffect == 10)
            setPadFxTremoloRateMenu(padFxTremoloRateMenu);
    }
    
}

#pragma mark Global mutator functions
//==================================================================
//=Global Methods===================================================
//==================================================================

void PadSettings::setMode(int value)
{
    
    
    if (value != mode) //if mode is different to previously
    {
        //=============================================================================
        
        if (mode == 1) //if previous mode = MIDI...
        {
            //kill the pad if currently active...
            alphaLiveEngineRef->getModeMidi()->killPad(padNumber);
        }
        else if (mode == 2) //if previous mode = Sampler
        {
            //delete instance of AudioFilePlayer for pad 'padNumber',
            //killing the pad if it is currently active
            alphaLiveEngineRef->getModeSampler()->deleteAudioFilePlayer(padNumber);
            
        }
        else if (mode == 3) //if previous mode = Sequencer...
        {
            //delete instance of SequencePlayer for pad 'padNumber',
            //killing the pad if it is currently active
            alphaLiveEngineRef->getModeSequencer()->deleteSequencePlayer(padNumber);
        }
        else if (mode == 4) //if previous mode = Controller...
        {
            //kill the pad if currently active...
            alphaLiveEngineRef->getModeController()->killPad(padNumber);
        }
        
        //=============================================================================
        
        if (value == 1) //if new mode = MIDI...
        {
            //This needs to be called as when copying&pasting pad data the new values need to be set within
            //modeMidi. This is handled automatically for other modes (inc. Controller Mode?)
            alphaLiveEngineRef->getModeMidi()->setPadData(padNumber);
        }
        else if (value == 2) //if new mode = Sampler...
        {
            //create an instance of AudioFilePlayer for pad 'padNumber'
            alphaLiveEngineRef->getModeSampler()->createAudioFilePlayer(padNumber);
        }
        else if (value == 3) //if new mode = Sequencer...
        {
            //create an instance of SequencePlayer for pad 'padNumber'
            alphaLiveEngineRef->getModeSequencer()->createSequencePlayer(padNumber);
        }
        
        //=============================================================================
        
        mode = value; //mode is set!
        
    }
}


void PadSettings::setPressureCurve (int value)
{
    pressureCurve = value;
}

void PadSettings::setExclusiveMode (int value)
{
    exclusiveMode = value;
    changeGuiPadText();
}

void PadSettings::setExclusiveGroup (int value)
{
    exclusiveGroup = value;
    changeGuiPadText();
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

void PadSettings::setVelocityCurve (int value)
{
    //for top row of pads, hard code to static velocity for now
    if (padNumber < 40)
        velocityCurve = value;
    else
        velocityCurve = 4;
}

void PadSettings::setStaticVelocity(int value)
{
    staticVelocity = value;
}

void PadSettings::setVelocityMinRange(int value)
{
    velocityMinRange = value;
}

void PadSettings::setVelocityMaxRange(int value)
{
    velocityMaxRange = value;
}


#pragma mark Global accessor functions

int PadSettings::getMode()
{
    return mode;
}

int PadSettings::getPressureCurve()
{
    return pressureCurve;
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
int PadSettings::getStaticVelocity()
{
    return staticVelocity;
}
int PadSettings::getVelocityCurve()
{
    return velocityCurve;
}
int PadSettings::getVelocityMinRange()
{
    return velocityMinRange;
}
int PadSettings::getVelocityMaxRange()
{
    return velocityMaxRange;
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
    changeGuiPadText();
}

void PadSettings::setMidiChannel(int value)
{
    midiChannel = value;
    alphaLiveEngineRef->getModeMidi()->setChannel(value, padNumber);
    changeGuiPadText();
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
    changeGuiPadText();
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
    changeGuiPadText();
}
void PadSettings::setMidiNoteStatus (bool value)
{
    midiNoteStatus = value;
    alphaLiveEngineRef->getModeMidi()->setNoteStatus(value, padNumber);
    changeGuiPadText();
}
void PadSettings::setMidiCcController (int value)
{
    midiCcController = value;
    alphaLiveEngineRef->getModeMidi()->setControllerNumber(value, padNumber);
    changeGuiPadText();
}
void PadSettings::setMidiDynamicChannelStatus (bool value)
{
    midiDynamicChannelStatus = value;
    alphaLiveEngineRef->getModeMidi()->setAutoMidiChannelStatus(value, padNumber);
    changeGuiPadText();
}
void PadSettings::setMidiDynamicChannels (int channel, bool status)
{
    midiDynamicChannels[channel] = status;
    alphaLiveEngineRef->getModeMidi()->setAutoMidiChannels(channel, status, padNumber);
}

#pragma mark MIDI mode accessor functions

int PadSettings::getMidiNote()
{
    return midiNote;
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
bool PadSettings::getMidiDynamicChannelStatus()
{
    return midiDynamicChannelStatus;
}
bool PadSettings::getMidiDynamicChannels (int channel)
{
    return midiDynamicChannels[channel];
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
    
    //Within PadSettings (and most of the application) all audio files are stored as absolute file paths.
    //It is only in AppDocumentState and when the files are saved to XML files that the file can be
    //stored as just a file name.
    
    if (value != File())
    {
        if (AppSettings::Instance()->getCopyExternalFiles() == true)
        {
            //create a new file for the imported audio file in the working directory but only if it doesn't already exist!!
            File audioFileCopy (File::getCurrentWorkingDirectory().getFullPathName() + File::getSeparatorString() + value.getFileName());
            
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
        samplerAudioFilePath = String();
    
    //set the audio file within the correct AudioFilePlayer Instance if it exists
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setAudioFile(value);
        changeGuiPadText();
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
        changeGuiPadText();
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

void PadSettings::setSamplerPolyphony (int value)
{
    samplerPolyphony = value;
    if (alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->setPolyphony(value);
    }
}

#pragma mark Sampler mode accessor functions

File PadSettings::getSamplerAudioFilePath()
{
    //Within PadSettings (and most of the application) all audio files are stored as absolute file paths.
    //It is only in AppDocumentState and when the files are saved to XML files that the file can be
    //stored as just a file name.
    
    if (samplerAudioFilePath != String())
    {
        File audioFile = samplerAudioFilePath;
        
        if (audioFile.existsAsFile() == false) //if for some reason it doesn't exist
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, translate("File not found!"), samplerAudioFilePath + " " + translate("could not be found."));
            //IN NEXT VERSION USE A YES/NO ALERTWINDOW THAT ASKS IF YOU WOULD LIKE TO MANUALLY LOCATE THE AUDIO FILE.
            //THIS WILL BRING UP A FILEBROWSERWINDOW AND THE CHOSEN FILE WILL THEN BE MOVED TO THE CURRENT WORKING DIRECTORY AND
            //SAVED INTO THE samplerAudioFilePath STRING IN THE CORRECT FORMAT.
            
            //set the relevent variables to nonexistent/empty
            audioFile = File();
            samplerAudioFilePath = String();
        }
        
        return audioFile;
    }
    
    else
        return File();
    
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

int PadSettings::getSamplerPolyphony()
{
    return samplerPolyphony;
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
        changeGuiPadText();
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
        sequencerDataString[seq] = String();
        
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
    tokens.addTokens(savedString, " ", String());
    
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
    tokens.addTokens(savedString, " ", String());
    
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
    //make it a series or 0's. Doing String() causes problems somewhere down the line in certain situations. OR DOES IT? check?
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
void PadSettings::setSequencerMidiChannel(int value)
{
    sequencerMidiChannel = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiChannel(value);
        changeGuiPadText();
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
        changeGuiPadText();
    }
}
void PadSettings::setSequencerMidiPressureStatus (bool value)
{
    sequencerMidiPressureStatus = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiPressureStatus(value);
        changeGuiPadText();
    }
}
void PadSettings::setSequencerMidiCcController (int value)
{
    sequencerMidiCcController = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setMidiControllerNumber(value);
        changeGuiPadText();
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
    
    //Within PadSettings (and most of the application) all audio files are stored as absolute file paths.
    //It is only in AppDocumentState and when the files are saved to XML files that the file can be
    //stored as just a file name.
    
    if (value != File())
    {
        if (AppSettings::Instance()->getCopyExternalFiles() == true)
        {
            //create a new file for the imported audio file in the working directory but only if it doesn't already exist!!
            File audioFileCopy (File::getCurrentWorkingDirectory().getFullPathName() + File::getSeparatorString() + value.getFileName());
            
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
        sequencerSamplesAudioFilePath[rowNumber] = String();
    
    
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
        changeGuiPadText();
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

void PadSettings::setSequencerSamplesPolyphony (int value)
{
    sequencerSamplesPolyphony = value;
    if (alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber) != nullptr)
    {
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->setSamplesPolyphony(value);
    }
}

void PadSettings::setSequencerRecordEnabled (bool value)
{
    sequencerRecordEnabled = value;
    alphaLiveEngineRef->setRecordingSequencerPadsState(padNumber, value);
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
    //Within PadSettings (and most of the application) all audio files are stored as absolute file paths.
    //It is only in AppDocumentState and when the files are saved to XML files that the file can be
    //stored as just a file name.
    
    if (sequencerSamplesAudioFilePath[rowNumber] != String())
    {
        File audioFile = sequencerSamplesAudioFilePath[rowNumber];
        
        if (audioFile.existsAsFile() == false) //if for some reason it doesn't exist
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, translate("File not found!"), sequencerSamplesAudioFilePath[rowNumber] + " " + translate("could not be found."));
            //IN NEXT VERSION USE A YES/NO ALERTWINDOW THAT ASKS IF YOU WOULD LIKE TO MANUALLY LOCATE THE AUDIO FILE.
            //THIS WILL BRING UP A FILEBROWSERWINDOW AND THE CHOSEN FILE WILL THEN BE MOVED TO THE CURRENT WORKING DIRECTORY AND
            //SAVED INTO THE samplerAudioFilePath STRING IN THE CORRECT FORMAT.
            
            //set the relevent variables to nonexistent/empty
            audioFile = File();
            sequencerSamplesAudioFilePath[rowNumber] = String();
        }

        return audioFile;
    }
    
    else
        return File();
    
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

int PadSettings::getSequencerSamplesPolyphony()
{
    return sequencerSamplesPolyphony;
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
    alphaLiveEngineRef->getModeController()->setControl(value, padNumber);
    changeGuiPadText();
}
void PadSettings::setControllerSceneNumber (int value)
{
    controllerSceneNumber = value;
    changeGuiPadText();
}
void PadSettings::setControllerOscIpAddress (String value)
{
    controllerOscIpAddress = value;
    changeGuiPadText();
}
void PadSettings::setControllerOscPort (int value)
{
    controllerOscPort = value;
    changeGuiPadText();
}
void PadSettings::setControllerMidiProgramChangeNumber (int value)
{
    controllerMidiProgramChangeNumber = value;
    changeGuiPadText();
}
void PadSettings::setControllerMidiProgramChangeChannel (int value)
{
    controllerMidiProgramChangeChannel = value;
    changeGuiPadText();
}
void PadSettings::setControllerPressureLatchPadNumber (int value)
{
    controllerPressureLatchPadNumber = value;
    changeGuiPadText();
}
void PadSettings::setControllerLedControl (int value)
{
    controllerLedControl = value;
    changeGuiPadText();
}

#pragma mark Controller mode accessor functions

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
int PadSettings::getControllerPressureLatchPadNumber()
{
    return controllerPressureLatchPadNumber;
}
int PadSettings::getControllerLedControl()
{
    return controllerLedControl;
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
    
    if (padFxDelaySync == true)
    {
        //work out delay time based on beat length selected and the tempo.
        double tempo = AppSettings::Instance()->getGlobalTempo();
        
        switch (padFxDelayTimeMenu)
        {
            case 1: // 4 beats
                setPadFxDelayTime(240000.0/tempo);
                break;
            case 2: // 2 beats
                setPadFxDelayTime(120000.0/tempo);
                break;
            case 3: // 1 beat
                setPadFxDelayTime(60000.0/tempo);
                break;
            case 4: //half beat
                setPadFxDelayTime(30000.0/tempo);
                break;
            case 5: // quarter beat
                setPadFxDelayTime(15000.0/tempo);
                break;
            default: // 1 beat
                setPadFxDelayTime(60000.0/tempo);
                break;
        }
    }
    
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
    
    if (padFxFlangerSync == true)
    {
        //work out LFO rate based on rate selected and the tempo.
        double tempo = AppSettings::Instance()->getGlobalTempo();
        double bps = tempo/60.0;
        
        switch (padFxFlangerRateMenu)
        {
            case 1: // 8 bars
                setPadFxFlangerRate(bps * 0.03125);
                break;
            case 2: // 4 bars
                setPadFxFlangerRate(bps * 0.0625);
                break;
            case 3: // 2 bars
                setPadFxFlangerRate(bps * 0.125);
                break;
            case 4: // 1 bar - 4 beats
                setPadFxFlangerRate(bps * 0.25);
                break;
            case 5: // 1/2 - 2 beats
                setPadFxFlangerRate(bps * 0.5);
                break;
            case 6: // 1/4 - 1 beat
                setPadFxFlangerRate(bps * 1.0);
                break;
            case 7: // 1/8 - half beat
                setPadFxFlangerRate(bps * 2.0);
                break;
            case 8: // 1/16 - quarter beat
                setPadFxFlangerRate(bps * 4.0);
                break;
            case 9: // 1/32 - 8th beat
                setPadFxFlangerRate(bps * 8.0);
                break;
            default: // 1/4 - 1 beat
                setPadFxFlangerRate(bps * 1.0);
                break;
        }
    }
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
    
    if (padFxTremoloSync == true)
    {
        //work out LFO rate based on rate selected and the tempo.
        double tempo = AppSettings::Instance()->getGlobalTempo();
        double bps = tempo/60.0;
        
        switch (padFxTremoloRateMenu)
        {
            case 1: // 1/1 - 4 beats
                setPadFxTremoloRate(bps * 0.25);
                break;
            case 2: // 1/2 - 2 beats
                setPadFxTremoloRate(bps * 0.5);
                break;
            case 3: // 1/4 - 1 beat
                setPadFxTremoloRate(bps * 1.0);
                break;
            case 4: // 1/8 - half beat
                setPadFxTremoloRate(bps * 2.0);
                break;
            case 5: // 1/16 - quarter beat
                setPadFxTremoloRate(bps * 4.0);
                break;
            case 6: // 1/32 - 8th beat
                setPadFxTremoloRate(bps * 8.0);
                break;
            case 7: // 1/64 - 16th beat
                setPadFxTremoloRate(bps * 16.0);
                break;
            default: // 1/4 - 1 beat
                setPadFxTremoloRate(bps * 1.0);
                break;
        }
    }
    
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


#pragma mark Distortion
void PadSettings::setPadFxDistortionInputGain (double value)
{
    padFxDistortionInputGain = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDistortion().setInputGain(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDistortion().setInputGain(value);
}
void PadSettings::setPadFxDistortionDrive (double value)
{
    padFxDistortionDrive = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDistortion().setDrive(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDistortion().setDrive(value);
}
void PadSettings::setPadFxDistortionWetDryMix (double value)
{
    padFxDistortionWetDryMix = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDistortion().setMix(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDistortion().setMix(value);
}
void PadSettings::setPadFxDistortionTone (double value)
{
    padFxDistortionTone = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDistortion().setTone(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDistortion().setTone(value);
}
void PadSettings::setPadFxDistortionAlphaTouch (int value)
{
    padFxDistortionAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDistortion().setAlphaTouchParam(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDistortion().setAlphaTouchParam(value);
}
void PadSettings::setPadFxDistortionAtReverse (int value)
{
    padFxDistortionAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDistortion().setAlphaTouchReverse(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDistortion().setAlphaTouchReverse(value);
}
void PadSettings::setPadFxDistortionAtIntensity (double value)
{
    padFxDistortionAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDistortion().setAlphaTouchIntensity(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDistortion().setAlphaTouchIntensity(value);
}

void PadSettings::setPadFxDistortionTypeMenu (int value)
{
    padFxDistortionType = value;
	
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getDistortion().setDistortionTypeMenu(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getDistortion().setDistortionTypeMenu(value);
}

double PadSettings::getPadFxDistortionInputGain()
{
    return padFxDistortionInputGain;
}
double PadSettings::getPadFxDistortionDrive()
{
    return padFxDistortionDrive;
}
double PadSettings::getPadFxDistortionWetDryMix()
{
    return padFxDistortionWetDryMix;
}
double PadSettings::getPadFxDistortionTone()
{
    return padFxDistortionTone;
}
int PadSettings::getPadFxDistortionTypeMenu()
{
    return padFxDistortionType;
}
int PadSettings::getPadFxDistortionAlphaTouch()
{
    return padFxDistortionAlphaTouch;
}
int PadSettings::getPadFxDistortionAtReverse()
{
    return padFxDistortionAtReverse;
}
float PadSettings::getPadFxDistortionAtIntensity()
{
    return padFxDistortionAtIntensity;
}


#pragma mark Bitcrusher
void PadSettings::setPadFxBitcrusherInputGain (double value)
{
    padFxBitcrusherInputGain = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBitcrusher().setInputGain(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBitcrusher().setInputGain(value);
}
void PadSettings::setPadFxBitcrusherDownsample (int value)
{
    padFxBitcrusherDownsample = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBitcrusher().setDownsample(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBitcrusher().setDownsample(value);
}
void PadSettings::setPadFxBitcrusherCrush (int value)
{
    padFxBitcrusherCrush = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBitcrusher().setCrush(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBitcrusher().setCrush(value);
}
void PadSettings::setPadFxBitcrusherWetDryMix (double value)
{
    padFxBitcrusherWetDryMix = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBitcrusher().setMix(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBitcrusher().setMix(value);
}
void PadSettings::setPadFxBitcrusherSmoothing (double value)
{
    padFxBitcrusherSmoothing = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBitcrusher().setSmoothing(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBitcrusher().setSmoothing(value);
}
void PadSettings::setPadFxBitcrusherAlphaTouch (int value)
{
    padFxBitcrusherAlphaTouch = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBitcrusher().setAlphaTouchParam(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBitcrusher().setAlphaTouchParam(value);
}
void PadSettings::setPadFxBitcrusherAtReverse (int value)
{
    padFxBitcrusherAtReverse = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBitcrusher().setAlphaTouchReverse(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBitcrusher().setAlphaTouchReverse(value);
}
void PadSettings::setPadFxBitcrusherAtIntensity (double value)
{
    padFxBitcrusherAtIntensity = value;
    if (mode == 2)
        alphaLiveEngineRef->getModeSampler()->getAudioFilePlayerInstance(padNumber)->getBitcrusher().setAlphaTouchIntensity(value);
    else if (mode == 3)
        alphaLiveEngineRef->getModeSequencer()->getSequencePlayerInstance(padNumber)->getBitcrusher().setAlphaTouchIntensity(value);
}

double PadSettings::getPadFxBitcrusherInputGain()
{
    return padFxBitcrusherInputGain;
}
int PadSettings::getPadFxBitcrusherDownsample()
{
    return padFxBitcrusherDownsample;
}
int PadSettings::getPadFxBitcrusherCrush()
{
    return padFxBitcrusherCrush;
}
double PadSettings::getPadFxBitcrusherWetDryMix()
{
    return padFxBitcrusherWetDryMix;
}
double PadSettings::getPadFxBitcrusherSmoothing()
{
    return padFxBitcrusherSmoothing;
}
int PadSettings::getPadFxBitcrusherAlphaTouch()
{
    return padFxBitcrusherAlphaTouch;
}
int PadSettings::getPadFxBitcrusherAtReverse()
{
    return padFxBitcrusherAtReverse;
}
float PadSettings::getPadFxBitcrusherAtIntensity()
{
    return padFxBitcrusherAtIntensity;
}

