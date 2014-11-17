//
//  OscOutput.cpp
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

#include "OscOutput.h"

#define OUTPUT_BUFFER_SIZE 1024

OscOutput::OscOutput()
{
    
}

OscOutput::~OscOutput()
{
    
}

//what stuff within here could become member variables?
void OscOutput::transmitPadMessage(int padNumber, int padValue, int padVelocity, String address, int port)
{
    const char* ipAddress = address.toUTF8();

    UdpTransmitSocket transmitSocket (IpEndpointName(ipAddress, port));
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p (buffer,OUTPUT_BUFFER_SIZE);
    
    p << osc::BeginBundleImmediate << osc::BeginMessage( "/alpha" ) << padNumber << padValue << padVelocity << osc::EndMessage << osc::EndBundle;

    transmitSocket.Send(p.Data(), p.Size());
}

void OscOutput::transmitEliteControlMessage(int controlNumber, float controlValue, String address, int port)
{
    const char* ipAddress = address.toUTF8();
    
    UdpTransmitSocket transmitSocket (IpEndpointName(ipAddress, port));
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p (buffer,OUTPUT_BUFFER_SIZE);
    
    p << osc::BeginBundleImmediate << osc::BeginMessage( "/alpha" ) << controlNumber << controlValue << osc::EndMessage << osc::EndBundle;
    
    transmitSocket.Send(p.Data(), p.Size());
}