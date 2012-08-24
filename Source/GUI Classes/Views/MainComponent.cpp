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
#include "../Binary Data/BinaryDataNew.h"


#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)



//==============================================================================
MainComponent::MainComponent(AlphaLiveEngine &ref, AppDocumentState &ref2, DocumentWindow* owner_)
                        :   alphaLiveEngineRef(ref),
                            appDocumentStateRef(ref2),
                            owner(owner_)
{
    infoTextBox = nullptr;
    
    //language/localisation stuff
    setLocalisation();
    
    //========command manager stuff==================
	commandManager->registerAllCommandsForTarget (this);
    
    //attach this class to the subject class
    appDocumentStateRef.attach(this);
    
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
    //guiSequencerMode->setInterceptsMouseClicks(false, true);
    addChildComponent(guiControllerMode = new GuiControllerMode(*this));
    addChildComponent(guiGlobalPadSettings = new GuiGlobalPadSettings(*this));
    
    
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
    
    //create gain slider
    //addAndMakeVisible(gainSlider = new AlphaRotarySlider((225 * (M_PI / 180)), (530 * (M_PI / 180)), 81));
	//gainSlider->setRotaryParameters((225 * (M_PI / 180)), (530 * (M_PI / 180)),true);
    addAndMakeVisible(gainSlider = new AlphaRotarySlider((225 * (M_PI / 180)), (495 * (M_PI / 180)), 81));
	gainSlider->setRotaryParameters((225 * (M_PI / 180)), (495 * (M_PI / 180)),true);
    gainSlider->setRange(0.0, 2.0);
    gainSlider->addListener(this);
	gainSlider->setValue(1.0, true);
    gainSlider->addMouseListener(this, true);
	
	//create pan slider
    addAndMakeVisible(panSlider = new AlphaRotarySlider((225 * (M_PI / 180)), (495 * (M_PI / 180)), 65));
	panSlider->setRotaryParameters((225 * (M_PI / 180)), (495 * (M_PI / 180)),true);
    panSlider->setRange(0.0, 1.0);
    panSlider->addListener(this);
    panSlider->setValue(0.5, true);
    panSlider->addMouseListener(this, false);
    
    //gain and pan label
    addAndMakeVisible(gainPanValueLabel = new Label("gain and pan label", "0.5555"));
    gainPanValueLabel->setJustificationType(Justification::horizontallyCentred);
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
    addAndMakeVisible(toolbox = new Toolbox());
    toolbox->addMouseListener(this, true);
    
    //pop up views
    addChildComponent(aboutComponent = new AboutComponent(*this));
    addChildComponent(preferencesComponent = new PreferencesComponent(*this, alphaLiveEngineRef));
    addChildComponent(projectSettingsComponent = new ProjectSettingsComponent(*this, alphaLiveEngineRef, appDocumentStateRef));
    
    //info box
    addAndMakeVisible(infoTextBox = new TextEditor());
    infoTextBox->setMultiLine(true);
    infoTextBox->setReadOnly(true);
    infoTextBox->setColour(TextEditor::backgroundColourId, Colours::black.withAlpha(1.0f));
    infoTextBox->setColour(TextEditor::outlineColourId, Colours::transparentBlack);
    infoTextBox->setCaretVisible(false);
    Font infoFont(11, Font::plain);
    infoTextBox->setFont(infoFont);
    infoTextBox->addMouseListener(this, true);
    infoTextBox->setPopupMenuEnabled(false);

    isInfoBoxEnabled = true;
    isClockRunning = false;

    noPadsSelected = 1;
	noModeSelected = 1;
    
    modeOffButton->setVisible(false);
    modeMidiButton->setVisible(false);
    modeSamplerButton->setVisible(false);
    modeSequencerButton->setVisible(false);
    modeControllerButton->setVisible(false);
    globalSettingsButton->setVisible(false);
    toolbox->setVisible(false); //or maybe it links to projects directory?
    midiPiano->setActive(false);
    
}


