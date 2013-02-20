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
#include "../Binary Data/BinaryDataNew.h"


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
    appDir = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName() + File::separatorString;
    infoBoxText = String::empty;
    
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
    
    
    //load Gui Images
    //load binary data into Image
    backgroundImage = ImageCache::getFromMemory(BinaryDataNew::interfacemain_png, BinaryDataNew::interfacemain_pngSize);
	padsOff = ImageCache::getFromMemory(BinaryDataNew::padsoff_png, BinaryDataNew::padsoff_pngSize);
	padsOn = ImageCache::getFromMemory(BinaryDataNew::padson_png, BinaryDataNew::padson_pngSize);
	modeOff = ImageCache::getFromMemory(BinaryDataNew::modeoff_png, BinaryDataNew::modeoff_pngSize);
	padsBg = ImageCache::getFromMemory(BinaryDataNew::padsbg_png, BinaryDataNew::padsbg_pngSize);
    
    
    //Mode Gui's
    addChildComponent(guiMidiMode = new GuiMidiMode(*this));
    addChildComponent(guiSamplerMode = new GuiSamplerMode(*this));
    addChildComponent(guiSequencerMode = new GuiSequencerMode(*alphaLiveEngineRef.getModeSequencer(), *this, appDocumentStateRef)); 
    addChildComponent(guiControllerMode = new GuiControllerMode(*this));
    addChildComponent(guiGlobalPadSettings = new GuiGlobalPadSettings(*this));
    addChildComponent(eliteControlsSettings = new GuiEliteControlsSettings(*this));
    
    
    //Pad Layout
	addAndMakeVisible(guiPadLayout = new GuiPadLayout(alphaLiveEngineRef, *this));
    guiPadLayout->addMouseListener(this, false);
	guiPadLayout->setInterceptsMouseClicks(false, true);
    
    
    //--------------------------------------------------------------------------
    //Mode Buttons
    //create off mode button
	Image *offModeImage = new Image(ImageCache::getFromMemory(BinaryDataNew::offsymbol_png, BinaryDataNew::offsymbol_pngSize)); 
	addAndMakeVisible(modeOffButton = new ModeButton(offModeImage));
	modeOffButton->addListener(this);
    modeOffButton->setOpaque(false);
	modeOffButton->setRadioGroupId (1234);
    modeOffButton->addMouseListener(this, false);
	
	//create global settings button
	Image *globalSettingsImage = new Image(ImageCache::getFromMemory(BinaryDataNew::padsettingssymbol_png, BinaryDataNew::padsettingssymbol_pngSize)); 
	addAndMakeVisible(globalSettingsButton = new ModeButton(globalSettingsImage));
	globalSettingsButton->addListener(this);
    globalSettingsButton->setOpaque(false);
    globalSettingsButton->addMouseListener(this, false);
    
    
    //create looper mode button
	Image *looperModeImage = new Image(ImageCache::getFromMemory(BinaryDataNew::loopsymbol_png, BinaryDataNew::loopsymbol_pngSize)); 
	addAndMakeVisible(modeSamplerButton = new ModeButton(looperModeImage));
	modeSamplerButton->addListener(this);
    modeSamplerButton->setOpaque(false);
	modeSamplerButton->setRadioGroupId (1234);
    modeSamplerButton->addMouseListener(this, false);
	
	//create midi mode button
	Image *midiModeImage = new Image(ImageCache::getFromMemory(BinaryDataNew::midisymbol_png, BinaryDataNew::midisymbol_pngSize)); 
	addAndMakeVisible(modeMidiButton = new ModeButton(midiModeImage));
	modeMidiButton->addListener(this);
    modeMidiButton->setOpaque(false);
	modeMidiButton->setRadioGroupId (1234);
    modeMidiButton->addMouseListener(this, false);
	
	//create sequencer mode button
	Image *sequencerModeImage = new Image(ImageCache::getFromMemory(BinaryDataNew::sequenceicon_png, BinaryDataNew::sequenceicon_pngSize)); 
	addAndMakeVisible(modeSequencerButton = new ModeButton(sequencerModeImage));
	modeSequencerButton->addListener(this);
    modeSequencerButton->setOpaque(false);
	modeSequencerButton->setRadioGroupId (1234);
    modeSequencerButton->addMouseListener(this, false);
	
    //createa controller mode button
	Image *controllerModeImage = new Image(ImageCache::getFromMemory(BinaryDataNew::controlsymbol_png, BinaryDataNew::controlsymbol_pngSize)); 
	addAndMakeVisible(modeControllerButton = new ModeButton(controllerModeImage));
	modeControllerButton->addListener(this);
    modeControllerButton->setOpaque(false);
	modeControllerButton->setRadioGroupId (1234);
    modeControllerButton->addMouseListener(this, false);
    
    
    //Global clock bar
    addAndMakeVisible(globalClock = new GuiGlobalClock(*this, alphaLiveEngineRef));
    globalClock->setInterceptsMouseClicks(false, true);
	
    //open/save buttons
	Image *openImage = new Image(ImageCache::getFromMemory(BinaryDataNew::loadsymbol_png, BinaryDataNew::loadsymbol_pngSize)); 
    addAndMakeVisible(openButton = new ModeButton(openImage));
	openButton->setClickingTogglesState(false);
    openButton->setCommandToTrigger(commandManager, CommandIDs::Open, false);
    openButton->addMouseListener(this, false);
    
	Image *saveImage = new Image(ImageCache::getFromMemory(BinaryDataNew::savesymbol_png, BinaryDataNew::savesymbol_pngSize)); 
    addAndMakeVisible(saveButton = new ModeButton(saveImage));
	saveButton->setClickingTogglesState(false);
    saveButton->setCommandToTrigger(commandManager, CommandIDs::Save, false);
    saveButton->addMouseListener(this, false);
    
    //scene component
    addAndMakeVisible(sceneComponent = new SceneComponent(appDocumentStateRef, *alphaLiveEngineRef.getModeController())); //pass in appDocumentStateRef so that appDocumentStateRef function calls can be made within sceneComponent
    sceneComponent->addMouseListener(this, true);
	
	//create pan slider
    addAndMakeVisible(panSlider = new AlphaRotarySlider((225 * (M_PI / 180)), (495 * (M_PI / 180)), 65));
	panSlider->setRotaryParameters((225 * (M_PI / 180)), (495 * (M_PI / 180)),true);
    panSlider->setRange(0.0, 1.0);
    panSlider->addListener(this);
    panSlider->setValue(0.5, dontSendNotification);
    panSlider->addMouseListener(this, false);
    
    //create gain slider
    //addAndMakeVisible(gainSlider = new AlphaRotarySlider((225 * (M_PI / 180)), (530 * (M_PI / 180)), 81));
	//gainSlider->setRotaryParameters((225 * (M_PI / 180)), (530 * (M_PI / 180)),true);
    addAndMakeVisible(gainSlider = new AlphaRotarySlider((225 * (M_PI / 180)), (495 * (M_PI / 180)), 81));
	gainSlider->setRotaryParameters((225 * (M_PI / 180)), (495 * (M_PI / 180)),true);
    gainSlider->setRange(0.0, 2.0);
    gainSlider->addListener(this);
	gainSlider->setValue(1.0, dontSendNotification);
    gainSlider->addMouseListener(this, true);
    
    //gain and pan label
    addAndMakeVisible(gainPanValueLabel = new Label("value label", String::empty));
    gainPanValueLabel->setJustificationType(Justification::horizontallyCentred);
    gainPanValueLabel->setColour(Label::textColourId, AlphaColours::blue);
    gainPanValueLabel->setFont(Font(12));
    gainPanValueLabel->addMouseListener(this, true);
    
    /*
     
    //killswitch button
    addAndMakeVisible(killswitchButton = new TextButton());
    killswitchButton->setColour(TextButton::buttonColourId, Colours::blue);
    killswitchButton->setCommandToTrigger(commandManager, CommandIDs::KillSwitch, false);
    killswitchButton->addMouseListener(this, true);*/
    
    
   
    /*
    addAndMakeVisible(autoShowSettingsSwitch = new GuiSwitch());
    autoShowSettingsSwitch->addListener(this);
    autoShowSettingsSwitch->setClickingTogglesState(true);
    autoShowSettingsSwitch->setToggleState(false, false);
    autoShowSettingsSwitch->addMouseListener(this, true);
     */
    
    
    /*
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
     */
    
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
    
    //pop up views
    addChildComponent(aboutComponent = new AboutComponent(*this));
    aboutComponent->setAlpha(0.975f);
    addChildComponent(preferencesComponent = new PreferencesComponent(*this, alphaLiveEngineRef));
    preferencesComponent->setAlpha(0.975f);
    addChildComponent(projectSettingsComponent = new ProjectSettingsComponent(*this, alphaLiveEngineRef, appDocumentStateRef));
    projectSettingsComponent->setAlpha(0.975f);
    
    //info box
    addAndMakeVisible(infoTextBox = new TextEditor());
    infoTextBox->setMultiLine(true);
    infoTextBox->setReadOnly(true);
    infoTextBox->setColour(TextEditor::backgroundColourId, Colours::black.withAlpha(1.0f));
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
    
}


