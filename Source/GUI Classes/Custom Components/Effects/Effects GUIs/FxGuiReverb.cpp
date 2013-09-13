//
//  FxGuiReverb.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 21/12/2011.
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

#include "FxGuiReverb.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/BinaryDataNew.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiReverb::GuiReverb(MainComponent &ref)
                        : mainComponentRef(ref)
{
    
    addAndMakeVisible(mixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	mixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    mixSlider->setRange(0.0, 1.0, 0.001);
    mixSlider->addListener(this);
    mixSlider->addMouseListener(this, true);
    
	addAndMakeVisible(roomSizeSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	roomSizeSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    roomSizeSlider->setRange(0, 1.0, 0.001);
    roomSizeSlider->addListener(this);
    roomSizeSlider->addMouseListener(this, true);
    
    addAndMakeVisible(dampingSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	dampingSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    dampingSlider->setRange(0.0, 1.0, 0.001);
    dampingSlider->addListener(this);
    dampingSlider->addMouseListener(this, true);
    
    addAndMakeVisible(widthSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	widthSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    widthSlider->setRange(0.0, 1.0, 0.001);
    widthSlider->addListener(this);
    widthSlider->addMouseListener(this, true);
    
    
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Mix"), 2);
    alphaTouchMenu->addItem(translate("Room Size"), 3);
    alphaTouchMenu->addItem(translate("Damping"), 4);
    alphaTouchMenu->addItem(translate("Width"), 5);
    alphaTouchMenu->setSelectedId(1, true);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::inverticon_png, BinaryDataNew::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0, 0.001);
    intensitySlider->setValue(1.0, dontSendNotification);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaTheme::getInstance()->mainColourLighter);
    
    //---------------parameter label -------------------------------------
    addAndMakeVisible(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->mainColour);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
    
    setInterceptsMouseClicks(false, true);
}

GuiReverb::~GuiReverb()
{
    deleteAllChildren();
}



void GuiReverb::resized()
{
    mixSlider->setBounds(97, 97, 130, 130);
    roomSizeSlider->setBounds(87, 87, 150, 150);
    dampingSlider->setBounds(77, 77, 170, 170);
	widthSlider->setBounds(67, 67, 190, 190);
    intensitySlider->setBounds(57, 57, 210, 210);
    
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    parameterHoverLabel->setBounds(144, 187, 36, 15);
}



void GuiReverb::sliderValueChanged (Slider *slider)
{
    if (slider == mixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxReverbMix(mixSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
    else if (slider == roomSizeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxReverbRoomSize(roomSizeSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
    else if (slider == dampingSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxReverbDamping(dampingSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
    else if (slider == widthSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxReverbWidth(widthSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    

    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxReverbAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
}

void GuiReverb::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxReverbAlphaTouch(alphaTouchMenu->getSelectedId());
        }
    }
    
}


void GuiReverb::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxReverbAtReverse(reverseButton->getToggleState());
        }
        
    }
}

void GuiReverb::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiReverb::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        mixSlider->setValue(PAD_SETTINGS->getPadFxReverbMix(), dontSendNotification);
        roomSizeSlider->setValue(PAD_SETTINGS->getPadFxReverbRoomSize(), dontSendNotification);
        dampingSlider->setValue(PAD_SETTINGS->getPadFxReverbDamping(), dontSendNotification);
        widthSlider->setValue(PAD_SETTINGS->getPadFxReverbWidth(), dontSendNotification);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxReverbAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxReverbAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxReverbAtIntensity(), dontSendNotification);
    }
    
    else if(MULTI_PADS)
    {
        mixSlider->setValue(0.7, dontSendNotification);
        roomSizeSlider->setValue(0.5, dontSendNotification);
        dampingSlider->setValue(0.5, dontSendNotification);
        widthSlider->setValue(0.5, dontSendNotification);
        
        alphaTouchMenu->setSelectedId(0, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, dontSendNotification);
    }
    
    
}

void GuiReverb::mouseEnter (const MouseEvent &e)
{
    if (mixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Wet/Dry Mix. Sets the Wet/Dry mix of the reverb level on the selected pads."));
        parameterHoverLabel->setText(String(mixSlider->getValue(), 3), dontSendNotification);
    }
    else if (roomSizeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Room Size. Sets the reverb room size of the selected pads. A larger room size will result in a longer reverberation time."));
        parameterHoverLabel->setText(String(roomSizeSlider->getValue(), 3), dontSendNotification);
    }
    else if (dampingSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Damping. Sets the reverb damping level of the selected pads. A larger damping value result in a more subtle reverberation tail."));
        parameterHoverLabel->setText(String(dampingSlider->getValue(), 3), dontSendNotification);
    }
    else if (widthSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Width. Sets the reverb width of the selected pads. A larger width will result in a wider reverb sound in the stereo spread."));
        parameterHoverLabel->setText(String(widthSlider->getValue(), 3), dontSendNotification);
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

void GuiReverb::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, dontSendNotification);
    
}