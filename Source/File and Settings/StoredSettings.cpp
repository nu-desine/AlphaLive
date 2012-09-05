

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

#include "StoredSettings.h"


//==============================================================================
StoredSettings::StoredSettings()
{
    flush();
}

StoredSettings::~StoredSettings()
{
    flush();
    props = nullptr;
    clearSingletonInstance();
}

juce_ImplementSingleton (StoredSettings);


//==============================================================================
PropertiesFile& StoredSettings::getProps()
{
    jassert (props != nullptr);
    return *props;
}

void StoredSettings::flush()
{
    if (props != nullptr)
    {
        props->setValue ("recentFiles", recentFiles.toString());
        props->setValue ("appProjectDir", appProjectDir.getFullPathName());
        props->setValue ("midiNoteDisplayType", midiNoteDisplayType);
        props->setValue ("launchTask", launchTask);
        props->setValue ("killOnClockStop", killOnClockStop);
        props->setValue ("cleanOnClose", cleanOnClose);
    }

    props = nullptr;

    {
        // These settings are used in defining the properties file's location.
        PropertiesFile::Options options;
        options.applicationName     = "AlphaLive";
        options.folderName          = "AlphaLive";
        options.filenameSuffix      = "preferences";
        options.osxLibrarySubFolder = "Application Support";

        props = new PropertiesFile (options);

    }

    //recent files...
    recentFiles.restoreFromString (props->getValue ("recentFiles"));
    recentFiles.removeNonExistentFiles();
    recentFiles.setMaxNumberOfItems(20);
    
    appProjectDir = props->getValue ("appProjectDir");
    midiNoteDisplayType = props->getIntValue("midiNoteDisplayType");
    launchTask = props->getIntValue("launchTask");
    killOnClockStop = props->getIntValue("killOnClockStop");
    cleanOnClose = props->getIntValue("cleanOnClose");
    

}

void StoredSettings::setDefaultValues()
{
    //effectively most of this function will only run   
    //the first time the app is launched on a computer
    
    if (appProjectDir == File::nonexistent)
    {
        //Create the AlphaLive Projects folder 
        File oldProjectDir = (File::getSpecialLocation(File::userMusicDirectory).getFullPathName())+ "/AlphaLive Projects";
        File defaultProjectDirectory = (File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName())+ "/AlphaLive Projects";
        
        //UPDATE - CAN NOW PROBABLY REMOVE ALL CODE THAT CHECKS IF THE PROJECT DIR IS IN THE OLD LOCATION
        if (oldProjectDir.exists() == true)
        {
            //if directory is currently in the previous set location, move the folder to the new default location
            oldProjectDir.moveFileTo (defaultProjectDirectory);
            StoredSettings::getInstance()->appProjectDir = defaultProjectDirectory;
        }
        else if (defaultProjectDirectory.exists() == false)
        {
            //if there is no directory on the system at all, create a default one
            defaultProjectDirectory.createDirectory();
            StoredSettings::getInstance()->appProjectDir = defaultProjectDirectory;
        }
    }
    
    if (appProjectDir.exists() == false)
    {
        //if the systems set directory doesn't exist for some strange reason, create it.
        //should i check whether the parent directory exists too? incase the user has set it
        //to be on an external HD which isn't currently plugged in
        appProjectDir.createDirectory();
    }
    
    
    if (midiNoteDisplayType == 0)
        midiNoteDisplayType = 1;
    
    if (launchTask == 0)
        launchTask = 1;
    
    if (cleanOnClose == 0)
        cleanOnClose = 2;
    
    flush();
}



