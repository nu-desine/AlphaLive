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

EliteControls::EliteControls(MidiOutput &midiOutput, AlphaLiveEngine &ref)
                            :   midiOutputDevice(&midiOutput),
                                alphaLiveEngineRef(ref)
{
    
}

EliteControls::~EliteControls()
{
    
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
        int controlType = AppSettings::Instance()->getEliteDialControl(eliteDialNumber);
        
        //global gain
        if (controlType == 1)
        {
            
        }
        
        //global pan
        else if (controlType == 2)
        {
            
        }
        
        //scene switcher
        else if (controlType == 3)
        {
            
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
                
                //if produced CC value is in range, send the midi message
                if (ccValue >= minValue && ccValue <= maxValue)
                {
                    message = MidiMessage::controllerEvent(channel, controllerNumber, ccValue);
                    sendMidiMessage(message);
                }
                //else, dont send the midi message and set the CC value to the min or max value
                else
                {
                    if (ccValue > maxValue)
                        ccValue = maxValue;
                    else if (ccValue < minValue)
                        ccValue = minValue;
                }
                
            }
            //relative controller type
            else if (controllerType == 1)
            {
                //min and max value sliders not needed within the gui here. Changed that!
                ccValue = eliteControlValue;
                
                message = MidiMessage::controllerEvent(channel, controllerNumber, ccValue);
                sendMidiMessage(message);
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
            double oscValue = 0;
            
            //incremented value
            if (eliteControlValue >= 1 && eliteControlValue <= 63)
            {
                //incremation value should be prevValue + (oscStepValue * eliteControlValue- maxValue [range of minValue to maxValue])
                //oscValue = prevValue + eliteControlValue;
            }
            //decremented value
            else if (eliteControlValue >= 64 && eliteControlValue <= 127)
            {
                oscValue = prevValue - (128 - eliteControlValue);
            }
            
            //if produced OSC value is in range, send the OSC message
            if (oscValue >= minValue && oscValue <= maxValue)
            {
                oscOutput.transmitOutputMessage(control+1, oscValue, ipAddress, portNumber);
            }
            //else, dont send the OSC message and set the OSC value to the min or max value
            else
            {
                if (oscValue > maxValue)
                    oscValue = maxValue;
                else if (oscValue < minValue)
                    oscValue = minValue;
            }
        }
            
    }
    
    
    
    
    
    
    //buttons
    else if (control >= 102 && control <= 104)
    {
        eliteButtonNumber = control - 102;
        int controlType = AppSettings::Instance()->getEliteButtonControl(eliteButtonNumber);
        
        //start/stop clock
        if (controlType == 1)
        {
            if (eliteControlValue == 1)
            {
                alphaLiveEngineRef.sendEliteControlCommand(2);
            }
        }
        
        //scene switcher
        else if (controlType == 2)
        {
            //how should i do this? similar to how modeController does it?
        }
        
        //save
        else if (controlType == 3)
        {
            if (eliteControlValue == 1)
            {
                alphaLiveEngineRef.sendEliteControlCommand(3);
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
            sendMidiMessage(message);
            
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
            
            
            oscOutput.transmitOutputMessage(control+1, oscValue, ipAddress, portNumber);
        }
    }
    
}

void EliteControls::sendMidiMessage(MidiMessage midiMessage)
{
    if (alphaLiveEngineRef.hasOpenedHidDevice() == true)
    {
        unsigned char dataToSend[5];
        
        uint8 *rawMidiMessage = midiMessage.getRawData();
        
        //std::cout << "raw midi message " << std::endl;
        //printf("%02hhx ", rawMidiMessage[0]);
        //printf("%02hhx ", rawMidiMessage[1]);
        //printf("%02hhx ", rawMidiMessage[2]);
        //printf("\n");
        
        dataToSend[0] = 0x00;   //if no reportID's are defined in the descriptor,
                                //must send 0x00. First byte MUST be the report ID.
        dataToSend[1] = MIDI_OUT_COMMAND_ID;
        dataToSend[2] = rawMidiMessage[0]; //midi status byte
        dataToSend[3] = rawMidiMessage[1]; //midi data byte 1
        dataToSend[4] = rawMidiMessage[2]; //midi data byte 2
        
        alphaLiveEngineRef.sendHidControlReport(dataToSend);
    }
    else
    {
        if(midiOutputDevice)
            midiOutputDevice->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
        else
            std::cout << "No MIDI output selected\n";
    }
}


void EliteControls::setMidiOutputDevice (MidiOutput &midiOutput)
{
    midiOutputDevice = &midiOutput;
}