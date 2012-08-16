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
    sequenceLength = 32;
    currentSequenceNumber = 1;
    controlDisplayId = 0;
    
    //grid
    addAndMakeVisible(sequencerGrid = new SequencerGrid(modeSequencerRef, *this));
    sequencerGrid->addMouseListener(this, true);
     
    //----------------number of sequences slider-------------------
	addAndMakeVisible(numberOfSequencesSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	numberOfSequencesSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	numberOfSequencesSlider->setRange(0, NO_OF_SEQS, 1);
    numberOfSequencesSlider->addListener(this);
    numberOfSequencesSlider->setValue(1, false);
    numberOfSequencesSlider->addMouseListener(this, true);
	
	//--------------- relative tempo slider-------------------
	addChildComponent(relativeTempoSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	relativeTempoSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	relativeTempoSlider->setRange(1, 5, 1);
    relativeTempoSlider->addListener(this);
    relativeTempoSlider->setValue(3, false);
    relativeTempoSlider->addMouseListener(this, true);
	
	//--------------- midi note length slider-------------------
	addChildComponent(noteLengthSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	noteLengthSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	noteLengthSlider->setRange(1, NO_OF_COLUMNS, 1);
    noteLengthSlider->addListener(this);
    noteLengthSlider->setValue(4, false);
    noteLengthSlider->addMouseListener(this, true);
	
	//--------------- audio gain slider-------------------
	addChildComponent(audioGainSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	audioGainSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	audioGainSlider->setRange(0.0, 2.0);
    audioGainSlider->addListener(this);
    audioGainSlider->setValue(0.01, false);
    audioGainSlider->addMouseListener(this, true);
	
	//--------------- audio pan slider-------------------
	addChildComponent(audioPanSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	audioPanSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	audioPanSlider->setRange(0.0, 1.0, 0.01);
    audioPanSlider->addListener(this);
    audioPanSlider->setValue(0.5, false);
    audioPanSlider->addMouseListener(this, true);

    
	addChildComponent(fxDial = new GuiFxDial(mainComponentRef));
	fxDial->setInterceptsMouseClicks(false, true);
    fxDial->addMouseListener(this, false);
	fxDial->setVisible(false);
    
    //----------------quantise button-------------------
	addAndMakeVisible(quantiseButton = new AlphaTextButton("Q"));
	quantiseButton->setClickingTogglesState(true);
	quantiseButton->setToggleState(false, false);	
	quantiseButton->addListener(this);
	quantiseButton->addMouseListener(this, true);
	quantiseButton->setOpaque(false);
	
    //----------------sequence settings button-------------------
	addAndMakeVisible(sequenceSettingsButton = new AlphaTextButton("SEQUENCE"));
	sequenceSettingsButton->setRadioGroupId (1234);
	sequenceSettingsButton->setClickingTogglesState(true);
	sequenceSettingsButton->setToggleState(false, false);	
	sequenceSettingsButton->addListener(this);
	sequenceSettingsButton->addMouseListener(this, true);
	sequenceSettingsButton->setOpaque(false);
    sequenceSettingsButton->setToggleState(true, false);
	
	//----------------trigger settings button-------------------
	addAndMakeVisible(triggerSettingsButton = new AlphaTextButton("TRIGGER"));
	triggerSettingsButton->setRadioGroupId (1234);
	triggerSettingsButton->setClickingTogglesState(true);
	triggerSettingsButton->setToggleState(false, false);	
	triggerSettingsButton->addListener(this);
	triggerSettingsButton->addMouseListener(this, true);
	triggerSettingsButton->setOpaque(false);
	
	//----------------pressure settings button-------------------
	addAndMakeVisible(pressureSettingsButton = new AlphaTextButton("PRESSURE"));
	pressureSettingsButton->setRadioGroupId (1234);
	pressureSettingsButton->setClickingTogglesState(true);
	pressureSettingsButton->setToggleState(false, false);	
	pressureSettingsButton->addListener(this);
	pressureSettingsButton->addMouseListener(this, true);
	pressureSettingsButton->setOpaque(false);
	
	//----------------sequence MIDI button-------------------
	addAndMakeVisible(modeMidiButton = new AlphaTextButton("MIDI"));
	modeMidiButton->setRadioGroupId (123);
	modeMidiButton->setClickingTogglesState(true);
	modeMidiButton->setToggleState(false, false);	
	modeMidiButton->addListener(this);
	modeMidiButton->addMouseListener(this, true);
	modeMidiButton->setOpaque(false);
    modeMidiButton->setToggleState(true, false);
	
	//----------------sequence Samples button-------------------
	addAndMakeVisible(modeSamplesButton = new AlphaTextButton("AUDIO"));
	modeSamplesButton->setRadioGroupId (123);
	modeSamplesButton->setClickingTogglesState(true);
	modeSamplesButton->setToggleState(false, false);	
	modeSamplesButton->addListener(this);
	modeSamplesButton->addMouseListener(this, true);
	modeSamplesButton->setOpaque(false);
	
	//---------------channel buttons---------------------
	for (int i = 0; i <= 15; i++)
	{
		midiChannelButtons.insert(i, new AlphaTextButton());
		midiChannelButtons[i]->setButtonText(String(i + 1));
		midiChannelButtons[i]->setClickingTogglesState(true);
		midiChannelButtons[i]->setToggleState(false, false);	
		midiChannelButtons[i]->setRadioGroupId (12);
		midiChannelButtons[i]->addListener(this);
        midiChannelButtons[i]->addMouseListener(this, true);
		midiChannelButtons[i]->setOpaque(false);
        addAndMakeVisible(midiChannelButtons[i]);
	}
    
    midiChannelButtons[0]->setToggleState(true, false);
	
	//---------------audio row buttons---------------------
	for (int i = 0; i <= 11; i++)
	{
		audioRowButtons.insert(i, new AlphaTextButton());
		audioRowButtons[i]->setButtonText(String(i + 1)) ;
		audioRowButtons[i]->setClickingTogglesState(false);
		audioRowButtons[i]->addListener(this);
        audioRowButtons[i]->addMouseListener(this, true);
        addChildComponent(audioRowButtons[i]);
	}
	
	
	//---------------CC controller Slider-------------------------------------
    addChildComponent(ccControllerSlider = new AlphaSlider());
    ccControllerSlider->setRange(0, 127, 1);
    ccControllerSlider->addListener(this);
    ccControllerSlider->addMouseListener(this, true);
    
    //------------------Pressure minimum range slider------------------------------
    addChildComponent(midiPressureMinRangeSlider = new AlphaSlider());
    midiPressureMinRangeSlider->setRange(0, 127, 1);
    midiPressureMinRangeSlider->addListener(this);
    midiPressureMinRangeSlider->setComponentValue(0);
    midiPressureMinRangeSlider->addMouseListener(this, true);
    
    
    //------------------Pressure maximum range slider------------------------------
    addChildComponent(midiPressureMaxRangeSlider = new AlphaSlider());
    midiPressureMaxRangeSlider->setRange(0, 127, 1);
    midiPressureMaxRangeSlider->addListener(this);
    midiPressureMaxRangeSlider->setComponentValue(127);
    midiPressureMaxRangeSlider->addMouseListener(this, true);
    
    
    addAndMakeVisible(plusButton = new SettingsButton("+", (135 * (M_PI / 180)), 
                                                   (160 * (M_PI / 180)),
                                                   0.7f, -5, 0.2, 0.27));
	plusButton->setClickingTogglesState(false);
	plusButton->addListener(this);
	plusButton->addMouseListener(this, false);
	
	addAndMakeVisible(minusButton = new SettingsButton("-", (200 * (M_PI / 180)), 
                                                      (225 * (M_PI / 180)),
                                                      0.7f, -5, 0.3, 0.27));
	minusButton->setClickingTogglesState(false);
	minusButton->addListener(this);
	minusButton->addMouseListener(this, false);
    
    addAndMakeVisible(parameterLabel = new Label());
	parameterLabel->setFont(Font(9));
	parameterLabel->setText("32", false);
    parameterLabel->setColour(Label::textColourId, Colours::white);
    parameterLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
    parameterLabel->setJustificationType(Justification::centred);
    parameterLabel->setEditable(false, true, true);
    parameterLabel->addListener(this);
	
	addChildComponent(currentParameterLabel = new Label());
	currentParameterLabel->setFont(Font(10));
	currentParameterLabel->setText("TEMPO", false);
    currentParameterLabel->setColour(Label::textColourId, Colours::white);
    currentParameterLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
    currentParameterLabel->setJustificationType(Justification::centred);
    currentParameterLabel->setEditable(false, false, false);
    currentParameterLabel->addListener(this);
    
    //----------------------Trigger mode buttons------------------
    for (int i = 0; i < 6; i++)
    {
        if (i == 0)
            triggerModeButtons.insert(i, new SettingsButton("STANDARD", (270 * (M_PI / 180)), 
                                                            (315 * (M_PI / 180)),
                                                            0.4f, -90, 0.45, 0.75));
        else if (i == 1)
            triggerModeButtons.insert(i, new SettingsButton("TOGGLE", (315 * (M_PI / 180)),
                                                            (2 * M_PI) , 0.4f, -90, 0.45, 0.68));
        else if (i == 2)
            triggerModeButtons.insert(i, new SettingsButton("LATCH", 0.0f, (45 * (M_PI / 180)), 
                                                            0.4f, 90, 0.55, 0.3));
        else if (i == 3)
            triggerModeButtons.insert(i, new SettingsButton("TRIGGER", (45 * (M_PI / 180)), 
                                                            (90 * (M_PI / 180)), 0.4f, 90, 0.55, 0.25));
        else if (i == 4)
            triggerModeButtons.insert(i, new SettingsButton("CYCLE", (225 * (M_PI / 180)), 
                                                            (270 * (M_PI / 180)),0.4f, -90, 0.45, 0.65));
        else if (i == 5)
            triggerModeButtons.insert(i, new SettingsButton("AUTO", (180 * (M_PI / 180)),
                                                            (225 * (M_PI/ 180)) , 0.4f, -90, 0.45, 0.6));
        
        triggerModeButtons[i]->addListener(this);
        triggerModeButtons[i]->setRadioGroupId (43);
        triggerModeButtons[i]->addMouseListener(this, false);
        addChildComponent(triggerModeButtons[i]);
    }
    
    triggerModeButtons[1]->setToggleState(true, false);
    
    //--------------pressure mode buttons--------------------------
	for (int i = 0; i < 6; i++)
    {
        if (i == 0)
            midiPressureModeButtons.insert(i, new SettingsButton("CAT", (270 * (M_PI / 180)), 
                                                             (315 * (M_PI / 180)), 0.4f, -90, 0.45, 0.7));
        if (i == 1)
            midiPressureModeButtons.insert(i, new SettingsButton("M.Wh", (315 * (M_PI / 180)),
                                                             (2 * M_PI) , 0.4f, -90, 0.45, 0.7));
        if (i == 2)
            midiPressureModeButtons.insert(i, new SettingsButton("CC", 0.0f, (45 * (M_PI / 180)), 
                                                             0.4f, 90, 0.55, 0.5));
        if (i == 3)
            midiPressureModeButtons.insert(i, new SettingsButton("PB UP", (45 * (M_PI / 180)), (90 * (M_PI / 180)), 
                                                             0.4f, 90, 0.55, 0.5));
        if (i == 4)
            midiPressureModeButtons.insert(i, new SettingsButton("PB DOWN", (225 * (M_PI / 180)), 
                                                             (270 * (M_PI / 180)),0.4f, -90, 0.45, 0.7));
        if (i == 5)
            midiPressureModeButtons.insert(i, new SettingsButton("REMOVE ME!", (180 * (M_PI / 180)),
                                                             (225 * (M_PI/ 180)) , 0.4f, -90, 0.45, 0.7));
        
        midiPressureModeButtons[i]->addListener(this);
        midiPressureModeButtons[i]->setRadioGroupId (53);
        midiPressureModeButtons[i]->addMouseListener(this, false);
        addChildComponent(midiPressureModeButtons[i]);
    }
    
    midiPressureModeButtons[0]->setToggleState(true, false);
    
    addChildComponent(loopButton = new AlphaTextButton("LOOP"));
    loopButton->addListener(this);
    loopButton->addMouseListener(this, true);
    loopButton->setClickingTogglesState(true);
    loopButton->setToggleState(1, false);
    
    addChildComponent(indestructibleButton = new AlphaTextButton("INDESTRUCT"));
    indestructibleButton->addListener(this);
    indestructibleButton->addMouseListener(this, true);
    indestructibleButton->setClickingTogglesState(true);
    
    addChildComponent(finishLoopButton = new AlphaTextButton("FINISH"));
    finishLoopButton->addListener(this);
    finishLoopButton->addMouseListener(this, true);
    finishLoopButton->setClickingTogglesState(true);
    
    addChildComponent(stickyButton = new AlphaTextButton("STICKY"));
    stickyButton->addListener(this);
    stickyButton->addMouseListener(this, true);
    stickyButton->setClickingTogglesState(true);
    
    
    //---------------status off bg-------------------------------------
    addChildComponent(notSelected = new GuiCircleBackground());
	notSelected->setVisible(false);
	
	//---------------pressure status button-------------------------------------
    
    addChildComponent(pressureStatusButton = new GuiSwitch());
    pressureStatusButton->addListener(this);
    pressureStatusButton->setClickingTogglesState(true);
    pressureStatusButton->setToggleState(true, false);
    pressureStatusButton->addMouseListener(this, false);
    
    
    addChildComponent(linkButton = new AlphaTextButton("LINK"));
    linkButton->addListener(this);
    linkButton->addMouseListener(this, true);
    linkButton->setClickingTogglesState(true);
    
    
    //--------------popup and sequence number button-------------------
	addAndMakeVisible(popUpButton = new AlphaPopUpButton());
	popUpButton->setButtonText("1");
    popUpButton->addListener(this);
    popUpButton->addMouseListener(this, true);
	
	//--------------preview button-------------------
	DrawablePath normal, down;
	
	Path p;
	p.addTriangle(0, 0, 0, 4, 4.5, 2);
	normal.setPath (p);
	normal.setFill (Colours::white);
	
	p.clear();
	p.addQuadrilateral(0, 0, 0, 4, 4, 4, 4, 0);
	down.setPath (p);
	down.setFill (Colours::white);
	p.clear();
	
	
	previewButton = new DrawableButton ("preview", DrawableButton::ImageOnButtonBackground);
	previewButton->setImages (&normal, &normal, &normal, 0, &down, &down, &down, 0);
	previewButton->setClickingTogglesState (true);
	previewButton->addListener (this);
	previewButton->setBackgroundColours (Colours::black, AlphaColours::blue);
	addAndMakeVisible (previewButton);
	
	addAndMakeVisible(nextSequenceButton = new AlphaTextButton());
	nextSequenceButton->setButtonText("+");
	nextSequenceButton->addListener(this);
	nextSequenceButton->addMouseListener(this, true);
	
	addAndMakeVisible(previousSequenceButton = new AlphaTextButton());
	previousSequenceButton->setButtonText("-");
	previousSequenceButton->addListener(this);
	previousSequenceButton->addMouseListener(this, true);
    
    //attach this class to the subject class
    modeSequencerRef.attach(this);
    
}

GuiSequencerMode::~GuiSequencerMode()
{
    triggerModeButtons.clear();
    midiPressureModeButtons.clear();
    midiChannelButtons.clear();
	audioRowButtons.clear();
    
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
            //update the currentSequenceNumber
            currentSequenceNumber = modeSequencerRef.getCurrentSequenceNumber()+1;
            setCurrentSequenceNumber();
            
        }
    }
    
    
    return true;
}

void GuiSequencerMode::resized()
{
   
    sequencerGrid->setBounds(689, 266, 312, 312);
	previewButton->setBounds(667 , 371, 17, 17);
	nextSequenceButton->setBounds(677, 342, 17, 17);
	previousSequenceButton->setBounds(662, 401, 17, 17);
    
	plusButton->setBounds(802, 379, 86, 86);
    minusButton->setBounds(802, 379, 86, 86);
	
	parameterLabel->setBounds(832,453,26,10);
	currentParameterLabel->setBounds(821, 398, 48, 48);
	
	popUpButton->setBounds(816, 393, 58, 58);
    
    numberOfSequencesSlider->setBounds(804, 381, 82, 82);
	relativeTempoSlider->setBounds(804, 381, 82, 82);
	noteLengthSlider->setBounds(804, 381, 82, 82);
	audioGainSlider->setBounds(804, 381, 82, 82);
	audioPanSlider->setBounds(804, 381, 82, 82);
	
	notSelected->setBounds(0, 0, getWidth(), getHeight());
	
	pressureStatusButton->setBounds(816, 393, 58, 58);
	
	midiChannelButtons[0]->setBounds(649,439,21, 21);
	midiChannelButtons[1]->setBounds(656,467,21, 21);
	midiChannelButtons[2]->setBounds(667,495,21, 21);
	midiChannelButtons[3]->setBounds(682,520,21, 21);
	midiChannelButtons[4]->setBounds(700,542,21, 21);
	midiChannelButtons[5]->setBounds(722,562,21, 21);
	midiChannelButtons[6]->setBounds(747,577,21, 21);
	midiChannelButtons[7]->setBounds(774,589,21, 21);
	midiChannelButtons[8]->setBounds(803,596,21, 21);
	midiChannelButtons[9]->setBounds(832,599,21, 21);
	midiChannelButtons[10]->setBounds(861,597,21, 21);
	midiChannelButtons[11]->setBounds(890,590,21, 21);
	midiChannelButtons[12]->setBounds(917,579,21, 21);
	midiChannelButtons[13]->setBounds(942,564,21, 21);
	midiChannelButtons[14]->setBounds(965,545,21, 21);
	midiChannelButtons[15]->setBounds(984,523,21, 21);
	
	audioRowButtons[0]->setBounds(649,439,21, 21);
	audioRowButtons[1]->setBounds(656,467,21, 21);
	audioRowButtons[2]->setBounds(667,495,21, 21);
	audioRowButtons[3]->setBounds(682,520,21, 21);
	audioRowButtons[4]->setBounds(700,542,21, 21);
	audioRowButtons[5]->setBounds(722,562,21, 21);
	audioRowButtons[6]->setBounds(747,577,21, 21);
	audioRowButtons[7]->setBounds(774,589,21, 21);
	audioRowButtons[8]->setBounds(803,596,21, 21);
	audioRowButtons[9]->setBounds(832,599,21, 21);
	audioRowButtons[10]->setBounds(861,597,21, 21);
	audioRowButtons[11]->setBounds(890,590,21, 21);
	
	quantiseButton->setBounds(681, 288,32, 32);
	modeMidiButton->setBounds(952, 263,32, 32);
	modeSamplesButton->setBounds(980, 293,32, 32);
	
	sequenceSettingsButton->setBounds(735, 241,42, 42);
	triggerSettingsButton->setBounds(789, 221,42, 42);
	pressureSettingsButton->setBounds(847, 219,42, 42);
	
	triggerModeButtons[0]->setBounds(728, 305, 234, 234);
	triggerModeButtons[1]->setBounds(728, 305, 234, 234);
	triggerModeButtons[2]->setBounds(728, 305, 234, 234);
	triggerModeButtons[3]->setBounds(728, 305, 234, 234);
	triggerModeButtons[4]->setBounds(728, 305, 234, 234);
	triggerModeButtons[5]->setBounds(728, 305, 234, 234);
	
	loopButton->setBounds(894, 472,32, 32);
	indestructibleButton->setBounds(853, 496,32, 32);
	stickyButton->setBounds(853, 496,32, 32);
	finishLoopButton->setBounds(918, 431,32, 32);
	
	linkButton->setBounds(803,268, 32, 32);
	
	midiPressureModeButtons[0]->setBounds(728, 305, 234, 234);
	midiPressureModeButtons[1]->setBounds(728, 305, 234, 234);
	midiPressureModeButtons[2]->setBounds(728, 305, 234, 234);
	midiPressureModeButtons[3]->setBounds(728, 305, 234, 234);
	midiPressureModeButtons[4]->setBounds(728, 305, 234, 234);
	midiPressureModeButtons[5]->setBounds(728, 305, 234, 234);
	
	ccControllerSlider->setBounds(890, 431, 58, 58);
    midiPressureMinRangeSlider->setBounds(800, 540, 42, 42);
    midiPressureMaxRangeSlider->setBounds(848, 540, 42, 42);
	
	fxDial->setBounds(683, 261, 324, 324);

    
}


void GuiSequencerMode::paint (Graphics& g)
{
	ColourGradient fillGradient(AlphaColours::nearlyblack,845 , 461, Colours::black, 845 , 383, false);
	g.setGradientFill(fillGradient);
	g.fillEllipse(802, 379, 86, 86);
	g.setColour(Colours::black);
	
	g.fillEllipse(786,218, 48, 48);
	g.fillEllipse(844,216, 48, 48);
	g.fillEllipse(732,238, 48, 48);
	g.fillEllipse(678,285, 38, 38);
	g.fillEllipse(949,260, 38, 38);
	g.fillEllipse(977,290, 38, 38);
	
	
	if(triggerSettingsButton->getToggleStateValue()==true)
	{
		g.fillEllipse(850,493, 38, 38);
		g.fillEllipse(891,469, 38, 38);
		g.fillEllipse(915,428, 38, 38);
	}
	
	if(pressureSettingsButton->getToggleStateValue()==true)
	{
		g.fillEllipse(850,493, 38, 38);
		g.fillEllipse(800,265, 38, 38);
	}
	
	g.fillEllipse(646,436, 27, 27);
	g.fillEllipse(653,464, 27, 27);
	g.fillEllipse(664,492, 27, 27);
	g.fillEllipse(679,517, 27, 27);
	g.fillEllipse(697,539, 27, 27);
	g.fillEllipse(719,559, 27, 27);
	g.fillEllipse(744,574, 27, 27);
	g.fillEllipse(771,586, 27, 27);
	
	g.fillEllipse(800,593, 27, 27);
	g.fillEllipse(829,596, 27, 27);
	g.fillEllipse(858,594, 27, 27);
	g.fillEllipse(887,587, 27, 27);
	
	if(modeMidiButton->getToggleStateValue()==true)
	{
        g.fillEllipse(914,576, 27, 27);
        g.fillEllipse(939,561, 27, 27);
        g.fillEllipse(962,542, 27, 27);
        g.fillEllipse(981,520, 27, 27);
	}
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	
	g.drawEllipse(678,285, 38, 38, 1.0);
	g.drawEllipse(949,260, 38, 38, 1.0);
	g.drawEllipse(977,290, 38, 38, 1.0);
	
	if(triggerSettingsButton->getToggleStateValue()==true)
	{
        g.drawEllipse(850,493, 38, 38, 1.0);
		g.drawEllipse(891,469, 38, 38, 1.0);
		g.drawEllipse(915,428, 38, 38, 1.0);
		
		g.setColour(Colours::black);
		Path pieSeg;
		pieSeg.addPieSegment(802, 379, 86, 86, (125 * (M_PI / 180)), (235 * (M_PI / 180)), 0.2f);
		g.fillPath(pieSeg);
		
		g.fillEllipse(816, 393, 58, 58);
		
		ColourGradient fillGradient(AlphaColours::blue, 816+(58*0.5), 393+(58*0.6), AlphaColours::lightblue, 816+(58*0.5), 393, false);
		g.setGradientFill(fillGradient);
        
		g.fillEllipse((816+(58*0.15)), (393+(58*0.15)), (58*0.7), (58*0.7));
		
		g.setColour(Colours::grey.withAlpha(0.3f));
		g.drawEllipse((816+(58*0.1)), (393+(58*0.1)), (58*0.8), (58*0.8), 1.0f);
	}
	
	if(pressureSettingsButton->getToggleStateValue()==true)
	{
		g.drawEllipse(850,493, 38, 38, 1.0);
		g.drawEllipse(800,265, 38, 38, 1.0);
	}
	
	g.drawEllipse(646,436, 27, 27, 1.0);
	g.drawEllipse(653,464, 27, 27, 1.0);
	g.drawEllipse(664,492, 27, 27, 1.0);
	g.drawEllipse(679,517, 27, 27, 1.0);
	g.drawEllipse(697,539, 27, 27, 1.0);
	g.drawEllipse(719,559, 27, 27, 1.0);
	g.drawEllipse(744,574, 27, 27, 1.0);
	g.drawEllipse(771,586, 27, 27, 1.0);
	
	g.drawEllipse(800,593, 27, 27, 1.0);
	g.drawEllipse(829,596, 27, 27, 1.0);
	g.drawEllipse(858,594, 27, 27, 1.0);
	g.drawEllipse(887,587, 27, 27, 1.0);
	
	if(modeMidiButton->getToggleStateValue()==true)
	{
        g.drawEllipse(914,576, 27, 27, 1.0);
        g.drawEllipse(939,561, 27, 27, 1.0);
        g.drawEllipse(962,542, 27, 27, 1.0);
        g.drawEllipse(981,520, 27, 27, 1.0);
		
	}
	
    if(sequenceSettingsButton->getToggleStateValue()==true)
	{
		
		g.setColour(Colours::black);
		g.fillEllipse(662, 366, 27, 27);
		g.fillEllipse(657, 396, 27, 27);
		g.fillEllipse(672, 337, 27, 27);
		
		g.setColour(AlphaColours::nearlyblack);
		g.fillEllipse(689, 266, 312, 312);
		
		g.setColour(Colours::black);
		g.fillEllipse(698, 275, 294, 294);
		
		g.setColour(AlphaColours::nearlyblack);
		g.fillEllipse(707, 284, 276, 276);
		
		g.setColour(Colours::black);
		g.fillEllipse(716, 293, 258, 258);
		
		g.setColour(AlphaColours::nearlyblack);
		g.fillEllipse(725, 302, 240, 240);
		
		g.setColour(Colours::black);
		g.fillEllipse(734, 311, 222, 222);
		
		g.setColour(AlphaColours::nearlyblack);
		g.fillEllipse(743, 320, 204, 204);
		
		g.setColour(Colours::black);
		g.fillEllipse(752, 329, 186, 186);
		
		g.setColour(AlphaColours::nearlyblack);
		g.fillEllipse(761, 338, 168, 168);
		
		g.setColour(Colours::black);
		g.fillEllipse(770, 347, 150, 150);
		
		g.setColour(AlphaColours::nearlyblack);
		g.fillEllipse(779, 356, 132, 132);
		
		g.setColour(Colours::black);
		g.fillEllipse(788, 365, 114, 114);
		
		g.setColour(AlphaColours::nearlyblack);
		g.drawEllipse(797, 374, 96, 96,1.0);
		
		ColourGradient fillGradient(AlphaColours::nearlyblack,845 , 461, Colours::black, 845 , 383, false);
		g.setGradientFill(fillGradient);
		
		g.fillEllipse(802, 379, 86, 86);
		
		g.setColour(Colours::black);
        Path pieSeg;
        pieSeg.addPieSegment(802, 379, 86, 86, (125 * (M_PI / 180)), (235 * (M_PI / 180)), 0.2f);
        g.fillPath(pieSeg);
		
	}
	
	
	
}



void GuiSequencerMode::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    fxDial->setCurrentlySelectedPad(selectedPads);
    
    //updateDisplay();    
}


void GuiSequencerMode::comboBoxChanged (ComboBox* comboBox)
{
    
}

void GuiSequencerMode::sliderDragStarted (Slider* slider)
{
		
}

void GuiSequencerMode::sliderValueChanged (Slider* slider)
{
    //=============central rotary sliders=====================
    if (slider == relativeTempoSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerRelativeTempoMode(slider->getValue());
        }
            
		setParameterLabelText(String(relativeTempoSlider->getValue()));
    }
	
	else if (slider == noteLengthSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMidiNoteLength(slider->getValue());
        }
        
		setParameterLabelText(String(noteLengthSlider->getValue()));
    }
    
	else if (slider == audioGainSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerGain(slider->getValue());
        }
        
		setParameterLabelText(String(audioGainSlider->getValue()));
    }
    
	else if (slider == audioPanSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerPan(slider->getValue());
        }
        
		setParameterLabelText(String(audioPanSlider->getValue()));
    }
    
    else if (slider == numberOfSequencesSlider)
    {
        if (slider->getValue() == 0)
            slider->setValue(1, false);
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerNumberOfSequences(numberOfSequencesSlider->getValue());
        }
        
        setParameterLabelText(String(slider->getValue()));
        
        if (slider->getValue() < currentSequenceNumber)
        {
            currentSequenceNumber = slider->getValue();
            setCurrentSequenceNumber();
        }
            
        //set the the range of the currentSequenceNumberSlider so that the max matches this sliders value
        //currentSequenceNumberSlider->setRange(1, numberOfSequencesSlider->getValue(), 1);
    }
    
    
    //CC Controller Number
    else if (slider == ccControllerSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMidiCcController(ccControllerSlider->getValue());
        }
    }
    
    //min pressure range slider
    else if (slider == midiPressureMinRangeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMidiMinPressureRange(midiPressureMinRangeSlider->getValue());
        }
        
    }
    
    
    //max pressure range slider
    else if (slider == midiPressureMaxRangeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMidiMaxPressureRange(midiPressureMaxRangeSlider->getValue());
        }
    }

       
}

