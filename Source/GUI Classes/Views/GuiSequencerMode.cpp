//
//  TestGuiSequencerMode.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 25/09/2011.
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

#include "GuiSequencerMode.h"
#include "../../Functionality Classes/Sequencer Mode/SequencerValues.h"
#include "../../File and Settings/AppSettings.h"
#include "../Binary Data/ImageSliderBinaryData.h"
#include "../Binary Data/BinaryData.h"
#include "GlobalValues.h"
#include "MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)
#define UPDATE_TIME 50

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GuiSequencerMode::GuiSequencerMode(ModeSequencer &ref, MainComponent &ref2, AppDocumentState &ref3)
                                            :   modeSequencerRef(ref),
                                                mainComponentRef(ref2),
                                                appDocumentStateRef(ref3)
{
    //create pop-up window
    addAndMakeVisible(popUpWindow = new GuiPopUpWindow());
    popUpWindow->setVisible(false);
    popUpWindow->setInterceptsMouseClicks(false, true);
    
    //general graphics stuff
    addAndMakeVisible(circleBackgroundLeft = new GuiCircleBackground());
    addAndMakeVisible(circleBackgroundRight = new GuiCircleBackground());
    
    //mode GUIs
    addAndMakeVisible(midiMode = new GuiSeqMidiMode(mainComponentRef));
    midiMode->setVisible(false);
    midiMode->setInterceptsMouseClicks(false, true);
    addAndMakeVisible(samplesMode = new GuiSeqSamplesMode(mainComponentRef));
    samplesMode->setVisible(true);
    samplesMode->setInterceptsMouseClicks(false, true);
    
    //mode buttons
    
    //create midi mode button
	Image *midiModeImage = new Image(ImageFileFormat::loadFrom(BinaryData::midisymbol_png, BinaryData::midisymbol_pngSize)); 
	addAndMakeVisible(modeMidiButton = new ModeButton(midiModeImage));
    modeMidiButton->setClickingTogglesState(true);
    modeMidiButton->setToggleState(true, false);
	modeMidiButton->addListener(this);
    modeMidiButton->setOpaque(false);
	modeMidiButton->setRadioGroupId (1234);
    modeMidiButton->addMouseListener(this, true);
    
    //create looper mode button
	Image *samplesModeImage = new Image(ImageFileFormat::loadFrom(BinaryData::loopsymbol_png, BinaryData::loopsymbol_pngSize)); 
	addAndMakeVisible(modeSamplesButton = new ModeButton(samplesModeImage));
    modeSamplesButton->setClickingTogglesState(true);
	modeSamplesButton->addListener(this);
    modeSamplesButton->setOpaque(false);
	modeSamplesButton->setRadioGroupId (1234);
    modeSamplesButton->addMouseListener(this, true);
    
    //grid
    addAndMakeVisible(sequencerGrid = new SequencerGrid(modeSequencerRef, *this));
    sequencerGrid->addMouseListener(this, true);
    
    //grid display button
    Image *sequencerGridButtonImage = new Image(ImageFileFormat::loadFrom(BinaryData::showseqsymbol_png, BinaryData::showseqsymbol_pngSize)); 
	addAndMakeVisible(sequencerGridToggleButton = new ModeButton(sequencerGridButtonImage));
    sequencerGridToggleButton->setClickingTogglesState(true);
    sequencerGridToggleButton->setToggleState(false, false);
	sequencerGridToggleButton->addListener(this);
    sequencerGridToggleButton->addMouseListener(this, true);
    
    
    //other components
    
    //channel/group slider
    Image *dialImage = new Image(ImageFileFormat::loadFrom(ImageSliderBinaryData::channeldial_png, ImageSliderBinaryData::channeldial_pngSize));
    addAndMakeVisible(channelSlider = new ImageSlider(dialImage, 16, true));
    channelSlider->setSliderStyle(Slider::Rotary);
    channelSlider->setRotaryParameters(0, M_PI*2, false);
    channelSlider->setRange(1, 16, 1);
    channelSlider->addListener(this);
    channelSlider->addMouseListener(this, true);
    
    addAndMakeVisible(clearButton = new TextButton("Clear"));
    clearButton->addListener(this);
    clearButton->addMouseListener(this, true);
    addAndMakeVisible(clearAllButton = new TextButton("Clear All"));
    clearAllButton->addListener(this);
    clearAllButton->addMouseListener(this, true);
    
    addAndMakeVisible(saveSeqButton = new TextButton("Save Sequence"));
    saveSeqButton->addListener(this);
    saveSeqButton->addMouseListener(this, true);
    addAndMakeVisible(saveSeqSetButton = new TextButton("Save Sequence Set"));
    saveSeqSetButton->addListener(this);
    saveSeqSetButton->addMouseListener(this, true);
    addAndMakeVisible(loadSeqButton = new TextButton("Load Sequence"));
    loadSeqButton->addListener(this);
    loadSeqButton->addMouseListener(this, true);
    addAndMakeVisible(loadSeqSetButton = new TextButton("Load Sequence Set"));
    loadSeqSetButton->addListener(this);
    loadSeqSetButton->addMouseListener(this, true);
    
    addAndMakeVisible(currentSequenceNumberSlider = new AlphaSlider());
    currentSequenceNumberSlider->setRange(1, NO_OF_SEQS, 1);
    currentSequenceNumberSlider->addListener(this);
    currentSequenceNumberSlider->setValue(1, false);
    currentSequenceNumberSlider->addMouseListener(this, true);
    
    addAndMakeVisible(sequenceLengthSlider = new AlphaSlider());
    sequenceLengthSlider->setRange(1, NO_OF_COLUMNS, 1);
    sequenceLengthSlider->addListener(this);
    sequenceLengthSlider->setValue(NO_OF_COLUMNS, false);
    sequenceLengthSlider->addMouseListener(this, true);
    
    addAndMakeVisible(numberOfSequencesSlider = new AlphaSlider());
    numberOfSequencesSlider->setRange(1, NO_OF_SEQS, 1);
    numberOfSequencesSlider->addListener(this);
    numberOfSequencesSlider->setValue(NO_OF_SEQS, false);
    numberOfSequencesSlider->addMouseListener(this, true);
    
    addAndMakeVisible(playStateMenu = new ComboBox());
    playStateMenu->addListener(this);
    playStateMenu->addItem("Play Once", 1);
    playStateMenu->addItem("Play Once (Continuous)", 2);
    playStateMenu->addItem("Play Once (Non-Destructive)", 21);
    playStateMenu->addItem("Loop", 3);
    playStateMenu->addItem("Toggle (Press-Off)", 4);
    playStateMenu->addItem("Toggle (Release-Off)", 5);
    playStateMenu->addItem("Toggle (Non-Destructive)", 25);
    playStateMenu->addItem("Sticky", 6);
    playStateMenu->addItem("Latch (Max-Latch)", 7);
    playStateMenu->addItem("Latch (Press-Latch)", 8);
    playStateMenu->addItem("Trigger (Looped)", 9);
    playStateMenu->addItem("Trigger (Looped, Non-Destructive)", 22);
    playStateMenu->addItem("Trigger (Non-Looped)", 10);
    playStateMenu->addItem("Trigger (Non-Looped, Non-Destructive)", 24);
    playStateMenu->addItem("Cycle (Looped)", 11);
    playStateMenu->addItem("Cycle (Linear)", 12);
    playStateMenu->addItem("Cycle (Linear, Non Destructive)", 23);
    playStateMenu->addItem("Auto-Cycle (Looped)", 13);
    playStateMenu->addItem("Auto-Cycle (Linear)", 14);
    playStateMenu->setSelectedId(3, true);
    playStateMenu->addMouseListener(this, true);
    
    addAndMakeVisible(quantizeModeMenu = new ComboBox());
    quantizeModeMenu->addItem("Free", 1);
    quantizeModeMenu->addItem("Quantized", 2);
    quantizeModeMenu->setSelectedId(1, true);
    quantizeModeMenu->addMouseListener(this, true);
    quantizeModeMenu->addListener(this);
    
    //relative tempo menu
    addAndMakeVisible(relativeTempoMenu = new ComboBox());
    relativeTempoMenu->addListener(this);
    relativeTempoMenu->addMouseListener(this, true);
    relativeTempoMenu->addItem("Quarter Time", 1);
    relativeTempoMenu->addItem("Half Time", 2);
    relativeTempoMenu->addItem("Regular Time", 3);
    relativeTempoMenu->addItem("Double Time", 4);
    relativeTempoMenu->addItem("Quadrupal Time", 5);
    relativeTempoMenu->setSelectedId(3, true);
    
    //Velocity Label
    addAndMakeVisible(velocityLabel = new Label());
    velocityLabel->setColour(Label::textColourId, Colours::silver);
    //velocityLabel->setColour(Label::backgroundColourId, Colours::lightgrey.withAlpha(0.3f));
    velocityLabel->addMouseListener(this, true);

    //set neccassary components hidden
    clearButton->setVisible(false);
    clearAllButton->setVisible(false);
    sequencerGrid->setVisible(false);
    currentSequenceNumberSlider->setVisible(false);
    saveSeqButton->setVisible(false);
    loadSeqButton->setVisible(false);
    saveSeqSetButton->setVisible(false);
    loadSeqSetButton->setVisible(false);
    velocityLabel->setVisible(false);
    
    
    
    //lastTime = Time::getMillisecondCounter();
    
    //attach this class to the subject class
    modeSequencerRef.attach(this);
    
}

