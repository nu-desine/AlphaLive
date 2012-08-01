//
//  MainComponent.cpp
//  AlphaMIDI
//
//  Created by Liam Meredith-Lacey on 14/09/2011.
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

#include "../../Application/Common.h"
#include "MainComponent.h"
#include "../../File and Settings/AppSettings.h"
#include "../Binary Data/PadBinaryData.h"
#include "../Binary Data/BinaryData.h"
#include "../Binary Data/ImageSliderBinaryData.h"
#include "GlobalValues.h"
#include "../../Application/CommandIDs.h"
#include "../../File and Settings/StoredSettings.h"


#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)



//==============================================================================
MainComponent::MainComponent(AlphaLiveEngine &ref, AppDocumentState &ref2, DocumentWindow* owner_)
                        :   alphaLiveEngineRef(ref),
                            appDocumentStateRef(ref2),
                            owner(owner_)
{
    //========command manager stuff==================
	commandManager->registerAllCommandsForTarget (this);
    
    //attach this class to the subject class
    appDocumentStateRef.attach(this);
    
    //Look-and-feel stuff
    blackChrome = new LookAndFeel();
    blackChrome->setDefaultSansSerifTypefaceName("Times New Roman"); //what does this actually do?
	blackChrome->setDefaultLookAndFeel(blackChrome);
    //Font::setFallbackFontName("Times New Roman");
	
	blackChrome->setColour(ComboBox::backgroundColourId, Colours::transparentBlack);
	blackChrome->setColour(ComboBox::textColourId, Colours::silver);
	blackChrome->setColour(ComboBox::buttonColourId, Colours::silver);
	blackChrome->setColour(ComboBox::outlineColourId, Colours::silver);
	blackChrome->setColour(PopupMenu::backgroundColourId, Colours::lightgrey.withAlpha(0.9f));
	blackChrome->setColour(PopupMenu::textColourId, Colours::black);
	blackChrome->setColour(PopupMenu::headerTextColourId, Colours::silver);
	blackChrome->setColour(PopupMenu::highlightedBackgroundColourId, Colours::grey.withAlpha(0.9f));
	blackChrome->setColour(PopupMenu::highlightedTextColourId, Colours::black);
	blackChrome->setColour(TextEditor::backgroundColourId, Colours::transparentBlack);
	blackChrome->setColour(TextEditor::textColourId, Colours::silver);
	blackChrome->setColour(TextEditor::highlightColourId, Colours::white.withAlpha(0.5f));
	blackChrome->setColour(TextEditor::outlineColourId, Colours::silver);
	blackChrome->setColour(TextEditor::focusedOutlineColourId, Colours::white.withAlpha(0.5f));
	blackChrome->setColour(TextButton::buttonColourId, Colours::lightgrey);
    blackChrome->setColour(TextButton::buttonOnColourId, Colours::orange);
	blackChrome->setColour(Slider::rotarySliderFillColourId, Colours::silver);
	blackChrome->setColour(Slider::rotarySliderOutlineColourId, Colours::grey);
    
    
    blackChrome->setUsingNativeAlertWindows(true);
    
    //load Gui Images
    //load binary data into Image
    backgroundImage = ImageFileFormat::loadFrom(BinaryData::main2_png, BinaryData::main2_pngSize);
    
    //--------------------------------------------------------------------------
    addAndMakeVisible(speakerLeft = new GuiSpeaker());
    addAndMakeVisible(speakerRight = new GuiSpeaker());
    addAndMakeVisible(circleBackground = new GuiCircleBackground());
    
    //Mode Gui's
    addAndMakeVisible(guiMidiMode = new GuiMidiMode(*this));
    addAndMakeVisible(guiSamplerMode = new GuiSamplerMode(*this));
    addAndMakeVisible(guiSequencerMode = new GuiSequencerMode(*alphaLiveEngineRef.getModeSequencer(), *this, appDocumentStateRef)); //pass in a ref to modeSequencer instance
    guiSequencerMode->setInterceptsMouseClicks(false, true);
    addAndMakeVisible(guiControllerMode = new GuiControllerMode(*this));
    
    
    //Pad Layout
	addAndMakeVisible(guiPadLayout = new GuiPadLayout(alphaLiveEngineRef, *this));
    guiPadLayout->addMouseListener(this, false);
	guiPadLayout->setInterceptsMouseClicks(false, true);
    
    addAndMakeVisible(padDisplayTextMenu = new ComboBox());
    padDisplayTextMenu->addListener(this);
    padDisplayTextMenu->addMouseListener(this, true);
    padDisplayTextMenu->addItem("Pad Numbers", 1);
    padDisplayTextMenu->addItem("Pad Contents", 2);
    padDisplayTextMenu->setSelectedId(1, true);
    
    //--------------------------------------------------------------------------
    //Mode Buttons
    
    //create off mode button
	Image *offModeImage = new Image(ImageFileFormat::loadFrom(BinaryData::offsymbol_png, BinaryData::offsymbol_pngSize)); 
	addAndMakeVisible(modeOffButton = new ModeButton(offModeImage));
	modeOffButton->addListener(this);
    modeOffButton->setOpaque(false);
	modeOffButton->setRadioGroupId (1234);
    modeOffButton->addMouseListener(this, false);
    
    
    //create sampler mode button
	Image *samplerModeImage = new Image(ImageFileFormat::loadFrom(BinaryData::loopsymbol_png, BinaryData::loopsymbol_pngSize)); 
	addAndMakeVisible(modeSamplerButton = new ModeButton(samplerModeImage));
	modeSamplerButton->addListener(this);
    modeSamplerButton->setOpaque(false);
	modeSamplerButton->setRadioGroupId (1234);
    modeSamplerButton->addMouseListener(this, false);
	
	//create midi mode button
	Image *midiModeImage = new Image(ImageFileFormat::loadFrom(BinaryData::midisymbol_png, BinaryData::midisymbol_pngSize)); 
	addAndMakeVisible(modeMidiButton = new ModeButton(midiModeImage));
	modeMidiButton->addListener(this);
    modeMidiButton->setOpaque(false);
	modeMidiButton->setRadioGroupId (1234);
    modeMidiButton->addMouseListener(this, false);
	
	//create sequencer mode button
	Image *sequencerModeImage = new Image(ImageFileFormat::loadFrom(BinaryData::sequencersymbol_png, BinaryData::sequencersymbol_pngSize)); 
	addAndMakeVisible(modeSequencerButton = new ModeButton(sequencerModeImage));
	modeSequencerButton->addListener(this);
    modeSequencerButton->setOpaque(false);
	modeSequencerButton->setRadioGroupId (1234);
    modeSequencerButton->addMouseListener(this, false);
	
    //createa controller mode button
	Image *controlModeImage = new Image(ImageFileFormat::loadFrom(BinaryData::controlsymbol_png, BinaryData::controlsymbol_pngSize)); 
	addAndMakeVisible(modeControllerButton = new ModeButton(controlModeImage));
	modeControllerButton->addListener(this);
    modeControllerButton->setOpaque(false);
	modeControllerButton->setRadioGroupId (1234);
    modeControllerButton->addMouseListener(this, false);
    
    
    //Global clock bar
    addAndMakeVisible(globalClock = new GuiGlobalClock(*this, alphaLiveEngineRef));
	
    addAndMakeVisible(loadButton = new TextButton("Open"));
    loadButton->setCommandToTrigger(commandManager, CommandIDs::Open, false);
    loadButton->addMouseListener(this, false);
    
    addAndMakeVisible(saveButton = new TextButton("Save"));
    saveButton->setCommandToTrigger(commandManager, CommandIDs::Save, false);
    saveButton->addMouseListener(this, false);
    
    addAndMakeVisible(saveAsButton = new TextButton("Save As"));
    saveAsButton->setCommandToTrigger(commandManager, CommandIDs::SaveAs, false);
    saveAsButton->addMouseListener(this, false);
    
    addAndMakeVisible(presetComponent = new PresetComponent(appDocumentStateRef, *alphaLiveEngineRef.getModeController())); //pass in appDocumentStateRef so that appDocumentStateRef function calls can be made within presetComponent
    presetComponent->addMouseListener(this, true);
    
    addAndMakeVisible(clearPresetsButton = new TextButton("Clear All"));
    clearPresetsButton->setCommandToTrigger(commandManager, CommandIDs::ClearAllPresets, false);
    clearPresetsButton->addMouseListener(this, false);
    
    addAndMakeVisible(cleanUpProjectButton = new TextButton("Clean Up Project"));
    cleanUpProjectButton->setCommandToTrigger(commandManager, CommandIDs::CleanUpProject, false);
    cleanUpProjectButton->addMouseListener(this, true);
    
    //label that displays the currently selected/group of pad
    /*
    addAndMakeVisible(padNumberDisplayLabel = new Label("pad display label", "All Pads Selected"));
    padNumberDisplayLabel->setColour(Label::textColourId, Colours::lightgrey);
    padNumberDisplayLabel->setJustificationType(Justification::topLeft);
    //what font shall we have? Copperplate, Bank Gothic, Calibri, Candara, Consolas, Lucida Console, Courier, BlairMdITC TT?
    Font newFont("Grixel Acme 7 Wide", 25, Font::bold);
    padNumberDisplayLabel->setFont(newFont);
     */
    
    //create gain slider
    addAndMakeVisible(gainSlider = new AlphaImageKnob(2));
    gainSlider->sliderComponent()->setRange(0.0, 2.0);
    gainSlider->sliderComponent()->addListener(this);
	gainSlider->sliderComponent()->setValue(1.0, true);
    gainSlider->sliderComponent()->addMouseListener(this, true);
	
	//create pan slider
    addAndMakeVisible(panSlider = new AlphaImageKnob(2, true));
    panSlider->sliderComponent()->setRange(0.0, 1.0);
    panSlider->sliderComponent()->addListener(this);
    panSlider->sliderComponent()->setValue(0.5, true);
    panSlider->sliderComponent()->addMouseListener(this, false);
    
    //killswitch button
    addAndMakeVisible(killswitchButton = new TextButton());
    killswitchButton->setColour(TextButton::buttonColourId, Colours::blue);
    killswitchButton->setCommandToTrigger(commandManager, CommandIDs::KillSwitch, false);
    killswitchButton->addMouseListener(this, true);
    
    //create tempo slider (IncDecButtons)
    addAndMakeVisible(tempoSlider = new AlphaSlider());
    tempoSlider->setRange(20.0, 360.0, 0.1);
    tempoSlider->addListener(this);
    tempoSlider->setTextBoxStyle(Slider::TextBoxAbove, false, 45, 15);
    tempoSlider->setValue(120.0, false);
    tempoSlider->addMouseListener(this, true);
    
    
    addAndMakeVisible(pressureSensitivityMenu = new ComboBox());
    pressureSensitivityMenu->addListener(this);
    pressureSensitivityMenu->addMouseListener(this, true);
    pressureSensitivityMenu->addItem("Non-Sensitive", 1);
    pressureSensitivityMenu->addItem("Standard", 2);
    pressureSensitivityMenu->addItem("Sensitive", 3);
    pressureSensitivityMenu->setSelectedId(2);
    
    addAndMakeVisible(autoShowSettingsSwitch = new GuiSwitch());
    autoShowSettingsSwitch->addListener(this);
    autoShowSettingsSwitch->setClickingTogglesState(true);
    autoShowSettingsSwitch->setToggleState(false, false);
    autoShowSettingsSwitch->addMouseListener(this, true);
    
    
    addAndMakeVisible(exclusiveModeButton = new TextButton("Exc Mode"));
    exclusiveModeButton->addListener(this);
    exclusiveModeButton->addMouseListener(this, true);
    exclusiveModeButton->setClickingTogglesState(true);
    exclusiveModeButton->setToggleState(0, false);
    
    addAndMakeVisible(exclusiveGroupSlider = new AlphaSlider());
    exclusiveGroupSlider->setRange(1, 24, 1);
    exclusiveGroupSlider->addListener(this);
    exclusiveGroupSlider->setValue(1, false);
    exclusiveGroupSlider->addMouseListener(this, true);
    exclusiveGroupSlider->setVisible(false);
    
    addAndMakeVisible(quantizeModeButton = new TextButton("Quantize"));
    quantizeModeButton->addListener(this);
    quantizeModeButton->addMouseListener(this, true);
    quantizeModeButton->setClickingTogglesState(true);
    quantizeModeButton->setToggleState(0, false);
     
    
    //create pop-up window
    addAndMakeVisible(popUpWindow = new GuiPopUpWindow());
    popUpWindow->setVisible(false);
    
    //addAndMakeVisible(tooltip = new TooltipWindow());
    
    //'rotate pad display' stuff
    addAndMakeVisible(padRotate = new Slider);
	padRotate->setSliderStyle(Slider::Rotary);
	padRotate->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	padRotate->setRotaryParameters(0.0f, 6.2831f, false);
	padRotate->addListener(this);
    padRotate->setOpaque(false);
	padRotate->setRange(0, 360, 1);
    padRotate->addMouseListener(this, false);
	rotateFlag = 0;
	pivotX = 0;
	pivotY = 0;
	offskew = 0;
    
    addChildComponent(aboutComponent = new AboutComponent(*this));
    //aboutComponent->setVisible(true);
    addChildComponent(preferencesComponent = new PreferencesComponent(*this, alphaLiveEngineRef));
    //preferencesComponent->setVisible(true);
    addChildComponent(projectSettingsComponent = new ProjectSettingsComponent(*this, alphaLiveEngineRef, appDocumentStateRef));
    //projectSettingsComponent->setVisible(true);
    
    addAndMakeVisible(infoTextBox = new TextEditor());
    infoTextBox->setMultiLine(true);
    infoTextBox->setReadOnly(true);
    infoTextBox->setColour(TextEditor::backgroundColourId, Colours::black.withAlpha(1.0f));
    infoTextBox->setColour(TextEditor::outlineColourId, Colours::transparentBlack);
    infoTextBox->setCaretVisible(false);
    Font infoFont("Arial", 11, Font::plain);
    infoTextBox->setFont(infoFont);
    infoTextBox->addMouseListener(this, true);
    infoTextBox->setPopupMenuEnabled(false);

    isInfoBoxEnabled = true;
    isClockRunning = false;
    
    //currentlySelectedPad = 99; //'all pads'
    guiMidiMode->setVisible(false);
    guiSamplerMode->setVisible(false);
    guiSequencerMode->setVisible(false);
    guiControllerMode->setVisible(false);
    speakerLeft->setVisible(true);
    speakerRight->setVisible(true);
    circleBackground->setVisible(true);

}


