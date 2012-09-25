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
    
    addChildComponent(dialMidiChannel = new AlphaSlider());
    dialMidiChannel->setRange(1, 16, 1);
    dialMidiChannel->addListener(this);
    dialMidiChannel->addMouseListener(this, true);
    
    addChildComponent(dialMidiMinRange = new AlphaSlider());
    dialMidiMinRange->setRange(0, 127, 1);
    dialMidiMinRange->addListener(this);
    dialMidiMinRange->addMouseListener(this, true);
    
    addChildComponent(dialMidiMaxRange = new AlphaSlider());
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
    
    addChildComponent(dialOscMinRange = new AlphaSlider());
    dialOscMinRange->setRange(0, 4096, 0.01);
    dialOscMinRange->addListener(this);
    dialOscMinRange->addMouseListener(this, true);
    
    addChildComponent(dialOscMaxRange = new AlphaSlider());
    dialOscMaxRange->setRange(0, 4096, 0.01);
    dialOscMaxRange->addListener(this);
    dialOscMaxRange->addMouseListener(this, true);
    
    //===============button stuff===============
    addChildComponent(buttonSceneNumber = new AlphaSlider());
    buttonSceneNumber->setRange(1, NO_OF_SCENES, 1);
    buttonSceneNumber->addListener(this);
    buttonSceneNumber->addMouseListener(this, true);
    
    addChildComponent(buttonMidiCcNumber = new AlphaSlider());
    buttonMidiCcNumber->setRange(0, 127, 1);
    buttonMidiCcNumber->addListener(this);
    buttonMidiCcNumber->addMouseListener(this, true);
    
    addChildComponent(buttonMidiChannel = new AlphaSlider());
    buttonMidiChannel->setRange(1, 16, 1);
    buttonMidiChannel->addListener(this);
    buttonMidiChannel->addMouseListener(this, true);
    
    addChildComponent(buttonMidiOffNumber = new AlphaSlider());
    buttonMidiOffNumber->setRange(0, 127, 1);
    buttonMidiOffNumber->addListener(this);
    buttonMidiOffNumber->addMouseListener(this, true);
    
    addChildComponent(buttonMidiOnNumber = new AlphaSlider());
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
    
    addChildComponent(buttonOscOffNumber = new AlphaSlider());
    buttonOscOffNumber->setRange(0, 4096, 0.01);
    buttonOscOffNumber->addListener(this);
    buttonOscOffNumber->addMouseListener(this, true);
    
    addChildComponent(buttonOscOnNumber = new AlphaSlider());
    buttonOscOnNumber->setRange(0, 4096, 0.01);
    buttonOscOnNumber->addListener(this);
    buttonOscOnNumber->addMouseListener(this, true);
}

GuiEliteControlsSettings::~GuiEliteControlsSettings()
{
    
}

void GuiEliteControlsSettings::resized()
{
    dialsMenu->setBounds(797, 300, 100, 20);
    buttonsMenu->setBounds(797, 300, 100, 20);
    
    dialMidiCcNumber->setBounds(797-29, 330, 58, 58);
    dialMidiChannel->setBounds((797-29)+100, 330, 58, 58);
    dialMidiMinRange->setBounds(797-29, 400, 58, 58);
    dialMidiMaxRange->setBounds((797-29)+100, 400, 58, 58);
    
    dialOscPortNumber->setBounds((797-29)+50, 330, 58, 58);
    dialOscMinRange->setBounds(797-29, 400, 58, 58);
    dialOscMaxRange->setBounds((797-29)+100, 400, 58, 58);
    dialOscIpAddressEditor->setBounds(797, 470, 100, 20);
    
    buttonSceneNumber->setBounds((797-29)+50, 330, 58, 58);
    
    buttonMidiCcNumber->setBounds(797-29, 330, 58, 58);
    buttonMidiChannel->setBounds((797-29)+100, 330, 58, 58);
    buttonMidiOffNumber->setBounds(797-29, 400, 58, 58);
    buttonMidiOnNumber->setBounds((797-29)+100, 400, 58, 58);
    
    buttonOscPortNumber->setBounds((797-29)+50, 330, 58, 58);
    buttonOscOffNumber->setBounds(797-29, 400, 58, 58);
    buttonOscOnNumber->setBounds((797-29)+100, 400, 58, 58);
    buttonOscIpAddressEditor->setBounds(797, 470, 100, 20);
}

void GuiEliteControlsSettings::paint (Graphics& g)
{
    
}

void GuiEliteControlsSettings::buttonClicked (Button* button)
{
    
}

