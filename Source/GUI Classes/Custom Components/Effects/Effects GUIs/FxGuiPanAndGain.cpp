//
//  FxGuiPanAndGain.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 22/12/2011.
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

#include "FxGuiPanAndGain.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/MainBinaryData.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiGainAndPan::GuiGainAndPan(MainComponent &ref)
                            : mainComponentRef(ref)
{
    addAndMakeVisible(gainSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	gainSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    gainSlider->setRange(0.0, 1.0, 0.001);
    gainSlider->addListener(this);
    gainSlider->addMouseListener(this, true);
    
    addAndMakeVisible(panSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	panSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    panSlider->setRange(0.0, 1.0, 0.001);
    panSlider->addListener(this);
    panSlider->addMouseListener(this, true);
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Gain"), 2);
    alphaTouchMenu->addItem(translate("Pan"), 3);
    alphaTouchMenu->setSelectedId(1, dontSendNotification);
    
	Image *reverseIcon = new Image(ImageCache::getFromMemory(MainBinaryData::inverticon_png, MainBinaryData::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0, 0.001);
    intensitySlider->setValue(1.0, dontSendNotification);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    
    //---------------parameter label -------------------------------------
    addAndMakeVisible(parameterHoverLabel = new Label("value label", String()));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setFont(Font(11 + AlphaTheme::getInstance()->fontSizeAddition));
    parameterHoverLabel->addMouseListener(this, true);
    
    setInterceptsMouseClicks(false, true);
}

GuiGainAndPan::~GuiGainAndPan()
{
    deleteAllChildren();
}



void GuiGainAndPan::resized()
{
    gainSlider->setBounds(97, 97, 130, 130);
    panSlider->setBounds(87, 87, 150, 150);
    intensitySlider->setBounds(77, 77, 170, 170);
	
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    
    parameterHoverLabel->setBounds(144, 187, 36, 15);
}


void GuiGainAndPan::paint(Graphics &g)
{
    parameterHoverLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->mainColour);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaTheme::getInstance()->mainColourLighter);
}


void GuiGainAndPan::sliderValueChanged (Slider *slider)
{
    if (slider == gainSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxGainPanGain(gainSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
    
    if (slider == panSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxGainPanPan(panSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }    
    
    if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxGainPanAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
    
}

void GuiGainAndPan::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxGainPanAlphaTouch(alphaTouchMenu->getSelectedId());
        }
    
    }
    
}


void GuiGainAndPan::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxGainPanAtReverse(reverseButton->getToggleState());
        }
        
    }
}

void GuiGainAndPan::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiGainAndPan::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        gainSlider->setValue(PAD_SETTINGS->getPadFxGainPanGain(), dontSendNotification);
        panSlider->setValue(PAD_SETTINGS->getPadFxGainPanPan(), dontSendNotification);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxGainPanAlphaTouch(), dontSendNotification);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxGainPanAtReverse(), dontSendNotification);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxGainPanAtIntensity(), dontSendNotification);
    }
    
    else if(MULTI_PADS)
    {
        gainSlider->setValue(0.5, dontSendNotification);
        panSlider->setValue(0.5, dontSendNotification);
        alphaTouchMenu->setSelectedId(0, dontSendNotification);
        reverseButton->setToggleState(0, dontSendNotification);
        intensitySlider->setValue(1.0, dontSendNotification);
    }
    
}

void GuiGainAndPan::mouseEnter (const MouseEvent &e)
{
    if (gainSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Gain. Sets the gain for the selected pads."));
        parameterHoverLabel->setText(String(gainSlider->getValue(), 3), dontSendNotification);
    }
    else if (panSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Pan. Sets the pan level for the selected pads."));
        parameterHoverLabel->setText(String(panSlider->getValue(), 3), dontSendNotification);
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

void GuiGainAndPan::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String());
    parameterHoverLabel->setText(String(), dontSendNotification);
    
}