MainComponent::~MainComponent()
{
    
    deleteAllChildren();
    delete blackChrome;
    
    //detach this class from the subject class
    //appDocumentStateRef.detach(this);
    
}

//==============================================================================

void MainComponent::resized()
{
    //Mode Gui's
    guiMidiMode->setBounds(0, 0, getWidth(), getHeight());
    guiSamplerMode->setBounds(0, 0, getWidth(), getHeight());
    guiSequencerMode->setBounds(0, 0, getWidth(), getHeight());
    guiControllerMode->setBounds(0, 0, getWidth(), getHeight());
    
    //'no mode' stuff
    speakerLeft->setBounds(14, 402, 230, 230);
    speakerRight->setBounds(780, 402, 230, 230);
    circleBackground->setBounds(850, 251, 142, 142);
    
    //Centre pad Layout Gui
    guiPadLayout->setBounds(0, 7, getWidth(), getHeight()-30);
    padRotate->setBounds(200, 95, 60, 60);
    
    //Native MainComponent components
	modeOffButton->setBounds(725, 5, 45, 45);
    modeMidiButton->setBounds(767, 5, 65, 65);
    modeSamplerButton->setBounds(829, 5, 65, 65);
	modeSequencerButton->setBounds(891, 5, 65, 65);
	modeControllerButton->setBounds(953, 5, 65, 65);
    
    loadButton->setBounds(5, 5, 100, 20);
    saveButton->setBounds(5, 30, 100, 20);
    saveAsButton->setBounds(5, 55, 100, 20);
    
    presetComponent->setBounds(110, 5, 80, 60);
    clearPresetsButton->setBounds(125, 66, 50, 15);
    
    cleanUpProjectButton->setBounds(200, 5, 100, 20);

    globalClock->setBounds(10, 120, 180, 75);

    infoTextBox->setBounds(0, getHeight()-30, getWidth(), 30);
    
    gainSlider->setBounds(45, 318, 45, 45);
	panSlider->setBounds(109, 318, 45, 45);
    killswitchButton->setBounds(86, 273, 25, 25);
    
    pressureSensitivityMenu->setBounds(800, 130, 100, 20);
    exclusiveModeButton->setBounds(820, 180, 70, 20);
    exclusiveGroupSlider->setBounds(820, 210, 70, 20);
  
    quantizeModeButton->setBounds(800, 100, 100, 20);
    
    autoShowSettingsSwitch->setBounds(323+5, 590+5, 35, 35);
    
    popUpWindow->setBounds(150, 340, 725, 300);

    pivotX = guiPadLayout->getX() + (guiPadLayout->getWidth() * 0.5);
	pivotY = guiPadLayout->getY() + (guiPadLayout->getHeight() * 0.5);
	offskew = guiPadLayout->getY();
    
    
    aboutComponent->setBounds(0, 0, getWidth(), getHeight());
    preferencesComponent->setBounds(0, 0, getWidth(), getHeight());
    projectSettingsComponent->setBounds(0, 0, getWidth(), getHeight());
}