MainComponent::~MainComponent()
{
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
    
    infoTextBox->setBounds(0, getHeight()-30, getWidth(), 30);
    
    pivotX = guiPadLayout->getX() + (guiPadLayout->getWidth() * 0.5);
	pivotY = guiPadLayout->getY() + (guiPadLayout->getHeight() * 0.5);
	offskew = guiPadLayout->getY();
    
    
    aboutComponent->setBounds(0, 0, getWidth(), getHeight());
    preferencesComponent->setBounds(0, 0, getWidth(), getHeight());
    projectSettingsComponent->setBounds(0, 0, getWidth(), getHeight());
	
	gainSlider->setBounds(38, 8, 81, 81);
	panSlider->setBounds(46, 16, 65, 65);
    gainPanValueLabel->setBounds(54, 24, 49, 49);
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
            gainSlider->setValue(AppSettings::Instance()->getGlobalGain(), false); 
            panSlider->setValue(AppSettings::Instance()->getGlobalPan(), false);
            
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
    }
    
    else if (slider == panSlider)
    {
        AppSettings::Instance()->setGlobalPan(panSlider->getValue());
        
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
       
		guiMidiMode->setVisible(false);
		guiSamplerMode->setVisible(false);
		guiSequencerMode->setVisible(false);
		guiControllerMode->setVisible(false);
		
		if (button->getToggleState()==true) 
        {
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
		}
        
		else 
        {
			
			if (modeMidiButton->getToggleState()==true)
				guiMidiMode->setVisible(true);
				
			else if (modeSamplerButton->getToggleState()==true)
				guiSamplerMode->setVisible(true);
				
			else if (modeSequencerButton->getToggleState()==true)
				guiSequencerMode->setVisible(true);
				
			else if (modeControllerButton->getToggleState()==true)
				guiControllerMode->setVisible(true);
            
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
		
	}
    
    
    //==============================================================================
    
    
    //==============================================================================
    //===SCENES, LOADING & SAVING - now handled by the command manager below!======
    //==============================================================================
    
    //=================Now handled by the command manager======================
    
   
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
		
        repaint();
    }
    else if (selectedPads_.size() > 0 && selectedPads.size() == 0) //if previously there were no pads selected,
                                                            //but now is.
    {
        noPadsSelected = 0;
		modeOffButton->setVisible(true);
		modeMidiButton->setVisible(true);
		modeSamplerButton->setVisible(true);
		modeSequencerButton->setVisible(true);
		modeControllerButton->setVisible(true);
		globalSettingsButton->setVisible(true);
        toolbox->setVisible(true); //or maybe it links to projects directory?
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
        
        //set other things
        //padNumberDisplayLabel->setText("Pad " + String(currentlySelectedPad+1) + " Selected", false);
        /*
        pressureSensitivityMenu->setSelectedId(PAD_SETTINGS->getPressureSensitivityMode(), true);
        exclusiveModeButton->setToggleState(PAD_SETTINGS->getExclusiveMode(), false);
        exclusiveGroupSlider->setValue(PAD_SETTINGS->getExclusiveGroup(), false);
        quantizeModeButton->setToggleState(PAD_SETTINGS->getQuantizeMode(), false);
        
        if (exclusiveModeButton->getToggleState() == true)
            exclusiveGroupSlider->setVisible(true);
        else
            exclusiveGroupSlider->setVisible(false);
        */
        
    }
    else if (MULTI_PADS)
    {
        /*
        //set to a default settings
        setToOffMode();
        modeOffButton->setToggleState(true, false);
        pressureSensitivityMenu->setSelectedId(2, true);
        exclusiveModeButton->setToggleState(0, false);
        exclusiveGroupSlider->setValue(1, false);
        exclusiveGroupSlider->setVisible(false);
        quantizeModeButton->setToggleState(0, false);
         
         */
        
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
        
        
        /*
        int pressureSens_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getPressureSensitivityMode();
        
        //loop through all selected pads expect for the first one
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            //if setting of this pad does NOT match setting of last pad, set default and break
            if (PAD_SETTINGS->getPressureSensitivityMode() != pressureSens_)
            {
                pressureSensitivityMenu->setSelectedId(0, true);
                break;
            }
            //if this is the last 'natural' iteraction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
                pressureSensitivityMenu->setSelectedId(pressureSens_, true);
        }
        
        
        int exclusiveMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getExclusiveMode();
        
        //loop through all selected pads expect for the first one
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            //if setting of this pad does NOT match setting of last pad, set default and break
            if (PAD_SETTINGS->getExclusiveMode() != exclusiveMode_)
            {
                exclusiveModeButton->setToggleState(0, false);
                exclusiveGroupSlider->setVisible(false);
                break;
            }
            //if this is the last 'natural' iteraction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
            {
                exclusiveModeButton->setToggleState(exclusiveMode_, false);
                if (exclusiveModeButton->getToggleState() == true)
                    exclusiveGroupSlider->setVisible(true);
                else
                    exclusiveGroupSlider->setVisible(false);
            }
        }
        
        int exclusiveGroup_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getExclusiveGroup();
        
        //loop through all selected pads expect for the first one
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            //if setting of this pad does NOT match setting of last pad, set default and break
            if (PAD_SETTINGS->getExclusiveGroup() != exclusiveGroup_)
            {
                exclusiveGroupSlider->setValue(1, false);
                break;
            }
            //if this is the last 'natural' iteraction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
                exclusiveGroupSlider->setValue(exclusiveGroup_, false);
        }
        
        
        int quantizeMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getQuantizeMode();
        
        //loop through all selected pads expect for the first one
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            //if setting of this pad does NOT match setting of last pad, set default and break
            if (PAD_SETTINGS->getQuantizeMode() != quantizeMode_)
            {
                quantizeModeButton->setToggleState(0, false);
                break;
            }
            //if this is the last 'natural' iteraction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
                quantizeModeButton->setToggleState(quantizeMode_, false);
        }
      */
        
        
        
        
        //how i should I handle this?
        /*
        if (globalSettingsButton->getToggleState() == true)
        {
            
        }
         */
        
        
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
    
    /*
    if (comboBox == pressureSensitivityMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPressureSensitivityMode(pressureSensitivityMenu->getSelectedId());
        }
    }
     */
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
    globalSettingsButton->setEnabled(false);
    globalSettingsButton->setAlpha(0.3f);
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
    
    globalSettingsButton->setEnabled(true);
    globalSettingsButton->setAlpha(1.0f);
    midiPiano->setActive(true);
    midiPiano->updateDisplay();
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
    
    globalSettingsButton->setEnabled(true);
    globalSettingsButton->setAlpha(1.0f);
    midiPiano->setActive(false);
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
    
    globalSettingsButton->setEnabled(true);
    globalSettingsButton->setAlpha(1.0f);
    //setting the midi piano state is done within
    //updateDisplay() of guiSequencerMode,
    //as it depends on states within that object
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
    
    globalSettingsButton->setEnabled(true);
    globalSettingsButton->setAlpha(1.0f);
    midiPiano->setActive(false);
	noModeSelected = 0;
	repaint();
}


