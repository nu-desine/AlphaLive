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
#include "../Application/Common.h"

#if JUCE_LINUX
#include <unistd.h>
#endif


#define PAD_SETTINGS AppSettings::Instance()->padSettings[recievedPad]
#define MAX_VELOCITY 127.0

AlphaLiveEngine::AlphaLiveEngine()
{
	#if JUCE_WINDOWS || JUCE_LINUX
	//start HID thread.
	//For OS X, see bottom of this function.
	startThread();

	//sleep to give the HID thread time to attempt to connect to the device.
	//I may be able to reduce the sleep time to 500ms.
    #ifdef WIN32
    sleep(1000);
    #else
    usleep(1000*1000);
    #endif

	#endif

    mainComponent = NULL;
    shouldUpdateFirmware = false;
    
    gain = gainPrev =  AppSettings::Instance()->getGlobalGain();
    panLeft = panLeftPrev = PanControl::leftChanPan_(AppSettings::Instance()->getGlobalPan());
    panRight = panRightPrev = PanControl::rightChanPan_(AppSettings::Instance()->getGlobalPan());
    
    midiClockValue = AppSettings::Instance()->getMidiClockValue();
    midiClockMessageFilter = AppSettings::Instance()->getMidiClockMessageFilter();
    receiveMidiProgramChanngeMessages = AppSettings::Instance()->getReceiveMidiProgramChangeMessages();
    
    recievedPad = 0;
    recievedValue = 0;
    recievedVelocity = 110;
    
    for (int i = 0; i < 48; i++)
    {
        padVelocity[i] = 0;
        minPressureValue[i] = 0;
        waitingToSetMinPressureValue[i] = 0;
        
        padVelocity[i] = -1;
        padPressure[i] = -1;
    }

    playingStatus = 0;
    
    for (int i = 0; i < 16; i++)
    {
        isMidiChannelActive[i] = false;
        previouslyUsedMidiChannels.add(i);
        
        midiChannelPressureHolder[i] = new MidiChannelPressureHolder;
    }
    
    resetMidiChannelPressureHolderData();
    
    //==========================================================================
    // initialise the device manager
    auto audioSettingsXml = XmlDocument::parse(StoredSettings::getInstance()->audioSettings);
    
	const String error (audioDeviceManager.initialise (0, /* number of input channels */
													   2, /* number of output channels */
                                                       audioSettingsXml.release(), /* XML settings from prefs*/
													   true  /* select default device on failure */));
	if (error.isNotEmpty())
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 "AlphaLive",
									 translate("Couldn't open an output device!") + "\n\n" + error);
	}
    
    audioDeviceManager.addAudioCallback (this);
    //delete audioSettingsXml;
    
    
    //SET UP MIDI OUTPUT AND INPUT if not currently connected to the HID device
    if (getDeviceStatus() == 0)
    {
        #if JUCE_MAC || JUCE_LINUX
        //==========================================================================
        //Create a virtual MIDI output device
        midiOutputDevice = MidiOutput::createNewDevice("AlphaLive");
        
        if(midiOutputDevice)
            midiOutputDevice->startBackgroundThread();
        else
            std::cout << "Failed to create a virtual MIDI output device!" << std::endl;
        
        //Create a virtual MIDI input device
        midiInputDevice = MidiInput::createNewDevice("AlphaLive", this);
        
        if (midiInputDevice)
            midiInputDevice->start();
        else
            std::cout << "Failed to create a virtual MIDI input device!" << std::endl;
        
        //==========================================================================
        #endif //JUCE_MAC || JUCE_LINUX
        
        #if JUCE_WINDOWS
        //==========================================================================
        //The MidiInput and MidiOutput objects used on Windows are created 
		//and set using the device selector component, so the one's in this
		//class aren't used here.
		midiOutputDevice = NULL;
        midiInputDevice = NULL;

		audioDeviceManager.addMidiInputCallback (String(), this);
        
        #endif //JUCE_WINDOWS
    }
    else
    {
        midiOutputDevice = NULL;
        midiInputDevice = NULL;
    }

    modeMidi = new ModeMidi (*this);
    modeSampler = new ModeSampler (*this);
    modeSequencer = new ModeSequencer (*this);
    modeController = new ModeController (*this);
    eliteControls = new EliteControls (*this);
    
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
    
    hasDisplayedNoMidiDeviceWarning = false;
    
    broadcaster.addActionListener(this);
    
	#if JUCE_MAC
    //Start HID thread here, and then remove the MIDI out stuff if the device exists.
    //For some reason if started any earlier in this function or within the HidComms constructor
    //it causes weird lagging issues with the MIDI stuff on OS X 10.8 if the AlphaSphere is connected
    //at launch. However this is just a quick fix - it is convoluted to create the MIDI out above
    //and then probably removing it here, so this issue needs to be resolved properly asap!!
    startThread();
    
    //sleep to give the HID thread time to attempt to connect to the device.
	//I may be able to reduce the sleep time to 500ms.
    sleep(1000);
    
    if (getDeviceStatus() != 0)
        removeMidiInAndOut();
	#endif
}

