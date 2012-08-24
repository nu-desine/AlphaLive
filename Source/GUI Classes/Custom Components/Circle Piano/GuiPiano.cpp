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
    addAndMakeVisible(midiNoteLabel = new Label("Note Label", String::empty));
    midiNoteLabel->setJustificationType(Justification::horizontallyCentred);
    midiNoteLabel->setFont(Font(12));
    midiNoteLabel->addMouseListener(this, true);
    
    noteDisplayType = StoredSettings::getInstance()->midiNoteDisplayType;
    noteNumber = 60;
	
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
	
    midiNoteLabel->setBounds(560, 578, 30, 30);
}


void GuiPiano::paint (Graphics& g)
{
	/*
	g.setColour(Colours::black);
	g.fillEllipse(520, 542, 90, 90);
		
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse(520, 542, 90, 90, 1.0f);
     */
    
    g.setColour(Colours::black);
	g.fillEllipse(540, 560, 55, 55);
    
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse(540, 560, 55, 55, 1.0f);
	
	
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
            //=========================================================================
            //regular click to set the selected midi pads note or sequencers pad root note
            if (modifier.isCommandDown() == false)
            {
                //first clear all keys
                for (int i = 0; i < 120; i++)
                    setKeyDisplay(i, false);
                selectedKeys.clear();
                
                
                for (int padIndex = 0; padIndex < selectedPads.size(); padIndex++)
                {
                    int padNum = selectedPads[padIndex];
                    
                    //=========================================================================
                    //MIDI MODE REG CLICK
                    if (PAD_SETTINGS->getMode() == 1) //Midi Mode
                    {
                        PAD_SETTINGS->setMidiNote(i);
                        
                        if (padIndex == 0)
                        {
                            setKeyDisplay (i, true);
                            selectedKeys.addIfNotAlreadyThere(i);
                            noteNumber = i;
                        }
                    }
                    
                    //=========================================================================
                    //SEQ MODE REG CLICK
                    else if (PAD_SETTINGS->getMode() == 3) //Sequencer Mode
                    {
                        
                        //get currently set 'root' note
                        int rootNote = PAD_SETTINGS->getSequencerMidiNote(0);
                        //get difference between root note and clicked note
                        int transposeValue = rootNote - i;
                        
                        //check to see if the top row of grid will be transposed
                        //to above 119. If so, don't transpose the notes, and just
                        //redraw/re-set what was last there in the below else statement
                        if (PAD_SETTINGS->getSequencerMidiNote(11)-transposeValue <= 119)
                        {
                            //add the transpose value to each note in the scale/set
                            for (int row = 0; row < 12; row++)
                            {
                                int currentVal = PAD_SETTINGS->getSequencerMidiNote(row);
                                
                                if (currentVal >= 0)
                                {
                                    int newVal = currentVal-transposeValue;
                                    
                                    PAD_SETTINGS->setSequencerMidiNote(newVal, row);
                                    
                                    //update the GUI
                                    if (padIndex == 0)
                                    {
                                        setKeyDisplay (newVal, true);
                                        selectedKeys.addIfNotAlreadyThere(newVal);
                                    }
                                    
                                    if (row == 0)
                                        noteNumber = i;
                                }
                                
                            }
                        }
                        else
                        {
                            //redraw and re-set what was previously there
                            //bit convoluted to clear everything above
                            //just to redraw it in a certain situation.
                            //Is there a better way to code it?
                            for (int row = 0; row < 12; row++)
                            {
                                if (padIndex == 0)
                                {
                                    setKeyDisplay (PAD_SETTINGS->getSequencerMidiNote(row), true);
                                    selectedKeys.addIfNotAlreadyThere(PAD_SETTINGS->getSequencerMidiNote(row));
                                }
                            }
                            
                        }
                        
                        recentlyUpdated = true;
                         
                    }
                }
            
                //break;
            }
            
            //=========================================================================
            //=========================================================================
            //cmd-click to select mutiple notes for midi pads or custom scale for sequencer pads
            else if (modifier.isCommandDown() == true)
            {
                //don't intially clear the keys
                
                //=========================================================================
                //MIDI MODE CMD-CLICK
                
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
        
                //=========================================================================
                //SEQ MODE CMD-CLICK
                
                for (int padIndex = 0; padIndex < selectedPads.size(); padIndex++)
                {
                    int padNum = selectedPads[padIndex];
                    
                    if (PAD_SETTINGS->getMode() == 3) //Sequencer Mode
                    {
                        /*
                         
                         Cmd-click is used to select a custom scale/set of notes for a the sequence grid of a sequencer pad.
                         Click on an unselected key to add it, click on a selected key to remove it.
                         The selected keys/notes will be applied in order of their value to the sequencer rows.
                         If less than 12 notes are selected, the top 'unselected' rows will note have any notes on them.
                         
                         This is the current way that the app handles command-clicks on the piano in seq mode:
                         
                         When the user cmd-clicks a key for the first time (when recentlyUpdated == true) it will clear the piano
                         and allow the user to select 12 individual different keys for each row if the sequence.
                         Each time a key is selected it orders the selectedkey array and
                         applies all the selected keys to the sequence rows.
                         If the number of selected keys is less than the number of rows (12), each row which
                         doesn't currently have a key selected for it is set to an 'off' value of -500.
                         I've chosen 500 as no degree of transposing a note set to -500 could be transposed to 
                         be positive by regualar clicking the piano).
                         The app will view any minus midi note number as 'off', whether its to display the note on the piano
                         or play the note as a midi message.
                         
                         This method seems to make more sense over the other possible methods:
                         1. any 'unchosen' note is set to a default note such as 0 or 60. This is flawed as when transposing
                         the selection it will transpose the default note which might not make sense, plus when
                         ordering the selected keys the default note could be layed out somewhere in the middle of the
                         selected notes, which doesn't make sense as any unselected notes should be set to the top of the grid.
                         2. any 'unchosen' note is kept the same as the previous note. This also flawed because of the 
                         second reason in the point above.
                         */
                        
                        //=========== click on unselected key ===============
                        
                        if (keys[i]->getToggleState() == true) //if was previous off and clicked
                        {
                            
                            if (recentlyUpdated == true && selectedKeys.size() == 12) // so th cmd-click doesn't delete if incomplete set
                            {
                                //first clear all keys
                                for (int i = 0; i < 120; i++)
                                    setKeyDisplay(i, false);
                                selectedKeys.clear();
                                
                            }
                            
                            recentlyUpdated = false;
                            
                            //if the number of selected keys is less than the number of sequencer rows
                            if (selectedKeys.size() < 12)
                            {
                                //update the GUI
                                if (padIndex == 0)
                                {
                                    setKeyDisplay (i, true);
                                    selectedKeys.addIfNotAlreadyThere(i);
                                    
                                    //sort selectedKeys into order.
                                    //The below loop checks the recently added key/note
                                    //and compares in to the previous key and moves
                                    //it to the correct location in the array 
                                    
                                    for (int x = selectedKeys.size() - 1; x >= 0; x--)
                                    {
                                        if (selectedKeys[x] < selectedKeys[x-1])
                                        {
                                            selectedKeys.swap(x, x-1);
                                        }
                                    }
                                }
                                
                                for (int row = 0; row < 12; row++)
                                {
                                    int note = selectedKeys[row];
                                    
                                    if (row > selectedKeys.size()-1)
                                        note = -500; //'off' note
                                    
                                    PAD_SETTINGS->setSequencerMidiNote(note, row);
                                    //std::cout << "row " << row << " set to note : " << note << std::endl;
                                }
                                
                                //std::cout << std::endl;
                            }
                        }
                        
                        //=========== click on selected key ===============
                        
                        else if (keys[i]->getToggleState() == false) //if was previous on and clicked 
                        {
                            recentlyUpdated = false;
                            
                            //update the GUI (only need to do this a single time)
                            if (padIndex == 0)
                            {
                                setKeyDisplay (i, false);
                                selectedKeys.removeFirstMatchingValue(i);
                                
                                //Don't need to sort here do I? 
                                //Removing an element will resize the array,
                                //which will cause the top row to be set to note 'off'
                            }
                            
                            for (int row = 0; row < 12; row++)
                            {
                                int note = selectedKeys[row];
                                
                                if (row > selectedKeys.size()-1)
                                    note = -500; //'off' note
                                
                                PAD_SETTINGS->setSequencerMidiNote(note, row);
                                //std::cout << "row " << row << " set to note : " << note << std::endl;
                            }
                            
                            //std::cout << std::endl;
                         
                        }  
                    }
                }
            }
                
            break;
            //=========================================================================
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
    //recentlyUpdated = true;
    
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
            
            noteNumber = PAD_SETTINGS->getMidiNote();
        }
        
        else if (PAD_SETTINGS->getMode() == 3 && PAD_SETTINGS->getSequencerMode() == 1) //Midi Sequencer Mode
        {
            for (int i = 0; i < 12; i++)
            {
                if (PAD_SETTINGS->getSequencerMidiNote(i) >= 0)
                {
                    setKeyDisplay (PAD_SETTINGS->getSequencerMidiNote(i), true);
                    selectedKeys.addIfNotAlreadyThere(PAD_SETTINGS->getSequencerMidiNote(i));
                    
                }
            }
            
            noteNumber = PAD_SETTINGS->getSequencerMidiNote(0);
        }
        
        setNoteLabelText(noteNumber);
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
                
                noteNumber = -1;
            }
        }
        
        else if (PAD_SETTINGS->getMode() == 3 && PAD_SETTINGS->getSequencerMode() == 1) //Midi Sequencer Mode
        {
            //what should I display here?
            noteNumber = -1;
        }
        
        setNoteLabelText(noteNumber);
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
            
            if (noteDisplayType == 2)
                menu.addItem(1, translate("Display MIDI note numbers"));
            else
                menu.addItem(2, translate("Display MIDI note names"));
            
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
            midiNoteLabel->setColour(Label::textColourId, AlphaColours::blue);
            setNoteLabelText(i);
            break;
        }
    }
}


