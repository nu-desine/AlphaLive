//
//  GuiSamplerMode.cpp
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

#include "GuiSamplerMode.h"
#include "../../File and Settings/AppSettings.h"
#include "../Binary Data/BinaryDataNew.h"
#include "GlobalValues.h"
#include "../../Application/CommonInfoBoxText.h"
#include "MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GuiSamplerMode::GuiSamplerMode(MainComponent &ref)
: mainComponentRef(ref)
										
{
    controlDisplayId = 0;
    
    //----------------quantise button-------------------
	
	Image *quantiseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::quantiseicon_png, BinaryDataNew::quantiseicon_pngSize));
	addAndMakeVisible(quantiseButton = new ModeButton(quantiseIcon));
	quantiseButton->setClickingTogglesState(true);
	quantiseButton->setToggleState(false, false);	
	quantiseButton->addListener(this);
	quantiseButton->addMouseListener(this, true);
	quantiseButton->setOpaque(false);
	
	
	//----------------trigger settings button-------------------
	
	Image *triggerSettingsImage = new Image(ImageCache::getFromMemory(BinaryDataNew::triggersettingsicon_png, BinaryDataNew::triggersettingsicon_pngSize));
	addAndMakeVisible(triggerSettingsButton = new ModeButton(triggerSettingsImage));
	triggerSettingsButton->setRadioGroupId (1234);
	triggerSettingsButton->setClickingTogglesState(true);
	triggerSettingsButton->setToggleState(false, false);	
	triggerSettingsButton->addListener(this);
	triggerSettingsButton->addMouseListener(this, true);
	triggerSettingsButton->setOpaque(false);
    triggerSettingsButton->setToggleState(true, false);
	
	//----------------pressure settings button-------------------
	
	Image *pressureSettingsImage = new Image(ImageCache::getFromMemory(BinaryDataNew::pressuresettingsicon_png, BinaryDataNew::pressuresettingsicon_pngSize));
	addAndMakeVisible(pressureSettingsButton = new ModeButton(pressureSettingsImage));
	pressureSettingsButton->setRadioGroupId (1234);
	pressureSettingsButton->setClickingTogglesState(true);
	pressureSettingsButton->setToggleState(false, false);	
	pressureSettingsButton->addListener(this);
	pressureSettingsButton->addMouseListener(this, true);
	pressureSettingsButton->setOpaque(false);

    
    addAndMakeVisible (waveform = new DemoThumbnailComp());
	waveform->setInterceptsMouseClicks(false, false);
    
    
    fileChooser = new FilenameComponent ("audiofile",
										 File::nonexistent,
										 false, false, false,
										 "*.wav; *.aif; *.aiff",
										 String::empty,
										 translate("(choose a WAV or AIFF file)"));
	fileChooser->addListener (this);					
	fileChooser->setBrowseButtonText ("+");
	fileChooser->setMaxNumberOfRecentFiles (20);
	addAndMakeVisible (fileChooser);
    fileChooser->addMouseListener(this, true);
    
    //--------------- gain slider-------------------
	addAndMakeVisible(gainSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	gainSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	gainSlider->setRange(0.0, 2.0, 0.01);
    gainSlider->addListener(this);
    gainSlider->setValue(1.0, dontSendNotification);
    gainSlider->addMouseListener(this, true);
	
	//--------------- pan slider -------------------
	addChildComponent(panSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	panSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	panSlider->setRange(0.0, 1.0, 0.01);
    panSlider->addListener(this);
    panSlider->setValue(0.5, dontSendNotification);
    panSlider->addMouseListener(this, true);
    
    //--------------- attack slider -------------------
	addChildComponent(attackSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	attackSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	attackSlider->setRange(0.0, 20.0, 0.01);
    attackSlider->setSkewFactor(0.4);
    attackSlider->addListener(this);
    attackSlider->setValue(0.01, dontSendNotification);
    attackSlider->addMouseListener(this, true);
    
    //--------------- release slider -------------------
	addChildComponent(releaseSlider = new AlphaRotarySlider((240 * (M_PI / 180)), (480 * (M_PI / 180)), 82));
	releaseSlider->setRotaryParameters((240 * (M_PI / 180)), (480 * (M_PI / 180)),true);
	releaseSlider->setRange(0.0, 20.0, 0.01);
    releaseSlider->setSkewFactor(0.4);
    releaseSlider->addListener(this);
    releaseSlider->setValue(0.01, dontSendNotification);
    releaseSlider->addMouseListener(this, true);
    
    //---------------------- plus and minus buttons ------------------
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
    
    //---------------------- parameter value label ------------------
    addAndMakeVisible(parameterLabel = new Label());
	parameterLabel->setFont(Font(9));
	parameterLabel->setText("1", false);
    parameterLabel->setJustificationType(Justification::centred);
    //parameterLabel->setEditable(false, true, true);
    //parameterLabel->addListener(this);

    
    //----------------------Trigger mode buttons------------------
    Image *standardIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::standardicon_png, BinaryDataNew::standardicon_pngSize));
	Image *toggleIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::toggleicon_png, BinaryDataNew::toggleicon_pngSize));
	Image *latchIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::latchicon_png, BinaryDataNew::latchicon_pngSize));
	Image *triggerIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::triggericon_png, BinaryDataNew::triggericon_pngSize));
	
    for (int i = 0; i < 4; i++)
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
		
        
        triggerModeButtons[i]->addListener(this);
        triggerModeButtons[i]->setOpaque(false);
        triggerModeButtons[i]->setRadioGroupId (43);
        triggerModeButtons[i]->addMouseListener(this, false);
        addAndMakeVisible(triggerModeButtons[i]);
    }
    
    triggerModeButtons[1]->setToggleState(true, false);
    
	Image *loopIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::loopicon_png, BinaryDataNew::loopicon_pngSize));
    addAndMakeVisible(loopButton =new ModeButton(loopIcon));
    loopButton->addListener(this);
    loopButton->addMouseListener(this, true);
    loopButton->setClickingTogglesState(true);
    loopButton->setToggleState(1, false);
    
    Image *destructIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::indestructableicon_png, BinaryDataNew::indestructableicon_pngSize));
	addChildComponent(indestructibleButton = new ModeButton(destructIcon));
    indestructibleButton->addListener(this);
    indestructibleButton->addMouseListener(this, true);
    indestructibleButton->setClickingTogglesState(true);
    indestructibleButton->setToggleState(0, false);
    
	Image *finishIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::finishicon_png, BinaryDataNew::finishicon_pngSize));
    addAndMakeVisible(finishLoopButton =new ModeButton(finishIcon));
    finishLoopButton->addListener(this);
    finishLoopButton->addMouseListener(this, true);
    finishLoopButton->setClickingTogglesState(true);
    finishLoopButton->setToggleState(0, false);
    
    addChildComponent(fxDial = new GuiFxDial(mainComponentRef));
	fxDial->setInterceptsMouseClicks(false, true);
    fxDial->addMouseListener(this, false);
    
	Image *stickyIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::stickyicon_png, BinaryDataNew::stickyicon_pngSize));
    addChildComponent(stickyButton = new ModeButton(stickyIcon));
    stickyButton->addListener(this);
    stickyButton->addMouseListener(this, true);
    stickyButton->setClickingTogglesState(true);
    stickyButton->setToggleState(0, false);
    
    
    //---------------status off bg-------------------------------------
    addAndMakeVisible(notSelected = new GuiCircleBackground());
	//notSelected->setInterceptsMouseClicks(false, false);
	notSelected->setVisible(false);
	
	//---------------pressure status button-------------------------------------
    
    addAndMakeVisible(pressureStatusButton = new GuiSwitch());
    pressureStatusButton->addListener(this);
    pressureStatusButton->setClickingTogglesState(true);
    pressureStatusButton->setToggleState(true, false);
    pressureStatusButton->addMouseListener(this, false);
    
    
    addAndMakeVisible(currentParameterLabel = new Label());
	currentParameterLabel->setFont(Font(10));
	currentParameterLabel->setText(translate("TEMPO"), false);
    currentParameterLabel->setColour(Label::textColourId, Colours::white);
    currentParameterLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
    currentParameterLabel->setJustificationType(Justification::centred);
    //currentParameterLabel->setEditable(false, false, false);
    
}


