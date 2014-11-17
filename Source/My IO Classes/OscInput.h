//
//  OscInput.h
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

#ifndef H_OSCINPUT
#define H_OSCINPUT

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Third Party Libraries/oscpack/osc/OscReceivedElements.h"
#include "../Third Party Libraries/oscpack/osc/OscPacketListener.h"
#include "../Third Party Libraries/oscpack/ip/UdpSocket.h"

/**
 Inherit from this class so that you can talk to the AlphaSphere and provide implementation for OscCallBack()
 */
class OscInput :    public Thread,
                    public osc::OscPacketListener
{
public:
    OscInput();
    virtual ~OscInput();
    
    //Thread callback function
    void run();

protected:
    //Osc messaging - virtual OscPacketListener callback function which inturn calls OscCallBack
    void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint );

private:
    UdpListeningReceiveSocket *recieveSocket; //pointer to an object that receives osc messages
    //pure virtual callback function
    virtual void oscCallBack (int receivedPad, int receivedValue)=0;
};


#endif // H_OSCINPUT