AlphaLiveEngine& MainComponent::getAlphaLiveEngineRef()
{
    return alphaLiveEngineRef;
}

GuiPiano* MainComponent::getGuiPiano()
{
    return midiPiano;
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
        setInfoTextBoxText (translate("Off Mode Button. Click this button to turn off the selected pads."));
        
    }
    else if (e.eventComponent == modeMidiButton)
    {
        setInfoTextBoxText (translate("MIDI Mode Button. Click this button to set the selected pads to MIDI Mode, which enables the pads to send MIDI data. The resultantly displayed controls can then be used to program the MIDI functionality of the AlphaSphere device."));
    }
    else if (e.eventComponent == modeSamplerButton)
    {
        setInfoTextBoxText (translate("Sampler Mode Button.  Click this button to set the selected pads to Sampler Mode. This enables audio files to be triggered, looped, and manipulated."));
    }
    else if (e.eventComponent == modeSequencerButton)
    {
        setInfoTextBoxText (translate("Sequencer Mode Button. Click this button to set the selected pads to Sequencer Mode. This enables sequences of midi data or audio samples to be created, looped and manipulated."));
    }
    else if (e.eventComponent == modeControllerButton)
    {
        setInfoTextBoxText (translate("Controller Mode Button. Click this button to set the selected pads to Controller Mode. This allows a set of other functionalities to be a applied to the pads."));
    }
    else if (globalSettingsButton->isMouseOver(true))
    {
        setInfoTextBoxText (translate("Global Pad Settings Button. Displays a set of pad settings which are not specific to any of the pad modes."));
    }
    else if (e.eventComponent == openButton)
    {
        setInfoTextBoxText (translate("Load Project. Allows a set of scenes to be loaded into the application."));
    }
    else if (e.eventComponent == saveButton)
    {
        setInfoTextBoxText (translate("Save Project. Allows a set of scenes to be saved to disk."));
    }
        else if (sceneComponent->isMouseOver(true))
    {
        setInfoTextBoxText (translate("Scenes. AlphaLive contains 20 'scene' slots which can each hold a full set of pad settings. Click on a scene to load up its set of settings; right-click to import, export or clear a scenes; or shift-click to copy the currently select scene to the clicked one. AlphaLive scene files (.alphascene) can be imported via drag-and-drop too."));
    }
    else if (gainSlider->isMouseOver(true)==true)
    {
        setInfoTextBoxText (translate("Global Gain Control. Sets an displays the gain of the overall output audio signal."));
    }
    else if (panSlider->isMouseOver(true)==true)
    {
        setInfoTextBoxText (translate("Global Pan Control. Sets and displays the stereo positioning of the overall output audio signal."));
    }
    /*
    else if (e.eventComponent == padRotate)
    {
        setInfoTextBoxText ("Pad Display Rotation Knob. Allows the central pad display to be rotated.");
    }
    */
    else if (e.eventComponent == infoTextBox)
    {
        setInfoTextBoxText (translate("Info Text Box. Hover the mouse over a control to view a description of what the control does here. It can be disabled using the option in the 'Controls' menu bar menu."));
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
    else if (toolbox->isMouseOver(true))
    {
        setInfoTextBoxText (translate("Toolbox. This component will display a set of items that can be applied to the selected pads based on their mode. This includes items such as presets, audio samples, scales/notational arrangements, and sequence arrangements."));
    }
    
    else if (midiPiano->isMouseOver(true))
    {
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getMode() == 1) //midi mode
        {
            setInfoTextBoxText (translate("MIDI Note Selector. Use this piano to select the MIDI notes of the selected pads. Use a regular click to select a single note for all selected pads, or cmd-click (Mac) or ctrl-click (Windows) to select multiple notes to apply to a set of pads. When selecting multiple notes, the order of selected notes will be applied to the pads in numerical order."));
        }
        else if (PAD_SETTINGS->getMode() == 3) //sequencer mode
        {
            setInfoTextBoxText (translate("MIDI Note Selector. Use this piano to select the MIDI notes of the sequencer grid for the selected pads. Cmd-click (Mac) or ctrl-click (Windows) to select the set of notes, which will be applied in numerical order, and use a regular click to set the root note. If less than 12 notes are selected no notes will be applied to the remaining rows of the grid."));
        }
    }
    
    
}