GuiSequencerMode::~GuiSequencerMode()
{
    deleteAllChildren();
    
    //detach this class from the subject class
    modeSequencerRef.detach(this);
    
}


bool GuiSequencerMode::update(const Subject& theChangedSubject)
{
    if (&theChangedSubject == &modeSequencerRef)
    {
        if (modeSequencerRef.getWhatShouldUpdateFlag() == 2) ///if notified by an 'update seq number' call
        {
            //update the currentSequenceNumberSlider which in turns changes the sequence displayed in sequencerGrid
            currentSequenceNumberSlider->setValue(modeSequencerRef.getCurrentSequenceNumber()+1, true);
        }
    }
    
    
    return true;
}

void GuiSequencerMode::resized()
{
    popUpWindow->setBounds(150, 340, 725, 300);
    
    circleBackgroundLeft->setBounds(14, 402, 230, 230);
    circleBackgroundRight->setBounds(780, 402, 230, 230);
    
    midiMode->setBounds(0, 0, getWidth(), getHeight());
    samplesMode->setBounds(0, 0, getWidth(), getHeight());
    
    modeMidiButton->setBounds(getWidth()-100, 120, 45, 45);
    modeSamplesButton->setBounds(getWidth()-50, 120, 45, 45);

    sequencerGridToggleButton->setBounds(720, 568, 60, 60);
    sequencerGrid->setBounds(275, 410, 400, 160);
    
    channelSlider->setBounds(850, 251, 142, 142);
    
    clearButton->setBounds(285, 385, 50, 20);
    clearAllButton->setBounds(335, 385, 50, 20);
    currentSequenceNumberSlider->setBounds(645, 390, COMPONENT_W, 15);
    saveSeqButton->setBounds(412, 575, 100, 20);
    loadSeqButton->setBounds(512, 575, 100, 20);
    saveSeqSetButton->setBounds(412, 600, 100, 20);
    loadSeqSetButton->setBounds(512, 600, 100, 20);
    velocityLabel->setBounds(270, 577, 40, 15);
    
    
    numberOfSequencesSlider->setBounds(LEFT_CIRCLE_X, 440, COMPONENT_W, COMPONENT_H);
    sequenceLengthSlider->setBounds(LEFT_CIRCLE_X, 470, COMPONENT_W, COMPONENT_H);
    playStateMenu->setBounds(LEFT_CIRCLE_X, 570, COMPONENT_W, COMPONENT_H);
    relativeTempoMenu->setBounds(LEFT_CIRCLE_X, 500, COMPONENT_W, COMPONENT_H);
    
    quantizeModeMenu->setBounds(800, 155, 100, 20);
    
}


