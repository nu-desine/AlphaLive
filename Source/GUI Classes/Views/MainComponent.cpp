//
//  MainComponent.cpp
//  AlphaLive
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
#include "GlobalValues.h"
#include "../../Application/CommandIDs.h"
#include "../../File and Settings/StoredSettings.h"
#include "../Binary Data/MainBinaryData.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)

//==============================================================================
MainComponent::MainComponent(AlphaLiveEngine &ref, AppDocumentState &ref2, DocumentWindow* owner_)
                        :   Thread("Info Box Updater"),
                            alphaLiveEngineRef(ref),
                            appDocumentStateRef(ref2),
                            owner(owner_)
                            
{
    appDir = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() + File::getSeparatorString();
    infoBoxText = String();
    
    //language/localisation stuff
    setLocalisation();
    
    //========command manager stuff==================
	commandManager->registerAllCommandsForTarget (this);
    
    //attach this class to the subject class
    appDocumentStateRef.attach(this);
    
    //allow AlphaLive Engine to communicate with this class (used by the elite controls)
    alphaLiveEngineRef.setMainComponent(this);
    
    //Look-and-feel stuff
    LookAndFeel::setDefaultLookAndFeel (&alphaLiveLookAndFeel);
    alphaLiveLookAndFeel.setUsingNativeAlertWindows(true);
    
    //Mode Gui's
    addChildComponent(guiMidiMode = new GuiMidiMode(*this));
    guiMidiMode->addMouseListener(this, true);
    
    addChildComponent(guiSamplerMode = new GuiSamplerMode(*this));
    guiSamplerMode->addMouseListener(this, true);
    
    addChildComponent(guiSequencerMode = new GuiSequencerMode(*alphaLiveEngineRef.getModeSequencer(), *this, appDocumentStateRef)); 
    guiSequencerMode->addMouseListener(this, true);
    
    addChildComponent(guiControllerMode = new GuiControllerMode(*this));
    guiControllerMode->addMouseListener(this, true);
    
    addChildComponent(guiGlobalPadSettings = new GuiGlobalPadSettings(*this));
    guiGlobalPadSettings->addMouseListener(this, true);
    
    addChildComponent(eliteControlsSettings = new GuiEliteControlsSettings(*this));
    eliteControlsSettings->addMouseListener(this, true);
    
    //Pad Layout
	addAndMakeVisible(guiPadLayout = new GuiPadLayout(alphaLiveEngineRef, *this));
    guiPadLayout->addMouseListener(this, false);
	guiPadLayout->setInterceptsMouseClicks(false, true);
    
    
    //--------------------------------------------------------------------------
    //Mode Buttons
    //create off mode button
	Image *offModeImage = new Image(ImageCache::getFromMemory(MainBinaryData::offsymbol_png, MainBinaryData::offsymbol_pngSize)); 
	addAndMakeVisible(modeOffButton = new ModeButton(offModeImage));
	modeOffButton->addListener(this);
    modeOffButton->setOpaque(false);
	modeOffButton->setRadioGroupId (1234, dontSendNotification);
    modeOffButton->addMouseListener(this, false);
	
	//create global settings button
	Image *globalSettingsImage = new Image(ImageCache::getFromMemory(MainBinaryData::padsettingssymbol_png, MainBinaryData::padsettingssymbol_pngSize)); 
	addAndMakeVisible(globalSettingsButton = new ModeButton(globalSettingsImage));
	globalSettingsButton->addListener(this);
    globalSettingsButton->setOpaque(false);
    globalSettingsButton->addMouseListener(this, false);
    
    
    //create looper mode button
	Image *looperModeImage = new Image(ImageCache::getFromMemory(MainBinaryData::loopsymbol_png, MainBinaryData::loopsymbol_pngSize)); 
	addAndMakeVisible(modeSamplerButton = new ModeButton(looperModeImage));
	modeSamplerButton->addListener(this);
    modeSamplerButton->setOpaque(false);
	modeSamplerButton->setRadioGroupId (1234, dontSendNotification);
    modeSamplerButton->addMouseListener(this, false);
	
	//create midi mode button
	Image *midiModeImage = new Image(ImageCache::getFromMemory(MainBinaryData::midisymbol_png, MainBinaryData::midisymbol_pngSize)); 
	addAndMakeVisible(modeMidiButton = new ModeButton(midiModeImage));
	modeMidiButton->addListener(this);
    modeMidiButton->setOpaque(false);
	modeMidiButton->setRadioGroupId (1234, dontSendNotification);
    modeMidiButton->addMouseListener(this, false);
	
	//create sequencer mode button
	Image *sequencerModeImage = new Image(ImageCache::getFromMemory(MainBinaryData::sequenceicon_png, MainBinaryData::sequenceicon_pngSize)); 
	addAndMakeVisible(modeSequencerButton = new ModeButton(sequencerModeImage));
	modeSequencerButton->addListener(this);
    modeSequencerButton->setOpaque(false);
	modeSequencerButton->setRadioGroupId (1234, dontSendNotification);
    modeSequencerButton->addMouseListener(this, false);
	
    //createa controller mode button
	Image *controllerModeImage = new Image(ImageCache::getFromMemory(MainBinaryData::controlsymbol_png, MainBinaryData::controlsymbol_pngSize)); 
	addAndMakeVisible(modeControllerButton = new ModeButton(controllerModeImage));
	modeControllerButton->addListener(this);
    modeControllerButton->setOpaque(false);
	modeControllerButton->setRadioGroupId (1234, dontSendNotification);
    modeControllerButton->addMouseListener(this, false);
    
    
    //Global clock bar
    addAndMakeVisible(globalClock = new GuiGlobalClock(*this, alphaLiveEngineRef));
    globalClock->setInterceptsMouseClicks(false, true);
    globalClock->addMouseListener(this, true);
	
    //open/save buttons
	Image *openImage = new Image(ImageCache::getFromMemory(MainBinaryData::loadsymbol_png, MainBinaryData::loadsymbol_pngSize)); 
    addAndMakeVisible(openButton = new ModeButton(openImage));
	openButton->setClickingTogglesState(false);
    openButton->setCommandToTrigger(commandManager, CommandIDs::Open, false);
    openButton->addMouseListener(this, false);
    
	Image *saveImage = new Image(ImageCache::getFromMemory(MainBinaryData::savesymbol_png, MainBinaryData::savesymbol_pngSize)); 
    addAndMakeVisible(saveButton = new ModeButton(saveImage));
	saveButton->setClickingTogglesState(false);
    saveButton->setCommandToTrigger(commandManager, CommandIDs::Save, false);
    saveButton->addMouseListener(this, false);
	
	//create pan slider
    addAndMakeVisible(panSlider = new AlphaRotarySlider((225 * (M_PI / 180)), (495 * (M_PI / 180)), 65));
	panSlider->setRotaryParameters((225 * (M_PI / 180)), (495 * (M_PI / 180)),true);
    panSlider->setRange(0.0, 1.0, 0.001);
    panSlider->addListener(this);
    panSlider->setValue(0.5, dontSendNotification);
    panSlider->addMouseListener(this, false);
    
    //create gain slider
    //addAndMakeVisible(gainSlider = new AlphaRotarySlider((225 * (M_PI / 180)), (530 * (M_PI / 180)), 81));
	//gainSlider->setRotaryParameters((225 * (M_PI / 180)), (530 * (M_PI / 180)),true);
    addAndMakeVisible(gainSlider = new AlphaRotarySlider((225 * (M_PI / 180)), (495 * (M_PI / 180)), 81));
	gainSlider->setRotaryParameters((225 * (M_PI / 180)), (495 * (M_PI / 180)),true);
    gainSlider->setRange(0.0, 2.0, 0.001);
    gainSlider->addListener(this);
	gainSlider->setValue(1.0, dontSendNotification);
    gainSlider->addMouseListener(this, true);
    
    //gain and pan label
    addAndMakeVisible(gainPanValueLabel = new Label("value label", String()));
    gainPanValueLabel->setJustificationType(Justification::horizontallyCentred);
    gainPanValueLabel->setFont(Font(12 + AlphaTheme::getInstance()->fontSizeAddition));
    gainPanValueLabel->addMouseListener(this, true);
    
    //piano
    addAndMakeVisible(midiPiano = new GuiPiano());
    midiPiano->addMouseListener(this, true);
    
    //toolbox
    addAndMakeVisible(toolbox = new Toolbox(*this));
    toolbox->addMouseListener(this, true);
    
    //elite controls display
    addChildComponent(eliteControls = new EliteControlsComponent(*this));
    eliteControls->addMouseListener(this, true);
	eliteControls->setInterceptsMouseClicks(false, true);
    
    //device status component
    addAndMakeVisible(deviceStatusComponent = new DeviceStatusComponent(alphaLiveEngineRef.getDeviceStatus()));
    deviceStatusComponent->addMouseListener(this, false);
    
    //scene component
    addAndMakeVisible(sceneComponent = new SceneComponent(appDocumentStateRef, *alphaLiveEngineRef.getModeController())); //pass in appDocumentStateRef so that appDocumentStateRef function calls can be made within sceneComponent
    sceneComponent->addMouseListener(this, true);
    sceneComponent->setInterceptsMouseClicks(false, true);
    
    //pop up views
    addChildComponent(aboutComponent = new AboutComponent(*this));
    aboutComponent->setAlpha(0.975f);
    addChildComponent(preferencesComponent = new PreferencesComponent(*this, alphaLiveEngineRef));
    preferencesComponent->setAlpha(0.975f);
    addChildComponent(projectSettingsComponent = new ProjectSettingsComponent(*this, alphaLiveEngineRef, appDocumentStateRef));
    projectSettingsComponent->setAlpha(0.975f);
    addChildComponent (softwareUpdateComponent = new SoftwareUpdateComponent (*this));
    
    //info box
    addAndMakeVisible(infoTextBox = new TextEditor());
    infoTextBox->setMultiLine(true);
    infoTextBox->setReadOnly(true);
    infoTextBox->setColour(TextEditor::outlineColourId, Colours::transparentBlack);
    infoTextBox->setCaretVisible(false);
    Font infoFont(infoBoxTextSize, Font::plain);
    infoTextBox->setFont(infoFont);
    infoTextBox->addMouseListener(this, true);
    infoTextBox->setPopupMenuEnabled(false);
    

    isInfoBoxEnabled = true;
    isClockRunning = false;

    noPadsSelected = 1;
	noModeSelected = 1;
    eliteControlSelected = 0;
    selectedEliteControl = 0;
    
    modeOffButton->setVisible(false);
    modeMidiButton->setVisible(false);
    modeSamplerButton->setVisible(false);
    modeSequencerButton->setVisible(false);
    modeControllerButton->setVisible(false);
    globalSettingsButton->setVisible(false);
    toolbox->setVisible(false); //or maybe it links to projects directory?
    midiPiano->setActive(false);
    
    if (StoredSettings::getInstance()->deviceType == 2)
        eliteControls->setVisible(true);
    else
        eliteControls->setVisible(false);
    
    mouseOverComponent = nullptr;
    
}


