//
//  LoadingWindow.cpp
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

#include "LoadingWindow.h"


LoadingWindow::LoadingWindow()
:               DocumentWindow (String::empty,
                                Colours::lightgrey,
                                0,
                                false)

{
    loadingComponent = new LoadingComponent();
    
    addToDesktop(getDesktopWindowStyleFlags());
    setTitleBarHeight(0); 
    
    //set loading component to own the content of the loading window
    setContentOwned(loadingComponent, false); 
    
    centreWithSize (500, 200);
    toFront(true);
    
    setVisible (true);
}

LoadingWindow::~LoadingWindow()
{
    //delete loadingComponent; //???
}