void GuiSequencerMode::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    
    midiMode->setCurrentlySelectedPad(selectedPads);
    samplesMode->setCurrentlySelectedPad(selectedPads);
    
    //updateDisplay();    
}


void GuiSequencerMode::comboBoxChanged (ComboBox* comboBox)
{
    //==============================================================================
    //play state combobox
    if (comboBox == playStateMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerPlayState(playStateMenu->getSelectedId());
        }
        
    }
    
    
    
    //==============================================================================
    //trigger mode combobox
    if (comboBox == quantizeModeMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerQuantizeMode(quantizeModeMenu->getSelectedId());
        }
        
    }
    
    //==============================================================================
    //relative tempo mode combobox
    if (comboBox == relativeTempoMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerRelativeTempoMode(relativeTempoMenu->getSelectedId());
        }
        
    }
    
}

void GuiSequencerMode::sliderValueChanged (Slider* slider)
{
    if (slider == numberOfSequencesSlider)
    {
        //set the the range of the currentSequenceNumberSlider so that the max matches this sliders value
        currentSequenceNumberSlider->setRange(1, numberOfSequencesSlider->getValue(), 1);
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerNumberOfSequences(numberOfSequencesSlider->getValue());
        }
        
    }
    
    if (slider == channelSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerChannel(channelSlider->getValue());
        }
        
    }
    
    if (slider == sequenceLengthSlider)
    {
        //set the range of the noteLengthSlider so that the max value matches this sliders value
        midiMode->setNoteLengthSliderRange(sequenceLengthSlider->getValue());
        
        sequencerGrid->repaint(); //repaint with bounds!
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerLength(sequenceLengthSlider->getValue());
        }
        
    }
    
    
    if (slider == currentSequenceNumberSlider)
    {
        //if individual pad number is selected
        if(SINGLE_PAD)
        {
            //this function will update the display of the grid
            sequencerGrid->setCurrentSequenceNumber(currentSequenceNumberSlider->getValue());
        }
        else if(MULTI_PADS)
        {
            //what do i do here? clear grid but don't cleat data. 
            sequencerGrid->resetGridGui();
        }
        
    }

    
}