void GuiPiano::mouseExit (const MouseEvent &e)
{
    //=====set key label text to currently selected note=====
    setNoteLabelText(noteNumber);
    midiNoteLabel->setColour(Label::textColourId, LookAndFeel::getDefaultLookAndFeel().findColour(Label::textColourId));
}


void GuiPiano::setNoteLabelText (int note)
{
    if (note >= 0)
    {
        if (noteDisplayType == 1)
        {
            midiNoteLabel->setText(String(note), false);
        }
        else if (noteDisplayType == 2)
        {
            midiNoteLabel->setText(MidiMessage::getMidiNoteName(note, true, true, 3), false);
        }
    }
    else
    {
        midiNoteLabel->setText(String::empty, false); 
    }
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
		
		int noteIndex = ((keyNumber + 1) % 12);		
        
        //set the key/button colour
        if(noteIndex == 2 || noteIndex == 4 || noteIndex == 7 || noteIndex == 9 || noteIndex == 11)
        {
			
			keys[keyNumber]->setColours(AlphaColours::lightblue, AlphaColours::lightblue, AlphaColours::lightblue);  

			
		}
		else {
			
			keys[keyNumber]->setColours(AlphaColours::blue, AlphaColours::blue, AlphaColours::lightblue);  
			
		}

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
