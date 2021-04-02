//
//  Common.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 13/06/2012.
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

#ifndef H_COMMON
#define H_COMMON

#include "../../JuceLibraryCode/JuceHeader.h"
#include "CommandIDs.h"

//Full path string for the directory where all the application supporting files
//(e.g. Application Data, Documentation, Library) are stored.
//Don't use File::currentApplicationFile or File::currentExecutableFile here, as on macOS
//if the app is flagged as being from an 'unidentified' developer (which it currently
//will be) then the app may actually be run from a different location (/private/var/folders/)
//meaning these file locations won't be what we expect and cause incorrect file access!
const String appFilesDirString (File::getSpecialLocation(File::globalApplicationsDirectory).getFullPathName() + File::getSeparatorString() + "AlphaLive" + File::getSeparatorString());

extern ScopedPointer<ApplicationCommandManager> commandManager;

static double scaleValue (double value, double minValue, double maxValue, double minRange, double maxRange)
{
    return (((maxRange - minRange) * 
             (value - minValue)) / 
            (maxValue - minValue)) + minRange;
}

#endif   // H_COMMON
