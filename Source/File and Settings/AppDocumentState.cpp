//
//  AppDocumentState.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 07/11/2011.
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

#include "AppDocumentState.h"
#include "AppSettings.h"
#include "../Functionality Classes/Sequencer Mode/SequencerValues.h"
#include "../Application/MainWindow.h"
#include "StoredSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[i]
#define PAD_SETTINGS_pad AppSettings::Instance()->padSettings[currentlySelectedPad]
#define PAD_SETTINGS_pads AppSettings::Instance()->padSettings[padNum]

AppDocumentState::AppDocumentState()
{
    for (int i = 0; i <= NO_OF_PRESETS-1; i++)
    {
        presetData.insert(i, new XmlElement("PRESET_" + String(i)));
    }
    
    projectData = new XmlElement("PROJECT_SETTINGS");
    
    guiUpdateFlag = 0;
    presetToUpdate = 0;
    presetStatus = 0;
    
    currentProjectFile = File::nonexistent;
    
    mainAppWindowRef = nullptr;
    
    shouldDisplayAlertWindow = true;
    currentlySelectedPreset = 0;
}


AppDocumentState::~AppDocumentState()
{

}

void AppDocumentState::setMainAppWindowRef (MainAppWindow *ref)
{
    mainAppWindowRef = ref;
}

void AppDocumentState::createNewProject()
{
    /*
     - ask the user if they want to save the current project
     - reset all settings; pad and global, and the project settings
     - clear all presetData XmlElements
     - reset the currentWorkingDirectory back to the temp directory. Clear it's current contents
     - set the currectProjectFile to nonexistent
     - reset the presetComponent
     - reset the window title bar text
     
     */
    
    //========== ask user if they would like to save the current project first =========
    int modeCheck = 0; //don't show 'save?' alert
    int shouldSave = 2; //don't save
    
    //check to see if the user might want to save anything first by looking for a pad
    //with a mode set to it
    //IDEALLY WE NEED A BETTER METHOD OF CHECKING WHETHER THE USER MIGHT WANT TO SAVE
    for (int i = 0; i <= 47; i++)
    {
        modeCheck = PAD_SETTINGS->getMode();
        if (modeCheck > 0)
            break;
    }
    
    //if found a pad with a mode set to it, ask if user wants to save first
    if (modeCheck != 0)
    {
        shouldSave = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon, "Create New Project", "Would you like to save the current project first?", "Yes", "No", "Cancel");
    }
    
    //if the user didn't press cancel on the alertwindow ('cancel load command')
    if (shouldSave != 0) 
    {
        
        if (shouldSave == 1) //'yes'
        {
            shouldDisplayAlertWindow = false;
            saveProject();
        }
        
        
        //============= reset all settings =================
        
        AppSettings::Instance()->resetData();
        
        for (int i = 0; i <= 47; i++)
            PAD_SETTINGS->resetData(0);
        
        
        currentProjectFile = File::nonexistent;
        
        File::getSpecialLocation(File::tempDirectory).deleteRecursively();
        File::getSpecialLocation(File::tempDirectory).setAsCurrentWorkingDirectory();
        
        //========= clear all XmlElement objects and update the presetComponent display ===========
        
        //let the observer know it will need to update the presetComponent GUI next time it is 'notified'
        guiUpdateFlag = 1;
        
        projectData->removeAllAttributes();
        
        for (int i = 0; i <= NO_OF_PRESETS-1; i++)
        {
            //accessed by observer in order to update the relevent preset slot's GUI
            presetToUpdate = i;
            
            //clear the xmlelement for the current preset number
            clearPreset(i);
            
            //display GUI preset slot as empty
            presetStatus = 0;
        
            //set the first preset to be display as 'selected'
            if (i == 0)
            {
                presetStatus = 2;
                setCurrentlySelectedPreset(0);
            }
            
            //update display
            notifyObs();
        }
        
        //let the observer know it will need to update the settings display next time it is 'notified'
        guiUpdateFlag = 0;
        
        //save the reset settings, then call update the main display
        saveToPreset(0);
        notifyObs();
        
        //change the window title bar text
        mainAppWindowRef->setTitleBarText("untitled");
        
    }
}


void AppDocumentState::saveProject()
{
    if (currentProjectFile == File::nonexistent) //if no performance has been saved yet
    {
        saveProjectAs();
    }
    
    else //replace currentProjectFile
    {
        //first, need to save the current project and preset settings
        saveProjectSettings();
        saveToPreset(currentlySelectedPreset);
        
        currentProjectFile.deleteFile();
        currentProjectFile.create(); //create the file

        XmlElement performanceSettings("PERFORMANCE");

        performanceSettings.addChildElement(projectData);
        
        for (int i = 0; i <= NO_OF_PRESETS-1; i++)
        {
            performanceSettings.addChildElement(presetData[i]);
        }
        
        String xmlDoc = performanceSettings.createDocument(String::empty, false);
        currentProjectFile.appendText(xmlDoc);
        
        //remove projectData and presetData child elements from performanceSettings so that they aren't deleted when
        //performanceSettings goes out of scope.
        
        performanceSettings.removeChildElement(projectData, false);
        
        for (int i = 0; i <= NO_OF_PRESETS-1; i++)
        {
            performanceSettings.removeChildElement(presetData[i], false);
        }
        
        if (shouldDisplayAlertWindow == true)
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Project Saved", "The project settings have been successfully saved to file");
        }
        shouldDisplayAlertWindow = true;
        
        
        //add the file to the 'recent files' list
        registerRecentFile (currentProjectFile);
        
    }
}


void AppDocumentState::saveProjectAs()
{
    //navigate to app directory
    FileChooser saveFileChooser("Create a AlphaLive project to save...", 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphalive");
    
    if (saveFileChooser.browseForFileToSave(false))
    {
        //create a project directory
        File savedDirectory (saveFileChooser.getResult());
        
        //create folder to hold the projects audio files (if it doesn't already exist, which it shouldnt (?))
        File audioFileDirectory = (savedDirectory.getFullPathName() + File::separatorString + "Audio Files");
        
        if (AppSettings::Instance()->getCopyExternalFiles() == true)
        {
            //copy current working directory to the audio files directory
            File::getCurrentWorkingDirectory().copyDirectoryTo(audioFileDirectory);
        }
        else
            audioFileDirectory.createDirectory();  
        //set the audio files directory as the new working directory so when audio files are imported they go straight into here
        audioFileDirectory.setAsCurrentWorkingDirectory();
        
        //create file
        File savedFile (savedDirectory.getFullPathName() + File::separatorString + savedDirectory.getFileName()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".alphalive"; //append an extension name to the filepath name
        savedFile = (stringFile); //set the file to this name
        
        bool overwrite = true; //by default true
        
        //how do i check for overriding here?
        
        //delete the file if it exists & write the new data
        if (savedFile.exists())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "This File Already Exists!", "Are you sure you want to overwrite this file?");
        }
        
        if (overwrite == true)
        {
            //first, need to save the current project and preset settings
            saveToPreset(currentlySelectedPreset);
            saveProjectSettings();
            
            savedDirectory.createDirectory();
            savedFile.deleteFile();
            savedFile.create(); //create the file
    
            XmlElement performanceSettings("PERFORMANCE");
            
            performanceSettings.addChildElement(projectData);
            
            for (int i = 0; i <= NO_OF_PRESETS-1; i++)
            {
                performanceSettings.addChildElement(presetData[i]);
            }
            
            String xmlDoc = performanceSettings.createDocument(String::empty, false);
            savedFile.appendText(xmlDoc);
            
            //remove projectData and presetData child elements from performanceSettings so that they aren't deleted when
            //performanceSettings goes out of scope.
            
            performanceSettings.removeChildElement(projectData, false);
            
            for (int i = 0; i <= NO_OF_PRESETS-1; i++)
            {
                performanceSettings.removeChildElement(presetData[i], false);
            }
            
            //set the current open document to the document just saved
            currentProjectFile = savedFile;
            //change the window title bar text
            mainAppWindowRef->setTitleBarText(currentProjectFile.getFileNameWithoutExtension());
            
            if (shouldDisplayAlertWindow == true)
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Project Saved", "The project settings have been successfully saved to file");
            
            shouldDisplayAlertWindow = true;
            
            //add the file to the 'recent files' list
            registerRecentFile (currentProjectFile);
            
        }
        
    }
    
}

