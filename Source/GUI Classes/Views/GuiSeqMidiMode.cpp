//
//  TestGuiSeqMidiMode.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 17/10/2011.
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

#include "GuiSeqMidiMode.h"
#include "../../File and Settings/AppSettings.h"
#include "../../Functionality Classes/Other/LayoutsAndScales.cpp"
#include "GlobalValues.h"
#include "MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)

GuiSeqMidiMode::GuiSeqMidiMode(MainComponent &ref)
                                        :   mainComponentRef(ref)
{
    
    for (int row = 0, i = 60; row <= NO_OF_ROWS-1; row++)
    {
        addAndMakeVisible(noteSlider[row] = new Slider());
        noteSlider[row]->setSliderStyle(Slider::IncDecButtons);
        noteSlider[row]->setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
        noteSlider[row]->setRange(0, 127, 1);
        noteSlider[row]->addListener(this);
        noteSlider[row]->setValue(i, false);
        noteSlider[row]->setVisible(false);
        noteSlider[row]->addMouseListener(this, true);
        
        i++;
    }
    
    
    addAndMakeVisible(scaleMenu = new ComboBox());
    scaleMenu->addListener(this);
    scaleMenu->addItem("None Selected...", 100);
    scaleMenu->addItem("Chromatic", 1);
    scaleMenu->addItem("Major", 2);
    scaleMenu->addItem("Major Pentatonic", 3);
    scaleMenu->addItem("Minor", 4);
    scaleMenu->addItem("Minor Pentatonic", 5);
    scaleMenu->addItem("Melodic Minor", 6);
    scaleMenu->addItem("Harmonic Minor", 7);
    scaleMenu->addItem("Blues", 8);
    scaleMenu->addItem("Rock n' Roll", 9);
    scaleMenu->addItem("Hungarian Gypsy", 10);
    scaleMenu->addItem("Raga Madhuri", 11);
    scaleMenu->setSelectedId(100, true);
    scaleMenu->addMouseListener(this, true);
     
    
    addAndMakeVisible(rootNoteSlider = new AlphaSlider());
    rootNoteSlider->setRange(0, 127, 1);
    rootNoteSlider->addListener(this);
    rootNoteSlider->setValue(60, false);
    rootNoteSlider->addMouseListener(this, true);
    
    addAndMakeVisible(velocitySlider = new AlphaSlider());
    velocitySlider->setRange(0, 127, 1);
    velocitySlider->addListener(this);
    velocitySlider->setValue(110, false);
    velocitySlider->addMouseListener(this, true);
    
    addAndMakeVisible(channelSlider = new AlphaSlider());
    channelSlider->setRange(1, 16, 1);
    channelSlider->addListener(this);
    channelSlider->setValue(1, false);
    channelSlider->addMouseListener(this, true);
    
    addAndMakeVisible(noteLengthSlider = new AlphaSlider());
    noteLengthSlider->setRange(1, 32, 1);
    noteLengthSlider->addListener(this);
    noteLengthSlider->setValue(4, false);
    noteLengthSlider->addMouseListener(this, true);
    
    
}

GuiSeqMidiMode::~GuiSeqMidiMode()
{
    deleteAllChildren();
    
}

void GuiSeqMidiMode::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    
}
void GuiSeqMidiMode::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/dissabled any unneeded components. 
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        //get the settings of the pad selected (in padMenu ComboBox)
        //set the stored values on the GUI components
        //Don't broadcast any changes to the component Listeners. Only want to update the GUI here
       
        scaleMenu->setSelectedId(100, true);
        rootNoteSlider->setValue(60,false);
        channelSlider->setValue(PAD_SETTINGS->getSequencerMidiChannel(), false);
        velocitySlider->setValue(PAD_SETTINGS->getSequencerMidiVelocity(), false);
        noteLengthSlider->setValue(PAD_SETTINGS->getSequencerMidiNoteLength(), false);
        
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            noteSlider[row]->setValue(PAD_SETTINGS->getSequencerMidiNote(row), false);
        }
  
    }
    
    
    else if(MULTI_PADS)
    {
        //set to a default setting
        scaleMenu->setSelectedId(100, true);
        rootNoteSlider->setValue(60, false);
        channelSlider->setValue(1, false);
        velocitySlider->setValue(110, false);
        noteLengthSlider->setValue(4, false);
        
        for (int row = 0, i = 60; row <= NO_OF_ROWS-1; row++)
        {
            noteSlider[row]->setValue(i, false);
            
            i++;
        }
        
    }

    
}

void GuiSeqMidiMode::resized()
{
    for (float row = NO_OF_ROWS-1, i = 415.0; row >= 0; row--)
    {
        noteSlider[int(row)]->setBounds(680, i, 75, 10);
        
        i += 12.5;
    }
    
    
    scaleMenu->setBounds(RIGHT_CIRCLE_X, 450, COMPONENT_W, COMPONENT_H);
    rootNoteSlider->setBounds(RIGHT_CIRCLE_X, 480, COMPONENT_W, COMPONENT_H);
    channelSlider->setBounds(RIGHT_CIRCLE_X, 510, COMPONENT_W, COMPONENT_H);
    //velocitySlider->setBounds(RIGHT_CIRCLE_X, 540, COMPONENT_W, COMPONENT_H);
    noteLengthSlider->setBounds(RIGHT_CIRCLE_X, 570, COMPONENT_W, COMPONENT_H);
}




void GuiSeqMidiMode::comboBoxChanged (ComboBox* comboBox)
{
    if (comboBox == scaleMenu)
    {
        setScale();
    }
    
}

