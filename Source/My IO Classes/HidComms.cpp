//
//  HidComms.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 08/11/2012.
//  Copyright 2012 nu desine.
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

#include "HidComms.h"
#include "../File and Settings/StoredSettings.h"
#include "../File and Settings/AppSettings.h"
#if JUCE_LINUX
#include <unistd.h>
#endif

#define RECONNECT_TIME 500

HidComms::HidComms() : Thread("HidThread")
{
    memset(outBuf,0,sizeof(buf));
    
    appHasInitialised = false;
    sendOutputReport = false;
    midiInOutExists = hidDeviceStatus = 0;
	reconnectCounter = RECONNECT_TIME;
    
    for (int i = 0; i < 48; i++)
        prevPadPressure[i] = 0;
    for (int i = 0; i < 3; i++)
        prevButtonValue[i] = 0;
    for (int i = 0; i < 2; i++)
    {
        dialCounter[i] = 0;
        dialValue[i] = 0;
        dialCounterFlag[i] = 0;
    }
}

HidComms::~HidComms()
{
    //Do I need to call stopThread() here if
    //I'm calling it in the deconstructor of
    //AlphaLiveEngine?
    stopThread(1000);
    
    hid_close(handle);
    
    #if ! JUCE_MAC
    //For some reason the app crashes on OS X 10.8 when it tries to call hid_exit().
    //For now I have quick fixed this by not calling it on Mac.
    //This isn't causing any noticeable problems, though is probably causing memory leaks,
    //so I should probably sort this issue out properly at a later date.
    //Ideally I should move to the latest tip of HIDAPI, studying the hidtest.cpp file
    //again, and make sure this is working on all platforms of OS X.
    
    hid_exit();
    #endif
}