void AppDocumentState::loadProject (bool openBrowser, File fileToOpen)
{
    //openBrower will be true when the 'Load' button is clicked, and false when a .alphalive file is clicked
    //fileToOpen will be equal to File::nonexistent when Load is click, and the file path when a .alphalive
    //file is clicked
    
    //========== NEW: ask user if they would like to save the current project first =========
    int modeCheck = 0; //don't show 'save?' alert
    int shouldSave = 2; //don't save
    
    //check to see if the user might want to save anything first by looking for a pad
    //with a mode set to it
    //IDEALLY WE NEED A BETTER METHOD OF CHECKING WHETHER THE USER MIGHT WANT TO SAVE
    for (int i = 0; i <= 47; i++)
    {
        modeCheck = PAD_SETTINGS->getMode();
        if (modeCheck > 0)
            break;
    }
    
    //if found a pad with a mode set to it, ask if user wants to save first
    if (modeCheck != 0)
    {
        shouldSave = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon, "Load New Project", "Would you like to save the current project first?", "Yes", "No", "Cancel");
    }
    
    //if the user didn't press cancel on the alertwindow ('cancel load command')
    if (shouldSave != 0) 
    {
        
        if (shouldSave == 1) //'yes'
        {
            shouldDisplayAlertWindow = false;
            saveProject();
        }
        
        // ========================== 'LOAD PROJECT' CODE ==================================
        
        //navigate to app directory
        FileChooser loadFileChooser("Select a .alphalive file to load...", 
                                    StoredSettings::getInstance()->appProjectDir, 
                                    "*.alphalive");
        
        bool shouldLoad;
        
        if (openBrowser == true)
            shouldLoad = loadFileChooser.browseForFileToOpen(); //open file browser
        
        if (shouldLoad || openBrowser == false)
        {
            //let the observer know it will need to update the presetComponent GUI next time it is 'notified'
            guiUpdateFlag = 1;
            
            //get file
            File loadedFile;
            if (openBrowser == true)
                loadedFile = loadFileChooser.getResult();
            else
                loadedFile = fileToOpen;
            
            //get the folder that holds the projects audio files
            File audioFileDirectory = (loadedFile.getParentDirectory().getFullPathName() + File::separatorString + "Audio Files");
            
            //if so some strange reason the directory doesn't exist, create it
            if (audioFileDirectory.exists() == false)
                audioFileDirectory.createDirectory();
            
            //set the Audio Files directory as the new working directory so loaded audio files can be found
            audioFileDirectory.setAsCurrentWorkingDirectory();
            
            //parse file into xml file
            ScopedPointer<XmlElement> loadedXml (XmlDocument::parse(loadedFile));
            
            if (loadedXml != nullptr && loadedXml->hasTagName("PERFORMANCE"))
            {
                //=====================load projectData settings=======================
                
                //reset/clear XmlElement.
                if (projectData != nullptr)
                    projectData->removeAllAttributes();
                    
                //put the loaded xml data into the xmlelement for the project settings
                XmlElement *projSettingsXml = loadedXml->getChildByName("PROJECT_SETTINGS");
                
                //check to see if the project settings child element actually exists (it won't within older AlphaLive Projects)
                if (loadedXml->containsChildElement(projSettingsXml) == true)
                    projectData = new XmlElement(*projSettingsXml);
                
                //apply the settings to AppSettings variables, even if "PROJECT_SETTINGS" doesn't exist (old file format), as default values should then be set
                loadProjectSettings();
                
                
                //WHY DON'T I NEED TO delete or remove projSettingsXml here?
                //I think it's because above I created a hard copy of projSettingsXml when applying it to projectData,
                //therefore when loadedXml goes out of scope and delete's projSettingsXml, it doesn't effect projectData
                //in anyway.
                
                //=========load the child elements of loadedXml and put them in the presetData objects===========
                for (int i = 0; i <= NO_OF_PRESETS-1; i++)
                {
                    //accessed by observer in order to update the relevent preset slot's GUI
                    presetToUpdate = i;
                    
                    //clear the xmlelement for the current preset number
                    clearPreset(i);
                    
                    //put the loaded xml data into the xmlelement for the current preset
                    XmlElement* childToInsert = loadedXml->getChildByName("PRESET_" + String(i));
                    presetData.insert (i, childToInsert);
                    //remove presetData childelement from loadedXml so it isn't deleted when loadedXml goes out of scope!
                    loadedXml->removeChildElement (childToInsert, false);
                    
                    if (presetData[i]->getNumChildElements() > 0) //if this preset contains something
                    {
                        //display GUI preset slot as filled 
                        presetStatus = 1;
                    }
                    else if (presetData[i]->getNumChildElements() == 0) //if this preset contains nothing
                    {
                        //display GUI preset slot as empty
                        presetStatus = 0;
                    }
                    
                    //set the first preset to be display as 'selected'
                    if (i == 0)
                    {
                        presetStatus = 2;
                        setCurrentlySelectedPreset(0);
                    }
                    
                    //update display
                    notifyObs();
                }
                
                //let the observer know it will need to update the settings display next time it is 'notified'
                guiUpdateFlag = 0;
                
                //call loadFromPreset to load the settings of preset 0 into the application
                loadFromPreset(0);
            }
            
            currentProjectFile = loadedFile;
            
            //=====================================================
            //==============NEW - reset unused mode settings=======
            //=====================================================
            /*
             Here, the settings of the modes that aren't being used for each pad are reset to their default values.
             */
            for (int i = 0; i <=47; i++)
            {
                PAD_SETTINGS->resetData(PAD_SETTINGS->getMode());
            }
            //=====================================================
            
            //change the window title bar text
            mainAppWindowRef->setTitleBarText(currentProjectFile.getFileNameWithoutExtension());
            
            //add the file to the 'recent files' list
            registerRecentFile (currentProjectFile);
            
        }
        
    }
     
}



void AppDocumentState::savePresetToDisk (int presetNumber)
{
    //first save the current settings to the current preset (saves the user having to first save into the preset and then save to disk!)
    saveToPreset(presetNumber);
    
    //navigate to app directory
    FileChooser saveFileChooser("Create an AlphaLive preset file to save...", 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphapreset");
    if (saveFileChooser.browseForFileToSave(false))
    {
        //create a project directory
        File savedDirectory (saveFileChooser.getResult());
        File savedDirectoryName = savedDirectory;
        String directoryString = savedDirectory.getFullPathName() + " (PRESET)";
        savedDirectory = directoryString;
        
        File savedFile (savedDirectory.getFullPathName() + File::separatorString + savedDirectoryName.getFileName()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".alphapreset"; //append an extension name to the filepath name
        savedFile = stringFile; //set the file to this name
        
        bool overwrite = true; //by default true
        
        //delete the file if it exists &write the new data
        if (savedFile.existsAsFile())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "This File Already Exists!", "Are you sure you want to overwrite this file?");
        }
        
        if (overwrite == true)
        {
            //------
            //MOVE ANY NEEDED AUDIO FILES INTO THE DEDICATED AUDIO FILES DIRECTORY FOR THIS PRESET
            //DO NOT CHANGE THE WORKING DIRECTORY HERE
            //------
            
            //create folder to hold the projects audio files
            File audioFileDirectory = (savedDirectory.getFullPathName() + File::separatorString + "Audio Files");
            audioFileDirectory.createDirectory();
            
            
            
            
            if (AppSettings::Instance()->getCopyExternalFiles() == true)
            {
                
                //search through all pads looking for audio files that need copying with the preset
                for (int i = 0; i <= 47; i++)
                {
                    //look for looper audio files
                    if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                    {
                        String newFileName(presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("looperAudioFilePath"));
                        
                        if (newFileName != String::empty) //if there is 'something'
                        {
                            File originalFile;
                            
                            //Need to check here if the saved file path is just file name, which 
                            //means it exists in the audio files directory and needs copying.
                            //else, nothing needs to be done.
                            
                            if (File::isAbsolutePath(newFileName) == false)
                            {
                                //get the original audio file in question
                                originalFile = File::getCurrentWorkingDirectory().getFullPathName()+ File::separatorString + newFileName;
                                
                                //create an new file in presets "Audio Files" folder;
                                File newFile (audioFileDirectory.getFullPathName() + File::separatorString + newFileName);
                                
                                if (originalFile.existsAsFile() == true) //if the file exists (which it should)
                                {
                                    if (newFile.existsAsFile() == false) //if it doesn't already exist
                                        originalFile.copyFileTo(newFile);
                                }
                                else
                                {
                                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File not found!", "\"" + newFileName + "\"" + " for Pad " + String(i+1) + " could not be found.");
                                    //do something here so the loaded data is string::empty and NOT the missing audio file name
                                    //can i do this here or does it need to be done in the loadforpreset method?
                                }
                            
                            }
                        }
                    }
                    //look for sequencer audio files
                    else if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                    {
                        for (int row = 0; row <= NO_OF_ROWS-1; row++)
                        {
                            String newFileName(presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row)));
                            
                            if (newFileName != String::empty) //if there is 'something'
                            {
                                File originalFile;
                                
                                //Need to check here if the saved file path is just file name, which 
                                //means it exists in the audio files directory and needs copying.
                                //else, nothing needs to be done.
                                
                                if (File::isAbsolutePath(newFileName) == false)
                                {
                                    //get the original audio file in question
                                    originalFile = File::getCurrentWorkingDirectory().getFullPathName()+ File::separatorString + newFileName;
                                    
                                    //create an new file in presets "Audio Files" folder;
                                    File newFile (audioFileDirectory.getFullPathName() + File::separatorString + newFileName);
                                    
                                    if (originalFile.existsAsFile() == true) //if the file exists (which it should)
                                    {
                                        if (newFile.existsAsFile() == false) //if it doesn't already exist
                                            originalFile.copyFileTo(newFile);
                                    }
                                    else
                                    {
                                        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File not found!", "\"" + newFileName + "\"" + " for Pad " + String(i+1) + " could not be found.");
                                        //do something here so the loaded data is string::empty and NOT the missing audio file name
                                        //can i do this here or does it need to be done in the loadforpreset method?
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }
             
            //----
            
            //----
            //SAVE THE FILE TO DISK
            //----
            savedFile.deleteFile();
            savedFile.create(); //create the file
            
            //can't just save the presetData[presetNumber] xmlelement straight into file, as the tag name needs to be different, so a new xmlelement must be
            //created which imports the child elements from presetData[presetNumber]
            
            //create xmlelement to be saved
            XmlElement *toBeSaved = new XmlElement("PRESET");
            
            //import child elements
            for (int i = 0; i <= 47; i++)
            {
                toBeSaved->addChildElement(new XmlElement(*presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))));
            }
            toBeSaved->addChildElement(new XmlElement(*presetData[presetNumber]->getChildByName("GLOBAL_DATA")));
            
            //save to file
            String xmlDoc = toBeSaved->createDocument(String::empty, false);
            savedFile.appendText(xmlDoc);
            
            delete toBeSaved;
            
            //----
            
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Single Preset Saved", "The preset has been successfully saved to file");
        }
    }
    
}




