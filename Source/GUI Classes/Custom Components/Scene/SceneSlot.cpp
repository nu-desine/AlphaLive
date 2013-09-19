//
//  SceneSlot.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 31/10/2011.
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

#include "SceneSlot.h"
#include "SceneComponent.h"
#include "../../../File and Settings/AppSettings.h"
#include "../../../File and Settings/StoredSettings.h"
#include "../../AlphaLiveLookandFeel.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[i]

SceneSlot::SceneSlot (int slotNumber_, SceneComponent &ref)
                    :   slotNumber(slotNumber_),
                        sceneComponentRef(ref)
{
    status = 0;
    mouseIsOver = mouseIsDown = false;
    slotNumberString = String(slotNumber+1);
    somethingIsBeingDraggedOver = false;
}

SceneSlot::~SceneSlot()
{
    
}

void SceneSlot::resized()
{
    
}

void SceneSlot::paint (Graphics &g)
{
    
    //set colour/fill
    if (status == 0) //empty
    {
        ColourGradient seqGradient(AlphaTheme::getInstance()->backgroundColour, (getWidth()*0.5), 0, AlphaTheme::getInstance()->foregroundColourDarker.withAlpha(0.5f), (getWidth()),(getHeight()), true);
        g.setGradientFill(seqGradient);
    }
    else if (status == 1) //settings applied but not selected
    {
        ColourGradient seqGradient(AlphaTheme::getInstance()->textColour.withAlpha(0.5f), (getWidth()*0.5), 0, AlphaTheme::getInstance()->foregroundColour.withAlpha(0.5f), (getWidth()*0.5),(getHeight()), true);
        g.setGradientFill(seqGradient);
    }
    else if (status == 2) //selected
    {
        ColourGradient seqGradient(AlphaTheme::getInstance()->mainColourLighter, (getWidth()*0.5), 0, AlphaTheme::getInstance()->mainColour, (getWidth()*0.5),(getHeight()), true);
        g.setGradientFill(seqGradient);
    }
    
    //draw circle
    g.fillEllipse(0, 0, getWidth(), getHeight());
    
    
    //highlight slot when mouse is over
    if (mouseIsOver == true)
    {
        g.setColour(Colours::white.withAlpha(0.5f));
        g.fillEllipse(0, 0, getWidth(), getHeight());
        
        //text
        g.setColour(AlphaTheme::getInstance()->backgroundColour);
        g.setFont(9);
        g.drawFittedText(slotNumberString, 0, 0, getWidth(), getHeight(), Justification::centred, 1);
    }
    
    if (status == 2)
    {
        //text
        g.setColour(AlphaTheme::getInstance()->backgroundColour);
        g.setFont(9);
        g.drawFittedText(slotNumberString, 0, 0, getWidth(), getHeight(), Justification::centred, 1);
    }
    
    
    if (somethingIsBeingDraggedOver == true)
    {
        //draw a 'highlight' eclipse. Another option would be to draw an image of a downwards arrow signifying 'drop here'?
        g.setColour(Colours::white.withAlpha(0.3f));
        g.fillEllipse(0, 0, getWidth(), getHeight());
    }
    
    
    //highlight when mouse is down
    if (mouseIsDown == true)
    {
        g.setColour(Colours::white.withAlpha(0.9f));
        g.fillEllipse(0, 0, getWidth(), getHeight());
    }

}


void SceneSlot::mouseDown (const MouseEvent &e)
{ 
    mouseIsDown = true;
    //don't call repaint() here - repaint is called when needed below
    
    /* NEW SCENE IMPLEMENTATION.
     All scenes always contain some data and are all selectable. 
     Changing scene saves the previous scene first if this option is not set to off in the prefs.
     Shift-click on a scene copies/saves the current scene to the clicked scene.
     */
    
    //---handle the GUI and 'status' value-----
    if (status != 2 && e.mods.isShiftDown() == true) //if clicked on an unselected slot while shift is down
    {
        selectSlot(true);

    }
    else if (status == 2 && e.mods.isShiftDown() == true) //if shift-clicked on the selected slot
    {
        //save data to scene slot
        sceneComponentRef.getAppDocumentState().saveToScene(slotNumber);
        repaint();
        
    }
    else if (status != 2 && e.mods.isPopupMenu() == false) //if clicked on an unselected slot but it isn't a 'right click'
    {
        selectSlot();
    }
    
    
    //right click to bring up the popup menu to allow single scenes to be loaded and saved to disk
    if (e.mods.isPopupMenu() == true)
    {
        
        PopupMenu menu;
        
        menu.addItem(1, translate("Import scene..."));
        if (status != 0) //if there is something store in the scene slot
        {
            menu.addItem(2, translate("Export scene..."));
            menu.addItem(3, translate("Clear scene..."));
        }
        if (status == 2)
            menu.addItem(4, translate("Revert to saved settings..."));
        
        const int result = menu.show();
        
        if (result == 0)
        {
            // user dismissed the menu without picking anything
        }
        else if (result == 1) // load scene
        {
            loadScene();
        }
        else if (result == 2) // save scene
        {
            saveScene();
        }
        else if (result == 3) // clear and remove (except for scene 0) scene
        {
            clearScene();
        }
        else if (result == 4) // revert back to settings saved within the scenes XML element
        {
            sceneComponentRef.getAppDocumentState().loadFromScene(slotNumber);
        }
        
    }
    
}