void GuiSequencerMode::buttonClicked (Button* button)
{
    
    if (button == plusButton || button == minusButton)
	{
        if (button == plusButton)
            controlDisplayId++;
        else if (button == minusButton)
            controlDisplayId--;
        
        setRotaryControlDisplay();
        
        //use buttons to change sequence length
        if (sequenceSettingsButton->getToggleState() == true)
        {
            if (button == plusButton)
                sequenceLength++;
            else if (button == minusButton)
                sequenceLength--;
            
            if (sequenceLength > 32)
                sequenceLength = 32;
            else if (sequenceLength < 1)
                sequenceLength = 1;
            
            setParameterLabelText(String(sequenceLength));
            
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSequencerLength(sequenceLength);
            }
            
            //and set noteLength to the new sequence length
            setNoteLengthSliderRange(sequenceLength);
        
            
        }
        
        
	}
	
	
	else if (button == nextSequenceButton || button == previousSequenceButton)
	{
        if (button == nextSequenceButton)
            currentSequenceNumber++;
        else if (button == previousSequenceButton)
            currentSequenceNumber--;
        
        if (currentSequenceNumber > numberOfSequencesSlider->getValue())
            currentSequenceNumber = numberOfSequencesSlider->getValue();
        else if (currentSequenceNumber < 1)
            currentSequenceNumber = 1;
        
        setCurrentSequenceNumber();
	
	}
    
    else if (button == previewButton) 
    {
		
	}
    
    else if(button == quantiseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setQuantizeMode(button->getToggleState());
        }
    }
	
	else if(button == popUpButton)
	{
		PopupMenu menu;
        
        menu.addItem(1, translate("Load sequence..."));
		menu.addItem(2, translate("Load sequence set..."));
        if (SINGLE_PAD)
        {
            menu.addItem(3, translate("Save sequence..."));
            menu.addItem(4, translate("Save sequence set..."));
        }
		menu.addItem(5, translate("Clear..."));
		menu.addItem(6, translate("Clear all..."));
        
        const int result = menu.show();
        
        if (result == 0)
        {
            // user dismissed the menu without picking anything
        }
        else if (result == 1) // load preset
        {
			appDocumentStateRef.loadSequence(currentSequenceNumber-1, selectedPads);
        }
        else if (result == 2) // load sequence set
        {
			appDocumentStateRef.loadSequenceSet(selectedPads);
        }
        else if (result == 3) // save sequence
        {
			appDocumentStateRef.saveSequence(currentSequenceNumber-1, selectedPads[0]);
        }
		else if (result == 4) // save sequence set
		{
			appDocumentStateRef.saveSequenceSet(selectedPads[0]);
		}
		else if (result == 5)
		{
			bool userSelection;
			userSelection = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("Are you sure?"), translate("You cannot undo this command"));
			if (userSelection == true)
			{
				sequencerGrid->clear();
			}			
		}
		else if (result == 6)
		{
			bool userSelection;
			userSelection = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("Are you sure?"), translate("You cannot undo this command"));
			if (userSelection == true)
			{
				sequencerGrid->clearAll();
			}			
		}
		
	}
    
    
    
    //this function sets the selected mode (based on the button pressed)
    //to the currently selected pad (or set of pads)
    int buttonIndex;
    
    //==============================================================================
    //get the selected button and set the right mode to screen
    if (button == modeMidiButton)
    {
        buttonIndex = 1;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMode(buttonIndex);
        }
        
        mainComponentRef.getGuiPiano()->setActive(true);
        mainComponentRef.getGuiPiano()->updateDisplay();
        
        for (int i = 0; i <= 15; i++)
            midiChannelButtons[i]->setVisible(true);
        
        for (int i = 0; i <= 11; i++)
            audioRowButtons[i]->setVisible(false);
        
        
        if(pressureSettingsButton->getToggleStateValue() == true)
        {
            
            for (int i = 0; i < 6; i++)
                midiPressureModeButtons[i]->setVisible(true);
            
            stickyButton->setVisible(true);
            linkButton->setVisible(true);
            
            if(midiPressureModeButtons[2]->getToggleStateValue()==true)
                ccControllerSlider->setVisible(true);
            else
                ccControllerSlider->setVisible(false);
            
            midiPressureMinRangeSlider->setVisible(true);
            midiPressureMaxRangeSlider->setVisible(true);
            
             pressureStatusButton->setVisible(true);
            
            if(pressureStatusButton->getToggleStateValue()==true)
                notSelected->setVisible(false);
            else
                notSelected->setVisible(true);
            
            fxDial->setVisible(false);
        }
            
        
        setRotaryControlDisplay();
        
        repaint(600, 200, 424, 469); // <- set bounds here!

    }
    else if (button == modeSamplesButton)
    {
        buttonIndex = 2;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMode(buttonIndex);
        }
        
        mainComponentRef.getGuiPiano()->setActive(false);
        
        for (int i = 0; i <= 15; i++)
            midiChannelButtons[i]->setVisible(false);
        
        for (int i = 0; i <= 11; i++)
            audioRowButtons[i]->setVisible(true);
        
        
        if(pressureSettingsButton->getToggleStateValue()==true)
        {
            
            for (int i = 0; i < 6; i++)
                midiPressureModeButtons[i]->setVisible(false);
            
            stickyButton->setVisible(true);
            linkButton->setVisible(true);
            
            ccControllerSlider->setVisible(false);
            midiPressureMinRangeSlider->setVisible(false);
            midiPressureMaxRangeSlider->setVisible(false);
            
            pressureStatusButton->setVisible(true);
            
            if(pressureStatusButton->getToggleStateValue()==true)
                notSelected->setVisible(false);
            else
                notSelected->setVisible(true);
            
            
            fxDial->setVisible(true);
            
        }
        
        setRotaryControlDisplay();

        repaint(600, 200, 424, 469); // <- set bounds here!
    }
     
    
    //Am I doing the below correctly?
    //or should I set both midi and samples mode pressure status each time?
    //the only problem with this is if you turn on the pressure status in midi
    //mode it could in tern create an effect object which we don't need.
    //With the current method i need to think about what happens to effects
    //objects when switching between samples and midi mode on a single pad.
    else if (button == pressureStatusButton)
	{
        if (modeMidiButton->getToggleState() == true)
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSequencerMidiPressureStatus(pressureStatusButton->getToggleState());
            }
        }
        
        else if (modeSamplesButton->getToggleState() == true)
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                //int padNum = selectedPads[i];
                //set sequence samples effect to 0 / off in PadSettings. see how sampler mode handles this
            }
        }
        
		if(pressureStatusButton->getToggleStateValue()==true)
			notSelected->setVisible(false);
		else
			notSelected->setVisible(true);
		
	}
    
    else if (button == sequenceSettingsButton)
    {
        setDisplay(3);
        
	}
    
    else if (button == triggerSettingsButton)
    {
        setDisplay(1);	
	}
    
    else if (button == pressureSettingsButton)
    {
        setDisplay(2);		
	}

    
    
    else if (button == loopButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerShouldLoop(button->getToggleState());
        }
    }
    
    else if (button == indestructibleButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerIndestructible(button->getToggleState());
        }
    }
    
    else if (button == finishLoopButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerShouldFinishLoop(button->getToggleState());
        }
    }
    else if (button == stickyButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerSticky(button->getToggleState());
        }
    }
    
    
    else if (button == linkButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerDynamicMode(button->getToggleState());
        }
    }
     
    
    //midi channel buttons
    for (int chan = 0; chan < 16; chan++)
    {
        if (button == midiChannelButtons[chan])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSequencerMidiChannel(chan + 1);
            }
            
            break;
        }
        
    }
    
    //triggerMode buttons
    for (int trig = 0; trig < 6; trig++)
    {
        if (button == triggerModeButtons[trig])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSequencerTriggerMode(trig+1);
            }
            
            break;
        }
    }
    
    //pressureMode buttons
    for (int pres = 0; pres < 6; pres++)
    {
        if (button == midiPressureModeButtons[pres])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSequencerMidiPressureMode(pres+1);
            }
            
            if (pres == 2)
                ccControllerSlider->setVisible(true);
            else
                ccControllerSlider->setVisible(false);
            
            break;
        }
    }
    
    
    for (int row = 0; row <= 11; row++)
	{
		if (button == audioRowButtons[row]) 
        {
        
            FileChooser myChooser (translate("Please select an audio file to load..."),
                                   File::getSpecialLocation (File::userMusicDirectory),
                                   "*.wav;*.aif;*.aiff");
            
            if (myChooser.browseForFileToOpen() == true)
            {
                File selectedAudioFile (myChooser.getResult());
                
                for (int i = 0; i < selectedPads.size(); i++)
                {
                    int padNum = selectedPads[i];
                    PAD_SETTINGS->setSequencerSamplesAudioFilePath(selectedAudioFile, row);
                }
            }
			
		}
	}
    
}

