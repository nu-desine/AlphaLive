//
//  GuiCirclePianoOneOctave.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 09/01/2012.
//  Copyright 2012 nu desine.
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

#include "GuiCirclePianoOneOctave.h"
#include "../../Binary Data/CirclePianoBinaryData.h"
#include "../../../Functionality Classes/Other/LayoutsAndScales.cpp"
#include "../../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[currentlySelectedPad]
#define PAD_SETTINGS_i AppSettings::Instance()->padSettings[i]

GuiCirclePianoOneOctave::GuiCirclePianoOneOctave() : Component ("GuiCirclePianoOneOctave")

{
    //background graphics
	image = ImageFileFormat::loadFrom(CirclePianoBinaryData::fatcirclepiano_png, CirclePianoBinaryData::fatcirclepiano_pngSize);
    
    //layout menu
    addAndMakeVisible(layoutMenu = new ComboBox());
    layoutMenu->addListener(this);
    layoutMenu->addMouseListener(this, true);
    
    layoutMenu->addItem("None Selected...", 100);
    layoutMenu->addSeparator();
    layoutMenu->addItem("Major", 1);
    layoutMenu->addItem("Minor", 2);
    layoutMenu->addItem("Melodic Minor", 3);
    layoutMenu->addItem("Harmonic Minor", 4);
    layoutMenu->addItem("Rock n Roll", 5);
    layoutMenu->addSeparator();
    layoutMenu->addItem("Chromatic", 11);
    layoutMenu->addItem("Chromatic (Two-Row Layout)", 10);
    layoutMenu->addItem("Harmonic Table", 6);
    layoutMenu->addItem("Harmonic Table 2", 7);
    layoutMenu->addItem("Wicki-Hayden", 9);
    layoutMenu->addItem("Perfect Fourths", 8);
    layoutMenu->setSelectedId(100, true);
    
    //note display label
    addAndMakeVisible(keyLabel = new Label("Key Label", "C"));
    keyLabel->setJustificationType(Justification::horizontallyCentred);
    keyLabel->setColour(Label::textColourId, Colours::lightgrey);
    //Font newFont;
    //newFont.setTypefaceName("Grixel Acme 7 Wide");
    //keyLabel->setFont(newFont);
	
	
	keyOffIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey0off_png, CirclePianoBinaryData::fatkey0off_pngSize)));
	keyOffIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey1off_png, CirclePianoBinaryData::fatkey1off_pngSize)));
	keyOffIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey2off_png, CirclePianoBinaryData::fatkey2off_pngSize)));
	keyOffIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey3off_png, CirclePianoBinaryData::fatkey3off_pngSize)));
	keyOffIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey4off_png, CirclePianoBinaryData::fatkey4off_pngSize)));
	keyOffIm.insert(5, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey5off_png, CirclePianoBinaryData::fatkey5off_pngSize)));
	keyOffIm.insert(6, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey6off_png, CirclePianoBinaryData::fatkey6off_pngSize)));
	keyOffIm.insert(7, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey7off_png, CirclePianoBinaryData::fatkey7off_pngSize)));
	keyOffIm.insert(8, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey8off_png, CirclePianoBinaryData::fatkey8off_pngSize)));
	keyOffIm.insert(9, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey9off_png, CirclePianoBinaryData::fatkey9off_pngSize)));
	keyOffIm.insert(10, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey10off_png, CirclePianoBinaryData::fatkey10off_pngSize)));
	keyOffIm.insert(11, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey11off_png, CirclePianoBinaryData::fatkey11off_pngSize)));
    
    keyOverIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey0over_png, CirclePianoBinaryData::fatkey0over_pngSize)));
	keyOverIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey1over_png, CirclePianoBinaryData::fatkey1over_pngSize)));
	keyOverIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey2over_png, CirclePianoBinaryData::fatkey2over_pngSize)));
	keyOverIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey3over_png, CirclePianoBinaryData::fatkey3over_pngSize)));
	keyOverIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey4over_png, CirclePianoBinaryData::fatkey4over_pngSize)));
	keyOverIm.insert(5, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey5over_png, CirclePianoBinaryData::fatkey5over_pngSize)));
	keyOverIm.insert(6, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey6over_png, CirclePianoBinaryData::fatkey6over_pngSize)));
	keyOverIm.insert(7, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey7over_png, CirclePianoBinaryData::fatkey7over_pngSize)));
	keyOverIm.insert(8, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey8over_png, CirclePianoBinaryData::fatkey8over_pngSize)));
	keyOverIm.insert(9, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey9over_png, CirclePianoBinaryData::fatkey9over_pngSize)));
    keyOverIm.insert(10, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey10over_png, CirclePianoBinaryData::fatkey10over_pngSize)));
	keyOverIm.insert(11, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey11over_png, CirclePianoBinaryData::fatkey11over_pngSize)));
    
    keyDownIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey0down_png, CirclePianoBinaryData::fatkey0down_pngSize)));
	keyDownIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey1down_png, CirclePianoBinaryData::fatkey1down_pngSize)));
	keyDownIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey2down_png, CirclePianoBinaryData::fatkey2down_pngSize)));
	keyDownIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey3down_png, CirclePianoBinaryData::fatkey3down_pngSize)));
	keyDownIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey4down_png, CirclePianoBinaryData::fatkey4down_pngSize)));
	keyDownIm.insert(5, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey5down_png, CirclePianoBinaryData::fatkey5down_pngSize)));
	keyDownIm.insert(6, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey6down_png, CirclePianoBinaryData::fatkey6down_pngSize)));
	keyDownIm.insert(7, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey7down_png, CirclePianoBinaryData::fatkey7down_pngSize)));
	keyDownIm.insert(8, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey8down_png, CirclePianoBinaryData::fatkey8down_pngSize)));
	keyDownIm.insert(9, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey9down_png, CirclePianoBinaryData::fatkey9down_pngSize)));
	keyDownIm.insert(10, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey10down_png, CirclePianoBinaryData::fatkey10down_pngSize)));
	keyDownIm.insert(11, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey11down_png, CirclePianoBinaryData::fatkey11down_pngSize)));
    
    keyOnIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey0on_png, CirclePianoBinaryData::fatkey0on_pngSize)));
	keyOnIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey1on_png, CirclePianoBinaryData::fatkey1on_pngSize)));
	keyOnIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey2on_png, CirclePianoBinaryData::fatkey2on_pngSize)));
	keyOnIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey3on_png, CirclePianoBinaryData::fatkey3on_pngSize)));
	keyOnIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey4on_png, CirclePianoBinaryData::fatkey4on_pngSize)));
    keyOnIm.insert(5, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey5on_png, CirclePianoBinaryData::fatkey5on_pngSize)));
	keyOnIm.insert(6, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey6on_png, CirclePianoBinaryData::fatkey6on_pngSize)));
	keyOnIm.insert(7, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey7on_png, CirclePianoBinaryData::fatkey7on_pngSize)));
	keyOnIm.insert(8, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey8on_png, CirclePianoBinaryData::fatkey8on_pngSize)));
	keyOnIm.insert(9, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey9on_png, CirclePianoBinaryData::fatkey9on_pngSize)));
	keyOnIm.insert(10, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey10on_png, CirclePianoBinaryData::fatkey10on_pngSize)));
	keyOnIm.insert(11, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatkey11on_png, CirclePianoBinaryData::fatkey11on_pngSize)));
    
    
    
    transposeOffIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb0off_png, CirclePianoBinaryData::fatb0off_pngSize)));
	transposeOffIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb1off_png, CirclePianoBinaryData::fatb1off_pngSize)));
	transposeOffIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb2off_png, CirclePianoBinaryData::fatb2off_pngSize)));
	transposeOffIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb3off_png, CirclePianoBinaryData::fatb3off_pngSize)));
	transposeOffIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb4off_png, CirclePianoBinaryData::fatb4off_pngSize)));
	
	transposeOverIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb0over_png, CirclePianoBinaryData::fatb0over_pngSize)));
	transposeOverIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb1over_png, CirclePianoBinaryData::fatb1over_pngSize)));
	transposeOverIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb2over_png, CirclePianoBinaryData::fatb2over_pngSize)));
	transposeOverIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb3over_png, CirclePianoBinaryData::fatb3over_pngSize)));
	transposeOverIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb4over_png, CirclePianoBinaryData::fatb4over_pngSize)));
	
	transposeDownIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb0down_png, CirclePianoBinaryData::fatb0down_pngSize)));
	transposeDownIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb1down_png, CirclePianoBinaryData::fatb1down_pngSize)));
	transposeDownIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb2down_png, CirclePianoBinaryData::fatb2down_pngSize)));
	transposeDownIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb3down_png, CirclePianoBinaryData::fatb3down_pngSize)));
	transposeDownIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::fatb4down_png, CirclePianoBinaryData::fatb4down_pngSize)));
	
	for (int i = 0; i <= 4; i++)
    {
		buttons.insert(i, new GuiFxButtons(transposeOffIm[i], transposeOverIm[i], transposeDownIm[i]));
		addAndMakeVisible(buttons[i]);
		buttons[i]->setRadioGroupId (600);
		buttons[i]->addListener(this);
		buttons[i]->setOpaque(false);
	}
    
    //set '0' octave as default
    buttons[2]->setToggleState(true, false);
    octaveValue = 0; 
    
	for (int i = 0; i <= 11; i++) 
    {
		keys.insert(i, new GuiPianoKeys(keyOffIm[i], keyOverIm[i], keyDownIm[i], keyOnIm[i]));
		addAndMakeVisible(keys[i]);
		keys[i]->setRadioGroupId (12);
		keys[i]->addListener(this);
        keys[i]->addMouseListener(this, true);
		keys[i]->setOpaque(false);
	}
    
    //set key to C
    keys[0]->setToggleState(true, false);
    keyValue = 24; //'C0'
	
	hitPath.addEllipse (0.0f, 0.0f, 230.0f, 230.0f);
}