void GuiSequencerMode::buttonClicked (Button* button)
{
    //this function sets the selected mode (based on the button pressed)
    //to the currently selected pad (or set of pads)
    int buttonIndex;
    
    //==============================================================================
    //get the selected button and set the right mode to screen
    if (button == modeMidiButton)
    {
        setToMidiMode();
        buttonIndex = 1;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMode(buttonIndex);
        }
        

    }
    else if (button == modeSamplesButton)
    {
        setToSamplesMode();
        buttonIndex = 2;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMode(buttonIndex);
        }

    }
       
    
    
    if (button == sequencerGridToggleButton)
    {
        if (sequencerGridToggleButton->getToggleState() == true) //on
        {
            this->toFront(false);
            
            popUpWindow->setVisible(true);
            clearButton->setVisible(true);
            clearAllButton->setVisible(true);
            sequencerGrid->setVisible(true);
            currentSequenceNumberSlider->setVisible(true);
            midiMode->setComponentsVisible(true);
            samplesMode->setComponentsVisible(true);
            saveSeqButton->setVisible(true);
            loadSeqButton->setVisible(true);
            saveSeqSetButton->setVisible(true);
            loadSeqSetButton->setVisible(true);
            velocityLabel->setVisible(true);
            
        }
        else
        {
            this->toBack();
            
            popUpWindow->setVisible(false);
            clearButton->setVisible(false);
            clearAllButton->setVisible(false);
            sequencerGrid->setVisible(false);
            currentSequenceNumberSlider->setVisible(false);
            midiMode->setComponentsVisible(false);
            samplesMode->setComponentsVisible(false);
            saveSeqButton->setVisible(false);
            loadSeqButton->setVisible(false);
            saveSeqSetButton->setVisible(false);
            loadSeqSetButton->setVisible(false);
            velocityLabel->setVisible(false);
        }
    }
    
    
    if (button == clearButton)
    {
        bool userSelection;
        userSelection = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Are you sure?", "You cannot undo this command", "Ok", "Cancel");
        if (userSelection == true)
        {
            sequencerGrid->clear();
        }
    }
    
    if (button == clearAllButton)
    {
        bool userSelection;
        userSelection = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Are you sure?", "You cannot undo this command", "Ok", "Cancel");
        if (userSelection == true)
        {
            sequencerGrid->clearAll();
        }
    }
    
    
    //the below if(SINGLE_PAD) checks are just for safety... if multi pads are selected the save buttons should be invisible.
    if (button == saveSeqButton)
    {
        if (SINGLE_PAD)
            appDocumentStateRef.saveSequence(currentSequenceNumberSlider->getValue()-1, selectedPads[0]);
    }
    
    if (button == loadSeqButton)
    {
        
        appDocumentStateRef.loadSequence(currentSequenceNumberSlider->getValue()-1, selectedPads);
    }
    
    if (button == saveSeqSetButton)
    {
        if (SINGLE_PAD)
            appDocumentStateRef.saveSequenceSet(selectedPads[0]);
    }
    
    if (button == loadSeqSetButton)
    {
        appDocumentStateRef.loadSequenceSet(selectedPads);
    }
    
     
}