void GuiSequencerMode::labelTextChanged (Label* labelThatHasChanged)
{
	
	
}

void GuiSequencerMode::hideComponents()
{
    //This function is used to hide most of the components
    //and put the GUI in a state where it is ready to have a
    //new display set
    
    //sequencer settings stuff
    sequencerGrid->setVisible(false);
    popUpButton->setVisible(false);
    previewButton->setVisible(false);
    nextSequenceButton->setVisible(false);
    previousSequenceButton->setVisible(false);
    numberOfSequencesSlider->setVisible(false); //is this actually visible or needed?
    
    //trigger settings stuff
    for (int i = 0; i < 6; i++)
        triggerModeButtons[i]->setVisible(false);
    loopButton->setVisible(false);
    indestructibleButton->setVisible(false);
    finishLoopButton->setVisible(false);

    //pressure settings stuff
    pressureStatusButton->setVisible(false);
    for (int i = 0; i < 6; i++)
        midiPressureModeButtons[i]->setVisible(false);
    stickyButton->setVisible(false);
    linkButton->setVisible(false);
    ccControllerSlider->setVisible(false);
    midiPressureMinRangeSlider->setVisible(false);
    midiPressureMaxRangeSlider->setVisible(false);
    notSelected->setVisible(false);
    fxDial->setVisible(false);
    
    //hybrid stuff
    relativeTempoSlider->setVisible(false);
    noteLengthSlider->setVisible(false);
    audioGainSlider->setVisible(false);
    audioPanSlider->setVisible(false);
    plusButton->setVisible(false);
    minusButton->setVisible(false);
    parameterLabel->setVisible(false);
    currentParameterLabel->setVisible(false);
    //setParameterLabelText(String::empty);
}