GuiSamplerMode::~GuiSamplerMode()
{
    
    //deleteAllChildren();
    
}


void GuiSamplerMode::resized()
{
    notSelected->setBounds(0, 0, getWidth(), getHeight());
	pressureStatusButton->setBounds(816, 393, 58, 58);
	
    waveform->setBounds(683, 261, 324, 324);
	
    //fileChooser->setBounds(787, 393, 116, 80);
    fileChooser->setBounds(787, 468, 96, 20);
    
    currentParameterLabel->setBounds(821, 398, 48, 48);
    plusButton->setBounds(802, 379, 86, 86);
    minusButton->setBounds(802, 379, 86, 86);
	parameterLabel->setBounds(832,453,26,10);
    gainSlider->setBounds(804, 381, 82, 82);
    panSlider->setBounds(804, 381, 82, 82);
    attackSlider->setBounds(804, 381, 82, 82);
    releaseSlider->setBounds(804, 381, 82, 82);
	
	quantiseButton->setBounds(681, 288,32, 32);
	triggerSettingsButton->setBounds(789, 221,42, 42);
	pressureSettingsButton->setBounds(847, 219,42, 42);
	
	triggerModeButtons[0]->setBounds(728, 305, 234, 234);
	triggerModeButtons[1]->setBounds(728, 305, 234, 234);
	triggerModeButtons[2]->setBounds(728, 305, 234, 234);
	triggerModeButtons[3]->setBounds(728, 305, 234, 234);
	
	finishLoopButton->setBounds(894, 472,32, 32);
	indestructibleButton->setBounds(853, 496,32, 32);
	loopButton->setBounds(918, 431,32, 32);
	
	stickyButton->setBounds(853, 496,32, 32);
    fxDial->setBounds(683, 261, 324, 324);

}

