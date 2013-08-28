//
//  FxGuiDelay.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/07/2012.
//  Copyright 2012 nu desine. All rights reserved.
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

#include "FxGuiDelay.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/BinaryDataNew.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiDelay::GuiDelay(MainComponent &ref)
:           mainComponentRef(ref)
{
    addAndMakeVisible(wetMixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	wetMixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    wetMixSlider->setRange(0.0, 1.0, 0.001);
    wetMixSlider->addListener(this);
    wetMixSlider->addMouseListener(this, true);
    
	addAndMakeVisible(feedbackSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	feedbackSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    feedbackSlider->setRange(0, 1.0, 0.001);
    feedbackSlider->addListener(this);
    feedbackSlider->addMouseListener(this, true);
    
    addAndMakeVisible(delayTimeSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	delayTimeSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    delayTimeSlider->setRange(0.0, 4000.0, 1);
    delayTimeSlider->addListener(this);
    delayTimeSlider->addMouseListener(this, true);
    
    addAndMakeVisible(lpfFrequencySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190));
	lpfFrequencySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    lpfFrequencySlider->setRange(30, 20000, 1);
    lpfFrequencySlider->setSkewFactor(0.35);
    lpfFrequencySlider->addListener(this);
    lpfFrequencySlider->addMouseListener(this, true);
    
    addAndMakeVisible(hpfFrequencySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	hpfFrequencySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    hpfFrequencySlider->setRange(30, 20000, 1);
    hpfFrequencySlider->setSkewFactor(0.35);
    hpfFrequencySlider->addListener(this);
    hpfFrequencySlider->addMouseListener(this, true);
    

    
    addAndMakeVisible(delayTimeMenu = new AlphaPopUpButton());
    delayTimeMenu->addListener(this);
    delayTimeMenu->addMouseListener(this, true);
	delayTimeMenu->setButtonText("1/2");
   /* delayTimeMenu->addItem(translate("4 Beats"), 1);
    delayTimeMenu->addItem(translate("2 Beats"), 2);
    delayTimeMenu->addItem(translate("1 Beat"), 3);
    delayTimeMenu->addItem(translate("Half Beat"), 4);
    delayTimeMenu->addItem(translate("Quarter Beat"), 5);
    delayTimeMenu->setSelectedId(4, true);*/
    
	Image *syncImage = new Image(ImageCache::getFromMemory(BinaryDataNew::syncicon_png, BinaryDataNew::syncicon_pngSize));
    addAndMakeVisible(syncButton = new ModeButton(syncImage));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, false);
    syncButton->addListener(this);
    syncButton->addMouseListener(this, true);

    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Wet Mix"), 2);
    alphaTouchMenu->addItem(translate("Delay Time"), 3);
    alphaTouchMenu->addItem(translate("Feedback"), 4);
    alphaTouchMenu->addItem(translate("LPF Frequency"), 5);
    alphaTouchMenu->addItem(translate("HPF Frequency"), 6);
    alphaTouchMenu->setSelectedId(1, true);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::inverticon_png, BinaryDataNew::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 230));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0, 0.001);
    intensitySlider->setValue(1.0, dontSendNotification);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaTheme::getInstance()->colour2);
    
    //---------------parameter label -------------------------------------
    addAndMakeVisible(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->colour1);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
    
    tempo = AppSettings::Instance()->getGlobalTempo();
    
    setInterceptsMouseClicks(false, true);
}

GuiDelay::~GuiDelay()
{
    deleteAllChildren();
}



void GuiDelay::resized()
{
    wetMixSlider->setBounds(97, 97, 130, 130);
    feedbackSlider->setBounds(87, 87, 150, 150);
    delayTimeSlider->setBounds(77, 77, 170, 170);
	lpfFrequencySlider->setBounds(67, 67, 190, 190);
    hpfFrequencySlider->setBounds(57, 57, 210, 210);
    
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    parameterHoverLabel->setBounds(144, 187, 36, 15);
    intensitySlider->setBounds(47, 47, 230, 230);
    
    delayTimeMenu->setBounds(71, 202, 48, 48);
    syncButton->setBounds(121, 235, 32, 32);
    
    
}

void GuiDelay::paint (Graphics& g)
{
	
	g.setColour(Colours::black);
	g.fillEllipse(118, 232, 38, 38);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse(118, 232, 38, 38, 1.0f);
	
	
}

void GuiDelay::sliderValueChanged (Slider *slider)
{
    if (slider == wetMixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayMix(wetMixSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
    else if (slider == delayTimeSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayTime(delayTimeSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 2), dontSendNotification);
    }
    
    else if (slider == feedbackSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayFeedback(feedbackSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    
    else if (slider == lpfFrequencySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayLpfFreq(lpfFrequencySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 0), dontSendNotification);
    }
    
    else if (slider == hpfFrequencySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayHpfFreq(hpfFrequencySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 0), dontSendNotification);
    }
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
    }
    

    
}

void GuiDelay::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
    }
    
}


