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
#include "../File and Settings/StoredSettings.h"
#include "../GUI Classes/Views/MainComponent.h"


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
    // initialise the device manager
    XmlElement *audioSettingsXml = XmlDocument::parse(StoredSettings::getInstance()->audioSettings);
    
	const String error (audioDeviceManager.initialise (0, /* number of input channels */
													   2, /* number of output channels */
													   audioSettingsXml, /* XML settings from prefs*/
													   true  /* select default device on failure */));
	if (error.isNotEmpty())
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 "AlphaLive",
									 translate("Couldn't open an output device!") + "\n\n" + error);
	}
    
    audioDeviceManager.addAudioCallback (this);
    delete audioSettingsXml;
    
    
    //SET UP MIDI OUTPUT if not connected to the HID device
    if (hasOpenedHidDevice() == false)
    {
        #if JUCE_MAC || JUCE_LINUX
        //==========================================================================
        //Create new virtual MIDI device
        midiOutputDevice = MidiOutput::createNewDevice("AlphaLive");
        
        if(midiOutputDevice)
            midiOutputDevice->startBackgroundThread();
        else
            std::cout << "Failed to create a virtual MIDI device!" << std::endl;
        
        //==========================================================================
        #endif //JUCE_MAC || JUCE_LINUX
        
        #if JUCE_WINDOWS
        //==========================================================================
        //connect to a MIDI device
        midiOutputDevice = NULL;
        #endif //JUCE_WINDOWS
    }
    else
        midiOutputDevice = NULL;

    modeMidi = new ModeMidi(*midiOutputDevice, *this);
    modeSampler = new ModeSampler(*this);
    modeSequencer = new ModeSequencer(*midiOutputDevice, *this);
    modeController = new ModeController(*midiOutputDevice, *this);
    eliteControls = new EliteControls(*midiOutputDevice, *this);
    
    //global clock stuff
    globalClock = new GlobalClock(*this);
    
    audioMixer.addInputSource(modeSampler, false); //add as inputsource to audioMixer
    audioMixer.addInputSource(modeSequencer, false); //add as inputsource to audioMixer
    audioMixer.addInputSource(globalClock, false); //add as inputsource to audioMixer
    audioPlayer.setSource(&audioMixer);
    
    isDualOutputMode = false;
    oscIpAddress = "127.0.0.1";
    oscPortNumber = 5004;
    
    for (int i = 0; i <= 23; i++)
        currentExclusivePad[i] = 100; //'100' here is used to signify an 'empty/NULL' value
    
}

AlphaLiveEngine::~AlphaLiveEngine()
{
    //save audio output settings to prefs
    XmlElement *audioSettingsXml = audioDeviceManager.createStateXml();
    if (audioSettingsXml != nullptr)
    {
        String audioSettingsString = audioSettingsXml->createDocument(String::empty, true, false);
        StoredSettings::getInstance()->audioSettings = audioSettingsString;
        StoredSettings::getInstance()->flush();
    }
    delete audioSettingsXml;

    audioMixer.removeAllInputs();
	audioPlayer.setSource(NULL);

    if (midiOutputDevice)
    {
        midiOutputDevice->stopBackgroundThread();
        delete midiOutputDevice;
    }
    
    audioDeviceManager.removeAudioCallback (this);//unregister the audio callback
    
    delete modeMidi;
    delete modeSampler;
    delete modeSequencer;
    delete modeController;
    delete eliteControls;
    
    delete globalClock;


}

ModeMidi* AlphaLiveEngine::getModeMidi()
{
    return modeMidi;
}

