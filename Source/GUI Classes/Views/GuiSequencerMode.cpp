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
#include "../Binary Data/MainBinaryData.h"
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
    numberOfSequencesSlider->setValue(1, dontSendNotification);
    numberOfSequencesSlider->addMouseListener(this, true);
	
	//--------------- relative tempo slider-------------------
	addChildComponent(relativeTempoSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	relativeTempoSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	relativeTempoSlider->setRange(-2, 2, 1);
    relativeTempoSlider->addListener(this);
    relativeTempoSlider->setValue(0, dontSendNotification);
    relativeTempoSlider->addMouseListener(this, true);
	
	//--------------- midi note length slider-------------------
	addChildComponent(noteLengthSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	noteLengthSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	noteLengthSlider->setRange(1, NO_OF_COLUMNS, 1);
    noteLengthSlider->addListener(this);
    noteLengthSlider->setValue(4, dontSendNotification);
    noteLengthSlider->addMouseListener(this, true);
	
	//--------------- audio gain slider-------------------
	addChildComponent(audioGainSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	audioGainSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	audioGainSlider->setRange(0.0, 2.0, 0.001);
    audioGainSlider->addListener(this);
    audioGainSlider->setValue(1.0, dontSendNotification);
    audioGainSlider->addMouseListener(this, true);
	
	//--------------- audio pan slider-------------------
	addChildComponent(audioPanSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	audioPanSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	audioPanSlider->setRange(0.0, 1.0, 0.001);
    audioPanSlider->addListener(this);
    audioPanSlider->setValue(0.5, dontSendNotification);
    audioPanSlider->addMouseListener(this, true);
    
    //--------------- audio attack slider-------------------
	addChildComponent(audioAttackSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	audioAttackSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	audioAttackSlider->setRange(0.0, 20.0, 0.01);
    audioAttackSlider->setSkewFactor(0.4);
    audioAttackSlider->addListener(this);
    audioAttackSlider->setValue(0.5, dontSendNotification);
    audioAttackSlider->addMouseListener(this, true);
    
    //--------------- audio polyphony slider-------------------
	addChildComponent(audioPolyphonySlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	audioPolyphonySlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	audioPolyphonySlider->setRange(0, 16, 1);
    audioPolyphonySlider->addListener(this);
    audioPolyphonySlider->setValue(4, dontSendNotification);
    audioPolyphonySlider->addMouseListener(this, true);

    
	addChildComponent(fxDial = new GuiFxDial(mainComponentRef));
	fxDial->setInterceptsMouseClicks(false, true);
    fxDial->addMouseListener(this, false);
	fxDial->setVisible(false);
    
    //----------------quantise button-------------------
	Image *quantiseIcon = new Image(ImageCache::getFromMemory(MainBinaryData::quantiseicon_png, MainBinaryData::quantiseicon_pngSize));
	addAndMakeVisible(quantiseButton = new ModeButton(quantiseIcon));
	quantiseButton->setClickingTogglesState(true);
	quantiseButton->setToggleState(false, dontSendNotification);
	quantiseButton->addListener(this);
	quantiseButton->addMouseListener(this, true);
	quantiseButton->setOpaque(false);
	
    //----------------sequence settings button-------------------
	Image *sequenceIcon = new Image(ImageCache::getFromMemory(MainBinaryData::sequenceicon_png, MainBinaryData::sequenceicon_pngSize));
	addAndMakeVisible(sequenceSettingsButton = new ModeButton(sequenceIcon));
	sequenceSettingsButton->setRadioGroupId (1234);
	sequenceSettingsButton->setClickingTogglesState(true);
	sequenceSettingsButton->setToggleState(false, dontSendNotification);
	sequenceSettingsButton->addListener(this);
	sequenceSettingsButton->addMouseListener(this, true);
	sequenceSettingsButton->setOpaque(false);
    sequenceSettingsButton->setToggleState(true, dontSendNotification);
	
	//----------------trigger settings button-------------------
	Image *triggerSettingsImage = new Image(ImageCache::getFromMemory(MainBinaryData::triggersettingsicon_png, MainBinaryData::triggersettingsicon_pngSize));
	addAndMakeVisible(triggerSettingsButton = new ModeButton(triggerSettingsImage));
	triggerSettingsButton->setRadioGroupId (1234);
	triggerSettingsButton->setClickingTogglesState(true);
	triggerSettingsButton->setToggleState(false, dontSendNotification);
	triggerSettingsButton->addListener(this);
	triggerSettingsButton->addMouseListener(this, true);
	triggerSettingsButton->setOpaque(false);
	
	//----------------pressure settings button-------------------
	Image *pressureSettingsImage = new Image(ImageCache::getFromMemory(MainBinaryData::pressuresettingsicon_png, MainBinaryData::pressuresettingsicon_pngSize));
	addAndMakeVisible(pressureSettingsButton = new ModeButton(pressureSettingsImage));
	pressureSettingsButton->setRadioGroupId (1234);
	pressureSettingsButton->setClickingTogglesState(true);
	pressureSettingsButton->setToggleState(false, dontSendNotification);
	pressureSettingsButton->addListener(this);
	pressureSettingsButton->addMouseListener(this, true);
	pressureSettingsButton->setOpaque(false);
	
	//----------------sequence MIDI button-------------------
	Image *modeMidiIcon = new Image(ImageCache::getFromMemory(MainBinaryData::midisymbol_png, MainBinaryData::midisymbol_pngSize));
	addAndMakeVisible(modeMidiButton = new ModeButton(modeMidiIcon));
	modeMidiButton->setRadioGroupId (123);
	modeMidiButton->setClickingTogglesState(true);
	modeMidiButton->addListener(this);
	modeMidiButton->addMouseListener(this, true);
    modeMidiButton->setToggleState(true, dontSendNotification);
	
	//----------------sequence Samples button-------------------
	Image *modeAudioIcon = new Image(ImageCache::getFromMemory(MainBinaryData::loopsymbol_png, MainBinaryData::loopsymbol_pngSize));
	addAndMakeVisible(modeSamplesButton = new ModeButton(modeAudioIcon));
	modeSamplesButton->setRadioGroupId (123);
	modeSamplesButton->setClickingTogglesState(true);
	modeSamplesButton->addListener(this);
	modeSamplesButton->addMouseListener(this, true);
    modeSamplesButton->setToggleState(false, dontSendNotification);
	
	//---------------channel buttons---------------------
	for (int i = 0; i <= 15; i++)
	{
		midiChannelButtons.insert(i, new AlphaTextButton());
		midiChannelButtons[i]->setButtonText(String(i + 1));
		midiChannelButtons[i]->setClickingTogglesState(true);
		midiChannelButtons[i]->setToggleState(false, dontSendNotification);
		midiChannelButtons[i]->setRadioGroupId (12);
		midiChannelButtons[i]->addListener(this);
        midiChannelButtons[i]->addMouseListener(this, true);
		midiChannelButtons[i]->setOpaque(false);
        addAndMakeVisible(midiChannelButtons[i]);
	}
    
    midiChannelButtons[0]->setToggleState(true, dontSendNotification);
	
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
    ccControllerSlider->setValue(-999);
    
    //------------------Pressure minimum range slider------------------------------
    addChildComponent(midiPressureMinRangeSlider = new AlphaRotarySlider((90 * (M_PI / 180)), (315 * (M_PI / 180)), 290));
	midiPressureMinRangeSlider->setRotaryParameters((90 * (M_PI / 180)), (315 * (M_PI / 180)),true);
    midiPressureMinRangeSlider->setRange(0, 127, 1);
    midiPressureMinRangeSlider->addListener(this);
    midiPressureMinRangeSlider->setValue(0, dontSendNotification);
    midiPressureMinRangeSlider->addMouseListener(this, true);
    
    
    //------------------Pressure maximum range slider------------------------------
    addChildComponent(midiPressureMaxRangeSlider = new AlphaRotarySlider((90 * (M_PI / 180)), (315 * (M_PI / 180)), 270));
	midiPressureMaxRangeSlider->setRotaryParameters((90 * (M_PI / 180)), (315 * (M_PI / 180)),true);
    midiPressureMaxRangeSlider->setRange(0, 127, 1);
    midiPressureMaxRangeSlider->addListener(this);
    midiPressureMaxRangeSlider->setValue(127, dontSendNotification);
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
	parameterLabel->setFont(Font(9 + AlphaTheme::getInstance()->fontSizeAddition));
	parameterLabel->setText("32", dontSendNotification);
    parameterLabel->setJustificationType(Justification::centred);
    //parameterLabel->setEditable(false, true, true);
    parameterLabel->addListener(this);
	
	addChildComponent(currentParameterLabel = new Label());
	currentParameterLabel->setFont(Font(10 + AlphaTheme::getInstance()->fontSizeAddition));
	currentParameterLabel->setText(translate("TEMPO"), dontSendNotification);
    currentParameterLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->iconOnColour);
    currentParameterLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
    currentParameterLabel->setJustificationType(Justification::centred);
    currentParameterLabel->setEditable(false, false, false);
    currentParameterLabel->addListener(this);
    
    //---------------hover parameter label -------------------------------------
    addChildComponent(parameterHoverLabel = new Label("value label", String()));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setFont(Font(11 + AlphaTheme::getInstance()->fontSizeAddition));
    parameterHoverLabel->addMouseListener(this, true);
    
    //----------------------Trigger mode buttons------------------
	Image *standardIcon = new Image(ImageCache::getFromMemory(MainBinaryData::standardicon_png, MainBinaryData::standardicon_pngSize));
	Image *toggleIcon = new Image(ImageCache::getFromMemory(MainBinaryData::toggleicon_png, MainBinaryData::toggleicon_pngSize));
	Image *latchIcon = new Image(ImageCache::getFromMemory(MainBinaryData::latchicon_png, MainBinaryData::latchicon_pngSize));
	Image *triggerIcon = new Image(ImageCache::getFromMemory(MainBinaryData::triggericon_png, MainBinaryData::triggericon_pngSize));
	Image *cycleIcon = new Image(ImageCache::getFromMemory(MainBinaryData::cycleicon_png, MainBinaryData::cycleicon_pngSize));
	Image *autoIcon = new Image(ImageCache::getFromMemory(MainBinaryData::autocycleicon_png, MainBinaryData::autocycleicon_pngSize));
	
	
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
    
    triggerModeButtons[1]->setToggleState(true, dontSendNotification);
    
    //--------------pressure mode buttons--------------------------
	
	//Image *patIcon = new Image(ImageCache::getFromMemory(MainBinaryData::polyphonicaftertouchicon_png, MainBinaryData::polyphonicaftertouchicon_pngSize));
	Image *catIcon = new Image(ImageCache::getFromMemory(MainBinaryData::channelaftertouchicon_png, MainBinaryData::channelaftertouchicon_pngSize));
	Image *mwheelIcon = new Image(ImageCache::getFromMemory(MainBinaryData::modwheelicon_png, MainBinaryData::modwheelicon_pngSize));
	Image *ccIcon = new Image(ImageCache::getFromMemory(MainBinaryData::ccmessageicon_png, MainBinaryData::ccmessageicon_pngSize));
	Image *pbUpIcon = new Image(ImageCache::getFromMemory(MainBinaryData::pitchbendupicon_png, MainBinaryData::pitchbendupicon_pngSize));
	Image *pbDownIcon = new Image(ImageCache::getFromMemory(MainBinaryData::pitchbenddownicon_png, MainBinaryData::pitchbenddownicon_pngSize));
	
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
    
    midiPressureModeButtons[0]->setToggleState(true, dontSendNotification);
        /*
    addAndMakeVisible(numberOfSequencesSlider = new AlphaSlider());
    numberOfSequencesSlider->setRange(1, NO_OF_SEQS, 1);
    numberOfSequencesSlider->addListener(this);
    numberOfSequencesSlider->setValue(NO_OF_SEQS, false);
    numberOfSequencesSlider->addMouseListener(this, true);
     */
    
	Image *loopIcon = new Image(ImageCache::getFromMemory(MainBinaryData::loopicon_png, MainBinaryData::loopicon_pngSize));
    addChildComponent(loopButton =new ModeButton(loopIcon));
    loopButton->addListener(this);
    loopButton->addMouseListener(this, true);
    loopButton->setClickingTogglesState(true);
    loopButton->setToggleState(1, dontSendNotification);
    
    Image *destructIcon = new Image(ImageCache::getFromMemory(MainBinaryData::indestructableicon_png, MainBinaryData::indestructableicon_pngSize));
	addChildComponent(indestructibleButton = new ModeButton(destructIcon));
    indestructibleButton->addListener(this);
    indestructibleButton->addMouseListener(this, true);
    indestructibleButton->setClickingTogglesState(true);
    
    Image *finishIcon = new Image(ImageCache::getFromMemory(MainBinaryData::finishicon_png, MainBinaryData::finishicon_pngSize));
    addChildComponent(finishLoopButton =new ModeButton(finishIcon));
    finishLoopButton->addListener(this);
    finishLoopButton->addMouseListener(this, true);
    finishLoopButton->setClickingTogglesState(true);
    
    Image *stickyIcon = new Image(ImageCache::getFromMemory(MainBinaryData::stickyicon_png, MainBinaryData::stickyicon_pngSize));
    addChildComponent(stickyButton = new ModeButton(stickyIcon));
    stickyButton->addListener(this);
    stickyButton->addMouseListener(this, true);
    stickyButton->setClickingTogglesState(true);

	//---------------reverse pressure range button-------------------------------------
    addChildComponent(reverseMidiPressureRangeButton = new AlphaTextButton());
    reverseMidiPressureRangeButton->setButtonText(translate("REV"));
    reverseMidiPressureRangeButton->addListener(this);
    reverseMidiPressureRangeButton->addMouseListener(this, true);
    
    //PLEASE NOTE: ANYTHING PLACED AFTER HERE WILL NOT BE HIDDEN BY THE STATUS/PRESSURE OFF GRAPHIC

    //---------------status off bg-------------------------------------
    addChildComponent(notSelected = new GuiCircleBackground());
	notSelected->setVisible(false);
	notSelected->drawButtonBackground();
	
	//---------------pressure status button-------------------------------------
    
    addChildComponent(pressureStatusButton = new GuiSwitch());
    pressureStatusButton->addListener(this);
    pressureStatusButton->setClickingTogglesState(true);
    pressureStatusButton->setToggleState(true, dontSendNotification);
    pressureStatusButton->addMouseListener(this, false);
    
    Image *linkIcon = new Image(ImageCache::getFromMemory(MainBinaryData::linkicon_png, MainBinaryData::linkicon_pngSize));
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
    //drawing and setting the images is now done in drawDrawableButtons()
	previewButton = new DrawableButton ("preview", DrawableButton::ImageOnButtonBackground);
	previewButton->setClickingTogglesState (true);
	previewButton->addListener (this);
    previewButton->addMouseListener(this, true);
	//previewButton->setBackgroundColours (AlphaTheme::getInstance()->backgroundColour, AlphaTheme::getInstance()->mainColour);
	addAndMakeVisible (previewButton);
	
	addAndMakeVisible(nextSequenceButton = new AlphaTextButton());
	nextSequenceButton->setButtonText("+");
	nextSequenceButton->addListener(this);
	nextSequenceButton->addMouseListener(this, true);
	
	addAndMakeVisible(previousSequenceButton = new AlphaTextButton());
	previousSequenceButton->setButtonText("-");
	previousSequenceButton->addListener(this);
	previousSequenceButton->addMouseListener(this, true);
    
    //record button
	//drawing and setting the images is now done in drawDrawableButtons()
    recordButton = new DrawableButton ("record", DrawableButton::ImageOnButtonBackground);
    recordButton->setClickingTogglesState(true);	
	recordButton->addListener(this);
	recordButton->addMouseListener(this, true);
    recordButton->setColour(TextButton::buttonOnColourId, Colours::red);
	addAndMakeVisible (recordButton);
    
    drawDrawableButtons();
    
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
    audioAttackSlider->setBounds(804, 381, 82, 82);
    audioPolyphonySlider->setBounds(804, 381, 82, 82);
	
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
	
	quantiseButton->setBounds(681, 288, 32, 32);
    recordButton->setBounds(914, 241, 20, 20);
	modeMidiButton->setBounds(952, 263, 32, 32);
	modeSamplesButton->setBounds(980, 293, 32, 32);
	
	sequenceSettingsButton->setBounds(735, 241,42, 42);
	triggerSettingsButton->setBounds(789, 221,42, 42);
	pressureSettingsButton->setBounds(847, 219,42, 42);
	
	triggerModeButtons[0]->setBounds(728, 305, 234, 234);
	triggerModeButtons[1]->setBounds(728, 305, 234, 234);
	triggerModeButtons[2]->setBounds(728, 305, 234, 234);
	triggerModeButtons[3]->setBounds(728, 305, 234, 234);
	triggerModeButtons[4]->setBounds(728, 305, 234, 234);
	triggerModeButtons[5]->setBounds(728, 305, 234, 234);
	
	finishLoopButton->setBounds(894, 472,32, 32);
	indestructibleButton->setBounds(853, 496,32, 32);
	stickyButton->setBounds(853, 496,32, 32);
	loopButton->setBounds(918, 431,32, 32);
	
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
    
    //reverseMidiPressureRangeButton->setBounds(753, 309, 21, 21); //at the end of the range sliders
    reverseMidiPressureRangeButton->setBounds(969, 395, 21, 21); //at the beginning of the range sliders
}


void GuiSequencerMode::paint (Graphics& g)
{
    parameterHoverLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->mainColour);

    //=============================================================================
    //draw background circles for particular controls
    
	ColourGradient fillGradient(AlphaTheme::getInstance()->childBackgroundColour,845 , 461, AlphaTheme::getInstance()->backgroundColour, 845 , 383, false);
	g.setGradientFill(fillGradient);
	g.fillEllipse(802, 379, 86, 86);
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	
	g.fillEllipse(786,218, 48, 48);
	g.fillEllipse(844,216, 48, 48);
	g.fillEllipse(732,238, 48, 48);
	g.fillEllipse(678,285, 38, 38);
	g.fillEllipse(949,260, 38, 38);
	g.fillEllipse(977,290, 38, 38);
	g.fillEllipse(910, 237, 28, 28);
	
	
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
		
		
		g.setColour(AlphaTheme::getInstance()->childBackgroundColour);
		g.fillPath(linkButtonBg, getTransform());
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		g.fillEllipse(800,265, 38, 38);
        
        //reverse pressure range button
        if (modeMidiButton->getToggleState())
            g.fillEllipse(966, 392, 27, 27);
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
    
    //=============================================================================
    //draw outlines for the background circles for particular controls
	
	g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
	
	g.drawEllipse(678,285, 38, 38, 1.0);
	g.drawEllipse(949,260, 38, 38, 1.0);
	g.drawEllipse(977,290, 38, 38, 1.0);
	g.drawEllipse(910, 237, 28, 28, 1.0);
	
	if(triggerSettingsButton->getToggleStateValue()==true)
	{
        g.drawEllipse(850,493, 38, 38, 1.0);
		g.drawEllipse(891,469, 38, 38, 1.0);
		g.drawEllipse(915,428, 38, 38, 1.0);
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		Path pieSeg;
		pieSeg.addPieSegment(802, 379, 86, 86, (125 * (M_PI / 180)), (235 * (M_PI / 180)), 0.2f);
		g.fillPath(pieSeg);
		
		g.fillEllipse(816, 393, 58, 58);
		
		ColourGradient fillGradient(AlphaTheme::getInstance()->mainColour, 816+(58*0.5), 393+(58*0.6), AlphaTheme::getInstance()->mainColourLighter, 816+(58*0.5), 393, false);
		g.setGradientFill(fillGradient);
        
		g.fillEllipse((816+(58*0.15)), (393+(58*0.15)), (58*0.7), (58*0.7));
		
		g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
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
        
        //reverse pressure range button
        if (pressureSettingsButton->getToggleState())
            g.drawEllipse(966, 392, 27, 27, 1.0);
		
	}
	
    if(sequenceSettingsButton->getToggleStateValue()==true)
	{
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		g.fillEllipse(662, 366, 27, 27);
		g.fillEllipse(657, 396, 27, 27);
		g.fillEllipse(672, 337, 27, 27);
		
		g.setColour(AlphaTheme::getInstance()->childBackgroundColour);
		g.fillEllipse(689, 266, 312, 312);
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		g.fillEllipse(698, 275, 294, 294);
		
		g.setColour(AlphaTheme::getInstance()->childBackgroundColour);
		g.fillEllipse(707, 284, 276, 276);
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		g.fillEllipse(716, 293, 258, 258);
		
		g.setColour(AlphaTheme::getInstance()->childBackgroundColour);
		g.fillEllipse(725, 302, 240, 240);
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		g.fillEllipse(734, 311, 222, 222);
		
		g.setColour(AlphaTheme::getInstance()->childBackgroundColour);
		g.fillEllipse(743, 320, 204, 204);
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		g.fillEllipse(752, 329, 186, 186);
		
		g.setColour(AlphaTheme::getInstance()->childBackgroundColour);
		g.fillEllipse(761, 338, 168, 168);
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		g.fillEllipse(770, 347, 150, 150);
		
		g.setColour(AlphaTheme::getInstance()->childBackgroundColour);
		g.fillEllipse(779, 356, 132, 132);
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		g.fillEllipse(788, 365, 114, 114);
		
		g.setColour(AlphaTheme::getInstance()->childBackgroundColour);
		g.drawEllipse(797, 374, 96, 96,1.0);
		
		ColourGradient fillGradient(AlphaTheme::getInstance()->childBackgroundColour,845 , 461, AlphaTheme::getInstance()->backgroundColour, 845 , 383, false);
		g.setGradientFill(fillGradient);
		
		g.fillEllipse(802, 379, 86, 86);
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
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
    
    else if (slider == audioAttackSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerSamplesAttackTime(slider->getValue());
        }
        
		setParameterLabelText(String(audioAttackSlider->getValue()));
    }
    
    else if (slider == audioPolyphonySlider)
    {
        if (slider->getValue() == 0)
            slider->setValue(1, dontSendNotification);
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerSamplesPolyphony(slider->getValue());
        }
        
		setParameterLabelText(String(audioPolyphonySlider->getValue()));
    }
    
    else if (slider == numberOfSequencesSlider)
    {
        if (slider->getValue() == 0)
            slider->setValue(1, dontSendNotification);
        
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
        
        parameterHoverLabel->setText(String(slider->getValue()), dontSendNotification);
    }
    
    
    //max pressure range slider
    else if (slider == midiPressureMaxRangeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerMidiMaxPressureRange(midiPressureMaxRangeSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue()), dontSendNotification);
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
		
		if (modeSamplesButton->getToggleState() == true) 
		{
            menu.addItem(16, translate("Import Sample Bank..."));
            
            if (SINGLE_PAD) 
            {
                menu.addItem(15, translate("Export Sample Bank..."));
            }
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
		else if (result == 15)//export and save .alphabank files
		{
			mainComponentRef.getAppDocumentStateRef().exportSampleBank(selectedPads[0]);
		}
        else if (result == 16)//import .alphabank files
		{
			mainComponentRef.getAppDocumentStateRef().importSampleBank(selectedPads);
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
        
        if (button->getToggleState() == true)
        {
            if (finishLoopButton->getToggleState() == true)
            {
                //turn off finish Loop mode, as these two modes can't work together
                finishLoopButton->setToggleState(false, sendNotification);
            }
        }
    }
    
    else if (button == finishLoopButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerShouldFinishLoop(button->getToggleState());
        }
        
        if (button->getToggleState() == true)
        {
            if (indestructibleButton->getToggleState() == true)
            {
                //turn off indestructible mode, as these two modes can't work together
                indestructibleButton->setToggleState(false, sendNotification);
            }
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
    
    else if (button == recordButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSequencerRecordEnabled(button->getToggleState());
        }
    }
    
    
    //reverse midi pressure range button
    else if(button == reverseMidiPressureRangeButton)
    {
        //reverse/swap the min and max pressure values
        //within PadSettings for each selected pad
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            int minPressure = PAD_SETTINGS->getSequencerMidiMinPressureRange();
            int maxPressure = PAD_SETTINGS->getSequencerMidiMaxPressureRange();
            
            PAD_SETTINGS->setSequencerMidiMinPressureRange(maxPressure);
            PAD_SETTINGS->setSequencerMidiMaxPressureRange(minPressure);
        }
        
        //reverse/swap the min and max pressure values
        //on the current GUI. This needs to be done
        //seperatly from that of above incase the sliders
        //are currently showing default values due to selecting
        //multiple pads containing different values. Also,
        //we only want to update the GUI once, unlike above.
        {
            int minPressure = midiPressureMinRangeSlider->getValue();
            int maxPressure = midiPressureMaxRangeSlider->getValue();
            
            midiPressureMinRangeSlider->setValue(maxPressure, dontSendNotification);
            midiPressureMaxRangeSlider->setValue(minPressure, dontSendNotification);
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
                                   AppSettings::Instance()->getLastAudioSampleDirectory(),
                                   "*.wav;*.aif;*.aiff");
            
            if (myChooser.browseForFileToOpen() == true)
            {
                File selectedAudioFile (myChooser.getResult());
                
                for (int i = 0; i < selectedPads.size(); i++)
                {
                    int padNum = selectedPads[i];
                    PAD_SETTINGS->setSequencerSamplesAudioFilePath(selectedAudioFile, row);
                }
                
                AppSettings::Instance()->setLastAudioSampleDirectory(selectedAudioFile.getParentDirectory());
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
    numberOfSequencesSlider->setVisible(false);
    
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
    audioAttackSlider->setVisible(false);
    audioPolyphonySlider->setVisible(false);
    minusButton->setVisible(false);
    parameterLabel->setVisible(false);
    currentParameterLabel->setVisible(false);
    
    for (int i = 0; i < 16; i++)
        midiChannelButtons[i]->setVisible(false);
    for (int i = 0; i < 12; i++)
        audioRowButtons[i]->setVisible(false);
    //setParameterLabelText(String());
    
    reverseMidiPressureRangeButton->setVisible(false);
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
                
                reverseMidiPressureRangeButton->setVisible(true);
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
    audioAttackSlider->setVisible(false);
    audioPolyphonySlider->setVisible(false);
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
            if (controlDisplayId > 4)
                controlDisplayId = 0;
            else if (controlDisplayId < 0)
                controlDisplayId = 4;
            
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
            else if (controlDisplayId == 3)
            {
                audioAttackSlider->setVisible(true);
            }
            else if (controlDisplayId == 4)
            {
                audioPolyphonySlider->setVisible(true);
            }
        }
    }
    
    
    else if(sequenceSettingsButton->getToggleStateValue() == true)
    {
        numberOfSequencesSlider->setVisible(true);
    }
    
    
    setParameterLabelText (String());
    
}




void GuiSequencerMode::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/disabled any uneeded components.
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getSequencerMode() == 1) //midi mode
        {
            modeMidiButton->setToggleState(true, dontSendNotification);
            pressureStatusButton->setToggleState(PAD_SETTINGS->getSequencerMidiPressureStatus(), dontSendNotification);
            
            mainComponentRef.getGuiPiano()->setActive(true);
            mainComponentRef.getGuiPiano()->updateDisplay();
            
            modeSamplesButton->setToggleState(false, dontSendNotification);
        }
        else if (PAD_SETTINGS->getSequencerMode() == 2) //samples mode
        {
            modeSamplesButton->setToggleState(true, dontSendNotification);
            mainComponentRef.getGuiPiano()->setActive(false);
            
            
            if (PAD_SETTINGS->getSequencerEffect() == 0)
                pressureStatusButton->setToggleState(false, dontSendNotification);
            else
                pressureStatusButton->setToggleState(true, dontSendNotification);
            //effect is found a set with fxDial
            
            modeMidiButton->setToggleState(false, dontSendNotification);
             
        }
        
        
        //calling setDisplay() at the bottom will in turn display the correct mode components
        
        quantiseButton->setToggleState(PAD_SETTINGS->getQuantizeMode(), dontSendNotification);
        sequenceLength = PAD_SETTINGS->getSequencerLength();
        numberOfSequencesSlider->setValue(PAD_SETTINGS->getSequencerNumberOfSequences(), dontSendNotification);
        relativeTempoSlider->setValue(PAD_SETTINGS->getSequencerRelativeTempoMode(), dontSendNotification);
        noteLengthSlider->setValue(PAD_SETTINGS->getSequencerMidiNoteLength(), dontSendNotification);
        setNoteLengthSliderRange(sequenceLength);
        audioGainSlider->setValue(PAD_SETTINGS->getSequencerGain(), dontSendNotification);
        audioPanSlider->setValue(PAD_SETTINGS->getSequencerPan(), dontSendNotification);
        audioAttackSlider->setValue(PAD_SETTINGS->getSequencerSamplesAttackTime(), dontSendNotification);
        audioPolyphonySlider->setValue(PAD_SETTINGS->getSequencerSamplesPolyphony(), dontSendNotification);
        triggerModeButtons[PAD_SETTINGS->getSequencerTriggerMode()-1]->setToggleState(true, dontSendNotification);
        midiPressureModeButtons[PAD_SETTINGS->getSequencerMidiPressureMode()-1]->setToggleState(true, dontSendNotification);
        midiChannelButtons[PAD_SETTINGS->getSequencerMidiChannel()-1]->setToggleState(true, dontSendNotification);
        ccControllerSlider->setValue(PAD_SETTINGS->getSequencerMidiCcController(), dontSendNotification);
        loopButton->setToggleState(PAD_SETTINGS->getSequencerShouldLoop(), dontSendNotification);
        indestructibleButton->setToggleState(PAD_SETTINGS->getSequencerIndestructible(), dontSendNotification);
        finishLoopButton->setToggleState(PAD_SETTINGS->getSequencerShouldFinishLoop(), dontSendNotification);
        stickyButton->setToggleState(PAD_SETTINGS->getSequencerSticky(), dontSendNotification);
        linkButton->setToggleState(PAD_SETTINGS->getSequencerDynamicMode(), dontSendNotification);
        midiPressureMinRangeSlider->setValue(PAD_SETTINGS->getSequencerMidiMinPressureRange(), dontSendNotification);
        midiPressureMaxRangeSlider->setValue(PAD_SETTINGS->getSequencerMidiMaxPressureRange(), dontSendNotification);
        recordButton->setToggleState(PAD_SETTINGS->getSequencerRecordEnabled(), dontSendNotification);
        
        
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
                modeMidiButton->setToggleState(0, dontSendNotification);
                modeSamplesButton->setToggleState(0, dontSendNotification);
                mainComponentRef.getGuiPiano()->setActive(false);
                pressureStatusButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
            {
                if (mode_ == 1)
                {
                    modeMidiButton->setToggleState(true, dontSendNotification);
                    mainComponentRef.getGuiPiano()->setActive(true);
                    mainComponentRef.getGuiPiano()->updateDisplay();
                    
                    //==================================================================================================
                    int pressureStatus_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiPressureStatus();
                    for (int i = 1; i < selectedPads.size(); i++)
                    {
                        int padNum = selectedPads[i];
                        if (PAD_SETTINGS->getSequencerMidiPressureStatus() != pressureStatus_)
                        {
                            pressureStatusButton->setToggleState(0, dontSendNotification);
                            break;
                        }
                        if (i == selectedPads.size()-1)
                            pressureStatusButton->setToggleState(pressureStatus_, dontSendNotification);
                    }
                }
                else
                {
                    modeSamplesButton->setToggleState(true, dontSendNotification);
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
                            pressureStatusButton->setToggleState(0, dontSendNotification);
                            break;
                        }
                        if (i == selectedPads.size()-1)
                        {
                            if (effect_ == 0)
                                pressureStatusButton->setToggleState(0, dontSendNotification);
                            else
                                pressureStatusButton->setToggleState(1, dontSendNotification);
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
                quantiseButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                quantiseButton->setToggleState(quantiseMode_, dontSendNotification);
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
                numberOfSequencesSlider->setValue(1, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                numberOfSequencesSlider->setValue(noOfSeqs_, dontSendNotification);
        }
        
        //==================================================================================================
        int relativeTempo_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerRelativeTempoMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerRelativeTempoMode() != relativeTempo_)
            {
                relativeTempoSlider->setValue(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                relativeTempoSlider->setValue(relativeTempo_, dontSendNotification);
        }
        
        //==================================================================================================
        int noteLength_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiNoteLength();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiNoteLength() != noteLength_)
            {
                noteLengthSlider->setValue(1, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                noteLengthSlider->setValue(noteLength_, dontSendNotification);
        }
        
        //==================================================================================================
        double gain_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerGain();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerGain() != gain_)
            {
                audioGainSlider->setValue(1.0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                audioGainSlider->setValue(gain_, dontSendNotification);
        }
        
        //==================================================================================================
        double pan_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerPan();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerPan() != pan_)
            {
                audioPanSlider->setValue(0.5, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                audioPanSlider->setValue(pan_, dontSendNotification);
        }
        
        //==================================================================================================
        double attack_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerSamplesAttackTime();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerSamplesAttackTime() != attack_)
            {
                audioAttackSlider->setValue(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                audioAttackSlider->setValue(attack_, dontSendNotification);
        }
        
        //==================================================================================================
        double polyphony_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerSamplesPolyphony();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerSamplesPolyphony() != polyphony_)
            {
                audioPolyphonySlider->setValue(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                audioPolyphonySlider->setValue(polyphony_, dontSendNotification);
        }
        
        //==================================================================================================
        int triggerMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerTriggerMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerTriggerMode() != triggerMode_)
            {
                for (int i = 0; i < 6; i++)
                    triggerModeButtons[i]->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                triggerModeButtons[triggerMode_-1]->setToggleState(true, dontSendNotification);
        }
        
        //==================================================================================================
        int pressureMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiPressureMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiPressureMode() != pressureMode_)
            {
                for (int i = 0; i < 5; i++)
                    midiPressureModeButtons[i]->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                midiPressureModeButtons[pressureMode_-1]->setToggleState(true, dontSendNotification);
        }
        
        //==================================================================================================
        int channel_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiChannel();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiChannel() != channel_)
            {
                for (int i = 0; i <16; i++)
                    midiChannelButtons[i]->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                midiChannelButtons[channel_-1]->setToggleState(true, dontSendNotification);
        }
        
        //==================================================================================================
        int ccNumber_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiCcController();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiCcController() != ccNumber_)
            {
                ccControllerSlider->setValue(-999);
                break;
            }
            if (i == selectedPads.size()-1)
                ccControllerSlider->setValue(ccNumber_);
        }
        
        //==================================================================================================
        int indestructible_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerIndestructible();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerIndestructible() != indestructible_)
            {
                indestructibleButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                indestructibleButton->setToggleState(indestructible_, dontSendNotification);
        }
        
        //==================================================================================================
        int sticky_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerSticky();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerSticky() != sticky_)
            {
                stickyButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                stickyButton->setToggleState(sticky_, dontSendNotification);
        }
        
        //==================================================================================================
        int loop_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerShouldLoop();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerShouldLoop() != loop_)
            {
                loopButton->setToggleState(1, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                loopButton->setToggleState(loop_, dontSendNotification);
        }
        
        //==================================================================================================
        int finishLoop_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerShouldFinishLoop();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerShouldFinishLoop() != finishLoop_)
            {
                finishLoopButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                finishLoopButton->setToggleState(finishLoop_, dontSendNotification);
        }
        
        //==================================================================================================
        int dynamicMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerDynamicMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerDynamicMode() != dynamicMode_)
            {
                linkButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                linkButton->setToggleState(dynamicMode_, dontSendNotification);
        }
        
        //==================================================================================================
        int pressureMinRange_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiMinPressureRange();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiMinPressureRange() != pressureMinRange_)
            {
                midiPressureMinRangeSlider->setValue(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                midiPressureMinRangeSlider->setValue(pressureMinRange_, dontSendNotification);
        }
        
        //==================================================================================================
        int pressureMaxRange_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerMidiMaxPressureRange();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerMidiMaxPressureRange() != pressureMaxRange_)
            {
                midiPressureMaxRangeSlider->setValue(127, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                midiPressureMaxRangeSlider->setValue(pressureMaxRange_, dontSendNotification);
        }
        
        int recordEnabled_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerRecordEnabled();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSequencerRecordEnabled() != recordEnabled_)
            {
                recordButton->setToggleState(0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                recordButton->setToggleState(recordEnabled_, dontSendNotification);
        }
        
    }
    
    if (previewButton->getToggleState() == true)
    {
        previewButton->setToggleState(false, dontSendNotification);
    }
    
    
    //update sequencerGrid GUI based on currently selected pad and currently selected sequence number
    sequencerGrid->setCurrentlySelectedPad(selectedPads); //why is this called here and not above?
                        //is it because setCurrentlySelectedPad is used as an updateDisplay() too?
    
    if (currentSequenceNumber > numberOfSequencesSlider->getValue())
        currentSequenceNumber = numberOfSequencesSlider->getValue();
    
    setCurrentSequenceNumber();
    
    fxDial->updateDisplay();
    mainComponentRef.getToolbox()->updateDisplay();
    sequencerGrid->setPlayHeadPos(-1);
    
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
    //String() and the currently visible control is found and the name is
    //put into the label, as well as putting the value of it into the other label.
    
    if (value != String())
    {
	parameterLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->mainColourLighter);
	parameterLabel->setText(value, dontSendNotification);
    }
	
	else if (value == String())
	{
		parameterLabel->setColour(Label::textColourId, LookAndFeel::getDefaultLookAndFeel().findColour(Label::textColourId));
		
		if (numberOfSequencesSlider->isVisible())
		{
            //here want to display sequenceLength, NOT number of sequences.
            //Is this too convoluted?
            parameterLabel->setText(String(sequenceLength), dontSendNotification);
            //parameterLabel->setText(String(numberOfSequencesSlider->getValue()), false);
        }
		
		else if (relativeTempoSlider->isVisible())
		{
			currentParameterLabel->setText(translate("TEMPO"), dontSendNotification);
			parameterLabel->setText(String(relativeTempoSlider->getValue()), dontSendNotification);
		}
		
		else if (noteLengthSlider->isVisible())
		{
			currentParameterLabel->setText(translate("N LENGTH"), dontSendNotification);
			parameterLabel->setText(String(noteLengthSlider->getValue()), dontSendNotification);
		}
		
		else if (audioGainSlider->isVisible())
		{
			currentParameterLabel->setText(translate("GAIN"), dontSendNotification);
			parameterLabel->setText(String(audioGainSlider->getValue()), dontSendNotification);
		}
		
		else if (audioPanSlider->isVisible())
		{
			currentParameterLabel->setText(translate("PAN"), dontSendNotification);
			parameterLabel->setText(String(audioPanSlider->getValue()), dontSendNotification);
			
		}
        
        else if (audioAttackSlider->isVisible())
		{
			currentParameterLabel->setText(translate("ATTACK"), dontSendNotification);
			parameterLabel->setText(String(audioAttackSlider->getValue()), dontSendNotification);
			
		}
        
        else if (audioPolyphonySlider->isVisible())
		{
			currentParameterLabel->setText(translate("POLYPHONY"), dontSendNotification);
			parameterLabel->setText(String(audioPolyphonySlider->getValue()), dontSendNotification);
			
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

Button* GuiSequencerMode::getRecordButton()
{
    return recordButton;
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
            noteLengthSlider->setValue(maxValue, dontSendNotification); //update the slider display, but don;t send an update
        }
    }
    
    
}

