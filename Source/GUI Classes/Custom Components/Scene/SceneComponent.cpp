//
//  SceneComponent.cpp
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

#include "SceneComponent.h"
#include "../../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]


SceneComponent::SceneComponent(AppDocumentState &ref, ModeController &ref2)
                                :   appDocumentStateRef(ref),
                                    mSubject(ref2)
{
    //attach this class to the subject class
    mSubject.attach(this);
    
    shouldShowSaveWindow = false; //to prevent the 'saved'alertwindow to be shown on start up and in other situtions
    
    for (int i = 0; i <= NO_OF_SCENES-1; i++)
    {
        addAndMakeVisible(slot[i] = new SceneSlot(i, *this));
        appDocumentStateRef.saveToScene(i);
    }
    
    
    selectedSceneNumber = 0;
    slot[0]->setStatus(2);
    
    //set this component to listen to itself
    addKeyListener(this);
    setWantsKeyboardFocus(true);
}

SceneComponent::~SceneComponent()
{
    deleteAllChildren();
    
    //detach this class from the subject class
    mSubject.detach(this);
}

void SceneComponent::resized()
{
    //draw slots (4 rows of 5)
    for (int i = 0, counter = 0; i <= NO_OF_SCENES-1; i++)
    {
        slot[i]->setBounds(0, counter, getWidth(), getWidth());
        
        counter = counter + 32;											
		
    } 
}

void SceneComponent::paint (Graphics &g)
{
    
}

void SceneComponent::clearAll()
{
    //reset pad settings data
    for (int i = 0; i <= 47; i++)
        AppSettings::Instance()->padSettings[i]->resetData(0);
    AppSettings::Instance()->resetData();
    
    //save reset data to all scenes and set the status of each scene
    for (int i = 0; i < NO_OF_SCENES; i++)
    {
        appDocumentStateRef.saveToScene(i);
        
        if (i == selectedSceneNumber)
        {
            appDocumentStateRef.loadFromScene(i);
        }
        else 
        {
            slot[i]->setStatus(0);
        }
    }

}



void SceneComponent::configureSelectedSlot (int clickedSlot, int lastSlotStatus)
{
    //de-select previously selected slot and set to the right status
    for (int i = 0; i <= NO_OF_SCENES-1; i++)
    {
        //if slot doesn't equal clicked slot, and slots status = 2 (selected)
        if (i != clickedSlot && slot[i]->getStatus() == 2) 
        {
            slot[i]->setStatus(lastSlotStatus);
        }
    } 
    
    //set the selected scene number
    selectedSceneNumber = clickedSlot;
    appDocumentStateRef.setCurrentlySelectedScene(clickedSlot);
}


int SceneComponent::getSelectedSceneNumber()
{
    return selectedSceneNumber;
}


void SceneComponent::setSlotStatus (int slotNumber, int statusValue)
{
    slot[slotNumber]->setStatus(statusValue);
    
    //does the below need to be done? this function is only called from loadProject (i think)
    if (statusValue == 2)
    {
        selectedSceneNumber = slotNumber;
    }
}



//observers update function, called everytime a pad set to the controller mode asks to load up scene settings
bool SceneComponent::update(const Subject& theChangedSubject)
{
    if (&theChangedSubject == &mSubject)
    {
        int sceneNumber = AppSettings::Instance()->padSettings[mSubject.getPadNumber()]->getControllerSceneNumber()-1;
        selectSlot(sceneNumber);
    }
    
    return true;
}

void SceneComponent::selectSlot (int slotNumber)
{
    
    //load up slot data 
    //slotClicked(slot[sceneNumber]);
    
    if(slotNumber == -1) //move to next slot number
    {
        slotNumber = selectedSceneNumber + 1;
        
        if (slotNumber >= NO_OF_SCENES)
            slotNumber = 0;
        
    }
    else if (slotNumber == -2)  //move to previous slot number
    {
        slotNumber = selectedSceneNumber - 1;
        
        if (slotNumber < 0)
            slotNumber = NO_OF_SCENES-1;
    }
    
    //else, slotNumber represents the actualy slot number that needs to be selected
    
    
    //should I be locking the message thread like I'm currently doing?
    //or could this cause delays/lagging, in which case I should use an aSyncUpdater?
    const MessageManagerLock mmLock;
    
    slot[slotNumber]->selectSlot();
}

bool SceneComponent::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    std::cout << "key press" << std::endl;
    
    if (key == KeyPress::downKey || key == KeyPress::rightKey)
    {
        selectSlot(-1);
        return true;
    }
    else if (key == KeyPress::upKey || key == KeyPress::leftKey)
    {
        selectSlot(-2);
        return true;
    }
    else
        return false; //incase the keypress is a shortcut that the parent needs to react to.
    
}

/*
void SceneComponent::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}
 */

AppDocumentState& SceneComponent::getAppDocumentState()
{
    return appDocumentStateRef;
}

void SceneComponent::setSelectedSceneNumber(int value)
{
    selectedSceneNumber = value;
}

SceneSlot* SceneComponent::getSelectedSceneSlot()
{
    return slot[selectedSceneNumber];
}

void SceneComponent::disableSaveAlertWindow()
{
    shouldShowSaveWindow = false;
}