void MainComponent::paint(juce::Graphics &g)
{
    //std::cout << "Paint main component...";
    
    g.setOrigin(0, 0);
	g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight()-30, 0, 0, 1600, 1000);
    
    if (rotateFlag == 1)  
    {
		g.setOrigin(0, 7);
		g.drawImageTransformed(rotatedPadsImage, (AffineTransform::rotation ((float) (padRotate->getValue() / (180.0 / double_Pi)), getWidth()*0.5, (getHeight()-30/*-30 as the image doesn't go all the way to the bottom of the component, due to the help box now being there*/)*0.5)), false);
		g.setOrigin(0, 0);
    
	}
    
}


//observers update function, called everytime settings are loaded from file or a preset to update the GUI to the correct display
bool MainComponent::update(const Subject& theChangedSubject)
{
    if (&theChangedSubject == &appDocumentStateRef)
    {
        //update settings display
        if (appDocumentStateRef.getGuiUpdateFlag() == 0)
        {
            //this call effectively updates the GUI display
            setCurrentlySelectedPad(selectedPads);
            
            //update GUI things (GUI of global settings) which aren't updated by setCurrentlySelectedPad
            //these things could be put in setCurrentlySelectedPad but they'll be updated everytime a pad is selected which would be inefficent
            //there is still some inefficiency in that these things will be updated everytime a sequence is loaded... sort this out?!!
            gainSlider->sliderComponent()->setValue(AppSettings::Instance()->getGlobalGain(), false); 
            panSlider->sliderComponent()->setValue(AppSettings::Instance()->getGlobalPan(), false);
            tempoSlider->setValue(AppSettings::Instance()->getGlobalTempo(), false);
            
            //set the mode colour ring of each pad
            for (int i = 0; i <= 47; i++)
                guiPadLayout->modeChange(i, AppSettings::Instance()->padSettings[i]->getMode());
            
            globalClock->updateDisplay();
            projectSettingsComponent->updateDisplay();
            
            //change the way in which the mode is displayed on the pads so the loading changes the values
            //PAD DISPLAY ROTATION VALUE LOADED IN HERE
        }
        
        
        //update presetComponent display
        else if (appDocumentStateRef.getGuiUpdateFlag() == 1)
        {
            presetComponent->setSlotStatus(appDocumentStateRef.getPresetToUpdate(), appDocumentStateRef.getPresetStatus());
        }
    }
    
    return true;
}