MainComponent::~MainComponent()
{
    if (isThreadRunning())
        stopThread(500);
    
    deleteAllChildren();
    //delete blackChrome;
    
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
    
    sceneComponent->setBounds(5, 10, 20, getHeight());
    //clearPresetsButton->setBounds(7, 646, 16, 16);
    
    globalClock->setBounds(479, 0, 266, 144);
    
    infoTextBox->setBounds(0, getHeight()-45, getWidth(), 45);
    
    pivotX = guiPadLayout->getX() + (guiPadLayout->getWidth() * 0.5);
	pivotY = guiPadLayout->getY() + (guiPadLayout->getHeight() * 0.5);
	offskew = guiPadLayout->getY();
    
    
    aboutComponent->setBounds(0, 0, getWidth(), getHeight());
    preferencesComponent->setBounds(0, 0, getWidth(), getHeight());
    projectSettingsComponent->setBounds(0, 0, getWidth(), getHeight());
	
	gainSlider->setBounds(38, 8, 81, 81);
	panSlider->setBounds(46, 16, 65, 65);
    gainPanValueLabel->setBounds(54, 24, 49, 49);
    
    deviceStatusComponent->setBounds(348, -5, 75, 20);
}



void MainComponent::paint(juce::Graphics &g)
{
    //Rectangle<int> rect = g.getClipBounds();
    //std::cout << "Painting main: " << rect.getX() << " " << rect.getY() << " " << rect.getRight() << " " << rect.getBottom() << std::endl;
    
    g.setOrigin(0, 0);
	g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
    
    
    //if (rotateFlag == 1)  
    //{
	//	g.setOrigin(0, 7);
	//	g.drawImageTransformed(rotatedPadsImage, (AffineTransform::rotation ((float) (padRotate->getValue() / (180.0 / double_Pi)), getWidth()*0.5, (getHeight()-30/*-30 as the image doesn't go all the way to the bottom of the component, due to the help box now being there*/)*0.5)), false);
	//	g.setOrigin(0, 0);
    
	//}
	
	
	if (noPadsSelected == 1) 
    {
        g.drawImage(padsOff, 0, 0, getWidth(), getHeight(), 0, 0, padsOff.getWidth(), padsOff.getHeight());
        
        if (eliteControlSelected == 0)
            g.drawImage(modeOff, 0, 0, getWidth(), getHeight(), 0, 0, modeOff.getWidth(), modeOff.getHeight());
	}	
	else if (noPadsSelected == 0) 
    {
		g.drawImage(padsOn, 0, 0, getWidth(), getHeight(), 0, 0, padsOn.getWidth(), padsOn.getHeight());
		
		g.setColour(Colours::black);
		
		g.fillRect(753, 10, 261, 121);
		
		g.setColour(Colours::grey.withAlpha(0.3f));
		
		g.drawRect(753, 10, 261, 121, 2);
		
		g.setColour(Colours::black);
	}
	
	if (noModeSelected == 1) 
    {
        if (eliteControlSelected == 0)
            g.drawImage(modeOff, 0, 0, getWidth(), getHeight(), 0, 0, modeOff.getWidth(), modeOff.getHeight());
	}
	
	g.drawImage(padsBg, 0, 0, getWidth(), getHeight(), 0, 0, padsBg.getWidth(), padsBg.getHeight());
	
	//gain and pan container
	
	g.setColour(Colours::black);
	g.fillEllipse(35, 5, 87, 87);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse(35, 5, 87, 87, 1.0f);
	
    //The below code is for the blue circle for the gain/pan control. what was it meant for?
	//g.setColour(Colours::black);
	//g.fillEllipse(32, 73, 58, 58);
	
	//ColourGradient fillGradient(AlphaColours::blue, 32+(58*0.5), 73+(58*0.6), AlphaColours::lightblue, 32+(58*0.5), 73, false);
	//g.setGradientFill(fillGradient);
	
	//g.fillEllipse((32+(58*0.15)), (73+(58*0.15)), (58*0.7), (58*0.7));
	
	//g.setColour(Colours::grey.withAlpha(0.3f));
	//g.drawEllipse((32+(58*0.1)), (73+(58*0.1)), (58*0.8), (58*0.8), 1.0f);

    
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
            setCurrentlySelectedPad(selectedPads);
            
            //update GUI things (GUI of global settings) which aren't updated by setCurrentlySelectedPad
            //these things could be put in setCurrentlySelectedPad but they'll be updated everytime a pad is selected which would be inefficent
            //there is still some inefficiency in that these things will be updated everytime a sequence is loaded... sort this out?!!
            gainSlider->setValue(AppSettings::Instance()->getGlobalGain(), dontSendNotification); 
            panSlider->setValue(AppSettings::Instance()->getGlobalPan(), dontSendNotification);
            
            //set the mode colour ring of each pad
            for (int i = 0; i <= 47; i++)
                guiPadLayout->modeChange(i, AppSettings::Instance()->padSettings[i]->getMode());
            
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
        gainPanValueLabel->setText(String(slider->getValue(), 3), false);
    }
    
    else if (slider == panSlider)
    {
        AppSettings::Instance()->setGlobalPan(panSlider->getValue());
        gainPanValueLabel->setText(String(slider->getValue(), 3), false);
        
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
        buttonIndex = 0;
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setMode(buttonIndex);
            //set the Gui pads to have a ring of colour signifying the pad's current mode
            guiPadLayout->modeChange(padNum, buttonIndex);
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
            guiPadLayout->modeChange(padNum, buttonIndex);
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
            guiPadLayout->modeChange(padNum, buttonIndex);
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
            guiPadLayout->modeChange(padNum, buttonIndex);
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
            guiPadLayout->modeChange(padNum, buttonIndex);
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


//this function is called every time a user selects a pad on the GUI
void MainComponent::setCurrentlySelectedPad(Array <int> selectedPads_)
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
		
        globalSettingsButton->setToggleState(false, false);
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
                modeOffButton->setToggleState(true, false);
                break;
            }
            //if this is the last 'natural' interaction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
            {
                if (mode_ == 0) //off mode
                {
                    setToOffMode();
                    modeOffButton->setToggleState(true, false);
                }
                if (mode_ == 1) //midi mode
                {
                    setToMidiMode();
                    modeMidiButton->setToggleState(true, false);
                }
                if (mode_ == 2) //sampler mode
                {
                    setToSamplerMode();
                    modeSamplerButton->setToggleState(true, false);
                }
                if (mode_ == 3) //sequencer mode
                {
                    setToSequencerMode();
                    modeSequencerButton->setToggleState(true, false);
                }
                if (mode_ == 4) //controller mode
                {
                    setToControllerMode();
                    modeControllerButton->setToggleState(true, false);
                }
            }
        }
        
    }
    
    //is this the best way to do it? At this point the Mode GUI's would have beeen updated already,
    //but if the following statement is true those calls would have been useless.
    if (globalSettingsButton->getToggleState() == true)
        setGlobalPadSettingsDisplay();

    commandManager->commandStatusChanged(); //so that if a single pad is selected, the copy and paste settings
                                            //command is enabled
            
     
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
    
    midiPiano->setActive(true);
    midiPiano->updateDisplay();
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
     - In the run() function below it will enter the while statement and pause for either 200 ms
     or until a notification wakes up the thread. If the timeout period expires it will exit the
     loop (as gotNewTextMessage will equal false) and set the infoTextBox Text, and the thread
     will exit.
     - If setInfoTextBoxText() is called whilst the thread is running (meaning the thread is 
     currently 'waiting', notify() will be called which will wake up the thread. However this will
     cause gotNewTextMessage to equal true, so it will re-enter the loop and wait for 200 ms again.
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
        setInfoTextBoxText (translate("Scenes. AlphaLive contains 20 'scene' slots, each can hold a full set of pad settings. Click on a scene to load up its settings and pads; right-click to import, export or clear a scene; or shift-click to copy the currently select scene to the clicked scene. AlphaLive scene files (.alphascene) can also be imported via drag-and-drop."));
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
        gainPanValueLabel->setText(String(gainSlider->getValue(), 3), false);
    else if (panSlider->isMouseOver(true))
        gainPanValueLabel->setText(String(panSlider->getValue(), 3), false);
    
}

