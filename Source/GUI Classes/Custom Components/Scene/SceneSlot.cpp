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
#include "../../AlphaLiveLookandFeel.h"

SceneSlot::SceneSlot (int slotNumber_, SceneComponent &ref)
                    :   slotNumber(slotNumber_),
                        sceneComponentRef(ref)
{
    //set this component to have keyboard focus
    //setWantsKeyboardFocus(true);
    
    status = 0;
    mouseIsOver = false;
    slotNumberString = String(slotNumber+1);
    
}

SceneSlot::~SceneSlot()
{
    
}

void SceneSlot::resized()
{
    
}

void SceneSlot::paint (Graphics &g)
{
    //std::cout << "Status = " << status << std::endl;
    
    //set colour/fill
    if (status == 0) //empty
    {
        //g.setColour(Colours::black);
        ColourGradient seqGradient(Colours::black, (getWidth()*0.5), 0, Colours::darkgrey.withAlpha(0.5f), (getWidth()),(getHeight()), true);
        g.setGradientFill(seqGradient);
    }
    else if (status == 1) //settings applied but not selected
    {
        ColourGradient seqGradient(Colours::white.withAlpha(0.5f), (getWidth()*0.5), 0, Colours::grey.withAlpha(0.5f), (getWidth()*0.5),(getHeight()), true);
        g.setGradientFill(seqGradient);
    }
    else if (status == 2) //selected
    {
        ColourGradient seqGradient(AlphaColours::lightblue, (getWidth()*0.5), 0, AlphaColours::blue, (getWidth()*0.5),(getHeight()), true);
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
        g.setColour(Colours::black);
        g.setFont(9);
        g.drawFittedText(slotNumberString, 0, 0, getWidth(), getHeight(), Justification::centred, 1);
    }
    
    
    

}


void SceneSlot::mouseDown (const MouseEvent &e)
{ 
    
    /* NEW SCENE IMPLEMENTATION.
     All scenes always contain some data and are all selectable. 
     Changing scene saves the previous scene first.
     Shift-click on a scene copies the current scene to the clicked scene.
     */
    
    
    //---handle the GUI and 'status' value-----
    if (status != 2 && e.mods.isShiftDown()==true) //if clicked on unselected slot shift is currently down
    {
        //sceneComponentRef.slotClicked(this);

    }
    else if (status != 2 && e.mods.isPopupMenu() == false) //if clicked on an unselected slot but it isn't a 'right click'
    {
        sceneComponentRef.slotClicked(this); //call callback function, where it will 'load' the data
    }
    
    
    //right click to bring up the popup menu to allow single scenes to be loaded and saved to disk
    if (e.mods.isPopupMenu() == true)
    {
        
        PopupMenu menu;
        
        menu.addItem(1, "Import scene...");
        if (status != 0) //if there is something store in the scene slot
        {
            menu.addItem(2, "Export scene...");
            menu.addItem(3, "Clear scene...");
        }
        
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
    }
    
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
            sceneComponentRef.slotClicked(this);
            
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
