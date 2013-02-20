//
//  DeviceStatusComponent.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 08/02/2013.
//  Copyright 2013 nu desine. 
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

#include "DeviceStatusComponent.h"
#include "../../AlphaLiveLookandFeel.h"


DeviceStatusComponent::DeviceStatusComponent (int status)
{
    deviceStatus = status;
}

DeviceStatusComponent::~DeviceStatusComponent()
{
    
}

void DeviceStatusComponent::resized()
{
    
}
void DeviceStatusComponent::paint (Graphics& g)
{
    g.setColour(Colours::black);
	g.fillRoundedRectangle(1, 1, getWidth()-2, getHeight()-2, 5);
    g.setColour(Colours::grey);
	g.drawRoundedRectangle(1, 1, getWidth()-2, getHeight()-2, 5, 1);
    
    if (deviceStatus == 1)
        g.setColour(AlphaColours::blue.withAlpha(0.7f));
    else if (deviceStatus == 2)
        g.setColour(Colours::red.withAlpha(0.7f));
    else
        g.setColour(AlphaColours::nearlyblack);
    
	g.fillRoundedRectangle(5, 8, getWidth()-10, getHeight()-12, 1);

}

void DeviceStatusComponent::mouseDown (const MouseEvent &e)
{
    if (deviceStatus == 0)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, 
                                          translate("No AlphaSphere Connected."), 
                                          translate("AlphaLive cannot detect an AlphaSphere connected to your computer."));
    }
    else if (deviceStatus == 1)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, 
                                          translate("AlphaSphere Connected."), 
                                          translate("AlphaLive has detected an AlphaSphere with no issues connected to your computer."));
    }
    else if (deviceStatus == 2)
    {
        //eventually this should be a yes/no alert window that asks if they want a report about the detected issues
        AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, 
                                          translate("AlphaSphere Error."), 
                                          translate("The connected AlphaSphere has reported errors in its performance."));
    }
    
}
void DeviceStatusComponent::mouseUp (const MouseEvent &e)
{
    
}
void DeviceStatusComponent::mouseDrag (const MouseEvent &e)
{
    
}

void DeviceStatusComponent::setDeviceStatus (int status)
{
    deviceStatus = status;
    repaint();
}