void MainComponent::sliderValueChanged (Slider *slider)
{
    if (slider == gainSlider->sliderComponent())
    {
        AppSettings::Instance()->setGlobalGain(gainSlider->sliderComponent()->getValue());
    }
    
    else if (slider == panSlider->sliderComponent())
    {
        AppSettings::Instance()->setGlobalPan(panSlider->sliderComponent()->getValue());
        
    }
    
    /*
    if (slider == tempoSlider)
    {
        AppSettings::Instance()->setGlobalTempo(tempoSlider->getValue());
        
    }
     */
    
    else if (slider == padRotate)
	{
		guiPadLayout->setVisible(false);
		rotateFlag = 1;
		repaint();
	}
    
    else if (slider == exclusiveGroupSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setExclusiveGroup(slider->getValue());
        }
    } 
}



void MainComponent::sliderDragEnded (Slider *slider)
{
	
	if (slider == padRotate)
	{
		guiPadLayout->setVisible(true);
		rotateFlag = 0;
		
		guiPadLayout->setTransform (AffineTransform::rotation ((float) (padRotate->getValue() / (180.0 / double_Pi)),
                                                       pivotX, pivotY));
		guiPadLayout->rotated(padRotate->getValue());
		repaint();
	}
}


void MainComponent::sliderDragStarted (Slider *slider)
{
	if (slider == padRotate)
	{
		rotatedPadsImage = guiPadLayout->snapshot();
	}
    
}



