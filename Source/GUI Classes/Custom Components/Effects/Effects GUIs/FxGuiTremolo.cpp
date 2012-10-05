//
//  FxGuiTremolo.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 13/07/2012.
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

#include "FxGuiTremolo.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/BinaryDataNew.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiTremolo::GuiTremolo(MainComponent &ref)
:           mainComponentRef(ref)
{
    addAndMakeVisible(depthSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	depthSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    depthSlider->setRange(0.0, 1.0);
    depthSlider->setValue(0.7, false);
    depthSlider->addListener(this);
    depthSlider->addMouseListener(this, true);
    
	addAndMakeVisible(rateSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	rateSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    rateSlider->setRange(0, 50.0);
    rateSlider->setValue(5.0, false);
    rateSlider->addListener(this);
    rateSlider->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0);
    intensitySlider->setValue(0.5, false);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaColours::lightblue);
    
    
	addAndMakeVisible(rateMenu = new AlphaPopUpButton());
    rateMenu->addListener(this);
    rateMenu->addMouseListener(this, true);
	rateMenu->setButtonText("1/4");
    /*rateMenu->addItem("1/1", 1);
    rateMenu->addItem("1/2", 2);
    rateMenu->addItem("1/4", 3);
    rateMenu->addItem("1/8", 4);
    rateMenu->addItem("1/16", 5);
    rateMenu->addItem("1/32", 6);
    rateMenu->addItem("1/64", 7);
    //what about other values, such as the d's and t's in Logic's tremolo?
    rateMenu->setSelectedId(3, true);*/
    
    addAndMakeVisible(syncButton = new AlphaTextButton(translate("SYNC")));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);
    
    /*addAndMakeVisible(shapeMenu = new ComboBox());
    shapeMenu->addListener(this);
    shapeMenu->addMouseListener(this, true);
    shapeMenu->addItem(translate("Sine"), 1);
    shapeMenu->addItem(translate("Square"), 2);
    shapeMenu->addItem(translate("Triangle"), 3);
    shapeMenu->addItem(translate("Rising Sawtooth"), 4);
    shapeMenu->addItem(translate("Falling Sawtooth"), 5);
    shapeMenu->setSelectedId(1, true);*/
	
	Image *sineIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::sinesymbol_png, BinaryDataNew::sinesymbol_pngSize));
	Image *squareIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::squaresymbol_png, BinaryDataNew::squaresymbol_pngSize));
	Image *triangleIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::trianglesymbol_png, BinaryDataNew::trianglesymbol_pngSize));
	Image *sawRiseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::sawrisingsymbol_png, BinaryDataNew::sawrisingsymbol_pngSize));
	Image *sawFallIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::sawfallingsymbol_png, BinaryDataNew::sawfallingsymbol_pngSize));
	
	
	for (int i = 0; i < 5; i++)
    {
        if (i == 0)
            shapeMenuButtons.insert(i, new SettingsButtonImage(sineIcon, (250 * (M_PI / 180)), 
																  (294 * (M_PI / 180)), 0.75f, 1.0f));
        if (i == 1)
            shapeMenuButtons.insert(i, new SettingsButtonImage(squareIcon, (294 * (M_PI / 180)),
																  (338 *(M_PI / 180)) , 0.75f, 1.0f));
        if (i == 2)
            shapeMenuButtons.insert(i, new SettingsButtonImage(triangleIcon, (338 * (M_PI / 180)), (382 * (M_PI / 180)), 
																  0.75f,1.0f));
        if (i == 3)
            shapeMenuButtons.insert(i, new SettingsButtonImage(sawRiseIcon, (382 * (M_PI / 180)), (426 * (M_PI / 180)), 
																  0.75f, 1.0f));
        if (i == 4)
            shapeMenuButtons.insert(i, new SettingsButtonImage(sawFallIcon, (426 * (M_PI / 180)), 
																  (470 * (M_PI / 180)),0.75f,1.0f));
       
        
        shapeMenuButtons[i]->addListener(this);
        shapeMenuButtons[i]->setRadioGroupId (212);
        shapeMenuButtons[i]->addMouseListener(this, false);
		addAndMakeVisible(shapeMenuButtons[i]);
    }
    
    shapeMenuButtons[0]->setToggleState(true, false);
    
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Depth/Mix"), 2);
    alphaTouchMenu->addItem(translate("Rate"), 3);
    alphaTouchMenu->addItem(translate("Wave Shape"), 4);
    alphaTouchMenu->setSelectedId(1, true);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::inverticon_png, BinaryDataNew::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    //---------------parameter label -------------------------------------
    addAndMakeVisible(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaColours::blue);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
    
    tempo = AppSettings::Instance()->getGlobalTempo();
    
    setInterceptsMouseClicks(false, true);
}

