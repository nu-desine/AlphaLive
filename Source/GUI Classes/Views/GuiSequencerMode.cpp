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
#include "../Binary Data/BinaryDataNew.h"
#include "GlobalValues.h"
#include "../../Application/CommonInfoBoxText.h"
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
	relativeTempoSlider->setRange(-2, 2, 1);
    relativeTempoSlider->addListener(this);
    relativeTempoSlider->setValue(0, false);
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
	audioGainSlider->setRange(0.0, 2.0, 0.01);
    audioGainSlider->addListener(this);
    audioGainSlider->setValue(1.0, false);
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
	Image *quantiseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::quantiseicon_png, BinaryDataNew::quantiseicon_pngSize));
	addAndMakeVisible(quantiseButton = new ModeButton(quantiseIcon));
	quantiseButton->setClickingTogglesState(true);
	quantiseButton->setToggleState(false, false);	
	quantiseButton->addListener(this);
	quantiseButton->addMouseListener(this, true);
	quantiseButton->setOpaque(false);
	
    //----------------sequence settings button-------------------
	Image *sequenceIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::sequenceicon_png, BinaryDataNew::sequenceicon_pngSize));
	addAndMakeVisible(sequenceSettingsButton = new ModeButton(sequenceIcon));
	sequenceSettingsButton->setRadioGroupId (1234);
	sequenceSettingsButton->setClickingTogglesState(true);
	sequenceSettingsButton->setToggleState(false, false);	
	sequenceSettingsButton->addListener(this);
	sequenceSettingsButton->addMouseListener(this, true);
	sequenceSettingsButton->setOpaque(false);
    sequenceSettingsButton->setToggleState(true, false);
	
	//----------------trigger settings button-------------------
	Image *triggerSettingsImage = new Image(ImageCache::getFromMemory(BinaryDataNew::triggersettingsicon_png, BinaryDataNew::triggersettingsicon_pngSize));
	addAndMakeVisible(triggerSettingsButton = new ModeButton(triggerSettingsImage));
	triggerSettingsButton->setRadioGroupId (1234);
	triggerSettingsButton->setClickingTogglesState(true);
	triggerSettingsButton->setToggleState(false, false);	
	triggerSettingsButton->addListener(this);
	triggerSettingsButton->addMouseListener(this, true);
	triggerSettingsButton->setOpaque(false);
	
	//----------------pressure settings button-------------------
	Image *pressureSettingsImage = new Image(ImageCache::getFromMemory(BinaryDataNew::pressuresettingsicon_png, BinaryDataNew::pressuresettingsicon_pngSize));
	addAndMakeVisible(pressureSettingsButton = new ModeButton(pressureSettingsImage));
	pressureSettingsButton->setRadioGroupId (1234);
	pressureSettingsButton->setClickingTogglesState(true);
	pressureSettingsButton->setToggleState(false, false);	
	pressureSettingsButton->addListener(this);
	pressureSettingsButton->addMouseListener(this, true);
	pressureSettingsButton->setOpaque(false);
	
	//----------------sequence MIDI button-------------------
	Image *modeMidiIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::midisymbol_png, BinaryDataNew::midisymbol_pngSize));
	addAndMakeVisible(modeMidiButton = new ModeButton(modeMidiIcon));
	modeMidiButton->setRadioGroupId (123);
	modeMidiButton->setClickingTogglesState(true);
	modeMidiButton->setToggleState(false, false);	
	modeMidiButton->addListener(this);
	modeMidiButton->addMouseListener(this, true);
	modeMidiButton->setOpaque(false);
    modeMidiButton->setToggleState(true, false);
	
	//----------------sequence Samples button-------------------
	Image *modeAudioIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::loopsymbol_png, BinaryDataNew::loopsymbol_pngSize));
	addAndMakeVisible(modeSamplesButton = new ModeButton(modeAudioIcon));
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
		audioRowButtons.insert(i, new FileDropButton(String(i + 1), i));
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
    ccControllerSlider->setComponentValue(-999);
    
    //------------------Pressure minimum range slider------------------------------
    addChildComponent(midiPressureMinRangeSlider = new AlphaRotarySlider((90 * (M_PI / 180)), (315 * (M_PI / 180)), 290));
	midiPressureMinRangeSlider->setRotaryParameters((90 * (M_PI / 180)), (315 * (M_PI / 180)),true);
    midiPressureMinRangeSlider->setRange(0, 127, 1);
    midiPressureMinRangeSlider->addListener(this);
    midiPressureMinRangeSlider->setValue(0, false);
    midiPressureMinRangeSlider->addMouseListener(this, true);
    
    
    //------------------Pressure maximum range slider------------------------------
    addChildComponent(midiPressureMaxRangeSlider = new AlphaRotarySlider((90 * (M_PI / 180)), (315 * (M_PI / 180)), 270));
	midiPressureMaxRangeSlider->setRotaryParameters((90 * (M_PI / 180)), (315 * (M_PI / 180)),true);
    midiPressureMaxRangeSlider->setRange(0, 127, 1);
    midiPressureMaxRangeSlider->addListener(this);
    midiPressureMaxRangeSlider->setValue(127, false);
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
    parameterLabel->setJustificationType(Justification::centred);
    //parameterLabel->setEditable(false, true, true);
    parameterLabel->addListener(this);
	
	addChildComponent(currentParameterLabel = new Label());
	currentParameterLabel->setFont(Font(10));
	currentParameterLabel->setText(translate("TEMPO"), false);
    currentParameterLabel->setColour(Label::textColourId, Colours::white);
    currentParameterLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
    currentParameterLabel->setJustificationType(Justification::centred);
    currentParameterLabel->setEditable(false, false, false);
    currentParameterLabel->addListener(this);
    
    //---------------hover parameter label -------------------------------------
    addChildComponent(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaColours::blue);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
    
    //----------------------Trigger mode buttons------------------
	Image *standardIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::standardicon_png, BinaryDataNew::standardicon_pngSize));
	Image *toggleIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::toggleicon_png, BinaryDataNew::toggleicon_pngSize));
	Image *latchIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::latchicon_png, BinaryDataNew::latchicon_pngSize));
	Image *triggerIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::triggericon_png, BinaryDataNew::triggericon_pngSize));
	Image *cycleIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::cycleicon_png, BinaryDataNew::cycleicon_pngSize));
	Image *autoIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::autocycleicon_png, BinaryDataNew::autocycleicon_pngSize));
	
	
    for (int i = 0; i < 6; i++)
    {
        if (i == 0)
			triggerModeButtons.insert(i, new SettingsButtonImage(standardIcon, (270 * (M_PI / 180)), 
																 (315 * (M_PI / 180)),
																 0.4f, 0.6f));
        else if (i == 1)
            triggerModeButtons.insert(i, new SettingsButtonImage(toggleIcon, (315 * (M_PI / 180)),
																 (2 * M_PI) , 0.4f, 0.6f));
        else if (i == 2)
            triggerModeButtons.insert(i, new SettingsButtonImage(latchIcon, 0.0f, (45 * (M_PI / 180)), 
																 0.4f, 0.6f));
        else if (i == 3)
            triggerModeButtons.insert(i, new SettingsButtonImage(triggerIcon, (45 * (M_PI / 180)), 
																 (90 * (M_PI / 180)), 0.4f, 0.6f));
        else if (i == 4)
            triggerModeButtons.insert(i, new SettingsButtonImage(cycleIcon, (225 * (M_PI / 180)), 
															(270 * (M_PI / 180)), 0.4f, 0.6f));
        else if (i == 5)
            triggerModeButtons.insert(i, new SettingsButtonImage(autoIcon, (180 * (M_PI / 180)), 
															(225 * (M_PI / 180)), 0.4f, 0.6f));
        
        triggerModeButtons[i]->addListener(this);
        triggerModeButtons[i]->setRadioGroupId (43);
        triggerModeButtons[i]->addMouseListener(this, false);
        addChildComponent(triggerModeButtons[i]);
    }
    
    triggerModeButtons[1]->setToggleState(true, false);
    
    //--------------pressure mode buttons--------------------------
	
	//Image *patIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::polyphonicaftertouchicon_png, BinaryDataNew::polyphonicaftertouchicon_pngSize));
	Image *catIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::channelaftertouchicon_png, BinaryDataNew::channelaftertouchicon_pngSize));
	Image *mwheelIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::modwheelicon_png, BinaryDataNew::modwheelicon_pngSize));
	Image *ccIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::ccmessageicon_png, BinaryDataNew::ccmessageicon_pngSize));
	Image *pbUpIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::pitchbendupicon_png, BinaryDataNew::pitchbendupicon_pngSize));
	Image *pbDownIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::pitchbenddownicon_png, BinaryDataNew::pitchbenddownicon_pngSize));
	
	for (int i = 0; i < 5; i++)
    {
        if (i == 0)
            midiPressureModeButtons.insert(i, new SettingsButtonImage(catIcon, (315 * (M_PI / 180)),
                                                                      (2 * M_PI) , 0.4f, 0.6f));
        if (i == 1)
            midiPressureModeButtons.insert(i, new SettingsButtonImage(mwheelIcon, 0.0f, (45 * (M_PI / 180)), 
                                                                      0.4f, 0.6f));
        if (i == 2)
            midiPressureModeButtons.insert(i, new SettingsButtonImage(ccIcon, (45 * (M_PI / 180)), (90 * (M_PI / 180)), 
                                                                      0.4f, 0.6f));
        if (i == 3)
            midiPressureModeButtons.insert(i, new SettingsButtonImage(pbUpIcon, (270 * (M_PI / 180)), 
                                                                      (315 * (M_PI / 180)), 0.4f, 0.6f));
        if (i == 4)
            midiPressureModeButtons.insert(i, new SettingsButtonImage(pbDownIcon, (225 * (M_PI / 180)), 
                                                                      (270 * (M_PI / 180)),0.4f,0.6f));
           
        midiPressureModeButtons[i]->addListener(this);
        midiPressureModeButtons[i]->setRadioGroupId (53);
        midiPressureModeButtons[i]->addMouseListener(this, false);
        addChildComponent(midiPressureModeButtons[i]);
    }
    
    midiPressureModeButtons[0]->setToggleState(true, false);
        /*
    addAndMakeVisible(numberOfSequencesSlider = new AlphaSlider());
    numberOfSequencesSlider->setRange(1, NO_OF_SEQS, 1);
    numberOfSequencesSlider->addListener(this);
    numberOfSequencesSlider->setValue(NO_OF_SEQS, false);
    numberOfSequencesSlider->addMouseListener(this, true);
     */
    
	Image *loopIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::loopicon_png, BinaryDataNew::loopicon_pngSize));
    addChildComponent(loopButton =new ModeButton(loopIcon));
    loopButton->addListener(this);
    loopButton->addMouseListener(this, true);
    loopButton->setClickingTogglesState(true);
    loopButton->setToggleState(1, false);
    
    Image *destructIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::indestructableicon_png, BinaryDataNew::indestructableicon_pngSize));
	addChildComponent(indestructibleButton = new ModeButton(destructIcon));
    indestructibleButton->addListener(this);
    indestructibleButton->addMouseListener(this, true);
    indestructibleButton->setClickingTogglesState(true);
    
    Image *finishIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::finishicon_png, BinaryDataNew::finishicon_pngSize));
    addChildComponent(finishLoopButton =new ModeButton(finishIcon));
    finishLoopButton->addListener(this);
    finishLoopButton->addMouseListener(this, true);
    finishLoopButton->setClickingTogglesState(true);
    
    Image *stickyIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::stickyicon_png, BinaryDataNew::stickyicon_pngSize));
    addChildComponent(stickyButton = new ModeButton(stickyIcon));
    stickyButton->addListener(this);
    stickyButton->addMouseListener(this, true);
    stickyButton->setClickingTogglesState(true);
    
    
    //---------------status off bg-------------------------------------
    addChildComponent(notSelected = new GuiCircleBackground());
	notSelected->setVisible(false);
	notSelected->drawButtonBackground();
	
	//---------------pressure status button-------------------------------------
    
    addChildComponent(pressureStatusButton = new GuiSwitch());
    pressureStatusButton->addListener(this);
    pressureStatusButton->setClickingTogglesState(true);
    pressureStatusButton->setToggleState(true, false);
    pressureStatusButton->addMouseListener(this, false);
    
    Image *linkIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::linkicon_png, BinaryDataNew::linkicon_pngSize));
    addChildComponent(linkButton = new ModeButton(linkIcon));
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
    previewButton->addMouseListener(this, true);
	//previewButton->setBackgroundColours (Colours::black, AlphaColours::blue);
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
    parameterHoverLabel->setBounds(832,453,26,10);
	
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
	//midiPressureModeButtons[5]->setBounds(728, 305, 234, 234);
	
	ccControllerSlider->setBounds(890, 431, 58, 58);
	midiPressureMinRangeSlider->setBounds(700, 277, 290, 290);
    midiPressureMaxRangeSlider->setBounds(710, 287, 270, 270);
	
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
		
		Path linkButtonBg;
		
		linkButtonBg.addCentredArc(805, 223, 72, 72, 0, (123 * (M_PI / 180)), (214 * (M_PI / 180)), true);
		linkButtonBg.addCentredArc(845, 423, 162, 162, 0, (330 * (M_PI / 180)), (367 * (M_PI / 180)), false);
		
		
		g.setColour(AlphaColours::nearlyblack);
		g.fillPath(linkButtonBg, getTransform());
		
		g.setColour(Colours::black);
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
    
    for (int i = 0; i <= 11; i++)
		audioRowButtons[i]->setCurrentlySelectedPad(selectedPads);
    
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
        
        parameterHoverLabel->setText(String(slider->getValue()), false);
    }
    
    
    //max pressure range slider
    else if (slider == midiPressureMaxRangeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMidiMaxPressureRange(midiPressureMaxRangeSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue()), false);
    }

       
}