MainComponent::~MainComponent()
{
    if (isThreadRunning())
        stopThread(500);
    
    deleteAllChildren();
    
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
	guiGlobalPadSettings->setBounds(0, 0, getWidth(), getHeight());
	
    eliteControls->setBounds(44, 537, 100, 80);
    eliteControlsSettings->setBounds(0, 0, getWidth(), getHeight());
    
	midiPiano->setBounds(0, 0, 660, 685);
    
    toolbox->setBounds(760, 17, 247, 107);
    
    //Centre pad Layout Gui
    guiPadLayout->setBounds(0, 0, getWidth(), getHeight()-30);
    //padRotate->setBounds(244, 568, 60, 60);
    //padRotate->setBounds(200, 95, 60, 60);
    
    //Native MainComponent components
	modeOffButton->setBounds(672, 205, 40, 40);
	globalSettingsButton->setBounds(632, 562, 50, 50);
    modeMidiButton->setBounds(736, 163, 50, 50);
    modeSamplerButton->setBounds(812, 148, 50, 50);
	modeSequencerButton->setBounds(890, 158, 50, 50);
	modeControllerButton->setBounds(960, 191, 50, 50);
	
	
    openButton->setBounds(153, 7, 30, 30);
    saveButton->setBounds(180, 38, 30, 30);
    
    sceneComponent->setBounds(5, 10, 20 * 8, getHeight());
    
    globalClock->setBounds(479, 0, 266, 144);
    
    infoTextBox->setBounds(0, getHeight()-50, getWidth(), 50);
    
    pivotX = guiPadLayout->getX() + (guiPadLayout->getWidth() * 0.5);
	pivotY = guiPadLayout->getY() + (guiPadLayout->getHeight() * 0.5);
	offskew = guiPadLayout->getY();
    
    
    aboutComponent->setBounds(0, 0, getWidth(), getHeight());
    preferencesComponent->setBounds(0, 0, getWidth(), getHeight());
    projectSettingsComponent->setBounds(0, 0, getWidth(), getHeight());
    softwareUpdateComponent->setBounds(0, 0, getWidth(), getHeight());
	
	gainSlider->setBounds(38, 8, 81, 81);
	panSlider->setBounds(46, 16, 65, 65);
    gainPanValueLabel->setBounds(54, 24, 49, 49);
    
    deviceStatusComponent->setBounds(348, -5, 75, 20);
}



void MainComponent::paint(juce::Graphics &g)
{
    infoTextBox->setColour(TextEditor::backgroundColourId, AlphaTheme::getInstance()->backgroundColour);
    gainPanValueLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->mainColour);
    
    g.setOrigin(0, 0);
    
	g.drawImage(AlphaTheme::getInstance()->mainImage,
                0,
                0,
                getWidth(),
                getHeight() - 5, // -5 as I have since increased the size of the info box
                0,
                0,
                AlphaTheme::getInstance()->mainImage.getWidth(),
                AlphaTheme::getInstance()->mainImage.getHeight());
	
	if (noPadsSelected == 1) 
    {
        g.drawImage(AlphaTheme::getInstance()->padsOffImage,
                    0,
                    0,
                    getWidth(),
                    getHeight() - 5,
                    0,
                    0,
                    AlphaTheme::getInstance()->padsOffImage.getWidth(),
                    AlphaTheme::getInstance()->padsOffImage.getHeight());
        
        if (eliteControlSelected == 0)
        {
            g.drawImage(AlphaTheme::getInstance()->modeOffImage,
                        0,
                        0,
                        getWidth(),
                        getHeight() - 5,
                        0,
                        0,
                        AlphaTheme::getInstance()->modeOffImage.getWidth(),
                        AlphaTheme::getInstance()->modeOffImage.getHeight());
        }
	}
	else if (noPadsSelected == 0) 
    {
		g.drawImage(AlphaTheme::getInstance()->padsOnImage,
                    0,
                    0,
                    getWidth(),
                    getHeight() - 5,
                    0,
                    0,
                    AlphaTheme::getInstance()->padsOnImage.getWidth(),
                    AlphaTheme::getInstance()->padsOnImage.getHeight());
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
		
		g.fillRect(753, 10, 261, 121);
		
		g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
		
		g.drawRect(753, 10, 261, 121, 2);
		
		g.setColour(AlphaTheme::getInstance()->backgroundColour);
	}
	
	if (noModeSelected == 1) 
    {
        if (eliteControlSelected == 0)
        {
            g.drawImage(AlphaTheme::getInstance()->modeOffImage,
                        0,
                        0,
                        getWidth(),
                        getHeight() - 5,
                        0,
                        0,
                        AlphaTheme::getInstance()->modeOffImage.getWidth(),
                        AlphaTheme::getInstance()->modeOffImage.getHeight());
        }
	}
	
	g.drawImage(AlphaTheme::getInstance()->padsBackgroundImage,
                0,
                0,
                getWidth(),
                getHeight() - 5,
                0,
                0,
                AlphaTheme::getInstance()->padsBackgroundImage.getWidth(),
                AlphaTheme::getInstance()->padsBackgroundImage.getHeight());
	
	//gain and pan container
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	g.fillEllipse(35, 5, 87, 87);
	g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
	g.drawEllipse(35, 5, 87, 87, 1.0f);
}


//observers update function, called everytime settings are loaded from file or a scene to update the GUI to the correct display
bool MainComponent::update(const Subject& theChangedSubject)
{
    if (&theChangedSubject == &appDocumentStateRef)
    {
        //update settings display
        if (appDocumentStateRef.getGuiUpdateFlag() == 0)
        {
            //this call effectively updates the GUI display
            setCurrentlySelectedPad(selectedPads, false);
            
            //update GUI things (GUI of global settings) which aren't updated by setCurrentlySelectedPad
            //these things could be put in setCurrentlySelectedPad but they'll be updated everytime a pad is selected which would be inefficent
            //there is still some inefficiency in that these things will be updated everytime a sequence is loaded... sort this out?!!
            gainSlider->setValue(AppSettings::Instance()->getGlobalGain(), dontSendNotification); 
            panSlider->setValue(AppSettings::Instance()->getGlobalPan(), dontSendNotification);
            
            //set the mode colour ring of each pad
            for (int i = 0; i <= 47; i++)
                guiPadLayout->setPadDisplay(i);
            
            globalClock->updateDisplay();
            projectSettingsComponent->updateDisplay();
            
            //change the way in which the mode is displayed on the pads so the loading changes the values
            //PAD DISPLAY ROTATION VALUE LOADED IN HERE
        }
        
        
        //update sceneComponent display
        else if (appDocumentStateRef.getGuiUpdateFlag() == 1)
        {
            sceneComponent->setSlotStatus(appDocumentStateRef.getSceneToUpdate(), appDocumentStateRef.getSceneStatus());
        }
    }
    
    return true;
}


void MainComponent::sliderValueChanged (Slider *slider)
{
    if (slider == gainSlider)
    {
        AppSettings::Instance()->setGlobalGain(gainSlider->getValue());
        gainPanValueLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
    else if (slider == panSlider)
    {
        AppSettings::Instance()->setGlobalPan(panSlider->getValue());
        gainPanValueLabel->setText(String(slider->getValue(), 3), dontSendNotification);
        
    }
    
    
    else if (slider == padRotate)
	{
		guiPadLayout->setVisible(false);
		rotateFlag = 1;
		repaint();
	}
    
}

void MainComponent::sliderDragEnded (Slider *slider)
{
	
}

void MainComponent::sliderDragStarted (Slider *slider)
{
    
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
        buttonIndex = 0;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->setPadDisplay(padNum);
        }
        
        setToOffMode();
    }
    
    else if (button == modeMidiButton)
    {
        buttonIndex = 1;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->setPadDisplay(padNum);
        }
        
        setToMidiMode();
    } 
    
    else if (button == modeSamplerButton)
    {
        buttonIndex = 2;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->setPadDisplay(padNum);
        }
        
        setToSamplerMode();
    }
    
    else if (button == modeSequencerButton)
    {
        buttonIndex = 3;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->setPadDisplay(padNum);
        }
        
        setToSequencerMode();
        
    }
    
    else if (button == modeControllerButton)
    {
        buttonIndex = 4;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->setPadDisplay(padNum);
        }
        
        setToControllerMode();
    }
    
    else if (button == globalSettingsButton)
	{
		setGlobalPadSettingsDisplay();
	}
    
    
   
    /*
    else if (button == autoShowSettingsSwitch)
    {
        guiPadLayout->setShouldDisplaySettings(autoShowSettingsSwitch->getToggleState());
    }
     */
    
}


