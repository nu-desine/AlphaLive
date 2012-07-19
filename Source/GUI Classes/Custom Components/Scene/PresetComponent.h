//
//  PresetComponent.h
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

#ifndef H_PRESETCOMPONENT
#define H_PRESETCOMPONENT

#include "../JuceLibraryCode/JuceHeader.h"
#include "PresetSlot.h"
#include "AppDocumentState.h"
#include "PresetValues.h"
#include "ModeController.h"
#include "AbstractSubjectAndObserver.h"

class PresetComponent : public Component,
                        public Observer //so this class can observe ModeController
{
public:
    PresetComponent(AppDocumentState &ref, ModeController &ref2);
    ~PresetComponent();
    
    void resized();
    void paint (Graphics &g);
    
    void clearAll();
    void slotClicked(PresetSlot *presetSlot);
    
    int getSelectedPresetNumber();
    void setSelectedPresetNumber(int value);
    
    void setSlotStatus (int slotNumber, int statusValue);
    void selectDefaultPreset();
    
    //override the Observer virtual update function
    bool update(const Subject& theChangedSubject);
    
    //void setCurrentlySelectedPad (Array<int> selectedPads_); //why did i have this here????!!!!
    void disableSaveAlertWindow();
    
    //access by the preset slots to call the loadPresetToDisk and savePresetToDisk functions
    AppDocumentState& getAppDocumentState();
    
    PresetSlot* getSelectedPresetSlot();
    
private:
    
    PresetSlot *slot[NO_OF_PRESETS];
    int selectedPresetNumber;
    
    AppDocumentState &appDocumentStateRef;
    
    //create a reference to the subject that you want this class to observe
    ModeController &mSubject;
    
    //int currentlySelectedPad; //why did I have this here???!!!
    //Array<int> selectedPads;
    
    bool shouldShowSaveWindow;
    
    
};


#endif