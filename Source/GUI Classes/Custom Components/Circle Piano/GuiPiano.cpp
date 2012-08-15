/*
 *  GuiPiano.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 07/06/2012.
 *  Copyright 2012 nu desine. All rights reserved.
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
 */

#include "GuiPiano.h"
#include "../../../File and Settings/AppSettings.h"
#include "../../AlphaLiveLookandFeel.h"
#include "../../../File and Settings/StoredSettings.h"


#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiPiano::GuiPiano() : Component ("GuiPiano")

{
    recentlyUpdated = true;
    
    //note display label
    addAndMakeVisible(midiNoteLabel = new Label("Note Label", "60"));
    midiNoteLabel->setJustificationType(Justification::horizontallyCentred);
    midiNoteLabel->setColour(Label::textColourId, Colours::lightgrey);
    midiNoteLabel->addMouseListener(this, true);
    
    noteDisplayType = StoredSettings::getInstance()->midiNoteDisplayType;
	
	keyWidth = ( 2.57142857 * (M_PI / 180));
	keySegWhite = (keyWidth * 0.9);
	keyGapWhite = (keyWidth * 0.1);
	keySegBlack = (keyWidth * 0.5);
	keyGapBlack = (keyWidth * 0.5);
	segEndWhite = (317 * (M_PI / 180)) - (keyGapWhite * 0.5);
	segStartWhite = segEndWhite - keySegWhite;
	segEndBlack = ((317 * (M_PI / 180)) - (keyGapBlack + (keyGapBlack * 0.5)));
	segStartBlack = segEndBlack - keySegBlack;
	
	
	for (int i = 0; i <= 119; i++) 
	{
		int keyIndex = i + 1;
		int noteIndex = (keyIndex % 12);
		
		if(noteIndex == 2 || noteIndex == 4 || noteIndex == 7 || noteIndex == 9 || noteIndex == 11)
		{
			keysPath.insert(i, new Path());
			keysPath[i]->addPieSegment(71, -18, 626, 626, segStartBlack, segEndBlack, 0.9f);
			segEndBlack = segStartBlack - keyGapBlack;
			segStartBlack = segEndBlack - keySegBlack;
		}
		else 
		{
			keysPath.insert(i, new Path());
			keysPath[i]->addPieSegment(41, -48, 686, 686, segStartWhite, segEndWhite, 0.82f);
			segEndWhite = segStartWhite - keyGapWhite;
			segStartWhite = segEndWhite - keySegWhite;
			
			if (noteIndex == 5 || noteIndex == 0 || noteIndex == 12) 
            {
				segEndBlack = segStartBlack - keyGapBlack;
				segStartBlack = segEndBlack - keySegBlack;
			}
		}
	}
	
	
	for (int i = 0; i <= 119; i++)
	{
		keyNumber = String(i);
		
		int noteIndex = ((i + 1) % 12);		
				
		if(noteIndex == 2 || noteIndex == 4 || noteIndex == 7 || noteIndex == 9 || noteIndex == 11)
		{
			keys.insert(i, new GuiKeys(keyNumber, Colours::grey, Colours::lightgrey, AlphaColours::lightblue));
			keys[i]->setShape(*keysPath[i], true, true, false);
			addAndMakeVisible(keys[i],(i+200));
		}
		else  if (noteIndex == 1)
		{
			keys.insert(i, new GuiKeys(keyNumber, AlphaColours::nearlyblack, AlphaColours::verydarkgrey, AlphaColours::lightblue));
			keys[i]->setShape(*keysPath[i], true, true, false);
			keys[i]->setOutline(Colours::darkgrey.withAlpha(0.5f), 1.0f);
			addAndMakeVisible(keys[i],i);
		}
		else
		{
			keys.insert(i, new GuiKeys(keyNumber, Colours::black, AlphaColours::verydarkgrey, AlphaColours::lightblue));
			keys[i]->setShape(*keysPath[i], true, true, false);
			keys[i]->setOutline(Colours::darkgrey.withAlpha(0.5f), 1.0f);
			addAndMakeVisible(keys[i],i);
		}

		keys[i]->setClickingTogglesState(true);
		keys[i]->setToggleState(false, false);	
		keys[i]->addListener(this);
        keys[i]->addMouseListener(this, true);
		keys[i]->setOpaque(false);
	}
}