void GuiSequencerMode::setDisplay (int settingsType)
{
    //This function sets the GUI depending on what
    //setting view (settingsType) has been selected
    //taking the mode into consideration
    
    hideComponents();
    
    if (settingsType == 1) //trigger settings
    {
        
        for (int i = 0; i < 6; i++)
            triggerModeButtons[i]->setVisible(true);
        
        loopButton->setVisible(true);
        indestructibleButton->setVisible(true);
        finishLoopButton->setVisible(true);
        
        parameterLabel->setVisible(true);
        currentParameterLabel->setVisible(true);
        plusButton->setVisible(true);
        minusButton->setVisible(true);
        
    }
    
    else if (settingsType == 2) //pressure settings
    {
        pressureStatusButton->setVisible(true);
        stickyButton->setVisible(true);
        linkButton->setVisible(true);
        
        if(pressureStatusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);
        
        if(modeMidiButton->getToggleStateValue()==true)
        {
            for (int i = 0; i < 6; i++)
                midiPressureModeButtons[i]->setVisible(true);
            
            if(midiPressureModeButtons[2]->getToggleStateValue()==true)
                ccControllerSlider->setVisible(true);
            else
                ccControllerSlider->setVisible(false);
            
            midiPressureMinRangeSlider->setVisible(true);
            midiPressureMaxRangeSlider->setVisible(true);
        }
        
        else if(modeSamplesButton->getToggleStateValue()==true)
        {
            fxDial->setVisible(true);
        }
        
    }
    
    else if (settingsType == 3) //sequence settings
    {
        sequencerGrid->setVisible(true);
        plusButton->setVisible(true);
        minusButton->setVisible(true);
        parameterLabel->setVisible(true);
        popUpButton->setVisible(true);
        previewButton->setVisible(true);
        nextSequenceButton->setVisible(true);
        previousSequenceButton->setVisible(true);
    }
    
    setRotaryControlDisplay();
    
    repaint(600, 200, 424, 469); // <- set bounds here!
}




