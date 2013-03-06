//
//  MainMenuModel.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 14/06/2012.
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

#include "MainMenuModel.h"
#include "Common.h"
#include "../File and Settings/StoredSettings.h"

/*
 Below are some strings that need wrapping in the translate() function
 so that the translationfilegenerator app will add them to the trans file,
 as for some reason they're not picked up within the juce library code.
 
 translate("Services")
 translate("Hide AlphaLive")
 translate("Hide Others")
 translate("Show All")
 translate("Quit AlphaLive")
 translate("Quit")
 */


MainMenuModel::MainMenuModel(AppDocumentState &ref)
                        : appDocumentStateRef(ref)
{
    setApplicationCommandManagerToWatch (commandManager);
}

MainMenuModel::~MainMenuModel()
{
    
}

StringArray MainMenuModel::getMenuBarNames()
{

    //OLD METHOD - caused an assertion when trying to display non-standard characters (e.g. japanese characters)
    /*
    //const char* const names[] = { file.toUTF8(), edit.toUTF8(), options.toUTF8(), controls.toUTF8(), help.toUTF8(), 0 };
    const char* const names[] = { "File", "Edit", "Options", "Controls", "Help", 0 };
    return StringArray ((const char**) names);
     */
    
    StringArray names;
    
    String file(translate("File"));
    String edit(translate("Edit"));
    String options(translate("Options"));
    String controls(translate("Controls"));
    String help(translate("Help"));
    
    names.add(file);
    names.add(edit);
    names.add(options);
    names.add(controls);
    names.add(help);
    
    return names;
}

PopupMenu MainMenuModel::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
    PopupMenu menu;
    
    if(topLevelMenuIndex == 0) //File
    {
        menu.addCommandItem (commandManager, CommandIDs::New);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::Open);
        
        PopupMenu recentFiles;
        StoredSettings::getInstance()->recentFiles.createPopupMenuItems (recentFiles, 100, true, true);
        recentFiles.addSeparator();
        recentFiles.addItem(99, translate("Clear Menu"));
        menu.addSubMenu (translate(translate("Open Recent")), recentFiles, StoredSettings::getInstance()->recentFiles.getNumFiles() > 0);
        
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::Save);
        menu.addCommandItem (commandManager, CommandIDs::SaveAs);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::ProjectSettings);
        menu.addCommandItem(commandManager, CommandIDs::CleanUpProject);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::LoadScene);
        menu.addCommandItem(commandManager, CommandIDs::SaveScene);
        #if !JUCE_MAC // add the prefrences and quit to the file menu on non-Mac platforms
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::Preferences);
        menu.addSeparator();
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::quit); //why isn't this appearing?
        #endif
    }
    else if(topLevelMenuIndex == 1) //Edit
    {
        menu.addCommandItem (commandManager, CommandIDs::CopyPadSettings);
        menu.addCommandItem (commandManager, CommandIDs::PastePadSettings);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::ClearScene);
        menu.addCommandItem(commandManager, CommandIDs::ClearAllScenes);
    }
    else if (topLevelMenuIndex == 2) //Options
    {
        menu.addCommandItem (commandManager, CommandIDs::DisableHelpBox);
    }
    else if (topLevelMenuIndex == 3) //Controls
    {
        menu.addCommandItem (commandManager, CommandIDs::KillSwitch);
        menu.addCommandItem (commandManager, CommandIDs::StartStopClock);
    }
    else if (topLevelMenuIndex == 4) //Help
    {
        //menu.addCommandItem (commandManager, CommandIDs::StarterGuide);
        menu.addCommandItem (commandManager, CommandIDs::ReferenceManual);
        
        //add the 'about' on non-mac platforms
        #if !JUCE_MAC
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::About);
        #endif
        
    }
    
    return menu;
}

void MainMenuModel::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
    if (menuItemID >= 100 && menuItemID < 200)
    {
        // open a file from the "recent files" menu
        const File file (StoredSettings::getInstance()->recentFiles.getFile (menuItemID - 100));
        
        //parent->openRecentProject(file);
        appDocumentStateRef.loadProject(false, file);
    }
    else if (menuItemID == 99)
    {
        //clear recent files list
        StoredSettings::getInstance()->recentFiles.clear();
        StoredSettings::getInstance()->flush();
        menuItemsChanged();
    }
    
    //everything else in the menu bar is handled by the command manager../
}