bool AppDocumentState::loadPresetFromDisk(int presetNumber)
{

    //navigate to app directory
    FileChooser loadFileChooser("Select a .alphapreset file to load...", 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphapreset");
    
    if (loadFileChooser.browseForFileToOpen())
    {
        //get file
        File loadedFile (loadFileChooser.getResult());
        
        //parse file into xml file
        ScopedPointer<XmlElement> loadedXml (XmlDocument::parse(loadedFile));
        
        if (loadedXml != nullptr && loadedXml->hasTagName("PRESET"))
        {
            //clear the xmlelement for the currently selected preset number
            clearPreset(presetNumber);
            
            //put the loaded xml data into the xmlelement for the current preset
            //howcome i need to load each child individually here but not anywhere else (where i just load/save first child and it weird does the same for the others)???
            for ( int i = 0; i <= 47; i++)
            {
                XmlElement* childToInsert = loadedXml->getChildByName("PAD_DATA_"+String(i));
                presetData[presetNumber]->addChildElement(childToInsert);
                loadedXml->removeChildElement(childToInsert, false);
            }
            XmlElement* childToInsert = loadedXml->getChildByName("GLOBAL_DATA");
            presetData[presetNumber]->addChildElement(childToInsert);
            
            //remove presetData childelement from loadedXml so it isn't deleted when loadedXml goes out of scope!
            loadedXml->removeChildElement (childToInsert, false);
            
            //------------
            if (AppSettings::Instance()->getCopyExternalFiles() == true)
            {
                
                //get the folder that holds the projects audio files
                File audioFileDirectory = (loadedFile.getParentDirectory().getFullPathName() + File::separatorString + "Audio Files");
                
                //copy contents of audioFileDirectory into the working directory's Audio Files folder
                //This must be done by searching through the loaded data for audio file names and copying the files individually if they exist
                for (int i = 0; i <= 47; i++)
                {
                    //look for looper audio files
                    if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                    {
                        String newFileName(presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("looperAudioFilePath"));
                        
                        if (newFileName != String::empty) //if there is 'something'
                        {
                            File originalFile(File::nonexistent);
                            File newFile(File::nonexistent);
                            
                            //Need to check here if the saved file path is just the file
                            //name or the full path, and handle the situation appropriatly
                            
                            if (File::isAbsolutePath(newFileName) == false)
                            {
                                //File should be an internal file
                                originalFile = audioFileDirectory.getFullPathName()+ File::separatorString + newFileName;
                                newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + newFileName;
                            }
                            
                            else if (File::isAbsolutePath(newFileName) == true)
                            {
                                //file is an external file.
                                originalFile = newFileName;
                                newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + originalFile.getFileName();
                                
                                //re-set the path name within presetData from the full path name to just the file name
                                presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("looperAudioFilePath", newFile.getFileName());
                            }
                            
                            
                            if (originalFile.existsAsFile() == true) //if the file exists (which it should)
                            {
                                if (newFile.existsAsFile() == false) //if it doesn't already exist
                                {
                                    originalFile.copyFileTo(newFile); 
                                    std::cout << "Copying audio file from loaded preset's dir to current working dir!\n";
                                }
                            }
                            else
                            {
                                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File not found!", "\"" + newFileName + "\"" + " for Pad " + String(i+1) + " could not be found.");
                                //do something here so the loaded data is string::empty and NOT the missing audio file name
                            }
                        }
                    }
                    
                    
                    
                    //look for sequencer audio files
                    else if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                    {
                        for (int row = 0; row <= NO_OF_ROWS-1; row++)
                        {
                            
                            String newFileName(presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row)));
                            
                            if (newFileName != String::empty) //if there is 'something'
                            {
                                File originalFile(File::nonexistent);
                                File newFile(File::nonexistent);
                                
                                //Need to check here if the saved file path is just the file
                                //name or the full path, and handle the situation appropriatly
                                
                                if (File::isAbsolutePath(newFileName) == false)
                                {
                                    //File should be an internal file
                                    originalFile = audioFileDirectory.getFullPathName()+ File::separatorString + newFileName;
                                    newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + newFileName;
                                }
                                
                                else if (File::isAbsolutePath(newFileName) == true)
                                {
                                    //file is an external file.
                                    originalFile = newFileName;
                                    newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + originalFile.getFileName();
                                    
                                    //re-set the path name within presetData from the full path name to just the file name
                                    presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("sequencerSamplesAudioFilePath"+String(row), newFile.getFileName());
                                }
                                
                                
                                if (originalFile.existsAsFile() == true) //if the file exists (which it should)
                                {
                                    if (newFile.existsAsFile() == false) //if it doesn't already exist
                                    {
                                        originalFile.copyFileTo(newFile); 
                                        std::cout << "Copying audio file from loaded preset's dir to current working dir!\n";
                                    }
                                }
                                else
                                {
                                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File not found!", "\"" + newFileName + "\"" + " for Pad " + String(i+1) + " could not be found.");
                                    //do something here so the loaded data is string::empty and NOT the missing audio file name
                                }
                                
                            }
                        }
                    }
                }
            }
            else if (AppSettings::Instance()->getCopyExternalFiles() == false)
            {
                //This else if statement is needed in case the option to copy 
                //external files is currently false when importing presets
                //which have their audio files stored in the directory 
                //(and hence only the file NAME (not path) is saved), as the audio files
                //will not be found once in the new project. The names need 
                //to be converted from just the file name to the full path to work
                
                //get the folder that holds the projects audio files
                File audioFileDirectory = (loadedFile.getParentDirectory().getFullPathName() + File::separatorString + "Audio Files");
                
                for (int i = 0; i <= 47; i++)
                {
                    //look for looper audio files
                    if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                    {
                        String newFileString(presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("looperAudioFilePath"));
                        
                        if (newFileString != String::empty) //if there is 'something'
                        {
    
                            if (File::isAbsolutePath(newFileString) == false)
                            {
                                //File should be an internal file
                                File newFile(audioFileDirectory.getFullPathName()+ File::separatorString + newFileString); //should be a String?
                                
                                //set the saved file name to be the full path...
                                presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("looperAudioFilePath", newFile.getFullPathName()); 
                            }
                        }
                    }
                    
                    //look for sequencer audio files
                    else if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                    {
                        for (int row = 0; row <= NO_OF_ROWS-1; row++)
                        {
                            
                            String newFileString(presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row)));
                            if (newFileString != String::empty) //if there is 'something'
                            {
                                //if the string saved is just the file name...
                                if (File::isAbsolutePath(newFileString) == false)
                                {
                                    //... get the full path name of the file...
                                    File newFile(audioFileDirectory.getFullPathName()+ File::separatorString + newFileString); //should be a String?
                                    //... a re-set the path name within presetData, NOT PadSettings as this data hasn't been loaded
                                    //into PadSettings yet
                                    presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("sequencerSamplesAudioFilePath"+String(row), newFile.getFullPathName());
                                }
                            }
                        }
                    }
                }
                
            }
            
        }
    
        return true;
    }
    
    else // 'cancel' button pressed
    {
        return false;
    }
    
}




void AppDocumentState::saveProjectSettings()
{
    //reset/clear XmlElement.
    if (projectData != nullptr) //if there is data in the preset's XmlElement
        projectData->removeAllAttributes();

    projectData->setAttribute("copyExternalFiles", AppSettings::Instance()->getCopyExternalFiles());
    
}

void AppDocumentState::loadProjectSettings()
{
    if (projectData->hasAttribute("copyExternalFiles") == true)
        AppSettings::Instance()->setCopyExternalFiles(projectData->getIntAttribute("copyExternalFiles"));
    else
        AppSettings::Instance()->setCopyExternalFiles(true); //default value
    
}