void GuiSequencerMode::setRotaryControlDisplay()
{
    //This function sets which rotary slider is displayed in the centre
    //based on the following parameters:
    // - controlDisplayId which is controlled by the + and - buttons
    // - which settings view is currently displayed (trigger, pressure, or sequence)
    // - which mode is currently selected (midi or audio)
    
    relativeTempoSlider->setVisible(false);
    noteLengthSlider->setVisible(false);
    audioGainSlider->setVisible(false);
    audioPanSlider->setVisible(false);
    numberOfSequencesSlider->setVisible(false);
    
    if(triggerSettingsButton->getToggleStateValue() == true)
    {
        if(modeMidiButton->getToggleStateValue() == true)
        {
            if (controlDisplayId > 1)
                controlDisplayId = 0;
            else if (controlDisplayId < 0)
                controlDisplayId = 1;
            
            
            if (controlDisplayId == 0)
            {
                relativeTempoSlider->setVisible(true);
            }
            
            else if (controlDisplayId == 1)
            {
                noteLengthSlider->setVisible(true);
            }
        }
        
        else if(modeSamplesButton->getToggleStateValue()==true)
        {
            if (controlDisplayId > 2)
                controlDisplayId = 0;
            else if (controlDisplayId < 0)
                controlDisplayId = 2;
            
            if (controlDisplayId == 0)
            {
                relativeTempoSlider->setVisible(true);
            }
            
            else if (controlDisplayId == 1)
            {
                audioGainSlider->setVisible(true);
            }
            
            else if (controlDisplayId == 2)
            {
                audioPanSlider->setVisible(true);
            }
        }
    }
    
    
    else if(sequenceSettingsButton->getToggleStateValue() == true)
    {
        numberOfSequencesSlider->setVisible(true);
    }
    
    
    setParameterLabelText (String::empty);
    
}