//called whenever a the mouse exits a component which has a MouseListener attached to it
void MainComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    setInfoTextBoxText (String::empty);
    
    
    if(e.eventComponent == gainSlider || e.eventComponent == panSlider)
        gainPanValueLabel->setText(String::empty, false);
        
        
}



void MainComponent::setLocalisation()
{
    static String countryCode = SystemStats::getDisplayLanguage();
    std::cout << "Language: " << countryCode << std::endl;
    
    //We may need to find suitable fonst that exists on the current system
    //for languages such as Chinese, Japanese, and Korean.
    //http://en.wikipedia.org/wiki/List_of_CJK_fonts
    
    StringArray availableFonts = Font::findAllTypefaceNames();
    
    infoBoxTextSize = 12;
    
    //countryCode will equal ISO 639-1 or ISO 639-2 codes as listed here:
    //http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
    
    if (countryCode == "de" || countryCode == "deu") //german
    {

        currentLanguage = "German";
    }
    else if (countryCode == "ja" || countryCode == "jpn") //japanese
    {
        File transFile (appDir + "Application Data" + File::separatorString + "trans_ja");
        trans = new LocalisedStrings (transFile);
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
        infoBoxTextSize = 13.5;
        
        currentLanguage = "Japanese";
        
    }
    else if (countryCode == "zh" || countryCode == "zho" || countryCode == "zh-Hant" || countryCode == "zh-Hans") //chinese. do i need the first two?
    {
        File transFile (appDir + "Application Data" + File::separatorString + "trans_zh");
        trans = new LocalisedStrings (transFile);
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
        infoBoxTextSize = 13.5;
        
        currentLanguage = "Chinese";
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
    
    //global gain
    if (command == 1)
        sliderToChange = gainSlider;
    //global pan
    else if (command == 2)
        sliderToChange = panSlider;
    
    
    //===process slider value===
    if (sliderToChange != nullptr)
    {
        double newVal = 0;
        double currentVal = sliderToChange->getValue();
        double maxValue = sliderToChange->getMaximum();
        double minValue = sliderToChange->getMinimum();
        double incremValue = 0.01; //is this value suitable?
        
        //incremented value
        if (eliteControlValue >= 1 && eliteControlValue <= 63)
            newVal = currentVal + (eliteControlValue * incremValue); //too simple?
        //decremented value
        else if (eliteControlValue >= 64 && eliteControlValue <= 127)
            newVal = currentVal - ((128 - eliteControlValue) * incremValue); //too simple?
        
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
            
            sliderToChange->setValue(newVal, dontSendNotification);
            std::cout << "New Slider Value: " << newVal << std::endl;
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
}

void MainComponent::setDeviceStatus (int status)
{
    
}

void MainComponent::openDocumentation (int type)
{
    String docDir (appDir + "Documentation" + File::separatorString);
    bool opened = true;
    
    if (currentLanguage == "Japanese")
    {
        if (type == 1) //Starter guide
            opened = File(docDir + "Test Japanese Starter Guide.pdf").startAsProcess();
        else if (type == 2) //Reference manual
            opened = File(docDir + "Test Japanese Reference Manual.pdf").startAsProcess();
    }
    else //English
    {
        if (type == 1) //Starter guide
            opened = File(docDir + "Test English Starter Guide.pdf").startAsProcess();
        else if (type == 2) //Reference manual
            opened = File(docDir + "Test English Reference Manual.pdf").startAsProcess();
    }
    
    if (opened == false) //probably due to the file not existing
        AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon, 
                                          translate("Documentation cannot be found!"), 
                                          translate("The documentation file seems to be missing from the AlphaLive application directory."));
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
        CommandIDs::ReferenceManual
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
        result.defaultKeypresses.add (KeyPress ('p', cmd|alt, 0));
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
    
    else if(info.commandID == CommandIDs::SaveScene)
	{
		sceneComponent->getSelectedSceneSlot()->saveScene();
	}
    
    else if(info.commandID == CommandIDs::LoadScene)
	{
		sceneComponent->getSelectedSceneSlot()->loadScene();
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
    
    else if(info.commandID == CommandIDs::ClearScene)
	{
		sceneComponent->getSelectedSceneSlot()->clearScene();
	}
    
    else if(info.commandID == CommandIDs::ClearAllScenes)
	{
		bool userSelection;
        userSelection = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("Are you sure?"), translate("You cannot undo this command."));
        
        if (userSelection == true)
        {
            sceneComponent->clearAll();
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
    
    else if(info.commandID == CommandIDs::StarterGuide)
    {
        openDocumentation (1);
    }
    
    else if(info.commandID == CommandIDs::ReferenceManual)
    {
        openDocumentation (2);
    }
    
	//else
	//	return false;
	
	return true;
	
}


ApplicationCommandManager* MainComponent::getCommandManager()
{
    return commandManager;
}