void AppDocumentState::saveToPreset (int presetNumber)
{
    //reset/clear XmlElement.
    clearPreset(presetNumber);
    
    //===global settings===
    XmlElement *globalData  = new XmlElement ("GLOBAL_DATA");
    globalData->setAttribute("globalGain", AppSettings::Instance()->getGlobalGain());
    globalData->setAttribute("globalPan", AppSettings::Instance()->getGlobalPan());
    
    globalData->setAttribute("globalTempo", AppSettings::Instance()->getGlobalTempo());
    globalData->setAttribute("quantisationValue", AppSettings::Instance()->getQuantisationValue());
    globalData->setAttribute("beatsPerBar", AppSettings::Instance()->getBeatsPerBar());
    globalData->setAttribute("autoStartClock", AppSettings::Instance()->getAutoStartClock());
    for (int i = 0; i <= 5; i++)
    {
        globalData->setAttribute("looperChannelMode" + String(i), AppSettings::Instance()->getLooperChannelMode(i));
        globalData->setAttribute("sequencerChannelMode" + String(i), AppSettings::Instance()->getSequencerChannelMode(i));
    }
    
    
    //===pad settings (pad number = i)
    for (int i = 0; i <= 47; i++)
    {
        //create a 'temp' XmlElement to store data for a single pad
        XmlElement *padData  = new XmlElement ("PAD_DATA_" + String(i));
        
        //put data into this the temp XmlElement
        padData->setAttribute("mode", PAD_SETTINGS->getMode());
        padData->setAttribute("pressureSensitivityMode", PAD_SETTINGS->getPressureSensitivityMode());
        
        //only save whats necessary
        if (PAD_SETTINGS->getMode() == 1) //midi mode
        {
            padData->setAttribute("midiNote", PAD_SETTINGS->getMidiNote());
            padData->setAttribute("midiVelocity", PAD_SETTINGS->getMidiVelocity());
            padData->setAttribute("midiChannel", PAD_SETTINGS->getMidiChannel());
            padData->setAttribute("midiMinPressureRange", PAD_SETTINGS->getMidiMinPressureRange());
            padData->setAttribute("midiMaxPressureRange", PAD_SETTINGS->getMidiMaxPressureRange());
            padData->setAttribute("midiPressureMode", PAD_SETTINGS->getMidiPressureMode());
            padData->setAttribute("midiPressurePlaystate", PAD_SETTINGS->getMidiPressurePlaystate());
            padData->setAttribute("midiPressureStatus", PAD_SETTINGS->getMidiPressureStatus());
            padData->setAttribute("midiNoteStatus", PAD_SETTINGS->getMidiNoteStatus());
            padData->setAttribute("midiCcController", PAD_SETTINGS->getMidiCcController());
            padData->setAttribute("midiExclusiveGroup", PAD_SETTINGS->getMidiExclusiveGroup());
            padData->setAttribute("midiTriggerMode", PAD_SETTINGS->getMidiTriggerMode());
        }
        else if (PAD_SETTINGS->getMode() == 2) //looper mode
        {
            
            if (AppSettings::Instance()->getCopyExternalFiles() == true)
            {
                //if project currently allows audio files to be copied to project, only save the file name
                padData->setAttribute("looperAudioFilePath", PAD_SETTINGS->getLooperAudioFilePath().getFileName());
                
            }
            else if (AppSettings::Instance()->getCopyExternalFiles() == false)
            {
                //else save the full pathname
                padData->setAttribute("looperAudioFilePath", PAD_SETTINGS->getLooperAudioFilePath().getFullPathName());
            }
    
            padData->setAttribute("looperPlayState", PAD_SETTINGS->getLooperPlayState());
            padData->setAttribute("looperEffect", PAD_SETTINGS->getLooperEffect());
            padData->setAttribute("looperPan", PAD_SETTINGS->getLooperPan());
            padData->setAttribute("looperGain", PAD_SETTINGS->getLooperGain());
            padData->setAttribute("looperChannel", PAD_SETTINGS->getLooperChannel());
            padData->setAttribute("looperTriggerMode", PAD_SETTINGS->getLooperTriggerMode());
            
            if (PAD_SETTINGS->getLooperEffect() == 1) //Gain and Pan
            {
                padData->setAttribute("looperFxGainPanGain", PAD_SETTINGS->getLooperFxGainPanGain());
                padData->setAttribute("looperFxGainPanPan", PAD_SETTINGS->getLooperFxGainPanPan());
                padData->setAttribute("looperFxGainPanAlphaTouch", PAD_SETTINGS->getLooperFxGainPanAlphaTouch());
                padData->setAttribute("looperFxGainPanAtReverse", PAD_SETTINGS->getLooperFxGainPanAtReverse());
                padData->setAttribute("looperFxGainPanAtIntensity", PAD_SETTINGS->getLooperFxGainPanAtIntensity());
            }
            else if (PAD_SETTINGS->getLooperEffect() == 2) //LPF
            {
                padData->setAttribute("looperFxLpfMix", PAD_SETTINGS->getLooperFxLpfMix());
                padData->setAttribute("looperFxLpfFreq", PAD_SETTINGS->getLooperFxLpfFreq());
                padData->setAttribute("looperFxLpfBandwidth", PAD_SETTINGS->getLooperFxLpfBandwidth());
                padData->setAttribute("looperFxLpfAlphaTouch", PAD_SETTINGS->getLooperFxLpfAlphaTouch());
                padData->setAttribute("looperFxLpfAtReverse", PAD_SETTINGS->getLooperFxLpfAtReverse());
                padData->setAttribute("looperFxLpfAtIntensity", PAD_SETTINGS->getLooperFxLpfAtIntensity());
            }
            else if (PAD_SETTINGS->getLooperEffect() == 3) //HPF
            {
                padData->setAttribute("looperFxHpfMix", PAD_SETTINGS->getLooperFxHpfMix());
                padData->setAttribute("looperFxHpfFreq", PAD_SETTINGS->getLooperFxHpfFreq());
                padData->setAttribute("looperFxHpfBandwidth", PAD_SETTINGS->getLooperFxHpfBandwidth());
                padData->setAttribute("looperFxHpfAlphaTouch", PAD_SETTINGS->getLooperFxHpfAlphaTouch());
                padData->setAttribute("looperFxHpfAtReverse", PAD_SETTINGS->getLooperFxHpfAtReverse());
                padData->setAttribute("looperFxHpfAtIntensity", PAD_SETTINGS->getLooperFxHpfAtIntensity());
            }
            else if (PAD_SETTINGS->getLooperEffect() == 4) //BPF
            {
                padData->setAttribute("looperFxBpfMix", PAD_SETTINGS->getLooperFxBpfMix());
                padData->setAttribute("looperFxBpfFreq", PAD_SETTINGS->getLooperFxBpfFreq());
                padData->setAttribute("looperFxBpfBandwidth", PAD_SETTINGS->getLooperFxBpfBandwidth());
                padData->setAttribute("looperFxBpfAlphaTouch", PAD_SETTINGS->getLooperFxBpfAlphaTouch());
                padData->setAttribute("looperFxBpfAtReverse", PAD_SETTINGS->getLooperFxBpfAtReverse());
                padData->setAttribute("looperFxBpfAtIntensity", PAD_SETTINGS->getLooperFxBpfAtIntensity());
            }
            else if (PAD_SETTINGS->getLooperEffect() == 6) //Delay
            {
                padData->setAttribute("looperFxDelayMix", PAD_SETTINGS->getLooperFxDelayMix());
                padData->setAttribute("looperFxDelayTime", PAD_SETTINGS->getLooperFxDelayTime());
                padData->setAttribute("looperFxDelayFeedback", PAD_SETTINGS->getLooperFxDelayFeedback());
                padData->setAttribute("looperFxDelayLpfFreq", PAD_SETTINGS->getLooperFxDelayLpfFreq());
                padData->setAttribute("looperFxDelayHpfFreq", PAD_SETTINGS->getLooperFxDelayHpfFreq());
                padData->setAttribute("looperFxDelaySync", PAD_SETTINGS->getLooperFxDelaySync());
                padData->setAttribute("looperFxDelayTimeMenu", PAD_SETTINGS->getLooperFxDelayTimeMenu());
                padData->setAttribute("looperFxDelayAlphaTouch", PAD_SETTINGS->getLooperFxDelayAlphaTouch());
                padData->setAttribute("looperFxDelayAtReverse", PAD_SETTINGS->getLooperFxDelayAtReverse());
                padData->setAttribute("looperFxDelayAtIntensity", PAD_SETTINGS->getLooperFxDelayAtIntensity());
            }
            else if (PAD_SETTINGS->getLooperEffect() == 7) //Reverb
            {
                padData->setAttribute("looperFxReverbMix", PAD_SETTINGS->getLooperFxReverbMix());
                padData->setAttribute("looperFxReverbRoomSize", PAD_SETTINGS->getLooperFxReverbRoomSize());
                padData->setAttribute("looperFxReverbDamping", PAD_SETTINGS->getLooperFxReverbDamping());
                padData->setAttribute("looperFxReverbWidth", PAD_SETTINGS->getLooperFxReverbWidth());
                padData->setAttribute("looperFxReverbFreezeMode", PAD_SETTINGS->getLooperFxReverbFreezeMode());
                padData->setAttribute("looperFxReverbAlphaTouch", PAD_SETTINGS->getLooperFxReverbAlphaTouch());
                padData->setAttribute("looperFxReverbAtReverse", PAD_SETTINGS->getLooperFxReverbAtReverse());
                padData->setAttribute("looperFxReverbAtIntensity", PAD_SETTINGS->getLooperFxReverbAtIntensity());
            }
            else if (PAD_SETTINGS->getLooperEffect() == 9) //Flanger
            {
                padData->setAttribute("looperFxFlangerMix", PAD_SETTINGS->getLooperFxFlangerMix());
                padData->setAttribute("looperFxFlangerRate", PAD_SETTINGS->getLooperFxFlangerRate());
                padData->setAttribute("looperFxFlangerFeedback", PAD_SETTINGS->getLooperFxFlangerFeedback());
                padData->setAttribute("looperFxFlangerIntensity", PAD_SETTINGS->getLooperFxFlangerIntensity());
                padData->setAttribute("looperFxFlangerRateMenu", PAD_SETTINGS->getLooperFxFlangerRateMenu());
                padData->setAttribute("looperFxFlangerSync", PAD_SETTINGS->getLooperFxFlangerSync());
                padData->setAttribute("looperFxFlangerAlphaTouch", PAD_SETTINGS->getLooperFxFlangerAlphaTouch());
                padData->setAttribute("looperFxFlangerAtReverse", PAD_SETTINGS->getLooperFxFlangerAtReverse());
                padData->setAttribute("looperFxFlangerAtIntensity", PAD_SETTINGS->getLooperFxFlangerAtIntensity());
            }
            else if (PAD_SETTINGS->getLooperEffect() == 10) //Tremolo
            {
                padData->setAttribute("looperFxTremoloDepth", PAD_SETTINGS->getLooperFxTremoloDepth());
                padData->setAttribute("looperFxTremoloRate", PAD_SETTINGS->getLooperFxTremoloRate());
                padData->setAttribute("looperFxTremoloShape", PAD_SETTINGS->getLooperFxTremoloShape());
                padData->setAttribute("looperFxTremoloSync", PAD_SETTINGS->getLooperFxTremoloSync());
                padData->setAttribute("looperFxTremoloRateMenu", PAD_SETTINGS->getLooperFxTremoloRateMenu());
                padData->setAttribute("looperFxTremoloAlphaTouch", PAD_SETTINGS->getLooperFxTremoloAlphaTouch());
                padData->setAttribute("looperFxTremoloAtReverse", PAD_SETTINGS->getLooperFxTremoloAtReverse());
                padData->setAttribute("looperFxTremoloAtIntensity", PAD_SETTINGS->getLooperFxTremoloAtIntensity());
            }
            
        }
        else if (PAD_SETTINGS->getMode() == 3) //sequencer mode
        {
            padData->setAttribute("sequencerMode", PAD_SETTINGS->getSequencerMode());
            for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
            {
                padData->setAttribute("newSequencerData"+String(seq), PAD_SETTINGS->getSequencerDataString(seq));
            }
            
            padData->setAttribute("sequencerNumberOfSequences", PAD_SETTINGS->getSequencerNumberOfSequences());
            padData->setAttribute("sequencerPlayState", PAD_SETTINGS->getSequencerPlayState());
            padData->setAttribute("sequencerChannel", PAD_SETTINGS->getSequencerChannel());
            padData->setAttribute("sequencerLength", PAD_SETTINGS->getSequencerLength());
            padData->setAttribute("sequencerTriggerMode", PAD_SETTINGS->getSequencerTriggerMode());
            padData->setAttribute("sequencerRelativeTempoMode", PAD_SETTINGS->getSequencerRelativeTempoMode());
            
            if (PAD_SETTINGS->getSequencerMode() == 1) //sequencer midi mode
            {
                for (int row = 0; row <= NO_OF_ROWS-1; row++)
                {
                    padData->setAttribute("sequencerMidiNote"+String(row), PAD_SETTINGS->getSequencerMidiNote(row));
                }
                padData->setAttribute("sequencerMidiVelocity", PAD_SETTINGS->getSequencerMidiVelocity());
                padData->setAttribute("sequencerMidiChannel", PAD_SETTINGS->getSequencerMidiChannel());
                padData->setAttribute("sequencerMidiNoteLength", PAD_SETTINGS->getSequencerMidiNoteLength());
            }
            else if (PAD_SETTINGS->getSequencerMode() == 2) //sequencer samples mode
            {
                for (int row = 0; row <= NO_OF_ROWS-1; row++)
                {
                    if (AppSettings::Instance()->getCopyExternalFiles() == true)
                    {
                        //if project currently allows external audio files to be copied to project, only save the file name
                        padData->setAttribute("sequencerSamplesAudioFilePath"+String(row), PAD_SETTINGS->getSequencerSamplesAudioFilePath(row).getFileName());
                    }
                    else
                    {
                        //else save the full pathname
                        padData->setAttribute("sequencerSamplesAudioFilePath"+String(row), PAD_SETTINGS->getSequencerSamplesAudioFilePath(row).getFullPathName());
                    }
                }
                padData->setAttribute("sequencerPan", PAD_SETTINGS->getSequencerPan());
                padData->setAttribute("sequencerGain", PAD_SETTINGS->getSequencerGain());
            }
        }
        
        else if (PAD_SETTINGS->getMode() == 4) //controller mode
        {
            padData->setAttribute("controllerControl", PAD_SETTINGS->getControllerControl());
            padData->setAttribute("controllerPresetNumber", PAD_SETTINGS->getControllerPresentNumber());
            padData->setAttribute("controllerOscIpAddress", PAD_SETTINGS->getControllerOscIpAddress());
            padData->setAttribute("controllerOscPortNumber", PAD_SETTINGS->getControllerOscPort());
            padData->setAttribute("controllerMidiProgramChangeNumber", PAD_SETTINGS->getControllerMidiProgramChangeNumber());
            padData->setAttribute("controllerMidiProgramChangeChannel", PAD_SETTINGS->getControllerMidiProgramChangeChannel());
        }
        
        //add temp xmlElement as a child element of the main preset XmlElement
        presetData[presetNumber]->addChildElement(new XmlElement(*padData));
        
        delete padData;
    }
    
    //add globalData as child element.
    //WHY DO I HAVE TO DO THIS HERE AND NOT BEFORE DOING THE PADDATA STUFF?
    //caused hours of confusion and crashing if this was done first
    presetData[presetNumber]->addChildElement(new XmlElement(*globalData));
    
    delete globalData;
}




