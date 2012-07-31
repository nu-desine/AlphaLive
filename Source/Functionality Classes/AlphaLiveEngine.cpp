//
//  AlphaLiveEngine.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 19/09/2011.
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

#include "AlphaLiveEngine.h"
#include "../File and Settings/AppSettings.h"
#include "Other/LayoutsAndScales.cpp"


#define PAD_SETTINGS AppSettings::Instance()->padSettings[recievedPad]

AlphaLiveEngine::AlphaLiveEngine()
{
    
    gain = gainPrev =  AppSettings::Instance()->getGlobalGain();
    panLeft = panLeftPrev = PanControl::leftChanPan_(AppSettings::Instance()->getGlobalPan());
    panRight = panRightPrev = PanControl::rightChanPan_(AppSettings::Instance()->getGlobalPan());
    
    
    recievedPad = 0;
    recievedValue = 0;
    
    pendingUpdatePadValue = 0;
    
    guiUpdateFlag = 0;
    padNumberForPlayingStatus = 0;
    playingStatus = 0;
    
    //==========================================================================
    // initialise the device manager with no settings so that it picks a
	// default device to use.
	const String error (audioDeviceManager.initialise (0, /* number of input channels */
													   2, /* number of output channels */
													   0, /* no XML settings.. */
													   true  /* select default device on failure */));
	if (error.isNotEmpty())
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 "AlphaLive",
									 "Couldn't open an output device!\n\n" + error);
	}
    
    audioDeviceManager.addAudioCallback (this);
    
    
    //SET UP MIDI OUTPUT
    
    #if JUCE_MAC || JUCE_LINUX
    //==========================================================================
    //Create new virtual MIDI device
    midiOutputDevice = MidiOutput::createNewDevice("AlphaLive");
    
    if(midiOutputDevice)
    {
        midiOutputDevice->startBackgroundThread();
    }
    else
    {
        std::cout << "Failed to create a virtual MIDI device!" << std::endl;
    }
    //==========================================================================
    #endif //JUCE_MAC || JUCE_LINUX
     
     
     
    
    #if JUCE_WINDOWS
    //==========================================================================
    //connect to a MIDI device
    midiOutputDevice = NULL;
    
    /*
    //connect to a default MIDI device?
    StringArray devices = MidiOutput::getDevices();
    audioDeviceManager.setDefaultMidiOutput(devices[0]);
    midiOutputDevice = MidiOutput::openDevice(MidiOutput::getDefaultDeviceIndex());
    
    if(midiOutputDevice)
        midiOutputDevice->startBackgroundThread();
    else
        std::cout << "Failed to connect to a MIDI device!" << std::endl;
     */
    
    //==========================================================================
    #endif //JUCE_WINDOWS

    modeMidi = new ModeMidi(*midiOutputDevice, *this);
    modeLooper = new ModeLooper(*this);
    modeSequencer = new ModeSequencer(*midiOutputDevice, *this);
    modeController = new ModeController();
    modeController->setMidiOutputDevice(*midiOutputDevice);
    
    audioMixer.addInputSource(modeLooper,false); //add as inputsource to audioMixer
    audioMixer.addInputSource(modeSequencer,false); //add as inputsource to audioMixer
    audioPlayer.setSource(&audioMixer);
    
    
    isDualOutputMode = false;
    oscIpAddress = "127.0.0.1";
    oscPortNumber = 5004;
    
    //global clock stuff
    globalClock = new GlobalClock(*this);
    
    for (int i = 0; i <= 23; i++)
        currentExclusivePad[i] = 100; //'100' here is used to signify an 'empty/NULL' value
    
}

AlphaLiveEngine::~AlphaLiveEngine()
{
    audioMixer.removeAllInputs();
	audioPlayer.setSource(NULL);

    if (midiOutputDevice)
    {
        midiOutputDevice->stopBackgroundThread();
        delete midiOutputDevice;
    }
    
    audioDeviceManager.removeAudioCallback (this);//unregister the audio callback
    
    delete modeMidi;
    delete modeLooper;
    delete modeSequencer;
    delete modeController;
    
    delete globalClock;


}