AlphaLiveEngine::~AlphaLiveEngine()
{
    killAll();
    stopThread(1000);
    
    //save audio output settings to prefs
    auto audioSettingsXml = audioDeviceManager.createStateXml();
    if (audioSettingsXml != nullptr)
    {
        String audioSettingsString = audioSettingsXml->createDocument(String(), true, false);
        StoredSettings::getInstance()->audioSettings = audioSettingsString;
        StoredSettings::getInstance()->flush();
    }
    //delete audioSettingsXml;

    audioMixer.removeAllInputs();
	audioPlayer.setSource(NULL);

    if (midiOutputDevice)
    {
        midiOutputDevice->stopBackgroundThread();
       //delete midiOutputDevice;
    }
    
    if (midiInputDevice)
    {
        midiInputDevice->stop();
        //delete midiInputDevice;
    }
    
    audioDeviceManager.removeAudioCallback (this);//unregister the audio callback
    
    for (int i = 0; i < 16; i++)
        delete midiChannelPressureHolder[i];
    
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
        
        //At the point the 'value' value is expected to be between minPressureValue[pad] and MAX_PRESSURE.
        //In the older version this was scaled in the serial input class.
        //This range is also scaled in the Pad.cpp class for emulating pad presses.
        
        recievedPad = pad;
        recievedValue = value;
        recievedVelocity = velocity;
        
        //std::cout << "received value: " << recievedValue << std::endl;
        
        //===============================================================================================
        //===============================================================================================
        
        if (waitingToSetMinPressureValue[recievedPad] == 0)
        {
            /*
             Only continue into the program if the app isn't currently waiting to set
             the current pads min pressure value (signified to do so in latchPressureValue()).
             If this check wasn't here, and it set the minPressureValue value directly in
             latchPressureValue() as before, as soon as latchPressureValue() is called it
             would jump the pressure value up (or down if unlatching) to the new scaled value.
             The else if statements below set it so that you need to release the pad and press it
             again for the new minPressureValue to be set, or press the pad to the current latched
             value to be unlatched. This results in a much more smoother interaction.
             */
            
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
                recievedValue = log(recievedValue + 1);
                recievedValue = recievedValue * (MAX_PRESSURE/6.23832);
                if (recievedValue > MAX_PRESSURE)
                    recievedValue = MAX_PRESSURE;
            }
            
            //else PAD_SETTINGS->getPressureCurve() = 2 = //linear mapping of pressure
            
            if (minPressureValue[pad] > 0)
                recievedValue = scaleValue (recievedValue, 0, MAX_PRESSURE, minPressureValue[recievedPad], MAX_PRESSURE);
            
            
            //===============================================================================================
            //===============================================================================================
            
            if (recievedVelocity != padVelocity[recievedPad])
            {
                padVelocity[recievedPad] = recievedVelocity;
                
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
                    
                    int minValue = PAD_SETTINGS->getVelocityMinRange();
                    int maxValue = PAD_SETTINGS->getVelocityMaxRange();
                    recievedVelocity = scaleValue (recievedVelocity, 0, 127.0, minValue, maxValue);
                }
                else if (PAD_SETTINGS->getVelocityCurve() == 3)
                {
                    //logarithmic mapping of velocity
                    recievedVelocity = log(recievedVelocity+1);
                    recievedVelocity = recievedVelocity * (MAX_VELOCITY/4.85); // not sure why 4.85 here!
                    if (recievedVelocity > MAX_VELOCITY)
                        recievedVelocity = MAX_VELOCITY;
                    
                    int minValue = PAD_SETTINGS->getVelocityMinRange();
                    int maxValue = PAD_SETTINGS->getVelocityMaxRange();
                    recievedVelocity = scaleValue (recievedVelocity, 0, 127.0, minValue, maxValue);
                }
                else if (PAD_SETTINGS->getVelocityCurve() == 2)
                {
                    //linear mapping of velocity
                    int minValue = PAD_SETTINGS->getVelocityMinRange();
                    int maxValue = PAD_SETTINGS->getVelocityMaxRange();
                    recievedVelocity = scaleValue (recievedVelocity, 0, 127.0, minValue, maxValue);
                }
                
                //static velocity stuff is done in the mode classes,
                //as each mode handles a static velocity in slight different ways
                //so doesn't make sense to apply any static values here
            }
            
            //===============================================================================================
            //===============================================================================================
            
            
            if (padPressure[recievedPad] != recievedValue)
            {
                //std::cout << "Pressure value of pad " << recievedPad << ": " << recievedValue << std::endl;
                
                //route message to midi mode
                if (PAD_SETTINGS->getMode() == 1) //if the pressed pad is set to Midi mode
                {
                    modeMidi->getInputData(recievedPad, recievedValue, recievedVelocity);
                }
                
                //route message to sampler mode
                else if (PAD_SETTINGS->getMode() == 2) //if the pressed pad is set to Sampler mode
                {
                    modeSampler->getInputData(recievedPad, recievedValue, recievedVelocity);
                }
                
                //route message to sequencer mode
                else if (PAD_SETTINGS->getMode() == 3) //if the pressed pad is set to Sequencer mode
                {
                    modeSequencer->getInputData(recievedPad, recievedValue);
                }
                
                //route message to controller mode
                else if (PAD_SETTINGS->getMode() == 4) //if the pressed pad is set to Controller mode
                {
                    modeController->getInputData(recievedPad, recievedValue, recievedVelocity);
                }
                
                //===============================================================================================
                
                sharedMemoryGui.enter();
                padPressure[recievedPad] = recievedValue;
                padPressureGuiQueue.add(recievedPad);
                broadcaster.sendActionMessage("UPDATE PRESSURE GUI");
                sharedMemoryGui.exit();
                
                //            //===============================================================================================
                //            //OSC OUTPUT MODE STUFF
                //
                //            if (isDualOutputMode == true)
                //            {
                //                oscOutput.transmitPadMessage(recievedPad+1, recievedValue, recievedVelocity, oscIpAddress, oscPortNumber);
                //            }
            }
        }
        else if (waitingToSetMinPressureValue[recievedPad] == 1 && recievedValue <= minPressureValue[recievedPad])
        {
            /*
             This statement will be true when a pad has been flagged to be latched from latchPressureValue()
             below and the pad has been released. This statement will then set the minPressureValue to
             the pressure value of that when the pad was flagged to be latched, and then allow the pressure value
             to continue into the program as per usual but using the new minPressureValue.
             */
            
            minPressureValue[recievedPad] = padPressure[recievedPad];
            waitingToSetMinPressureValue[recievedPad] = 0;
            
        }
        else if (waitingToSetMinPressureValue[recievedPad] == 2 && recievedValue >= minPressureValue[recievedPad])
        {
            /*
             This statement will be true when a pad has been flagged to be unlatched from latchPressureValue()
             below and the pad has been pressed to its current minPressureValue value. 
             This statement will then set the minPressureValue to 0, and then allow the pressure value
             to continue into the program as per usual but using the default minPressureValue value.
             */
            
            minPressureValue[recievedPad] = 0;
            waitingToSetMinPressureValue[recievedPad] = 0;
            
            //display that the pressure is unlatched
            sharedMemoryGui2.enter();
            padPressureStatusQueue.add(recievedPad);
            broadcaster.sendActionMessage("UPDATE PRESSURE STATUS");
            sharedMemoryGui2.exit();
        }
        
    }
    
    //===============================================================================================
    //===============================================================================================
    
    else 
    {
        //an elite control has been touched. Do your thang!
        eliteControls->getInputData(pad, value);
        
        sharedMemoryGui.enter();
        eliteControlValue[pad-100] = value;
        eliteControlGuiQueue.add (pad-100);
        broadcaster.sendActionMessage("UPDATE ELITE GUI");
        sharedMemoryGui.exit();
    }
    
    //===============================================================================================
    //===============================================================================================
}