void GuiSamplerMode::paint (Graphics& g)
{
    float xBrowseButton = 787 + (116 * 0.25);
	float yBrowseButton = 393;
	float widthBrowseButton = 58;
	
	
	ColourGradient fillGradient(AlphaColours::nearlyblack,845 , 461, Colours::black, 845 , 383, false);
	g.setGradientFill(fillGradient);
	
	g.fillEllipse(802, 379, 86, 86);
	
	g.setColour(Colours::black);
	
	g.fillEllipse(786,218, 48, 48);
	g.fillEllipse(844,216, 48, 48);
	
	g.fillEllipse(678,285, 38, 38);
	g.fillEllipse(850,493, 38, 38);
	
	if(triggerSettingsButton->getToggleStateValue()==true)
	{
        g.fillEllipse(891,469, 38, 38);
        g.fillEllipse(915,428, 38, 38);
		
		Path pieSeg;
        pieSeg.addPieSegment(802, 379, 86, 86, (125 * (M_PI / 180)), (235 * (M_PI / 180)), 0.2f);
        g.fillPath(pieSeg);
	}
	
	
	
	g.fillEllipse(xBrowseButton, yBrowseButton, widthBrowseButton, widthBrowseButton);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	
	g.drawEllipse(678,285, 38, 38, 1.0);
	g.drawEllipse(850,493, 38, 38, 1.0);
	
	if(triggerSettingsButton->getToggleStateValue()==true)
	{
        g.setColour(Colours::black);
		g.fillEllipse(816, 393, 58, 58);
		
		ColourGradient fillGradient(AlphaColours::blue, 816+(58*0.5), 393+(58*0.6), AlphaColours::lightblue, 816+(58*0.5), 393, false);
		g.setGradientFill(fillGradient);
        
		g.fillEllipse((816+(58*0.15)), (393+(58*0.15)), (58*0.7), (58*0.7));
		
	}
	
    g.setColour(Colours::grey.withAlpha(0.3f));
    
	g.drawEllipse((xBrowseButton + (widthBrowseButton * 0.1)), (yBrowseButton + (widthBrowseButton * 0.1)),
				  (widthBrowseButton * 0.8),(widthBrowseButton * 0.8), 1.0f);
}




void GuiSamplerMode::comboBoxChanged (ComboBox* comboBox)
{
    


}


void GuiSamplerMode::sliderValueChanged (Slider* slider)
{
    if (slider == gainSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerGain(slider->getValue());
        }
        
		setParameterLabelText(String(gainSlider->getValue()));
    }
    
	else if (slider == panSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerPan(slider->getValue());
        }
        
		setParameterLabelText(String(panSlider->getValue()));
    }
    
    else if (slider == attackSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerAttackTime(slider->getValue());
        }
        
		setParameterLabelText(String(attackSlider->getValue()));
    }
    else if (slider == releaseSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerReleaseTime(slider->getValue());
        }
        
		setParameterLabelText(String(releaseSlider->getValue()));
    }

}