void GuiSequencerMode::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/dissabled any unneeded components. 
    
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getSequencerMode() == 1) //midi mode
        {
            modeMidiButton->setToggleState(true, false);
            pressureStatusButton->setToggleState(PAD_SETTINGS->getSequencerMidiPressureStatus(), false);
            
            mainComponentRef.getGuiPiano()->setActive(true);
            mainComponentRef.getGuiPiano()->updateDisplay();
        }
        else if (PAD_SETTINGS->getSequencerMode() == 2) //samples mode
        {
            modeSamplesButton->setToggleState(true, false);
            mainComponentRef.getGuiPiano()->setActive(false);
            
            //uncomment the below when effects and the corresonding 
            //PadSettings stuff for sequencer mode has been implemented
            /*
            if (PAD_SETTINGS->getSequencerSamplesEffect() == 0)
                pressureStatusButton->setToggleState(false, false);
            else
                pressureStatusButton->setToggleState(true, false);
             */
        }
        
        
        //calling setDisplay() at the bottom will in turn display the correct mode components
        
        quantiseButton->setToggleState(PAD_SETTINGS->getQuantizeMode(), false);
        sequenceLength = PAD_SETTINGS->getSequencerLength();
        numberOfSequencesSlider->setValue(PAD_SETTINGS->getSequencerNumberOfSequences(), false);
        relativeTempoSlider->setValue(PAD_SETTINGS->getSequencerRelativeTempoMode(), false);
        noteLengthSlider->setValue(PAD_SETTINGS->getSequencerMidiNoteLength(), false);
        setNoteLengthSliderRange(sequenceLength);
        audioGainSlider->setValue(PAD_SETTINGS->getSequencerGain(), false);
        audioPanSlider->setValue(PAD_SETTINGS->getSequencerPan(), false);
        triggerModeButtons[PAD_SETTINGS->getSequencerTriggerMode()-1]->setToggleState(true, false);
        midiPressureModeButtons[PAD_SETTINGS->getSequencerMidiPressureMode()-1]->setToggleState(true, false);
        midiChannelButtons[PAD_SETTINGS->getSequencerMidiChannel()-1]->setToggleState(true, false);
        ccControllerSlider->setComponentValue(PAD_SETTINGS->getSequencerMidiCcController());
        loopButton->setToggleState(PAD_SETTINGS->getSequencerShouldLoop(), false);
        indestructibleButton->setToggleState(PAD_SETTINGS->getSequencerIndestructible(), false);
        finishLoopButton->setToggleState(PAD_SETTINGS->getSequencerShouldFinishLoop(), false);
        stickyButton->setToggleState(PAD_SETTINGS->getSequencerSticky(), false);
        linkButton->setToggleState(PAD_SETTINGS->getSequencerDynamicMode(), false);
        midiPressureMinRangeSlider->setComponentValue(PAD_SETTINGS->getSequencerMidiMinPressureRange());
        midiPressureMaxRangeSlider->setComponentValue(PAD_SETTINGS->getSequencerMidiMaxPressureRange());
        
        
    }
    
    
    else if(MULTI_PADS)
    {
      
        modeMidiButton->setToggleState(true, false);
        mainComponentRef.getGuiPiano()->setActive(true);
        mainComponentRef.getGuiPiano()->updateDisplay();
        pressureStatusButton->setToggleState(true, false);
        
        quantiseButton->setToggleState(false, false);
        sequenceLength = 32;
        numberOfSequencesSlider->setValue(1, false);
        relativeTempoSlider->setValue(3, false);
        noteLengthSlider->setValue(4, false);
        setNoteLengthSliderRange(32);
        audioGainSlider->setValue(2.0, false);
        audioPanSlider->setValue(0.5, false);
        triggerModeButtons[1]->setToggleState(true, false);
        midiPressureModeButtons[0]->setToggleState(true, false);
        midiChannelButtons[0]->setToggleState(true, false);
        ccControllerSlider->setComponentValue(13);
        loopButton->setToggleState(true, false);
        indestructibleButton->setToggleState(false, false);
        finishLoopButton->setToggleState(false, false);
        stickyButton->setToggleState(false, false);
        linkButton->setToggleState(false, false);
        midiPressureMinRangeSlider->setComponentValue(0);
        midiPressureMaxRangeSlider->setComponentValue(127);

        
    }
    
    
    //update sequencerGrid GUI based on currently selected pad and currently selected sequence number
    sequencerGrid->setCurrentlySelectedPad(selectedPads); //why is this called here and not above?
                        //is it because setCurrentlySelectedPad is used as an updateDisplay() too?
    currentSequenceNumber = 1;
    sequencerGrid->setCurrentSequenceNumber(currentSequenceNumber);
    
    
    //uncomment the below line when effects have been properly added to sequencer mode
    //fxDial->updateDisplay();
    
    //sequencerGrid->setPlayHeadPos(0); //??
    
    if (sequenceSettingsButton->getToggleState() == true)
        setDisplay(3);
    else if (triggerSettingsButton->getToggleState() == true)
        setDisplay(1);
    else if (pressureSettingsButton->getToggleState() == true)
        setDisplay(2);
    
}   


