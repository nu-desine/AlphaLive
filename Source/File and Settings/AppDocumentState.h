//
//  AppDocumentState.h
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

/*
 DESCRIPTION OF PROBLEMS I'VE BEEN HAVING WITH THIS CLASS.
 I've encountered many problems when trying to get loading and saving performances and scenes to work, and here are things to know to make sure things work.
 When creating a copy of one of the sceneData XmlElement objects or adding one as a child eleent to a new element, remember that the
 sceneData objects are pointers which means you must make sure you are creating a new copy/child of the ACTUAL OBJECT, not a copy of the pointer, as
 this new pointer would still point to the original object and then when the copy of the element is deleted it would also delete the orginal element! (double delete exceptions / double deletion asserts)
 So either create a deep copy of the XmlElement or make sure you remove child elements from the new XmlElements if they contain pointers to the original object.
 All this is explained at http://www.rawmaterialsoftware.com/viewtopic.php?f=2&t=8182&p=46208#p46208
 
 I think currently in the code I've done a mixture of the two methods described in the forum post, so this class definetly needs tidying up at some point!!
 
 */

/*
 HOW AUDIO FILES SHOULD BE HANDLED IN THE APPLICATION.
 Current method - When an audio file is imported into the app a copy of it is created in the currentWorkingDirectory (the project's Audio Files Directory or the temp directory if nothing has been saved yet).
 When the application is closed it will remove any unneeded audio files.
 
 Possible better method - Audio files are only imported into the currentWorkingDirectory once a project has been saved.
 This will reduce the build-up of redunent audio files that could be caused by the current method (For example, testing out all the sequencer drum banks).
 A clean up will still need to be done on exit though as there may be audio files included in the saved files previous state that aren't being used anymore.
 
 
 There are two types of audio files recognised within this application:
 1. External files, saved to XML as the full path name.
 2. Internal files, saved to XML as just the file name (with extension). Internal files are created when an audio file
    is added to the application when copyExternalFiles or copyLibraryFiles is true, and a copy of the file is created
    within the Audio Files directory of the project. Generally the default type of file 'recognised' by the app is an
    internal one.
 At the moment, external files are recognised by the XML full path name, and internal by the XML file name only. 
 It would probably make the code shorter and easier to read if
 both external and internal files have associated variables/attributes that tell the program what kind of file it is.
 
 All "/" have been changed to File::separatorString as I've just learnt Windows uses "\" instead.
 Will this make it incompatible to open up windows projects on mac. Apparrently windows can handle "/" too,
 so should I change it back? Must test this.
 */

#ifndef H_APPDOCUMENTSTATE
#define H_APPDOCUMENTSTATE

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Application/AbstractSubjectAndObserver.h"
#include "../GUI Classes/Custom Components/Scene/SceneValues.h"


/*
 This class handles ALL the loading and saving of settings, including scenes, within the application.
 All settings and documents are in the form of XmlElement objects.
 */
class MainAppWindow;

class AppDocumentState :    public Subject //so this class can notify observers
{
public:
    AppDocumentState();
    ~AppDocumentState();
    
    void setMainAppWindowRef (MainAppWindow *ref);
    
    void createNewProject();
    void saveProject();
    void saveProjectAs();
    void loadProject(bool openBrowser, File fileToOpen = File::nonexistent);
    void saveSceneToDisk (int sceneNumber);
    bool loadSceneFromDisk(int sceneNumber, bool openBrowser = true, File fileToOpen = File::nonexistent);
    
    void saveProjectSettings();
    void loadProjectSettings();
    void saveToScene (int sceneNumber);
    void loadFromScene (int sceneNumber);
    void clearScene (int sceneNumber);
    
    void saveSequence (int currentlySelectedSeqNumber, int currentlySelectedPad);
    void loadSequence (int currentlySeletedSeqNumber, Array<int> selectedPads_);
    void saveSequenceSet (int currentlySelectedPad);
    void loadSequenceSet (Array<int> selectedPads_);
    
    int getGuiUpdateFlag();
    int getSceneToUpdate();
    int getSceneStatus();
    
    void removeUneededAudioFiles();
    void importAudioFiles();   
    void setCurrentlySelectedScene(int value);
    
    static void registerRecentFile (const File& file);
    
private:
    
    OwnedArray<XmlElement> sceneData;
    ScopedPointer <XmlElement> projectData;
    
    //guiUpdateFlag is accessed by the observer class (mainComponent) so it knows what part of the GUI it should be updating.
    // 0 = settings (default)
    // 1 = sceneComponent (so that when a performance is loaded the included scenes can be selected)
    int guiUpdateFlag;
    //accessed by the observer class (mainComponent) so it knows what slot in the sceneComponent it needs to update
    int sceneToUpdate;
    //accessed by the observer class (mainComponent) so it knows what to set the selected scene GUI to (0 = empty, 1 = filled)
    int sceneStatus;
    
    File currentProjectFile; //the currently open perfomance/project
    
    MainAppWindow *mainAppWindowRef;
    
    bool shouldDisplayAlertWindow;  //Don't think this is needed anymore as it was a 
                                    //bad idea to display an alert window when saving
    
    int currentlySelectedScene;
    
};

#endif