GuiTremolo::~GuiTremolo()
{
	shapeMenuButtons.clear();
    deleteAllChildren();
}



void GuiTremolo::resized()
{
    //this needs rearranging
    //maybe the sync button going above the rate menu.
    //and the sticky and invert button are spaced equally at the bottom (should be applied to every effect)
    //with the shape menu central under the alphatouch menu
    
    depthSlider->setBounds(97, 97, 130, 130);
    rateSlider->setBounds(87, 87, 150, 150);
    intensitySlider->setBounds(77, 77, 170, 170);
    
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    parameterHoverLabel->setBounds(144, 187, 36, 15);
    
	rateMenu->setBounds(71, 202, 48, 48);
    //shapeMenu->setBounds(119, 52, 87, 15);
	syncButton->setBounds(121, 235, 32, 32);
	
	shapeMenuButtons[0]->setBounds(46, 46, 232, 232);
	shapeMenuButtons[1]->setBounds(46, 46, 232, 232);
	shapeMenuButtons[2]->setBounds(46, 46, 232, 232);
	shapeMenuButtons[3]->setBounds(46, 46, 232, 232);
	shapeMenuButtons[4]->setBounds(46, 46, 232, 232);
    
    
}

void GuiTremolo::paint (Graphics& g)
{
	
	g.setColour(Colours::black);
	g.fillEllipse(118, 232, 38, 38);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse(118, 232, 38, 38, 1.0f);
	
	
}



void GuiTremolo::sliderValueChanged (Slider *slider)
{
    if (slider == depthSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxTremoloDepth(depthSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
    
    else if (slider == rateSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxTremoloRate(rateSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
    
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxTremoloAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
    
    
    
}

void GuiTremolo::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxTremoloAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
    }
    
    
}


void GuiTremolo::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxTremoloAtReverse(reverseButton->getToggleState());
        }
        
    }
    
    else if (button == syncButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxTremoloSync(syncButton->getToggleState());
        }
        
        rateSlider->setVisible(false);
        rateMenu->setVisible(false);
        
        if (syncButton->getToggleState() == 0) //off
            rateSlider->setVisible(true);
        else //on
            rateMenu->setVisible(true);
        
    }
	
	else if (button == rateMenu)
    {
		
		PopupMenu menu;
		
		menu.addItem(1, "1/1");
		menu.addItem(2, "1/2");
		menu.addItem(3, "1/4");
		menu.addItem(4, "1/8");
		menu.addItem(5, "1/16");
		menu.addItem(6, "1/32");
		menu.addItem(7, "1/64");
		
        //work out LFO rate based on rate selected and the tempo.
        tempo = AppSettings::Instance()->getGlobalTempo();
        double bps = tempo/60.0;
        double lfoRate;
		
		const int result = menu.show();

        
        switch (result)
        {
                
            case 1: // 1/1 - 4 beats
                lfoRate = bps * 0.25;
				rateMenu->setButtonText("1/1");
                break;
            case 2: // 1/2 - 2 beats
                lfoRate = bps * 0.5;
				rateMenu->setButtonText("1/2");
                break;
            case 3: // 1/4 - 1 beat
                lfoRate = bps * 1.0;
				rateMenu->setButtonText("1/4");
                break;
            case 4: // 1/8 - half beat
                lfoRate = bps * 2.0;
				rateMenu->setButtonText("1/8");
                break;
            case 5: // 1/16 - quarter beat
                lfoRate = bps * 4.0;
				rateMenu->setButtonText("1/16");
                break;
            case 6: // 1/32 - 8th beat
                lfoRate = bps * 8.0;
				rateMenu->setButtonText("1/32");
                break;
            case 7: // 1/64 - 16th beat
                lfoRate = bps * 16.0;
				rateMenu->setButtonText("1/64");
                break;
            default: // 1/4 - 1 beat
                lfoRate = bps * 1.0;
				rateMenu->setButtonText("1/4");
                break;
				
        }
        
        rateSlider->setValue(lfoRate, false);
        
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxTremoloRate(lfoRate);
            PAD_SETTINGS->setPadFxTremoloRateMenu(result);
        }
        
    }
	for (int waveform = 0; waveform < 4; waveform++)
    {
        if (button == shapeMenuButtons[waveform])
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setPadFxTremoloShape(waveform+1);
            }
            
            break;
        }
    }
	
}