void GuiSeqMidiMode::sliderValueChanged (Slider* slider)
{
    if (slider == channelSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMidiChannel(channelSlider->getValue());
        }
        
    }
    
    
    if (slider == velocitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMidiVelocity(velocitySlider->getValue());
        }
        
    }
    
    
    if (slider == noteLengthSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMidiNoteLength(noteLengthSlider->getValue());
        }
        
    }

    
    
    
    
    if (slider == rootNoteSlider)
    {
        setScale();
    }
    
    
    
    
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        if (slider == noteSlider[row])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSequencerMidiNote(noteSlider[row]->getValue(), row);
            }
        }
    }
    
    

}

//this function is called when the sequenceLength slider value is changed in GuiSequencerMode (parent class)
//it sets the range of the noteLengthSlider so that the max value matches the sequenceLengthSlider
//and sets the pad/s corresponding values to the max value if they are now out of range, as well as the slider display

//can't just check if the slider value is out of range and change the display and send an update if it is,
//as if a row or global is currently selected the slider value would be set to default and will mistakenly set pad values
void GuiSeqMidiMode::setNoteLengthSliderRange (int maxValue)
{
    
    //set the range
    noteLengthSlider->setRange(1, maxValue, 1);
    
    for (int i = 0; i < selectedPads.size(); i++)
    {
        int padNum = selectedPads[i];
        
        if (PAD_SETTINGS->getSequencerMidiNoteLength() > maxValue) //if now out of range
        {
            PAD_SETTINGS->setSequencerMidiNoteLength(maxValue); //set the PAD_SETTINGS value to the new maxValue
            noteLengthSlider->setValue(maxValue, false); //update the slider display, but don;t send an update
        }
    }

    
}


void GuiSeqMidiMode::buttonClicked (Button* button)
{
    
}


//called when either the scale combobox or the root note slider is changed
void GuiSeqMidiMode::setScale()
{
    //get the selected scale and store it into 'selectedScale' array
    int selectedScale[NO_OF_ROWS];
    bool doNothing = false;
    
    for (int i = 0; i <=NO_OF_ROWS-1; i++)
    {
        switch (scaleMenu->getSelectedId()) 
        {
            case 1:
                selectedScale[i] = Scales::chromatic[i]+rootNoteSlider->getValue();
                break;
            case 2:
                selectedScale[i] = Scales::major[i]+rootNoteSlider->getValue();
                break;
            case 3:
                selectedScale[i] = Scales::majorPentatonic[i]+rootNoteSlider->getValue();
                break;
            case 4:
                selectedScale[i] = Scales::minor[i]+rootNoteSlider->getValue();
                break;
            case 5:
                selectedScale[i] = Scales::minorPentatonic[i]+rootNoteSlider->getValue();
                break;
            case 6:
                selectedScale[i] = Scales::melodicMinor[i]+rootNoteSlider->getValue();
                break;
            case 7:
                selectedScale[i] = Scales::harmonicMinor[i]+rootNoteSlider->getValue();
                break;
            case 8:
                selectedScale[i] = Scales::blues[i]+rootNoteSlider->getValue();
                break;
            case 9:
                selectedScale[i] = Scales::rockNRoll[i]+rootNoteSlider->getValue();
                break;
            case 10:
                selectedScale[i] = Scales::hungarianGypsy[i]+rootNoteSlider->getValue();
                break;
            case 11:
                selectedScale[i] = Scales::ragaMadhuri[i]+rootNoteSlider->getValue();
                break;
                selectedScale[i] = Scales::chromatic[i]+rootNoteSlider->getValue();
            default:
                doNothing = true; //this sets it so no scale will be applied below
                break;
        }
    }
    
    if (doNothing == false)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            
            for (int row = 0; row <= NO_OF_ROWS-1; row++)
            {
                PAD_SETTINGS->setSequencerMidiNote(selectedScale[row], row);
                noteSlider[row]->setValue(selectedScale[row], false);
                
            }
            
        }
        

    }
}


void GuiSeqMidiMode::setComponentsVisible(bool isVisible)
{
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        noteSlider[row]->setVisible(isVisible);
        
    } 
}


void GuiSeqMidiMode::mouseEnter (const MouseEvent &e)
{
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        if (noteSlider[row]->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText("Row " + String(row+1) + " Note Selector. Sets and displays the MIDI note number of sequencer grid row " + String(row+1) + " for the selected pad/pads. Sequencer MIDI notes can also be set using the Scale Drop-Down Menu.");
        }
    }
    
    if (scaleMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Scale Drop-Down Menu. Sets a series of MIDI notes for the sequencer grid for the selected pad/pads. Note's can be individually set and displayed using the Note Selector controls to the right-side of the sequencer grid.");
    }
    else if (rootNoteSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Scale Root-Note Selector. Sets the root MIDI note of the selected scale for the sequencer grid for the selected pad/pads.");
    }
    else if (channelSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("MIDI Channel Selector. Sets the MIDI channel number for the MIDI sequence of the selected pad/pads.");
    }
    else if (velocitySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("MIDI Velocity Selector. Sets the MIDI velocity for the MIDI sequence of the selected pad/pads.");
    }
    else if (noteLengthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("MIDI Note Length Selector. Sets the MIDI note length (in beats/steps) for the MIDI sequence of the selected pad/pads. The note length can be no longer than the sequence length set using the Sequence Length Selector.");
    }
    
    
    
}
void GuiSeqMidiMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}