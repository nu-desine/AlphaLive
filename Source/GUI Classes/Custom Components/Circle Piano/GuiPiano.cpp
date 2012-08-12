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
	
	keyValue = 60;
	
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
		//keys[i]->setRadioGroupId (12);
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
	for (int i = 0; i <= 119; i++)
	{
        if (button == keys[i])
        {
            //by default, no key is selected, so a default value of 400 is set to key index.
            //therefore the first time you select a key/button and this function is called,
            //it won't go into this if statement that is used to set the previously selected to key to 'off',
            //which should now also be calling setToggleState(false) since I've removed the keys being a radio group.
            //the new key/note is then set after the if statement.
            //This is not actually how it needs to be implemented!
            if (keyValue < 400) 
				setKeyDisplay (keyValue, false);
            
            keyValue = i;
            
            setKeyDisplay (keyValue, true);
            
            for (int j = 0; j < selectedPads.size(); j++)
            {
                int padNum = selectedPads[j];
                
                if (PAD_SETTINGS->getMode() == 1) //Midi Mode
                {
                    PAD_SETTINGS->setMidiNote(i);
                }
            }
            
            
            
            
            //keys[i]->setColours(AlphaColours::blue, AlphaColours::blue, AlphaColours::lightblue);
        }
	}

}


void GuiPiano::setNote()
{
   
	
	
}



//called when a pad is selected from the pad layout
void GuiPiano::setCurrentlySelectedPad (Array <int> selectedPads_)
{
	selectedPads = selectedPads_;
}


//called when a pad with midi mode on it needs to be displayed
void GuiPiano::updateDisplay()
{
    //first clear all keys
    for (int i = 0; i < 120; i++)
        setKeyDisplay(i, false);

    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getMode() == 1) //Midi Mode
        {
            setKeyDisplay (PAD_SETTINGS->getMidiNote(), true);
        }
        
    }
    
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
            std::cout << i << " toggle state: " << keys[i]->getToggleState() << std::endl;
            //int newNote = (transposeValue * 12) + i;
           // setNoteLabelText(newNote);
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
    
}