void GuiEliteControlsSettings::sliderValueChanged (Slider* slider)
{
    if (slider == dialMidiCcNumber)
        AppSettings::Instance()->setEliteDialMidiCcNumber(slider->getValue(), DIAL_NO);
    else if (slider == dialMidiChannel)
        AppSettings::Instance()->setEliteDialMidiChannel(slider->getValue(), DIAL_NO);
    else if (slider == dialMidiMinRange)
        AppSettings::Instance()->setEliteDialMidiMinRange(slider->getValue(), DIAL_NO);
    else if (slider == dialMidiMaxRange)
        AppSettings::Instance()->setEliteDialMidiMaxRange(slider->getValue(), DIAL_NO);
    else if (slider == dialOscPortNumber)
        AppSettings::Instance()->setEliteDialOscPortNumber(slider->getValue(), DIAL_NO);
    else if (slider == dialOscMinRange)
        AppSettings::Instance()->setEliteDialOscMinRange(slider->getValue(), DIAL_NO);
    else if (slider == dialOscMaxRange)
        AppSettings::Instance()->setEliteDialOscMaxRange(slider->getValue(), DIAL_NO);
    
    else if (slider == buttonSceneNumber)
        AppSettings::Instance()->setEliteButtonSceneNumber(slider->getValue(), BUTTON_NO);
    else if (slider == buttonMidiCcNumber)
        AppSettings::Instance()->setEliteButtonMidiCcNumber(slider->getValue(), BUTTON_NO);
    else if (slider == buttonMidiChannel)
        AppSettings::Instance()->setEliteButtonMidiChannel(slider->getValue(), BUTTON_NO);
    else if (slider == buttonMidiOffNumber)
        AppSettings::Instance()->setEliteButtonMidiOffNumber(slider->getValue(), BUTTON_NO);
    else if (slider == buttonMidiOnNumber)
        AppSettings::Instance()->setEliteButtonMidiOnNumber(slider->getValue(), BUTTON_NO);
    else if (slider == buttonOscPortNumber)
        AppSettings::Instance()->setEliteButtonOscPortNumber(slider->getValue(), BUTTON_NO);
    else if (slider == buttonOscOffNumber)
        AppSettings::Instance()->setEliteButtonOscOffNumber(slider->getValue(), BUTTON_NO);
    else if (slider == buttonOscOnNumber)
        AppSettings::Instance()->setEliteButtonOscOnNumber(slider->getValue(), BUTTON_NO);
    
    
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
        dialMidiChannel->setVisible(false);
        dialMidiMinRange->setVisible(false);
        dialMidiMaxRange->setVisible(false);
        dialOscPortNumber->setVisible(false);
        dialOscMinRange->setVisible(false);
        dialOscMaxRange->setVisible(false);
        dialOscIpAddressEditor->setVisible(false);
        buttonSceneNumber->setVisible(false);
        buttonMidiCcNumber->setVisible(false);
        buttonMidiChannel->setVisible(false);
        buttonMidiOffNumber->setVisible(false);
        buttonMidiOnNumber->setVisible(false);
        buttonOscPortNumber->setVisible(false);
        buttonOscOffNumber->setVisible(false);
        buttonOscOnNumber->setVisible(false);
        buttonOscIpAddressEditor->setVisible(false);
        
        if (comboBox == dialsMenu)
        {
            AppSettings::Instance()->setEliteDialControl(comboBox->getSelectedId(), DIAL_NO);
            
            switch (comboBox->getSelectedId())
            {
                case 4:
                    dialMidiCcNumber->setVisible(true);
                    dialMidiChannel->setVisible(true);
                    dialMidiMinRange->setVisible(true);
                    dialMidiMaxRange->setVisible(true);
                    break;
                case 5:
                    dialOscPortNumber->setVisible(true);
                    dialOscMinRange->setVisible(true);
                    dialOscMaxRange->setVisible(true);
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
                    buttonSceneNumber->setVisible(true);
                    break;
                case 4:
                    buttonMidiCcNumber->setVisible(true);
                    buttonMidiChannel->setVisible(true);
                    buttonMidiOffNumber->setVisible(true);
                    buttonMidiOnNumber->setVisible(true);
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
  
        
        
    }
}

void GuiEliteControlsSettings::setDisplay (int controlNumber)
{
    //control number determines which controls settings should be displayed and editted
    currentlySelectedControl = controlNumber;
    
    //Hide all components
    dialsMenu->setVisible(false);
    buttonsMenu->setVisible(false);
    dialMidiCcNumber->setVisible(false);
    dialMidiChannel->setVisible(false);
    dialMidiMinRange->setVisible(false);
    dialMidiMaxRange->setVisible(false);
    dialOscPortNumber->setVisible(false);
    dialOscMinRange->setVisible(false);
    dialOscMaxRange->setVisible(false);
    dialOscIpAddressEditor->setVisible(false);
    buttonSceneNumber->setVisible(false);
    buttonMidiCcNumber->setVisible(false);
    buttonMidiChannel->setVisible(false);
    buttonMidiOffNumber->setVisible(false);
    buttonMidiOnNumber->setVisible(false);
    buttonOscPortNumber->setVisible(false);
    buttonOscOffNumber->setVisible(false);
    buttonOscOnNumber->setVisible(false);
    buttonOscIpAddressEditor->setVisible(false);
    
    //Set what component should be displayed and their values
    if (controlNumber == 1 || controlNumber == 2)
    {
        dialsMenu->setSelectedId(AppSettings::Instance()->getEliteDialControl(DIAL_NO), true);
        dialMidiCcNumber->setComponentValue(AppSettings::Instance()->getEliteDialMidiCcNumber(DIAL_NO));
        dialMidiChannel->setComponentValue(AppSettings::Instance()->getEliteDialMidiChannel(DIAL_NO));
        dialMidiMinRange->setComponentValue(AppSettings::Instance()->getEliteDialMidiMinRange(DIAL_NO));
        dialMidiMaxRange->setComponentValue(AppSettings::Instance()->getEliteDialMidiMaxRange(DIAL_NO));
        dialOscPortNumber->setComponentValue(AppSettings::Instance()->getEliteDialOscPortNumber(DIAL_NO));
        dialOscMinRange->setComponentValue(AppSettings::Instance()->getEliteDialOscMinRange(DIAL_NO));
        dialOscMaxRange->setComponentValue(AppSettings::Instance()->getEliteDialOscMaxRange(DIAL_NO));
        dialOscIpAddressEditor->setText(AppSettings::Instance()->getEliteDialOscIpAddress(DIAL_NO), false);
        
        dialsMenu->setVisible(true);
        
        if (dialsMenu->getSelectedId() == 4)
        {
            dialMidiCcNumber->setVisible(true);
            dialMidiChannel->setVisible(true);
            dialMidiMinRange->setVisible(true);
            dialMidiMaxRange->setVisible(true);
        }
        else if (dialsMenu->getSelectedId() == 5)
        {
            dialOscPortNumber->setVisible(true);
            dialOscMinRange->setVisible(true);
            dialOscMaxRange->setVisible(true);
            dialOscIpAddressEditor->setVisible(true);
        }
        
    }
    else if (controlNumber == 3 || controlNumber == 4 || controlNumber == 5)
    {
        buttonsMenu->setSelectedId(AppSettings::Instance()->getEliteButtonControl(BUTTON_NO), true);
        buttonSceneNumber->setComponentValue(AppSettings::Instance()->getEliteButtonSceneNumber(BUTTON_NO));
        buttonMidiCcNumber->setComponentValue(AppSettings::Instance()->getEliteButtonMidiCcNumber(BUTTON_NO));
        buttonMidiChannel->setComponentValue(AppSettings::Instance()->getEliteButtonMidiChannel(BUTTON_NO));
        buttonMidiOffNumber->setComponentValue(AppSettings::Instance()->getEliteButtonMidiOffNumber(BUTTON_NO));
        buttonMidiOnNumber->setComponentValue(AppSettings::Instance()->getEliteButtonMidiOnNumber(BUTTON_NO));
        buttonOscPortNumber->setComponentValue(AppSettings::Instance()->getEliteButtonOscPortNumber(BUTTON_NO));
        buttonOscOffNumber->setComponentValue(AppSettings::Instance()->getEliteButtonOscOffNumber(BUTTON_NO));
        buttonOscOnNumber->setComponentValue(AppSettings::Instance()->getEliteButtonOscOnNumber(BUTTON_NO));
        buttonOscIpAddressEditor->setText(AppSettings::Instance()->getEliteButtonOscIpAddress(BUTTON_NO), false);

        buttonsMenu->setVisible(true);
        
        if (buttonsMenu->getSelectedId() == 2)
        {
            buttonSceneNumber->setVisible(true);
        }
        else if (buttonsMenu->getSelectedId() == 4)
        {
            buttonMidiCcNumber->setVisible(true);
            buttonMidiChannel->setVisible(true);
            buttonMidiOffNumber->setVisible(true);
            buttonMidiOnNumber->setVisible(true);
        }
        else if (buttonsMenu->getSelectedId() == 5)
        {
            buttonOscPortNumber->setVisible(true);
            buttonOscOffNumber->setVisible(true);
            buttonOscOnNumber->setVisible(true);
            buttonOscIpAddressEditor->setVisible(true);
        }
    }
    
}

void GuiEliteControlsSettings::mouseEnter (const MouseEvent &e)
{
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
    else if (dialMidiChannel->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteMidiChannel));
    }
    else if (dialMidiMinRange->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Minimum MIDI Range Selector. Along with the Maximum Range Selector it sets and displays the MIDI CC data range for the selected elite control."));
    }
    else if (dialMidiMaxRange->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Maximum MIDI Range Selector. Along with the Minimum Range Selector it sets and displays the MIDI CC data range for the selected elite control."));
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
    else if (dialOscIpAddressEditor->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteOscIpAddress));
    }
    
    else if (buttonSceneNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Scene Number Selector. Sets and displays the scene number that the selected elite button is used to switch to."));
    }
    else if (buttonMidiCcNumber->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteMidiCcNumber));
    }
    else if (buttonMidiChannel->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::eliteMidiChannel));
    }
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
    
}

void GuiEliteControlsSettings::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}