//this function is called every time a user selects a pad on the GUI,
//or when certain items are selected from the Toolbox
void MainComponent::setCurrentlySelectedPad(Array <int> selectedPads_, bool calledFromMouseClick)
{
    //is this the best place to call this?
    //One problem is that this will be called when importing from the toolbox,
    //where there are situtations where you might not want the sequence to stop
    //this is also called when the sequencer mode buttons are pressed,
    //as this updates the button state so the playing states needs to be updated too
    alphaLiveEngineRef.getModeSequencer()->stopLastPreviewedSequence();
    
    if (selectedPads_.size() == 0) //no pads selected
    {
        noPadsSelected = 1;
		modeOffButton->setVisible(false);
		modeMidiButton->setVisible(false);
		modeSamplerButton->setVisible(false);
		modeSequencerButton->setVisible(false);
		modeControllerButton->setVisible(false);
		globalSettingsButton->setVisible(false);
        toolbox->setVisible(false); //or maybe it links to projects directory?
        midiPiano->setActive(false);
        
        guiMidiMode->setVisible(false);
        guiSamplerMode->setVisible(false);
        guiSequencerMode->setVisible(false);
        guiControllerMode->setVisible(false);
		
        globalSettingsButton->setToggleState(false, dontSendNotification);
        setGlobalPadSettingsDisplay();
        //i think the below if statement should be outside and above of its parent if statement.
        if (eliteControlSelected == 1)
        {
            //This is done here instead of within setEliteControlsSettingsDisplay()
            //so that the display is updated correctly if the user switches scenes
            //whilst an elite control display is currently in view
            eliteControlsSettings->setDisplay(selectedEliteControl);
            eliteControlsSettings->setVisible(true);
			//eliteControls->turnOffButtons();
        }
        
        //repaint(); //this is called in setGlobalPadSettingsDisplay() above
    }
    else if (selectedPads_.size() > 0 && selectedPads.size() == 0) //if previously there were no pads selected,
                                                            //but now is.
    {
        noPadsSelected = 0;
        eliteControlSelected = 0;
		modeOffButton->setVisible(true);
		modeMidiButton->setVisible(true);
		modeSamplerButton->setVisible(true);
		modeSequencerButton->setVisible(true);
		modeControllerButton->setVisible(true);
		globalSettingsButton->setVisible(true);
        toolbox->setVisible(true); //or maybe it links to projects directory?
        eliteControlsSettings->setVisible(false);
		eliteControls->turnOffButtons();
		repaint();
    }
    
    int prevSelectedPad = selectedPads[0];
    selectedPads = selectedPads_;
    
    
    if (selectedPads.size() > 0)
    {
        //pass in the currently selected pads to the mode GUIs for them to use
        //in order to display the right settings from appSettings
        AppSettings::Instance()->setCurrentlySelectedPad(selectedPads);
        guiMidiMode->setCurrentlySelectedPad(selectedPads);
        guiSamplerMode->setCurrentlySelectedPad(selectedPads);
        guiSequencerMode->setCurrentlySelectedPad(selectedPads);
        guiControllerMode->setCurrentlySelectedPad(selectedPads);
        guiGlobalPadSettings->setCurrentlySelectedPad(selectedPads);
        midiPiano->setCurrentlySelectedPad(selectedPads);
        toolbox->setCurrentlySelectedPad(selectedPads);
    }
    
    
    //==============================================================================
    //if current pad selected is a single pad
    if (SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getMode() == 0) //off mode
        {
            setToOffMode();
            modeOffButton->setToggleState(true, dontSendNotification);
        }
        
        if (PAD_SETTINGS->getMode() == 1) //midi mode
        {
            setToMidiMode();
            modeMidiButton->setToggleState(true, dontSendNotification);
            
            if (selectedPads[0] == prevSelectedPad && calledFromMouseClick)
                guiMidiMode->changeView(0);
        }
        
        if (PAD_SETTINGS->getMode() == 2) //sampler mode
        {
            setToSamplerMode();
            modeSamplerButton->setToggleState(true, dontSendNotification);
            
            if (selectedPads[0] == prevSelectedPad && calledFromMouseClick)
                guiSamplerMode->changeView(0);
        }
    
        if (PAD_SETTINGS->getMode() == 3) //sequencer mode
        {
            setToSequencerMode();
            modeSequencerButton->setToggleState(true, dontSendNotification);
            
            if (selectedPads[0] == prevSelectedPad && calledFromMouseClick)
                guiSequencerMode->changeView(0);
        }
        if (PAD_SETTINGS->getMode() == 4) //controller mode
        {
            setToControllerMode();
            modeControllerButton->setToggleState(true, dontSendNotification);
        }
        
    }
    else if (MULTI_PADS)
    {
       
        //could we use a function here to compare settings instead?
        //use an enum to represent the PAD_SETTINGS->get....? which is passed in,
        //along with the value of the corresponding setting of the first pad
        //and then returns a boolean which tells us whether to set the default value or not
        
        
        int mode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getMode();
        
        //loop through all selected pads except for the first one
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            //if setting of this pad does NOT match setting of last pad, set default and break
            if (PAD_SETTINGS->getMode() != mode_)
            {
                setToOffMode();
                modeOffButton->setToggleState(true, dontSendNotification);
                break;
            }
            //if this is the last 'natural' interaction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
            {
                if (mode_ == 0) //off mode
                {
                    setToOffMode();
                    modeOffButton->setToggleState(true, dontSendNotification);
                }
                if (mode_ == 1) //midi mode
                {
                    setToMidiMode();
                    modeMidiButton->setToggleState(true, dontSendNotification);
                }
                if (mode_ == 2) //sampler mode
                {
                    setToSamplerMode();
                    modeSamplerButton->setToggleState(true, dontSendNotification);
                }
                if (mode_ == 3) //sequencer mode
                {
                    setToSequencerMode();
                    modeSequencerButton->setToggleState(true, dontSendNotification);
                }
                if (mode_ == 4) //controller mode
                {
                    setToControllerMode();
                    modeControllerButton->setToggleState(true, dontSendNotification);
                }
            }
        }
        
    }
    
    //is this the best way to do it? At this point the Mode GUI's would have beeen updated already,
    //but if the following statement is true those calls would have been useless.
    if (globalSettingsButton->getToggleState() == true)
        setGlobalPadSettingsDisplay();

    commandManager->commandStatusChanged(); //so that pad copy/paste and pad settings view options
                                            //are updated correctly.
            
     
}

void MainComponent::comboBoxChanged (ComboBox *comboBox)
{
   
    
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
    
    midiPiano->setActive(false);
    toolbox->updateDisplay();
	noModeSelected = 1;
	repaint();
    
}
void MainComponent::setToMidiMode()
{
    guiMidiMode->setVisible(true);
    guiSamplerMode->setVisible(false);
    guiSequencerMode->setVisible(false);
    guiControllerMode->setVisible(false);
    
    //update the display of the midi mode so it shows the
    //currently selected pad's settings, and makes any uneeded
    //components invisble or dissabled
    guiMidiMode->updateDisplay();
    
    //setting the midi piano state is done within
    //updateDisplay()/setDisplay() of guiMidiMode,
    //as it depends on states within that object.
    toolbox->updateDisplay();
	noModeSelected = 0;
	repaint();

    
}
void MainComponent::setToSamplerMode()
{
    guiMidiMode->setVisible(false);
    guiSamplerMode->setVisible(true);
    guiSequencerMode->setVisible(false);
    guiControllerMode->setVisible(false);
    
    //update the display of the sampler mode so it shows the
    //currently selected pad's settings, and makes any unneeded
    //components invisble or dissabled
    guiSamplerMode->updateDisplay();
    
    midiPiano->setActive(false);
    toolbox->updateDisplay();
	noModeSelected = 0;
	repaint();

    
}
void MainComponent::setToSequencerMode()
{
    guiMidiMode->setVisible(false);
    guiSamplerMode->setVisible(false);
    guiSequencerMode->setVisible(true);
    guiControllerMode->setVisible(false);
    
    guiSequencerMode->updateDisplay();
    
    //setting the midi piano state is done within
    //updateDisplay() of guiSequencerMode,
    //as it depends on states within that object.
    //This is also true of the toolbox.
	noModeSelected = 0;
	repaint();
}

void MainComponent::setToControllerMode()
{
    guiMidiMode->setVisible(false);
    guiSamplerMode->setVisible(false);
    guiSequencerMode->setVisible(false);
    guiControllerMode->setVisible(true);
    
    guiControllerMode->updateDisplay();
    
    midiPiano->setActive(false);
    toolbox->updateDisplay();
	noModeSelected = 0;
	repaint();
}