//called whenever a the mouse exits a component which has a MouseListener attached to it
void MainComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    setInfoTextBoxText (String::empty);
}



void MainComponent::setLocalisation()
{
    
    static String countryCode = SystemStats::getDisplayLanguage();
    
    //countryCode will equal ISO 639-1 or ISO 639-2 codes as listed here:
    //http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
    
    if (countryCode == "de" || countryCode == "deu") //german
    {
        File transFile("/Users/Liam/Desktop/AlphaSphere Software Dev/AlphaLive NEW vDEV/Translation files/ger_trans");
        trans = new LocalisedStrings (transFile);
        LocalisedStrings::setCurrentMappings(trans);
        
        alphaLiveLookAndFeel.setDefaultSansSerifTypefaceName(translate("FontToRenderThisLanguageIn"));
    }
    else if (countryCode == "ja" || countryCode == "jpn") //japanese
    {
        File transFile ("/Users/Liam/Desktop/AlphaSphere Software Dev/AlphaLive NEW vDEV/Translation files/jap_trans");
        trans = new LocalisedStrings (transFile);
        LocalisedStrings::setCurrentMappings(trans);
        alphaLiveLookAndFeel.setDefaultSansSerifTypefaceName(translate("FontToRenderThisLanguageIn"));
        
    }
    else //english
    {
        LocalisedStrings::setCurrentMappings(0);
        alphaLiveLookAndFeel.setDefaultSansSerifTypefaceName(translate("FontToRenderThisLanguageIn")); //will set to fallback
    }
    
    
    //commandManager->commandStatusChanged();
    //owner->repaint();
     
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
		result.setInfo (translate("Disable Help Box"),
						"Disables the help box to reduce CPU usage.",
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
    
	else
		return false;
	
	return true;
	
}


ApplicationCommandManager* MainComponent::getCommandManager()
{
    return commandManager;
}

