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

#define PAD_SETTINGS AppSettings::Instance()->padSettings[currentlySelectedPad]
#define PAD_SETTINGS_i AppSettings::Instance()->padSettings[i]


GuiPiano::GuiPiano() : Component ("GuiPiano")

{
    
    //note display label
    addAndMakeVisible(midiNoteLabel = new Label("Note Label", "60"));
    midiNoteLabel->setJustificationType(Justification::horizontallyCentred);
    midiNoteLabel->setColour(Label::textColourId, Colours::lightgrey);
    //Font newFont;
    //newFont.setTypefaceName("Grixel Acme 7 Wide");
    //midiNoteLabel->setFont(newFont);
    midiNoteLabel->addMouseListener(this, true);
    
    noteDisplayType = 0;
	
	keyWidth = ( 2.57142857 * (M_PI / 180));
	
	keySegWhite = (keyWidth * 0.9);
	keyGapWhite = (keyWidth * 0.1);

	keySegBlack = (keyWidth * 0.5);
	keyGapBlack = (keyWidth * 0.5);

	segEndWhite = (317 * (M_PI / 180)) - (keyGapWhite * 0.5);
	segStartWhite = segEndWhite - keySegWhite;
	
	segEndBlack = ((317 * (M_PI / 180)) - (keyGapBlack + (keyGapBlack * 0.5)));
	segStartBlack = segEndBlack - keySegBlack;
	
	
	keyValue = 400;
	
	
	
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
			
			if (noteIndex == 5 || noteIndex == 0 || noteIndex == 12) {
				
				segEndBlack = segStartBlack - keyGapBlack;
				segStartBlack = segEndBlack - keySegBlack;
				
			}
			
		}

	}
	
	
	for (int i = 0; i <= 119; i++)
	{
		
		keyNumber << i;
		
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
		keys[i]->setRadioGroupId (12);
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
            
            if (keyValue < 400) 
            {
				int noteIndex = ((keyValue + 1) % 12);		
				
				if(noteIndex == 2 || noteIndex == 4 || noteIndex == 7 || noteIndex == 9 || noteIndex == 11)
				{
					keys[keyValue]->setColours(Colours::grey, Colours::lightgrey, AlphaColours::lightblue);
				}
				else if (noteIndex == 1)
				{
					
					keys[keyValue]->setColours(AlphaColours::nearlyblack, AlphaColours::verydarkgrey, AlphaColours::lightblue);
					
				}
				else
                {
					keys[keyValue]->setColours(Colours::black, AlphaColours::verydarkgrey, AlphaColours::lightblue);
				}
            }
            
            keyValue = i;
            setNote();
            keys[i]->setColours(AlphaColours::blue, AlphaColours::blue, AlphaColours::lightblue);
        }
	}

}


void GuiPiano::setNote()
{
   
	
	
}



//called when a pad is selected from the pad layout
void GuiPiano::setCurrentlySelectedPad (int padNumber)
{
	
    currentlySelectedPad = padNumber;
	
}


//called when a pad with midi mode on it needs to be displayed
void GuiPiano::updateDisplay()
{
    //if an individual pad number is currently selected (this should always be the case here!)
	if(currentlySelectedPad < 99)
	{
		//noteDisplayType = AppSettings::Instance()->getMidiNoteDisplayType(); //update this!
		
		//get stored note
		int midiNote = PAD_SETTINGS->getMidiNote();
		
		///update label display
		setNoteLabelText(midiNote);
		note = midiNote;
		
		
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
                noteDisplayType = 0; // note number
                setNoteLabelText(note);
                //AppSettings::Instance()->setMidiNoteDisplayType(0); //update this! is it removed now?
            }
            else if (result == 2)
            {
                noteDisplayType = 1; // note name
                setNoteLabelText(note);
                // AppSettings::Instance()->setMidiNoteDisplayType(1); //update this! is it removed now?
            }
        }
    }
}

void GuiPiano::mouseEnter (const MouseEvent &e)
{
    for (int i = 0; i <= 47; i++)
    {
        //===display note number when hovering mouse over a piano key===
        if (keys[i]->isMouseOver(true))
        {
            //int newNote = (transposeValue * 12) + i;
           // setNoteLabelText(newNote);
            midiNoteLabel->setColour(Label::textColourId, Colours::orange);
        }
    }
}


void GuiPiano::mouseExit (const MouseEvent &e)
{
    //=====set key label text to currently selected note=====
    setNoteLabelText(note);
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

