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
    wetDryMixSlider->setRange(0, 1.0, 0.001);
    wetDryMixSlider->addListener(this);
    wetDryMixSlider->addMouseListener(this, true);
	
	addAndMakeVisible(toneSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	toneSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    toneSlider->setRange(0., 1.0, 0.001);
    toneSlider->addListener(this);
    toneSlider->addMouseListener(this, true);
	
	addAndMakeVisible(driveSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	driveSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    driveSlider->setRange(0.1, 60, 0.1);
    driveSlider->addListener(this);
    driveSlider->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0, 0.001);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaColours::getInstance()->colour2);
    
    //below is temporily hidden in resized()!
    addAndMakeVisible(inputGainSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	inputGainSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    inputGainSlider->setRange(0.0, 1.0, 0.001);
    inputGainSlider->addListener(this);
    inputGainSlider->addMouseListener(this, true);
    
    /*
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
     */
    
    addAndMakeVisible(distortionTypeMenu = new AlphaPopUpButton());
    distortionTypeMenu->addListener(this);
    distortionTypeMenu->addMouseListener(this, true);
    distortionTypeMenu->setButtonText("Soft");
    
    addAndMakeVisible(syncButton = new AlphaTextButton(translate("SYNC")));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);    
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    //alphaTouchMenu->addItem(translate("Input Gain"), 2);
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
    parameterHoverLabel->setColour(Label::textColourId, AlphaColours::getInstance()->colour1);
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
    toneSlider->setBounds(87, 87, 150, 150);
	driveSlider->setBounds(77, 77, 170, 170);
    intensitySlider->setBounds(67, 67, 190, 190);
    //inputGainSlider->setBounds(57, 57, 210, 210);
    
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    parameterHoverLabel->setBounds(144, 187, 36, 15);
	
	//distortionTypeMenu->setBounds(119, 22, 87, 20);
    distortionTypeMenu->setBounds(71, 202, 48, 48);
    
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
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
        
    }
    
    else if (slider == driveSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionDrive(driveSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
        
    }
	
	else if (slider == toneSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionTone(toneSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
        
    }
	
	else if (slider == wetDryMixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionWetDryMix(wetDryMixSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
        
    }
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
        
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
    
    /*
    else if (comboBox == distortionTypeMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDistortionTypeMenu(distortionTypeMenu->getSelectedId());
        }
        
    }
     */
    
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
    
    else if (button == distortionTypeMenu)
    {
		
		PopupMenu menu;
        menu.addItem(1, "Soft");
        menu.addItem(2, "Hard");
        menu.addItem(3, "HF0");
        menu.addItem(4, "HF1");
        menu.addItem(5, "HF2");
        menu.addItem(6, "Digital");
        //menu.addItem("Recitify", 7);
        //menu.addItem("AddSine", 8);
		
		const int result = menu.show();
        
        if (result != 0) //if the user selects something
        {
            setDistortionTypeMenu(result);
            
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setPadFxDistortionTypeMenu(result);
            }
        }
    }
    
}

void GuiDistortion::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}


void GuiDistortion::setDistortionTypeMenu (int selectedItem)
{
    switch (selectedItem)
    {
        case 1:
            distortionTypeMenu->setButtonText("Soft");
            break;
        case 2:
            distortionTypeMenu->setButtonText("Hard");
            break;
        case 3:
            distortionTypeMenu->setButtonText("HF0");
            break;
        case 4:
            distortionTypeMenu->setButtonText("HF1");
            break;
        case 5:
            distortionTypeMenu->setButtonText("HF2");
            break;
        case 6:
            distortionTypeMenu->setButtonText("Digital");
            break;
        default:
            distortionTypeMenu->setButtonText("Soft");
            break;
            
    }
}


void GuiDistortion::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        inputGainSlider->setValue(PAD_SETTINGS->getPadFxDistortionInputGain(), dontSendNotification);
        driveSlider->setValue(PAD_SETTINGS->getPadFxDistortionDrive(), dontSendNotification);
		toneSlider->setValue(PAD_SETTINGS->getPadFxDistortionTone(), dontSendNotification);
		wetDryMixSlider->setValue(PAD_SETTINGS->getPadFxDistortionWetDryMix(), dontSendNotification);
        setDistortionTypeMenu(PAD_SETTINGS->getPadFxDistortionTypeMenu());
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxDistortionAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxDistortionAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxDistortionAtIntensity(), dontSendNotification);
    }
    
    else if(MULTI_PADS)
    {
        inputGainSlider->setValue(0.7, dontSendNotification);
        driveSlider->setValue(1.0, dontSendNotification);
		toneSlider->setValue(1.0, dontSendNotification);
		wetDryMixSlider->setValue(1.0, dontSendNotification);
        distortionTypeMenu->setButtonText("-");
        syncButton->setToggleState(true, false);
        
        alphaTouchMenu->setSelectedId(0, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, dontSendNotification);
    }
    
}

void GuiDistortion::mouseEnter (const MouseEvent &e)
{
    if (inputGainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Input Gain. Sets the gain of the signal to be distorted."));
        parameterHoverLabel->setText(String(inputGainSlider->getValue(), 3), dontSendNotification);
    }
    else if (driveSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Drive. Sets the amount of drive/distortion for the selected pads."));
        parameterHoverLabel->setText(String(driveSlider->getValue(), 3), dontSendNotification);
    }
	else if (toneSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Tone. Sets the tone/filtering of the distortion on selected pads. Set this control to 0 to bypass the filter."));
        parameterHoverLabel->setText(String(toneSlider->getValue(), 3), dontSendNotification);
    }
	else if (wetDryMixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Wet/Dry Mix. Sets the wet/dry mix for the distortion effect on the selected pads."));
        parameterHoverLabel->setText(String(wetDryMixSlider->getValue(), 3), dontSendNotification);
    }
    else if (distortionTypeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Distortion Type Menu. Sets the type of distortion for the selected pads."));
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
        parameterHoverLabel->setText(String(intensitySlider->getValue(), 3), dontSendNotification);
    }
    
}

void GuiDistortion::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, dontSendNotification);
    
}
