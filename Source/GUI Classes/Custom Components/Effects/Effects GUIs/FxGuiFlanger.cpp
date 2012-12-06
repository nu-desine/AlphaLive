//
//  FxGuiFlanger.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 14/07/2012.
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

#include "FxGuiFlanger.h"
#include "../../../../File and Settings/AppSettings.h"
#include "../../../Views/MainComponent.h"
#include "../../../Binary Data/BinaryDataNew.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiFlanger::GuiFlanger(MainComponent &ref)
:           mainComponentRef(ref)
{
    addAndMakeVisible(mixSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	mixSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    mixSlider->setRange(0.0, 1.0);
    mixSlider->addListener(this);
    mixSlider->addMouseListener(this, true);
    
	addAndMakeVisible(rateSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	rateSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    rateSlider->setRange(0, 20.0);
    rateSlider->addListener(this);
    rateSlider->addMouseListener(this, true);
    
    addAndMakeVisible(feedbackSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	feedbackSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    feedbackSlider->setRange(0.0, 1.0);
    feedbackSlider->addListener(this);
    feedbackSlider->addMouseListener(this, true);
    
    addAndMakeVisible(flangerIntensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 190)); //LFO depth
	flangerIntensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    flangerIntensitySlider->setRange(0.0, 1.0);
    flangerIntensitySlider->addListener(this);
    flangerIntensitySlider->addMouseListener(this, true);
    
    
    addAndMakeVisible(rateMenu = new AlphaPopUpButton());
    rateMenu->addListener(this);
    rateMenu->addMouseListener(this, true);
	rateMenu->setButtonText("1");
    /*rateMenu->addItem(translate("8 Bars"), 1);
    rateMenu->addItem(translate("4 Bars"), 2);
    rateMenu->addItem(translate("2 Bars"), 3);
    rateMenu->addItem(translate("1 Bar"), 4);
    rateMenu->addItem("1/2", 5);
    rateMenu->addItem("1/4", 6);
    rateMenu->addItem("1/8", 7);
    rateMenu->addItem("1/16", 8);
    rateMenu->addItem("1/32", 9);
    rateMenu->setSelectedId(4, true);*/
    
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
    alphaTouchMenu->addItem(translate("Mix"), 2);
    alphaTouchMenu->addItem(translate("Rate"), 3);
    alphaTouchMenu->addItem(translate("Feedback"), 4);
    alphaTouchMenu->addItem(translate("Intensity"), 5);
    alphaTouchMenu->setSelectedId(1, true);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::inverticon_png, BinaryDataNew::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0);
    intensitySlider->setValue(1.0, dontSendNotification);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaColours::lightblue);

    tempo = AppSettings::Instance()->getGlobalTempo();
    
    //---------------parameter label -------------------------------------
    addAndMakeVisible(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaColours::blue);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
    
    setInterceptsMouseClicks(false, true);
}

GuiFlanger::~GuiFlanger()
{
    deleteAllChildren();
}



void GuiFlanger::resized()
{
    
    mixSlider->setBounds(97, 97, 130, 130);
    rateSlider->setBounds(87, 87, 150, 150);
    feedbackSlider->setBounds(77, 77, 170, 170);
	flangerIntensitySlider->setBounds(67, 67, 190, 190);
    
    alphaTouchMenu->setBounds(119, 202, 87, 15);
    reverseButton->setBounds(211,211, 32, 32);
    parameterHoverLabel->setBounds(144, 187, 36, 15);
    intensitySlider->setBounds(57, 57, 210, 210);
    
    rateMenu->setBounds(71, 202, 48, 48);
    syncButton->setBounds(121, 235, 32, 32);
    
    
}

void GuiFlanger::paint (Graphics& g)
{
	
	g.setColour(Colours::black);
	g.fillEllipse(118, 232, 38, 38);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse(118, 232, 38, 38, 1.0f);
	
	
}

void GuiFlanger::sliderValueChanged (Slider *slider)
{
    if (slider == mixSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerMix(mixSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }

    else if (slider == rateSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerRate(rateSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }
    
    
    else if (slider == feedbackSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerFeedback(feedbackSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }
    
    
    else if (slider == flangerIntensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerIntensity(flangerIntensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }
    
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), false);
    }
    
    
    
}

void GuiFlanger::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == alphaTouchMenu)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerAlphaTouch(alphaTouchMenu->getSelectedId());
        }
        
    }
    
    
    
}