void MainComponent::setGlobalPadSettingsDisplay()
{
    guiMidiMode->setVisible(false);
    guiSamplerMode->setVisible(false);
    guiSequencerMode->setVisible(false);
    guiControllerMode->setVisible(false);
    
    if (globalSettingsButton->getToggleState()==true) 
    {
        //should the piano be dissabled here?
        
        guiGlobalPadSettings->updateDisplay();
        guiGlobalPadSettings->setVisible(true);
        
        modeOffButton->setEnabled(false);
        modeOffButton->setAlpha(0.3f);
        modeMidiButton->setEnabled(false);
        modeMidiButton->setAlpha(0.3f);
        modeSamplerButton->setEnabled(false);
        modeSamplerButton->setAlpha(0.3f);
        modeSequencerButton->setEnabled(false);
        modeSequencerButton->setAlpha(0.3f);
        modeControllerButton->setEnabled(false);
        modeControllerButton->setAlpha(0.3f);
        
        noModeSelected = 0;
    }
    else 
    {
        if (noPadsSelected == false)
        {
            if (modeOffButton->getToggleState()==true)
                noModeSelected = 1;
            
            else if (modeMidiButton->getToggleState()==true)
            {
                guiMidiMode->updateDisplay(); //incase the quantise state has been changed
                guiMidiMode->setVisible(true);
            }
            
            else if (modeSamplerButton->getToggleState()==true)
            {
                guiSamplerMode->updateDisplay(); //incase the quantise state has been changed
                guiSamplerMode->setVisible(true);
            }
            
            else if (modeSequencerButton->getToggleState()==true)
            {
                guiSequencerMode->updateDisplay(); //incase the quantise state has been changed
                guiSequencerMode->setVisible(true);
            }
            
            else if (modeControllerButton->getToggleState()==true)
                guiControllerMode->setVisible(true);
        }
        
        guiGlobalPadSettings->setVisible(false);
        
        modeOffButton->setEnabled(true);
        modeOffButton->setAlpha(1.0f);
        modeMidiButton->setEnabled(true);
        modeMidiButton->setAlpha(1.0f);
        modeSamplerButton->setEnabled(true);
        modeSamplerButton->setAlpha(1.0f);
        modeSequencerButton->setEnabled(true);
        modeSequencerButton->setAlpha(1.0f);
        modeControllerButton->setEnabled(true);
        modeControllerButton->setAlpha(1.0f);
    }
    
    repaint();
}

void MainComponent::setEliteControlsSettingsDisplay (int controlNumber)
{
    eliteControlSelected = 1;
    selectedEliteControl = controlNumber;
    guiPadLayout->deselectAllPads();    //this in turn calls setSelectedPads within this class,
                                        //which sets and displays the elite controls display
}

AlphaLiveEngine& MainComponent::getAlphaLiveEngineRef()
{
    return alphaLiveEngineRef;
}

AppDocumentState& MainComponent::getAppDocumentStateRef()
{
     return appDocumentStateRef;
}

GuiSamplerMode* MainComponent::getGuiSamplerMode()
{
    return guiSamplerMode;
}

GuiSequencerMode* MainComponent::getGuiSequencerMode()
{
    return guiSequencerMode;
}

GuiPiano* MainComponent::getGuiPiano()
{
    return midiPiano;
}

Toolbox* MainComponent::getToolbox()
{
    return toolbox;
}

SceneComponent* MainComponent::getSceneComponent()
{
    return sceneComponent;
}

GuiPadLayout* MainComponent::getGuiPadLayout()
{
    return guiPadLayout;
}

GuiGlobalClock* MainComponent::getGuiGlobalClock()
{
    return globalClock;
}

EliteControlsComponent* MainComponent::getEliteControlsComponent()
{
    return eliteControls;
}


void MainComponent::setInfoTextBoxText (String text)
{
    /* 
     Setting the Info Box is now done using a thread so that the info box will
     only be set/updated if the mouse has been hovering over the control for a certain
     amount of time. This reduces CPU usage when moving the mouse over the interface quickily
     as it won't unneccesarily updated udpdating the text all the time, which also looks smoother.
     
     This is achieved using the following algorithm:
     - Everytime the mouse events or exists a control it calls this function with the controls
     description as an argument. The description is put into the infoBoxText String variable.
     - If the thread is not currently running, start the thread.
     - In the run() function below it will enter the while statement and pause for either 100 ms
     or until a notification wakes up the thread. If the timeout period expires it will exit the
     loop (as gotNewTextMessage will equal false) and set the infoTextBox Text, and the thread
     will exit.
     - If setInfoTextBoxText() is called whilst the thread is running (meaning the thread is 
     currently 'waiting', notify() will be called which will wake up the thread. However this will
     cause gotNewTextMessage to equal true, so it will re-enter the loop and wait for 100 ms again.
     Therefore the info box text will only by updated when the thread is woken by notifications
     sent by new controls being enter exitted by the mouse.
     */
    
    infoBoxText = text;
    
    if (isThreadRunning() == false)
        startThread(3);
    else
        notify();
}


void MainComponent::run()
{
    bool gotNewTextMessage = true;
    
    while (gotNewTextMessage == true)
    {
        gotNewTextMessage = wait(100);
    }
    
    if (isInfoBoxEnabled == true)
    {
        const MessageManagerLock mmLock;
        
        infoTextBox->clear();
        infoTextBox->setText(infoBoxText);
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
        setInfoTextBoxText (translate("Off Mode. Click this button to turn off the selected pads."));
        
    }
    else if (e.eventComponent == modeMidiButton)
    {
        setInfoTextBoxText (translate("MIDI Mode. Click to set the selected pads to MIDI Mode. This enables these pads to send MIDI data. Its controls can then be used to program the MIDI functionality of the AlphaSphere."));
    }
    else if (e.eventComponent == modeSamplerButton)
    {
        setInfoTextBoxText (translate("Sampler Mode.  Click to set the selected pads to Sampler Mode. This enables audio files to be triggered, looped, and manipulated."));
    }
    else if (e.eventComponent == modeSequencerButton)
    {
        setInfoTextBoxText (translate("Sequencer Mode. Click to set the selected pads to Sequencer Mode. This enables sequences of midi data or audio samples to be created, looped and manipulated."));
    }
    else if (e.eventComponent == modeControllerButton)
    {
        setInfoTextBoxText (translate("Controller Mode. Click to set the selected pads to Controller Mode. This allows the pads to control features of AlphaLive or external software."));
    }
    else if (globalSettingsButton->isMouseOver(true))
    {
        setInfoTextBoxText (translate("Global Pad Settings. Click to show settings which are not specific to any of the pad modes."));
    }
    else if (e.eventComponent == openButton)
    {
        setInfoTextBoxText (translate("Load Project. Loads a set of scenes into the application from an AlphaLive Project file."));
    }
    else if (e.eventComponent == saveButton)
    {
        setInfoTextBoxText (translate("Save Project. Saves the current set of scenes to disk as an AlphaLive Project."));
    }
        else if (sceneComponent->isMouseOver(true))
    {
        setInfoTextBoxText (translate("Scenes. AlphaLive contains 20 'scene' slots that can each hold a full set of pad settings. Click on a scene to load up its settings; shift-click to copy the currently select scene to the clicked scene; or right-click for more options. Use the text box to name a scene. AlphaLive scene files (.alphascene) can also be imported via drag-and-drop.")); //
    }
    else if (gainSlider->isMouseOver(true)==true)
    {
        setInfoTextBoxText (translate("Global Gain. Controls AlphaLive's master gain."));
    }
    else if (panSlider->isMouseOver(true)==true)
    {
        setInfoTextBoxText (translate("Global Pan. Controls the stereo positioning of AlphaLive's master output."));
    }
    /*
    else if (e.eventComponent == padRotate)
    {
        setInfoTextBoxText ("Pad Display Rotation Knob. Allows the central pad display to be rotated.");
    }
    */
    else if (e.eventComponent == infoTextBox)
    {
        setInfoTextBoxText (translate("Info Box. Hover the mouse over an interface element to view a description of what it does. This can be disabled in the 'Controls' menu bar menu."));
    }
    /*
    else if(killswitchButton->isMouseOver(true))
    {
        setInfoTextBoxText("Kill Switch. Instantly stops the clock and any playing pads.");
    }
     */
    /*
    else if (autoShowSettingsSwitch->isMouseOver(true))
    {
        setInfoTextBoxText("'Show Pad Settings When Pressed' Option. If this switch is set to 'on' the pad settings will  beautomatically displayed when a pad is pressed. It is recommended that you only use this feature for editing projects and not playing/performing.");
    }
    */
    
    else if (midiPiano->isMouseOver(true))
    {
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getMode() == 1) //midi mode
        {
            setInfoTextBoxText (translate("MIDI Note Selector. Use this piano to select the MIDI notes of the selected pads. Use a regular click to select a single note for all selected pads, or cmd-click (Mac) or ctrl-click (Windows) to select multiple notes to apply to a set of pads. When selecting multiple notes, the order of selected notes will be applied to the pads in the order they were selected. Alt-click to transpose a set of notes."));
        }
        else if (PAD_SETTINGS->getMode() == 3) //sequencer mode
        {
            setInfoTextBoxText (translate("MIDI Note Selector. Use this piano to select the MIDI notes of the sequencer grid for the selected pads. Cmd-click (Mac) or ctrl-click (Windows) to select the set of notes, which will be applied in numerical order, and use a regular click to set the root note. If less than 12 notes are selected no notes will be applied to the remaining rows of the grid."));
        }
    }
    
    
    //update gainPanValueLabel
    if (gainSlider->isMouseOver(true))
        gainPanValueLabel->setText(String(gainSlider->getValue(), 3), dontSendNotification);
    else if (panSlider->isMouseOver(true))
        gainPanValueLabel->setText(String(panSlider->getValue(), 3), dontSendNotification);
    
    
    mouseOverComponent = e.eventComponent;
    
}

