//
//  PresetSlot.cpp
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

#include "PresetSlot.h"
#include "PresetComponent.h"
#include "AppSettings.h"

PresetSlot::PresetSlot (int slotNumber_, PresetComponent &ref)
                    :   slotNumber(slotNumber_),
                        presetComponentRef(ref)
{
    //set this component to have keyboard focus
    //setWantsKeyboardFocus(true);
    
    status = 0;
    mouseIsOver = false;
    slotNumberString = String(slotNumber+1);
    
}

PresetSlot::~PresetSlot()
{
    
}

void PresetSlot::resized()
{
    
}

void PresetSlot::paint (Graphics &g)
{
    //std::cout << "Status = " << status << std::endl;
    
    //set colour/fill
    if (status == 0) //empty
    {
        //g.setColour(Colours::black);
        ColourGradient seqGradient(Colours::black, (getWidth()*0.3),(getHeight()*0.3), Colours::darkgrey, (getWidth()),(getHeight()), true);
        g.setGradientFill(seqGradient);
    }
    else if (status == 1) //Filled but not selected
    {
        ColourGradient seqGradient(Colours::white, (getWidth()*0.45),(getHeight()*0.45), Colours::grey, (getWidth()),(getHeight()), true);
        g.setGradientFill(seqGradient);
    }
    else if (status == 2) //selected
    {
        ColourGradient seqGradient(Colours::skyblue, (getWidth()*0.45),(getHeight()*0.45), Colours::blue, (getWidth()),(getHeight()), true);
        g.setGradientFill(seqGradient);
    }
    
    //draw circle
    g.fillEllipse(0, 0, getWidth()*0.9, getHeight()*0.9);
    
    
    //highlight slot when mouse is over
    if (mouseIsOver == true)
    {
        g.setColour(Colours::white.withAlpha(0.5f));
        g.fillEllipse(0, 0, getWidth()*0.9, getHeight()*0.9);
        
        //text
        g.setColour(Colours::black);
        g.setFont(9);
        g.drawFittedText(slotNumberString, (getWidth()*0.1), 0, (getWidth()*0.8), getHeight(), Justification::centred, 1);
    }
    
    
    

}


void PresetSlot::mouseDown (const MouseEvent &e)
{ 
    /*how presets should work:
     - shift-click on any slot to saved the current settings into that preset. The slot should then be display as 'selected'.
     - click on any 'filled' slot to load settings from the clicked preset. The slot should then be displayed as selected,
        with the previously selected pad now just displayed as 'filled'.
     - You can also reclick on a selected slot to load it back up? Or would this cause errors in terms of losing data by forgetting to hold shift when attempting to save?
     
     AM I HAPPY WITH THE CURRENT WAY THIS IS IMPLEMENTED? goo through the data code and make sure its effecient/not repeaeted
     */
    
    
    //---handle the GUI and 'status' value-----
    if (e.mods.isShiftDown()==true) //if shift is currently down
    {
        status = 1; //set as 'filled'
        presetComponentRef.slotClicked(this);//call callback function, where it will 'save' the data
        
        status = 2; //set as 'selected'
        repaint(); //update display
    }
    else if (status == 1 && e.mods.isPopupMenu() == false) //if clicked on a 'filled' slot but it isn't a 'right click'
    {
        status = 2; //set as selected
        presetComponentRef.slotClicked(this); //call callback function, where it will 'load' the data
        repaint();
    }
    
    
    //right click to bring up the popup menu to allow single presets to be loaded and saved to disk
    if (e.mods.isPopupMenu() == true)
    {
        
        PopupMenu menu;
        
        menu.addItem(1, "Import preset...");
        if (status != 0) //if there is something store in the preset slot
        {
            menu.addItem(2, "Export preset...");
            menu.addItem(3, "Clear and remove preset...");
        }
        
        
        const int result = menu.show();
        
        if (result == 0)
        {
            // user dismissed the menu without picking anything
        }
        else if (result == 1) // load preset
        {
            loadPreset();
        }
        else if (result == 2) // save preset
        {
            savePreset();
        }
        else if (result == 3) // clear and remove (except for preset 0) preset
        {
            clearPreset();
        }
    }
    
}



void PresetSlot::mouseEnter	(const MouseEvent & e)
{
    mouseIsOver = true;
    repaint();
}

void PresetSlot::mouseExit	(const MouseEvent & e)
{
    mouseIsOver = false;
    repaint();
}




void PresetSlot::setStatus (int value)
{
    status = value;
    repaint();
}

int PresetSlot::getStatus()
{
    return status;
}


void PresetSlot::savePreset()
{
    presetComponentRef.getAppDocumentState().savePresetToDisk(slotNumber);
}



void PresetSlot::loadPreset()
{
    //presetComponentRef.setSelectedPresetNumber(slotNumber);
    bool shouldLoad = presetComponentRef.getAppDocumentState().loadPresetFromDisk(slotNumber);
    
    if (shouldLoad == true) //when the filebrowser 'cancel' button isn't pressed
    {
        status = 2;
        presetComponentRef.slotClicked(this); //call callback function, where it will 'load' the data
        repaint();
    }
}


void PresetSlot::clearPreset()
{
    if (slotNumber != 0)
    {
        presetComponentRef.getAppDocumentState().clearPreset(slotNumber);
        
        if (status == 2) //if currently 'selected'
        {
            //set default preset 0 as 'selected' and load its settings
            presetComponentRef.selectDefaultPreset();
        }
        setStatus(0);
    }
    else //if this is preset 0 
    {
        //set slot to 'selected' but reset global data and mode 0 for each pad
        //reset all current padSettings 
        for (int i = 0; i <= 47; i++)
        {
            AppSettings::Instance()->padSettings[i]->resetData(0);
        }
        //reset global data
        AppSettings::Instance()->resetData();
        
        status = 1; //set as 'filled'
        presetComponentRef.disableSaveAlertWindow();
        presetComponentRef.slotClicked(this);//call callback function, where it will 'save' the data
        status = 2; //set as 'selected'
        repaint(); //update display
        presetComponentRef.slotClicked(this);
        
    }
}