GuiPiano::~GuiPiano()
{
	keysPath.clear();
    keys.clear();
	deleteAllChildren();
}


void GuiPiano::resized()
{
	for (int i = 0; i <= 119; i++)
	{
		if (i == 29 || i == 89) 
        {
			Rectangle<float> newBounds (keysPath[i]->getBounds());
			newBounds.expand (0.2f, 0.2f);
			keys[i]->setBounds(newBounds.getSmallestIntegerContainer());
		}
		else
        {
			keys[i]->setBounds(keysPath[i]->getBounds().getSmallestIntegerContainer());
        }
	}
	
}


void GuiPiano::paint (Graphics& g)
{
	
	
	
}


void GuiPiano::buttonClicked(Button *button)
{
    
    //get modifier key so we can handle cmd-clicks when selecting keys
    ModifierKeys modifier = ModifierKeys::getCurrentModifiers();
    
    
	for (int i = 0; i <= 119; i++)
	{
        if (button == keys[i])
        {
            //=========================================================================
            //regular click to set the selected midi pads note or sequencers root note
            if (modifier.isCommandDown() == false)
            {
                //first clear all keys
                for (int i = 0; i < 120; i++)
                    setKeyDisplay(i, false);
                selectedKeys.clear();
                
                
                for (int padIndex = 0; padIndex < selectedPads.size(); padIndex++)
                {
                    int padNum = selectedPads[padIndex];
                    
                    
                    
                    if (PAD_SETTINGS->getMode() == 1) //Midi Mode
                    {
                        PAD_SETTINGS->setMidiNote(i);
                        
                        if (padIndex == 0)
                        {
                            setKeyDisplay (i, true);
                            selectedKeys.addIfNotAlreadyThere(i);
                        }
                    }
                    
                    else if (PAD_SETTINGS->getMode() == 3) //Sequencer Mode
                    {
                        recentlyUpdated = true;
                        
                        //get currently set 'root' note
                        int rootNote = PAD_SETTINGS->getSequencerMidiNote(0);
                        //get difference between root note and clicked note
                        int transposeValue = rootNote - i;
                        
                        //add the transpose value to each note in the scale/set
                        for (int row = 0; row < 12; row++)
                        {
                            int currentVal = PAD_SETTINGS->getSequencerMidiNote(row);
                            int newVal = currentVal-transposeValue;
                            if (newVal > 119)
                                newVal = 119;
                            else if (newVal < 0)
                                newVal = 0;
                            
                            //the above check is needed, as keys above 119 don't exist
                            //and will cause a crash. 
                            //if a user selected 119 all notes will be set to 119,
                            //however if the user then selects 60, all the notes will
                            //be set to 60 as currentVal-transposeValue will be the same
                            //for each note. Is there a way to get back the original scale/set?
                            
                            PAD_SETTINGS->setSequencerMidiNote(newVal, row);
                            
                            //update the GUI (only need to do this a single time)
                            if (padIndex == 0)
                            {
                                setKeyDisplay (newVal, true);
                                selectedKeys.addIfNotAlreadyThere(newVal);
                            }
                                
                        }
                    }
                }
            
                
                
                break;
            }
            
            //=========================================================================
            //cmd-click to select mutiple notes for midi pads or custom scale for sequencer pads
            else if (modifier.isCommandDown() == true)
            {
                //don't intially clear the keys
                
                //if the number of selected keys is less than the number of selected pads
                if (selectedKeys.size() < selectedPads.size())
                {
                    //cycle through the SELECTED KEYS in the order they were selected,
                    //applying them to the selected pads in chronilogical order
                    for (int padIndex = 0; padIndex < selectedKeys.size(); padIndex++)
                    {
                        int padNum = selectedPads[padIndex];
                        
                        if (PAD_SETTINGS->getMode() == 1) //Midi Mode
                        {
                            
                            PAD_SETTINGS->setMidiNote(selectedKeys[padIndex]);
                            
                            //update the GUI
                            if (padIndex == 0)
                            {
                                //add the key
                                setKeyDisplay (i, true);
                                selectedKeys.addIfNotAlreadyThere(i); 
                            }
                            
                        }
                        
                    }
                     
                }
                
                for (int padIndex = 0; padIndex < selectedPads.size(); padIndex++)
                {
                    int padNum = selectedPads[padIndex];
                    
                    if (PAD_SETTINGS->getMode() == 3) //Sequencer Mode
                    {
                        //currently, if the user cmd-clicks a key for the first time
                        //(when recentlyUpdated == true) it will clear the piano
                        //and allow the user to select 12 individual different keys,
                        //each time applying the selected set of keys to the sequencer
                        //rows. if the user doesn't selected all 12, and the does a normal
                        //click will will change the root note, what should the unselected
                        //note be? The previously set note of that row? 0? another default value?
                        // if it it the prev note... should cmd-click delete/replace each note
                        //of the row 1 at a time rather than clearing everything? OR will
                        //that get confusing?
                        
                        
                        if (recentlyUpdated == true)
                        {
                            //first clear all keys
                            for (int i = 0; i < 120; i++)
                                setKeyDisplay(i, false);
                            selectedKeys.clear();
                            
                            recentlyUpdated = false;
                        }
                         
                        
                        
                        //if the number of selected keys is less than the number of sequencer rows
                        if (selectedKeys.size() < 12)
                        {
                            //update the GUI (only need to do this a single time)
                            if (padIndex == 0)
                            {
                                setKeyDisplay (i, true);
                                selectedKeys.addIfNotAlreadyThere(i);
                            }
                            
                            for (int row = 0; row < 12; row++)
                            {
                                
                                int note = selectedKeys[row];
                                
                                if (row > selectedKeys.size()-1)
                                    note = 0; //default note
                                
                                PAD_SETTINGS->setSequencerMidiNote(note, row);
                                std::cout << "row " << row << " set to note : " << note << std::endl;
                            }
                            
                            
                            
                        }
                    }
                    
                }
               
                
            }
                
                break;

            //=========================================================================
        }
	}
}



