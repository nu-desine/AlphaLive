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
#define MAX_VELOCITY 127.0

AlphaLiveEngine::AlphaLiveEngine()
{
	#if JUCE_WINDOWS || JUCE_LINUX
	//start HID thread.
	//For OS X, see bottom of this function.
	startThread();
	#endif

    mainComponent = NULL;
    shouldUpdateFirmware = false;
    
    gain = gainPrev =  AppSettings::Instance()->getGlobalGain();
    panLeft = panLeftPrev = PanControl::leftChanPan_(AppSettings::Instance()->getGlobalPan());
    panRight = panRightPrev = PanControl::rightChanPan_(AppSettings::Instance()->getGlobalPan());
    
    recievedPad = 0;
    recievedValue = 0;
    recievedVelocity = 110;

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
    if (getDeviceStatus() == 0)
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
    
    broadcaster.addActionListener(this);
    
	#if JUCE_MAC
    //Start HID thread here, and then remove the MIDI out stuff if the device exists.
    //For some reason if started any earlier in this function or within the HidComms constructor
    //it causes weird lagging issues with the MIDI stuff on OS X 10.8 if the AlphaSphere is connected
    //at launch. However this is just a quick fix - it is convoluted to create the MIDI out above
    //and then probably removing it here, so this issue needs to be resolved properly asap!!
    startThread();
    
    //sleep to give the HID thread time to attempt to connect to the device.
    #ifdef WIN32
    sleep(100); //should this actually be Sleep() which need a windows library defined? See hidtest.
    #else
    usleep(100*1000);
    #endif
    
    if (getDeviceStatus() != 0)
        removeMidiOut();
	#endif
}

