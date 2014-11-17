//
//  EliteControls.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 19/11/2012.
//  Copyright 2012 nu desine. All rights reserved.
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

#include "EliteControls.h"
#include "../File and Settings/AppSettings.h"
#include "../Application/CommandIDs.h"
#include "../GUI Classes/Views/MainComponent.h"

EliteControls::EliteControls(AlphaLiveEngine &ref)
                            : alphaLiveEngineRef(ref)
{
    
}

EliteControls::~EliteControls()
{
    
}

void EliteControls::setMainComponent(MainComponent *mainComponent_)
{
    mainComponent = mainComponent_;
}

void EliteControls::getInputData(int control, int value)
{
    //here, control values of 100-101 are for the dials,
    //and control values of 102-104 are for the buttons.
    //However, within AppSettings the dial data is in an array of 0-1
    //and the button data is in an array of 0-2.
    
    eliteControlValue = value;
    
    //dials
    if (control == 100 || control == 101)
    {
        eliteDialNumber = control - 100;
        
        if (AppSettings::Instance()->getEliteDialStatus(eliteDialNumber) == 1)
        {
            int controlType = AppSettings::Instance()->getEliteDialControl(eliteDialNumber);
            
            //global gain
            if (controlType == 1)
            {
                mainComponent->sendEliteDialCommand (1, eliteControlValue);
            }
            
            //global pan
            else if (controlType == 2)
            {
                mainComponent->sendEliteDialCommand (2, eliteControlValue);
            }
            
            //tempo
            else if (controlType == 6)
            {
                mainComponent->sendEliteDialCommand (3, eliteControlValue);
            }
            
            //smart dial
            else if (controlType == 7)
            {
                mainComponent->sendEliteDialCommand (4, eliteControlValue);
            }
            
            //scene switcher
            else if (controlType == 3)
            {
                //move to next scene slot
                if (eliteControlValue >= 1 && eliteControlValue <= 63)
                    mainComponent->getSceneComponent()->selectSlot(-1);
                //move to previous scene slot
                else if (eliteControlValue >= 64 && eliteControlValue <= 127)
                    mainComponent->getSceneComponent()->selectSlot(-2);
            }
            
            //MIDI CC
            else if (controlType == 4)
            {
                MidiMessage message;
                int controllerType = AppSettings::Instance()->getEliteDialMidiControllerType(eliteDialNumber);
                int channel = AppSettings::Instance()->getEliteDialMidiChannel(eliteDialNumber);
                int controllerNumber = AppSettings::Instance()->getEliteDialMidiCcNumber(eliteDialNumber);
                int ccValue = 0;
                
                //absolute controller type
                if (controllerType == 0)
                {
                    int prevValue = AppSettings::Instance()->getEliteDialPrevValue(eliteDialNumber);
                    int minValue = AppSettings::Instance()->getEliteDialMidiMinRange(eliteDialNumber);
                    int maxValue = AppSettings::Instance()->getEliteDialMidiMaxRange(eliteDialNumber);
                    
                    //incremented value
                    if (eliteControlValue >= 1 && eliteControlValue <= 63)
                        ccValue = prevValue + eliteControlValue;
                    //decremented value
                    else if (eliteControlValue >= 64 && eliteControlValue <= 127)
                        ccValue = prevValue - (128 - eliteControlValue);
                    
                    //if produced CC value is out of range, set in range
                    if (!(ccValue >= minValue && ccValue <= maxValue))
                    {
                        if (ccValue > maxValue)
                            ccValue = maxValue;
                        else if (ccValue < minValue)
                            ccValue = minValue;
                    }
                    
                    //if new CC value is different from the previous CC value, send the MIDI message
                    if (ccValue != prevValue)
                    {
                        message = MidiMessage::controllerEvent(channel, controllerNumber, ccValue);
                        alphaLiveEngineRef.sendMidiMessage(message);
                    }
                    
                    //update prevValue
                    AppSettings::Instance()->setEliteDialPrevValue(ccValue, eliteDialNumber);
                    
                }
                //relative controller type
                else if (controllerType == 1)
                {
                    //min and max value sliders not needed within the gui here. Changed that!
                    ccValue = eliteControlValue;
                    
                    message = MidiMessage::controllerEvent(channel, controllerNumber, ccValue);
                    alphaLiveEngineRef.sendMidiMessage(message);
                }
                
            }
            
            //OSC
            else if (controlType == 5)
            {
                //Similar to the absolute controller with the MIDI mode above, however there
                //is a user-defined incremation/decremation value that is also based on the speed of dial turn.
                
                String ipAddress = AppSettings::Instance()->getEliteDialOscIpAddress(eliteDialNumber);
                int portNumber = AppSettings::Instance()->getEliteDialOscPortNumber(eliteDialNumber);
                double minValue = AppSettings::Instance()->getEliteDialOscMinRange(eliteDialNumber);
                double maxValue = AppSettings::Instance()->getEliteDialOscMaxRange(eliteDialNumber);
                double prevValue = AppSettings::Instance()->getEliteDialPrevValue(eliteDialNumber);
                double oscStepValue = AppSettings::Instance()->getEliteDialOscStepValue(eliteDialNumber);
                double oscValue = 0;
                
                //incremented value
                if (eliteControlValue >= 1 && eliteControlValue <= 63)
                {
                    double incremValue;
                    
                    if (eliteControlValue == 1)
                    {
                        incremValue = oscStepValue;
                    }
                    else
                    {
                        //is this right? What if the step value is quite big?
                        incremValue = oscStepValue * (eliteControlValue * (((maxValue-minValue)/2) / (63 - 1)));
                    }
                    
                    oscValue = prevValue + incremValue;
                    
                }
                //decremented value
                else if (eliteControlValue >= 64 && eliteControlValue <= 127)
                {
                    double decremValue;
                    
                    if (eliteControlValue == 127)
                    {
                        decremValue = oscStepValue;
                    }
                    else
                    {
                        //is this right? What if the step value is quite big?
                        decremValue = oscStepValue * ((128-eliteControlValue) * (((maxValue-minValue)/2) / (63 - 1)));
                    }
                    
                    oscValue = prevValue - decremValue;
                }
                
                //if produced OSC value is out of range, set in range
                if (!(oscValue >= minValue && oscValue <= maxValue))
                {
                    if (oscValue > maxValue)
                        oscValue = maxValue;
                    else if (oscValue < minValue)
                        oscValue = minValue;
                }
                
                //if new OSC value is different from the previous OSC value, send the OSC message
                if (oscValue != prevValue)
                {
                    oscOutput.transmitEliteControlMessage(control+1, oscValue, ipAddress, portNumber);
                }
                
                //update prevValue
                AppSettings::Instance()->setEliteDialPrevValue(oscValue, eliteDialNumber);
            }
        }    
    }
    
    
    
    
    
    
    //buttons
    else if (control >= 102 && control <= 104)
    {
        eliteButtonNumber = control - 102;
        
        if (AppSettings::Instance()->getEliteButtonStatus(eliteButtonNumber) == 1)
        {
            int controlType = AppSettings::Instance()->getEliteButtonControl(eliteButtonNumber);
            
            //start/stop clock
            if (controlType == 1)
            {
                if (eliteControlValue == 1)
                {
                    //const MessageManagerLock mmLock;
                    mainComponent->perform(CommandIDs::StartStopClock);
                }
            }
            
            //scene switcher
            else if (controlType == 2)
            {
                if (eliteControlValue == 1)
                {
                    int sceneMode = AppSettings::Instance()->getEliteButtonSceneMode(eliteButtonNumber);
                    
                    if (sceneMode == 1) //next scene
                        mainComponent->getSceneComponent()->selectSlot(-1);
                    else if (sceneMode == 2) //previous scene
                        mainComponent->getSceneComponent()->selectSlot(-2);
                    else //a specific scene
                        mainComponent->getSceneComponent()->selectSlot(AppSettings::Instance()->getEliteButtonSceneNumber(eliteButtonNumber) - 1);
                }
            }
            
            //metronome
            else if (controlType == 7)
            {
                if (eliteControlValue == 1)
                {
                    //const MessageManagerLock mmLock;
                    mainComponent->getGuiGlobalClock()->getMetronomeButton()->triggerClick();
                }
            }
            
            //Sequencer record button
            else if (controlType == 8)
            {
                if (eliteControlValue == 1)
                {
                    //const MessageManagerLock mmLock;
                    mainComponent->getGuiSequencerMode()->getRecordButton()->triggerClick();
                }
            }
            
            //save
            else if (controlType == 3)
            {
                if (eliteControlValue == 1)
                {
                    //const MessageManagerLock mmLock;
                    mainComponent->getAppDocumentStateRef().saveProject();
                }
            }
            
            //MIDI CC
            else if (controlType == 4)
            {
                MidiMessage message;
                int channel = AppSettings::Instance()->getEliteButtonMidiChannel(eliteButtonNumber);
                int controllerNumber = AppSettings::Instance()->getEliteButtonMidiCcNumber(eliteButtonNumber);
                int ccValue = 0;
                
                if (eliteControlValue == 0)
                    ccValue = AppSettings::Instance()->getEliteButtonMidiOffNumber(eliteButtonNumber);
                else if (eliteControlValue == 1)
                    ccValue = AppSettings::Instance()->getEliteButtonMidiOnNumber(eliteButtonNumber);
                
                message = MidiMessage::controllerEvent(channel, controllerNumber, ccValue);
                alphaLiveEngineRef.sendMidiMessage(message);
                
            }
            
            //OSC
            else if (controlType == 5)
            {
                String ipAddress = AppSettings::Instance()->getEliteButtonOscIpAddress(eliteButtonNumber);
                int portNumber = AppSettings::Instance()->getEliteButtonOscPortNumber(eliteButtonNumber);
                double oscValue = 0;
                
                if (eliteControlValue == 0)
                    oscValue = AppSettings::Instance()->getEliteButtonOscOffNumber(eliteButtonNumber);
                else if (eliteControlValue == 1)
                    oscValue = AppSettings::Instance()->getEliteButtonOscOnNumber(eliteButtonNumber);
                
                
                oscOutput.transmitEliteControlMessage(control+1, oscValue, ipAddress, portNumber);
            }
            
            //Killswitch
            else if (controlType == 6)
            {
                if (eliteControlValue == 1)
                {
                    const MessageManagerLock mmLock;
                    mainComponent->perform(CommandIDs::KillSwitch);
                }
            }
            
            //=============== LED Controls ====================
            
            //LED Status
            else if (controlType == 9)
            {
                if (eliteControlValue == 1)
                    JUCEApplication::getInstance()->perform(CommandIDs::EnableLed);
            }
            //LED Pressure Status
            else if (controlType == 10)
            {
                if (eliteControlValue == 1)
                    JUCEApplication::getInstance()->perform(CommandIDs::EnableLedPressure);
            }
            //LED Clock Status
            else if (controlType == 11)
            {
                if (eliteControlValue == 1)
                    JUCEApplication::getInstance()->perform(CommandIDs::EnableLedClock);
            }
            //LED MIDI CC Control Status
            else if (controlType == 12)
            {
                if (eliteControlValue == 1)
                    JUCEApplication::getInstance()->perform(CommandIDs::EnableLedMidiMode);
            }
        }
    }
    
}
