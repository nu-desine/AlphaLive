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
    appHasInitialised = false;
    sendOutputReport = false;
    midiOutExists = hidDeviceStatus =  0;
    
    for (int i = 0; i < 48; i++)
        prevPadPressure[i] = 0;
    for (int i = 0; i < 2; i++)
        prevButtonValue[i] = 0;
    
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
                            
                                //std::cout << pressure << " : " << velocity << std::endl;
                                hidInputCallback(i, pressure, velocity);
                                
                                prevPadPressure[i] = pressure;
                            }
                        }
                        
                        //The following algorithm must be set up so that
                        //it allows for the possibility of multiple buttton 
                        //presses/releases or multiple dial rotations to be 
                        //sent in a single message within the HID report.
                        //Therefore we must check each pair of bits within
                        //the button data byte (buf[messageIndex + 1]) 
                        //and the dial data byte (buf[messageIndex + 2]).
                        //Using the bitwise right-shift operator we need to check
                        //for value of 1 and 2 for each control.
                        
                        //hidInputCallback() expects the following values:
                        //button numbers = 102 to 104 (ignore reset button)
                        //dial numbers = 100 - 101
                        //button press = 1
                        //button release = 0
                        //dial left turn = 127
                        //dial right turn = 1
                        
                        int eliteByte = 97;
                        
                        //decode bits 1-3 as pairs from the button/dial data byte,
                        //looking for a button presses and releases.
                        for (int i = 1; i < 4; i++)
                        {
                            int buttonNum = i + 101;
                            int buttonVal = buf[eliteByte] >> i;
                            
                            if (buttonVal != prevButtonValue[i-1])
                            {
                                hidInputCallback(buttonNum, buttonVal, 0);
                                prevButtonValue[i-1] = buttonVal;
                            }
                        }
                        
                        //decode bits 4-7 as pairs from the button/dial data byte,
                        //looking for a dial rotations.
                        for (int i = 0; i < 2; i++)
                        {
                            int dialNum = i + 100;
                            int dialVal = buf[eliteByte] >> (i * 2) + 4;
                            
                            if (dialVal == 1) //left/anti-clockwise
                            {
                                hidInputCallback(dialNum, 127, 0);
                            }
                            else if (dialVal == 2) //right/clockwise
                            {
                                hidInputCallback(dialNum, 1, 0);
                            }
                        }
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

//should i be passing in a pointer here instead of an array?
void HidComms::sendHidControlReport (uint8 *bytesToSend)
{

    if (handle)
    {
//        std::cout << "writing to device: ";
//        printf("%02hhx ", bytesToSend[0]);
//        printf("%02hhx ", bytesToSend[1]);
//        printf("%02hhx ", bytesToSend[2]);
//        printf("%02hhx ", bytesToSend[3]);
//        printf("%02hhx ", bytesToSend[4]);
//        printf("\n");
        hid_write(handle, bytesToSend, 9);
    }
}



void HidComms::connectToDevice()
{
    struct hid_device_info *devs, *cur_dev;
    
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
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
    
    // Open the device using the VID, PID,
    // and optionally the Serial number.
    //handle = hid_open(0x3eb, 0x204f, NULL); // << LUFA demo HID device
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
        
        //===============================================================================
        //Send a report to the device requesting a report containing AlphaLive setup data,
        //and then process the received reports data to set up AlphaLive correctly.
        //===============================================================================
        
        // Set the hid_read() function to be blocking to start with.
        hid_set_nonblocking(handle, 0);
        res = 0;
        
        unsigned char dataToSend[1];
        dataToSend[0] = 0x05; //host setup data request command ID
        hid_write(handle, dataToSend, 9);
        
        //TEMPORARILY COMMENTED OUT TO PREVENT PAUSING FOR THE TIME BEING
        //res = hid_read(handle, buf, sizeof(buf));
        int uncommentThisLine;
        
        if (res > 0 && buf[0] == 0x04)
        {
            std::cout << "Received AlphaLive setup report with the following data: " << std::endl;
            for (int i = 0; i < res; i++)
                printf("%02hhx ", buf[i]);
            printf("\n");
            
            //========process received report data here...========
            
            //check to see if the firmware needs updating
            File appDataDir(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() + File::separatorString + "Application Data");
            String wildcard = "SphereWare*";
            Array<File> hexFile;
            appDataDir.findChildFiles(hexFile, 2, false, wildcard);
            
            if (hexFile.size() > 0) //which it should
            {
                int currentFirmwareNo = buf[1];
                int newFirmwareNo = hexFile.getLast().getFileNameWithoutExtension().getTrailingIntValue();
                
                std::cout << hexFile[0].getFileNameWithoutExtension() << " " << newFirmwareNo << std::endl;
                
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
            
            //set AlphaSphere device type
            setDeviceType (buf[2] + 1);
        }
                
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