void MainComponent::buttonClicked(Button *button)
{
    //==============================================================================
    //MODE BUTTONS
    //==============================================================================
    
    //this function sets the selected mode (based on the button pressed)
    //to the currently selected pad (or set of pads)
    int buttonIndex;
    
    //get the selected button and set the right mode to screen
    if (button == modeOffButton)
    {
        setToOffMode();
        buttonIndex = 0;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->modeChange(padNum, buttonIndex);
        }
    }
    
    else if (button == modeMidiButton)
    {
        setToMidiMode();
        buttonIndex = 1;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->modeChange(padNum, buttonIndex);
        }
    } 
    
    else if (button == modeSamplerButton)
    {
        setToSamplerMode();
        buttonIndex = 2;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->modeChange(padNum, buttonIndex);
        }
    }
    
    else if (button == modeSequencerButton)
    {
        setToSequencerMode();
        buttonIndex = 3;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->modeChange(padNum, buttonIndex);
        }
        
    }
    
    else if (button == modeControllerButton)
    {
        setToControllerMode();
        buttonIndex = 4;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->modeChange(padNum, buttonIndex);
        }
    }
    
    
    //==============================================================================
    
    else if (button == exclusiveModeButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setExclusiveMode(button->getToggleState());
        }
        
        if (button->getToggleState() == true)
            exclusiveGroupSlider->setVisible(true);
        else
           exclusiveGroupSlider->setVisible(false); 
    }
    
    
    else if (button == quantizeModeButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setQuantizeMode(button->getToggleState());
        }
        
    }
    
    //==============================================================================
    //===PRESETS, LOADING & SAVING - now handled by the command manager below!======
    //==============================================================================
    
    //=================Now handled by the command manager======================
    
   
    else if (button == autoShowSettingsSwitch)
    {
        guiPadLayout->setShouldDisplaySettings(autoShowSettingsSwitch->getToggleState());
    }
    
}


//this function is called every time a user selects a pad on the GUI
void MainComponent::setCurrentlySelectedPad(Array <int> selectedPads_)
{
    /*
    for (int i = 0; i < selectedPads.size(); i++)
    {
        std::cout << "Pad " << selectedPads[i] << " is selected!\n";
    }
    std::cout << std::endl;
     */
    
    selectedPads = selectedPads_;
    
    //pass in the currently selected pads to the mode GUIs for them to use
    //in order to display the right settings from appSettings
    AppSettings::Instance()->setCurrentlySelectedPad(selectedPads);
    guiMidiMode->setCurrentlySelectedPad(selectedPads);
    guiSamplerMode->setCurrentlySelectedPad(selectedPads);
    guiSequencerMode->setCurrentlySelectedPad(selectedPads);
    guiControllerMode->setCurrentlySelectedPad(selectedPads);
    //presetComponent->setCurrentlySelectedPad(selectedPads);
    
    
    //==============================================================================
    //if current pad selected is a single pad
    if (SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getMode() == 0) //off mode
        {
            setToOffMode();
            modeOffButton->setToggleState(true, false);
        }
        
        if (PAD_SETTINGS->getMode() == 1) //midi mode
        {
            setToMidiMode();
            modeMidiButton->setToggleState(true, false);
        }
        
        if (PAD_SETTINGS->getMode() == 2) //sampler mode
        {
            setToSamplerMode();
            modeSamplerButton->setToggleState(true, false);
        }
    
        if (PAD_SETTINGS->getMode() == 3) //sequencer mode
        {
            setToSequencerMode();
            modeSequencerButton->setToggleState(true, false);
        }
        if (PAD_SETTINGS->getMode() == 4) //controller mode
        {
            setToControllerMode();
            modeControllerButton->setToggleState(true, false);
        }
        
        //set other things
        //padNumberDisplayLabel->setText("Pad " + String(currentlySelectedPad+1) + " Selected", false);
        pressureSensitivityMenu->setSelectedId(PAD_SETTINGS->getPressureSensitivityMode(), true);
        exclusiveModeButton->setToggleState(PAD_SETTINGS->getExclusiveMode(), false);
        exclusiveGroupSlider->setValue(PAD_SETTINGS->getExclusiveGroup(), false);
        quantizeModeButton->setToggleState(PAD_SETTINGS->getQuantizeMode(), false);
        
        if (exclusiveModeButton->getToggleState() == true)
            exclusiveGroupSlider->setVisible(true);
        else
            exclusiveGroupSlider->setVisible(false);
        
        
    }
    else if (MULTI_PADS)
    {
        //set to a default setting
        setToOffMode();
        modeOffButton->setToggleState(true, false);

        //set other things
        //padNumberDisplayLabel->setText("All Pads Selected", false);
        pressureSensitivityMenu->setSelectedId(2, true);
        
        exclusiveModeButton->setToggleState(0, false);
        exclusiveGroupSlider->setValue(1, false);
        exclusiveGroupSlider->setVisible(false);
        quantizeModeButton->setToggleState(0, false);
        
    }
    

    commandManager->commandStatusChanged(); //so that if a single pad is selected, the copy and paste settings
                                            //command is enabled
            
     
}

void MainComponent::comboBoxChanged (ComboBox *comboBox)
{
    
    /*
    if (comboBox == padDisplayTextMenu)
    {
        AppSettings::Instance()->setPadDisplayTextMode(padDisplayTextMenu->getSelectedId());
    }
     */
    
    if (comboBox == pressureSensitivityMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPressureSensitivityMode(pressureSensitivityMenu->getSelectedId());
        }
    }

}


void MainComponent::buttonStateChanged (Button* button)
{ 
    
	
}