void GuiSequencerMode::updateDisplay()
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
        
        
        
        if (PAD_SETTINGS->getSequencerMode() == 1) //midi mode
        {
            setToMidiMode();
            modeMidiButton->setToggleState(true, false); 
        }
        
        if (PAD_SETTINGS->getSequencerMode() == 2) //samples mode
        {
            setToSamplesMode();
            modeSamplesButton->setToggleState(true, false);
        }
        
        
        
        sequencerGrid->setPlayHeadPos(0); //??

        
        int i;
        i = PAD_SETTINGS->getSequencerNumberOfSequences();
        numberOfSequencesSlider->setValue(i, false); 
        i = PAD_SETTINGS->getSequencerChannel();
        channelSlider->setValue(i, false);
        i = PAD_SETTINGS->getSequencerLength();
        sequenceLengthSlider->setValue(i, false);
        i = PAD_SETTINGS->getSequencerPlayState();
        playStateMenu->setSelectedId(i, true);
        //currentSequenceNumberSlider->setValue(1);
        quantizeModeMenu->setSelectedId(PAD_SETTINGS->getSequencerQuantizeMode(), true);
        relativeTempoMenu->setSelectedId(PAD_SETTINGS->getSequencerRelativeTempoMode(), true);
        
        //set the range of the noteLengthSlider so that the max value matches this sliders value
        midiMode->setNoteLengthSliderRange(sequenceLengthSlider->getValue());
        //set the the range of the currentSequenceNumberSlider so that the max matches this sliders value
        currentSequenceNumberSlider->setRange(1, numberOfSequencesSlider->getValue(), 1);

        saveSeqButton->setVisible(true);
        saveSeqSetButton->setVisible(true);
        
        
    }
    
    else if(MULTI_PADS)
    {
        
        //set to a default setting
        setToMidiMode();
        modeMidiButton->setToggleState(true, false);
        
    
        sequencerGrid->setPlayHeadPos(0); // ??
        
        
        //set default values
        numberOfSequencesSlider->setValue(8, false); 
        channelSlider->setValue(1, false);
        sequenceLengthSlider->setValue(250, false);
        playStateMenu->setSelectedId(3, true);
        //currentSequenceNumberSlider->setValue(1);
        quantizeModeMenu->setSelectedId(1, true);
        relativeTempoMenu->setSelectedId(3, true);
        
        //set the range of the noteLengthSlider so that the max value matches this sliders value
        midiMode->setNoteLengthSliderRange(sequenceLengthSlider->getValue());
        //set the the range of the currentSequenceNumberSlider so that the max matches this sliders value
        currentSequenceNumberSlider->setRange(1, numberOfSequencesSlider->getValue(), 1);

        
        saveSeqButton->setVisible(false);
        saveSeqSetButton->setVisible(false);
        
    }
    
    //update sequencerGrid GUI based on currently selected pad and currently selected sequence number
    sequencerGrid->setCurrentlySelectedPad(selectedPads); //why is this called here and not above?
    currentSequenceNumberSlider->setValue(1);
    sequencerGrid->setCurrentSequenceNumber(currentSequenceNumberSlider->getValue());
    
    
}