//called whenever a the mouse exits a component which has a MouseListener attached to it
void MainComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    setInfoTextBoxText (String());
    
    
    if(e.eventComponent == gainSlider || e.eventComponent == panSlider)
        gainPanValueLabel->setText(String(), dontSendNotification);
        
        
}



void MainComponent::setLocalisation()
{
    static String countryCode = SystemStats::getDisplayLanguage();
    std::cout << "Language: " << countryCode << std::endl;
    
    //We may need to find suitable fonst that exists on the current system
    //for languages such as Chinese, Japanese, and Korean.
    //http://en.wikipedia.org/wiki/List_of_CJK_fonts
    
    StringArray availableFonts = Font::findAllTypefaceNames();
    
    infoBoxTextSize = 12.0; //default font size
    AlphaTheme::getInstance()->fontSizeAddition = 0;
    
    //countryCode will equal ISO 639-1 or ISO 639-2 codes as listed here:
    //http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
    
    if (countryCode == "ja" || countryCode == "jpn") //japanese
    {
        File transFile (appDir + "Application Data" + File::getSeparatorString() + "trans_ja.txt");
        trans = new LocalisedStrings (transFile, false);
        LocalisedStrings::setCurrentMappings(trans);
        
        String fontToUse = "Arial Unicode MS"; // available on OSX 10.5 and above
        
        if (availableFonts.contains(fontToUse) == false)
        {
            fontToUse = "Meiryo"; // available on Windows Vista and above
            
            if (availableFonts.contains(fontToUse) == false)
            {
                fontToUse = "MS PGothic"; // available on Windows XP
                
                //what about on Linux?
            }
        }

        alphaLiveLookAndFeel.setDefaultSansSerifTypefaceName(fontToUse);
        
        AlphaTheme::getInstance()->fontSizeAddition = 2; //should this change with the typeface?
        infoBoxTextSize += AlphaTheme::getInstance()->fontSizeAddition;
        
        currentLanguage = "Japanese";
        
    }
    else if (countryCode == "zh" || countryCode == "zho" || countryCode == "zh-Hant" || countryCode == "zh-Hans") //chinese. do i need the first two?
    {
        File transFile (appDir + "Application Data" + File::getSeparatorString() + "trans_zh.txt");
        trans = new LocalisedStrings (transFile, false);
        LocalisedStrings::setCurrentMappings(trans);
        
        String fontToUse = "Arial Unicode MS"; // available on OSX 10.5 and above
        
        if (availableFonts.contains(fontToUse) == false)
        {
            fontToUse = "Microsoft JhengHei"; // available on Windows Vista and above
            
            if (availableFonts.contains(fontToUse) == false)
            {
                fontToUse = "SimHei"; // available on Windows XP
                
                //do we neeed to consider simplified vs traditional?
                //what about on Linux?
            }
        }

		alphaLiveLookAndFeel.setDefaultSansSerifTypefaceName(fontToUse);
        
        AlphaTheme::getInstance()->fontSizeAddition = 2; //should this change with the typeface?
        infoBoxTextSize += AlphaTheme::getInstance()->fontSizeAddition;
        
        currentLanguage = "Chinese";
    }
    else if (countryCode == "ko" || countryCode == "kor") //Korean
    {
        File transFile (appDir + "Application Data" + File::getSeparatorString() + "trans_ko.txt");
        trans = new LocalisedStrings (transFile, false);
        LocalisedStrings::setCurrentMappings(trans);
        
        String fontToUse = "AppleMyungjo"; // available on OSX 10.5 and above
        
        if (availableFonts.contains(fontToUse) == false)
        {
            fontToUse = "Batang"; // available on Windows.. XP and above?
        }
        
		alphaLiveLookAndFeel.setDefaultSansSerifTypefaceName(fontToUse);
        
        AlphaTheme::getInstance()->fontSizeAddition = 2; //should this change with the typeface?
        infoBoxTextSize += AlphaTheme::getInstance()->fontSizeAddition;
        
        currentLanguage = "Korean";
    }
    else //english
    {
        LocalisedStrings::setCurrentMappings(0);
        
        currentLanguage = "English";
    }
     
}


void MainComponent::sendEliteDialCommand (int command, int eliteControlValue)
{
    //command signifies which control within this class needs changing
    //eliteControlValue is the value sent by the elite dial  
    
    //===get slider depending on command value===
    Slider *sliderToChange = nullptr;
    double incremValue;
    
    //global gain
    if (command == 1)
    {
        sliderToChange = gainSlider;
        incremValue = 0.01;
    }
    //global pan
    else if (command == 2)
    {
        sliderToChange = panSlider;
        incremValue = 0.01;
    }
    //tempo
    else if (command == 3)
    {
        sliderToChange = globalClock->getTempoSlider();
        incremValue = 1.0;
    }
    
    //smart dial
    else if (command == 4)
    {
        const bool isSlider = dynamic_cast<juce::Slider *> (mouseOverComponent) != nullptr;
        
        if (isSlider)
        {
            sliderToChange = dynamic_cast<juce::Slider *> (mouseOverComponent);

            //Determine the increm value.
            //Increm value is set based on the sliders set interval
            //An increm value of interval * 10 seems appropriate for intervals below 1.
            //However for intervals of 1 it seems more appropriate to set it using
            //the sliders maximum value.
            
            if (sliderToChange->getInterval() < 1.0)
            {
                incremValue = sliderToChange->getInterval() * 10;
            }
            else
            {
                incremValue = sliderToChange->getInterval();
                
                if (sliderToChange->getMaximum() >= 10000)
                {
                    incremValue *= 100;
                }
                else if (sliderToChange->getMaximum() >= 1000)
                {
                    incremValue *= 10;
                }
            }
            
            //just to be safe (this could be caused is a sliders interval isn't set)...
            if (incremValue == 0)
                incremValue = 0.1;
            
        }
    
    }
    
    //===process slider value===
    if (sliderToChange != nullptr)
    {
        double newVal = 0;
        double currentVal = sliderToChange->getValue();
        double maxValue = sliderToChange->getMaximum();
        double minValue = sliderToChange->getMinimum();
        
        //incremented value
        if (eliteControlValue >= 1 && eliteControlValue <= 63)
            newVal = currentVal + (eliteControlValue * incremValue);
        //decremented value
        else if (eliteControlValue >= 64 && eliteControlValue <= 127)
            newVal = currentVal - ((128 - eliteControlValue) * incremValue);
        
        //if produced value is out of range, set in range
        if (!(newVal >= minValue && newVal <= maxValue))
        {
            if (newVal > maxValue)
                newVal = maxValue;
            else if (newVal < minValue)
                newVal = minValue;
        }
        
        //if new value is different from the previous value, change the slider value
        if (newVal != currentVal)
        {
            //should I be locking the message thread like I'm currently doing?
            //or could this cause delays/lagging, in which case I should use an aSyncUpdater?
            const MessageManagerLock mmLock;
            
            sliderToChange->setValue(newVal, sendNotification);
            //std::cout << "New Slider Value: " << newVal << std::endl;
        }
    }
    
}


void MainComponent::editInterfaceFromDeviceConnectivity (int command)
{
    //This function will be used to change the appearence of the interface
    //when a HID device is connected on runtime.
    
    //remove midi output selector from preferences view
    if (command == 1)
    {
        preferencesComponent->removeMidiOutputSelector();
    }
    
    //set the the correct GUI based on the current device type
    else if (command == 2)
    {
        if (StoredSettings::getInstance()->deviceType == 1)
        {
            eliteControls->setVisible(false);
        }
        else if (StoredSettings::getInstance()->deviceType == 2)
        {
            eliteControls->setVisible(true);
        }
        
    }
    
    //set the device status component
    else if (command == 3)
    {
        // 0 - disconnected, 1 - connected, 2 - error
        deviceStatusComponent->setDeviceStatus(alphaLiveEngineRef.getDeviceStatus());
    }
    
    //set firmware version and device serial display
    else if (command == 4)
    {
        aboutComponent->setFirmwareDetails (alphaLiveEngineRef.getFirmwareVersion(), alphaLiveEngineRef.getDeviceSerial());
    }
}

void MainComponent::setDeviceStatus (int status)
{
    
}

void MainComponent::openDocumentation (int type)
{
    String docDir (appDir + "Documentation" + File::getSeparatorString());
    bool opened = true;
    
    if (currentLanguage == "Japanese")
    {
        if (type == 2) //Reference manual
            opened = File(docDir + "AlphaLive Reference Manual Japanese.pdf").startAsProcess();
    }
    else if (currentLanguage == "Chinese")
    {
        if (type == 2) //Reference manual
            opened = File(docDir + "AlphaLive Reference Manual Chinese.pdf").startAsProcess();
    }
    
    
    if (currentLanguage == "English" || opened == false)
    {
        if (type == 2) //Reference manual
            opened = File(docDir + "AlphaLive Reference Manual English.pdf").startAsProcess();
    }
    
    if (opened == false) //probably due to the file not existing
        AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, 
                                          translate("Documentation cannot be found!"), 
                                          translate("The documentation file seems to be missing from the AlphaLive application directory."));
}