void MainComponent::setToOffMode()
{
    guiMidiMode->setVisible(false);
    guiSamplerMode->setVisible(false);
    guiSequencerMode->setVisible(false);
    guiControllerMode->setVisible(false);
    speakerLeft->setVisible(true);
    speakerRight->setVisible(true);
    circleBackground->setVisible(true);
    
    
}
void MainComponent::setToMidiMode()
{
    guiMidiMode->setVisible(true);
    guiSamplerMode->setVisible(false);
    guiSequencerMode->setVisible(false);
    guiControllerMode->setVisible(false);
    speakerLeft->setVisible(false);
    speakerRight->setVisible(false);
    circleBackground->setVisible(false);
    
    //update the display of the midi mode so it shows the
    //currently selected pad's settings, and makes any uneeded
    //components invisble or dissabled
    guiMidiMode->updateDisplay();

    
}
void MainComponent::setToSamplerMode()
{
    guiMidiMode->setVisible(false);
    guiSamplerMode->setVisible(true);
    guiSequencerMode->setVisible(false);
    guiControllerMode->setVisible(false);
    speakerLeft->setVisible(false);
    speakerRight->setVisible(false);
    circleBackground->setVisible(false);
    
    //update the display of the sampler mode so it shows the
    //currently selected pad's settings, and makes any unneeded
    //components invisble or dissabled
    guiSamplerMode->updateDisplay();

    
}
void MainComponent::setToSequencerMode()
{
    guiMidiMode->setVisible(false);
    guiSamplerMode->setVisible(false);
    guiSequencerMode->setVisible(true);
    guiControllerMode->setVisible(false);
    speakerLeft->setVisible(false);
    speakerRight->setVisible(false);
    circleBackground->setVisible(false);
    
    guiSequencerMode->updateDisplay();
    
}

void MainComponent::setToControllerMode()
{
    guiMidiMode->setVisible(false);
    guiSamplerMode->setVisible(false);
    guiSequencerMode->setVisible(false);
    guiControllerMode->setVisible(true);
    speakerLeft->setVisible(false);
    speakerRight->setVisible(false);
    circleBackground->setVisible(false);
    
    guiControllerMode->updateDisplay();
}


AlphaLiveEngine& MainComponent::getAlphaLiveEngineRef()
{
    return alphaLiveEngineRef;
}


void MainComponent::setInfoTextBoxText (String text)
{
    if (isInfoBoxEnabled == true)
    {
        infoTextBox->clear();
        infoTextBox->setText(text);
    }
    
}

void MainComponent::setIsClockRunning (bool value)
{
    isClockRunning = value;
    commandManager->commandStatusChanged();
}

void MainComponent::mouseDown (const MouseEvent &e)
{
    /*
    if (infoTextBox->isMouseOver(true))
    {
        //right click to bring up the popup menu to allow the help box to be disabled/enabled
        if (e.mods.isPopupMenu() == true)
        {
            
            PopupMenu menu;
            
            if (isInfoBoxEnabled == true)
                menu.addItem(1, "Disable");
            else
                menu.addItem(2, "Enable");
            
            const int result = menu.show();
            
            if (result == 0)
            {
                // user dismissed the menu without picking anything
            } 
            else if (result == 1)
            {
                setInfoTextBoxText ("Help Box currently disabled. Right-click here to enable it.");
                isInfoBoxEnabled = false; // disable
            }
            else if (result == 2)
            {
                isInfoBoxEnabled = true; // enable
                setInfoTextBoxText ("Help Text Box. Hover the mouse over a control to view here a description of what the control does. Right-click here to dissable it.");
            }
        }
    }
     */
}