void GuiDelay::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelayAtReverse(reverseButton->getToggleState());
        }
    }
    
    else if (button == syncButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxDelaySync(syncButton->getToggleState());
            
            //if set to sync, grab the currently set rate menu value and re-set the rate
            if (syncButton->getToggleState() == true)
            {
                PAD_SETTINGS->setPadFxDelayTimeMenu(PAD_SETTINGS->getPadFxDelayTimeMenu());
                delayTimeSlider->setValue(PAD_SETTINGS->getPadFxDelayTime(), dontSendNotification);
            }
        }
        
        delayTimeSlider->setVisible(false);
        delayTimeMenu->setVisible(false);
        
        if (syncButton->getToggleState() == 0) //off
            delayTimeSlider->setVisible(true);
        else //on
            delayTimeMenu->setVisible(true);
            
        
    }
	else if (button == delayTimeMenu)
    {
		
		PopupMenu menu;
		menu.addItem(1, translate("4 Beats"));
		menu.addItem(2, translate("2 Beats"));
		menu.addItem(3, translate("1 Beat"));
		menu.addItem(4, translate("Half Beat"));
		menu.addItem(5, translate("Quarter Beat"));
		
		const int result = menu.show();
        
        if (result != 0) //if the user selects something
        {
            switch (result)
            {
                case 1: // 4 beats
                    delayTimeMenu->setButtonText("4");
                    break;
                case 2: // 2 beats
                    delayTimeMenu->setButtonText("2");
                    break;
                case 3: // 1 beat
                    delayTimeMenu->setButtonText("1");
                    break;
                case 4: //half beat
                    delayTimeMenu->setButtonText("1/2");
                    break;
                case 5: // quarter beat
                    delayTimeMenu->setButtonText("1/4");
                    break;
                default: // 1 beat
                    delayTimeMenu->setButtonText("1");
                    break;
            }
            
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setPadFxDelayTimeMenu(result); //in turn sets the actual delay time
            }
            
            delayTimeSlider->setValue(AppSettings::Instance()->padSettings[selectedPads[0]]->getPadFxDelayTime(), dontSendNotification);
        }
        
    }
	
}


void GuiDelay::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiDelay::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    delayTimeSlider->setVisible(false);
    delayTimeMenu->setVisible(false);
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        wetMixSlider->setValue(PAD_SETTINGS->getPadFxDelayMix(), dontSendNotification);
        delayTimeSlider->setValue(PAD_SETTINGS->getPadFxDelayTime(), dontSendNotification);
		
		switch (PAD_SETTINGS->getPadFxDelayTimeMenu())
        {
            case 1: // 4 beats
             
				delayTimeMenu->setButtonText("4");
                break;
            case 2: // 2 beats
               
				delayTimeMenu->setButtonText("2");
                break;
            case 3: // 1 beat
                
				delayTimeMenu->setButtonText("1");
                break;
            case 4: //half beat
                
				delayTimeMenu->setButtonText("1/2");
                break;
            case 5: // quarter beat
                
				delayTimeMenu->setButtonText("1/4");
                break;
            default: // 1 beat
                
				delayTimeMenu->setButtonText("1");
                break;
        }
		
        feedbackSlider->setValue(PAD_SETTINGS->getPadFxDelayFeedback(), dontSendNotification);
        lpfFrequencySlider->setValue(PAD_SETTINGS->getPadFxDelayLpfFreq(), dontSendNotification);
        hpfFrequencySlider->setValue(PAD_SETTINGS->getPadFxDelayHpfFreq(), dontSendNotification);
        syncButton->setToggleState(PAD_SETTINGS->getPadFxDelaySync(), false);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxDelayAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxDelayAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxDelayAtIntensity(), dontSendNotification);
    }
    
    else if(MULTI_PADS)
    {
        wetMixSlider->setValue(0.7, dontSendNotification);
        delayTimeSlider->setValue(500.0, dontSendNotification);
        delayTimeMenu->setButtonText("-");
        feedbackSlider->setValue(0.5, dontSendNotification);
        lpfFrequencySlider->setValue(5000, dontSendNotification);
        hpfFrequencySlider->setValue(50, dontSendNotification);
        syncButton->setToggleState(1, false);
        
        alphaTouchMenu->setSelectedId(0, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, dontSendNotification);
    }
    
    
    if (syncButton->getToggleState() == 0) //off
        delayTimeSlider->setVisible(true);
    else //on
        delayTimeMenu->setVisible(true);
    
}

//need this?? not currently being used
void GuiDelay::setTempo (float value)
{
    tempo = value;
    //OR
    tempo = AppSettings::Instance()->getGlobalTempo();
}

void GuiDelay::mouseEnter (const MouseEvent &e)
{
    if (wetMixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Wet/Dry Mix. Sets the Wet/Dry mix on the selected pads."));
        parameterHoverLabel->setText(String(wetMixSlider->getValue(), 3), dontSendNotification);
    }
    else if (delayTimeSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Delay Time. Sets the delay time in milliseconds for the selected pads. To set the delay time based on note length, click the 'Sync' button."));
        parameterHoverLabel->setText(String(delayTimeSlider->getValue(), 2), dontSendNotification);
    }
    else if (delayTimeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Delay Time Menu. Sets the delay time in note length for the selected pads. To set the delay time based on note length, click the 'Sync' button."));
    }
    else if (syncButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Tempo Sync. When on the delay will be in sync to the global tempo. When off the tempo can be set in milliseconds."));
    }
    else if (feedbackSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Delay Feedback. Sets the feedback level for the selected pads. Please note that high values cause the delay to continuously feedback and get louder and louder."));
        parameterHoverLabel->setText(String(feedbackSlider->getValue(), 3), dontSendNotification);
    }
    else if (lpfFrequencySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Low Pass Filter Frequency. Sets the cutoff frequency of the Low-pass Filter on the selected pads. Please note that when this value is lower than the Hi-pass Filter cutoff frequency the delayed signal will not be audible."));
        parameterHoverLabel->setText(String(lpfFrequencySlider->getValue(), 0), dontSendNotification);
    }
    else if (hpfFrequencySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("High Pass Filter Frequency. Sets the cutoff frequency of the Hi-pass Filter for the selected pads. Please note that when this value is higher than the Hi-pass Filter cutoff frequency the delayed signal will not be audible."));
        parameterHoverLabel->setText(String(hpfFrequencySlider->getValue(), 0), dontSendNotification);
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

void GuiDelay::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, dontSendNotification);
    
}
