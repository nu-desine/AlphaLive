//
//  OscOutput.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 01/12/2011.
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


#ifndef H_OSCOUTPUT
#define H_OSCOUTPUT

#include "../Third Party Libraries/oscpack/osc/OscOutboundPacketStream.h"
#include "../Third Party Libraries/oscpack/ip/UdpSocket.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class OscOutput
{
public:
    OscOutput();
    ~OscOutput();
    
    void transmitPadMessage(int padNumber, int padValue, int padVelocity, String address, int port);
    void transmitEliteControlMessage(int controlNumber, float controlValue, String address, int port);
    
private:

};

#endif // H_OSCOUTPUT