void AppDocumentState::loadFromPreset (int presetNumber)
{
    if (presetData[presetNumber] != nullptr && presetData[presetNumber]->hasTagName("PRESET_"+String(presetNumber)))
    {
        
        //===global settings===
        XmlElement *globalData = new XmlElement(*presetData[presetNumber]->getChildByName("GLOBAL_DATA"));
        
        AppSettings::Instance()->setGlobalGain(globalData->getDoubleAttribute("globalGain"));
        AppSettings::Instance()->setGlobalPan(globalData->getDoubleAttribute("globalPan"));
        
        AppSettings::Instance()->setGlobalTempo(globalData->getDoubleAttribute("globalTempo"));
        if (globalData->hasAttribute("quantisationValue") == true)
            AppSettings::Instance()->setQuantisationValue(globalData->getIntAttribute("quantisationValue"));
        if (globalData->hasAttribute("beatsPerBar") == true)
            AppSettings::Instance()->setBeatsPerBar(globalData->getIntAttribute("beatsPerBar"));
        if (globalData->hasAttribute("autoStartClock") == true)
            AppSettings::Instance()->setAutoStartClock(globalData->getIntAttribute("autoStartClock"));
        
        
        for (int i = 0; i <= 5; i++)
        {
            AppSettings::Instance()->setLooperChannelMode(i, globalData->getIntAttribute("looperChannelMode"+String(i)));
            AppSettings::Instance()->setSequencerChannelMode(i, globalData->getIntAttribute("sequencerChannelMode"+String(i)));
        }
        
        
        delete globalData;
        
        //===pad settings (pad number = i)
        for (int i = 0; i <= 47; i++)
        {
            
            XmlElement *padData = new XmlElement(*presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))); //creates a deep copy of presetData, not just a copy of the pointer
            
            PAD_SETTINGS->setMode(padData->getIntAttribute("mode"));
            
            if (padData->hasAttribute("pressureSensitivityMode") == true) //if it exists (might not in older files)
                PAD_SETTINGS->setPressureSensitivityMode(padData->getIntAttribute("pressureSensitivityMode"));
            
            //only load needed data to reduce loading times and CPU usage, plus
            //can not load settings into seq and looper modes where the pads player objects don't exist (yet)
            
            //midi mode
            if (padData->getIntAttribute("mode") == 1)
            {
                PAD_SETTINGS->setMidiNote(padData->getIntAttribute("midiNote"));
                PAD_SETTINGS->setMidiVelocity(padData->getIntAttribute("midiVelocity"));
                PAD_SETTINGS->setMidiChannel(padData->getIntAttribute("midiChannel"));
                PAD_SETTINGS->setMidiMinPressureRange(padData->getIntAttribute("midiMinPressureRange"));
                PAD_SETTINGS->setMidiMaxPressureRange(padData->getIntAttribute("midiMaxPressureRange"));
                PAD_SETTINGS->setMidiPressureMode(padData->getIntAttribute("midiPressureMode"));
                PAD_SETTINGS->setMidiPressurePlayState(padData->getIntAttribute("midiPressurePlaystate"));
                PAD_SETTINGS->setMidiPressureStatus(padData->getBoolAttribute("midiPressureStatus"));
                PAD_SETTINGS->setMidiNoteStatus(padData->getBoolAttribute("midiNoteStatus"));
                PAD_SETTINGS->setMidiCcController(padData->getIntAttribute("midiCcController"));
                if (padData->hasAttribute("midiExclusiveGroup") == true)
                    PAD_SETTINGS->setMidiExclusiveGroup(padData->getIntAttribute("midiExclusiveGroup"));
                if (padData->hasAttribute("midiTriggerMode") == true)
                    PAD_SETTINGS->setMidiTriggerMode(padData->getIntAttribute("midiTriggerMode"));
            }
            
            //looper mode
            else if (padData->getIntAttribute("mode") == 2)
            {
                File newFile;
                String newFileString = padData->getStringAttribute("looperAudioFilePath");
                
                if (newFileString != String::empty) //to prevent trying to load in a file if there's nothing to load
                {
                    if (File::isAbsolutePath(newFileString) == false)
                    {
                        //check if the saved audio file path is just the file name
                        //if so, get it from the working directory and apply the full pathname to it
                        newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + newFileString;
                    }
                    else if (File::isAbsolutePath(newFileString) == true)
                    {
                        //else, it should be the full path name already
                        newFile = newFileString;
                    }
                    
                    PAD_SETTINGS->setLooperAudioFilePath(newFile);
                }
                else
                    PAD_SETTINGS->setLooperAudioFilePath(File::nonexistent);
                
                PAD_SETTINGS->setLooperPlayState(padData->getIntAttribute("looperPlayState"));
                PAD_SETTINGS->setLooperEffect(padData->getIntAttribute("looperEffect"));
                PAD_SETTINGS->setLooperPan(padData->getDoubleAttribute("looperPan"));
                PAD_SETTINGS->setLooperGain(padData->getDoubleAttribute("looperGain"));
                PAD_SETTINGS->setLooperChannel(padData->getIntAttribute("looperChannel"));
                if (padData->hasAttribute("looperTriggerMode") == true)
                    PAD_SETTINGS->setLooperTriggerMode(padData->getIntAttribute("looperTriggerMode"));
                if (PAD_SETTINGS->getLooperEffect() == 1) //Gain and Pan
                {
                    if (padData->hasAttribute("looperFxGainPanGain") == true) //if data exists
                        //will probably be able to remove this check eventually
                    {
                        PAD_SETTINGS->setLooperFxGainPanGain(padData->getDoubleAttribute("looperFxGainPanGain"));
                        PAD_SETTINGS->setLooperFxGainPanPan(padData->getDoubleAttribute("looperFxGainPanPan"));
                        PAD_SETTINGS->setLooperFxGainPanAlphaTouch(padData->getIntAttribute("looperFxGainPanAlphaTouch"));
                        PAD_SETTINGS->setLooperFxGainPanAtReverse(padData->getIntAttribute("looperFxGainPanAtReverse"));
                        PAD_SETTINGS->setLooperFxGainPanAtIntensity(padData->getDoubleAttribute("looperFxGainPanAtIntensity"));
                    }
                }
                else if (PAD_SETTINGS->getLooperEffect() == 2) //LPF
                {
                    if (padData->hasAttribute("looperFxLpfMix") == true) //if data exists
                        //will probably be able to remove this check eventually
                    {
                        PAD_SETTINGS->setLooperFxLpfMix(padData->getDoubleAttribute("looperFxLpfMix"));
                        PAD_SETTINGS->setLooperFxLpfFreq(padData->getDoubleAttribute("looperFxLpfFreq"));
                        PAD_SETTINGS->setLooperFxLpfBandwidth(padData->getDoubleAttribute("looperFxLpfBandwidth"));
                        PAD_SETTINGS->setLooperFxLpfAlphaTouch(padData->getIntAttribute("looperFxLpfAlphaTouch"));
                        PAD_SETTINGS->setLooperFxLpfAtReverse(padData->getIntAttribute("looperFxLpfAtReverse"));
                        PAD_SETTINGS->setLooperFxLpfAtIntensity(padData->getDoubleAttribute("looperFxLpfAtIntensity"));
                    }
                }
                else if (PAD_SETTINGS->getLooperEffect() == 3) //HPF
                {
                    if (padData->hasAttribute("looperFxHpfMix") == true) //if data exists
                        //will probably be able to remove this check eventually
                    {
                        PAD_SETTINGS->setLooperFxHpfMix(padData->getDoubleAttribute("looperFxHpfMix"));
                        PAD_SETTINGS->setLooperFxHpfFreq(padData->getDoubleAttribute("looperFxHpfFreq"));
                        PAD_SETTINGS->setLooperFxHpfBandwidth(padData->getDoubleAttribute("looperFxHpfBandwidth"));
                        PAD_SETTINGS->setLooperFxHpfAlphaTouch(padData->getIntAttribute("looperFxHpfAlphaTouch"));
                        PAD_SETTINGS->setLooperFxHpfAtReverse(padData->getIntAttribute("looperFxHpfAtReverse"));
                        PAD_SETTINGS->setLooperFxHpfAtIntensity(padData->getDoubleAttribute("looperFxHpfAtIntensity"));
                    }
                }
                else if (PAD_SETTINGS->getLooperEffect() == 4) //BPF
                {
                    if (padData->hasAttribute("looperFxBpfMix") == true) //if data exists
                        //will probably be able to remove this check eventually
                    {
                        PAD_SETTINGS->setLooperFxBpfMix(padData->getDoubleAttribute("looperFxBpfMix"));
                        PAD_SETTINGS->setLooperFxBpfFreq(padData->getDoubleAttribute("looperFxBpfFreq"));
                        PAD_SETTINGS->setLooperFxBpfBandwidth(padData->getDoubleAttribute("looperFxBpfBandwidth"));
                        PAD_SETTINGS->setLooperFxBpfAlphaTouch(padData->getIntAttribute("looperFxBpfAlphaTouch"));
                        PAD_SETTINGS->setLooperFxBpfAtReverse(padData->getIntAttribute("looperFxBpfAtReverse"));
                        PAD_SETTINGS->setLooperFxBpfAtIntensity(padData->getDoubleAttribute("looperFxBpfAtIntensity"));
                    }
                }
                else if (PAD_SETTINGS->getLooperEffect() == 6) //Delay
                {
                    if (padData->hasAttribute("looperFxDelayMix") == true) //if data exists
                        //will probably be able to remove this check eventually
                    {
                        PAD_SETTINGS->setLooperFxDelayMix(padData->getDoubleAttribute("looperFxDelayMix"));
                        PAD_SETTINGS->setLooperFxDelayTime(padData->getDoubleAttribute("looperFxDelayTime"));
                        PAD_SETTINGS->setLooperFxDelayFeedback(padData->getDoubleAttribute("looperFxDelayFeedback"));
                        PAD_SETTINGS->setLooperFxDelayLpfFreq(padData->getDoubleAttribute("looperFxDelayLpfFreq"));
                        PAD_SETTINGS->setLooperFxDelayHpfFreq(padData->getDoubleAttribute("looperFxDelayHpfFreq"));
                        PAD_SETTINGS->setLooperFxDelaySync(padData->getIntAttribute("looperFxDelaySync"));
                        PAD_SETTINGS->setLooperFxDelayTimeMenu(padData->getIntAttribute("looperFxDelayTimeMenu"));
                        PAD_SETTINGS->setLooperFxDelayAlphaTouch(padData->getIntAttribute("looperFxDelayAlphaTouch"));
                        PAD_SETTINGS->setLooperFxDelayAtReverse(padData->getIntAttribute("looperFxDelayAtReverse"));
                        PAD_SETTINGS->setLooperFxDelayAtIntensity(padData->getDoubleAttribute("looperFxDelayAtIntensity"));
                        
                    }
                }
                else if (PAD_SETTINGS->getLooperEffect() == 7) //Reverb
                {
                    if (padData->hasAttribute("looperFxReverbMix") == true) //if data exists
                        //will probably be able to remove this check eventually
                    {
                        PAD_SETTINGS->setLooperFxReverbMix(padData->getDoubleAttribute("looperFxReverbMix"));
                        PAD_SETTINGS->setLooperFxReverbRoomSize(padData->getDoubleAttribute("looperFxReverbRoomSize"));
                        PAD_SETTINGS->setLooperFxReverbDamping(padData->getDoubleAttribute("looperFxReverbDamping"));
                        PAD_SETTINGS->setLooperFxReverbWidth(padData->getDoubleAttribute("looperFxReverbWidth"));
                        PAD_SETTINGS->setLooperFxReverbFreezeMode(padData->getDoubleAttribute("looperFxReverbFreezeMode"));
                        PAD_SETTINGS->setLooperFxReverbAlphaTouch(padData->getIntAttribute("looperFxReverbAlphaTouch"));
                        PAD_SETTINGS->setLooperFxReverbAtReverse(padData->getIntAttribute("looperFxReverbAtReverse"));
                        PAD_SETTINGS->setLooperFxReverbAtIntensity(padData->getDoubleAttribute("looperFxReverbAtIntensity"));
                    }
                }
                else if (PAD_SETTINGS->getLooperEffect() == 9) //Flanger
                {
                    if (padData->hasAttribute("looperFxFlangerMix") == true) //if data exists
                        //will probably be able to remove this check eventually
                    {
                        PAD_SETTINGS->setLooperFxFlangerMix(padData->getDoubleAttribute("looperFxFlangerMix"));
                        PAD_SETTINGS->setLooperFxFlangerRate(padData->getDoubleAttribute("looperFxFlangerRate"));
                        PAD_SETTINGS->setLooperFxFlangerFeedback(padData->getDoubleAttribute("looperFxFlangerFeedback"));
                        PAD_SETTINGS->setLooperFxFlangerIntensity(padData->getDoubleAttribute("looperFxFlangerIntensity"));
                        PAD_SETTINGS->setLooperFxFlangerSync(padData->getIntAttribute("looperFxFlangerSync"));
                        PAD_SETTINGS->setLooperFxFlangerRateMenu(padData->getIntAttribute("looperFxFlangerRateMenu"));
                        PAD_SETTINGS->setLooperFxFlangerAlphaTouch(padData->getIntAttribute("looperFxFlangerAlphaTouch"));
                        PAD_SETTINGS->setLooperFxFlangerAtReverse(padData->getIntAttribute("looperFxFlangerAtReverse"));
                        PAD_SETTINGS->setLooperFxFlangerAtIntensity(padData->getDoubleAttribute("looperFxFlangerAtIntensity"));
                        
                    }
                }
                
                else if (PAD_SETTINGS->getLooperEffect() == 10) //Tremolo
                {
                    if (padData->hasAttribute("looperFxTremoloDepth") == true) //if data exists
                        //will probably be able to remove this check eventually
                    {
                        PAD_SETTINGS->setLooperFxTremoloDepth(padData->getDoubleAttribute("looperFxTremoloDepth"));
                        PAD_SETTINGS->setLooperFxTremoloRate(padData->getDoubleAttribute("looperFxTremoloRate"));
                        PAD_SETTINGS->setLooperFxTremoloShape(padData->getIntAttribute("looperFxTremoloShape"));
                        PAD_SETTINGS->setLooperFxTremoloSync(padData->getIntAttribute("looperFxTremoloSync"));
                        PAD_SETTINGS->setLooperFxTremoloRateMenu(padData->getIntAttribute("looperFxTremoloRateMenu"));
                        PAD_SETTINGS->setLooperFxTremoloAlphaTouch(padData->getIntAttribute("looperFxTremoloAlphaTouch"));
                        PAD_SETTINGS->setLooperFxTremoloAtReverse(padData->getIntAttribute("looperFxTremoloAtReverse"));
                        PAD_SETTINGS->setLooperFxTremoloAtIntensity(padData->getDoubleAttribute("looperFxTremoloAtIntensity"));
                        
                    }
                }
            }
            
            //sequencer mode
            else if (padData->getIntAttribute("mode") == 3)
            {
                PAD_SETTINGS->setSequencerMode(padData->getIntAttribute("sequencerMode"));
                
                if (padData->hasAttribute("newSequencerData0") == true) //new seq data format (0-127)
                    {
                        for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
                        {
                            PAD_SETTINGS->stringToSeqData(padData->getStringAttribute("newSequencerData"+String(seq)), seq);
                        }
                    }
                
                else if (padData->hasAttribute("sequencerData0") == true) //old seq data format (0-1)
                    {
                        for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
                        {
                            //convert any '1s' in the string to '110'
                            PAD_SETTINGS->stringToSeqDataFormatConversion(padData->getStringAttribute("sequencerData"+String(seq)), seq);
                        }
                    }
                
            
                    
                PAD_SETTINGS->setSequencerNumberOfSequences(padData->getIntAttribute("sequencerNumberOfSequences"));
                PAD_SETTINGS->setSequencerPlayState(padData->getIntAttribute("sequencerPlayState"));
                PAD_SETTINGS->setSequencerChannel(padData->getIntAttribute("sequencerChannel"));
                PAD_SETTINGS->setSequencerLength(padData->getIntAttribute("sequencerLength"));
                if (padData->hasAttribute("sequencerTriggerMode") == true)
                    PAD_SETTINGS->setSequencerTriggerMode(padData->getIntAttribute("sequencerTriggerMode"));
                if (padData->hasAttribute("sequencerRelativeTempoMode") == true)
                    PAD_SETTINGS->setSequencerRelativeTempoMode(padData->getIntAttribute("sequencerRelativeTempoMode"));
                
                //sequencer midi mode
                if (padData->getIntAttribute("sequencerMode") == 1)
                {
                    for (int row = 0; row <= NO_OF_ROWS-1; row++)
                    {
                        PAD_SETTINGS->setSequencerMidiNote(padData->getIntAttribute("sequencerMidiNote"+String(row)), row);
                    }
                    PAD_SETTINGS->setSequencerMidiVelocity(padData->getIntAttribute("sequencerMidiVelocity"));
                    PAD_SETTINGS->setSequencerMidiChannel(padData->getIntAttribute("sequencerMidiChannel"));
                    PAD_SETTINGS->setSequencerMidiNoteLength(padData->getIntAttribute("sequencerMidiNoteLength"));
                }
                
                //sequencer samples mode
                else if (padData->getIntAttribute("sequencerMode") == 2)
                {
                    for (int row = 0; row <= NO_OF_ROWS-1; row++)
                    {
                        File newFile;
                        String newFileString = padData->getStringAttribute("sequencerSamplesAudioFilePath"+String(row));
                        
                        if (newFileString != String::empty) //to prevent trying to load in a file if there's nothing to load
                        {
                            if (File::isAbsolutePath(newFileString) == false)
                            {
                                //check if the saved audio file path is just the file name
                                //if so, get it from the working directory and apply the full pathname to it
                                newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + newFileString;
                            }
                            else
                            {
                                //else, it should be the full path name already
                                newFile = newFileString;
                            }
                            
                            PAD_SETTINGS->setSequencerSamplesAudioFilePath(newFile, row);
                        }
                        else
                            PAD_SETTINGS->setSequencerSamplesAudioFilePath(File::nonexistent, row); 
                    }
                    
                    PAD_SETTINGS->setSequencerPan(padData->getDoubleAttribute("sequencerPan"));
                    PAD_SETTINGS->setSequencerGain(padData->getDoubleAttribute("sequencerGain"));
                }
            }
            
            //controller mode
            else if (padData->getIntAttribute("mode") == 4)
            {
                PAD_SETTINGS->setControllerControl(padData->getIntAttribute("controllerControl"));
                PAD_SETTINGS->setControllerPresetNumber(padData->getIntAttribute("controllerPresetNumber"));
                
                if (padData->hasAttribute("controllerOscIpAddress") == true)
                        PAD_SETTINGS->setControllerOscIpAddress(padData->getStringAttribute("controllerOscIpAddress"));
                if (padData->hasAttribute("controllerOscPortNumber") == true)
                        PAD_SETTINGS->setControllerOscPort(padData->getIntAttribute("controllerOscPortNumber"));
                if (padData->hasAttribute("controllerMidiProgramChangeNumber") == true)
                    PAD_SETTINGS->setControllerMidiProgramChangeNumber(padData->getIntAttribute("controllerMidiProgramChangeNumber"));
                if (padData->hasAttribute("controllerMidiProgramChangeChannel") == true)
                    PAD_SETTINGS->setControllerMidiProgramChangeChannel(padData->getIntAttribute("controllerMidiProgramChangeChannel"));
                
            }
            
            delete padData;
        }
        
    }
    
    //update GUI - calls update() in mainComponent
    notifyObs();
    
    
}


