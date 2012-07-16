//
//  GuiBeatIndicator.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/02/2012.
//  Copyright 2012 nu desine.
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
/* A simple shape that flickers/pulses to act as a visual metronome.
 Currently this probably doesn't need to be it's own class,
 however with more complicated animations that may be applied in the future
 it makes more sense to have it seperated from the GuiGlobalClock class.
 Also have it as a component in itself makes it easy to apply a mouse listener to it.
 
 This component could also be used as the tap tempo component!
 */

#ifndef H_GUIBEATINDICATOR
#define H_GUIBEATINDICATOR

#include "../../../JuceLibraryCode/JuceHeader.h"

class GuiBeatIndicator    :   public Component
{
public:
    GuiBeatIndicator();
    ~GuiBeatIndicator();
    
    void resized();
    void paint (Graphics& g);
    
    void triggerOn(); //currently not needed!
    void triggerOff(); //currently not needed!
    
    
private:
    int status; //0 - light 'off', 1 - light 'on' //currently not needed!
    
};

#endif //H_GUIBEATINDICATOR