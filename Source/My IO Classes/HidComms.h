//
//  HidComms.h
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

#ifndef H_HIDCOMMS
#define H_HIDCOMMS

#define MAX_STR 255

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Third Party Libraries/HIDAPI/hidapi.h"

class HidComms  :   public Thread
{
public:
    HidComms();
    virtual ~HidComms();
    
    void run();
    virtual void hidInputCallback (int pad, int value, int velocity) = 0;
    
    void sendHidControlReport (uint8 *bytesToSend);
    
    bool hasOpenedHidDevice();
    void setAppHasInitialised();
    
private:
    int res;
    unsigned char buf[8]; //how big should this be?
    wchar_t wstr[MAX_STR];
    
    hid_device *handle;
    
    bool hasOpenedDevice;
    
    CriticalSection sharedMemory;
    
    unsigned char dataToSend[5];
    bool sendOutputReport;
    
    bool appHasInitialised;
};


#endif //H_HIDCOMMS