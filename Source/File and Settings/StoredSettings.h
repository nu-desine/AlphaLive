
//  Created by Liam Meredith-Lacey on 01/05/2012.
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


#ifndef H_STOREDSETTINGS
#define H_STOREDSETTINGS

#include "./../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
    A singleton to hold persistent settings, and to save them in a
    suitable PropertiesFile.
*/
class StoredSettings
{
public:
    //==============================================================================
    StoredSettings();
    ~StoredSettings();

    juce_DeclareSingleton (StoredSettings, false);

    PropertiesFile& getProps();
    void flush();
    
    void setDefaultValues();

    //==============================================================================
    RecentlyOpenedFilesList recentFiles;
    File appProjectDir;
    int midiNoteDisplayType; // 1 - note number, 2 - note name
    int launchTask; // 1 - Open new project, 2 - open last project
    int killOnClockStop; // 0 - don't kill, 1 - kill
    int cleanOnClose; //1 - don't clean, 2 - clean
    
private:
    ScopedPointer<PropertiesFile> props;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoredSettings);
};


#endif   // H_STOREDSETTINGS