void GuiFlanger::buttonClicked (Button *button)
{
    if (button == reverseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerAtReverse(reverseButton->getToggleState());
        }
    }
    
    else if (button == syncButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxFlangerSync(syncButton->getToggleState());
            
            //if set to sync, grab the currently set rate menu value and re-set the rate
            if (syncButton->getToggleState() == true)
            {
                PAD_SETTINGS->setPadFxFlangerRateMenu(PAD_SETTINGS->getPadFxFlangerRateMenu());
                rateSlider->setValue(PAD_SETTINGS->getPadFxFlangerRate(), dontSendNotification);
            }
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
		
		menu.addItem(1, translate("8 Bars"));
		menu.addItem(2, translate("4 Bars"));
		menu.addItem(3, translate("2 Bars"));
		menu.addItem(4, translate("1 Bar"));
		menu.addItem(5, "1/2");
		menu.addItem(6, "1/4");
		menu.addItem(7, "1/8");
		menu.addItem(8, "1/16");
		menu.addItem(9, "1/32");
		
        const int result = menu.show();
        
        if (result != 0) //if the user selects something
        {
            switch (result)
            {
                case 1: // 8 bars
                    rateMenu->setButtonText("8");
                    break;
                case 2: // 4 bars
                    rateMenu->setButtonText("4");
                    break;
                case 3: // 2 bars
                    rateMenu->setButtonText("2");
                    break;
                case 4: // 1 bar - 4 beats
                    rateMenu->setButtonText("1");
                    break;
                case 5: // 1/2 - 2 beats
                    rateMenu->setButtonText("1/2");
                    break;
                case 6: // 1/4 - 1 beat
                    rateMenu->setButtonText("1/4");
                    break;
                case 7: // 1/8 - half beat
                    rateMenu->setButtonText("1/8");
                    break;
                case 8: // 1/16 - quarter beat
                    rateMenu->setButtonText("1/16");
                    break;
                case 9: // 1/32 - 8th beat
                    rateMenu->setButtonText("1/32");
                    break;
                default: // 1/4 - 1 beat
                    rateMenu->setButtonText("1/4");
                    break;
            }
            
            
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setPadFxFlangerRateMenu(result); //in turn sets the actual rate
            }
            
            rateSlider->setValue(AppSettings::Instance()->padSettings[selectedPads[0]]->getPadFxFlangerRate(), dontSendNotification);
        }
    }
}

void GuiFlanger::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
}

void GuiFlanger::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings.
    rateSlider->setVisible(false);
    rateMenu->setVisible(false);
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        mixSlider->setValue(PAD_SETTINGS->getPadFxFlangerMix(), dontSendNotification);
        rateSlider->setValue(PAD_SETTINGS->getPadFxFlangerRate(), dontSendNotification);
        feedbackSlider->setValue(PAD_SETTINGS->getPadFxFlangerFeedback(), dontSendNotification);
        flangerIntensitySlider->setValue(PAD_SETTINGS->getPadFxFlangerIntensity(), dontSendNotification);
        syncButton->setToggleState(PAD_SETTINGS->getPadFxFlangerSync(), false);
		
		switch (PAD_SETTINGS->getPadFxFlangerRateMenu())
        {
            case 1: // 8 bars
                
				rateMenu->setButtonText("8");
                break;
            case 2: // 4 bars
              
				rateMenu->setButtonText("4");
                break;
            case 3: // 2 bars
               
				rateMenu->setButtonText("2");
                break;
            case 4: // 1 bar - 4 beats
               
				rateMenu->setButtonText("1");
                break;
            case 5: // 1/2 - 2 beats
            
				rateMenu->setButtonText("1/2");
                break;
            case 6: // 1/4 - 1 beat
              
				rateMenu->setButtonText("1/4");
                break;
            case 7: // 1/8 - half beat
                
				rateMenu->setButtonText("1/8");
                break;
            case 8: // 1/16 - quarter beat
              
				rateMenu->setButtonText("1/16");
                break;
            case 9: // 1/32 - 8th beat
                
				rateMenu->setButtonText("1/32");
                break;
            default: // 1/4 - 1 beat
               
				rateMenu->setButtonText("1/4");
                break;
                
        }
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxFlangerAlphaTouch(), true);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxFlangerAtReverse(), false);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxFlangerAtIntensity(), dontSendNotification);
    }
    
    else if(MULTI_PADS)
    {
        mixSlider->setValue(0.7, dontSendNotification);
        rateSlider->setValue(0.25, dontSendNotification);
        feedbackSlider->setValue(0.9, dontSendNotification);
        flangerIntensitySlider->setValue(0.1, dontSendNotification);
        rateMenu->setButtonText("-");
        syncButton->setToggleState(1, false);
        
        alphaTouchMenu->setSelectedId(0, true);
        reverseButton->setToggleState(0, false);
        intensitySlider->setValue(1.0, dontSendNotification);
    }
    
    
    if (syncButton->getToggleState() == 0) //off
        rateSlider->setVisible(true);
    else //on
        rateMenu->setVisible(true);
    
}

//need this?? not currently being used
void GuiFlanger::setTempo (float value)
{
    tempo = value;
    //OR
    tempo = AppSettings::Instance()->getGlobalTempo();
}

void GuiFlanger::mouseEnter (const MouseEvent &e)
{
    if (mixSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Wet/Dry Mix. Sets the Wet/Dry mix of the Flanger effect for the selected pads."));
        parameterHoverLabel->setText(String(mixSlider->getValue(), 3), false);
    }
    else if (rateSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("LFO Rate. Sets the rate in Hz for the selected pads. To set the rate based on the tempo, click on the 'Sync' button."));
        parameterHoverLabel->setText(String(rateSlider->getValue(), 3), false);
    }
    else if (rateMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("LFO Rate Menu. Sets the LFO rate of the selected pads. To set the rate in Hz, click on the 'Sync' button."));
    }
    else if (syncButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Tempo Sync. When on the LFO will sync to the tempo. When off the LFO rate is set in Hz."));
    }
    else if (feedbackSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Feedback Selector. Sets the flanger feedback on the selected pads."));
        parameterHoverLabel->setText(String(feedbackSlider->getValue(), 3), false);
    }
    else if (flangerIntensitySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Flanger Intensity Selector. Sets the intensity of the effect on the selected pads."));
        parameterHoverLabel->setText(String(flangerIntensitySlider->getValue(), 3), false);
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

void GuiFlanger::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, false);
    
}