void GuiSequencerMode::buttonClicked (Button* button)
{
    //Alot of the code in the following if/else statements that determines whats being set and displayed
    //when a mode button is pressed is repeated elsewhere in the updateDisplay and setDisplay functions.
    //should i just calling updatedisplay instead? Or does this do more than needed and can I just call
    //set display instead?
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
            PAD_SETTINGS->setSequencerEffect(0); //set effect to off to remove the effect object
            
        }
        
        updateDisplay(); //or setDisplay()?
        modeSequencerRef.stopLastPreviewedSequence();
        
        /*
        int pressureStatus_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiPressureStatus();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiPressureStatus() != pressureStatus_)
            {
                pressureStatusButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                pressureStatusButton->setToggleState(pressureStatus_, false);
        }
        
        mainComponentRef.getGuiPiano()->setActive(true);
        mainComponentRef.getGuiPiano()->updateDisplay();
        mainComponentRef.getToolbox()->updateDisplay();
        
        
        for (int i = 0; i <= 15; i++)
            midiChannelButtons[i]->setVisible(true);
        
        for (int i = 0; i <= 11; i++)
            audioRowButtons[i]->setVisible(false);
        
        
        if(pressureSettingsButton->getToggleStateValue() == true)
        {
            
            for (int i = 0; i < 5; i++)
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
         */

    }
    else if (button == modeSamplesButton)
    {
        buttonIndex = 2;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMode(buttonIndex);
            
        }
        
        updateDisplay(); //or setDisplay()?
         modeSequencerRef.stopLastPreviewedSequence();
        
        /*
        //==================================================================================================
        //for pressure status button
        int effect_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerEffect();
        if (effect_ > 0)
            effect_ = 1;
        
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            int effect_2 = PAD_SETTINGS->getSequencerEffect();
            if (effect_2 > 0)
                effect_2 = 1;
            
            if (effect_2 != effect_)
            {
                pressureStatusButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                pressureStatusButton->setToggleState(effect_, false);
        }
        
        
        
        fxDial->updateDisplay();
        mainComponentRef.getGuiPiano()->setActive(false);
        mainComponentRef.getToolbox()->updateDisplay();
        
        for (int i = 0; i <= 15; i++)
            midiChannelButtons[i]->setVisible(false);
        
        for (int i = 0; i <= 11; i++)
            audioRowButtons[i]->setVisible(true);
        
        
        if(pressureSettingsButton->getToggleStateValue()==true)
        {
            
            for (int i = 0; i < 5; i++)
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
         */
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
                int padNum = selectedPads[i];
                if (button->getToggleState() == false)
                {
                    PAD_SETTINGS->setSequencerEffect(0);
                }
                else if (button->getToggleState() == true)
                {
                    PAD_SETTINGS->setSequencerEffect(1); //set default effect
                    fxDial->updateDisplay();
                }
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
        
        if (previewButton->getToggleState() == true)
            modeSequencerRef.setPreviewSequenceNumber(selectedPads[0], currentSequenceNumber-1);
        
        setCurrentSequenceNumber();
        
	}
    
    else if (button == previewButton) 
    {
        modeSequencerRef.previewSequence(selectedPads[0], button->getToggleState());
        modeSequencerRef.setPreviewSequenceNumber(selectedPads[0], currentSequenceNumber-1);
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
        
        if (SINGLE_PAD)
            menu.addItem(7, translate(translate("Copy sequence...")));
        menu.addItem(8, translate(translate("Paste sequence...")));
        menu.addSeparator();
        menu.addItem(1, translate(translate("Import sequence...")));
		menu.addItem(2, translate(translate("Import sequence set...")));
        if (SINGLE_PAD)
        {
            menu.addItem(3, translate(translate("Export sequence...")));
            menu.addItem(4, translate(translate("Export sequence set...")));
        }
            menu.addSeparator();
        
        if (modeMidiButton->getToggleState() == true)
        {
            PopupMenu importMidiSingle;
            importMidiSingle.addItem(11, translate("With note data..."));
            importMidiSingle.addItem(12, translate("Without note data..."));
            menu.addSubMenu(translate("Import sequence from MIDI file"), importMidiSingle);
            
            PopupMenu importMidiSet;
            importMidiSet.addItem(13, translate("With note data..."));
            importMidiSet.addItem(14, translate("Without note data..."));
            menu.addSubMenu(translate("Import sequence set from MIDI file"), importMidiSet);
        }
        else //don't want to provide the option to import note data
        {
            menu.addItem(12, translate(translate("Import sequence from MIDI file")));
            menu.addItem(14, translate(translate("Import sequence set from MIDI file")));
            //translate("Whole pattern...");
            //translate("Matching GM drum mapping only...");
        }
        
        if (SINGLE_PAD)
        {
            menu.addItem(9, translate(translate("Export sequence as MIDI file...")));
            menu.addItem(10, translate(translate("Export sequence set as MIDI file...")));
        }
        menu.addSeparator();
		menu.addItem(5, translate(translate("Clear...")));
		menu.addItem(6, translate(translate("Clear all...")));
        
        const int result = menu.show();
        
        if (result == 0)
        {
            // user dismissed the menu without picking anything
        }
        else if (result == 1) // import sequence
        {
			appDocumentStateRef.loadSequence(currentSequenceNumber-1, selectedPads);
        }
        else if (result == 2) // import sequence set
        {
			appDocumentStateRef.loadSequenceSet(selectedPads);
        }
        else if (result == 3) // export sequence
        {
			appDocumentStateRef.saveSequence(currentSequenceNumber-1, selectedPads[0]);
        }
		else if (result == 4) // export sequence set
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
        else if (result == 7)
        {
            int padNum = selectedPads[0];
            
            for (int row = 0; row <= NO_OF_ROWS-1; row++)
            {
                for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
                {
                    AppSettings::Instance()->copySequenceData(PAD_SETTINGS->getSequencerData
                                                              (currentSequenceNumber-1, row, column), 
                                                              row, 
                                                              column);
                }
            }
            
        }
        else if (result == 8)
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                
                for (int row = 0; row <= NO_OF_ROWS-1; row++)
                {
                    for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
                    {
                        PAD_SETTINGS->setSequencerData(currentSequenceNumber-1, 
                                                       row, 
                                                       column, 
                                                       AppSettings::Instance()->pasteSequenceData(row, column),
                                                       false);
                    }
                }
                
                PAD_SETTINGS->seqDataToString();
            }
            
            sequencerGrid->setCurrentlySelectedPad(selectedPads);
            setCurrentSequenceNumber();
            
        }
        else if (result == 9) // export sequence as midi file
        {
			appDocumentStateRef.createMidiFile(currentSequenceNumber-1, selectedPads[0], false);
        }
        else if (result == 10) // export sequence set as midi file
        {
			appDocumentStateRef.createMidiFile(currentSequenceNumber-1, selectedPads[0], true);
        }
        else if (result == 11) // import sequence and note data from midi file
        {
			appDocumentStateRef.importMidiFile (currentSequenceNumber-1, selectedPads, false, true);
            
            sequencerGrid->setCurrentlySelectedPad(selectedPads);
            setCurrentSequenceNumber();
            mainComponentRef.getGuiPiano()->updateDisplay();
        }
        else if (result == 12) // import sequence only from midi file
        {
			appDocumentStateRef.importMidiFile (currentSequenceNumber-1, selectedPads, false, false);
            
            sequencerGrid->setCurrentlySelectedPad(selectedPads);
            setCurrentSequenceNumber();
        }
        else if (result == 13) // import sequence set and note data from midi file
        {
			appDocumentStateRef.importMidiFile (currentSequenceNumber-1, selectedPads, true, true);
            
            //sequencerGrid->setCurrentlySelectedPad(selectedPads);
            //setCurrentSequenceNumber();
            //mainComponentRef.getGuiPiano()->updateDisplay();
            updateDisplay();
        }
        else if (result == 14) // import sequence set only from midi file
        {
			appDocumentStateRef.importMidiFile (currentSequenceNumber-1, selectedPads, true, false);
            
            //sequencerGrid->setCurrentlySelectedPad(selectedPads);
            //setCurrentSequenceNumber();
            updateDisplay();
        }
        
        
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
    for (int pres = 0; pres < 5; pres++)
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
    for (int i = 0; i < 5; i++)
        midiPressureModeButtons[i]->setVisible(false);
    stickyButton->setVisible(false);
    linkButton->setVisible(false);
    ccControllerSlider->setVisible(false);
    midiPressureMinRangeSlider->setVisible(false);
    midiPressureMaxRangeSlider->setVisible(false);
    notSelected->setVisible(false);
    fxDial->setVisible(false);
    parameterHoverLabel->setVisible(false);
    
    //hybrid stuff
    relativeTempoSlider->setVisible(false);
    noteLengthSlider->setVisible(false);
    audioGainSlider->setVisible(false);
    audioPanSlider->setVisible(false);
    plusButton->setVisible(false);
    minusButton->setVisible(false);
    parameterLabel->setVisible(false);
    currentParameterLabel->setVisible(false);
    
    for (int i = 0; i < 16; i++)
        midiChannelButtons[i]->setVisible(false);
    for (int i = 0; i < 12; i++)
        audioRowButtons[i]->setVisible(false);
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
        
        if (modeMidiButton->getToggleStateValue() == true || modeSamplesButton->getToggleStateValue() == true)
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
        else //when multiple pads are selected but with different modes
        {
            notSelected->setVisible(true);
        }
        
    }
    
    else if (settingsType == 2) //pressure settings
    {
        if (modeMidiButton->getToggleStateValue() == true || modeSamplesButton->getToggleStateValue() == true)
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
                for (int i = 0; i < 5; i++)
                    midiPressureModeButtons[i]->setVisible(true);
                
                if(midiPressureModeButtons[2]->getToggleStateValue()==true)
                    ccControllerSlider->setVisible(true);
                else
                    ccControllerSlider->setVisible(false);
                
                midiPressureMinRangeSlider->setVisible(true);
                midiPressureMaxRangeSlider->setVisible(true);
                parameterHoverLabel->setVisible(true);
            }
            
            else if(modeSamplesButton->getToggleStateValue()==true)
            {
                fxDial->setVisible(true);
            }
        }
        else //when multiple pads are selected but with different modes
        {
            notSelected->setVisible(true);
        }
        
    }
    
    else if (settingsType == 3) //sequence settings
    {
        sequencerGrid->setVisible(true);
        plusButton->setVisible(true);
        minusButton->setVisible(true);
        parameterLabel->setVisible(true);
        popUpButton->setVisible(true);
        nextSequenceButton->setVisible(true);
        previousSequenceButton->setVisible(true);
        
        if (SINGLE_PAD)
            previewButton->setVisible(true);
    }
    
    if (modeMidiButton->getToggleState())
    {
        for (int i = 0; i < 16; i++)
            midiChannelButtons[i]->setVisible(true);
    }
    else if (modeSamplesButton->getToggleState())
    {
        for (int i = 0; i < 12; i++)
            audioRowButtons[i]->setVisible(true);
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
            
            
            if (PAD_SETTINGS->getSequencerEffect() == 0)
                pressureStatusButton->setToggleState(false, false);
            else
                pressureStatusButton->setToggleState(true, false);
            //effect is found a set with fxDial
             
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
        midiPressureMinRangeSlider->setValue(PAD_SETTINGS->getSequencerMidiMinPressureRange(), false);
        midiPressureMaxRangeSlider->setValue(PAD_SETTINGS->getSequencerMidiMaxPressureRange(), false);
        
        
        
    }
    
    
    else if(MULTI_PADS)
    {
        
        //==================================================================================================
        int mode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMode() != mode_)
            {
                modeMidiButton->setToggleState(0, false);
                modeSamplesButton->setToggleState(0, false);
                mainComponentRef.getGuiPiano()->setActive(false);
                pressureStatusButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
            {
                if (mode_ == 1)
                {
                    modeMidiButton->setToggleState(true, false);
                    mainComponentRef.getGuiPiano()->setActive(true);
                    mainComponentRef.getGuiPiano()->updateDisplay();
                    
                    //==================================================================================================
                    int pressureStatus_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiPressureStatus();
                    for (int i = 1; i < selectedPads.size(); i++)
                    {
                        int padNum = selectedPads[i];
                        if (PAD_SETTINGS->getSequencerMidiPressureStatus() != pressureStatus_)
                        {
                            pressureStatusButton->setToggleState(0, false);
                            break;
                        }
                        if (i == selectedPads.size()-1)
                            pressureStatusButton->setToggleState(pressureStatus_, false);
                    }
                }
                else
                {
                    modeSamplesButton->setToggleState(true, false);
                    mainComponentRef.getGuiPiano()->setActive(false);
                    
                    //==================================================================================================
                    //for pressure status button
                    int effect_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerEffect();
                    
                    for (int i = 1; i < selectedPads.size(); i++)
                    {
                        int padNum = selectedPads[i];
                        int effect_2 = PAD_SETTINGS->getSequencerEffect();
                        
                        if (effect_2 != effect_)
                        {
                            pressureStatusButton->setToggleState(0, false);
                            break;
                        }
                        if (i == selectedPads.size()-1)
                        {
                            if (effect_ == 0)
                                pressureStatusButton->setToggleState(0, false);
                            else
                                pressureStatusButton->setToggleState(1, false);
                        }
                    }
                }
            }
        }
        
        //==================================================================================================
        int quantiseMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getQuantizeMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getQuantizeMode() != quantiseMode_)
            {
                quantiseButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                quantiseButton->setToggleState(quantiseMode_, false);
        }
        
        //==================================================================================================
        int seqLength_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerLength();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerLength() != seqLength_)
            {
                sequenceLength = 32;
                setNoteLengthSliderRange(sequenceLength);
                break;
            }
            if (i == selectedPads.size()-1)
            {
                sequenceLength = seqLength_;
                setNoteLengthSliderRange(sequenceLength);
            }
        }
        
        //==================================================================================================
        int noOfSeqs_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerNumberOfSequences();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerNumberOfSequences() != noOfSeqs_)
            {
                numberOfSequencesSlider->setValue(1, false);
                break;
            }
            if (i == selectedPads.size()-1)
                numberOfSequencesSlider->setValue(noOfSeqs_, false);
        }
        
        //==================================================================================================
        int relativeTempo_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerRelativeTempoMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerRelativeTempoMode() != relativeTempo_)
            {
                relativeTempoSlider->setValue(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                relativeTempoSlider->setValue(relativeTempo_, false);
        }
        
        //==================================================================================================
        int noteLength_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiNoteLength();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiNoteLength() != noteLength_)
            {
                noteLengthSlider->setValue(1, false);
                break;
            }
            if (i == selectedPads.size()-1)
                noteLengthSlider->setValue(noteLength_, false);
        }
        
        //==================================================================================================
        double gain_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerGain();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerGain() != gain_)
            {
                audioGainSlider->setValue(1.0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                audioGainSlider->setValue(gain_, false);
        }
        
        //==================================================================================================
        double pan_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerPan();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerPan() != pan_)
            {
                audioPanSlider->setValue(0.5, false);
                break;
            }
            if (i == selectedPads.size()-1)
                audioPanSlider->setValue(pan_, false);
        }
        
        //==================================================================================================
        int triggerMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerTriggerMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerTriggerMode() != triggerMode_)
            {
                for (int i = 0; i < 6; i++)
                    triggerModeButtons[i]->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                triggerModeButtons[triggerMode_-1]->setToggleState(true, false);
        }
        
        //==================================================================================================
        int pressureMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiPressureMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiPressureMode() != pressureMode_)
            {
                for (int i = 0; i < 5; i++)
                    midiPressureModeButtons[i]->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                midiPressureModeButtons[pressureMode_-1]->setToggleState(true, false);
        }
        
        //==================================================================================================
        int channel_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiChannel();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiChannel() != channel_)
            {
                for (int i = 0; i <16; i++)
                    midiChannelButtons[i]->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                midiChannelButtons[channel_-1]->setToggleState(true, false);
        }
        
        //==================================================================================================
        int ccNumber_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiCcController();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiCcController() != ccNumber_)
            {
                ccControllerSlider->setComponentValue(-999);
                break;
            }
            if (i == selectedPads.size()-1)
                ccControllerSlider->setComponentValue(ccNumber_);
        }
        
        //==================================================================================================
        int indestructible_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerIndestructible();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerIndestructible() != indestructible_)
            {
                indestructibleButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                indestructibleButton->setToggleState(indestructible_, false);
        }
        
        //==================================================================================================
        int sticky_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerSticky();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerSticky() != sticky_)
            {
                stickyButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                stickyButton->setToggleState(sticky_, false);
        }
        
        //==================================================================================================
        int loop_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerShouldLoop();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerShouldLoop() != loop_)
            {
                loopButton->setToggleState(1, false);
                break;
            }
            if (i == selectedPads.size()-1)
                loopButton->setToggleState(loop_, false);
        }
        
        //==================================================================================================
        int finishLoop_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerShouldFinishLoop();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerShouldFinishLoop() != finishLoop_)
            {
                finishLoopButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                finishLoopButton->setToggleState(finishLoop_, false);
        }
        
        //==================================================================================================
        int dynamicMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerDynamicMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerDynamicMode() != dynamicMode_)
            {
                linkButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                linkButton->setToggleState(dynamicMode_, false);
        }
        
        //==================================================================================================
        int pressureMinRange_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiMinPressureRange();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiMinPressureRange() != pressureMinRange_)
            {
                midiPressureMinRangeSlider->setValue(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                midiPressureMinRangeSlider->setValue(pressureMinRange_, false);
        }
        
        //==================================================================================================
        int pressureMaxRange_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiMaxPressureRange();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiMaxPressureRange() != pressureMaxRange_)
            {
                midiPressureMaxRangeSlider->setValue(127, false);
                break;
            }
            if (i == selectedPads.size()-1)
                midiPressureMaxRangeSlider->setValue(pressureMaxRange_, false);
        }
        
    }
    
    if (previewButton->getToggleState() == true)
    {
        previewButton->setToggleState(false, false);
    }
    
    
    //update sequencerGrid GUI based on currently selected pad and currently selected sequence number
    sequencerGrid->setCurrentlySelectedPad(selectedPads); //why is this called here and not above?
                        //is it because setCurrentlySelectedPad is used as an updateDisplay() too?
    
    if (currentSequenceNumber > numberOfSequencesSlider->getValue())
        currentSequenceNumber = numberOfSequencesSlider->getValue();
    
    setCurrentSequenceNumber();
    
    fxDial->updateDisplay();
    mainComponentRef.getToolbox()->updateDisplay();
    
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
		parameterLabel->setColour(Label::textColourId, LookAndFeel::getDefaultLookAndFeel().findColour(Label::textColourId));
		
		if (numberOfSequencesSlider->isVisible())
		{
            //here want to display sequenceLength, NOT number of sequences.
            //Is this too convoluted?
            parameterLabel->setText(String(sequenceLength), false);
            //parameterLabel->setText(String(numberOfSequencesSlider->getValue()), false);
        }
		
		else if (relativeTempoSlider->isVisible())
		{
			currentParameterLabel->setText(translate("TEMPO"), false);
			parameterLabel->setText(String(relativeTempoSlider->getValue()), false);
		}
		
		else if (noteLengthSlider->isVisible())
		{
			currentParameterLabel->setText(translate("N LENGTH"), false);
			parameterLabel->setText(String(noteLengthSlider->getValue()), false);
		}
		
		else if (audioGainSlider->isVisible())
		{
			currentParameterLabel->setText(translate("GAIN"), false);
			parameterLabel->setText(String(audioGainSlider->getValue()), false);
		}
		
		else if (audioPanSlider->isVisible())
		{
			currentParameterLabel->setText(translate("PAN"), false);
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

int GuiSequencerMode::getCurrentSequenceNumber()
{
    return currentSequenceNumber;
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
    
    // ======= param label text command =========
    // =======================================
    
    if (numberOfSequencesSlider->isMouseOver(true))
		setParameterLabelText(String(numberOfSequencesSlider->getValue()));
	else if (relativeTempoSlider->isMouseOver(true))
		setParameterLabelText(String(relativeTempoSlider->getValue()));
	else if (noteLengthSlider->isMouseOver(true))
		setParameterLabelText(String(noteLengthSlider->getValue()));
	else if (audioGainSlider->isMouseOver(true))
		setParameterLabelText(String(audioGainSlider->getValue()));
	else if (audioPanSlider->isMouseOver(true))
		setParameterLabelText(String(audioPanSlider->getValue()));
    
    
    //update parameterHoverLabel
    if (midiPressureMinRangeSlider->isMouseOver(true))
        parameterHoverLabel->setText(String(midiPressureMinRangeSlider->getValue()), false);
    else if (midiPressureMaxRangeSlider->isMouseOver(true))
        parameterHoverLabel->setText(String(midiPressureMaxRangeSlider->getValue()), false);
     
    
    // ======= info box text command =========
    // =======================================
    
    if (quantiseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::quantizeButton));
    }
    else if (sequenceSettingsButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sequence Settings. Click here to display the circular step-sequencer grid to draw and edit the sequence arrangement for the selected pads, as well as access other sequence settings controls."));
    }
    else if (triggerSettingsButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerSettingsButton));
    }
    else if (pressureSettingsButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureSettingsButton));
    }
    
    if (modeMidiButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sequencer MIDI Mode Button. Click here to set the selected pads to the MIDI sequencer mode and display the its MIDI settings controls. This mode allows sequences of MIDI note-on and note-off messages programmed into the AlphaSphere device."));
    }
    
    else if (sequencerGrid->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Circular Step-Sequencer Grid. This is where the sequence arrangement for the selected pads is drawn, edited and displayed. Like a traditional step-sequencer interface click on a grid block to create a 'note', or click on a note to delete it. Perform alt-click-drag on a note to change the velocity/gain of the note."));
    }
    else if (numberOfSequencesSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Number of Sequences Selector. Each Sequencer pad can hold up to 8 individual sequences which can then be switched between on-the-fly using the pads pressure or using certain Trigger Modes. This control sets and displays the number of sequences for the selected pads."));
    }
    else if (relativeTempoSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Relative Tempo Selector. Sets and displays the relative tempo of the sequences of the selected pads. The tempo can be set to Quarter Time (-2), Half Time (-1), Standard Time (0), Double Time (1), or Quadruple Time (2)."));
    }
    else if (noteLengthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("MIDI Note Length Selector. Sets and displays the MIDI note length (in sequence steps/beats) for the MIDI sequences of the selected pads. The note length can be no longer than the sequence length."));
    }
    else if (midiPressureMinRangeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiMinPressureRangeSlider));
    }
    else if (midiPressureMaxRangeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiMaxPressureRangeSlider));
    }
    
    if (audioPanSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Audio Pan Control. Sets and displays the stereo positioning of the audio signal of the sequences for the selected pads."));
    }
    else if (audioGainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Audio Gain Control. Sets and displays the gain of the audio signal of the sequences for the selected pads."));
    }
    
    else if (triggerModeButtons[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerModeStandard) + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    else if (triggerModeButtons[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerModeToggle) + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    else if (triggerModeButtons[2]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerModeLatch) + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    else if (triggerModeButtons[3]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerModeTrigger) + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    else if (triggerModeButtons[4]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("'Cycle' Mode - press the pad to play the first sequence, press again to change to the next sequence, and press to the pads full depth and release to stop.") + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    else if (triggerModeButtons[5]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("'Auto-Cycle' Mode - press the pad to play the sequence, and press again to stop. When it reaches the end of sequences it will automatically move onto the next sequence.") + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    
    else if (midiPressureModeButtons[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureChannelAT) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (midiPressureModeButtons[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureModWheel) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (midiPressureModeButtons[2]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureCC) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (midiPressureModeButtons[3]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressurePitchUp) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (midiPressureModeButtons[4]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressurePitchDown) + " " + translate(CommonInfoBoxText::midiPressureModes));
    }
    else if (pressureStatusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureStatusButton));
    }
    else if (indestructibleButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::indestructibleButton));
    }
    else if (stickyButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::stickyButton));
    }
    else if (loopButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::loopButton));
    }
    else if (finishLoopButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::finishLoopButton));
    }
    else if (linkButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Pressure Link Button. If this button is set to 'on' the pressure of the selected pads will be used to switch between the different sequences."));
    }
    else if (popUpButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sequence Options Button. Click this button to display a list of other sequence options. This button will also display the currently displayed/playing sequence number."));
    }
    else if (previewButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Preview Sequence Button. Use this button to preview the currently displayed sequence."));
    }
    else if (nextSequenceButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Next Sequence Button. Click this button to display the next sequence in the set."));
    }
    else if (previousSequenceButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Previous Sequence Button. Click this button to display the previous sequence in the set."));
    }
    
    
    if (sequenceSettingsButton->getToggleState() == true)
    {
        if (plusButton->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate("Increase Sequence Length Button. Use this button to make the sequences longer for the selected pads."));
        }
        
        else if (minusButton->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate("Decrease Sequence Length Button. Use this button to make the sequences shorter for the selected pads."));
        }
    }
    else if (triggerSettingsButton->getToggleState() == true)
    {
        if (plusButton->isMouseOver(true) || minusButton->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate("Use this pair of buttons to switch between a set of rotary controls above."));
        }
    }
    
    
    for (int i = 0; i < 16; i++)
    {
        if (midiChannelButtons[i]->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiChannelButtons) + " " + String(i+1) + ".");
            break;
        }
    }
    
    for (int i = 0; i < 12; i++)
    {
        
        
        if (audioRowButtons[i]->isMouseOver(true))
        {
            if (SINGLE_PAD)
            {
                int padNum = selectedPads[0];
                
                mainComponentRef.setInfoTextBoxText(translate("Sequencer Audio Sample Selector Buttons. Use these buttons to select the audio samples for the rows of the sequencer grid for the selected pads. Audio files can also be dragged-and-dropped onto the buttons. Use this button to select the audio sample for row") + " " + String(i+1) + ". " + translate("The current audio file set here is:") + " " + PAD_SETTINGS->getSequencerSamplesAudioFilePath(i).getFileName());
            }
            else if (MULTI_PADS)
            {
                 mainComponentRef.setInfoTextBoxText(translate("Sequencer Audio Sample Selector Buttons. Use these buttons to select the audio samples for the rows of the sequencer grid for the selected pads. Audio files can also be dragged-and-dropped onto the buttons. Use this button to select the audio sample for row") + " " + String(i+1) + ".");
            }
            
            break;
        }
    }
    
    
}

void GuiSequencerMode::mouseExit (const MouseEvent &e)
{
    
    if (e.eventComponent == numberOfSequencesSlider)
		setParameterLabelText(String::empty);
	else if (e.eventComponent == relativeTempoSlider)
		setParameterLabelText(String::empty);
	else if (e.eventComponent == noteLengthSlider)
		setParameterLabelText(String::empty);
	else if (e.eventComponent == audioGainSlider)
		setParameterLabelText(String::empty);
	else if (e.eventComponent == audioPanSlider)
		setParameterLabelText(String::empty);
    
    if(e.eventComponent == midiPressureMinRangeSlider || e.eventComponent == midiPressureMaxRangeSlider)
        parameterHoverLabel->setText(String::empty, false);
     
    
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