void AppDocumentState::clearPreset (int presetNumber)
{
    
    //reset/clear XmlElement.
    if (presetData[presetNumber] != nullptr) //if there is data in the preset's XmlElement
    {
        presetData[presetNumber]->deleteAllChildElements();
    }
}




void AppDocumentState::saveSequence (int currentlySelectedSeqNumber, int currentlySelectedPad)
{
    //navigate to app directory
    FileChooser saveFileChooser("Create a single sequence file to save...", 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphaseq");
    
    if (saveFileChooser.browseForFileToSave(false))
    {
        //Surely there's a easier way to do the following code? FileBasedDocument
        File savedFile (saveFileChooser.getResult()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".alphaseq"; //append an extension name to the filepath name
        savedFile = stringFile; //set the file to this name
        
        bool overwrite = true; //by default true
        
        //delete the file if it exists &write the new data
        if (savedFile.existsAsFile())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "This File Already Exists!", "Are you sure you want to overwrite this file?");
        }
        
        if (overwrite == true)
        {
            savedFile.deleteFile();
            savedFile.create(); //create the file
            
            XmlElement sequenceDataXml("SEQUENCE_DATA");
            
            //get single sequence data string based on currently selected sequencer number slider value
            sequenceDataXml.setAttribute("sequenceData", PAD_SETTINGS_pad->getSequencerDataString(currentlySelectedSeqNumber));
            
            String xmlDoc = sequenceDataXml.createDocument(String::empty, true);
            savedFile.appendText(xmlDoc);
            
            std::cout << savedFile.getFullPathName() << std::endl;
            
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Single Sequence Saved", "The sequence has been successfully saved to file");
        }
    }
    
}