void GuiPiano::setCurrentlySelectedPad (Array <int> selectedPads_)
{
	selectedPads = selectedPads_;
}



void GuiPiano::updateDisplay()
{
    recentlyUpdated = true;
    
    //first clear all keys
    for (int i = 0; i < 120; i++)
        setKeyDisplay(i, false);
    
    selectedKeys.clear();

    //=========================================================================
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getMode() == 1) //Midi Mode
        {
            setKeyDisplay (PAD_SETTINGS->getMidiNote(), true);
            selectedKeys.addIfNotAlreadyThere(PAD_SETTINGS->getMidiNote());
        }
        
        else if (PAD_SETTINGS->getMode() == 3 && PAD_SETTINGS->getSequencerMode() == 1) //Midi Sequencer Mode
        {
            for (int i = 0; i < 12; i++)
            {
                setKeyDisplay (PAD_SETTINGS->getSequencerMidiNote(i), true);
                selectedKeys.addIfNotAlreadyThere(PAD_SETTINGS->getMidiNote());
            }
        }
    }
    
    //=========================================================================
    if(MULTI_PADS)
    {
    
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getMode() == 1) //Midi Mode
        {
            
            for (int i = 0; i < selectedPads.size(); i++)
            {
                padNum = selectedPads[i];
                setKeyDisplay (PAD_SETTINGS->getMidiNote(), true);
                selectedKeys.addIfNotAlreadyThere(PAD_SETTINGS->getMidiNote());
            }
        }
        
        else if (PAD_SETTINGS->getMode() == 3 && PAD_SETTINGS->getSequencerMode() == 1) //Midi Sequencer Mode
        {
            //what should I display here?
        }
    }
    //=========================================================================
    
}