ModeMidi* AlphaLiveEngine::getModeMidi()
{
    return modeMidi;
}

ModeLooper* AlphaLiveEngine::getModeLooper()
{
    return modeLooper;
}

ModeSequencer* AlphaLiveEngine::getModeSequencer()
{
    return modeSequencer;
}

ModeController* AlphaLiveEngine::getModeController()
{
    return modeController;
}

int AlphaLiveEngine::getGuiUpdateFlag()
{
    return guiUpdateFlag;
}

void AlphaLiveEngine::setGuiUpdateFlag (int value)
{
    guiUpdateFlag = value;
}

//emulate a pad press for the pad GUI
//DO I NEED THIS FUNCTION ANYMORE?
void AlphaLiveEngine::playPadFromMouse (int pad, int value)
{
    //this is the function that is called when a shift-click and/or
    //shift-drag happens on a pad GUI.
    
    //As currently the OSC messages don't match the pad numbers,
    //before calling oscCallBack to emulate an osc input message being received
    //the GUI pad numbers need to be mapped so that they match their
    //physical pads' relevent OSC message pad number.
    //This can be done by using the Layouts::padArrangementLayout 'in reverse'.
    //This won't be needed once the OSC numbers match the pad numbers
    
    for (int arrayIndex = 0; arrayIndex <= sizeof(Layouts::padArrangementLayout); arrayIndex++)
    {
        //cycle through Layouts::padArrangementLayout array looking for the index
        //which holds the matching pad number
        if (Layouts::padArrangementLayout[arrayIndex] == pad)
        {
            //set the pad number to the array number where it was foud
            pad = arrayIndex;
            break;
        }
    }
    
    //mimic an OSC message being recieved.
    inputData(pad, value);
}

void AlphaLiveEngine::inputData(int pad, int value)
{
    //At the point the 'value' value is expected to be between 0 and 511.
    //In the older version this was scaled in the serial input class.
    //This range is also scaled in the Pad.cpp class for emulating pad presses.
    
    //if the incoming data is too fast for asyncUpdater to handle and has caused a pending update,
    //and the stored pad value of the pending update is 0, force a GUI update.
    //This will occasionally be needed to reset a pad's GUI back to 'off' when mutiple pads
    //are simultaneously being pressed.
    if (isUpdatePending() == true && pendingUpdatePadValue == 0) 
    {
        const MessageManagerLock mmLock; //lock event thread so it is safe to make calls in the message thread
        guiUpdateFlag = 0;
        handleUpdateNowIfNeeded(); //force GUI to be updated now
    }

    recievedPad = pad;
    recievedValue = value;
    
    
    //determine pressure mapping/sensitivity
    if (PAD_SETTINGS->getPressureSensitivityMode() == 1)
        {
            //non-sensitive - exponential mapping of pressure
            recievedValue = exp((float)recievedValue/511)-1;
            recievedValue = recievedValue * (511.0/1.71828);
            if (recievedValue > 511)
                recievedValue = 511;
            if (recievedValue > 0 && recievedValue < 1) //value 1 = 0.6, which is rounded to 0
                recievedValue = 1;
        }
    else if (PAD_SETTINGS->getPressureSensitivityMode() == 3)
    {
        //sensitive - logarithmic mapping of pressure
        recievedValue = log(recievedValue+1);
        recievedValue = recievedValue * (511.0/6.23832);
        if (recievedValue > 511)
            recievedValue = 511;
    }
    
    //else, pressureSensitivityMode == 2 which is a linear mapping of pressure

    
    //==========================================================================
    //route message to midi mode
    if (PAD_SETTINGS->getMode() == 1) //if the pressed pad is set to Midi mode
    {
        modeMidi->convertToMidi(recievedPad, recievedValue);
    }
    //==========================================================================
    
    //route message to looper mode
    if (PAD_SETTINGS->getMode() == 2) //if the pressed pad is set to Looper mode
    {
        modeLooper->getOscData(recievedPad, recievedValue);
    }
    //==========================================================================
    
    //route message to sequencer mode
    if (PAD_SETTINGS->getMode() == 3) //if the pressed pad is set to Sequencer mode
    {
        modeSequencer->getOscData(recievedPad, recievedValue);
    }
    //==========================================================================
    
    //route message to controller mode
    if (PAD_SETTINGS->getMode() == 4) //if the pressed pad is set to Controller mode
    {
        modeController->getOscData(recievedPad, recievedValue);
    }
    //==========================================================================

    
    //update GUI asyncronously
    guiUpdateFlag = 0;
    triggerAsyncUpdate();
    
    //store the pad pressure value 
    if (isUpdatePending() == true)
        pendingUpdatePadValue = recievedValue;
    
    
    //=========================================================================
    //OSC OUTPUT MODE STUFF
    
    //scale the range back down to 0-127
    int recievedValue2 = recievedValue * (127.0/511.0);
    
    if (isDualOutputMode == true)
    {
        oscOutput.transmitThruMessage(recievedPad+1, recievedValue2, oscIpAddress, oscPortNumber);
    }
    
}


