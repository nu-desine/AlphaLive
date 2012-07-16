//
//  LoadingWindow.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 17/11/2011.
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

#ifndef H_LOADINGWINDOW
#define H_LOADINGWINDOW

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "LoadingComponent.h"

//==============================================================================
class LoadingWindow   : public DocumentWindow
{
public:
    //==============================================================================
    LoadingWindow();
    ~LoadingWindow();
    
    
    /* Note: Be careful when overriding DocumentWindow methods - the base class
     uses a lot of them, so by overriding you might break its functionality.
     It's best to do all your work in you content component instead, but if
     you really have to override any DocumentWindow methods, make sure your
     implementation calls the superclass's method.
     */
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoadingWindow)
    
    LoadingComponent *loadingComponent;
   
};


#endif  //H_LOADINGWINDOW