void GuiSequencerMode::setToMidiMode()
{
    midiMode->setVisible(true);
    samplesMode->setVisible(false);
    
    //update the display of the midi mode so it shows the
    //currently selected pad's settings, and makes any uneeded
    //components invisble or dissabled
    midiMode->updateDisplay();
    
}

void GuiSequencerMode::setToSamplesMode()
{
    midiMode->setVisible(false);
    samplesMode->setVisible(true);
    
    //update the display of the midi mode so it shows the
    //currently selected pad's settings, and makes any uneeded
    //components invisble or dissabled
    samplesMode->updateDisplay();
}

void GuiSequencerMode::setVelocityLabelText (String velocity)
{
    if (velocity != velocityLabel->getText())
        velocityLabel->setText(velocity, false);
}



void GuiSequencerMode::mouseEnter (const MouseEvent &e)
{
    if (channelSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Sequencer Group. Sets and displays the sequencer group for the selected pad/pads. If the pad is set to a group other than group 1 the sequence will be in 'Exclusive Mode' - playing this sequence will turn off the previously playing sequence of the same group. The default group 1 allows does not enable this exclusive mode.");
    }
    else if (modeMidiButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Sequencer MIDI-Mode Button. Click this button to set the selected pad/pads to the MIDI-sequencer mode.\nThis mode allows dynamic sequences of MIDI note-on and note-off messages to be sent out of the application.");
    }
    else if (modeSamplesButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Sequencer Samples-Mode Button. Click this button to set the selected pad/pads to the samples-sequencer mode.\nThis mode allows dynamic sequences of audio samples to be created and played.");
    }
    else if (sequencerGridToggleButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("View Sequencer Grid Button. Toggles a pop-up window that displays the sequencer grid which is used to view and edit the sequences.");
    }
    else if (clearButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Clear Sequence Button. Click this button to clear and delete the currently displayed sequence.");
    }
    else if (clearAllButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Clear-All Sequence Button. Click this button to clear and delete all the sequences on the selected pad/pads.");
    }
    else if (currentSequenceNumberSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Current Sequence Number Selector. Sets and displays the sequence displayed on the grid for the selected pad/pads.");
    }
    else if (saveSeqButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Save Sequence Button. Saves the currently displayed sequence to an external file.");
    }
    else if (loadSeqButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Load Sequence Button. Loads a sequence from an external file into the selected sequence number which is then displayed on the grid.");
    }
    else if (saveSeqSetButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Save Sequence Set Button. Saves the selected pad/pads set of sequences to an external file.");
    }
    else if (loadSeqSetButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Load Sequence Set Button. Loads a set of sequences from an external file into the the selected pad/pads.");
    }
    else if (sequenceLengthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Sequence Length Selector. Sets and displays the length of the sequence (in sequence steps) for the selected pad/pads. This can be used to crudely set the sequence time signature.");
    }
    else if (numberOfSequencesSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Number of Sequences Selector. Sets and displays the number of sequences in the sequencer set for the selected pad/pads.");
    }
    else if (playStateMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("PlayState Drop-down Menu. Sets and displays the sequencer PlayState for the selected pad/pads. Sequencer PlayStates determine how interation with a pad controls the playback and modification of a sequence.");
    }
    else if (sequencerGrid->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Step-Sequencer Grid. This is where the actual sequence is editted and displayed. Like a traditional step-sequencer interface, click on a grid point (where the grid lines intersect) to create a 'note', or click on a note to delete it. Use alt-click-drag on a note to change the note's velocity/gain.");
    }
    else if (quantizeModeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Trigger Mode Menu. The trigger mode determines the start time/position of the sequence. 'Free' will trigger the sequence as soon as the pad is pressed, 'Quantized' will trigger the sequence at the next quantized time value set using the global clock above.");
    }
    else if (relativeTempoMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Relative Tempo Menu. Sets the relative tempo of the individual sequencer.");
    }
    
    
    
}

void GuiSequencerMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
}