void GuiSamplerMode::filenameComponentChanged (FilenameComponent* filenameComponent)
{
    if (filenameComponent == fileChooser)
    {
        // this is called when the user changes the filename in the file chooser box
        File audioFile (filenameComponent->getCurrentFile().getFullPathName());
        
        if(audioFile.existsAsFile())
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSamplerAudioFilePath(audioFile);
				
            }
            
            setAudioFileDisplay(audioFile);
            
        }
        
        else
        {
            AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                         "AlphaLive",
                                         translate("Couldn't open file!"));
        }	
    }
    
}





void GuiSamplerMode::buttonClicked (Button* button)
{
    if(button == triggerSettingsButton)
		setDisplay(1);        
    
	
	else if(button == pressureSettingsButton)
        setDisplay(2);
    
    else if(button == quantiseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setQuantizeMode(button->getToggleState());
        }
    }
    
    else if (button == loopButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerShouldLoop(button->getToggleState());
        }
    }
    
    else if (button == indestructibleButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerIndestructible(button->getToggleState());
        }
    }
    
    else if (button == finishLoopButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerShouldFinishLoop(button->getToggleState());
        }
    }
    else if (button == stickyButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setSamplerSticky(button->getToggleState());
        }
    }
    
    
    else if(button == pressureStatusButton)
	{
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            
            if (button->getToggleState() == false)
            {
                PAD_SETTINGS->setSamplerEffect(0);
            }
            else if (button->getToggleState() == true)
            {
                PAD_SETTINGS->setSamplerEffect(1); //set default effect
                fxDial->updateDisplay();
            }
                
            
            //PAD_SETTINGS->setSamplerPressureStatus(button->getToggleState());
        }
        
        if(pressureStatusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);
		
	}
    
    
    //triggerMode buttons
    for (int trig = 0; trig < 4; trig++)
    {
        if (button == triggerModeButtons[trig])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSamplerTriggerMode(trig+1);
            }
            
            break;
        }
    }
    
    if (button == plusButton || button == minusButton)
	{
        if (button == plusButton)
            controlDisplayId++;
        else if (button == minusButton)
            controlDisplayId--;
        
        if (controlDisplayId > 3)
            controlDisplayId = 0;
        else if (controlDisplayId < 0)
            controlDisplayId = 3;
        
        setRotaryControlDisplay();
	}
}

void GuiSamplerMode::setAudioFileDisplay(File file)
{
    waveform->setFile (file);
    
    //can't use 'file' below as this will be the original file, not the copied file in the projects directory
    fileChooser->setCurrentFile (AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerAudioFilePath(), 
                                 true, 
                                 false);
}

void GuiSamplerMode::setDisplay(int settingsType)
{
    if (settingsType == 1) //trigger settings
    {
        
        waveform->setVisible(true);
        fileChooser->setVisible(true);
        
        for (int i = 0; i < 4; i++)
            triggerModeButtons[i]->setVisible(true);
        
        loopButton->setVisible(true);
        indestructibleButton->setVisible(true);
        finishLoopButton->setVisible(true);
        
        plusButton->setVisible(true);
        minusButton->setVisible(true);
        parameterLabel->setVisible(true);
        currentParameterLabel->setVisible(true);
        
        fxDial->setVisible(false);
        pressureStatusButton->setVisible(false);
        notSelected->setVisible(false);
        stickyButton->setVisible(false);
        
        repaint(750, 200, 274, 469); 
        
    }
    
    else if (settingsType == 2) //pressure settings
    {
        
        waveform->setVisible(false);
        fileChooser->setVisible(false);
        
        for (int i = 0; i < 4; i++)
            triggerModeButtons[i]->setVisible(false);
        
        loopButton->setVisible(false);
        indestructibleButton->setVisible(false);
        finishLoopButton->setVisible(false);
        
        plusButton->setVisible(false);
        minusButton->setVisible(false);
        parameterLabel->setVisible(false);
        currentParameterLabel->setVisible(false);
        
        fxDial->setVisible(true);
        pressureStatusButton->setVisible(true);
        stickyButton->setVisible(true);
        
        if(pressureStatusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);
        
        repaint(750, 200, 274, 469);
        
        
    }
    
    setRotaryControlDisplay();
    
}


void GuiSamplerMode::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    fxDial->setCurrentlySelectedPad(selectedPads);
}