void HidComms::run()
{
    handle = nullptr;
    res = 0;
    
    while( ! threadShouldExit())
    {
        
        //=== if device is connected ===
        if (handle)
        {
            res = hid_read(handle, buf, sizeof(buf));
            
            //std::cout << res << std::endl;
            
            if (res == 0)
            {
                //printf("no report...\n");
            }
            if (res < 0)
            {
                //printf("Unable to read()\n");
                
                //if this statement is entered does it always means that
                //the device has been disconnected?
                handle = nullptr;
                hidDeviceStatus = 0;
            }
            
            if (res > 0)
            {
                //            for (int i = 0; i < res; i++)
                //                printf("%02hhx ", buf[i]);
                //            printf("\n");
                
                if (appHasInitialised == true)
                {
                    //NEW PROTOCOL READING
                    //refer to http://kaspar.h1x.com/nuwiki/index.php/HID_Protocol_0.2.1
                    //for details on the protocol and HID report format.
                    
                    if (buf[0] == 0x01)
                    {
                        //==== pad data ====
                        for (int i = 0; i < 48; i++)
                        {
                            int padIndex = (i * 2) + 1;
                            
                            int pressure = buf[padIndex + 1] << 1;
                            pressure += buf[padIndex] >> 7;
                            
                            if (pressure != prevPadPressure[i])
                            {
                                int velocity = buf[padIndex] & 127;
                            
                                //std::cout << i << " : " << pressure << " : " << velocity << std::endl;
                                
                                //There's a bug with pad 31/32 where the velocity value isn't always
                                //sent with the first pressure value, which causes pads to not 'trigger'
                                //within the rest of the application. Therefore, only forward a HID report
                                //if both the velocity and pressure value is greater than 0.
                                if (! (pressure > 0 && velocity == 0))
                                {
                                    hidInputCallback(i, pressure, velocity);
                                    prevPadPressure[i] = pressure;
                                }
                            }
                        }
                        
                        //The following algorithm must be set up so that
                        //it allows for the possibility of multiple buttton 
                        //presses/releases or multiple dial rotations to be 
                        //sent in a single message within the HID report.
                        
                        //hidInputCallback() expects the following values:
                        //button numbers = 102 to 104 (ignore reset button)
                        //dial numbers = 100 - 101
                        //button press = 1
                        //button release = 0
                        //dial left turn = 127-64
                        //dial right turn = 1-63
                        
                        //decode bits 1-3 from the button data byte (97),
                        //looking for button presses and releases.
                        for (int i = 1; i < 4; i++)
                        {
                            int buttonNum = i + 101;
                            //shift the value to the first bit
                            int buttonVal = buf[97] >> i;
                            //mask the first bit
                            buttonVal = buttonVal & 1;
                            
                            if (buttonVal != prevButtonValue[i-1])
                            {
                                //std::cout << buttonNum << " : " << buttonVal << std::endl;
                                hidInputCallback(buttonNum, buttonVal, 0);
                                prevButtonValue[i-1] = buttonVal;
                            }
                        }
                        
                        //decode bytes 98 and 99 for dial rotations
                        for (int i = 0; i < 2; i++)
                        {
                            int dialNum = i + 100;
                            int dialVal = buf[i + 98];
                            
                            /*
                             Below, hidInputCallback() calls aren't made straight away.
                             Instead, it counts the number of turns recieved within a certain
                             amount of time and then sends a larger value. This allows for
                             faster turns by the user to send bigger values, providing the user
                             with both fine and coarse control of parameters using these dials.
                            */
                            
                            if (dialVal <= 255 && dialVal >= 128) //left/anti-clockwise
                            {
                                //should I use the exact dialVal here?
                                
                                //flag that the 'timer' should start or be on.
                                dialCounterFlag[i] = 1;
                                //decrement the dial value.
                                //Ideally we want some more complex algorithm here
                                //that creates a non-linear value.
                                dialValue[i] -= 1;
                            }
                            else if (dialVal >= 1 && dialVal <= 127) //right/clockwise
                            {
                                //should I use the exact dialVal here?
                                
                                //flag that the 'timer' should start or be on.
                                dialCounterFlag[i] = 1;
                                //increment the dial value.
                                //Ideally we want some more complex algorithm here
                                //that creates a non-linear value.
                                dialValue[i] += 1;
                            }
                            
                            //if the timer is on...
                            if (dialCounterFlag[i] == 1)
                            {
                                dialCounter[i]++;
                                
                                //if the timer has 'finished'...
                                if (dialCounter[i] >= 10)
                                {
                                    if (dialValue[i] < 0)
                                    {
                                        /*
                                         The value will be a negative value the dial
                                         has been turned anti-clockwise. However the 
                                         application expects anti-clockwise turns to
                                         be of a value between 127-64, with a smaller
                                         value indicating a more coarse rotation. 
                                         Therefore we must convert the value to a 
                                         postive one and cap it at 64.
                                        */
                                        
                                        //multiply it by itself to create an exponential curve
                                        dialValue[i] *= dialValue[i];
                                        
                                        dialValue[i] = 128 - dialValue[i];
                                        
                                        if (dialValue[i] < 64)
                                            dialValue[i] = 64;
                                        
                                    }
                                    else
                                    {
                                        /*
                                         The application expects clockwise turns to
                                         be of a value between 1-63, with a larger
                                         value indicating a more coarse rotation. 
                                         Therefore we must cap the value at 63.
                                         */
                                        
                                        //multiply it by itself to create an exponential curve
                                        dialValue[i] *= dialValue[i];
                                        
                                        if (dialValue[i] > 63)
                                            dialValue[i] = 63;
                                    }
                                    
                                    //send the value to AlphaLiveEngine
                                    //std::cout << "Dial Value: " << dialValue[i] << std::endl;
                                    hidInputCallback(dialNum, dialValue[i], 0);
                                    
                                    //reset the relevant values so that the timer 'stops'
                                    //and that the whole timing algorithm is ready to start
                                    //again when requested.
                                    dialValue[i] = 0;
                                    dialCounter[i] = 0;
                                    dialCounterFlag[i] = 0;
                                }
                            }
                        }
                    }
                    
                    //===================================================================================
                    //===================================================================================
                    /*
                     Processing any incoming MIDI messages.
                     The firmware can send up to 6 messages in a HID report.
                     buf[100] is equal to the number of containing messages,
                     and then the following groups of 3 bytes are the individual
                     messages.
                     */
                    
                    if (buf[100] > 0)
                    {
                        //get number of MIDI messages within report
                        int noOfMessages = buf[100];
                        
                        //retrieve each MIDI message from the report
                        for (int i = 0; i < noOfMessages; i++)
                        {
                            //get the first byte index of the message
                            int messageIndex  = (i * 3) + 101;
                            
                            int message[3];
                            message[0] = buf[messageIndex];
                            message[1] = buf[messageIndex + 1];
                            message[2] = buf[messageIndex + 2];
                            
                            if (MidiMessage::getMessageLengthFromFirstByte((uint8) message[0]) == 1)
                            {
                                MidiMessage midiMessage (message[0]);
                                
                                //send the MIDI message to AlphaLiveEngine
                                processMidiInput(midiMessage);
                            }
                            else if (MidiMessage::getMessageLengthFromFirstByte((uint8) message[0]) == 2)
                            {
                                MidiMessage midiMessage (message[0], message[1]);
                                
                                //send the MIDI message to AlphaLiveEngine
                                processMidiInput(midiMessage);
                            }
                            else if (MidiMessage::getMessageLengthFromFirstByte((uint8) message[0]) == 3)
                            {
                                MidiMessage midiMessage (message[0], message[1], message[2]);
                                
                                //send the MIDI message to AlphaLiveEngine
                                processMidiInput(midiMessage);
                            }
                            
                        }
                        
                    }
                    
//                        //process any incoming midi messages
//                        //if the byte 100 is < 128 or > 255 it is not a correct MIDI message which
//                        //will cause an asseration failure when creating the MidiMessage object below.
//                        //Though this will probably need to be changed if we start using MIDI SysEx at all.
//                        
//                        if (buf[100] > 127 && buf[100] <= 255)
//                        {
//                            int message[3];
//                            message[0] = buf[100];
//                            message[1] = buf[101];
//                            message[2] = buf[102];
//                            
//                            std::cout << "MIDI message: " << message[0] << " " << message[1] << " " << message[2] << std::endl;
//                            
//                            //determine the message length, and init the relevent MidiMessage
//                            if (MidiMessage::getMessageLengthFromFirstByte((uint8) message[0]) == 1)
//                            {
//                                MidiMessage midiMessage (buf[100]);
//                            
//                                //send the MIDI message to AlphaLiveEngine
//                                processMidiInput(midiMessage);
//                            }
//                            else if (MidiMessage::getMessageLengthFromFirstByte((uint8) message[0]) == 2)
//                            {
//                                MidiMessage midiMessage (buf[100], buf[101]);
//                                
//                                //send the MIDI message to AlphaLiveEngine
//                                processMidiInput(midiMessage);
//                            }
//                            else if (MidiMessage::getMessageLengthFromFirstByte((uint8) message[0]) == 3)
//                            {
//                                MidiMessage midiMessage (buf[100], buf[101], buf[102]);
//                                
//                                //send the MIDI message to AlphaLiveEngine
//                                processMidiInput(midiMessage);
//                            }
//                        }
                
                    
                    // ==== write output report ====
                    
                    sharedMemory.enter();
                    
                    //if report contains messages, send it
                    if (outBuf[2] > 0)
                    {
                        outBuf[0] = 0x00;
                        outBuf[1] = 0x01;
                        hid_write(handle, outBuf, 128);
                        
//                        std::cout << "Report data: ";
//                        for (int i = 0; i < sizeof(outBuf); i++)
//                            printf("%02hhx ", outBuf[i]);
//                        printf("\n");
                    
                        //reset number of messages byte
                        outBuf[2] = 0x00;
                    }
                    
                    sharedMemory.exit();
                    
                }
                
                memset(buf,0,sizeof(buf));
                
            }
            
            
        }
        
        //=== if device is not currently connected ===
        else
        {
            //try and connect to the device
            
            if (reconnectCounter >= RECONNECT_TIME)
            {
                connectToDevice();
                reconnectCounter = 0;
            }
        }
        
        //what should the following sleep value be?
        #ifdef WIN32
        sleep(1); //should this actually be Sleep() which need a windows library defined? See hidtest.
        #else
        usleep(1*1000);
        #endif
        
        reconnectCounter ++;
        //std::cout << "listening... ";
    }
}