void AppDocumentState::loadSequence (int currentlySeletedSeqNumber, Array<int> selectedPads_)
{
    //navigate to app directory
    FileChooser loadFileChooser("Select a .alphaseq file to load...", 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphaseq");
    
    if (loadFileChooser.browseForFileToOpen())
    {
        File loadedFile (loadFileChooser.getResult());
        
        XmlElement* xml = XmlDocument::parse(loadedFile);
        if (xml != nullptr && xml->hasTagName("SEQUENCE_DATA"))
        {
            for (int i = 0; i < selectedPads_.size(); i++)
            {
                int padNum = selectedPads_[i];
                //get the saved string and call stringToSeqData() to convert it back int a stream of int's
                PAD_SETTINGS_pads->stringToSeqData(xml->getStringAttribute("sequenceData"), currentlySeletedSeqNumber);
            }
            

        }
        
        delete xml;
        
        //update GUI
        notifyObs();
    }
}




void AppDocumentState::saveSequenceSet(int currentlySelectedPad)
{
    //navigate to app directory
    FileChooser saveFileChooser("Create a sequence set file to save...", 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphaseqset");
    
    if (saveFileChooser.browseForFileToSave(false))
    {
        //Surely there's a easier way to do the following code? FileBasedDocument
        File savedFile (saveFileChooser.getResult()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".alphaseqset"; //append an extension name to the filepath name
        savedFile = stringFile; //set the file to this name
        
        bool overwrite = true; //by default true
        
        //delete the file if it exists &write the new data
        if (savedFile.existsAsFile())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "This File Already Exists!", "Are you sure you want to overwrite this file?");
        }
        
        if (overwrite == true)
        {
            savedFile.deleteFile();
            savedFile.create(); //create the file
            
            XmlElement sequenceDataXml("SEQUENCE_DATA");
            
            //get all sequence data strings 
            for (int i = 0; i <= NO_OF_SEQS-1; i++)
            {
                sequenceDataXml.setAttribute("sequenceData"+String(i), PAD_SETTINGS_pad->getSequencerDataString(i));
            }
            
            String xmlDoc = sequenceDataXml.createDocument(String::empty, true);
            savedFile.appendText(xmlDoc);
            
            std::cout << savedFile.getFullPathName() << std::endl;
            
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Sequence Set Saved", "The sequence set has been successfully saved to file");
        }
    }
}





void AppDocumentState::loadSequenceSet(Array<int> selectedPads_)
{
    //i haven't updated this function yet to allow for old format seq strings to be correctly converted to the new format.
    //it's not hard but will it be really needed? -  since i bet of the few ppl who currently use the software don't save
    //and load individual seq's and seq sets
    
    //navigate to app directory
    FileChooser loadFileChooser("Select a .alphaseqset file to load...", 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphaseqset");
    
    if (loadFileChooser.browseForFileToOpen())
    {
        File loadedFile (loadFileChooser.getResult());
        
        XmlElement* xml = XmlDocument::parse(loadedFile);
        if (xml != nullptr && xml->hasTagName("SEQUENCE_DATA"))
        {
            for (int i = 0; i < selectedPads_.size(); i++)
            {
                int padNum = selectedPads_[i];
                //get the saved string for each of the seq and call stringToSeqData() to convert them back int a stream of int's
                for (int seqNumber = 0; seqNumber <= NO_OF_SEQS-1; seqNumber++)
                {
                    PAD_SETTINGS_pads->stringToSeqData(xml->getStringAttribute("sequenceData"+String(seqNumber)), seqNumber);
                }
            }
            
        }
        
        delete xml;
        
        //update GUI
        notifyObs();
        
    }
}


void AppDocumentState::removeUneededAudioFiles()
{
    
    if (currentProjectFile != File::nonexistent) //if there is currently an open project
    {
        
        bool shouldCleanUp = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Clean Up Project", "This command will go through the current project's Audio Files directory and delete any files which aren't currently being used. Over time this will prevent an excessive build-up of redundant data. It was also reset any unused mode settings to default values. Please note that you can not undo this command!");
        if (shouldCleanUp == true)
        {
            //this function must check all the settings of all the presetData elements,
            //and delete any audio files in the project Audio Files directory that aren't included in these settings.
            //need to move all the currently used audio files somewhere, delete any that are left, and move the used ones back
            //Is is the best way to do it?
            
            //first must save the current settings into the current preset to prevent missing audio files errors once the clean up has been completed.
            //For example, if you imported in a new audio file and then instantly cleaned up without saving the new settings it might delete the current
            //audio file as a reference to it wouldn't be found in any of the presetData elements, so when the clean up is complete the audio file would 
            //now be missing.
            //instead of saving, you could load up the preset data for the current preset which would delete the current settings that havent been saved. What would be more natural?
            saveToPreset(currentlySelectedPreset);
            
            File tempAudioDirectory = File::getCurrentWorkingDirectory().getParentDirectory().getFullPathName() + File::separatorString + "tempDir";
            tempAudioDirectory.createDirectory();
            
            //search through all presets
            for (int presetNumber = 0; presetNumber <= NO_OF_PRESETS-1; presetNumber++)
            {
                
                if (presetData[presetNumber]->getNumChildElements() > 0) //if the preset XmlElement contains data
                {
                    
                    //search through all pads looking for audio files 
                    for (int i = 0; i <= 47; i++)
                    {
                        //look for looper audio files
                        if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                        {
                            String originalFile(presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("looperAudioFilePath"));
                            
                            if (originalFile != String::empty) //if there is 'something'
                            {
                                File audioFile(File::nonexistent);
                                
                                //Look for internal files, which are saved just by just their file name
                                
                                if (File::isAbsolutePath(originalFile) == false)
                                {
                                    //File should be an internal file
                                    audioFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + originalFile;
                                    
                                    if (audioFile.existsAsFile() == true)
                                    {
                                        File newFile = tempAudioDirectory.getFullPathName()+ File::separatorString + originalFile;
                                        if (newFile.existsAsFile() == false) //if it doesn't already exist
                                        {
                                            audioFile.copyFileTo(newFile);//copy the file
                                        }
                                    }
                                    
                                }
                                else
                                {
                                    //external file.
                                    //check to see if the external file is actually stored internally,
                                    //which could be caused by importing files to project, 
                                    //and then selecting 'don't copy' afterwards.
                                    
                                    audioFile = originalFile;
                                    
                                    if (audioFile.isAChildOf(File::getCurrentWorkingDirectory()) == true)
                                    {
                                        originalFile = audioFile.getFileName();
                                        File newFile = tempAudioDirectory.getFullPathName()+ File::separatorString + originalFile;
                                        if (newFile.existsAsFile() == false) //if it doesn't already exist
                                        {
                                            audioFile.copyFileTo(newFile);//copy the file
                                        } 
                                        
                                        //would it be a good idea to 'fix' the saved file path here so that the file
                                        //is now recignised as being internal instead of external?
                                    }
                                }
                            }
                        }
                        //look for sequencer audio files
                        else if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                        {
                            for (int row = 0; row <= NO_OF_ROWS-1; row++)
                            {
                                String originalFile(presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row)));
                                
                                if (originalFile != String::empty) //if there is 'something'
                                {
                                    File audioFile(File::nonexistent);
                                    
                                    //Look for internal files, which are saved just by just their file name
                                    
                                    if (File::isAbsolutePath(originalFile) == false)
                                    {
                                        //File should be an internal file
                                        audioFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + originalFile;
                                        
                                        if (audioFile.existsAsFile() == true)
                                        {
                                            File newFile = tempAudioDirectory.getFullPathName()+ File::separatorString + originalFile;
                                            if (newFile.existsAsFile() == false) //if it doesn't already exist
                                            {
                                                audioFile.copyFileTo(newFile);//copy the file
                                            }
                                        }
                                        
                                    }
                                    else
                                    {
                                        //external file.
                                        //check to see if the external file is actually stored internally,
                                        //which could be caused by importing files to project, 
                                        //and then selecting 'don't copy' afterwards.
                                        
                                        audioFile = originalFile;
                                        
                                        if (audioFile.isAChildOf(File::getCurrentWorkingDirectory()) == true)
                                        {
                                            originalFile = audioFile.getFileName();
                                            File newFile = tempAudioDirectory.getFullPathName()+ File::separatorString + originalFile;
                                            if (newFile.existsAsFile() == false) //if it doesn't already exist
                                            {
                                                audioFile.copyFileTo(newFile);//copy the file
                                            } 
                                            
                                            //would it be a good idea to 'fix' the saved file path here so that the file
                                            //is now recignised as being internal instead of external?
                                        }
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }
            //create a copy of the working dir (Audio Files dir)
            File audioFileDirectory = File::getCurrentWorkingDirectory();
            //delete the current working dir
            File::getCurrentWorkingDirectory().deleteRecursively();
            //rename the temp dir (which should hold all the needed audio files) to 'Audio Files' so it can be used as the working dir when the project is next loaded up
            tempAudioDirectory.moveFileTo(audioFileDirectory);
            
            //set the currentWorkingDirectory
            audioFileDirectory.setAsCurrentWorkingDirectory();
            
            //=====================================================
            //==============NEW - reset unused mode settings=======
            //=====================================================
            /*
             Here, the settings of the modes that aren't being used for each pad are reset to their default values.
             */
            for (int i = 0; i <=47; i++)
            {
                PAD_SETTINGS->resetData(PAD_SETTINGS->getMode());
            }
            //=====================================================
            
            //automatically save the new settings
            shouldDisplayAlertWindow = false;
            saveProject();
            
            AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Project Cleaned Up!", "All redundant files and settings have been deleted and reset.");
        }
    }
    
    else
    {
        AlertWindow::showMessageBox(AlertWindow::InfoIcon, "No project currently open!", "There is no project open to clean up.");
    }
}


void AppDocumentState::importAudioFiles()
{
    //==============================================================================================
    //this function is called if copyExternalFilesSwitch is set to on/true
    //==============================================================================================
    
    saveToPreset(currentlySelectedPreset);
    
    //search through all presets
    for (int presetNumber = 0; presetNumber <= NO_OF_PRESETS-1; presetNumber++)
    {
        if (presetData[presetNumber]->getNumChildElements() > 0) //if the preset XmlElement contains data
        {
            //search through all pads looking for audio files 
            for (int i = 0; i <= 47; i++)
            {
                //look for looper audio files
                if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                {
                    File currentFile;
                    File newFile;
                    String fileName = presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("looperAudioFilePath");
                    
                    if (fileName != String::empty)
                    {
                        //Here, we are looking for external files. If we find one, we need to copy it
                        if (File::isAbsolutePath(fileName) == true)
                        {
                            //if we are looking for external files and the pads file is an external file...
                            currentFile = fileName;
                        }
                        else
                        {
                            //no match...
                            break;
                        }
                        
                        newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + currentFile.getFileName();
                        
                        if (newFile.existsAsFile() == false) //if it doesn't yet exist
                        {
                            //copy the added audio file into the newly created file
                            currentFile.copyFileTo(newFile);
                        }
                        
                        else if (newFile.existsAsFile() == true) //if it already exists 
                        {
                            Array<File> matchingFilesArray;
                            String fileWildCard (newFile.getFileNameWithoutExtension()+"*");
                            bool importedFileNeedsCopying = true;
                            
                            //Find all possible duplicates of the imported file using fileWildCard
                            //and add reference of all possible files to matchingFilesArray.
                            File::getCurrentWorkingDirectory().findChildFiles(matchingFilesArray, 2, false, fileWildCard);
                            
                            for (int i = 0; i < matchingFilesArray.size(); i++)
                            {
                                if (currentFile.hasIdenticalContentTo(matchingFilesArray[i]) == true)
                                {
                                    //if it finds a duplicate, flag that the file doesn't need copying
                                    importedFileNeedsCopying = false;
                                    //set the file
                                    newFile = matchingFilesArray[i];
                                    //break out of the for loop once a duplicate has been found to prevent unnecessary checks
                                    break;
                                }
                            }
                            
                            if (importedFileNeedsCopying == true) 
                            {
                                //if no duplicate was found...
                                //... copy the added file with an appended name
                                newFile = newFile.getNonexistentSibling();
                                currentFile.copyFileTo(newFile); 
                            }
                            
                        }
                        
                        
                        presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("looperAudioFilePath", newFile.getFileName());
                        
                        if (presetNumber == currentlySelectedPreset)
                            AppSettings::Instance()->padSettings[i]->setLooperAudioFilePath(newFile);
                    }
                }
                
                //look for sequencer audio files
                else if (presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                {
                    for (int row = 0; row <= NO_OF_ROWS-1; row++)
                    {
                        File currentFile;
                        File newFile;
                        String fileName = presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row));
                        
                        if (fileName != String::empty)
                        {
                            //Here, we are looking for external files. If we find one, we need to copy it
                            if (File::isAbsolutePath(fileName) == true)
                            {
                                //if we are looking for external files and the pads file is an external file...
                                currentFile = fileName;
                            }
                            else
                            {
                                //no match...
                                break;
                            }
                            
                            newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + currentFile.getFileName();
                            
                            if (newFile.existsAsFile() == false) //if it doesn't yet exist
                            {
                                //copy the added audio file into the newly created file
                                currentFile.copyFileTo(newFile);
                            }
                            
                            else if (newFile.existsAsFile() == true) //if it already exists 
                            {
                                Array<File> matchingFilesArray;
                                String fileWildCard (newFile.getFileNameWithoutExtension()+"*");
                                bool importedFileNeedsCopying = true;
                                
                                //Find all possible duplicates of the imported file using fileWildCard
                                //and add reference of all possible files to matchingFilesArray.
                                File::getCurrentWorkingDirectory().findChildFiles(matchingFilesArray, 2, false, fileWildCard);
                                
                                for (int i = 0; i < matchingFilesArray.size(); i++)
                                {
                                    if (currentFile.hasIdenticalContentTo(matchingFilesArray[i]) == true)
                                    {
                                        //if it finds a duplicate, flag that the file doesn't need copying
                                        importedFileNeedsCopying = false;
                                        //set the file
                                        newFile = matchingFilesArray[i];
                                        //break out of the for loop once a duplicate has been found to prevent unnecessary checks
                                        break;
                                    }
                                }
                                
                                if (importedFileNeedsCopying == true) 
                                {
                                    //if no duplicate was found...
                                    //... copy the added file with an appended name
                                    newFile = newFile.getNonexistentSibling();
                                    currentFile.copyFileTo(newFile); 
                                }
                                
                            }
                            
                            
                            presetData[presetNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("sequencerSamplesAudioFilePath"+String(row), newFile.getFileName());
                            
                            if (presetNumber == currentlySelectedPreset)
                                AppSettings::Instance()->padSettings[i]->setSequencerSamplesAudioFilePath(newFile, row);
                        }
                    }
                }
            }
        }
    }
    
    //loadFromPreset(currentlySelectedPreset);
}

void AppDocumentState::registerRecentFile (const File& file)
{
    RecentlyOpenedFilesList::registerRecentFileNatively (file);
    StoredSettings::getInstance()->recentFiles.addFile (file);
    StoredSettings::getInstance()->flush();
}


void AppDocumentState::setCurrentlySelectedPreset(int value)
{
    currentlySelectedPreset = value;
    std::cout << "Currently Selected Preset = " << currentlySelectedPreset << std::endl;
}


int AppDocumentState::getGuiUpdateFlag()
{
    return guiUpdateFlag;
}
int AppDocumentState::getPresetToUpdate()
{
    return presetToUpdate;
}
int AppDocumentState::getPresetStatus()
{
    return presetStatus;
}
