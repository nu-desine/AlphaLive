//
//  OscInput.cpp
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

#include "OscInput.h"
#include "../Functionality Classes/Other/LayoutsAndScales.cpp"

//incoming osc port
#define PORT 5003


OscInput::OscInput() :  Thread ("OscThread")
{
    recieveSocket = new UdpListeningReceiveSocket(IpEndpointName( IpEndpointName::ANY_ADDRESS, PORT), this);
	
    //start the osc thread
    startThread();
}

OscInput::~OscInput()
{
    recieveSocket->AsynchronousBreak();
	stopThread (500);		//maybe need to call break or async break on the listener object below 
    delete recieveSocket;
}

//get OSC message
void OscInput::ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
{
    try
    {
        if( strcmp( m.AddressPattern(), "/alpha" ) == 0 )
        {
            osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
            int recievedPad = (arg++)->AsInt32();
            int recievedValue = (arg++)->AsInt32();
            if( arg != m.ArgumentsEnd() )
                throw osc::ExcessArgumentException();
            
            recievedPad = Layouts::padArrangementLayout[recievedPad];
            
            //call callback function
            oscCallBack(recievedPad, recievedValue);
        }
    }
    catch( osc::Exception& e )
    {
        // any parsing errors such as unexpected argument types, or 
        // missing arguments get thrown as exceptions.
        std::cout << "error while parsing message: "
        << m.AddressPattern() << ": " << e.what() << "\n";
    }
}

//OSC thread
void OscInput::run()
{
    recieveSocket->RunUntilSigInt();
}