void AlphaLiveEngine::processMidiInput (const MidiMessage midiMessage)
{
    //==== MIDI Clock stuff ====
    if ((midiMessage.isMidiStart() || midiMessage.isMidiContinue()) && midiClockValue == 3)
    {
        globalClock->startClock();
    }
    else if (midiMessage.isMidiStop() && midiClockValue == 3)
    {
        globalClock->stopClock();
    }
    else if (midiMessage.isMidiClock() && midiClockValue == 3)
    {
        if (midiClockMessageFilter == 1)
        {
            if (globalClock->isThreadRunning())
                globalClock->setMidiClockMessageTimestamp();
        }
    }
    
    //==== MIDI Program Change stuff (to change scenes) ====
    
    else if (midiMessage.isProgramChange() && receiveMidiProgramChanngeMessages == true)
    {
        int programNumber = midiMessage.getProgramChangeNumber();
        
        if (programNumber >= 0 && programNumber < NO_OF_SCENES)
        {
            mainComponent->getSceneComponent()->selectSlot(programNumber);
        }
    }
    
    //==== MIDI LED Control Mode Status message ====
    else if (midiMessage.isControllerOfType(20) && midiMessage.getChannel() == 16)
    {
        if (midiMessage.getControllerValue() > 0 && AppSettings::Instance()->getHardwareLedMode() == 0)
        {
            AppSettings::Instance()->setHardwareLedMode(1);
            
            //update the menu bar items status
            commandManager->commandStatusChanged();
        }
        else if (midiMessage.getControllerValue() == 0 && AppSettings::Instance()->getHardwareLedMode() == 1)
        {
            AppSettings::Instance()->setHardwareLedMode(0);
            
            //update the menu bar items status if this was called not from the menu bar
            commandManager->commandStatusChanged();
        }
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
    
    //check to see if there are any pads within the same exclive group
    //that are currently waiting to play or stop, and kill them if so.
    killQueuedExclusivePads(padNum);
        
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
    //are set to exclusive mode with the same exclusive group.
    //If so they should be removed and the pad gui should be updated
    killQueuedExclusivePads(padNum);
    
}

void AlphaLiveEngine::killQueuedExclusivePads (int padNum)
{
    //checks for pads that are set to exclusive mode with the same group number as pad padNum is set to,
    //and kills and removes any pads that are currently waiting to play or waiting to stop.
    
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
        modeController->killPad(i);
    }
    
    if (globalClock->isThreadRunning() == true)
        globalClock->stopClock(); //currently all mode's are being killed again here
    
    resetMidiChannelPressureHolderData();
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

void AlphaLiveEngine::setMidiClockValue (int value)
{
    midiClockValue = value;
}
void AlphaLiveEngine::setMidiClockMessageFilter (int value)
{
    midiClockMessageFilter = value;
}
void AlphaLiveEngine::setReceiveMidiProgramChangeMessages(bool value)
{
    receiveMidiProgramChanngeMessages = value;
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

void AlphaLiveEngine::handleIncomingMidiMessage(MidiInput* midiInput, const MidiMessage& midiMessage)
{
    processMidiInput(midiMessage);
}

void AlphaLiveEngine::setDeviceType (int type)
{
    StoredSettings::getInstance()->deviceType = type;
    StoredSettings::getInstance()->flush();
    
    if (mainComponent != NULL)
    {
        //const MessageManagerLock mmLock;
        //mainComponent->editInterfaceFromDeviceConnectivity(2);
        broadcaster.sendActionMessage("SET DEVICE TYPE");
    }
}

void AlphaLiveEngine::setDeviceStatus()
{
    if (mainComponent != NULL)
    {
        //const MessageManagerLock mmLock;
        //mainComponent->editInterfaceFromDeviceConnectivity(3);
        broadcaster.sendActionMessage("SET DEVICE STATUS");
    }
    
    if (getDeviceStatus() != 0)
        hasDisplayedNoMidiDeviceWarning = false;
}


//Output the MIDI messages
void AlphaLiveEngine::sendMidiMessage(MidiMessage midiMessage)
{
    sharedMemoryMidi.enter();
    
    if (getDeviceStatus() != 0)
    {
		//===============================================================
		//Sending MIDI over HID

        unsigned char dataToSend[4];
        
        const uint8 *rawMidiMessage = midiMessage.getRawData();
        
        dataToSend[0] = 0x00; //MIDI command ID
        dataToSend[1] = rawMidiMessage[0]; //midi status byte
        dataToSend[2] = rawMidiMessage[1]; //midi data byte 1
        dataToSend[3] = rawMidiMessage[2]; //midi data byte 2
        
        addMessageToHidOutReport (dataToSend);
    }
    else
    {
		//===============================================================
		//Sending MIDI using MidiOutput object

		#if JUCE_MAC || JUCE_LINUX
        if(midiOutputDevice)
		{
            midiOutputDevice->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
		}
		else
		{
			if (!hasDisplayedNoMidiDeviceWarning)
			{
				String instructionString = translate("AlphaLive cannot currently send any MIDI messages as the AlphaSphere has been disconnected. To start sending MIDI messages again please reconnect the AlphaSphere, or if you would like to use AlphaLive's virtual MIDI port, quit and relaunch AlphaLive without the AlphaSphere connected.");
				AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
								                              translate("No MIDI device available!"),
									                          translate(instructionString));
			}

			hasDisplayedNoMidiDeviceWarning = true;
		}

		#elif JUCE_WINDOWS

		//If midi output exists (it won't if the user hasn't chosen an output device...)
        if (audioDeviceManager.getDefaultMidiOutput())
        {
            audioDeviceManager.getDefaultMidiOutput()->startBackgroundThread();
            audioDeviceManager.getDefaultMidiOutput()->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
        }
        else
        {
			if (!hasDisplayedNoMidiDeviceWarning)
			{
				String instructionString = translate("AlphaLive cannot currently send any MIDI messages as the AlphaSphere has been disconnected or no MIDI output port has be selected. To start sending MIDI messages please reconnect the AlphaSphere, or select an external MIDI output device from the Preferences view (if the option to select a MIDI output port is not available here, quit and relaunch AlphaLive without the AlphaSphere connected).");
				AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
								                              translate("No MIDI device available!"),
									                          translate(instructionString));
			}

			hasDisplayedNoMidiDeviceWarning = true;
		}

		#endif

    }
    
    //==================================================================================
    //log data about the MIDI channels that is then used by the Dynamic MIDI Channel Mode
    if (midiMessage.isNoteOn())
    {
        //put this channel at the end of the array
        previouslyUsedMidiChannels.removeAllInstancesOf(midiMessage.getChannel()-1);
        previouslyUsedMidiChannels.add(midiMessage.getChannel()-1);
        
        //flag that the channel is active
        isMidiChannelActive[midiMessage.getChannel()-1] = true;
    }
    else if (midiMessage.isNoteOff())
    {
        //flag that the channel is free to use
        isMidiChannelActive[midiMessage.getChannel()-1] = false;
    }

    sharedMemoryMidi.exit();
}


