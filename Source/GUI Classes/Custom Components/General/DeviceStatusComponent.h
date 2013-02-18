//
//  DeviceStatusComponent.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 08/02/2013.
//  Copyright 2013 nu desine. All rights reserved.
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

#ifndef DEVICESTATUSCOMPONENT_H
#define DEVICESTATUSCOMPONENT_H

#include "../../../../JuceLibraryCode/JuceHeader.h"

class DeviceStatusComponent :	public Component
{
public:
    
    DeviceStatusComponent();
    ~DeviceStatusComponent();
	
	void resized();
	void paint (Graphics& g);

    void mouseDown (const MouseEvent &e);
    void mouseUp (const MouseEvent &e);
    void mouseDrag (const MouseEvent &e);
    
    void setDeviceStatus (int status);

private:
    //==============================================================================
    
    int deviceStatus; //0 - not connected, 1 - connected, 2 - connected with problems
};  


#endif //DEVICESTATUSCOMPONENT_H