void GuiSequencerMode::changeView (int view)
{
    // view - 1 - trigger
    // view - 2 - pressure
    // view - 3 - sequence
    // view - 0 - switch to other view
    
    switch (view)
    {
        case 1:
            triggerSettingsButton->triggerClick();
            break;
        case 2:
            pressureSettingsButton->triggerClick();
            break;
        case 3:
            sequenceSettingsButton->triggerClick();
            break;
        default:
            if (sequenceSettingsButton->getToggleState())
                triggerSettingsButton->triggerClick();
            else if (triggerSettingsButton->getToggleState())
                pressureSettingsButton->triggerClick();
            else if (pressureSettingsButton->getToggleState())
                sequenceSettingsButton->triggerClick();
            break;
    }
}

void GuiSequencerMode::drawDrawableButtons()
{
    Path r;
	r.addEllipse(0, 0, 2, 2);
	recordButtonNormalPath.setPath (r);
	recordButtonNormalPath.setFill (AlphaTheme::getInstance()->iconColour);
	
	r.clear();
	r.addEllipse(0, 0, 2, 2);
	recordButtonDownPath.setPath (r);
	recordButtonDownPath.setFill (AlphaTheme::getInstance()->iconColour);
	r.clear();
	
	
	recordButton->setImages (&recordButtonNormalPath,
                             &recordButtonNormalPath,
                             &recordButtonNormalPath,
                             0,
                             &recordButtonDownPath,
                             &recordButtonDownPath,
                             &recordButtonDownPath, 0);
    
    Path p;
	p.addTriangle(0, 0, 0, 4, 4.5, 2);
	previewButtonNormalPath.setPath (p);
	previewButtonNormalPath.setFill (AlphaTheme::getInstance()->iconColour);
	
	p.clear();
	p.addQuadrilateral(0, 0, 0, 4, 4, 4, 4, 0);
	previewButtonDownPath.setPath (p);
	previewButtonDownPath.setFill (AlphaTheme::getInstance()->iconColour);
	p.clear();
	
	
    
	previewButton->setImages (&previewButtonNormalPath,
                              &previewButtonNormalPath,
                              &previewButtonNormalPath,
                              0, &previewButtonDownPath,
                              &previewButtonDownPath,
                              &previewButtonDownPath,
                              0);
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
    else if (audioAttackSlider->isMouseOver(true))
		setParameterLabelText(String(audioAttackSlider->getValue()));
    else if (audioPolyphonySlider->isMouseOver(true))
		setParameterLabelText(String(audioPolyphonySlider->getValue()));
    
    
    //update parameterHoverLabel
    if (midiPressureMinRangeSlider->isMouseOver(true))
        parameterHoverLabel->setText(String(midiPressureMinRangeSlider->getValue()), dontSendNotification);
    else if (midiPressureMaxRangeSlider->isMouseOver(true))
        parameterHoverLabel->setText(String(midiPressureMaxRangeSlider->getValue()), dontSendNotification);
     
    
    // ======= info box text command =========
    // =======================================
    
    if (quantiseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::quantizeButton));
    }
    else if (sequenceSettingsButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sequence Settings. Click to display the circular step-sequencer grid. The grid can be used to draw and edit the sequence arrangement for the selected pads and to access other sequence settings."));
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
        mainComponentRef.setInfoTextBoxText(translate("Sequencer MIDI Mode. Click to set the selected pads to MIDI sequencer mode. This mode allows sequences of MIDI messages to be recorded, programmed and edited. This will also display the MIDI settings controls for the selected pads."));
    }
	if (modeSamplesButton->isMouseOver(true))
	{
		mainComponentRef.setInfoTextBoxText(translate("Sequencer Sampler Mode. Click to set the selected pads to sampler sequencer mode. This mode allows sequences of audio samples to be recorded, programmed and edited. This will also display the sampler settings controls for the selected pads."));
	}
    
    else if (sequencerGrid->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Circular Step-Sequencer Grid. This is where the sequence arrangement for the selected pads is drawn and edited. This works like a traditional step-sequencer interface - clicking a grid block creates a 'note', clicking the note again removes it. Alt-click-drag on a note to change its velocity."));
    }
    else if (numberOfSequencesSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Number of Sequences Selector. Each Sequencer pad can hold up to 8 sequences. You can switch between these sequences on-the-fly using pad pressure and certain Pad Behaviour Modes. This control sets the number of sequences for the selected pads."));
    }
    else if (relativeTempoSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Relative Tempo Selector. Sets the relative tempo of all sequences on selected pads. The tempo settings are...  Quarter Time (-2), Half Time (-1), Standard Time (0), Double Time (1), or Quadruple Time (2)."));
    }
    else if (noteLengthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("MIDI Note Length Selector. Sets MIDI note length (in sequence steps/beats) for all MIDI sequences on the selected pads. Note length can be not longer than the sequence length."));
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
        mainComponentRef.setInfoTextBoxText(translate("Audio Pan. Sets the stereo positioning of the sequencer audio signal on the selected pads."));
    }
    else if (audioGainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Audio Gain. Sets the gain of the sequencer audio signal on the selected pads."));
    }
    else if (audioAttackSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sample Attack Time. Sets the audio sample attack time in seconds for selected pads."));
    }
    
    else if (audioPolyphonySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Polyphony Value. Sets how many instances of each audio sample can be playing simultaneously."));
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
        mainComponentRef.setInfoTextBoxText(translate("'Cycle' Mode - press the pad to play the first sequence, press again to switch to the next sequence, and press the pad to its full depth and release to stop.") + " " + translate(CommonInfoBoxText::triggerModeButtons));
    }
    else if (triggerModeButtons[5]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("'Auto-Cycle' Mode - press the pad to play through the sequences, and press again to stop. At the end of each sequence the sequencer will automatically move onto the next.") + " " + translate(CommonInfoBoxText::triggerModeButtons));
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
        mainComponentRef.setInfoTextBoxText(translate("Pressure Link. If this button is set to 'on' the pressure of the selected pads will be used to switch between the different sequences."));
    }
    else if (popUpButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sequence Options. Click to display a list of other sequence options. This button also displays the currently selected sequence number."));
    }
    else if (previewButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Preview Sequence. Use this button to preview the currently displayed sequence."));
    }
    else if (nextSequenceButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Next Sequence. Click this button to display the next sequence in the set."));
    }
    else if (previousSequenceButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Previous Sequence. Click this button to display the previous sequence in the set."));
    }
    else if (recordButton->isMouseOver(true))
    {
        if (modeMidiButton->getToggleState())
            mainComponentRef.setInfoTextBoxText(translate("Record. When a sequencer pad is set to Record Mode it will listen for MIDI note-on data from MIDI pads that match the channel and note numbers set within this pad, and records the notes to the sequence."));
        else if (modeSamplesButton->getToggleState())
            mainComponentRef.setInfoTextBoxText(translate("Record. When a sequencer pad is set to Record Mode it will listen for triggered audio samples that match the the set of samples set within this pad, and record the notes to the sequence."));
    }
    
    if (sequenceSettingsButton->getToggleState() == true)
    {
        if (plusButton->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate("Increase Sequence Length. This is used to make the sequences on the selected pads longer."));
        }
        
        else if (minusButton->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate("Decrease Sequence Length. This is used to make the sequences on the selected pads shorter."));
        }
    }
    else if (triggerSettingsButton->getToggleState() == true)
    {
        if (plusButton->isMouseOver(true) || minusButton->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate("These buttons switch between a set of rotary controls above."));
        }
    }
    
    else if (reverseMidiPressureRangeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::midiReversePressureRangeButton));
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
                
                mainComponentRef.setInfoTextBoxText(translate("Sequencer Audio Sample Selector. Use these buttons to select the audio samples for the rows of the sequencer grid for the selected pads. Audio files can also be dragged-and-dropped onto the buttons. Use this button to select the audio sample for row") + " " + String(i+1) + ". " + translate("The current audio file set here is:") + " " + PAD_SETTINGS->getSequencerSamplesAudioFilePath(i).getFileName());
            }
            else if (MULTI_PADS)
            {
                 mainComponentRef.setInfoTextBoxText(translate("Sequencer Audio Sample Selector. Use these buttons to select the audio samples for the rows of the sequencer grid for the selected pads. Audio files can also be dragged-and-dropped onto the buttons. Use this button to select the audio sample for row") + " " + String(i+1) + ".");
            }
            
            break;
        }
    }
    
    
}