bool MainComponent::updateSoftware (bool autoCheck)
{
    /*
     
     This function and the AlphaLive Updater executable should share responsibilities
     of the software updating process in such a way so there is scope for the user
     to be able to update the software on a machine without an internet connection.
     This would involve:
     -  manually downloading the zip file on a networked machine 
     -  uncompressing it into the right place on the machine containing AlphaLive 
     -  manually moving the new AlphaLive Updater exe into the right place,
     if it exists.
     -  manually launching AlphaLive Updater
     
     Therefore this function should be responsible for the following steps:
     -  Getting the latest AlphaLive version number by decoding the String
     from the relevant .php file on our server.
     -  Downloading the update zip file from our server
     -  Uncompressing the zip file into the AlphaLive application directory
     -  Correct the file permissions of any executable files. Or should this
     -  be done within AlphaLive Updater?
     -  If there is a new AlphaLive Updater exe, move that into the
     correct place.
     -  Launch the AlphaLive Updater executable.
     -  Close AlphaLive.
     
     The AlphaLive Updater executable will then execute the following steps:
     -  Copying all the new files into place
     -  Deleting all downloaded files
     -  Reopening AlphaLive.
     */
    
    bool isUpdating = false;
    
    //get latest AlphaLive version from somewhere online
    URL versionUrl ("http://www.alphasphere.com/AlphaLive_Version.php");
    String urlString = String();
    
    //get the text using an InputStream object so that we
    //can set a timeout incase there is a problem with the
    //internet connection (which would cause hanging)
    ScopedPointer<InputStream> inputStream;
    inputStream = versionUrl.createInputStream(false, nullptr, nullptr, String(), 8000); //what should the timeout be?
    if (inputStream)
        urlString = inputStream->readEntireStreamAsString();
    
    //std::cout << urlString << std::endl;
    
	int versionNumber = 0;
	
	if (urlString.contains("AlphaLiveVersion="))
		{
			int startIndex = urlString.indexOf("AlphaLiveVersion=") + 18;
			int endIndex = urlString.indexOf(startIndex, "\"");
			String versionString (urlString.substring(startIndex, endIndex));
			versionNumber = versionString.getHexValue32();
		}
    
    if (versionNumber == 0)
    {
        //Probably means the computer is not connected to the internet
        
        if (autoCheck == false)
        {
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                              translate("Error!"),
                                              translate("This computer's internet connection appears to be offline, or there is a problem connecting to our server. Please check your network settings or try again later."));
        }
    }
    else
    {
        std::cout << "Current Version: " << ProjectInfo::versionNumber << " Latest Version: " << versionNumber << std::endl;
        
        //compare current version number with latest version number
        if (versionNumber > ProjectInfo::versionNumber)
        {
            bool userSelection;
            
            if (autoCheck == false)
            {
                userSelection = AlertWindow::showOkCancelBox(AlertWindow::InfoIcon, 
                                                            translate("Software update available!"), 
                                                            translate("There is a new version of AlphaLive available to download. Press OK to install the update. The installation process involves restarting AlphaLive so you may want to go back and save the current project first."));
            }
            else
            {
                userSelection = AlertWindow::showOkCancelBox(AlertWindow::InfoIcon, 
                                                            translate("Software update available!"), 
                                                            translate("There is a new version of AlphaLive available to download. Press OK to install the update. You can turn off automatic software update checks via the preferences display."));
            }
            
            if (userSelection == true)
            {
                // kill any pads to help prevent crashes
                //is there a way to prevent the users from
                //pressing pads when updating? Ideally I just
                //need to make sure the app won't crash on close
                //when there are playing pads.
                alphaLiveEngineRef.killAll();
                globalClock->toggleTransportButtonOff();
                
                //trigger update procedure 
                softwareUpdateComponent->startThread();
                isUpdating = true;
                
            }
            //else, no updating will take place
        }
        else
        {
            if (autoCheck == false)
            {
                AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                                  translate("AlphaLive is up-to-date!"),
                                                  translate("There is no update available for AlphaLive at this time."));
            }
        }
    }
    
    //===========
    
    return isUpdating;
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
        CommandIDs::SaveScene,
        CommandIDs::LoadScene,
        CommandIDs::DisableHelpBox,
        CommandIDs::CopyPadSettings,
        CommandIDs::PastePadSettings,
        CommandIDs::ClearScene,
        CommandIDs::ClearAllScenes,
        CommandIDs::KillSwitch,
        CommandIDs::StartStopClock,
        CommandIDs::StarterGuide,
        CommandIDs::ReferenceManual,
        CommandIDs::UpdateSoftware,
        CommandIDs::CopyDataToSequencer,
        CommandIDs::HardwarePreferences,
        CommandIDs::HardwareProjectSettings,
        CommandIDs::SendMidiClock,
        CommandIDs::SyncToMidiClock,
        CommandIDs::MidiClockSettings,
        CommandIDs::WebsiteHomeLink,
        CommandIDs::WebsiteTutorialsLink,
        CommandIDs::WebsiteSupportLink,
        CommandIDs::WebsiteForumLink,
        CommandIDs::ViewTriggerSettings,
        CommandIDs::ViewPressureSettings,
        CommandIDs::ViewGlobalPadSettings,
        CommandIDs::ViewSequenceSettings,
        CommandIDs::ViewScenePresets,
        CommandIDs::EnabledPadContentsDisplay,
        CommandIDs::DisablePressureFeedback
    };
	
	commands.addArray (ids, numElementsInArray (ids));
}


