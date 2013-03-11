//
//  GuiEliteControlsSettings.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 17/09/2012.
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

#include "GuiEliteControlsSettings.h"
#include "../../File and Settings/AppSettings.h"
#include "../Views/MainComponent.h"
#include "../Views/GlobalValues.h"
#include "../../Application/CommonInfoBoxText.h"

#define DIAL_NO (currentlySelectedControl - 1)
#define BUTTON_NO (currentlySelectedControl - 3)


GuiEliteControlsSettings::GuiEliteControlsSettings(MainComponent &ref)
                                                : mainComponentRef(ref)
{
    currentlySelectedControl = 0;
    
    addChildComponent(dialsMenu = new ComboBox());
    dialsMenu->addItem(translate("Global Gain"), 1);
    dialsMenu->addItem(translate("Global Pan"), 2);
    dialsMenu->addItem(translate("Scene Switcher"), 3);
    dialsMenu->addItem(translate("MIDI CC"), 4);
    dialsMenu->addItem(translate("OSC"), 5);
    dialsMenu->addListener(this);
    dialsMenu->addMouseListener(this, true);
    
    addChildComponent(buttonsMenu = new ComboBox());
    buttonsMenu->addItem(translate("Start/Stop Clock"), 1);
    buttonsMenu->addItem(translate("Killswitch"), 6);
    buttonsMenu->addItem(translate("Scene Switcher"), 2);
    buttonsMenu->addItem(translate("Save"), 3);
    buttonsMenu->addItem(translate("MIDI CC"), 4);
    buttonsMenu->addItem(translate("OSC"), 5);
    buttonsMenu->addListener(this);
    buttonsMenu->addMouseListener(this, true);
    
    //===============dial stuff===============
    addChildComponent(dialMidiCcNumber = new AlphaSlider());
    dialMidiCcNumber->setRange(0, 127, 1);
    dialMidiCcNumber->addListener(this);
    dialMidiCcNumber->addMouseListener(this, true);
    
    /*addChildComponent(dialMidiChannel = new AlphaSlider());
    dialMidiChannel->setRange(1, 16, 1);
    dialMidiChannel->addListener(this);
    dialMidiChannel->addMouseListener(this, true);*/
	
	//---------------channel buttons---------------------
	for (int i = 0; i <= 15; i++)
	{
		dialMidiChannel.insert(i, new AlphaTextButton());
		dialMidiChannel[i]->setButtonText(String(i + 1));
		dialMidiChannel[i]->setClickingTogglesState(true);
		dialMidiChannel[i]->setToggleState(false, false);	
		dialMidiChannel[i]->setRadioGroupId (12);
		dialMidiChannel[i]->addListener(this);
        dialMidiChannel[i]->addMouseListener(this, true);
		addChildComponent(dialMidiChannel[i]);
	}
    
    /*addChildComponent(dialMidiMinRange = new AlphaSlider());
    dialMidiMinRange->setRange(0, 127, 1);
    dialMidiMinRange->addListener(this);
    dialMidiMinRange->addMouseListener(this, true);*/
	
	addChildComponent(dialMidiControllerType = new AlphaTextButton());
	dialMidiControllerType->setButtonText("ABSOLUTE");
	dialMidiControllerType->setClickingTogglesState(true);
	dialMidiControllerType->setToggleState(false, false);	
	dialMidiControllerType->addListener(this);
	dialMidiControllerType->addMouseListener(this, true);
	
	addChildComponent(dialMidiMinRange = new AlphaRotarySlider((210 * (M_PI / 180)), (470 * (M_PI / 180)), 230));
	dialMidiMinRange->setRotaryParameters((210 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    dialMidiMinRange->setRange(0, 127, 1);
    dialMidiMinRange->addListener(this);
    dialMidiMinRange->addMouseListener(this, true);
    
    /*addChildComponent(dialMidiMaxRange = new AlphaSlider());
    dialMidiMaxRange->setRange(0, 127, 1);
    dialMidiMaxRange->addListener(this);
    dialMidiMaxRange->addMouseListener(this, true);*/
	
	addChildComponent(dialMidiMaxRange = new AlphaRotarySlider((210 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	dialMidiMaxRange->setRotaryParameters((210 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    dialMidiMaxRange->setRange(0, 127, 1);
    dialMidiMaxRange->addListener(this);
    dialMidiMaxRange->addMouseListener(this, true);
    
    //for some reason using a textEditor was creating gui problems, so using a label instead now
    addChildComponent(dialOscIpAddressEditor = new Label());
    dialOscIpAddressEditor->setColour(Label::textColourId, Colours::grey);
    dialOscIpAddressEditor->setColour(Label::backgroundColourId, Colours::lightgrey);
    dialOscIpAddressEditor->setJustificationType(Justification::centred);
    dialOscIpAddressEditor->setEditable(true);
    dialOscIpAddressEditor->addMouseListener(this, true);
    dialOscIpAddressEditor->addListener(this);
    //AT SOME POINT USE LABEL::CREATEEDITORCOMPONENT() TO SET INPUT RESTRICTIONS TO THE EDITOR
    
    addChildComponent(dialOscPortNumber = new AlphaSlider());
    dialOscPortNumber->setRange(0, 65535, 1);
    dialOscPortNumber->addListener(this);
    dialOscPortNumber->addMouseListener(this, true);
    
   /* addChildComponent(dialOscMinRange = new AlphaSlider());
    dialOscMinRange->setRange(0, 4096, 0.01);
    dialOscMinRange->addListener(this);
    dialOscMinRange->addMouseListener(this, true);*/
	
	addChildComponent(dialOscMinRange = new AlphaRotarySlider((210 * (M_PI / 180)), (470 * (M_PI / 180)), 230));
	dialOscMinRange->setRotaryParameters((210 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    dialOscMinRange->setRange(0, 4096, 0.01);
    dialOscMinRange->addListener(this);
    dialOscMinRange->addMouseListener(this, true);
	
	addChildComponent(dialOscMaxRange = new AlphaRotarySlider((210 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	dialOscMaxRange->setRotaryParameters((210 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    dialOscMaxRange->setRange(0, 4096, 0.01);
    dialOscMaxRange->addListener(this);
    dialOscMaxRange->addMouseListener(this, true);
	
	addChildComponent(dialOscStepSlider = new AlphaRotarySlider((210 * (M_PI / 180)), (470 * (M_PI / 180)), 250));
	dialOscStepSlider->setRotaryParameters((210 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    dialOscStepSlider->setRange(0.01, 2048, 0.01);
    dialOscStepSlider->addListener(this);
    dialOscStepSlider->addMouseListener(this, true);
    
   /* addChildComponent(dialOscMaxRange = new AlphaSlider());
    dialOscMaxRange->setRange(0, 4096, 0.01);
    dialOscMaxRange->addListener(this);
    dialOscMaxRange->addMouseListener(this, true);
    */
	
    //===============button stuff===============
    addChildComponent(buttonSceneModeMenu = new ComboBox());
    buttonSceneModeMenu->addItem(translate("Next Scene"), 1);
    buttonSceneModeMenu->addItem(translate("Previous Scene"), 2);
    buttonSceneModeMenu->addItem(translate("Specific Scene"), 3);
    buttonSceneModeMenu->addListener(this);
    buttonSceneModeMenu->addMouseListener(this, true);
    
    addChildComponent(buttonSceneNumber = new AlphaSlider());
    buttonSceneNumber->setRange(1, NO_OF_SCENES, 1);
    buttonSceneNumber->addListener(this);
    buttonSceneNumber->addMouseListener(this, true);
    
    addChildComponent(buttonMidiCcNumber = new AlphaSlider());
    buttonMidiCcNumber->setRange(0, 127, 1);
    buttonMidiCcNumber->addListener(this);
    buttonMidiCcNumber->addMouseListener(this, true);
    
    /*addChildComponent(buttonMidiChannel = new AlphaSlider());
    buttonMidiChannel->setRange(1, 16, 1);
    buttonMidiChannel->addListener(this);
    buttonMidiChannel->addMouseListener(this, true);*/
	
	//---------------channel buttons---------------------
	for (int i = 0; i <= 15; i++)
	{
		buttonMidiChannel.insert(i, new AlphaTextButton());
		buttonMidiChannel[i]->setButtonText(String(i + 1));
		buttonMidiChannel[i]->setClickingTogglesState(true);
		buttonMidiChannel[i]->setToggleState(false, false);	
		buttonMidiChannel[i]->setRadioGroupId (13);
		buttonMidiChannel[i]->addListener(this);
        buttonMidiChannel[i]->addMouseListener(this, true);
		addChildComponent(buttonMidiChannel[i]);
	}
    
   /* addChildComponent(buttonMidiOffNumber = new AlphaSlider());
    buttonMidiOffNumber->setRange(0, 127, 1);
    buttonMidiOffNumber->addListener(this);
    buttonMidiOffNumber->addMouseListener(this, true);
    
    addChildComponent(buttonMidiOnNumber = new AlphaSlider());
    buttonMidiOnNumber->setRange(0, 127, 1);
    buttonMidiOnNumber->addListener(this);
    buttonMidiOnNumber->addMouseListener(this, true);*/
	
	addChildComponent(buttonMidiOffNumber = new AlphaRotarySlider((210 * (M_PI / 180)), (470 * (M_PI / 180)), 230));
	buttonMidiOffNumber->setRotaryParameters((210 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    buttonMidiOffNumber->setRange(0, 127, 1);
    buttonMidiOffNumber->addListener(this);
    buttonMidiOffNumber->addMouseListener(this, true);
	
	addChildComponent(buttonMidiOnNumber = new AlphaRotarySlider((210 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	buttonMidiOnNumber->setRotaryParameters((210 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    buttonMidiOnNumber->setRange(0, 127, 1);
    buttonMidiOnNumber->addListener(this);
    buttonMidiOnNumber->addMouseListener(this, true);
    
    //for some reason using a textEditor was creating gui problems, so using a label instead now
    addChildComponent(buttonOscIpAddressEditor = new Label());
    buttonOscIpAddressEditor->setColour(Label::textColourId, Colours::grey);
    buttonOscIpAddressEditor->setColour(Label::backgroundColourId, Colours::lightgrey);
    buttonOscIpAddressEditor->setJustificationType(Justification::centred);
    buttonOscIpAddressEditor->setEditable(true);
    buttonOscIpAddressEditor->addMouseListener(this, true);
    buttonOscIpAddressEditor->addListener(this);
    //AT SOME POINT USE LABEL::CREATEEDITORCOMPONENT() TO SET INPUT RESTRICTIONS TO THE EDITOR
    
    addChildComponent(buttonOscPortNumber = new AlphaSlider());
    buttonOscPortNumber->setRange(0, 65535, 1);
    buttonOscPortNumber->addListener(this);
    buttonOscPortNumber->addMouseListener(this, true);
    
    /*addChildComponent(buttonOscOffNumber = new AlphaSlider());
    buttonOscOffNumber->setRange(0, 4096, 0.01);
    buttonOscOffNumber->addListener(this);
    buttonOscOffNumber->addMouseListener(this, true);
    
    addChildComponent(buttonOscOnNumber = new AlphaSlider());
    buttonOscOnNumber->setRange(0, 4096, 0.01);
    buttonOscOnNumber->addListener(this);
    buttonOscOnNumber->addMouseListener(this, true);*/
	
	addChildComponent(buttonOscOffNumber = new AlphaRotarySlider((210 * (M_PI / 180)), (470 * (M_PI / 180)), 230));
	buttonOscOffNumber->setRotaryParameters((210 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    buttonOscOffNumber->setRange(0, 4096, 0.01);
    buttonOscOffNumber->addListener(this);
    buttonOscOffNumber->addMouseListener(this, true);
	
	addChildComponent(buttonOscOnNumber = new AlphaRotarySlider((210 * (M_PI / 180)), (470 * (M_PI / 180)), 210));
	buttonOscOnNumber->setRotaryParameters((210 * (M_PI / 180)), (470 * (M_PI / 180)),true);
    buttonOscOnNumber->setRange(0, 4096, 0.01);
    buttonOscOnNumber->addListener(this);
    buttonOscOnNumber->addMouseListener(this, true);
	
	//---------------parameter label -------------------------------------
    addChildComponent(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaColours::blue);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
	
	//---------------status off bg-------------------------------------
    addChildComponent(notSelected = new GuiCircleBackground());
	
	
	addAndMakeVisible(statusButton = new GuiSwitch());
    statusButton->addListener(this);
    statusButton->setClickingTogglesState(true);
    statusButton->setToggleState(true, false);
    statusButton->addMouseListener(this, true);
	
	
}

GuiEliteControlsSettings::~GuiEliteControlsSettings()
{
    
	dialMidiChannel.clear();
	
}

void GuiEliteControlsSettings::resized()
{
    dialsMenu->setBounds(802, 463, 86, 15);
    buttonsMenu->setBounds(802, 463, 86, 15);
    
    dialMidiCcNumber->setBounds(816, 490, 58, 58);
    //dialMidiChannel->setBounds((797-29)+100, 330, 58, 58);
    dialMidiMinRange->setBounds(683 + 47,261 + 47, 230, 230);
    dialMidiMaxRange->setBounds(683 + 57,261 + 57, 210, 210);
    dialMidiControllerType->setBounds(824, 261, 42, 42);
	
	dialMidiChannel[0]->setBounds(649,439,21, 21);
	dialMidiChannel[1]->setBounds(656,467,21, 21);
	dialMidiChannel[2]->setBounds(667,495,21, 21);
	dialMidiChannel[3]->setBounds(682,520,21, 21);
	dialMidiChannel[4]->setBounds(700,542,21, 21);
	dialMidiChannel[5]->setBounds(722,562,21, 21);
	dialMidiChannel[6]->setBounds(747,577,21, 21);
	dialMidiChannel[7]->setBounds(774,589,21, 21);
	dialMidiChannel[8]->setBounds(803,596,21, 21);
	dialMidiChannel[9]->setBounds(832,599,21, 21);
	dialMidiChannel[10]->setBounds(861,597,21, 21);
	dialMidiChannel[11]->setBounds(890,590,21, 21);
	dialMidiChannel[12]->setBounds(917,579,21, 21);
	dialMidiChannel[13]->setBounds(942,564,21, 21);
	dialMidiChannel[14]->setBounds(965,545,21, 21);
	dialMidiChannel[15]->setBounds(984,523,21, 21);
	
	buttonMidiChannel[0]->setBounds(649,439,21, 21);
	buttonMidiChannel[1]->setBounds(656,467,21, 21);
	buttonMidiChannel[2]->setBounds(667,495,21, 21);
	buttonMidiChannel[3]->setBounds(682,520,21, 21);
	buttonMidiChannel[4]->setBounds(700,542,21, 21);
	buttonMidiChannel[5]->setBounds(722,562,21, 21);
	buttonMidiChannel[6]->setBounds(747,577,21, 21);
	buttonMidiChannel[7]->setBounds(774,589,21, 21);
	buttonMidiChannel[8]->setBounds(803,596,21, 21);
	buttonMidiChannel[9]->setBounds(832,599,21, 21);
	buttonMidiChannel[10]->setBounds(861,597,21, 21);
	buttonMidiChannel[11]->setBounds(890,590,21, 21);
	buttonMidiChannel[12]->setBounds(917,579,21, 21);
	buttonMidiChannel[13]->setBounds(942,564,21, 21);
	buttonMidiChannel[14]->setBounds(965,545,21, 21);
	buttonMidiChannel[15]->setBounds(984,523,21, 21);
    
    dialOscPortNumber->setBounds(816, 490, 58, 58);
    dialOscMinRange->setBounds(683 + 47,261 + 47, 230, 230);
    dialOscMaxRange->setBounds(683 + 57,261 + 57, 210, 210);
	dialOscStepSlider->setBounds(683 + 37,261 + 37, 250, 250);
    dialOscIpAddressEditor->setBounds(797, 555, 100, 15);
    
    buttonSceneModeMenu->setBounds(802, 490, 86, 15);
    buttonSceneNumber->setBounds(816, 520, 58, 58);
    
    buttonMidiCcNumber->setBounds(816, 490, 58, 58);
    //buttonMidiChannel->setBounds((797-29)+100, 330, 58, 58);
    buttonMidiOffNumber->setBounds(683 + 47,261 + 47, 230, 230);
    buttonMidiOnNumber->setBounds(683 + 57,261 + 57, 210, 210);
    
    buttonOscPortNumber->setBounds(816, 490, 58, 58);
    buttonOscOffNumber->setBounds(683 + 47,261 + 47, 230, 230);
    buttonOscOnNumber->setBounds(683 + 57,261 + 57, 210, 210);
    buttonOscIpAddressEditor->setBounds(797, 555, 100, 15);
	
	notSelected->setBounds(0, 0, getWidth(), getHeight());
    statusButton->setBounds(816, 393, 58, 58);
    parameterHoverLabel->setBounds(825, 450, 40, 10);
	
}

void GuiEliteControlsSettings::paint (Graphics& g)
{
	
	ColourGradient fillGradient(AlphaColours::nearlyblack,845 , 461, Colours::black, 845 , 383, false);
	g.setGradientFill(fillGradient);
	
	g.fillEllipse(802, 379, 86, 86);
	
	g.setColour(Colours::black);
	Path pieSeg;
	pieSeg.addPieSegment(802, 379, 86, 86, (125 * (M_PI / 180)), (235 * (M_PI / 180)), 0.2f);
	g.fillPath(pieSeg);
	
	if(dialMidiChannel[0]->isVisible() || buttonMidiChannel[0]->isVisible())
	{	
        g.fillEllipse(646,436, 27, 27);
        g.fillEllipse(653,464, 27, 27);
        g.fillEllipse(664,492, 27, 27);
        g.fillEllipse(679,517, 27, 27);
        g.fillEllipse(697,539, 27, 27);
        g.fillEllipse(719,559, 27, 27);
        g.fillEllipse(744,574, 27, 27);
        g.fillEllipse(771,586, 27, 27);
        
        g.fillEllipse(800,593, 27, 27);
        g.fillEllipse(829,596, 27, 27);
        g.fillEllipse(858,594, 27, 27);
        g.fillEllipse(887,587, 27, 27);
        g.fillEllipse(914,576, 27, 27);
        g.fillEllipse(939,561, 27, 27);
        g.fillEllipse(962,542, 27, 27);
        g.fillEllipse(981,520, 27, 27);
        
        g.setColour(Colours::grey.withAlpha(0.3f));
        
        g.drawEllipse(646,436, 27, 27, 1.0);
        g.drawEllipse(653,464, 27, 27, 1.0);
        g.drawEllipse(664,492, 27, 27, 1.0);
        g.drawEllipse(679,517, 27, 27, 1.0);
        g.drawEllipse(697,539, 27, 27, 1.0);
        g.drawEllipse(719,559, 27, 27, 1.0);
        g.drawEllipse(744,574, 27, 27, 1.0);
        g.drawEllipse(771,586, 27, 27, 1.0);
        
        g.drawEllipse(800,593, 27, 27, 1.0);
        g.drawEllipse(829,596, 27, 27, 1.0);
        g.drawEllipse(858,594, 27, 27, 1.0);
        g.drawEllipse(887,587, 27, 27, 1.0);
        g.drawEllipse(914,576, 27, 27, 1.0);
        g.drawEllipse(939,561, 27, 27, 1.0);
        g.drawEllipse(962,542, 27, 27, 1.0);
        g.drawEllipse(981,520, 27, 27, 1.0);
	}
	
//	if(dialMidiChannel[0]->isVisible())
//	{
//		g.setColour(Colours::black);
//		g.fillEllipse(786,218, 48, 48);
//	}
    
}

void GuiEliteControlsSettings::buttonClicked (Button* button)
{
   
	
	if(button == statusButton)
    {
        if (currentlySelectedControl == 1 || currentlySelectedControl == 2) //dials
        {
            AppSettings::Instance()->setEliteDialStatus(button->getToggleState(), DIAL_NO);
        }
        else if (currentlySelectedControl >= 3 && currentlySelectedControl <= 5) //buttons
        {
            AppSettings::Instance()->setEliteButtonStatus(button->getToggleState(), BUTTON_NO);
        }
        
        if(statusButton->getToggleStateValue()==true)
            notSelected->setVisible(false);
        else
            notSelected->setVisible(true);
        
    }
	else if(button == dialMidiControllerType)
	{
		AppSettings::Instance()->setEliteDialMidiControllerType(button->getToggleState(), DIAL_NO);
        
		if(dialMidiControllerType->getToggleStateValue()==true)
        {
			dialMidiControllerType->setButtonText("RELATIVE");
            dialMidiMinRange->setVisible(false);
            dialMidiMaxRange->setVisible(false);
        }
		else 
        {
			dialMidiControllerType->setButtonText("ABSOLUTE");
            dialMidiMinRange->setVisible(true);
            dialMidiMaxRange->setVisible(true);
        }
		

		
	}
	
	for (int chan = 0; chan < 16; chan++)
    {
        if (button == dialMidiChannel[chan])
        {
            
			AppSettings::Instance()->setEliteDialMidiChannel(chan + 1, DIAL_NO);
            
            
            break;
        }
        
    }
	
	for (int chan = 0; chan < 16; chan++)
    {
        if (button == buttonMidiChannel[chan])
        {
            
			AppSettings::Instance()->setEliteButtonMidiChannel(chan + 1, BUTTON_NO);
            
            
            break;
        }
        
    }
	
	
}

void GuiEliteControlsSettings::sliderValueChanged (Slider* slider)
{
    if (slider == dialMidiCcNumber)
        AppSettings::Instance()->setEliteDialMidiCcNumber(slider->getValue(), DIAL_NO);
    /*else if (slider == dialMidiChannel)
        AppSettings::Instance()->setEliteDialMidiChannel(slider->getValue(), DIAL_NO);*/
    else if (slider == dialMidiMinRange)
	{
        AppSettings::Instance()->setEliteDialMidiMinRange(slider->getValue(), DIAL_NO);
		parameterHoverLabel->setText(String(slider->getValue()), false);
        
        if (slider->getValue() >= dialMidiMaxRange->getValue())
            dialMidiMaxRange->setValue(slider->getValue()+2, sendNotification);
            
	}
    else if (slider == dialMidiMaxRange)
	{
        AppSettings::Instance()->setEliteDialMidiMaxRange(slider->getValue(), DIAL_NO);
		parameterHoverLabel->setText(String(slider->getValue()), false);
        
        if (slider->getValue() <= dialMidiMinRange->getValue())
            dialMidiMinRange->setValue(slider->getValue()-2, sendNotification);
	}
    else if (slider == dialOscPortNumber)
        AppSettings::Instance()->setEliteDialOscPortNumber(slider->getValue(), DIAL_NO);
    else if (slider == dialOscMinRange)
	{
        AppSettings::Instance()->setEliteDialOscMinRange(slider->getValue(), DIAL_NO);
		parameterHoverLabel->setText(String(slider->getValue()), false);
        
        setDialOscStepValueSliderRange(DIAL_NO);
	}
    else if (slider == dialOscMaxRange)
	{
        AppSettings::Instance()->setEliteDialOscMaxRange(slider->getValue(), DIAL_NO);
		parameterHoverLabel->setText(String(slider->getValue()), false);
        
        setDialOscStepValueSliderRange(DIAL_NO);
    }
	else if (slider == dialOscStepSlider)
	{
        AppSettings::Instance()->setEliteDialOscStepValue(slider->getValue(), DIAL_NO);
		parameterHoverLabel->setText(String(slider->getValue()), false);
    }
    else if (slider == buttonSceneNumber)
        AppSettings::Instance()->setEliteButtonSceneNumber(slider->getValue(), BUTTON_NO);
    else if (slider == buttonMidiCcNumber)
        AppSettings::Instance()->setEliteButtonMidiCcNumber(slider->getValue(), BUTTON_NO);
    /*else if (slider == buttonMidiChannel)
        AppSettings::Instance()->setEliteButtonMidiChannel(slider->getValue(), BUTTON_NO);*/
    else if (slider == buttonMidiOffNumber)
	{
        AppSettings::Instance()->setEliteButtonMidiOffNumber(slider->getValue(), BUTTON_NO);
		parameterHoverLabel->setText(String(slider->getValue()), false);
	}
    else if (slider == buttonMidiOnNumber)
	{
        AppSettings::Instance()->setEliteButtonMidiOnNumber(slider->getValue(), BUTTON_NO);
		parameterHoverLabel->setText(String(slider->getValue()), false);
	}
    else if (slider == buttonOscPortNumber)
        AppSettings::Instance()->setEliteButtonOscPortNumber(slider->getValue(), BUTTON_NO);
    else if (slider == buttonOscOffNumber)
	{
        AppSettings::Instance()->setEliteButtonOscOffNumber(slider->getValue(), BUTTON_NO);
		parameterHoverLabel->setText(String(slider->getValue()), false);
	}
    else if (slider == buttonOscOnNumber)
	{
        AppSettings::Instance()->setEliteButtonOscOnNumber(slider->getValue(), BUTTON_NO);
		parameterHoverLabel->setText(String(slider->getValue()), false);
	}
    
    
}

void GuiEliteControlsSettings::labelTextChanged (Label* labelThatHasChanged)
{
    if (labelThatHasChanged == dialOscIpAddressEditor)
        AppSettings::Instance()->setEliteDialOscIpAddress(labelThatHasChanged->getText(), DIAL_NO);
    else if (labelThatHasChanged == buttonOscIpAddressEditor)
        AppSettings::Instance()->setEliteButtonOscIpAddress(labelThatHasChanged->getText(), BUTTON_NO);
}

void GuiEliteControlsSettings::comboBoxChanged (ComboBox* comboBox)
{
    if ( comboBox == dialsMenu || comboBox == buttonsMenu)
    {
        //hide all components
        dialMidiCcNumber->setVisible(false);
        //dialMidiChannel->setVisible(false);
        dialMidiMinRange->setVisible(false);
        dialMidiMaxRange->setVisible(false);
		dialMidiControllerType->setVisible(false);
        dialOscPortNumber->setVisible(false);
        dialOscMinRange->setVisible(false);
        dialOscMaxRange->setVisible(false);
		 dialOscStepSlider->setVisible(false);
        dialOscIpAddressEditor->setVisible(false);
        buttonSceneModeMenu->setVisible(false);
        buttonSceneNumber->setVisible(false);
        buttonMidiCcNumber->setVisible(false);
        //buttonMidiChannel->setVisible(false);
        buttonMidiOffNumber->setVisible(false);
        buttonMidiOnNumber->setVisible(false);
        buttonOscPortNumber->setVisible(false);
        buttonOscOffNumber->setVisible(false);
        buttonOscOnNumber->setVisible(false);
        buttonOscIpAddressEditor->setVisible(false);
		
		
		for (int chan = 0; chan < 16; chan++)
		{
			dialMidiChannel[chan]->setVisible(false);
		}
		
		for (int chan = 0; chan < 16; chan++)
		{
			buttonMidiChannel[chan]->setVisible(false);
		}

        
        if (comboBox == dialsMenu)
        {
            AppSettings::Instance()->setEliteDialControl(comboBox->getSelectedId(), DIAL_NO);
            
            switch (comboBox->getSelectedId())
            {
                case 4:
                    dialMidiCcNumber->setVisible(true);
					dialMidiControllerType->setVisible(true);
                    
                    if(dialMidiControllerType->getToggleStateValue()==true)
                    {
                        dialMidiMinRange->setVisible(false);
                        dialMidiMaxRange->setVisible(false);
                    }
                    else 
                    {
                        dialMidiMinRange->setVisible(true);
                        dialMidiMaxRange->setVisible(true);
                    }
					
					for (int chan = 0; chan < 16; chan++)
					{
						dialMidiChannel[chan]->setVisible(true);
					}
					
					
					
                    break;
                case 5:
                    dialOscPortNumber->setVisible(true);
                    dialOscMinRange->setVisible(true);
                    dialOscMaxRange->setVisible(true);
					dialOscStepSlider->setVisible(true);
                    dialOscIpAddressEditor->setVisible(true);
					
                default:
                    break;
            }
            
        }
        else if (comboBox == buttonsMenu)
        {
            AppSettings::Instance()->setEliteButtonControl(comboBox->getSelectedId(), BUTTON_NO);
            
            switch (comboBox->getSelectedId())
            {
                case 2:
                    buttonSceneModeMenu->setVisible(true);
                    if (buttonSceneModeMenu->getSelectedId() == 3)
                        buttonSceneNumber->setVisible(true);
                    break;
                case 4:
                    buttonMidiCcNumber->setVisible(true);
                    //buttonMidiChannel->setVisible(true);
                    buttonMidiOffNumber->setVisible(true);
                    buttonMidiOnNumber->setVisible(true);
					
					for (int chan = 0; chan < 16; chan++)
					{
						buttonMidiChannel[chan]->setVisible(true);
					}
					
					
					
                    break;
                case 5:
                    buttonOscPortNumber->setVisible(true);
                    buttonOscOffNumber->setVisible(true);
                    buttonOscOnNumber->setVisible(true);
                    buttonOscIpAddressEditor->setVisible(true);
					
                    break;
                default:
                    break;
            }
        }
        repaint();
    }
    else if (comboBox == buttonSceneModeMenu)
    {
        
        AppSettings::Instance()->setEliteButtonSceneMode(comboBox->getSelectedId(), BUTTON_NO);
        
        if (buttonSceneModeMenu->getSelectedId() == 3)
            buttonSceneNumber->setVisible(true);
        else
            buttonSceneNumber->setVisible(false);
            
    }
	
}


void GuiEliteControlsSettings::setDialOscStepValueSliderRange (int dialNumber)
{
    //called any time the dialOscMinRange or dialOscMaxRange slider values are changed
    //and updates the dialOscStepSlider range.
    //It also sets an new dialOscStepValue if it is now out of range.
    
    //this is causing some GUI lagging on the corresponding sliders. Why?
    
    double minRange = AppSettings::Instance()->getEliteDialOscMinRange(dialNumber);
    double maxRange = AppSettings::Instance()->getEliteDialOscMaxRange(dialNumber);
    double range = fabs(maxRange-minRange);
    
    dialOscStepSlider->setRange(0.01, range/2.0, 0.01);
    
    double currentStepValue = AppSettings::Instance()->getEliteDialOscStepValue(dialNumber);
    if (currentStepValue > range/2.0)
    {
        dialOscStepSlider->setValue(range/2.0);
        AppSettings::Instance()->setEliteDialOscStepValue(range/2.0, dialNumber);
    }
}

void GuiEliteControlsSettings::setDisplay (int controlNumber)
{
    //control number determines which controls settings should be displayed and editted
    currentlySelectedControl = controlNumber;
    
    //Hide all components
    notSelected->setVisible(false);
    dialsMenu->setVisible(false);
    buttonsMenu->setVisible(false);
    dialMidiCcNumber->setVisible(false);
    //dialMidiChannel->setVisible(false);
    dialMidiMinRange->setVisible(false);
    dialMidiMaxRange->setVisible(false);
	dialMidiControllerType->setVisible(false);
    dialOscPortNumber->setVisible(false);
    dialOscMinRange->setVisible(false);
    dialOscMaxRange->setVisible(false);
	dialOscStepSlider->setVisible(false);
    dialOscIpAddressEditor->setVisible(false);
    buttonSceneModeMenu->setVisible(false);
    buttonSceneNumber->setVisible(false);
    buttonMidiCcNumber->setVisible(false);
    //buttonMidiChannel->setVisible(false);
    buttonMidiOffNumber->setVisible(false);
    buttonMidiOnNumber->setVisible(false);
    buttonOscPortNumber->setVisible(false);
    buttonOscOffNumber->setVisible(false);
    buttonOscOnNumber->setVisible(false);
    buttonOscIpAddressEditor->setVisible(false);
	 
	
	for (int chan = 0; chan < 16; chan++)
	{
		dialMidiChannel[chan]->setVisible(false);
	}
	
	for (int chan = 0; chan < 16; chan++)
	{
		buttonMidiChannel[chan]->setVisible(false);
	}
    
    //Set what component should be displayed and their values
    if (controlNumber == 1 || controlNumber == 2)
    {
        
        statusButton->setToggleState(AppSettings::Instance()->getEliteDialStatus(DIAL_NO), false);
        dialsMenu->setSelectedId(AppSettings::Instance()->getEliteDialControl(DIAL_NO), true);
        dialMidiCcNumber->setValue(AppSettings::Instance()->getEliteDialMidiCcNumber(DIAL_NO));
        //dialMidiChannel->setValue(AppSettings::Instance()->getEliteDialMidiChannel(DIAL_NO));
        dialMidiMinRange->setValue(AppSettings::Instance()->getEliteDialMidiMinRange(DIAL_NO));
        dialMidiMaxRange->setValue(AppSettings::Instance()->getEliteDialMidiMaxRange(DIAL_NO));
		dialMidiControllerType->setToggleState(AppSettings::Instance()->getEliteDialMidiControllerType(DIAL_NO), false);
        dialOscPortNumber->setValue(AppSettings::Instance()->getEliteDialOscPortNumber(DIAL_NO));
        dialOscMinRange->setValue(AppSettings::Instance()->getEliteDialOscMinRange(DIAL_NO));
        dialOscMaxRange->setValue(AppSettings::Instance()->getEliteDialOscMaxRange(DIAL_NO));
        setDialOscStepValueSliderRange(DIAL_NO);
		dialOscStepSlider->setValue(AppSettings::Instance()->getEliteDialOscStepValue(DIAL_NO));
        dialOscIpAddressEditor->setText(AppSettings::Instance()->getEliteDialOscIpAddress(DIAL_NO), false);
		
        if (statusButton->getToggleState() == false)
            notSelected->setVisible(true);
		
		int channel_ = AppSettings::Instance()->getEliteDialMidiChannel(DIAL_NO);
		dialMidiChannel[channel_-1]->setToggleState(true, false);
    
        dialsMenu->setVisible(true);
        
        if (dialsMenu->getSelectedId() == 4)
        {
            dialMidiCcNumber->setVisible(true);
			dialMidiControllerType->setVisible(true);
            
            if(dialMidiControllerType->getToggleStateValue()==true)
            {
                dialMidiControllerType->setButtonText("RELATIVE");
                dialMidiMinRange->setVisible(false);
                dialMidiMaxRange->setVisible(false);
            }
            else 
            {
                dialMidiControllerType->setButtonText("ABSOLUTE");
                dialMidiMinRange->setVisible(true);
                dialMidiMaxRange->setVisible(true);
            }
            
			for (int chan = 0; chan < 16; chan++)
				dialMidiChannel[chan]->setVisible(true);
			
        }
        else if (dialsMenu->getSelectedId() == 5)
        {
            dialOscPortNumber->setVisible(true);
            dialOscMinRange->setVisible(true);
            dialOscMaxRange->setVisible(true);
			dialOscStepSlider->setVisible(true);
            dialOscIpAddressEditor->setVisible(true);
        }
        
    }
    else if (controlNumber == 3 || controlNumber == 4 || controlNumber == 5)
    {
        statusButton->setToggleState(AppSettings::Instance()->getEliteButtonStatus(BUTTON_NO), false);
        buttonsMenu->setSelectedId(AppSettings::Instance()->getEliteButtonControl(BUTTON_NO), true);
        buttonSceneModeMenu->setSelectedId(AppSettings::Instance()->getEliteButtonSceneMode(BUTTON_NO));
        buttonSceneNumber->setValue(AppSettings::Instance()->getEliteButtonSceneNumber(BUTTON_NO));
        buttonMidiCcNumber->setValue(AppSettings::Instance()->getEliteButtonMidiCcNumber(BUTTON_NO));
        //buttonMidiChannel->setValue(AppSettings::Instance()->getEliteButtonMidiChannel(BUTTON_NO));
        buttonMidiOffNumber->setValue(AppSettings::Instance()->getEliteButtonMidiOffNumber(BUTTON_NO));
        buttonMidiOnNumber->setValue(AppSettings::Instance()->getEliteButtonMidiOnNumber(BUTTON_NO));
        buttonOscPortNumber->setValue(AppSettings::Instance()->getEliteButtonOscPortNumber(BUTTON_NO));
        buttonOscOffNumber->setValue(AppSettings::Instance()->getEliteButtonOscOffNumber(BUTTON_NO));
        buttonOscOnNumber->setValue(AppSettings::Instance()->getEliteButtonOscOnNumber(BUTTON_NO));
        buttonOscIpAddressEditor->setText(AppSettings::Instance()->getEliteButtonOscIpAddress(BUTTON_NO), false);

        if (statusButton->getToggleState() == false)
            notSelected->setVisible(true);
        
		int channel_ = AppSettings::Instance()->getEliteButtonMidiChannel(BUTTON_NO);
		for (int i = 0; i <16; i++)
			buttonMidiChannel[i]->setToggleState(0, false);
		
		buttonMidiChannel[channel_-1]->setToggleState(true, false);
    
        buttonsMenu->setVisible(true);
        
        if (buttonsMenu->getSelectedId() == 2)
        {
            buttonSceneModeMenu->setVisible(true);
            
            if (buttonSceneModeMenu->getSelectedId() == 3)
                buttonSceneNumber->setVisible(true);
        }
        else if (buttonsMenu->getSelectedId() == 4)
        {
            buttonMidiCcNumber->setVisible(true);
            //buttonMidiChannel->setVisible(true);
            buttonMidiOffNumber->setVisible(true);
            buttonMidiOnNumber->setVisible(true);
			for (int chan = 0; chan < 16; chan++)
				buttonMidiChannel[chan]->setVisible(true);
			
        }
        else if (buttonsMenu->getSelectedId() == 5)
        {
            buttonOscPortNumber->setVisible(true);
            buttonOscOffNumber->setVisible(true);
            buttonOscOnNumber->setVisible(true);
            buttonOscIpAddressEditor->setVisible(true);
			
        }
    }
	
	repaint();
    
}

void GuiEliteControlsSettings::mouseEnter (const MouseEvent &e)
{
    
	for (int i = 0; i < 16; i++)
    {
        if (dialMidiChannel[i]->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteMidiChannel) + " " + String(i+1) + ".");
            break;
        }
    }
	
	for (int i = 0; i < 16; i++)
    {
        if (buttonMidiChannel[i]->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteMidiChannel) + " " + String(i+1) + ".");
            break;
        }
    }
	
	if (dialsMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteControlMenu));
    }
    else if (buttonsMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteControlMenu));
    }
    else if (dialMidiCcNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteMidiCcNumber));
    }
    /*else if (dialMidiChannel->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteMidiChannel));
    }*/
    else if (dialMidiMinRange->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Minimum MIDI Range Selector. Along with the Maximum Range Selector it sets and displays the MIDI CC data range for the selected elite control."));
    }
    else if (dialMidiMaxRange->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Maximum MIDI Range Selector. Along with the Minimum Range Selector it sets and displays the MIDI CC data range for the selected elite control."));
    }
	else if (dialMidiControllerType->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("MIDI Controller Type Selector. MIDI dials can be set to be either Absolute controllers or Relative controllers. Absolute dials will send an actual CC value, whereas Relative controllers can be used to tell its software control to increment or decrement from its current position by a certain value."));
    }
    else if (dialOscPortNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteOscPortNumber));
    }
    else if (dialOscMinRange->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Minimum OSC Range Selector. Along with the Maximum Range Selector it sets and displays the OSC data range for the selected elite control."));
    }
    else if (dialOscMaxRange->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Maximum OSC Range Selector. Along with the Minimum Range Selector it sets and displays the OSC data range for the selected elite control."));
    }
	else if (dialOscStepSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("OSC Step Amount Selector. Sets and displays how much the OSC value is changed by when the selected elite dial is turned. This value can be no larger than half of the dials OSC range."));
    }
    else if (dialOscIpAddressEditor->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteOscIpAddress));
    }
    else if (buttonSceneModeMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Scene Switcher Mode Selector. Sets and displays which scene the selected elite button is used to switch to."));
    }
    else if (buttonSceneNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Scene Number Selector. Sets and displays the scene number that the selected elite button is used to switch to."));
    }
    else if (buttonMidiCcNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteMidiCcNumber));
    }
    /*else if (buttonMidiChannel->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteMidiChannel));
    }*/
    else if (buttonMidiOffNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("'Off' Value Selector. Sets and displays the MIDI CC value that is sent when the selected elite button is unclicked."));
    }
    else if (buttonMidiOnNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("'On' Value Selector. Sets and displays the MIDI CC value that is sent when the selected elite button is clicked."));
    }
    else if (buttonOscPortNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteOscPortNumber));
    }
    else if (buttonOscOffNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("'Off' Value Selector. Sets and displays the OSC value that is sent when the selected elite button is unclicked."));
    }
    else if (buttonOscOnNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("'On' Value Selector. Sets and displays the OSC value that is sent when the selected elite button is clicked."));
    }
    else if (buttonOscIpAddressEditor->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteOscIpAddress));
    }
    else if (statusButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Elite Control Status Button. Set this button to 'off' to disable the selected elite control."));
    }
    
	if (dialMidiMinRange->isMouseOver(true))
	{
        parameterHoverLabel->setText(String(dialMidiMinRange->getValue()), false);
		parameterHoverLabel->setVisible(true);
	}
    else if (dialMidiMaxRange->isMouseOver(true))
	{
        parameterHoverLabel->setText(String(dialMidiMaxRange->getValue()), false);
		parameterHoverLabel->setVisible(true);
	}
	else if (buttonMidiOffNumber->isMouseOver(true))
	{
        parameterHoverLabel->setText(String(buttonMidiOffNumber->getValue()), false);
		parameterHoverLabel->setVisible(true);
	}
	else if (buttonMidiOnNumber->isMouseOver(true))
	{
        parameterHoverLabel->setText(String(buttonMidiOnNumber->getValue()), false);
		parameterHoverLabel->setVisible(true);
	}
	else if (dialOscMinRange->isMouseOver(true))
	{
        parameterHoverLabel->setText(String(dialOscMinRange->getValue()), false);
		parameterHoverLabel->setVisible(true);
	}
	else if (dialOscMaxRange->isMouseOver(true))
	{
        parameterHoverLabel->setText(String(dialOscMaxRange->getValue()), false);
		parameterHoverLabel->setVisible(true);
	}
	else if (dialOscStepSlider->isMouseOver(true))
	{
        parameterHoverLabel->setText(String(dialOscStepSlider->getValue()), false);
		parameterHoverLabel->setVisible(true);
	}
	else if (buttonOscOffNumber->isMouseOver(true))
	{
        parameterHoverLabel->setText(String(buttonOscOffNumber->getValue()), false);
		parameterHoverLabel->setVisible(true);
	}
	else if (buttonOscOnNumber->isMouseOver(true))
	{
        parameterHoverLabel->setText(String(buttonOscOnNumber->getValue()), false);
		parameterHoverLabel->setVisible(true);
	}
	
}

void GuiEliteControlsSettings::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
	
	if(e.eventComponent == dialMidiMinRange || e.eventComponent == dialMidiMaxRange ||
	   e.eventComponent == buttonMidiOffNumber || e.eventComponent == buttonMidiOnNumber ||
	   e.eventComponent == dialOscMinRange || e.eventComponent == dialOscMaxRange ||
	   e.eventComponent == buttonOscOffNumber || e.eventComponent == buttonOscOnNumber
	   || e.eventComponent == dialOscStepSlider)
	{
        parameterHoverLabel->setText(String::empty, false);
		parameterHoverLabel->setVisible(false);
	}
}