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
#include "AppSettings.h"

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
    }
    
    //by default first scene slot (scene 0) should be filled with default settings and selected
    //the below code basically saves the default empty settings to the first scene when you launch the app
    selectedSceneNumber = 0;
    slot[0]->setStatus(1);
    slotClicked(slot[0]);
    slot[0]->setStatus(2);
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
    for (int i = 0, col = 2, row = 2, counter = 0; i <= NO_OF_SCENES-1; i++)
    {
        slot[i]->setBounds(col, row, (getWidth()-4)/5, (getHeight()-4)/4);
        col += (getWidth()-4)/5;
        
        counter++;
        
        if (counter == 5) //draw next row
        {
            row += ((getHeight()-4)/4);
            col = 2;
            counter = 0;
        }
    }  
}

void SceneComponent::paint (Graphics &g)
{
    //draw background
    //g.setColour(Colours::darkgrey);
    ColourGradient seqGradient(Colours::grey, (getWidth()*0.5),(getHeight()*0.5), Colours::darkgrey, (getWidth()),(getHeight()), true);
    g.setGradientFill(seqGradient);
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 5.0);
}

void SceneComponent::clearAll()
{
    //DONT CLEAR SCENE 0 AS IT SHOULD ALWAYS BE FILLED
    for (int i = 1; i <= NO_OF_SCENES-1; i++)
    {
        //clear scene data
        appDocumentStateRef.clearScene(i);
        //clear scene slot GUI
        slot[i]->setStatus(0);
    }
    
    //set slot 0 to 'selected' but will reset global data and mode 0 for each pad
    
    //reset all current padSettings
    for (int i = 0; i <= 47; i++)
    {
        AppSettings::Instance()->padSettings[i]->resetData(0);
    }
    //reset global data
    AppSettings::Instance()->resetData();
    
    //set as 'filled'
    slot[0]->setStatus(1);
    //emulate a click that with its current status will 'save' the current settings
    shouldShowSaveWindow = false; //prevents the 'saved' window to appear 
    slotClicked(slot[0]);
    //set as 'selected'
    slot[0]->setStatus(2);
    //emulate click to 'load' settings
    slotClicked(slot[0]);

    
}


//called whenever a scene slot is clicked on
void SceneComponent::slotClicked(SceneSlot *sceneSlot)
{
    //find which slot was clicked on
    for (int i = 0; i <= NO_OF_SCENES-1; i++)
    {
        if (sceneSlot == slot[i])
        {
            //set the selected scene number
            selectedSceneNumber = i;
            
            appDocumentStateRef.setCurrentlySelectedScene(i);
            
            
            //get status of slot clicked
            int status = sceneSlot->getStatus();
            
            //--- SAVE DATA T0 SCENE----
            if (status == 1)
            {
                //save current app data
                appDocumentStateRef.saveToScene(i);
                
                std::cout << "Current settings have been saved into scene " + String(i) << std::endl;
                
                //de-select any other slecected slots
                for (int j = 0; j <= NO_OF_SCENES-1; j++)
                {
                    if (slot[j] != sceneSlot && slot[j]->getStatus() == 2) //if slot doesn't equal clicked slot, and slot's status = 2 (selected)
                    {
                        slot[j]->setStatus(1); //set status to filled but not sleceted
                    }
                } 
                
                if (shouldShowSaveWindow == true)
                {
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Saved!", "The current settings have been saved to scene " + String(i+1));
                }
                
                shouldShowSaveWindow = true; 
            }
            
            //--- LOAD UP SCENE DATA ----
            else if (status == 2) //if clicked on a filled slot
            {
                //save current app data
                appDocumentStateRef.loadFromScene(i); //or should the 'loading' be done from somewhere else?
                
                std::cout << "Settings have been loaded from scene " + String(i) << std::endl;
                
                //de-select any other slecected slots
                for (int i = 0; i <= NO_OF_SCENES-1; i++)
                {
                    if (slot[i] != sceneSlot && slot[i]->getStatus() == 2) //if slot doesn't equal clicked slot, and slot's status = 2 (selected)
                    {
                        slot[i]->setStatus(1); //set status to filled but not sleceted
                    }
                } 
            }
        }
    }
}


int SceneComponent::getSelectedSceneNumber()
{
    return selectedSceneNumber;
}


void SceneComponent::setSlotStatus (int slotNumber, int statusValue)
{
    slot[slotNumber]->setStatus(statusValue);
}


void SceneComponent::selectDefaultScene()
{
    slot[0]->setStatus(2);
    slotClicked(slot[0]); //'clicked' with a status of '2' will load the data
    
}


//observers update function, called everytime a pad set to the controller mode asks to load up scene settings
bool SceneComponent::update(const Subject& theChangedSubject)
{
    if (&theChangedSubject == &mSubject)
    {
        std::cout << "Attempting to change scene!\n";
        
        int presNumber = AppSettings::Instance()->padSettings[mSubject.getPadNumber()]->getControllerPresentNumber()-1;
        
        //change currently selected scene
        if (slot[presNumber]->getStatus() != 0) //if the scene slot isn't empty
        {
            //load up slot data 
            slot[presNumber]->setStatus(2);
            slotClicked(slot[presNumber]);
        }
    }
    
    return true;
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

void SceneComponent::disableSaveAlertWindow()
{
    shouldShowSaveWindow = false;
}

SceneSlot* SceneComponent::getSelectedSceneSlot()
{
    return slot[selectedSceneNumber];
}