void SceneSlot::mouseUp (const MouseEvent &e)
{
    mouseIsDown = false;
    repaint();
}

void SceneSlot::mouseEnter	(const MouseEvent & e)
{
    mouseIsOver = true;
    repaint();
}

void SceneSlot::mouseExit	(const MouseEvent & e)
{
    mouseIsOver = false;
    repaint();
}


void SceneSlot::selectSlot (bool isShiftDown)
{
    //this will only be called when shift-clicking a slot which ISN'T the currently selected one
    
    setStatus(2); //set as selected
    
    //if set to do so, save previously selected scene data
    if (StoredSettings::getInstance()->autoSaveScenes == 2)
    {
        sceneComponentRef.getAppDocumentState().saveToScene(sceneComponentRef.getSelectedSceneNumber());
    }
    
    //search through all the pads of prev scene checking their modes.
    //if all pads are set to off, the slots status will need
    //to be set to 0 in SceneComponent::configureSelectedSlot()
    //which called below
    int modeCheck = 0;
    int prevStatus = 0;
    for (int i = 0; i <= 47; i++)
    {
        //there's a bug here where if prefs says don't auto save scenes and the scene originally
        //contained no data and hasn't been saved, the status of the prev scene slot will be set to
        //1 when it should be set to 0, displaying that it has data when it actually doesn't.
        modeCheck = AppSettings::Instance()->padSettings[i]->getMode();
        if (modeCheck > 0)
        {
            prevStatus = 1;
            break;
        }
    }
    
    if (isShiftDown == true)
    {
        //save/copy data to clicked scene
        sceneComponentRef.getAppDocumentState().saveToScene(slotNumber);
    }
    else
    {
        //load data from clicked scene
        sceneComponentRef.getAppDocumentState().loadFromScene(slotNumber);
    }
    
    // reset unused mode settings
    for (int i = 0; i <=47; i++)
        PAD_SETTINGS->resetData(PAD_SETTINGS->getMode());
    
    sceneComponentRef.configureSelectedSlot (slotNumber, prevStatus);
}


void SceneSlot::setStatus (int value)
{
    status = value;
    repaint();
}

int SceneSlot::getStatus()
{
    return status;
}


void SceneSlot::saveScene()
{
    sceneComponentRef.getAppDocumentState().saveSceneToDisk(slotNumber);
}



void SceneSlot::loadScene()
{
    //sceneComponentRef.setSelectedSceneNumber(slotNumber);
    bool shouldLoad = sceneComponentRef.getAppDocumentState().loadSceneFromDisk(slotNumber);
    
    if (shouldLoad == true) //when the filebrowser 'cancel' button isn't pressed
    {
        
        if (slotNumber == sceneComponentRef.getSelectedSceneNumber())
        {
            sceneComponentRef.getAppDocumentState().loadFromScene(slotNumber);
        }
        else
        {
            //sceneComponentRef.slotClicked(this);
            selectSlot();
            
            //the above method automatically sets the loaded scene as the selected scene.
            //if we would prefer to just load the scene but not select it, we could just call
            //setStatus(1) I think.
        }
        
    }
}


void SceneSlot::clearScene()
{
    //save the current settings into the selected secen
    sceneComponentRef.getAppDocumentState().saveToScene(sceneComponentRef.getSelectedSceneNumber());
    
    //reset pad settings data
    for (int i = 0; i <= 47; i++)
        AppSettings::Instance()->padSettings[i]->resetData(0);
    AppSettings::Instance()->resetData();
    
    //save the reset data into the clicked slot
    sceneComponentRef.getAppDocumentState().saveToScene(slotNumber);
    
    if (slotNumber != sceneComponentRef.getSelectedSceneNumber())
    {
        setStatus(0);
    }
    //else, status should be 2 and needs to stay as 2
    
    //load the saved data back into the currently selected scene
    sceneComponentRef.getAppDocumentState().loadFromScene(sceneComponentRef.getSelectedSceneNumber());
    
}



bool SceneSlot::isInterestedInFileDrag (const StringArray& files)
{
    File selectedFile(files[0]);
	String extension = selectedFile.getFileExtension();
    
	if (extension == ".alphascene")
		return true;
	else
		return false;
    
}
void SceneSlot::fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/)
{
    somethingIsBeingDraggedOver = true;
	repaint();
    
}
void SceneSlot::fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/)
{
    
}
void SceneSlot::fileDragExit (const StringArray& /*files*/)
{
    somethingIsBeingDraggedOver = false;
	repaint();
}
void SceneSlot::filesDropped (const StringArray& files, int /*x*/, int /*y*/)
{
    //string of filepath
	String message = files.joinIntoString ("\n");
	
    //file of filepath
    File droppedFile (message);
    
    //'put' scene data into the slot
    sceneComponentRef.getAppDocumentState().loadSceneFromDisk(slotNumber, false, droppedFile);
    
    if (slotNumber == sceneComponentRef.getSelectedSceneNumber())
        sceneComponentRef.getAppDocumentState().loadFromScene(slotNumber);
    else
    {
        selectSlot();
        //the above method automatically sets the loaded scene as the selected scene.
        //if we would prefer to just load the scene but not select it, we could just call
        //setStatus(1) I think.
    }
    
	somethingIsBeingDraggedOver = false;
	repaint();
    
}