void GuiTremolo::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiTremolo::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    rateSlider->setVisible(false);
    rateMenu->setVisible(false);
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        depthSlider->setValue(PAD_SETTINGS->getPadFxTremoloDepth(), false);
        rateSlider->setValue(PAD_SETTINGS->getPadFxTremoloRate(), false);
        //rateMenu->setSelectedId(PAD_SETTINGS->getPadFxTremoloRateMenu(), true);
        syncButton->setToggleState(PAD_SETTINGS->getPadFxTremoloSync(), false);
        //shapeMenu->setSelectedId(PAD_SETTINGS->getPadFxTremoloShape(), true);
		shapeMenuButtons[PAD_SETTINGS->getPadFxTremoloShape()-1]->setToggleState(true, false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxTremoloAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxTremoloAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxTremoloAtIntensity(), false);
		
		switch (PAD_SETTINGS->getPadFxTremoloRateMenu())
        {
                
            case 1: // 1/1 - 4 beats
               
				rateMenu->setButtonText("1/1");
                break;
            case 2: // 1/2 - 2 beats
               
				rateMenu->setButtonText("1/2");
                break;
            case 3: // 1/4 - 1 beat
               
				rateMenu->setButtonText("1/4");
                break;
            case 4: // 1/8 - half beat
                
				rateMenu->setButtonText("1/8");
                break;
            case 5: // 1/16 - quarter beat
                
				rateMenu->setButtonText("1/16");
                break;
            case 6: // 1/32 - 8th beat
               
				rateMenu->setButtonText("1/32");
                break;
            case 7: // 1/64 - 16th beat
                
				rateMenu->setButtonText("1/64");
                break;
            default: // 1/4 - 1 beat
               
				rateMenu->setButtonText("1/4");
                break;
				
        }
    }
    
    else if(MULTI_PADS)
    {
        depthSlider->setValue(0.7, false);
        rateSlider->setValue(5.0, false);
        rateMenu->setButtonText("1/4");
        syncButton->setToggleState(true, false);
        //shapeMenu->setSelectedId(2, true);
		shapeMenuButtons[0]->setToggleState(true, false);
        
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, false);
    }
    
    
    if (syncButton->getToggleState() == 0) //off
        rateSlider->setVisible(true);
    else //on
        rateMenu->setVisible(true);
    
}

//need this?? not currently being used
void GuiTremolo::setTempo (float value)
{
    tempo = value;
    //OR
    tempo = AppSettings::Instance()->getGlobalTempo();
}

void GuiTremolo::mouseEnter (const MouseEvent &e)
{
    if (depthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Depth/Mix Control. Sets the depth/mix of the LFO for the selected pads."));
        parameterHoverLabel->setText(String(depthSlider->getValue(), 3), false);
    }
    else if (rateSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("LFO Rate Control. Sets the rate in Hz for the selected pads. If you would like to set the rate based on the tempo, click on the 'Sync' button."));
        parameterHoverLabel->setText(String(rateSlider->getValue(), 3), false);
    }
    else if (rateMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("LFO Rate Menu. Sets the LFO rate for the selected pads. If you would like to set the rate in Hz, click on the 'Sync' button."));
    }
    else if (syncButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Tempo Sync Button. Turn this button on to sync the LFO to the tempo, else you can set the LFO is Hz."));
    }
    else if (shapeMenuButtons[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sine wave. Sets the LFO wave shape for the selected pads."));
    }
	else if (shapeMenuButtons[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Square wave. Sets the LFO wave shape for the selected pads."));
    }
	else if (shapeMenuButtons[2]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Triangle wave. Sets the LFO wave shape for the selected pads."));
    }
	else if (shapeMenuButtons[3]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sawtooth Rising wave. Sets the LFO wave shape for the selected pads."));
    }
	else if (shapeMenuButtons[4]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sawtooth Falling wave. Sets the LFO wave shape for the selected pads."));
    }
    else if (alphaTouchMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::alphaTouchMenu));
    }
    else if (reverseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::inverseButton));
    }
    else if (intensitySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::intensitySlider));
        parameterHoverLabel->setText(String(intensitySlider->getValue(), 3), false);
    }
    
}

void GuiTremolo::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, false);
    
}