void AlphaLiveEngine::removeMidiInAndOut()
{
    sharedMemoryMidi.enter();
    
	#if JUCE_MAC || JUCE_LINUX
    //if currently connected to midiOutputDevice and midiInputDevice dissconnected and delete them
    if (midiOutputDevice)
    {
        midiOutputDevice->stopBackgroundThread();
        //delete midiOutputDevice;
        midiOutputDevice = NULL;
    }
    
    if (midiInputDevice)
    {
        midiInputDevice->stop();
        //delete midiInputDevice;
        midiInputDevice = NULL;
    }
    
    #elif JUCE_WINDOWS

	if (audioDeviceManager.getDefaultMidiOutput())
	{
		audioDeviceManager.getDefaultMidiOutput()->stopBackgroundThread();
		audioDeviceManager.setDefaultMidiOutput(String());
	}

	audioDeviceManager.removeMidiInputCallback(String(), this);
	//what about dissabling MIDI input object?

    //remove MIDI output and input selectors from the preferences view
    if (mainComponent != NULL)
    {
        //const MessageManagerLock mmLock;
        //mainComponent->editInterfaceFromDeviceConnectivity(1);
        broadcaster.sendActionMessage("RM MIDI IO");
    }
    
    #endif //JUCE_WINDOWS
    
    sharedMemoryMidi.exit();
    
}

