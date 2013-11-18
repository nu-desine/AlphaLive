

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

#if JUCE_MAC || JUCE_LINUX
#include <sys/stat.h>
#endif

#if JUCE_WINDOWS
#include <Windows.h>
#endif


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
        props->setValue ("interfaceTheme", interfaceTheme);
        props->setValue ("hardwareLedStatus", hardwareLedStatus);
        props->setValue ("hardwareLedPressureStatus", hardwareLedPressureStatus);
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
    interfaceTheme = props->getIntValue("interfaceTheme");
    hardwareLedStatus = props->getIntValue("hardwareLedStatus");
    hardwareLedPressureStatus = props->getIntValue("hardwareLedPressureStatus");

}

void StoredSettings::setDefaultValues()
{
    //effectively most of this function will only run
    //the first time the app is launched on a computer
    
    //============================================================================
    
    if (appProjectDir == File::nonexistent)
    {
        //Create the AlphaLive Projects folder 
        File defaultProjectDirectory = (File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName()) +
                                        File::separatorString +
                                        "AlphaLive Projects";
        
        if (defaultProjectDirectory.exists() == false)
        {
            //if there is no directory on the system at all, create a default one
            defaultProjectDirectory.createDirectory();
        }
        
        appProjectDir = defaultProjectDirectory;
    }
    
    if (appProjectDir.exists() == false)
    {
        //if the systems set directory doesn't exist for some strange reason, create it.
        //should i check whether the parent directory exists too? incase the user has set it
        //to be on an external HD which isn't currently plugged in
        appProjectDir.createDirectory();
    }
    
    //============================================================================
    
    //If this is the first time the application has been run after downloading it from the
    //AlphaLive webpage, we must move the Demo Project (Basic) project to the AlphaLive
    //Projects directory.
    installBasicDemoProj();
    
    //============================================================================
    
    if (initialLaunch == 0)
    {
        //this statement will only be true when AlphaLive is first run on a computer, in which
        //we want one of the Demo Projects to be added to the recently opened files list that will
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
        else
        {
            //see if the Demo Project (Basic) project exists, and if so load that.
            File demoProjBasicFile (File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() +
                                    File::separatorString +
                                    "AlphaLive Projects" +
                                    File::separatorString +
                                    "Demo Project (Basic)" +
                                    File::separatorString +
                                    "Demo Project (Basic).alphalive");
            
            if (demoProjBasicFile.exists())
            {
                recentFiles.addFile(demoProjBasicFile);
            }
        }
        
        initialLaunch = 1;
    }
    
    //============================================================================
    
    if (midiNoteDisplayType == 0)
        midiNoteDisplayType = 1;
    
    if (launchTask == 0)
        launchTask = 2;
    
    if (cleanOnClose == 0)
        cleanOnClose = 1;
    
    if (killOnClockStop == 0)
        killOnClockStop = 2;
    
    if (autoSaveScenes == 0)
        autoSaveScenes = 2;
    
    if (deviceType == 0)
        deviceType = 1;
    
    if (autoCheckUpdates == 0)
        autoCheckUpdates = 2;
    
    if (interfaceTheme == 0)
        interfaceTheme = 1;
    
    if (hardwareLedStatus == 0)
        hardwareLedStatus = 2;
    
    if (hardwareLedPressureStatus == 0)
        hardwareLedPressureStatus = 2;
    
    flush();
}

void StoredSettings::installBasicDemoProj()
{
    //This stuff is done here and called from setDefaultValues()
    //instead of within Main as it needs to be done after the
    //app project directory is created/set but before the default
    //project to open at launch is set.
    
    File basicDemoProjDir = (File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() +
                             File::separatorString +
                             "Demo Project (Basic)");
    
    if (basicDemoProjDir.exists())
    {
        //move this folder to the appProjectDir
        
        File newBasicDemoProjDir (appProjectDir.getFullPathName() +
                                  File::separatorString +
                                  "Demo Project (Basic)");
        
        if (newBasicDemoProjDir.exists())
            newBasicDemoProjDir.deleteRecursively();
        
        basicDemoProjDir.copyDirectoryTo(newBasicDemoProjDir);
        basicDemoProjDir.deleteRecursively();
        
        // set the project file to read-only.
        
        File basicDemoProjFile (newBasicDemoProjDir.getFullPathName() + File::separatorString + "Demo Project (Basic).alphalive");
        
        #if JUCE_MAC || JUCE_LINUX
        //To set the file permissions to read only, you can use the command chmod 555 in terminal
        //or use th chmod() function as documented here:
        //http://www.manpagez.com/man/2/chmod/osx-10.4.php
        
        chmod (basicDemoProjFile.getFullPathName().toUTF8(), S_IRUSR | S_IRGRP | S_IROTH);
        
        #endif
        
        #if JUCE_WINDOWS
        //Use the SetFileAttributes() function here like so:
        //http://msdn.microsoft.com/en-us/library/windows/desktop/aa365535(v=vs.85).aspx
        //to get more control over editting file permissions, you can use cacls
        
        SetFileAttributes(basicDemoProjFile.getFullPathName().toUTF8(), FILE_ATTRIBUTE_READONLY);
        
        #endif  
    }
    
}