void GuiSamplerMode::updateDisplay()
{
    
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as to hide/dissabled any unneeded components. 
    
    //if an individual pad number is currently selected
    if (SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        fileChooser->setCurrentFile(PAD_SETTINGS->getSamplerAudioFilePath(), true, false);
		File audioFile(PAD_SETTINGS->getSamplerAudioFilePath());
		waveform->setFile (audioFile);
		
        gainSlider->setValue(PAD_SETTINGS->getSamplerGain(), dontSendNotification);
        panSlider->setValue(PAD_SETTINGS->getSamplerPan(), dontSendNotification);
        attackSlider->setValue(PAD_SETTINGS->getSamplerAttackTime(), dontSendNotification);
        releaseSlider->setValue(PAD_SETTINGS->getSamplerReleaseTime(), dontSendNotification);
        quantiseButton->setToggleState(PAD_SETTINGS->getQuantizeMode(), false);
        triggerModeButtons[PAD_SETTINGS->getSamplerTriggerMode()-1]->setToggleState(true, false);
        loopButton->setToggleState(PAD_SETTINGS->getSamplerShouldLoop(), false);
        indestructibleButton->setToggleState(PAD_SETTINGS->getSamplerIndestructible(), false);
        finishLoopButton->setToggleState(PAD_SETTINGS->getSamplerShouldFinishLoop(), false);
        stickyButton->setToggleState(PAD_SETTINGS->getSamplerSticky(), false);
        //effect is found a set with fxDial
        
        if (PAD_SETTINGS->getSamplerEffect() == 0)
            pressureStatusButton->setToggleState(false, false);
        else
            pressureStatusButton->setToggleState(true, false);
    }

    else if (MULTI_PADS)
    {
        /*
        //set default values
        fileChooser->setCurrentFile(File::nonexistent, false, false);
        waveform->setFile (File::nonexistent);
        //gainSlider->sliderComponent()->setValue(0.7, false);
        //panSlider->sliderComponent()->setValue(0.5, false);
        quantiseButton->setToggleState(false, false);
        triggerModeButtons[0]->setToggleState(true, false); //ideally nothing should be selected here
        loopButton->setToggleState(1, false);
        indestructibleButton->setToggleState(0, false);
        finishLoopButton->setToggleState(0, false);
        stickyButton->setToggleState(0, false);
        pressureStatusButton->setToggleState(false, false);
         */
        
        //==================================================================================================
        File file_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerAudioFilePath();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerAudioFilePath() != file_)
            {
                fileChooser->setCurrentFile(File::nonexistent, false, false);
                waveform->setFile(File::nonexistent);
                break;
            }
            if (i == selectedPads.size()-1)
            {
                fileChooser->setCurrentFile(file_, false, false);
                waveform->setFile(file_);
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
        int triggerMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerTriggerMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerTriggerMode() != triggerMode_)
            {
                for (int i = 0; i < 4; i++)
                    triggerModeButtons[i]->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                triggerModeButtons[triggerMode_-1]->setToggleState(true, false);
        }
        
        //==================================================================================================
        int indestructible_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerIndestructible();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerIndestructible() != indestructible_)
            {
                indestructibleButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                indestructibleButton->setToggleState(indestructible_, false);
        }
        
        //==================================================================================================
        int sticky_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerSticky();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerSticky() != sticky_)
            {
                stickyButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                stickyButton->setToggleState(sticky_, false);
        }
        
        //==================================================================================================
        int loop_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerShouldLoop();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerShouldLoop() != loop_)
            {
                loopButton->setToggleState(1, false);
                break;
            }
            if (i == selectedPads.size()-1)
                loopButton->setToggleState(loop_, false);
        }
        
        //==================================================================================================
        int finishLoop_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerShouldFinishLoop();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerShouldFinishLoop() != finishLoop_)
            {
                finishLoopButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                finishLoopButton->setToggleState(finishLoop_, false);
        }
        
        //==================================================================================================
        double gain_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerGain();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerGain() != gain_)
            {
                gainSlider->setValue(1.0, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                gainSlider->setValue(gain_, dontSendNotification);
        }
        
        //==================================================================================================
        double pan_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerPan();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerPan() != pan_)
            {
                panSlider->setValue(0.5, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                panSlider->setValue(pan_, dontSendNotification);
        }
        
        //==================================================================================================
        double attack_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerAttackTime();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerAttackTime() != attack_)
            {
                attackSlider->setValue(0.01, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                attackSlider->setValue(attack_, dontSendNotification);
        }
        
        //==================================================================================================
        double release_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerReleaseTime();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getSamplerReleaseTime() != release_)
            {
                releaseSlider->setValue(0.01, dontSendNotification);
                break;
            }
            if (i == selectedPads.size()-1)
                releaseSlider->setValue(release_, dontSendNotification);
        }
        
        //==================================================================================================
        //for pressure status button
        int effect_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerEffect();
        
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            int effect_2 = PAD_SETTINGS->getSamplerEffect();
            
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

    
    //set visibility of certain components
    
    notSelected->setVisible(false);
    
    if (triggerSettingsButton->getToggleState() == true)
        setDisplay(1);
    else if (pressureSettingsButton->getToggleState() == true)
        setDisplay(2);
    
    fxDial->updateDisplay();
}


