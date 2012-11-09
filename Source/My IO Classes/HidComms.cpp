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
    struct hid_device_info *devs, *cur_dev;
    
    devs = hid_enumerate(0x0, 0x0);
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
    handle = hid_open(0x3eb, 0x204f, NULL); // << LUFA demo HID device
    //handle = hid_open(0x1d50, 0x6021, NULL); // << AlphaSphere HID device
    //handle = hid_open(0x1d50, 0x6041, NULL); // << AlphaSphere bootloader HID device 
    
    if (!handle) 
    {
        printf("unable to open device\n");
        hasOpenedDevice = false;
    }
    
    else
    {
        hasOpenedDevice = true;
        
        // Read the Manufacturer String
        wstr[0] = 0x0000;
        res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
        if (res < 0)
        {
            printf("Unable to read manufacturer string\n");
        }
        printf("Manufacturer String: %ls\n", wstr);
        
        // Read the Product String
        wstr[0] = 0x0000;
        res = hid_get_product_string(handle, wstr, MAX_STR);
        if (res < 0)
        {
            printf("Unable to read product string\n");
        }
        printf("Product String: %ls\n", wstr);
        
        // Read the Serial Number String
        wstr[0] = 0x0000;
        res = hid_get_serial_number_string(handle, wstr, MAX_STR);
        if (res < 0)
        {
            printf("Unable to read serial number string\n");
        }
        printf("Serial Number String: (%d) %ls", wstr[0], wstr);
        printf("\n");
        
        // Read Indexed String 1
        wstr[0] = 0x0000;
        res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
        if (res < 0)
        {
            printf("Unable to read indexed string 1\n");
        }
        printf("Indexed String 1: %ls\n", wstr);
        
        
        
        // Set the hid_read() function to be blocking.
        hid_set_nonblocking(handle, 1);
        
        memset(buf,0,sizeof(buf));
        
        startThread();
        
    }
}

HidComms::~HidComms()
{
    stopThread(100);
    
    hid_close(handle);
    // Free static HIDAPI objects. 
    hid_exit();
}

void HidComms::run()
{
    res = 0;
    
    //curently in the below loop, as the hid_read is set to be blocking
    //when you attempt to exit the thread at close it will force kill it.
    //instead, should I be using a non-blocking hid_read and have a call
    //to sleep at 5 ms?
    while( ! threadShouldExit())
    {
        res = hid_read(handle, buf, sizeof(buf));
        if (res == 0)
            //printf("no report...\n");
        if (res < 0)
            printf("Unable to read()\n");
        if (res > 0)
        {
            for (int i = 0; i < res; i++)
                printf("%02hhx ", buf[i]);
            printf("\n");
            
            //encode the recieved report here based on the report ID,
            //and pass in the correct values to hidInputCallback()
            
            hidInputCallback(0, 0, 0);
        }
        
        //what should the following sleep value be
        #ifdef WIN32
        Sleep(5);
        #else
        usleep(5*1000);
        #endif
    }
}

//should i be passing in a pointer here instead of an array?
void HidComms::sendHidControlReport (unsigned char bytesToSend[])
{
    if (handle)
    {
        std::cout << "writing to device " << std::endl;
        printf("%02hhx ", bytesToSend[0]);
        printf("%02hhx ", bytesToSend[1]);
        printf("%02hhx ", bytesToSend[2]);
        printf("%02hhx ", bytesToSend[3]);
        printf("\n");
        hid_write(handle, bytesToSend, sizeof(bytesToSend));
    }
}

bool HidComms::hasOpenedHidDevice()
{
    return hasOpenedDevice;
}
