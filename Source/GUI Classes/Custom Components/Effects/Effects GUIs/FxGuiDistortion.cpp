//
//  FxGuiDistortion.cpp
//  AlphaLive
//
//  Created by Felix Godden on the 25/09/2012
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

#include "FxGuiDistortion.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/BinaryDataNew.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiDistortion::GuiDistortion(MainComponent &ref)
:           mainComponentRef(ref)
{
	
	addAndMakeVisible(wetDryMixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	wetDryMixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    wetDryMixSlider->setRange(0, 1.0);
    wetDryMixSlider->addListener(this);
    wetDryMixSlider->addMouseListener(this, true);
	
	addAndMakeVisible(toneSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	toneSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    toneSlider->setRange(0., 1.0);
    toneSlider->addListener(this);
    toneSlider->addMouseListener(this, true);
	
	addAndMakeVisible(driveSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	driveSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    driveSlider->setRange(0.1, 60);
    driveSlider->addListener(this);
    driveSlider->addMouseListener(this, true);
	
    addAndMakeVisible(inputGainSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	inputGainSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    inputGainSlider->setRange(0.0, 1.0);
    inputGainSlider->addListener(this);
    inputGainSlider->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 230));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaColours::lightblue);
    
    
    addAndMakeVisible(distortionTypeMenu = new ComboBox());
    distortionTypeMenu->addListener(this);
    distortionTypeMenu->addMouseListener(this, true);
    distortionTypeMenu->addItem("Soft", 1);
    distortionTypeMenu->addItem("Hard", 2);
    distortionTypeMenu->addItem("HF0", 3);
    distortionTypeMenu->addItem("HF1", 4);
    distortionTypeMenu->addItem("HF2", 5);
    distortionTypeMenu->addItem("Digital", 6);
//	distortionTypeMenu->addItem("Recitify", 7);
//  distortionTypeMenu->addItem("AddSine", 8);
    distortionTypeMenu->setSelectedId(1, true);
    
    addAndMakeVisible(syncButton = new AlphaTextButton(translate("SYNC")));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);    
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Input Gain"), 2);
    alphaTouchMenu->addItem(translate("Drive"), 3);
    alphaTouchMenu->addItem(translate("Tone"), 4);
	alphaTouchMenu->addItem(translate("Wet/Dry Mix"), 5);
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
        
    setInterceptsMouseClicks(false, true);
}

GuiDistortion::~GuiDistortion()
{
    deleteAllChildren();
}



void GuiDistortion::resized()
{
	wetDryMixSlider->setBounds(97, 97, 130, 130);
    toneSlider->setBounds(77, 77, 170, 170);
	driveSlider->setBounds(67, 67, 190, 190);
	inputGainSlider->setBounds(57, 57, 210, 210);
	
    intensitySlider->setBounds(47, 47, 230, 230);
    
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    parameterHoverLabel->setBounds(144, 187, 36, 15);
	
	distortionTypeMenu->setBounds(119, 22, 87, 20);
    
}



void GuiDistortion::sliderValueChanged (Slider *slider)
{
    if (slider == inputGainSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionInputGain(inputGainSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
    
    else if (slider == driveSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionDrive(driveSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
	
	else if (slider == toneSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionTone(toneSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
	
	else if (slider == wetDryMixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionWetDryMix(wetDryMixSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
        
    }
    
    
    
}

void GuiDistortion::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
    }
    
    else if (comboBox == distortionTypeMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionTypeMenu(distortionTypeMenu->getSelectedId());
        }
        
    }
    
}


void GuiDistortion::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionAtReverse(reverseButton->getToggleState());
        }
        
    }
    
}

void GuiDistortion::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiDistortion::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        inputGainSlider->setValue(PAD_SETTINGS->getPadFxDistortionInputGain(), false);
        driveSlider->setValue(PAD_SETTINGS->getPadFxDistortionDrive(), false);
		toneSlider->setValue(PAD_SETTINGS->getPadFxDistortionTone(), false);
		wetDryMixSlider->setValue(PAD_SETTINGS->getPadFxDistortionWetDryMix(), false);
        distortionTypeMenu->setSelectedId(PAD_SETTINGS->getPadFxDistortionTypeMenu(), true);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxDistortionAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxDistortionAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxDistortionAtIntensity(), false);
    }
    
    else if(MULTI_PADS)
    {
        inputGainSlider->setValue(0.7, true);
        driveSlider->setValue(1.0, true);
		toneSlider->setValue(1.0, true);
		wetDryMixSlider->setValue(1.0, true);
        distortionTypeMenu->setSelectedId(3, true);
        syncButton->setToggleState(true, true);
        
        alphaTouchMenu->setSelectedId(1, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, false);
    }
    
}

void GuiDistortion::mouseEnter (const MouseEvent &e)
{
    if (inputGainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Input Gain Control. Sets the gain of the signal to be distorted."));
        parameterHoverLabel->setText(String(inputGainSlider->getValue(), 3), false);
    }
    else if (driveSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Drive Contol. Sets the amount of drive effecting the distorted signal."));
        parameterHoverLabel->setText(String(driveSlider->getValue(), 3), false);
    }
	else if (toneSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Tone Control. Sets the tone of the effected signal."));
        parameterHoverLabel->setText(String(toneSlider->getValue(), 3), false);
    }
	else if (wetDryMixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Wet/Dry Mix Control. Sets the balance of distorted signal to clean signal."));
        parameterHoverLabel->setText(String(wetDryMixSlider->getValue(), 3), false);
    }
    else if (distortionTypeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Distortion Type Menu. Sets the type current type of distortion."));
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

void GuiDistortion::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, false);
    
}
