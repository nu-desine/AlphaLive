//
//  SceneComponent.h
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

#ifndef H_SCENECOMPONENT
#define H_SCENECOMPONENT

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "SceneSlot.h"
#include "../../../File and Settings/AppDocumentState.h"
#include "SceneValues.h"
#include "../../../Functionality Classes/Controller Mode/ModeController.h"

class SceneComponent :  public Component,
                        public KeyListener
{
public:
    SceneComponent(AppDocumentState &ref, ModeController &ref2);
    ~SceneComponent();
    
    void resized();
    void paint (Graphics &g);
    
    void clearAll();
    void configureSelectedSlot (int clickedSlot, int lastSlotStatus);
    
    int getSelectedSceneNumber();
    void setSelectedSceneNumber(int value);
    
    void setSlotStatus (int slotNumber, int statusValue);
    
    void disableSaveAlertWindow();
    
    //access by the scene slots to call the loadSceneToDisk and saveSceneToDisk functions
    AppDocumentState& getAppDocumentState();
    
    SceneSlot* getSelectedSceneSlot();
    
    void selectSlot (int slotNumber);
    
    bool keyPressed (const KeyPress &key, Component *originatingComponent);
    
private:
    
    SceneSlot *slot[NO_OF_SCENES];
    int selectedSceneNumber;
    
    AppDocumentState &appDocumentStateRef;
    
    bool shouldShowSaveWindow;
};

#endif