AlphaLiveEngine::~AlphaLiveEngine()
{
    killAll();
    stopThread(100);
    
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
        //pad = Layouts::padArrangementLayout[pad];
        
        //At the point the 'value' value is expected to be between 0 and MAX_PRESSURE.
        //In the older version this was scaled in the serial input class.
        //This range is also scaled in the Pad.cpp class for emulating pad presses.
        
        recievedPad = pad;
        recievedValue = value;
        
        //The firmware only sends a velocity value on the initial press, and then a 0 after that.
        //This will cause issues when converting to OSC messages.
        //Therefore, never set recievedVelocity to 0.
        if (velocity > 0)
        {
            recievedVelocity = velocity;
        }
        
        //===determine pressure curve===
        if (PAD_SETTINGS->getPressureCurve() == 1)
        {
            //exponential mapping of pressure
            recievedValue = exp((float)recievedValue/MAX_PRESSURE)-1;
            recievedValue = recievedValue * (MAX_PRESSURE/1.71828);
            if (recievedValue > MAX_PRESSURE)
                recievedValue = MAX_PRESSURE;
            if (recievedValue > 0 && recievedValue < 1) //value 1 = 0.6, which is rounded to 0
                recievedValue = 1;
        }
        else if (PAD_SETTINGS->getPressureCurve() == 3)
        {
            //logarithmic mapping of pressure
            recievedValue = log(recievedValue+1);
            recievedValue = recievedValue * (MAX_PRESSURE/6.23832);
            if (recievedValue > MAX_PRESSURE)
                recievedValue = MAX_PRESSURE;
        }
        //else, pressureCurve == 2 which is a linear mapping of pressure
        
        
        //===determine velocity curve===
        if (PAD_SETTINGS->getVelocityCurve() == 1)
        {
            //exponential mapping of velocity
            recievedVelocity = exp((float)recievedVelocity/MAX_VELOCITY)-1;
            recievedVelocity = recievedVelocity * (MAX_VELOCITY/1.71828);
            if (recievedVelocity > MAX_VELOCITY)
                recievedVelocity = MAX_VELOCITY;
            if (recievedVelocity > 0 && recievedVelocity < 1) //value 1 = 0.6, which is rounded to 0
                recievedVelocity = 1;
        }
        else if (PAD_SETTINGS->getVelocityCurve() == 3)
        {
            //logarithmic mapping of velocity
            recievedVelocity = log(recievedVelocity+1);
            recievedVelocity = recievedVelocity * (MAX_VELOCITY/4.85); // not sure why 4.85 here!
            if (recievedVelocity > MAX_VELOCITY)
                recievedVelocity = MAX_VELOCITY;
        }
        //else, velocityCurve == 2 which is a linear mapping of velocity,
        //or 4 which is a static velocity.
        
        //std::cout << "Pad: " << recievedPad << " Raw Vel: " << velocity << " Scaled Vel: " << recievedVelocity << std::endl;
        
        //==========================================================================
        //route message to midi mode
        if (PAD_SETTINGS->getMode() == 1) //if the pressed pad is set to Midi mode
        {
            modeMidi->getInputData(recievedPad, recievedValue, recievedVelocity);
        }
        //==========================================================================
        
        //route message to sampler mode
        else if (PAD_SETTINGS->getMode() == 2) //if the pressed pad is set to Sampler mode
        {
            modeSampler->getInputData(recievedPad, recievedValue, recievedVelocity);
        }
        //==========================================================================
        
        //route message to sequencer mode
        else if (PAD_SETTINGS->getMode() == 3) //if the pressed pad is set to Sequencer mode
        {
            modeSequencer->getInputData(recievedPad, recievedValue);
        }
        //==========================================================================
        
        //route message to controller mode
        else if (PAD_SETTINGS->getMode() == 4) //if the pressed pad is set to Controller mode
        {
            modeController->getInputData(recievedPad, recievedValue, recievedVelocity);
        }
        //==========================================================================
        
        sharedMemoryGui.enter();
        padPressure[recievedPad] = recievedValue;
        padPressureGuiQueue.add(recievedPad);
        broadcaster.sendActionMessage("UPDATE PRESSURE GUI");
        sharedMemoryGui.exit();
        
//        //=========================================================================
//        //OSC OUTPUT MODE STUFF
//        
//        if (isDualOutputMode == true)
//        {
//            oscOutput.transmitPadMessage(recievedPad+1, recievedValue, recievedVelocity, oscIpAddress, oscPortNumber);
//        }
    }
    else 
    {
        //an elite control has been touched. Do your thang!
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
    //is this function called from an external actionListenerCallback function every time?
    //If not do we need a MessageManagerLock here?
    if (mainComponent != NULL)
        mainComponent->getGuiPadLayout()->setPadPlayingState(padNumber, status);
    
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
                        
                        //here we only want to 'kill' a queued pad if it isn't currently playing
                        switch (padMode)
                        {
                            case 1:
                                if (modeMidi->isCurrentlyPlaying(queuedPads[i]) == false)
                                {
                                    modeMidi->killPad(queuedPads[i]);
                                }
                                break;
                            case 2:
                                if (modeSampler->getAudioFilePlayerInstance(queuedPads[i])->isCurrentlyPlaying() == false)
                                {
                                    modeSampler->killPad(queuedPads[i]);
                                }
                                break;
                            case 3:
                                if (modeSequencer->getSequencePlayerInstance(queuedPads[i])->isCurrentlyPlaying() == false)
                                {
                                    modeSequencer->killPad(queuedPads[i]);
                                }
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

void AlphaLiveEngine::setDeviceType (int type)
{
    StoredSettings::getInstance()->deviceType = type;
    StoredSettings::getInstance()->flush();
    
    if (mainComponent != NULL)
    {
        const MessageManagerLock mmLock;
        mainComponent->editInterfaceFromDeviceConnectivity(2);
    }
}

void AlphaLiveEngine::setDeviceStatus()
{
    if (mainComponent != NULL)
    {
        const MessageManagerLock mmLock;
        mainComponent->editInterfaceFromDeviceConnectivity(3);
    }
}


void AlphaLiveEngine::removeMidiOut()
{
    std::cout << "removing midi output stuff" << std::endl;
    
    //if currently connected to a midiOutputDevice (either the virtual port on mac/linux
    //or a hardware port on Windows) delete/dissconnected it.
    if (midiOutputDevice)
    {
        midiOutputDevice->stopBackgroundThread();
        delete midiOutputDevice;
        
        midiOutputDevice = NULL;
    }
    
    #if JUCE_WINDOWS
    //remove MIDI output selector from the preferences view
    if (mainComponent != NULL)
    {
        const MessageManagerLock mmLock;
        mainComponent->editInterfaceFromDeviceConnectivity(1);
    }
    
    #endif //JUCE_WINDOWS
    
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
    
    //set the MidiOutput objects in the child classes
    modeMidi->setMidiOutputDevice(*midiOutputDevice);
    modeSequencer->setMidiOutputDevice(*midiOutputDevice);
    modeController->setMidiOutputDevice(*midiOutputDevice);
    eliteControls->setMidiOutputDevice(*midiOutputDevice);
    
    
}

void AlphaLiveEngine::updateFirmware()
{
    broadcaster.sendActionMessage("UPDATE FIRMWARE");
}


void AlphaLiveEngine::actionListenerCallback (const String& message)
{
    if (message == "UPDATE PRESSURE GUI")
    {
        
        sharedMemoryGui.enter();
        
        for (int i = 0; i < padPressureGuiQueue.size(); i++)
        {
            int padNum = padPressureGuiQueue[i];
            if (mainComponent != NULL)
                mainComponent->getGuiPadLayout()->setPadPressure(padNum, padPressure[padNum]);
        }
        
        padPressureGuiQueue.clear();
        
        sharedMemoryGui.exit();
    }
    
    else if (message == "UPDATE FIRMWARE")
    {
        if (shouldUpdateFirmware == true)
        {
            bool userSelection = AlertWindow::showOkCancelBox(AlertWindow::InfoIcon, 
                                                              translate("Firmware update available!"), 
                                                              translate("The AlphaSphere firmware needs updating. It is strongly recommended that you apply this update as there may be features in the latest AlphaLive version that requires the latest firmware to work properly. Press OK to apply this update."));
            
            if (userSelection == true)
            {
                /*
                 Firmware updater feature.
                 Below is the code needed to apply a firmware update to the AlphaSphere.
                 It creates a ChildProcess object that runs the bootloader command line app, passing in the
                 mmcu type and hex file destination as parameters. It then returns the output as a String.
                 The bootloader app and hex file should exist in the Application Data directory.
                 
                  Firmware updating should be applied using the following method:
                 - The hex file should be numbered to signify the firware version (e.g. SphereWare_1_0.hex, SphereWare_1_1.hex).
                 - When the sphere is connected to AlphaLive it sends a report stating its current firmware version.
                 - If the included hex file has a greater number than the current firmware version, it pops up
                 an alert window telling the user that there is a new firmware version available and asks if they want to update
                 it (stressing that they should as otherwise it could limit the softwares functionality - Maybe the user shouldn't
                 have an option to not update it?). 
                 - The user is then told how to enter the bootloader mode
                 - The device will then automatically be reconnected and the user can rock out the new firmware. Boo ya. 
                 
                 Things to consider:
                 - Error handling and feedback - make sure all possible errors can be caught and displayed.
                 - Where in the AlphaLive code should this be placed when the above method is implemented? 
                 - What about if we introduce different firmware versions for different types of foam? We would
                 then need an option somewhere for the user to change the firmware, probably within Preferences. 
                 
                 */
                
                ChildProcess bootloader;
                
                StringArray arguments;
                String appDir(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() + File::separatorString);
                
                #if JUCE_MAC || JUCE_LINUX //is this right for Linux?
                File bootloaderFile(appDir + "Application Data" + File::separatorString + "firmwareUpdater");
                #endif
                #if JUCE_WINDOWS
                File bootloaderFile(appDir + "Application Data" + File::separatorString + "firmwareUpdater.exe");
                #endif
                
                String mmcuString("-mmcu=atmega32u4");
                
                //Get the hexFile. 
                File appDataDir(appDir + "Application Data");
                String wildcard = "SphereWare*";
                Array<File> hexFile;
                appDataDir.findChildFiles(hexFile, 2, false, wildcard);
                
                if (bootloaderFile.exists() == true && hexFile.getLast().exists() == true)
                {
                    arguments.add(bootloaderFile.getFullPathName());
                    arguments.add(mmcuString);
                    
                    #if JUCE_MAC || JUCE_LINUX //is this right for Linux?
                    arguments.add(hexFile.getLast().getFullPathName());
                    #endif
                    #if JUCE_WINDOWS
                    arguments.add(hexFile.getLast().getFullPathName().quoted());  //Needs to be quoted else updaterFirmware.exe
                    //thinks the whole filepath is just from the last space.
                    #endif
                    
                    int tryToUpdate = true;
                    
                    while (tryToUpdate)
                    {
                        //sleep to make it more clear that a new alert has popped up...
                        #if JUCE_WINDOWS
                        sleep(500); //should this actually be Sleep() which need a windows library defined?
                        #else
                        usleep(500*1000);
                        #endif
                        
                        bool shouldContinue = AlertWindow::showOkCancelBox (AlertWindow::NoIcon, 
                                                      translate("Enter Updater Mode"), 
                                                      translate("To allow the AlphaSphere to be updated, please unplug the device, hold down the reset button (next to the USB port), and plug the device back in. If the AlphaSphere is now flashing red, release the reset button and the device will successfully enter the updater mode. Press OK to continue."));
                        
                        if (shouldContinue)
                        {
                            bootloader.start(arguments);
                            
                            String bootloaderReport =  bootloader.readAllProcessOutput();
                            
                            std::cout << "..." << bootloaderReport << "..." << std::endl;
                            
                            if (bootloaderReport.contains("Unable to open device"))
                            {
                                //sleep to make it more clear that a new alert has popped up...
                                #if JUCE_WINDOWS
                                sleep(500); //should this actually be Sleep() which need a windows library defined?
                                #else
                                usleep(500*1000);
                                #endif
                                
                                tryToUpdate = AlertWindow::showOkCancelBox(AlertWindow::InfoIcon, 
                                                                           translate("Cannot Find Device!"), 
                                                                           translate("The AlphaSphere does not appear to be connected to the computer or be set to the updater mode. Would you like to try again?"));
                            }
                            else if (bootloaderReport.isEmpty())
                            {
                                AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, translate("Firmware Updated!"), translate("The AlphaSphere firmware has been successfully updated."));
                                
                                tryToUpdate = false;
                            }
                            else
                            {
                                //sleep to make it more clear that a new alert has popped up...
                                #if JUCE_WINDOWS
                                sleep(500); //should this actually be Sleep() which need a windows library defined?
                                #else
                                usleep(500*1000);
                                #endif
                                
                                //catch any other outputs (errors most likely)
                                AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, translate("Error!"), bootloaderReport);
                                
                                tryToUpdate = false;
                            }
                        }
                        else
                            tryToUpdate = false;
                    }
                    
                }
                else
                {
                    AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, translate("Missing Files!"), translate("One or more of the files needed to update the firmware are missing. Please consult the FAQ of the reference manual."));
                }
            }
        }
    }    
    
        
}
void AlphaLiveEngine::setFirmwareUpdateStatus (bool status)
{
    shouldUpdateFirmware = status;
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
    globalClock->setMainComponent(mainComponent_);
}