void AlphaLiveEngine::handleExclusiveMode (int padNum)
{

    //get exclusive group number
    int exclusiveGroup = AppSettings::Instance()->padSettings[padNum]->getExclusiveGroup();
    //get currently stored pad from the exclusive mode array
    int prevPad = currentExclusivePad[exclusiveGroup-1];
    
    //if a pad exists (not NULL) and not equal to the current pad
    if (prevPad != 100 && prevPad != padNum) 
    {
        //get mode of prevPad
        int prevPadMode = AppSettings::Instance()->padSettings[prevPad]->getMode();
        
        //kill the pad based on the mode
        switch (prevPadMode)
        {
            case 1:
                modeMidi->killPad(prevPad);
                break;
            case 2:
                modeLooper->killPad(prevPad);
                break;
            case 3:
                modeSequencer->killPad(prevPad);
                break;
            case 4:
                //do nothing
                break;
            default:
                //do nothing
                break;
        }
        
    }
    
    //add new pad to the exclusive group array, replacing the old one.
    currentExclusivePad[exclusiveGroup-1] = padNum;
        
}

void AlphaLiveEngine::updatePadPlayingStatus (int padNumber, int status)
{
    guiUpdateFlag = 1;
    padNumberForPlayingStatus = padNumber;
    playingStatus = status;
    
    const MessageManagerLock mmLock; //lock event thread so it is safe to make calls in the message thread
    notifyObs();
    
}

void AlphaLiveEngine::triggerQuantizationPoint()
{
    modeLooper->triggerQuantizationPoint();
    modeSequencer->triggerQuantizationPoint();
    modeMidi->triggerQuantizationPoint();
}


void AlphaLiveEngine::handleAsyncUpdate()
{
    //notify the observers
    notifyObs();
}


void AlphaLiveEngine::killAll()
{
    for (int i = 0; i <= 47; i++)
    {
        modeMidi->killPad(i);
        modeLooper->killPad(i);
        modeSequencer->killPad(i);
    }
    
    if (globalClock->isThreadRunning() == true)
        globalClock->stopClock(); //currently all mode's are being killed again here
}


int AlphaLiveEngine::getPadNumberForPlayingStatus()
{
    return padNumberForPlayingStatus;
}

int AlphaLiveEngine::getPlayingStatus()
{
    return playingStatus;
}

int AlphaLiveEngine::getRecievedPad()
{
    return recievedPad;
    
}
int AlphaLiveEngine::getRecievedValue()
{
    return recievedValue;
}


void AlphaLiveEngine::setGain (float value)
{
    sharedMemory.enter();
    
    if (value <= 1.0)
        gain = value * value * value;
    else
        gain = value;
    
    sharedMemory.exit();
}

void AlphaLiveEngine::setPan (float value)
{
    sharedMemory.enter();
    panLeft = PanControl::leftChanPan_(value);
    panRight = PanControl::rightChanPan_(value);
    sharedMemory.exit();
}

void AlphaLiveEngine::setIsDualOutputMode (bool value)
{
    isDualOutputMode = value;
}

void AlphaLiveEngine::setOscIpAddress (String value)
{
    oscIpAddress = value;
}

