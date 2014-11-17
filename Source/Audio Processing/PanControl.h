//
//  PanControl.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 22/11/2011.
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

#ifndef H_PANCONTROL
#define H_PANCONTROL

namespace PanControl 
{
    //Should these be inline functions????????
    
    static float leftChanPan_(float control)
    {
        //takes the the pan control value and formulates
        //and returns the gain of the left channel based on the value
        
        float gain;
        
        if (control > 0.5)
            gain = 2.0 - (2.0 * control);
        else
            gain = 1.0;
        
        return gain;
    }
    
    static float rightChanPan_(float control)
    {
        //takes the the pan control value and formulates
        //and returns the gain of the right channel based on the value
        
        float gain;
        
        if (control < 0.5)
            gain = 2.0 * control;
        else
            gain = 1.0;
        
        return gain;
    }
}

#endif