GuiCirclePianoOneOctave::~GuiCirclePianoOneOctave()
{
    buttons.clear();
    keys.clear();
	deleteAllChildren();
}


void GuiCirclePianoOneOctave::resized()
{
	hitPath.addEllipse(0.0f, 0.0f, getWidth(), getHeight());
    layoutMenu->setBounds((getWidth()/2)-50, 73, 100, 15);
    keyLabel->setBounds((getWidth()/2)-25, getHeight()-70, 50, 20);
	
	
}

void GuiCirclePianoOneOctave::paint (Graphics& g)
{
	
	
	g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 0, 800, 800);
	
	for (int i = 0; i <= 11; i++) 
    {
		keys[i]->setBounds(0.0f, 0.0f, getWidth(), getHeight());
	}
	
	for (int i = 0; i <= 4; i++) 
    {
		buttons[i]->setBounds(0.0f, 0.0f, getWidth(), getHeight());
	}
	
    
}

void GuiCirclePianoOneOctave::buttonClicked(Button *button)
{
	
	for (int i = 0; i <=4; i++)
    {
        //octave buttons
        if (button == buttons[i])
		{
			octaveValue = i-2;//to get the values in the range of -2 to +2
            setLayout();
		}
	}
	
    
	for (int i = 0; i <=11; i++)
    {
        //piano keys
		if (button == keys[i])
		{
            keyValue = i+24; //represents a MIDI note number
            setLayout();
		}
	}
}

