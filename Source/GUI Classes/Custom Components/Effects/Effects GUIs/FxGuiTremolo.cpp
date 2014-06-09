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
#include "../../../Binary Data/MainBinaryData.h"
#include "../../../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)


GuiTremolo::GuiTremolo(MainComponent &ref)
:           mainComponentRef(ref)
{
    addAndMakeVisible(depthSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 130));
	depthSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    depthSlider->setRange(0.0, 1.0, 0.001);
    depthSlider->addListener(this);
    depthSlider->addMouseListener(this, true);
    
	addAndMakeVisible(rateSlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 150));
	rateSlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    rateSlider->setRange(0, 50.0, 0.01);
    rateSlider->addListener(this);
    rateSlider->addMouseListener(this, true);
    
    addAndMakeVisible(intensitySlider = new AlphaRotarySlider((250 * (M_PI / 180)), (470 * (M_PI / 180)), 170));
	intensitySlider->setRotaryParameters((250 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    intensitySlider->setRange(0.0, 1.0, 0.001);
    intensitySlider->setValue(0.5, dontSendNotification);
    intensitySlider->addListener(this);
    intensitySlider->addMouseListener(this, true);
    
    
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
    
    Image *syncImage = new Image(ImageCache::getFromMemory(MainBinaryData::syncicon_png, MainBinaryData::syncicon_pngSize));
    addAndMakeVisible(syncButton = new ModeButton(syncImage));
    syncButton->setClickingTogglesState(true);
    syncButton->setToggleState(1, dontSendNotification);
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
	
	Image *sineIcon = new Image(ImageCache::getFromMemory(MainBinaryData::sinesymbol_png, MainBinaryData::sinesymbol_pngSize));
	Image *squareIcon = new Image(ImageCache::getFromMemory(MainBinaryData::squaresymbol_png, MainBinaryData::squaresymbol_pngSize));
	Image *triangleIcon = new Image(ImageCache::getFromMemory(MainBinaryData::trianglesymbol_png, MainBinaryData::trianglesymbol_pngSize));
	Image *sawRiseIcon = new Image(ImageCache::getFromMemory(MainBinaryData::sawrisingsymbol_png, MainBinaryData::sawrisingsymbol_pngSize));
	Image *sawFallIcon = new Image(ImageCache::getFromMemory(MainBinaryData::sawfallingsymbol_png, MainBinaryData::sawfallingsymbol_pngSize));
	
	
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
    
    shapeMenuButtons[0]->setToggleState(true, dontSendNotification);
    
    
    addAndMakeVisible(alphaTouchMenu = new ComboBox());
    alphaTouchMenu->addListener(this);
    alphaTouchMenu->addMouseListener(this, true);
    alphaTouchMenu->addItem(translate("Off"), 1);
    alphaTouchMenu->addItem(translate("Depth/Mix"), 2);
    alphaTouchMenu->addItem(translate("Rate"), 3);
    alphaTouchMenu->addItem(translate("Wave Shape"), 4);
    alphaTouchMenu->setSelectedId(1, dontSendNotification);
    
    Image *reverseIcon = new Image(ImageCache::getFromMemory(MainBinaryData::inverticon_png, MainBinaryData::inverticon_pngSize));
    addAndMakeVisible(reverseButton = new ModeButton(reverseIcon));
    reverseButton->setClickingTogglesState(true);
    reverseButton->addListener(this);
    reverseButton->addMouseListener(this, true);
    
    //---------------parameter label -------------------------------------
    addAndMakeVisible(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setFont(Font(11 + AlphaTheme::getInstance()->fontSizeAddition));
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
    parameterHoverLabel->setColour(Label::textColourId, AlphaTheme::getInstance()->mainColour);
    intensitySlider->setColour(Slider::rotarySliderFillColourId, AlphaTheme::getInstance()->mainColourLighter);
    
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	g.fillEllipse(118, 232, 38, 38);
	
	g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
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
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
        
    }
    
    else if (slider == rateSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxTremoloRate(rateSlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
        
    }
    
    
    else if (slider == intensitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setPadFxTremoloAtIntensity(intensitySlider->getValue());
        }
        
        parameterHoverLabel->setText(String(slider->getValue(), 3), dontSendNotification);
        
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
            
            //if set to sync, grab the currently set rate menu value and re-set the rate
            if (syncButton->getToggleState() == true)
            {
                PAD_SETTINGS->setPadFxTremoloRateMenu(PAD_SETTINGS->getPadFxTremoloRateMenu());
                rateSlider->setValue(PAD_SETTINGS->getPadFxTremoloRate(), dontSendNotification);
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
		
		menu.addItem(1, "1/1");
		menu.addItem(2, "1/2");
		menu.addItem(3, "1/4");
		menu.addItem(4, "1/8");
		menu.addItem(5, "1/16");
		menu.addItem(6, "1/32");
		menu.addItem(7, "1/64");
		
		const int result = menu.show();

        if (result != 0) //if the user selects something
        {
            switch (result)
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

            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setPadFxTremoloRateMenu(result); //in turn sets the actual rate
            }
            
            rateSlider->setValue(AppSettings::Instance()->padSettings[selectedPads[0]]->getPadFxTremoloRate(), dontSendNotification);
        }
        
    }
	for (int waveform = 0; waveform < 5; waveform++)
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
        depthSlider->setValue(PAD_SETTINGS->getPadFxTremoloDepth(), dontSendNotification);
        rateSlider->setValue(PAD_SETTINGS->getPadFxTremoloRate(), dontSendNotification);
        syncButton->setToggleState(PAD_SETTINGS->getPadFxTremoloSync(), dontSendNotification);
		shapeMenuButtons[PAD_SETTINGS->getPadFxTremoloShape()-1]->setToggleState(true, dontSendNotification);
        
        alphaTouchMenu->setSelectedId(PAD_SETTINGS->getPadFxTremoloAlphaTouch(), dontSendNotification);
        reverseButton->setToggleState(PAD_SETTINGS->getPadFxTremoloAtReverse(), dontSendNotification);
        intensitySlider->setValue(PAD_SETTINGS->getPadFxTremoloAtIntensity(), dontSendNotification);
		
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
        depthSlider->setValue(0.7, dontSendNotification);
        rateSlider->setValue(5.0, dontSendNotification);
        rateMenu->setButtonText("-");
        syncButton->setToggleState(true, dontSendNotification);
        //shapeMenu->setSelectedId(2, true);
        
        for (int j = 0; j < 5; j++)
            shapeMenuButtons[j]->setToggleState(false, dontSendNotification);
        
        alphaTouchMenu->setSelectedId(0, dontSendNotification);
        reverseButton->setToggleState(0, dontSendNotification);
        intensitySlider->setValue(1.0, dontSendNotification);
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
        mainComponentRef.setInfoTextBoxText(translate("Depth/Mix. Sets the depth/mix of the LFO on the selected pads."));
        parameterHoverLabel->setText(String(depthSlider->getValue(), 3), dontSendNotification);
    }
    else if (rateSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("LFO Rate. Sets the rate in Hz for the selected pads. To set the rate based on the tempo, click on the 'Sync' button."));
        parameterHoverLabel->setText(String(rateSlider->getValue(), 3), dontSendNotification);
    }
    else if (rateMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("LFO Rate Menu. Sets the LFO rate of the selected pads. To set the rate in Hz, click on the 'Sync' button."));
    }
    else if (syncButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Tempo Sync. When on the LFO will sync to the tempo. When off the LFO rate is set in Hz."));
    }
    else if (shapeMenuButtons[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sine wave. Sets the LFO wave shape of the selected pads."));
    }
	else if (shapeMenuButtons[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Square wave. Sets the LFO wave shape of the selected pads."));
    }
	else if (shapeMenuButtons[2]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Triangle wave. Sets the LFO wave shape of the selected pads."));
    }
	else if (shapeMenuButtons[3]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sawtooth Rising wave. Sets the LFO wave shape of the selected pads."));
    }
	else if (shapeMenuButtons[4]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Sawtooth Falling wave. Sets the LFO wave shape of the selected pads."));
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

void GuiTremolo::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
    parameterHoverLabel->setText(String::empty, dontSendNotification);
    
}