void AlphaLiveEngine::updateFirmware()
{
    broadcaster.sendActionMessage("UPDATE FIRMWARE");
}

void AlphaLiveEngine::uploadFirmware (bool applyingUpdate)
{
    /*
     Firmware uploader feature.
     Below is the code needed to apply a firmware upload to the AlphaSphere.
     It creates a ChildProcess object that runs the bootloader command line app, passing in the
     mmcu type and hex file destination as parameters. It then returns the output as a String.
     The bootloader app and latest firmware hex file should exist in the Application Data directory.
     
     This function is called from two situations:
     1. When a firmware update is being applied, using the hex file from the Application Data directory
     2. When the user is uploading a custom firmware file, chosen using a file browser pop up.
     The function knows which situation it is currently in using the applyingUpdate argument.
     
     Firmware UPDATING should be applied using the following method:
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
     
     */
    
    
    ChildProcess bootloader;
    
    StringArray arguments;
    String appDir(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() + File::getSeparatorString());
    
    //Get the hex file...
    
    File firmwareFile = File();
    bool shouldContinue = true;
    
    if (applyingUpdate)
    {
        File appDataDir(appDir + "Application Data");
        String wildcard = "SphereWare*";
        
        //There could be multiple instances of SphereWare* files,
        //so store them all in an Array and select the last one,
        //which should be the one with the highest number which
        //is the firmware file we want.
        Array<File> hexFile;
        appDataDir.findChildFiles(hexFile, 2, false, wildcard);
        firmwareFile = hexFile.getLast();
    }
    else
    {
        FileChooser loadFileChooser(translate("Select an AlphaSphere firmware (.hex) file to open..."),
                                    File(),
                                    "*.hex");
        
        shouldContinue = loadFileChooser.browseForFileToOpen(); //open file browser
        firmwareFile = loadFileChooser.getResult();
        
    }
    
    //Get the bootloader app
    #if JUCE_MAC || JUCE_LINUX //is this right for Linux?
    File bootloaderFile(appDir + "Application Data" + File::getSeparatorString() + "firmwareUpdater");
    #endif
    #if JUCE_WINDOWS
    File bootloaderFile(appDir + "Application Data" + File::getSeparatorString() + "firmwareUpdater.exe");
    #endif
    
    String mmcuString("-mmcu=atmega32u4");
    
    if (bootloaderFile.exists() == true && firmwareFile.exists() == true)
    {
        arguments.add(bootloaderFile.getFullPathName());
        arguments.add(mmcuString);
        
        #if JUCE_MAC || JUCE_LINUX //is this right for Linux?
        arguments.add(firmwareFile.getFullPathName());
        #endif
        #if JUCE_WINDOWS
        arguments.add(firmwareFile.getFullPathName().quoted());  //Needs to be quoted else updaterFirmware.exe
        //thinks the whole filepath is just from the last space.
        #endif
        
        int tryToUpload = true;
        
        while (tryToUpload)
        {
            String instructionsString;
            
            //sleep to make it more clear that a new alert has popped up...
            #if JUCE_WINDOWS
            sleep(500); //should this actually be Sleep() which need a windows library defined?
            instructionsString = translate("To allow the AlphaSphere to be updated, please unplug the device, hold down the reset button (next to the USB port), and plug the device back in. If the AlphaSphere is now flashing red, release the reset button and the device will successfully enter the updater mode. If Windows is now installing new drivers for the device please wait for the drivers to be installed, and then press OK to continue.");
            #else
            sleep(1000);
            instructionsString = translate("To allow the AlphaSphere to be updated, please unplug the device, hold down the reset button (next to the USB port), and plug the device back in. If the AlphaSphere is now flashing red, release the reset button and the device will successfully enter the updater mode. If Press OK to continue.");
            #endif
            
            bool shouldContinue = AlertWindow::showOkCancelBox (AlertWindow::NoIcon,
                                                                translate("PLEASE FOLLOW THESE INSTRUCTIONS CAREFULLY"),
                                                                translate(instructionsString));
            
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
                    sleep(500);
                    #endif
                    
                    tryToUpload = AlertWindow::showOkCancelBox(AlertWindow::InfoIcon,
                                                               translate("Cannot Find Device!"),
                                                               translate("The AlphaSphere does not appear to be connected to the computer or be set to the updater mode. Would you like to try again?"));
                }
                else if (bootloaderReport.isEmpty())
                {
                    AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, translate("Firmware Updated!"), translate("The AlphaSphere firmware has been successfully updated."));
                    
                    tryToUpload = false;
                }
                else
                {
                    //sleep to make it more clear that a new alert has popped up...
                    #if JUCE_WINDOWS
                    sleep(500); //should this actually be Sleep() which need a windows library defined?
                    #else
                    sleep(500);
                    #endif
                    
                    //catch any other outputs (errors most likely)
                    AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, translate("Error!"), bootloaderReport);
                    
                    tryToUpload = false;
                }
            }
            else
                tryToUpload = false;
        }
        
    }
    else
    {
        if (shouldContinue)
            AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, translate("Missing Files!"), translate("One or more of the files needed to update the firmware are missing. Please consult the FAQ of the reference manual."));
    }

}