//called whenever a the mouse enters a component which has a MouseListener attached to it
//any components checked in here must have a mouseListener added to it (in the initialiser)
void MainComponent::mouseEnter (const MouseEvent &e)
{
    //std::cout << "Mouse entered a component... ";
    /*
     there are two ways to get the relative component that the mouse is over:
     - call eventComponent on MouseEvent e. This finds the component that the MouseEvent occured on
     - call isMouseOver(true) on the relative component. This finds whether the mouse is currently over the said component.
      
     I originally started using the first method, however for components such as IncDecButtons and custom components which have nested child components (e.g. IncDecButtons Slider includes a TextEditor and TextButtons) the second method needed to be used as the child components are recognised as seperate components and using the first method results in the infoTextBox not displaying any text when the mouse is over these child components. The isMouseOver() function needs the arguement 'true' so children are included, and the addMouseListener() call on the relative components needs to have the 'wantsEventsForAllNestedChildComponents' argument set to true so th child components are included.
     
     From now on I will use the second method as this will work for any component, whereas the first one doesn't.
     */
    
    //STILL IN A CHECK THAT THE NEWLY ENTERED COMPONENT ISN'T A CHILD OF THE PREV COMPONENT SO WE DONT UPDATE TEXT WHICH WILL BE THE SAME?
    
    if (e.eventComponent == modeOffButton)
    {
        setInfoTextBoxText ("Off-Mode Button. Click this button to disable/mute the selected pad/pads.");
    }
    else if (e.eventComponent == modeMidiButton)
    {
        setInfoTextBoxText ("MIDI-Mode Button. Click this button to put MIDI functionality onto the selected pad/pads.This enables the AlphaSphere to act as a virtual MIDI device.");
    }
    else if (e.eventComponent == modeSamplerButton)
    {
        setInfoTextBoxText ("Sampler-Mode Button. Click this button to put Sampler functionality onto the selected pad/pads. This enables audio files to be triggered, looped, and manipulated.");
    }
    else if (e.eventComponent == modeSequencerButton)
    {
        setInfoTextBoxText ("Sequencer-Mode Button. Click this button to put Sequencer functionality onto the selected pad/pads. This enables dynamic sequences of midi data or audio samples to be created.");
    }
    else if (e.eventComponent == modeControllerButton)
    {
        setInfoTextBoxText ("Controller-Mode Button. Click this button to put Controller functionality onto the selected pad/pads. This enables certain components within the application to be controlled by pads.");
    }
    else if (e.eventComponent == loadButton)
    {
        setInfoTextBoxText ("Load Performance. Allows a set of presets to be loaded into the application.");
    }
    else if (e.eventComponent == saveButton)
    {
        setInfoTextBoxText ("Save Performance. Allows a set of presets to be saved to the computer.");
    }
    else if (e.eventComponent == saveAsButton)
    {
        setInfoTextBoxText ("Save Performance As. Allows a set of presets to be saved to the computer with a given name.");
    }
    else if (e.eventComponent == cleanUpProjectButton)
    {
        setInfoTextBoxText ("Clean-up project. Deletes any unneeded audio files from the current project's 'Audio Files' directory. Use this command regularly to prevent an excessive build-up of redundant data.");
    }
        else if (presetComponent->isMouseOver(true))
    {
        setInfoTextBoxText ("Presets. Allows application settings to be saved into indvidual presets that can then be saved to disk as a group. Shift-Click a slot to save a preset to the object, or click to load a preset from the object. Right-click to save and load single presets.");
    }
    else if (e.eventComponent == clearPresetsButton)
    {
        setInfoTextBoxText ("Clear Presets. Allows all preset slots in the preset object to be cleared.");
    }
    else if (tempoSlider->isMouseOver(true)==true)
    {
        setInfoTextBoxText ("Global Tempo Selector. Sets and displays the tempo which controls the playback speed of sequences.");
    }
    else if (gainSlider->isMouseOver(true)==true)
    {
        setInfoTextBoxText ("Global Gain Knob. Sets an displays the gain/volume of the overall output audio signal.");
    }
    else if (panSlider->isMouseOver(true)==true)
    {
        setInfoTextBoxText ("Global Pan Knob. Sets and displays the panning/stereo positioning of the overall output audio signal.");
    }
    else if (e.eventComponent == padRotate)
    {
        setInfoTextBoxText ("Pad Display Rotation Knob. Allows the central pad display to be rotated.");
    }
    
    else if (e.eventComponent == infoTextBox)
    {
        setInfoTextBoxText ("Help Text Box. Hover the mouse over a control to view here a description of what the control does."/* Right-click here to dissable it */);
    }
    else if (pressureSensitivityMenu->isMouseOver(true)==true)
    {
        setInfoTextBoxText("Pad Pressure Sensitivity Mode. Sets and displays the sensitivity of the selected pad/pads in terms of mapping the pressure to paramaters within the application.");
    }
    else if(killswitchButton->isMouseOver(true))
    {
        setInfoTextBoxText("Kill Switch. Instantly stops the clock and any playing pads.");
    }
    else if (autoShowSettingsSwitch->isMouseOver(true))
    {
        setInfoTextBoxText("'Show Pad Settings When Pressed' Option. If this switch is set to 'on' the pad settings will  beautomatically displayed when a pad is pressed. It is recommended that you only use this feature for editing projects and not playing/performing.");
    }
    
    
    
}

//called whenever a the mouse exits a component which has a MouseListener attached to it
void MainComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    setInfoTextBoxText (String::empty);
}


//=========================command manager stuff=================================
ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
    //return 0;
	return findFirstTargetParentComponent();
}

void MainComponent::getAllCommands (Array <CommandID>& commands)
{
	const CommandID ids[] = 
    {	
        CommandIDs::About,
        CommandIDs::Preferences,
        CommandIDs::ProjectSettings,
        CommandIDs::SavePreset,
        CommandIDs::LoadPreset,
        CommandIDs::DisableHelpBox,
        CommandIDs::CopyPadSettings,
        CommandIDs::PastePadSettings,
        CommandIDs::ClearPreset,
        CommandIDs::ClearAllPresets,
        CommandIDs::KillSwitch,
        CommandIDs::StartStopClock
    };
	
	commands.addArray (ids, numElementsInArray (ids));
}