void GuiCirclePianoOneOctave::comboBoxChanged (ComboBox* comboBox)
{
    if (comboBox == layoutMenu)
    {
        setLayout();
    }
}


void GuiCirclePianoOneOctave::setLayout()
{
    //get the 'root note' (note for pad 0) of the layout based on the selected octave and selected key
    int rootNote = (12 * octaveValue) + keyValue;
    
    //apply the layout based on the rootNote and selected scale from the relevant combobox
    if (layoutMenu->getSelectedId() == 1) //Major scale 
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::major[i] + rootNote);
        }
    }
    else if (layoutMenu->getSelectedId() == 2) //Minor scale 
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::minor[i] + rootNote);
        }
    }
    else if (layoutMenu->getSelectedId() == 3) //Melodic Minor scale 
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::melodicMinor[i] + rootNote);
        }
    }
    else if (layoutMenu->getSelectedId() == 4) //Harmonic Minor scale 
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::harmonicMinor[i] + rootNote);
        }
    }
    else if (layoutMenu->getSelectedId() == 5) //Rock n Roll scale 
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::rockNRoll[i] + rootNote);
        }
    }
    
    else if (layoutMenu->getSelectedId() == 11) //chromatic layout
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::chromatic[i] + rootNote); 
            
        }
    }
    else if (layoutMenu->getSelectedId() == 10) //Two-row chromatic layout
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::chromaticTwoRow[i] + rootNote); 
            
        }
    }
    else if (layoutMenu->getSelectedId() == 6) //Harmonic Table layout
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::harmonicTable1[i] + rootNote); 
                                                                            
        }
    }
    else if (layoutMenu->getSelectedId() == 7) //Harmonic Table 2 
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::harmonicTable2[i] + rootNote - 24);
        }
    }
    else if (layoutMenu->getSelectedId() == 9) //Wicki-Hayden 
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::wickiHayden[i] + rootNote - 24);
        }
    }
    
    else if (layoutMenu->getSelectedId() == 8) //Perfect fourths layout
    {
        for(int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setMidiNote(Layouts::perfectFourths[i] + rootNote); 
            
        }
    }
     
    

    //=====set key label text=====
    String noteName = MidiMessage::getMidiNoteName(keyValue, true, false, 0);
    keyLabel->setText(String(noteName), false);
}