void GuiSequencerMode::setParameterLabelText (String value)
{
	//This function is used to update the display of currentParameterLabel
    //which displays the name of the currently visible rotary control,
    //and parameterLabel which displays the current value of the visible control
    
    //The function is called everytime the value of one of the controls is changed
    //and passes in the controls value as a string and sets it to parameterLabel.
    
    //If you want to update the currentParameterLabel text call this function with
    //String::empty and the currently visible control is found and the name is
    //put into the label, as well as putting the value of it into the other label.
    
    if (value != String::empty)
    {
	parameterLabel->setColour(Label::textColourId, AlphaColours::lightblue);
	parameterLabel->setText(value, false);
    }
	
	else if (value == String::empty)
	{
		parameterLabel->setColour(Label::textColourId, Colours::white);
		
		if (numberOfSequencesSlider->isVisible())
		{
            //here want to display sequenceLength, NOT number of sequences.
            //Is this too convoluted?
            parameterLabel->setText(String(sequenceLength), false);
            //parameterLabel->setText(String(numberOfSequencesSlider->getValue()), false);
        }
		
		else if (relativeTempoSlider->isVisible())
		{
			currentParameterLabel->setText("TEMPO", false);
			parameterLabel->setText(String(relativeTempoSlider->getValue()), false);
		}
		
		else if (noteLengthSlider->isVisible())
		{
			currentParameterLabel->setText("N LENGTH", false);
			parameterLabel->setText(String(noteLengthSlider->getValue()), false);
		}
		
		else if (audioGainSlider->isVisible())
		{
			currentParameterLabel->setText("GAIN", false);
			parameterLabel->setText(String(audioGainSlider->getValue()), false);
		}
		
		else if (audioPanSlider->isVisible())
		{
			currentParameterLabel->setText("PAN", false);
			parameterLabel->setText(String(audioPanSlider->getValue()), false);
			
		}
	}
    
}