void AlphaLiveEngine::setOscPortNumber (int value)
{
    oscPortNumber = value;
}



//==============================================================================
//Audio Callbacks
//==============================================================================
void AlphaLiveEngine::audioDeviceIOCallback (const float** inputChannelData,
                                          int totalNumInputChannels,
                                          float** outputChannelData,
                                          int totalNumOutputChannels,
                                          int numSamples)
{
    //pass the audio callback on to audioPlayer, which its source set as audioMixer, which have the Looper and Sequencer modes as input sources
	audioPlayer.audioDeviceIOCallback (inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
    
    AudioSampleBuffer buffer (outputChannelData, totalNumOutputChannels, numSamples);
    AudioSourceChannelInfo info;
    info.buffer = &buffer;
    info.startSample = 0;
    info.numSamples = numSamples;
    
    /*
    //OLD METHOD FOR GAIN AND PAN
	//streams audio into the outputChannelData buffer so this is both our input and output buffer.
	float *inOutL, *inOutR; 
	inOutL = outputChannelData[0];
	inOutR = outputChannelData[1];
    
    sharedMemory.enter();
    
	while(numSamples--)
	{
        //gain
        *inOutL = *inOutL * gain;
        *inOutR = *inOutR * gain;
        
        //pan
        *inOutL = panControl.leftChanPan(*inOutL);
        *inOutR = panControl.rightChanPan(*inOutR);
        
        //output samples
		inOutL++;
		inOutR++;
	}
    
	sharedMemory.exit();
     */
    
    //gain and pan
    sharedMemory.enter();
    
    for (int i = 0; i < info.buffer->getNumChannels(); ++i)
    {
        if (i == 0) //left chan
            info.buffer->applyGainRamp (i,
                                        info.startSample,
                                        info.numSamples,
                                        panLeftPrev * gainPrev,
                                        panLeft * gain);
        else if (i == 1) // right chan
            info.buffer->applyGainRamp (i,
                                        info.startSample,
                                        info.numSamples,
                                        panRightPrev * gainPrev,
                                        panRight * gain);
    }
    
    gainPrev = gain;
    panLeftPrev = panLeft;
    panRightPrev = panRight;
    sharedMemory.exit();
    
}

void AlphaLiveEngine::audioDeviceAboutToStart (AudioIODevice* device)
{
    audioPlayer.audioDeviceAboutToStart (device);
}

void AlphaLiveEngine::audioDeviceStopped()
{
	audioPlayer.audioDeviceStopped();
}

void AlphaLiveEngine::setMidiOutputDevice (int deviceIndex)
{
    if (deviceIndex >= 0)
    {
        std::cout << "Setting MIDI output device! " << std::endl;
        
        if (midiOutputDevice) //close open port
        {
            midiOutputDevice->stopBackgroundThread();
            delete midiOutputDevice;
        }
        
        midiOutputDevice = MidiOutput::openDevice(deviceIndex);
        
        if(midiOutputDevice)
            midiOutputDevice->startBackgroundThread();
        else
            std::cout << "Failed to open output device: " << MidiOutput::getDefaultDeviceIndex() << std::endl;
    }
    
    else if (deviceIndex == -1) //'none' selected
    {
        if (midiOutputDevice) //close open port
        {
            midiOutputDevice->stopBackgroundThread();
            delete midiOutputDevice;
        }
        
        midiOutputDevice = NULL;
    }
    
    //set the MidiOutput objects in modeMidi and ModeSequencer
    modeMidi->setMidiOutputDevice(*midiOutputDevice);
    modeSequencer->setMidiOutputDevice(*midiOutputDevice);
    modeController->setMidiOutputDevice(*midiOutputDevice);
    
    
}

MidiOutput* AlphaLiveEngine::getMidiOutputDevice()
{
    return midiOutputDevice;
}



AudioDeviceManager& AlphaLiveEngine::getAudioDeviceManager()
{
    return audioDeviceManager;
}

//global clock stuff

GlobalClock* AlphaLiveEngine::getGlobalClock()
{
    return globalClock;
}