void AlphaLiveEngine::setFirmwareDetails (String version, String serial)
{
    firmwareVersion = version;
    deviceSerial = serial;
    
    if (mainComponent != NULL)
    {
        //const MessageManagerLock mmLock;
        
        broadcaster.sendActionMessage("SET FW DETAILS");
       // mainComponent->editInterfaceFromDeviceConnectivity(4);
    }
}

void AlphaLiveEngine::setLedSettings (uint8 setting, uint8 value)
{
    //This is the declaration of the abstract HidComms function.
    //It is used to set the general settings of the LED of the hardware.
    //The 'setting' argument can have the following values:
    // 1 - LED on/off status - set 'value' to 0 or 1
    // 2 - LED pressure interaction status - set 'value' to 0 or 1
    // 3 - LED clock interaction -  set 'value' to 0 for 'off',
    //                              1 for 'fade to black', or
    //                              2 for 'fade from max colour to min colour' (not yet implemented).
    // 4 - LED mode - set 'value' to 0 for normal or 1 for MIDI CC controlled mode
    
    if (getDeviceStatus() != 0)
    {
        unsigned char dataToSend[4];
        dataToSend[0] = 0x01; //General LED settings command
        dataToSend[1] = setting;
        dataToSend[2] = value;
        dataToSend[3] = 0x00;
        addMessageToHidOutReport(dataToSend);
    }
}

