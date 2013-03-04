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


HidComms::HidComms() : Thread("HidThread")
{
    memset(outBuf,0,sizeof(buf));
    
    appHasInitialised = false;
    sendOutputReport = false;
    midiOutExists = hidDeviceStatus =  0;
    
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
    
    startThread();
}

HidComms::~HidComms()
{
    stopThread(1500);
    
    hid_close(handle);
    hid_exit();
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
                                hidInputCallback(i, pressure, velocity);
                                
                                prevPadPressure[i] = pressure;
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
                    
                    
                    // ==== write output report (just MIDI messages?) ====
                    
                    //if report contains messages, send it
                    if (outBuf[2] > 0)
                    {
                        sharedMemory.enter();
                        
                        outBuf[0] = 0x00;
                        outBuf[1] = 0x01;
                        hid_write(handle, outBuf, 129);
                        
//                        std::cout << "Report data: ";
//                        for (int i = 0; i < sizeof(outBuf); i++)
//                            printf("%02hhx ", outBuf[i]);
//                        printf("\n");
                    
                        //reset number of messages byte
                        outBuf[2] = 0x00;
                        
                        sharedMemory.exit();
                    }
                    
                }
                
                memset(buf,0,sizeof(buf));
                
            }
            
            //what should the following sleep value be?
            #ifdef WIN32
            sleep(1); //should this actually be Sleep() which need a windows library defined? See hidtest.
            #else
            usleep(1*1000);
            #endif
        }
        
        //=== if device is not currently connected ===
        else
        {
            //try and connect to the device
            connectToDevice();
            
            //std::cout << "no device connected" << std::endl;
            //what should the following sleep value be?
            #ifdef WIN32
            sleep(500); //should this actually be Sleep() which need a windows library defined? See hidtest.
            #else
            usleep(500*1000);
            #endif
        }
        
        //std::cout << "listening... ";
    }
}

void HidComms::addMessageToHidOutReport (uint8 message[])
{
    int noOfMessages = outBuf[2];
    
    if (noOfMessages < 30)
    {
        sharedMemory.enter();
        
        //==== append message to out report ====
        
        //get index of the report where the new message should go
        int newMessageIndex = (noOfMessages * 4) + 3;
        
        outBuf[newMessageIndex] = message[0];
        outBuf[newMessageIndex + 1] = message[1];
        outBuf[newMessageIndex + 2] = message[2];
        outBuf[newMessageIndex + 3] = message[3];
        
        //increase number of messages byte value
        outBuf[2] = outBuf[2] + 1;
        
        sharedMemory.exit();
    }
    
}



void HidComms::connectToDevice()
{
    struct hid_device_info *devs, *cur_dev;
    float currentFirmwareNo = 0;
    
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
        
        currentFirmwareNo = cur_dev->release_number / 100.0; 
        
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
            
            midiOutExists = true;
        }
        
        hidDeviceStatus = 0;
        setDeviceStatus();
    }
    
    //device found
    else
    {
        if (midiOutExists == true)
        {
            //if the midi output stuff currently exists, which would have been caused
            //by the application initialising without the hid device connected,
            //remove it.
            removeMidiOut();
            midiOutExists = false;
        }
        
        memset(buf,0,sizeof(buf));
        
        
        //==== check to see if the firmware needs updating ====
        
        File appDataDir(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() + File::separatorString + "Application Data");
        String wildcard = "SphereWare*";
        Array<File> hexFile;
        appDataDir.findChildFiles(hexFile, 2, false, wildcard);
        
        if (hexFile.size() > 0) //which it should
        {
            //get the new firmware version number from the hex file name
            StringArray tokens;
            tokens.addTokens(hexFile.getLast().getFileNameWithoutExtension(), "_", String::empty);
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
        
        //===============================================================================
        //Send a report to the device requesting a report containing AlphaLive setup data,
        //and then process the received reports data to set up AlphaLive correctly.
        //===============================================================================
        
        // Set the hid_read() function to be blocking to start with.
        hid_set_nonblocking(handle, 0);
        res = 0;
        
        unsigned char dataToSend[1];
        dataToSend[0] = 0x05; //host setup data request command ID
        //hid_write(handle, dataToSend, 129);
        int uncommentThisLine;
        
        //TEMPORARILY COMMENTED OUT TO PREVENT PAUSING FOR THE TIME BEING
        //res = hid_read(handle, buf, sizeof(buf));
        int uncommentThisLineAsWell;
        
        std::cout << "Received AlphaLive setup report with the following data: " << std::endl;
        for (int i = 0; i < res; i++)
            printf("%02hhx ", buf[i]);
        printf("\n");
        
        //========process received report data here...========
        
        //set AlphaSphere device type
        //setDeviceType (buf[2] + 1);
                
        // Set the hid_read() function to be non-blocking.
        hid_set_nonblocking(handle, 1);
        
        hidDeviceStatus = 1;
        setDeviceStatus();
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