void HidComms::addMessageToHidOutReport (uint8 message[])
{
    sharedMemory.enter();
    
    int noOfMessages = outBuf[2];
    
    if (noOfMessages < 30)
    {
        //==== append message to out report ====
        
        //get index of the report where the new message should go
        int newMessageIndex = (noOfMessages * 4) + 3;
        
        outBuf[newMessageIndex] = message[0];
        outBuf[newMessageIndex + 1] = message[1];
        outBuf[newMessageIndex + 2] = message[2];
        outBuf[newMessageIndex + 3] = message[3];
        
        //increase number of messages byte value
        outBuf[2] = outBuf[2] + 1;
    }
    
    sharedMemory.exit();
    
}



void HidComms::connectToDevice()
{
    struct hid_device_info *devs, *cur_dev;
    double currentFirmwareNo = 0;
    String serialString, firmwareString;
    
    devs = hid_enumerate(0x1d50, 0x6021);
    cur_dev = devs;	
    while (cur_dev) 
    {
        printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
        printf("\n");
        printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("  Release:      %hx\n", cur_dev->release_number);
        printf("  Interface:    %d\n",  cur_dev->interface_number);
        printf("\n");
        
        //will the following algorithm work if the firmware number starts using characters (as it's a hex number)?
        currentFirmwareNo = String::toHexString(cur_dev->release_number).getIntValue() / 100.0;
        std::cout << "Firmware Version: " << currentFirmwareNo << std::endl;
        
        firmwareString = String (currentFirmwareNo);
        serialString = String(cur_dev->serial_number);
        
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
    
    // Open the device using the VID, PID,
    // and optionally the Serial number.
    handle = hid_open(0x1d50, 0x6021, NULL); // << AlphaSphere HID device
    //handle = hid_open(0x1d50, 0x6041, NULL); // << AlphaSphere bootloader HID device 
    
    //device not found
    if (!handle) 
    {
        //printf("unable to open device\n");
        
        if (appHasInitialised == false)
        {
            // if appliication is currently initialising, flag that
            // the midi output stuff will exist. Setup of midi output
            // stuff cannot be called from here as at this point the
            // AlphaLiveEngine won't exist.
            
            midiInOutExists = true;
        }
        
        hidDeviceStatus = 0;
        setDeviceStatus();
        
        setFirmwareDetails ("-", "-");
    }
    
    //device found
    else
    {
		hidDeviceStatus = 1;
		setDeviceStatus();
		setFirmwareDetails (firmwareString, serialString);

        if (midiInOutExists == true)
        {
            //if the midi output stuff currently exists, which would have been caused
            //by the application initialising without the hid device connected,
            //remove it.
            removeMidiInAndOut();
            midiInOutExists = false;
        }
        
        memset(buf,0,sizeof(buf));
        
        
        //==== check to see if the firmware needs updating ====
        
        File appDataDir(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() + File::getSeparatorString() + "Application Data");
        String wildcard = "SphereWare*";
        Array<File> hexFile;
        appDataDir.findChildFiles(hexFile, 2, false, wildcard);
        
        if (hexFile.size() > 0) //which it should
        {
            //get the new firmware version number from the hex file name
            StringArray tokens;
            tokens.addTokens(hexFile.getLast().getFileNameWithoutExtension(), "_", String());
            float newFirmwareNo = tokens[1].getFloatValue();
            //std::cout << hexFile.getLast().getFileNameWithoutExtension() << " " << newFirmwareNo << std::endl;
            
            //if new firware version number is greater than current firmware number,
            //flag that the firmware needs updating. 
            if (newFirmwareNo > currentFirmwareNo)
            {
                //On app launch the updateFirmware function is called from main.
                //Can't be called from here at launch as the mainWindow/Component needs
                //to be present, so need to just set a flag here. 
                //However if we are current not at app launch, it is
                //possible to call it directly from here.
                
                setFirmwareUpdateStatus (true);
                
                if (appHasInitialised == true)
                {
                    updateFirmware();
                }
            }
        }
        
        //==== check whether it's an elite model or not ====
        
        // Set the hid_read() function to be blocking
        // so that it won't continue until the IN report
        // is received.
        hid_set_nonblocking(handle, 0);
        
        res = 0;
        res = hid_read(handle, buf, sizeof(buf));
        
        //get the device type bit
        int deviceType = buf[97];
        deviceType = deviceType & 1;
        
        //set AlphaSphere device type
        setDeviceType (deviceType + 1);
                
        // Set the hid_read() function to be non-blocking
        // for the rest of the program
        hid_set_nonblocking(handle, 1);
        
        
        // ==== Configure LED settings ====
        
        //set LED status
        setLedSettings(1, AppSettings::Instance()->getHardwareLedStatus());
        //set LED pressure interation status
        setLedSettings(2, AppSettings::Instance()->getHardwareLedPressureStatus());
        //set LED clock interation status
        setLedSettings(3, AppSettings::Instance()->getHardwareLedClockStatus());
        //set LED mode
        setLedSettings(4, AppSettings::Instance()->getHardwareLedMode());
        
        //set LED colours
        for (int i = 0; i < 3; i ++)
            setLedColour(i, StoredSettings::getInstance()->hardwareLedColour[i]);

    }
}


int HidComms::getDeviceStatus()
{
    return hidDeviceStatus;
}

void HidComms::setAppHasInitialised()
{
    appHasInitialised = true;
}