ModeSampler* AlphaLiveEngine::getModeSampler()
{
    return modeSampler;
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
void AlphaLiveEngine::playPadFromMouse (int pad, int value)
{
    //as current we have a temp pad number reversal layout that is
    //processed at the beginnning of input data, when emulating a pad
    //press from the pad layout GUI we must first reverse the pad numbers
    //so that it will be correct within hidInputCallback
    
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
    
    //mimic input data being recieved from the hardware.
    hidInputCallback(pad, value, 110); //what should the velocity value be here?
}


void AlphaLiveEngine::hidInputCallback (int pad, int value, int velocity)
{
    if (pad < 48) //an actual pad as opposed to an elite control
    {
        //=====TEMPORARY PAD NUMBER REVERSAL======
        pad = Layouts::padArrangementLayout[pad];
        
        //At the point the 'value' value is expected to be between 0 and MAX_PRESSURE.
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
        if (PAD_SETTINGS->getPressureCurve() == 1)
        {
            //non-sensitive - exponential mapping of pressure
            recievedValue = exp((float)recievedValue/MAX_PRESSURE)-1;
            recievedValue = recievedValue * (MAX_PRESSURE/1.71828);
            if (recievedValue > MAX_PRESSURE)
                recievedValue = MAX_PRESSURE;
            if (recievedValue > 0 && recievedValue < 1) //value 1 = 0.6, which is rounded to 0
                recievedValue = 1;
        }
        else if (PAD_SETTINGS->getPressureCurve() == 3)
        {
            //sensitive - logarithmic mapping of pressure
            recievedValue = log(recievedValue+1);
            recievedValue = recievedValue * (MAX_PRESSURE/6.23832);
            if (recievedValue > MAX_PRESSURE)
                recievedValue = MAX_PRESSURE;
        }
        //else, pressureCurve == 2 which is a linear mapping of pressure
        
        
        //==========================================================================
        //route message to midi mode
        if (PAD_SETTINGS->getMode() == 1) //if the pressed pad is set to Midi mode
        {
            modeMidi->getInputData(recievedPad, recievedValue);
        }
        //==========================================================================
        
        //route message to sampler mode
        if (PAD_SETTINGS->getMode() == 2) //if the pressed pad is set to Sampler mode
        {
            modeSampler->getInputData(recievedPad, recievedValue);
        }
        //==========================================================================
        
        //route message to sequencer mode
        if (PAD_SETTINGS->getMode() == 3) //if the pressed pad is set to Sequencer mode
        {
            modeSequencer->getInputData(recievedPad, recievedValue);
        }
        //==========================================================================
        
        //route message to controller mode
        if (PAD_SETTINGS->getMode() == 4) //if the pressed pad is set to Controller mode
        {
            modeController->getInputData(recievedPad, recievedValue);
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
        int recievedValue2 = recievedValue * (127.0/MAX_PRESSURE);
        
        if (isDualOutputMode == true)
        {
            oscOutput.transmitOutputMessage(recievedPad+1, recievedValue2, oscIpAddress, oscPortNumber);
        }
    }
    else 
    {
        //an elite control has been pressed. Do your thang!
        eliteControls->getInputData(pad, value);
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
                modeMidi->stopPrevExclusivePad(prevPad);
                break;
            case 2:
                modeSampler->stopPrevExclusivePad(prevPad);
                break;
            case 3:
                modeSequencer->stopPrevExclusivePad(prevPad);
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

void AlphaLiveEngine::addPadToQueue (int padNum)
{
    queuedPads.addIfNotAlreadyThere(padNum);

    //If the pad being added is set to exclusive mode,
    //check to see if any of the other queued pads 
    //are set to exlusive mode with the same exclusive group.
    //If so they should be removed and the pad gui should be updated
    
    if (AppSettings::Instance()->padSettings[padNum]->getExclusiveMode() == 1)
    {
        int group = AppSettings::Instance()->padSettings[padNum]->getExclusiveGroup();
        
        for (int i = 0; i < queuedPads.size(); i++)
        {
            if (queuedPads[i] != padNum)
            {
                if (AppSettings::Instance()->padSettings[queuedPads[i]]->getExclusiveMode() == 1)
                {
                    if (AppSettings::Instance()->padSettings[queuedPads[i]]->getExclusiveGroup() == group)
                    {
                        int padMode = AppSettings::Instance()->padSettings[queuedPads[i]]->getMode();
                        
                        switch (padMode)
                        {
                            case 1:
                                modeMidi->killPad(queuedPads[i]);
                                break;
                            case 2:
                                modeSampler->killPad(queuedPads[i]);
                                break;
                            case 3:
                                modeSequencer->killPad(queuedPads[i]);
                                break;
                            default:
                                break;
                        }
                        
                        queuedPads.remove(i);
                        
                    }
                }
            }
            
        }
    }
}

void AlphaLiveEngine::triggerQuantizationPoint()
{
    
    if (queuedPads.size() > 0)
    {
        for (int i = 0; i < queuedPads.size(); i++)
        {
            int padMode = AppSettings::Instance()->padSettings[queuedPads[i]]->getMode();
            
            switch (padMode)
            {
                case 1:
                    modeMidi->triggerQuantizationPoint(queuedPads[i]);
                    break;
                case 2:
                    modeSampler->triggerQuantizationPoint(queuedPads[i]);
                    break;
                case 3:
                    modeSequencer->triggerQuantizationPoint(queuedPads[i]);
                    break;
                default:
                    break;
            }
        }
        
        //remove items from array so they no longer recieve alerts of
        //quantization points in time
        queuedPads.clear();
    }
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
        modeSampler->killPad(i);
        modeSequencer->killPad(i);
    }
    
    if (globalClock->isThreadRunning() == true)
        globalClock->stopClock(); //currently all mode's are being killed again here
}

void AlphaLiveEngine::setRecordingSequencerPadsState (int padNum, int state)
{
    if (state == 1)
    {
        recordingPads.addIfNotAlreadyThere(padNum);
    }
    else if (state == 0)
    {
        recordingPads.removeFirstMatchingValue(padNum);
    }
}

Array<int> AlphaLiveEngine::getRecordingPads()
{
    return recordingPads;
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
    //pass the audio callback on to audioPlayer, which its source set as audioMixer, which have the Sampler and Sequencer modes as input sources
	audioPlayer.audioDeviceIOCallback (inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
    
    AudioSampleBuffer buffer (outputChannelData, totalNumOutputChannels, numSamples);
    AudioSourceChannelInfo info;
    info.buffer = &buffer;
    info.startSample = 0;
    info.numSamples = numSamples;
    
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
    eliteControls->setMidiOutputDevice(*midiOutputDevice);
    
    
}

MidiOutput* AlphaLiveEngine::getMidiOutputDevice()
{
    return midiOutputDevice;
}



AudioDeviceManager& AlphaLiveEngine::getAudioDeviceManager()
{
    return audioDeviceManager;
}

GlobalClock* AlphaLiveEngine::getGlobalClock()
{
    return globalClock;
}

MainComponent* AlphaLiveEngine::getMainComponent()
{
    return mainComponent;
}

void AlphaLiveEngine::setMainComponent(MainComponent *mainComponent_)
{
    mainComponent = mainComponent_;
    eliteControls->setMainComponent(mainComponent_);
}