void MainComponent::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
	const int cmd = ModifierKeys::commandModifier;
	//const int shift = ModifierKeys::shiftModifier;
    const int alt = ModifierKeys::altModifier;
    
    //within 'setInfo()' below, the name sets the String that appears in the Menu bar,
    //and the description sets what would appear in the tooltip if the command is set to a button
    //and the tooltip parameter is set to 'true'
	
    if(commandID == CommandIDs::About)
    {
		result.setInfo (String("About AlphaLive"),
						"Opens the application 'about' view.",
						CommandCategories::OtherCommands, 0);
	}
	else if(commandID == CommandIDs::Preferences)
    {
		result.setInfo ("Preferences...",
						"Opens the application preferences view.",
						CommandCategories::OtherCommands, 0);
		result.defaultKeypresses.add (KeyPress (',', cmd, 0));
	}
    
    else if (commandID == CommandIDs::ProjectSettings)
    {
        result.setInfo ("Project Settings...",
						"Opens the Project Settings view.",
						CommandCategories::FileCommands, 0);
        result.defaultKeypresses.add (KeyPress ('p', cmd|alt, 0));
    }
    else if (commandID == CommandIDs::SavePreset)
    {
        result.setInfo ("Export Preset...",
						"Saves the currently selected preset settings to file.",
						CommandCategories::FileCommands, 0);
    }
    else if(commandID == CommandIDs::LoadPreset)
	{
		result.setInfo ("Import Preset...",
						"Loads an individual preset settings into the currently selected preset.",
						CommandCategories::FileCommands, 0);
	}
    else if(commandID == CommandIDs::DisableHelpBox)
	{
		result.setInfo ("Disable Help Box",
						"Disables the help box to reduce CPU usage.",
						CommandCategories::OptionCommands, 0);
        result.setTicked(! isInfoBoxEnabled);
	}
    
    else if(commandID == CommandIDs::CopyPadSettings)
	{
		result.setInfo ("Copy Pad Settings",
						"Copy's the settings of the currently selected pad.",
						CommandCategories::FileCommands, 0);
		result.defaultKeypresses.add (KeyPress ('c', cmd, 0));
        result.setActive(SINGLE_PAD);
	}
    else if(commandID == CommandIDs::PastePadSettings)
	{
		result.setInfo ("Paste Pad Settings",
						"Paste's settings to the currently selected pads.",
						CommandCategories::FileCommands, 0);
		result.defaultKeypresses.add (KeyPress ('v', cmd, 0));
        result.setActive(SINGLE_PAD);
	}
    
    else if (commandID == CommandIDs::ClearPreset)
    {
		result.setInfo ("Clear Preset",
						"Clears and removes the currently selected preset.",
						CommandCategories::EditCommands, 0);
		
	}
    else if (commandID == CommandIDs::ClearAllPresets)
    {
		result.setInfo ("Clear All Presets",
						"Clears and removes all presets.",
						CommandCategories::EditCommands, 0);
	}
    
    else if (commandID == CommandIDs::KillSwitch)
    {
		result.setInfo ("Killswitch",
						"Kills all playing sound and any hanging MIDI notes.",
						CommandCategories::ControlCommands, 0);
        result.defaultKeypresses.add (KeyPress ('k', cmd, 0));
	}
    
    else if (commandID == CommandIDs::StartStopClock)
    {
        if (isClockRunning == false)
        {
            result.setInfo ("Start Clock",
                            "Starts the global clock.",
                            CommandCategories::ControlCommands, 0);
        }
        else
        {
            result.setInfo ("Stop Clock",
                            "Stops the global clock.",
                            CommandCategories::ControlCommands, 0);
        }
        
        result.addDefaultKeypress(KeyPress::spaceKey, NULL);
        
        
	}
    
	
}

bool MainComponent::perform (const InvocationInfo& info)
{
	if(info.commandID == CommandIDs::About)
	{
        projectSettingsComponent->setVisible(false);
		preferencesComponent->setVisible(false);
		aboutComponent->setVisible(true);
        aboutComponent->grabKeyboardFocus();    //so that the ESC to close works without having to
                                                //click on the component first
        
	}
    
	else if(info.commandID == CommandIDs::Preferences)
	{
        aboutComponent->setVisible(false);
        projectSettingsComponent->setVisible(false);
		preferencesComponent->setVisible(true);
        preferencesComponent->grabKeyboardFocus();
	}
    
    else if(info.commandID == CommandIDs::ProjectSettings)
	{
        aboutComponent->setVisible(false);
		preferencesComponent->setVisible(false);
		projectSettingsComponent->setVisible(true);
        projectSettingsComponent->grabKeyboardFocus();
	}
    
    else if(info.commandID == CommandIDs::SavePreset)
	{
		presetComponent->getSelectedPresetSlot()->savePreset();
	}
    
    else if(info.commandID == CommandIDs::LoadPreset)
	{
		presetComponent->getSelectedPresetSlot()->loadPreset();
	}
    
    else if(info.commandID == CommandIDs::DisableHelpBox)
	{
        if (isInfoBoxEnabled == true)
            isInfoBoxEnabled = false;
        else
            isInfoBoxEnabled = true;
	}
    
    else if(info.commandID == CommandIDs::CopyPadSettings) 
	{
		guiPadLayout->copyPadSettings();
	}
    
    else if(info.commandID == CommandIDs::PastePadSettings) 
	{
		guiPadLayout->pastePadSettings();
	}
    
    else if(info.commandID == CommandIDs::ClearPreset)
	{
		presetComponent->getSelectedPresetSlot()->clearPreset();
	}
    
    else if(info.commandID == CommandIDs::ClearAllPresets)
	{
		bool userSelection;
        userSelection = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Are you sure?", "You cannot undo this command", "Ok", "Cancel");
        
        if (userSelection == true)
        {
            //reset/clear xml objects
            //DONT CLEAR PRESET 0 AS THIS ALWAYS NEED TO BE FILLED
            for (int i = 1; i <= 9; i++)
            {
                appDocumentStateRef.clearPreset(i);
            }
            
            //reset/clear gui
            presetComponent->clearAll();
        }
	}
    
    else if(info.commandID == CommandIDs::KillSwitch)
	{
		alphaLiveEngineRef.killAll();
        globalClock->toggleTransportButtonOff();
	}
    
    else if(info.commandID == CommandIDs::StartStopClock)
	{
		globalClock->triggerTransportButton();
	}
    
	else
		return false;
	
	return true;
	
}


ApplicationCommandManager* MainComponent::getCommandManager()
{
    return commandManager;
}