void AlphaLiveEngine::setLedColour (uint8 colourNumber, Colour colour)
{
    if (getDeviceStatus() != 0)
    {
        unsigned char dataToSend[4];
        dataToSend[0] = colourNumber + 2; //LED colour command
        dataToSend[1] = colour.getRed();
        dataToSend[2] = colour.getGreen();
        dataToSend[3] = colour.getBlue();
        addMessageToHidOutReport(dataToSend);
    }
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
                mainComponent->getGuiPadLayout()->setPadPressure(padNum, padPressure[padNum], minPressureValue[padNum]);
        }
        
        padPressureGuiQueue.clear();
        
        sharedMemoryGui.exit();
    }
    
    else if (message == "UPDATE PRESSURE STATUS")
    {
        sharedMemoryGui2.enter();
        
        for (int i = 0; i < padPressureStatusQueue.size(); i++)
        {
            int padNum = padPressureStatusQueue[i];
            bool pressureIsLatched;
            
            if (waitingToSetMinPressureValue[padNum] != 0)
                pressureIsLatched = true;
            else
                pressureIsLatched = false;
            
            if (mainComponent != NULL)
                mainComponent->getGuiPadLayout()->setPadPressureStatus(padNum, pressureIsLatched);
        }
        
        padPressureStatusQueue.clear();
        
        sharedMemoryGui2.exit();
    }
    
    else if (message == "UPDATE ELITE GUI")
    {
        
        sharedMemoryGui.enter();
        
        for (int i = 0; i < eliteControlGuiQueue.size(); i++)
        {
            int eliteControlNum = eliteControlGuiQueue[i];
            
            if (mainComponent != NULL)
                mainComponent->getEliteControlsComponent()->updateDisplay(eliteControlNum, 
                                                                          eliteControlValue[eliteControlNum]);
        }
        
        eliteControlGuiQueue.clear();
        
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
                uploadFirmware();
            }
            
        }
    }
    
    else if (message == "SET DEVICE TYPE")
    {
        mainComponent->editInterfaceFromDeviceConnectivity(2);
    }
    
    else if (message == "SET DEVICE STATUS")
    {
        mainComponent->editInterfaceFromDeviceConnectivity(3);
    }
    
    else if (message == "SET FW DETAILS")
    {
        mainComponent->editInterfaceFromDeviceConnectivity(4);
    }
    
    else if (message == "RM MIDI IO")
    {
        mainComponent->editInterfaceFromDeviceConnectivity(1);
    }
}
void AlphaLiveEngine::setFirmwareUpdateStatus (bool status)
{
    shouldUpdateFirmware = status;
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

String AlphaLiveEngine::getFirmwareVersion()
{
    return firmwareVersion;
}

String AlphaLiveEngine::getDeviceSerial()
{
    return deviceSerial;
}

void AlphaLiveEngine::setMainComponent(MainComponent *mainComponent_)
{
    mainComponent = mainComponent_;
    eliteControls->setMainComponent(mainComponent_);
    globalClock->setMainComponent(mainComponent_);
    modeController->setMainComponent(mainComponent_);
}

bool AlphaLiveEngine::getMidiChannelStatus (int channel)
{
    return isMidiChannelActive[channel];
}

Array<int> AlphaLiveEngine::getPreviouslyUsedMidiChannels()
{
    return previouslyUsedMidiChannels;
}

void AlphaLiveEngine::latchPressureValue (int padNum, bool shouldLatch, bool setPressureInstantaneously)
{
    //setPressureInstantaneously will only be true here when this function was called
    //from ModeController::killPad() so that the connected latched pad can be truely reset.
    //Otherwise, waitingToSetMinPressureValue is set to flag that the minPressureValue
    //needs to be set within hidInputCallback() above when the incoming pressure value
    //is correct.
    
    if (setPressureInstantaneously == false)
    {
        
        if (padPressure[padNum] > 0)
        {
            if (shouldLatch)
            {
                waitingToSetMinPressureValue[padNum] = 1;
                
                //display that the pressure is latched
                sharedMemoryGui2.enter();
                padPressureStatusQueue.add(padNum);
                broadcaster.sendActionMessage("UPDATE PRESSURE STATUS");
                sharedMemoryGui2.exit();
            }
            else
            {
                waitingToSetMinPressureValue[padNum] = 2;
            }
        }
    }
    else if (setPressureInstantaneously == true)
    {
        
        if (shouldLatch)
            minPressureValue[padNum] = padPressure[padNum];
        else
            minPressureValue[padNum] = 0;
        
        waitingToSetMinPressureValue[padNum] = 0;
        
        //update latched pad pressure GUI
        sharedMemoryGui.enter();
        padPressure[padNum] = 0;
        padPressureGuiQueue.add(padNum);
        broadcaster.sendActionMessage("UPDATE PRESSURE GUI");
        sharedMemoryGui.exit();
        
        //display that the pressure is unlatched
        sharedMemoryGui2.enter();
        padPressureStatusQueue.add(padNum);
        broadcaster.sendActionMessage("UPDATE PRESSURE STATUS");
        sharedMemoryGui2.exit();
        
    }
    
    
//    if (shouldLatch)
//        minPressureValue[padNum] = padPressure[padNum];
//    else
//        minPressureValue[padNum] = 0;
}


void AlphaLiveEngine::changeGuiPadText (int padNum)
{
    //its really hacky and bed design having this here,
    //but oh well.
    
    mainComponent->getGuiPadLayout()->setPadDisplay(padNum);
}

MidiChannelPressureHolder* AlphaLiveEngine::getMidiChannelPressureHolderPtr (int chan)
{
    return midiChannelPressureHolder[chan];
}

void AlphaLiveEngine::resetMidiChannelPressureHolderData()
{
    for (int i = 0; i < 16; i++)
    {
        midiChannelPressureHolder[i]->aftertouch = -1;
        midiChannelPressureHolder[i]->pitchBend = -1;
        for (int j = 0; j < 128; j++)
            midiChannelPressureHolder[i]->controlChange[j] = -1;
    }
}