bool GuiCirclePianoOneOctave::hitTest (int x, int y)
{
	return hitPath.contains(x, y);
}


//called when a pad is selected from the pad layout
void GuiCirclePianoOneOctave::setCurrentlySelectedPad (int padNumber)
{
    currentlySelectedPad = padNumber;
}

//called when a pad with midi mode on it needs to be displayed
void GuiCirclePianoOneOctave::updateDisplay()
{
    layoutMenu->setSelectedId(100, true);
    buttons[2]->setToggleState(true, false);
    keys[0]->setToggleState(true, false);
    keyLabel->setText("C", false);
    keyValue = 24;
    octaveValue = 0;

}


void GuiCirclePianoOneOctave::mouseEnter (const MouseEvent &e)
{
    for (int i = 0; i <= 11; i++)
    {
        //===display key when hovering mouse over a piano key===
        if (keys[i]->isMouseOver(true))
        {
            String noteName = MidiMessage::getMidiNoteName(i, true, false, 3);
            keyLabel->setText(String(noteName), false);
            keyLabel->setColour(Label::textColourId, Colours::orange);
        }
    }
}
void GuiCirclePianoOneOctave::mouseExit (const MouseEvent &e)
{
    //=====set key label text to currently selected key=====
    String noteName = MidiMessage::getMidiNoteName(keyValue, true, false, 3);
    keyLabel->setText(String(noteName), false);
    keyLabel->setColour(Label::textColourId, Colours::lightgrey);
}

/*
 else if (octaveSlider->isMouseOver(true))
 {
 mainComponentRef.setInfoTextBoxText("Note Layout Octave Selector. \nSets the octave of the selected note layout for the whole set of pads.");
 }
 else if (layoutKeyMenu->isMouseOver(true))
 {
 mainComponentRef.setInfoTextBoxText("Note Layout Key Selector. \nSets the key of the selected note layout for the whole set of pads.");
 }
 else if (layoutMenu->isMouseOver(true))
 {
 mainComponentRef.setInfoTextBoxText("Note Layout Drop-down Menu. \nSets the MIDI note layout for the whole set of pads.");
 }
 else if (octaveSlider->isMouseOver(true))
 {
 mainComponentRef.setInfoTextBoxText("Note Layout Octave Selector. \nSets the octave of the selected note layout for the whole set of pads.");
 }
*/