void MainComponent::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
	const int cmd = ModifierKeys::commandModifier;
	const int shift = ModifierKeys::shiftModifier;
    const int alt = ModifierKeys::altModifier;
    
    //within 'setInfo()' below, the name sets the String that appears in the Menu bar,
    //and the description sets what would appear in the tooltip if the command is set to a button
    //and the tooltip parameter is set to 'true'
	
    if(commandID == CommandIDs::About)
    {
		result.setInfo (translate("About AlphaLive"),
						"Opens the application 'about' view.",
						CommandCategories::OtherCommands, 0);
	}
	else if(commandID == CommandIDs::Preferences)
    {
		result.setInfo (translate("Preferences..."),
						"Opens the application preferences view.",
						CommandCategories::OtherCommands, 0);
		result.defaultKeypresses.add (KeyPress (',', cmd, 0));
	}
    
    else if (commandID == CommandIDs::ProjectSettings)
    {
        result.setInfo (translate("Project Settings..."),
						"Opens the Project Settings view.",
						CommandCategories::FileCommands, 0);
        result.defaultKeypresses.add (KeyPress ('p', cmd|shift, 0));
    }
    else if (commandID == CommandIDs::SaveScene)
    {
        result.setInfo (translate("Export Scene..."),
						"Saves the currently selected scene settings to file.",
						CommandCategories::FileCommands, 0);
    }
    else if(commandID == CommandIDs::LoadScene)
	{
		result.setInfo (translate("Import Scene..."),
						"Loads an individual scene settings into the currently selected scene.",
						CommandCategories::FileCommands, 0);
	}
    else if(commandID == CommandIDs::DisableHelpBox)
	{
		result.setInfo (translate("Disable Info Box"),
						"Disables the info box to reduce CPU usage.",
						CommandCategories::OptionCommands, 0);
        result.setTicked(! isInfoBoxEnabled);
	}
    
    else if(commandID == CommandIDs::CopyPadSettings)
	{
		result.setInfo (translate("Copy Pad Settings"),
						"Copy's the settings of the currently selected pad.",
						CommandCategories::FileCommands, 0);
		result.defaultKeypresses.add (KeyPress ('c', cmd, 0));
        result.setActive(SINGLE_PAD);
	}
    else if(commandID == CommandIDs::PastePadSettings)
	{
		result.setInfo (translate("Paste Pad Settings"),
						"Paste's settings to the currently selected pads.",
						CommandCategories::FileCommands, 0);
		result.defaultKeypresses.add (KeyPress ('v', cmd, 0));
        result.setActive(SINGLE_PAD);
	}
    
    else if (commandID == CommandIDs::ClearScene)
    {
		result.setInfo (translate("Clear Scene"),
						"Clears and removes the currently selected scene.",
						CommandCategories::EditCommands, 0);
		
	}
    else if (commandID == CommandIDs::ClearAllScenes)
    {
		result.setInfo (translate("Clear All Scenes"),
						"Clears and removes all scenes.",
						CommandCategories::EditCommands, 0);
	}
    
    else if (commandID == CommandIDs::KillSwitch)
    {
		result.setInfo (translate("Killswitch"),
						"Kills all playing sound and any hanging MIDI notes.",
						CommandCategories::ControlCommands, 0);
        result.defaultKeypresses.add (KeyPress ('k', cmd, 0));
	}
    
    else if (commandID == CommandIDs::StartStopClock)
    {
        if (isClockRunning == false)
        {
            result.setInfo (translate("Start Clock"),
                            "Starts the global clock.",
                            CommandCategories::ControlCommands, 0);
        }
        else
        {
            result.setInfo (translate("Stop Clock"),
                            "Stops the global clock.",
                            CommandCategories::ControlCommands, 0);
        }
        
        result.addDefaultKeypress(KeyPress::spaceKey, NULL);
        
	}
    
    else if (commandID == CommandIDs::StarterGuide)
    {
        result.setInfo (translate("User Guide..."),
                        "Opens the AlphaLive User Guide using the systems default PDF viewer application.",
                        CommandCategories::OtherCommands, 0);
    }
    else if (commandID == CommandIDs::ReferenceManual)
    {
        result.setInfo (translate("Reference Manual..."),
                        "Opens the AlphaLive Reference Manual using the systems default PDF viewer application.",
                        CommandCategories::OtherCommands, 0);
    }
    else if (commandID == CommandIDs::UpdateSoftware)
    {
        result.setInfo (translate("Check For Updates..."),
                        "Checks online to see if there is an AlphaLive update available, and installs it if so.",
                        CommandCategories::FileCommands, 0);
    }
    
    else if (commandID == CommandIDs::CopyDataToSequencer)
    {
        // This item should only be active if the user has selected only MIDI pads that all have
        // the same MIDI channel along with 1 or more sequencer pads, OR only sampler pads along
        // with 1 or more sequencer pads.
        
        bool shouldBeActive = false;
        String itemString (translate("Copy Notes/Samples To Sequencer"));
        
        Array <int> padModes;
        
        //get the modes of the selected pads
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            padModes.insert(i, PAD_SETTINGS->getMode());
        }
        
        //check to see if the selected pads are set to MIDI and sequencer mode only
        if (padModes.contains(1) && padModes.contains(3) && !padModes.contains(2))
        {
            Array<int> midiChannels;
            
            //get the MIDI channels of the selected MIDI pads
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                
                if (PAD_SETTINGS->getMode() == 1)
                    midiChannels.addIfNotAlreadyThere(PAD_SETTINGS->getMidiChannel());
            }
            
            //check to see if all MIDI pad channels are the same
            if (midiChannels.size() == 1)
            {
                shouldBeActive = true;
                itemString = translate("Copy Selected MIDI Notes To Sequencer");
            }
            
        }
        //check to see if the selected pads are set to Sampler and Sequencer mode only
        else if (padModes.contains(2) && padModes.contains(3) && !padModes.contains(1))
        {
            shouldBeActive = true;
            itemString = translate("Copy Selected Samples To Sequencer");
        }
        
        result.setInfo (translate(itemString),
                        "Applies the notes/samples of the selected pads to the selected sequencer pad.",
                        CommandCategories::EditCommands, 0);
        
        result.setActive(shouldBeActive);
    }
    
    else if (commandID == CommandIDs::HardwarePreferences)
    {
        result.setInfo (translate("Hardware Preferences..."),
						"Opens the application hadware preferences view.",
						CommandCategories::HardwareCommands, 0);
    }
    
    else if (commandID == CommandIDs::HardwareProjectSettings)
    {
        result.setInfo (translate("Hardware Project Settings..."),
						"Opens the application hardware project settings view.",
						CommandCategories::HardwareCommands, 0);
    }
    
    else if (commandID == CommandIDs::SendMidiClock)
    {
        result.setInfo (translate("Send MIDI Clock"),
						"Enables the application to send MIDI Clock messages when the global clock is running.",
						CommandCategories::OptionCommands, 0);
        
        bool status = false;
        if (AppSettings::Instance()->getMidiClockValue() == 2)
            status = true;
        
        result.setTicked(status);
    }
    
    else if (commandID == CommandIDs::SyncToMidiClock)
    {
        result.setInfo (translate("Sync to External MIDI Clock"),
						"Enables the application's clock to sync to an external MIDI clock.",
						CommandCategories::OptionCommands, 0);
        
        bool status = false;
        if (AppSettings::Instance()->getMidiClockValue() == 3)
            status = true;
        
        result.setTicked(status);
        result.setActive(!alphaLiveEngineRef.getGlobalClock()->isThreadRunning());
    }
    
    else if (commandID == CommandIDs::MidiClockSettings)
    {
        result.setInfo (translate("MIDI Clock Settings..."),
						"Opens the application MIDI clock project settings view.",
						CommandCategories::OptionCommands, 0);
    }
    
    else if (commandID == CommandIDs::WebsiteHomeLink)
    {
        result.setInfo (translate("Visit alphasphere.com..."),
						"Opens the AlphaSphere website homepage in an internet browser.",
						CommandCategories::OtherCommands, 0);
    }
    
    else if (commandID == CommandIDs::WebsiteTutorialsLink)
    {
        result.setInfo (translate("Tutorials..."),
						"Opens the AlphaSphere website tutorial page in an internet browser.",
						CommandCategories::OtherCommands, 0);
    }
    
    else if (commandID == CommandIDs::WebsiteSupportLink)
    {
        result.setInfo (translate("Troubleshooting/Support..."),
						"Opens the AlphaSphere website troubleshooting/support page in an internet browser.",
						CommandCategories::OtherCommands, 0);
    }
    
    else if (commandID == CommandIDs::WebsiteForumLink)
    {
        result.setInfo (translate("User Forums..."),
						"Opens the AlphaSphere website forums page in an internet browser.",
						CommandCategories::OtherCommands, 0);
    }
    
    
    else if (commandID == CommandIDs::ViewTriggerSettings)
    {
        result.setInfo (translate("Show Pads Touch/Trigger Settings..."),
						"Displays the touch/trigger settings view for the selected pads.",
						CommandCategories::ViewCommands, 0);
        
        result.defaultKeypresses.add (KeyPress ('1', cmd, 0));
        result.setActive(selectedPads.size() && !(guiControllerMode->isVisible()) && !(modeOffButton->getToggleState()));
    }
    else if (commandID == CommandIDs::ViewPressureSettings)
    {
        result.setInfo (translate("Show Pads Pressure Settings..."),
						"Displays the pressure settings view for the selected pads.",
						CommandCategories::ViewCommands, 0);
        
        result.defaultKeypresses.add (KeyPress ('2', cmd, 0));
        result.setActive(selectedPads.size() && !(guiControllerMode->isVisible()) && !(modeOffButton->getToggleState()));
    }
    else if (commandID == CommandIDs::ViewGlobalPadSettings)
    {
        result.setInfo (translate("Show Pads Global Settings..."),
						"Displays the global settings view for the selected pads.",
						CommandCategories::ViewCommands, 0);
        
        result.defaultKeypresses.add (KeyPress ('3', cmd, 0));
        result.setActive(selectedPads.size() && !(guiControllerMode->isVisible()));
    }
    else if (commandID == CommandIDs::ViewSequenceSettings)
    {
        result.setInfo (translate("Show Pads Sequence Settings..."),
						"Displays the sequence settings view for the selected pads.",
						CommandCategories::ViewCommands, 0);
        
        result.defaultKeypresses.add (KeyPress ('4', cmd, 0));
        result.setActive(selectedPads.size() && (modeSequencerButton->getToggleState()));
    }
    else if (commandID == CommandIDs::ViewScenePresets)
    {
        result.setInfo (translate("Show Scene Presets in Toolbox..."),
						"Selects all pads and displays the scene preset tab in the Toolbox.",
						CommandCategories::ViewCommands, 0);
    }
    else if (commandID == CommandIDs::EnabledPadContentsDisplay)
    {
        result.setInfo (translate("Display Pad Contents on Pads..."),
						"Sets a preferences option that sets what is display on the Pad Layout pads.",
						CommandCategories::ViewCommands, 0);
        result.defaultKeypresses.add (KeyPress ('o', cmd|alt, 0));
        result.setTicked(StoredSettings::getInstance()->padContentDisplay - 1);
    }
    else if (commandID == CommandIDs::DisablePressureFeedback)
    {
        result.setInfo (translate("Disable Pressure Feedback"),
						"Sets a project setting that disables the pressure feedback on the pads.",
						CommandCategories::OptionCommands, 0);
        result.setTicked(AppSettings::Instance()->getDisablePressureFeedback());
    }

}