void GuiSequencerMode::mouseExit (const MouseEvent &e)
{
    
    if (e.eventComponent == numberOfSequencesSlider)
		setParameterLabelText(String());
	else if (e.eventComponent == relativeTempoSlider)
		setParameterLabelText(String());
	else if (e.eventComponent == noteLengthSlider)
		setParameterLabelText(String());
	else if (e.eventComponent == audioGainSlider)
		setParameterLabelText(String());
	else if (e.eventComponent == audioPanSlider)
		setParameterLabelText(String());
    else if (e.eventComponent == audioAttackSlider)
		setParameterLabelText(String());
    else if (e.eventComponent == audioPolyphonySlider)
		setParameterLabelText(String());
    
    if(e.eventComponent == midiPressureMinRangeSlider || e.eventComponent == midiPressureMaxRangeSlider)
        parameterHoverLabel->setText(String(), dontSendNotification);
     
    
    //remove any text
    mainComponentRef.setInfoTextBoxText (String());
    
//    if (e.eventComponent == numberOfSequencesSlider)
//		setParameterLabelText(String());
//	if (e.eventComponent == relativeTempoSlider)
//		setParameterLabelText(String());
//	if (e.eventComponent == noteLengthSlider)
//		setParameterLabelText(String());
//	if (e.eventComponent == audioGainSlider)
//		setParameterLabelText(String());
//	if (e.eventComponent == audioPanSlider)
//		setParameterLabelText(String());
//    if (e.eventComponent == audioAttackSlider)
//		setParameterLabelText(String());
//    if (e.eventComponent == audioPolyphonySlider)
//		setParameterLabelText(String());
}

