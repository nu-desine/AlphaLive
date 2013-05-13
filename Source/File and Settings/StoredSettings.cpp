

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
        props->setValue ("initialLaunch", initialLaunch);
        props->setValue ("recentFiles", recentFiles.toString());
        props->setValue ("appProjectDir", appProjectDir.getFullPathName());
        props->setValue ("audioSettings", audioSettings);
        props->setValue ("midiNoteDisplayType", midiNoteDisplayType);
        props->setValue ("launchTask", launchTask);
        props->setValue ("killOnClockStop", killOnClockStop);
        props->setValue ("cleanOnClose", cleanOnClose);
        props->setValue ("autoSaveScenes", autoSaveScenes);
        props->setValue ("deviceType", deviceType);
        props->setValue ("autoCheckUpdates", autoCheckUpdates);
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
    
    initialLaunch = props->getIntValue("initialLaunch");

    //recent files...
    recentFiles.restoreFromString (props->getValue ("recentFiles"));
    recentFiles.removeNonExistentFiles();
    recentFiles.setMaxNumberOfItems(20);
    
    appProjectDir = props->getValue ("appProjectDir");
    audioSettings = props->getValue ("audioSettings");
    midiNoteDisplayType = props->getIntValue("midiNoteDisplayType");
    launchTask = props->getIntValue("launchTask");
    killOnClockStop = props->getIntValue("killOnClockStop");
    cleanOnClose = props->getIntValue("cleanOnClose");
    autoSaveScenes = props->getIntValue("autoSaveScenes");
    deviceType = props->getIntValue("deviceType");
    autoCheckUpdates = props->getIntValue("autoCheckUpdates");

}

void StoredSettings::setDefaultValues()
{
    //effectively most of this function will only run   
    //the first time the app is launched on a computer
    
    if (initialLaunch == 0)
    {  
        //this statement will only be true when AlphaLive is first run on a computer, in which
        //we want the Demo Project to be added to the recently opened files list that will
        //then be automatically launched.
        
        File demoProjFile (File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() + 
                           File::separatorString + 
                           "AlphaLive Projects" + 
                           File::separatorString +  
                           "Demo Project" + 
                           File::separatorString + 
                           "Demo Project.alphalive");
        
        if (demoProjFile.exists())
        {
            recentFiles.addFile(demoProjFile);
        }
        
        initialLaunch = 1;
    }
    
    if (appProjectDir == File::nonexistent)
    {
        //Create the AlphaLive Projects folder 
        File oldProjectDir = (File::getSpecialLocation(File::userMusicDirectory).getFullPathName()) + File::separatorString + "AlphaLive Projects";
        File defaultProjectDirectory = (File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName()) + File::separatorString + "AlphaLive Projects";
        
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
        launchTask = 2;
    
    if (cleanOnClose == 0)
        cleanOnClose = 2;
    
    if (autoSaveScenes == 0)
        autoSaveScenes = 2;
    
    if (deviceType == 0)
        deviceType = 1;
    
    if (autoCheckUpdates == 0)
        autoCheckUpdates = 2;
    
    flush();
}