void GuiSequencerMode::setCurrentSequenceNumber()
{
    if (sequenceSettingsButton->getToggleState() == true)
    {
        popUpButton->setButtonText(String(currentSequenceNumber));
        sequencerGrid->setCurrentSequenceNumber(currentSequenceNumber);
        //do i need to do an alternative to the above line if multiple pads are selected?
    }
}

void GuiSequencerMode::setNoteLengthSliderRange (int maxValue)
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



void GuiSequencerMode::mouseEnter (const MouseEvent &e)
{
    /*
    if (modeMidiButton->isMouseOver(true))
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
    else if (triggerModeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("TriggerMode Drop-down Menu. Sets and displays the sequencer TriggerMode for the selected pad/pads. Sequencer TriggerModes determine how interation with a pad controls the playback and modification of a sequence.");
    }
    else if (sequencerGrid->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Step-Sequencer Grid. This is where the actual sequence is editted and displayed. Like a traditional step-sequencer interface, click on a grid point (where the grid lines intersect) to create a 'note', or click on a note to delete it. Use alt-click-drag on a note to change the note's velocity/gain.");
    }
    else if (relativeTempoMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Relative Tempo Menu. Sets the relative tempo of the individual sequencer.");
    }
    
    
    */
}

void GuiSequencerMode::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    
    if (e.eventComponent == numberOfSequencesSlider)
		setParameterLabelText(String::empty);

	if (e.eventComponent == relativeTempoSlider)
		setParameterLabelText(String::empty);

	if (e.eventComponent == noteLengthSlider)
		setParameterLabelText(String::empty);

	if (e.eventComponent == audioGainSlider)
		setParameterLabelText(String::empty);
    
	if (e.eventComponent == audioPanSlider)
		setParameterLabelText(String::empty);
    
}
