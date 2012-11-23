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
    midiOutExists = hidDeviceExists =  false;
    
    connectToDevice();
    
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
                hidDeviceExists = false;
            }
            
            if (res > 0)
            {
                //            for (int i = 0; i < res; i++)
                //                printf("%02hhx ", buf[i]);
                //            printf("\n");
                
                //encode the recieved command byte here based on the report ID
                if (appHasInitialised == true)
                {
                    if (buf[0] == 0x01) //pad data report
                    {
                        unsigned short int pressure = 0;
                        pressure = buf[2] + (buf[3]<<8);
                        
                        hidInputCallback(buf[1], pressure, buf[4]);  
                    }
                    
                    //The elite dials and buttons could probably use a the same
                    //command ID now we're not using such a specific report descriptor.
                    
                    else if (buf[0] == 0x02) //elite button report
                    {
                        //set 'pad' value to be 102-104 to represent the elite buttons
                        hidInputCallback(buf[1]+102, buf[2], 0);
                    }
                    else if (buf[0] == 0x03) //elite dial
                    {
                        //set 'pad' value to be 100-101 to represent the elite dials
                        hidInputCallback(buf[1]+100, buf[2], 0);
                    }
                    
                    else if (buf[0] == 0x07) //test
                    {
                        for (int i = 0; i < res; i++)
                            printf("%02hhx ", buf[i]);
                        printf("\n");
                    }
                }
                
                memset(buf,0,sizeof(buf));
                
            }
            
            
            //        if (handle && sendOutputReport == true)
            //        {
            //            std::cout << "writing to device: ";
            //            printf("%02hhx ", dataToSend[0]);
            //            printf("%02hhx ", dataToSend[1]);
            //            printf("%02hhx ", dataToSend[2]);
            //            printf("%02hhx ", dataToSend[3]);
            //            printf("%02hhx ", dataToSend[4]);
            //            printf("\n");
            //            std::cout << hid_write(handle, dataToSend, 6) << std::endl;
            //            
            //            sendOutputReport = false;
            //            //dataToSend = nullptr;
            //        }
            
            //what should the following sleep value be?
            #ifdef WIN32
            sleep(5); //should this actually be Sleep() which need a windows library defined? See hidtest.
            #else
            usleep(5*1000);
            #endif
        }
        
        //=== if device is not currently connected ===
        else
        {
            //std::cout << "no device connected" << std::endl;
            //what should the following sleep value be?
            #ifdef WIN32
            sleep(1000); //should this actually be Sleep() which need a windows library defined? See hidtest.
            #else
            usleep(1000*1000);
            #endif
            
            //try and connect to the device
            connectToDevice();
        }
        
        
    }
}

//should i be passing in a pointer here instead of an array?
void HidComms::sendHidControlReport (uint8 *bytesToSend)
{
//    sharedMemory.enter();
//    dataToSend[0] = bytesToSend[0];
//    dataToSend[1] = bytesToSend[1];
//    dataToSend[2] = bytesToSend[2];
//    dataToSend[3] = bytesToSend[3];
//    dataToSend[4] = bytesToSend[4];
//    std::cout << "writing to device: ";
//    printf("%02hhx ", dataToSend[0]);
//    printf("%02hhx ", dataToSend[1]);
//    printf("%02hhx ", dataToSend[2]);
//    printf("%02hhx ", dataToSend[3]);
//    printf("%02hhx ", dataToSend[4]);
//    printf("\n");
//
//    //can ++the unit8 here in a for loop to apply values
//    sendOutputReport = true;
//    sharedMemory.exit();
    
    
    if (handle)
    {
        std::cout << "writing to device: ";
        printf("%02hhx ", bytesToSend[0]);
        printf("%02hhx ", bytesToSend[1]);
        printf("%02hhx ", bytesToSend[2]);
        printf("%02hhx ", bytesToSend[3]);
        printf("%02hhx ", bytesToSend[4]);
        printf("\n");
        std::cout << hid_write(handle, bytesToSend, 6) << std::endl;
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
        
        hidDeviceExists = false;
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
        
        hidDeviceExists = true;
        
        // Set the hid_read() function to be non-blocking.
        hid_set_nonblocking(handle, 1);
        memset(buf,0,sizeof(buf));
        
        //Here (possibly somewhere else?) we will need to send a report to request a report back
        //to find out what AlphaSphere model is plugged in. AlphaLive will then use the information
        //to initialise/change the GUI.
    }
}


bool HidComms::hasOpenedHidDevice()
{
    return hidDeviceExists;
}

void HidComms::setAppHasInitialised()
{
    appHasInitialised = true;
}