bool MainComponent::perform (const InvocationInfo& info)
{
	if(info.commandID == CommandIDs::About)
	{
        projectSettingsComponent->setVisible(false);
		preferencesComponent->setVisible(false);
        
        aboutComponent->toFront(true);
        infoTextBox->toFront(false);
		aboutComponent->setVisible(true);
        aboutComponent->grabKeyboardFocus();    //so that the ESC to close works without having to
                                                //click on the component first
        
        return true;
        
	}
    
	else if(info.commandID == CommandIDs::Preferences)
	{
        aboutComponent->setVisible(false);
        projectSettingsComponent->setVisible(false);
        
        preferencesComponent->toFront(true);
        infoTextBox->toFront(false);
		preferencesComponent->setVisible(true);
        preferencesComponent->grabKeyboardFocus();
        
        return true;
	}
    
    else if(info.commandID == CommandIDs::ProjectSettings)
	{
        aboutComponent->setVisible(false);
		preferencesComponent->setVisible(false);
        
        projectSettingsComponent->toFront(true);
        infoTextBox->toFront(false);
		projectSettingsComponent->setVisible(true);
        projectSettingsComponent->grabKeyboardFocus();
        
        return true;
	}
    
    else if(info.commandID == CommandIDs::SaveScene)
	{
		sceneComponent->getSelectedSceneSlot()->saveScene();
        return true;
	}
    
    else if(info.commandID == CommandIDs::LoadScene)
	{
		sceneComponent->getSelectedSceneSlot()->loadScene();
        return true;
	}
    
    else if(info.commandID == CommandIDs::DisableHelpBox)
	{
        if (isInfoBoxEnabled == true)
            isInfoBoxEnabled = false;
        else
            isInfoBoxEnabled = true;
        
        return true;
	}
    
    else if(info.commandID == CommandIDs::CopyPadSettings) 
	{
		guiPadLayout->copyPadSettings();
        return true;
	}
    
    else if(info.commandID == CommandIDs::PastePadSettings) 
	{
		guiPadLayout->pastePadSettings();
        return true;
	}
    
    else if(info.commandID == CommandIDs::ClearScene)
	{
		sceneComponent->getSelectedSceneSlot()->clearScene();
        return true;
	}
    
    else if(info.commandID == CommandIDs::ClearAllScenes)
	{
		bool userSelection;
        userSelection = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("Are you sure?"), translate("You cannot undo this command."));
        
        if (userSelection == true)
        {
            sceneComponent->clearAll();
        }
        
        return true;
	}
    
    else if(info.commandID == CommandIDs::KillSwitch)
	{
		alphaLiveEngineRef.killAll();
        globalClock->toggleTransportButtonOff();
        return true;
	}
    
    else if(info.commandID == CommandIDs::StartStopClock)
	{
		globalClock->triggerTransportButton();
        return true;
	}
    
    else if(info.commandID == CommandIDs::StarterGuide)
    {
        openDocumentation (1);
        return true;
    }
    
    else if(info.commandID == CommandIDs::ReferenceManual)
    {
        openDocumentation (2);
        return true;
    }
    
    else if(info.commandID == CommandIDs::UpdateSoftware)
    {
        updateSoftware(false);
        return true;
    }
    
    else if(info.commandID == CommandIDs::CopyDataToSequencer)
    {
        Array <int> padModes;
        
        //get the modes of the selected pads
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            padModes.insert(i, PAD_SETTINGS->getMode());
        }
        
        //applying MIDI notes to sequencer pad/s
        if (padModes.contains(1))
        {
            Array <int> midiNotes;
            int midiChannel;
            
            //get all MIDI notes of the selected MIDI pads
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                
                if (PAD_SETTINGS->getMode() == 1)
                {
                    midiNotes.add(PAD_SETTINGS->getMidiNote());
                    //don't sort the array into note order, as if
                    //the user has selected more than 12 MIDI pads
                    //we want to use the first 12 selected notes not
                    //the first 12 lowest notes.
                    
                    midiChannel = PAD_SETTINGS->getMidiChannel();
                }
           
            }
            
            int noOfNotes = midiNotes.size();
            if (noOfNotes > 12)
                noOfNotes = 12;
            
            //apply the selected MIDI notes to the selected sequencer pad/s
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                
                if (PAD_SETTINGS->getMode() == 3)
                {
                    PAD_SETTINGS->setSequencerMode(1);
                    PAD_SETTINGS->setSequencerMidiChannel(midiChannel);
                    
                    for (int row = 0; row < noOfNotes; row++)
                    {
                        PAD_SETTINGS->setSequencerMidiNote(midiNotes[row], row);
                    }
                }
            }
        }
        
        //applying samples to sequencer pad/s
        else if (padModes.contains(2))
        {
            Array <File> samples;
            
            //get all samples of the selected Sampler pads
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                
                if (PAD_SETTINGS->getMode() == 2)
                {
                    samples.add(PAD_SETTINGS->getSamplerAudioFilePath());
                }
                
            }
            
            int noOfSamples = samples.size();
            if (noOfSamples > 12)
                noOfSamples = 12;
            
            //apply the selected samples to the selected sequencer pad/s
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                
                if (PAD_SETTINGS->getMode() == 3)
                {
                    PAD_SETTINGS->setSequencerMode(2);
                    
                    for (int row = 0; row < noOfSamples; row++)
                    {
                        PAD_SETTINGS->setSequencerSamplesAudioFilePath(samples[row], row);
                    }
                }
            }
        }
        
        return true;
    }
    
    else if(info.commandID == CommandIDs::HardwarePreferences)
	{
		aboutComponent->setVisible(false);
        projectSettingsComponent->setVisible(false);
        
        preferencesComponent->toFront(true);
        infoTextBox->toFront(false);
        
        preferencesComponent->selectHardwareTab();
		preferencesComponent->setVisible(true);
        preferencesComponent->grabKeyboardFocus();
        
        return true;
	}
    
    else if(info.commandID == CommandIDs::HardwareProjectSettings)
	{
		aboutComponent->setVisible(false);
        preferencesComponent->setVisible(false);
        
        projectSettingsComponent->toFront(true);
        infoTextBox->toFront(false);
        
        projectSettingsComponent->selectTab(1);
		projectSettingsComponent->setVisible(true);
        projectSettingsComponent->grabKeyboardFocus();
        
        return true;
	}
    
    else if (info.commandID == CommandIDs::SendMidiClock)
    {
        uint8 status;
        
        if (AppSettings::Instance()->getMidiClockValue() == 2)
        {
            //turn off MIDI clock
            status = 1;
        }
        else
        {
            //turn on sending
            status = 2;
        }
        
        AppSettings::Instance()->setMidiClockValue(status);
        globalClock->updateTransportButtonDisplay(alphaLiveEngineRef.getGlobalClock()->isThreadRunning());
        
        return true;
    }
    
    else if (info.commandID == CommandIDs::SyncToMidiClock)
    {
        uint8 status;
        
        if (AppSettings::Instance()->getMidiClockValue() == 3)
        {
            //turn off MIDI clock
            status = 1;
        }
        else
        {
            //turn on syncing
            status = 3;
        }
        
        AppSettings::Instance()->setMidiClockValue(status);
        globalClock->updateTransportButtonDisplay(alphaLiveEngineRef.getGlobalClock()->isThreadRunning());

        return true;
    }
    
    else if(info.commandID == CommandIDs::MidiClockSettings)
	{
		aboutComponent->setVisible(false);
        preferencesComponent->setVisible(false);
        
        projectSettingsComponent->toFront(true);
        infoTextBox->toFront(false);
        
        projectSettingsComponent->selectTab(0);
		projectSettingsComponent->setVisible(true);
        projectSettingsComponent->grabKeyboardFocus();
        
        return true;
	}
    
    else if (info.commandID == CommandIDs::WebsiteHomeLink)
    {
        URL url ("http://www.alphasphere.com/");
        url.launchInDefaultBrowser();
        return true;
    }
    
    else if (info.commandID == CommandIDs::WebsiteTutorialsLink)
    {
        URL url ("http://www.alphasphere.com/tutorials/");
        url.launchInDefaultBrowser();
        return true;
    }
    
    else if (info.commandID == CommandIDs::WebsiteSupportLink)
    {
        URL url ("http://www.alphasphere.com/support/");
        url.launchInDefaultBrowser();
        return true;
    }
    
    else if (info.commandID == CommandIDs::WebsiteForumLink)
    {
        URL url ("http://forums.alphasphere.com");
        url.launchInDefaultBrowser();
        return true;
    }
    
    else if (info.commandID == CommandIDs::ViewTriggerSettings)
    {
        if (guiGlobalPadSettings->isVisible())
        {
            globalSettingsButton->setToggleState(false, dontSendNotification);
            setGlobalPadSettingsDisplay();
        }
        
        if (guiMidiMode->isVisible())
            guiMidiMode->changeView(1);
        else if (guiSamplerMode->isVisible())
            guiSamplerMode->changeView(1);
        else if (guiSequencerMode->isVisible())
            guiSequencerMode->changeView(1);
        
        return true;
    }
    else if (info.commandID == CommandIDs::ViewPressureSettings)
    {
        if (guiGlobalPadSettings->isVisible())
        {
            globalSettingsButton->setToggleState(false, dontSendNotification);
            setGlobalPadSettingsDisplay();
        }
        
        if (guiMidiMode->isVisible())
            guiMidiMode->changeView(2);
        else if (guiSamplerMode->isVisible())
            guiSamplerMode->changeView(2);
        else if (guiSequencerMode->isVisible())
            guiSequencerMode->changeView(2);
        return true;
    }
    else if (info.commandID == CommandIDs::ViewGlobalPadSettings)
    {
        if (guiGlobalPadSettings->isVisible() == false)
        {
            globalSettingsButton->setToggleState(true, dontSendNotification);
            setGlobalPadSettingsDisplay();
        }
        
        return true;

    }
    else if (info.commandID == CommandIDs::ViewSequenceSettings)
    {
        if (guiGlobalPadSettings->isVisible())
        {
            globalSettingsButton->setToggleState(false, dontSendNotification);
            setGlobalPadSettingsDisplay();
        }

        guiSequencerMode->changeView(3);
        return true;
    }
    else if (info.commandID == CommandIDs::ViewScenePresets)
    {
        guiPadLayout->selectAllPads();
        toolbox->setCurrentTabIndex(-1);
        return true;
    }
    else if (info.commandID == CommandIDs::EnabledPadContentsDisplay)
    {
        if (StoredSettings::getInstance()->padContentDisplay == 2)
            StoredSettings::getInstance()->padContentDisplay = 1;
        else
            StoredSettings::getInstance()->padContentDisplay = 2;
        
        StoredSettings::getInstance()->flush();
        
        for (int i = 0; i < 48; i++)
            guiPadLayout->setPadDisplay(i);
        
        return true;
    }
    else if (info.commandID == CommandIDs::DisablePressureFeedback)
    {
        AppSettings::Instance()->setDisablePressureFeedback(!AppSettings::Instance()->getDisablePressureFeedback());
        return true;
    }
    
	return false;
}


ApplicationCommandManager* MainComponent::getCommandManager()
{
    return commandManager;
}


void MainComponent::changeLookAndFeel()
{
    alphaLiveLookAndFeel.setTheme (StoredSettings::getInstance()->interfaceTheme);
    
    guiSequencerMode->drawDrawableButtons();
    toolbox->setTabColour();
    preferencesComponent->setTabColour();
    preferencesComponent->redrawAudioSettingsComponent();
    projectSettingsComponent->setTabColour();
    
    sendLookAndFeelChange();
}