void GuiPiano::mouseDown (const MouseEvent &e)
{
    
    if (midiNoteLabel->isMouseOver(true))
    {
        //right click to bring up the popup menu to allow midi notes to be display either as numbers or names
        if (e.mods.isPopupMenu() == true)
        {
            
            PopupMenu menu;
            
            if (noteDisplayType == 1)
                menu.addItem(1, "Display MIDI note numbers");
            else
                menu.addItem(2, "Display MIDI note names");
            
            const int result = menu.show();
            
            if (result == 0)
            {
                // user dismissed the menu without picking anything
            } 
            else if (result == 1)
            {
                noteDisplayType = 1; // note number
                setNoteLabelText(noteNumber);
                StoredSettings::getInstance()->midiNoteDisplayType = 1;
            }
            else if (result == 2)
            {
                noteDisplayType = 2; // note name
                setNoteLabelText(noteNumber);
                StoredSettings::getInstance()->midiNoteDisplayType = 2;
            }
        }
    }
}

void GuiPiano::mouseEnter (const MouseEvent &e)
{
    for (int i = 0; i <= 119; i++)
    {
        //===display note number when hovering mouse over a piano key===
        if (keys[i]->isMouseOver(true))
        {
            midiNoteLabel->setColour(Label::textColourId, Colours::orange);
        }
    }
}


void GuiPiano::mouseExit (const MouseEvent &e)
{
    //=====set key label text to currently selected note=====
    setNoteLabelText(noteNumber);
    midiNoteLabel->setColour(Label::textColourId, Colours::lightgrey);
}


void GuiPiano::setNoteLabelText (int noteNumber)
{
    if (noteDisplayType == 0)
        midiNoteLabel->setText(String(noteNumber), false);
    
    else if (noteDisplayType == 1)
        midiNoteLabel->setText(MidiMessage::getMidiNoteName(noteNumber, true, true, 3), false);
}


void GuiPiano::setNoteDisplayType (int value)
{
    noteDisplayType = value;
}




void GuiPiano::setKeyDisplay (int keyNumber, bool keyOn)
{
    if (keyOn == false) //off
    {
        int noteIndex = ((keyNumber + 1) % 12);		
        
        //set the key/button colour
        if(noteIndex == 2 || noteIndex == 4 || noteIndex == 7 || noteIndex == 9 || noteIndex == 11)
        {
            keys[keyNumber]->setColours(Colours::grey, Colours::lightgrey, AlphaColours::lightblue);
        }
        else if (noteIndex == 1)
        {
            keys[keyNumber]->setColours(AlphaColours::nearlyblack, AlphaColours::verydarkgrey, AlphaColours::lightblue);
        }
        else
        {
            keys[keyNumber]->setColours(Colours::black, AlphaColours::verydarkgrey, AlphaColours::lightblue);
        }
    }
    
    else if (keyOn == true)
    {
        //set the key/button colour
      keys[keyNumber]->setColours(AlphaColours::blue, AlphaColours::blue, AlphaColours::lightblue);  
    }
    
    //set the button toggle/state
    keys[keyNumber]->setToggleState(keyOn, false);
}






void GuiPiano::setActive (bool value)
{
    setInterceptsMouseClicks(false, value);
    
    if (value == true)
        setAlpha(1.0f);
    else
        setAlpha(0.3f);
    
    if (value == false)
    {
        for (int i = 0; i < 120; i++)
            setKeyDisplay(i, false);
    }
    /*
    else if (value == true)
    {
        updateDisplay();
    }
     */
    
}