void GuiSamplerMode::setParameterLabelText (String value)
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
		
		if (gainSlider->isVisible())
		{
            currentParameterLabel->setText(translate("GAIN"), false);
			parameterLabel->setText(String(gainSlider->getValue()), false);
		}
		else if (panSlider->isVisible())
		{
            currentParameterLabel->setText(translate("PAN"), false);
			parameterLabel->setText(String(panSlider->getValue()), false);
		}
        else if (attackSlider->isVisible())
		{
            currentParameterLabel->setText(translate("ATTACK"), false);
			parameterLabel->setText(String(attackSlider->getValue()), false);
		}
        else if (releaseSlider->isVisible())
		{
            currentParameterLabel->setText(translate("RELEASE"), false);
			parameterLabel->setText(String(releaseSlider->getValue()), false);
		}
	}
    
}

void GuiSamplerMode::setRotaryControlDisplay()
{
    gainSlider->setVisible(false);
    panSlider->setVisible(false);
    attackSlider->setVisible(false);
    releaseSlider->setVisible(false);
    
    if (triggerSettingsButton->getToggleState())
    {
        if (controlDisplayId == 0)
            gainSlider->setVisible(true);
        else if (controlDisplayId == 1)
            panSlider->setVisible(true);
        else if (controlDisplayId == 2)
            attackSlider->setVisible(true);
        else if (controlDisplayId == 3)
            releaseSlider->setVisible(true);
    }
    
    setParameterLabelText (String::empty);
}


void GuiSamplerMode::mouseEnter (const MouseEvent &e)
{
    // ======= param label text command =========
    // =======================================
    
	if (gainSlider->isMouseOver(true))
		setParameterLabelText(String(gainSlider->getValue()));
	else if (panSlider->isMouseOver(true))
		setParameterLabelText(String(panSlider->getValue()));
    else if (attackSlider->isMouseOver(true))
		setParameterLabelText(String(attackSlider->getValue()));
    else if (releaseSlider->isMouseOver(true))
		setParameterLabelText(String(releaseSlider->getValue()));
    
    // ======= info box text command =========
    // =======================================
    
    
    if (quantiseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::quantizeButton));
    }
    else if (triggerSettingsButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::triggerSettingsButton));
    }
    else if (pressureSettingsButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::pressureSettingsButton));
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
    else if (attackSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Attack Time. Set the audio sample attack time in seconds for selected pads."));
    }
    else if (releaseSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Release Time. Set the audio sample release time in seconds for selected pads."));
    }
    
    if (fileChooser->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Audio File Selector. Set the filepath name of the audio file for selected pads. Click the '+' button to open a File Browser Window, or use the drop-down menu to select from recently used files."));
    }
    
    else if (panSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Pan. Set the stereo positioning of the audio signal for selected pads."));
    }
    else if (gainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Gain. Sets the gain of the audio signal for the selected pads."));
    }
    
    if (plusButton->isMouseOver(true) || minusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Use these buttons to switch between a set of rotary controls above."));
    }
    
    //else if (fxDial->isMouseOver(true))
    //{
    //    mainComponentRef.setInfoTextBoxText("Sampler FX Dial. Sets and displays the audio processing effect that the selected pad/pads pressure controls.");
    //}
    
    
}

void GuiSamplerMode::mouseExit (const MouseEvent &e)
{
    
	if (e.eventComponent == gainSlider)
		setParameterLabelText(String::empty);
	else if (e.eventComponent == panSlider)
		setParameterLabelText(String::empty);
    else if (e.eventComponent == attackSlider)
		setParameterLabelText(String::empty);
    else if (e.eventComponent == releaseSlider)
		setParameterLabelText(String::empty);
    
    
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}


