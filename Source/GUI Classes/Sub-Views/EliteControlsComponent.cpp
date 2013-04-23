//
//  EliteControlsComponent.cpp
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

#include "EliteControlsComponent.h"
#include "../Views/MainComponent.h"
#include "../AlphaLiveLookandFeel.h"
#include "../../Application/CommonInfoBoxText.h"
#include "../Binary Data/BinaryDataNew.h"

#define X_OFFSET 44
#define Y_OFFSET 537

enum
{
    DIAL_1 = 1,
    DIAL_2,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
};

EliteControlsComponent::EliteControlsComponent(MainComponent &ref)
        : mainComponentRef(ref)
{
    Path buttonShape;
    buttonShape.addRectangle(0, 0, 30, 30);
	
	Image *dial1Image = new Image(ImageCache::getFromMemory(BinaryDataNew::controlsymbol_png, BinaryDataNew::controlsymbol_pngSize)); 
	addAndMakeVisible(dial1Button = new ModeButton(dial1Image));
	dial1Button->addListener(this);
	dial1Button->setRadioGroupId (84);
    dial1Button->addMouseListener(this, false);
	
	Image *dial2Image = new Image(ImageCache::getFromMemory(BinaryDataNew::controlsymbol_png, BinaryDataNew::controlsymbol_pngSize)); 
	addAndMakeVisible(dial2Button = new ModeButton(dial2Image));
	dial2Button->addListener(this);
	dial2Button->setRadioGroupId (84);
    dial2Button->addMouseListener(this, false);
	
	addAndMakeVisible(button1Button = new AlphaSquareButton(2.0f));
	button1Button->addListener(this);
	button1Button->setRadioGroupId (84);
    button1Button->addMouseListener(this, false);
	
	addAndMakeVisible(button2Button = new AlphaSquareButton(2.0f));
	button2Button->addListener(this);
	button2Button->setRadioGroupId (84);
    button2Button->addMouseListener(this, false);
	
	addAndMakeVisible(button3Button = new AlphaSquareButton(2.0f));
	button3Button->addListener(this);
	button3Button->setRadioGroupId (84);
    button3Button->addMouseListener(this, false);
    
    dialRotationValue[0] = dialRotationValue[1] = 0;
    buttonStatusValue[0] = buttonStatusValue[1] = buttonStatusValue[2] = 0;
    
}

EliteControlsComponent::~EliteControlsComponent()
{
    deleteAllChildren();
}

void EliteControlsComponent::resized()
{
    dial1Button->setBounds(58-X_OFFSET, 545-Y_OFFSET, 32, 32);
    dial2Button->setBounds(102-X_OFFSET, 545-Y_OFFSET, 32, 32);
    
    button1Button->setBounds(52-X_OFFSET, 593-Y_OFFSET, 20, 20);
    button2Button->setBounds(86-X_OFFSET, 593-Y_OFFSET, 20, 20);
    button3Button->setBounds(120-X_OFFSET, 593-Y_OFFSET, 20, 20);
}

void EliteControlsComponent::paint (Graphics& g)
{
	
	g.setColour(Colours::black);
	
	g.fillRoundedRectangle(49-X_OFFSET, 590-Y_OFFSET, 26, 26, 2);
	g.fillRoundedRectangle(83-X_OFFSET, 590-Y_OFFSET, 26, 26, 2);
	g.fillRoundedRectangle(117-X_OFFSET, 590-Y_OFFSET, 26, 26, 2);
	g.fillEllipse(55-X_OFFSET, 542-Y_OFFSET, 38, 38);
	g.fillEllipse(99-X_OFFSET, 542-Y_OFFSET, 38, 38);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	
	g.drawRoundedRectangle(49-X_OFFSET, 590-Y_OFFSET, 26, 26, 2, 1.0f);
	g.drawRoundedRectangle(83-X_OFFSET, 590-Y_OFFSET, 26, 26, 2, 1.0f);
	g.drawRoundedRectangle(117-X_OFFSET, 590-Y_OFFSET, 26, 26, 2, 1.0f);
	g.drawEllipse(55-X_OFFSET, 542-Y_OFFSET, 38, 38, 1.0f);
	g.drawEllipse(99-X_OFFSET, 542-Y_OFFSET, 38,38, 1.0f);
    
}

void EliteControlsComponent::paintOverChildren (Graphics& g)
{
    g.setColour(Colours::white.withAlpha(0.4f));
    
    if (buttonStatusValue[0] == 1)
    {
        g.fillRoundedRectangle(52-X_OFFSET, 593-Y_OFFSET, 20, 20, 2);
    }
    if (buttonStatusValue[1] == 1)
    {
        g.fillRoundedRectangle(86-X_OFFSET, 593-Y_OFFSET, 20, 20, 2);
    }
    if (buttonStatusValue[2] == 1)
    {
        g.fillRoundedRectangle(120-X_OFFSET, 593-Y_OFFSET, 20, 20, 2);
    }
}

void EliteControlsComponent::buttonClicked (Button* button)
{
    int clickedControl = 0;
    
    if (button == dial1Button)
        clickedControl = DIAL_1;
    else if (button == dial2Button)
        clickedControl = DIAL_2;
    else if (button == button1Button)
        clickedControl = BUTTON_1;
    else if (button == button2Button)
        clickedControl = BUTTON_2;
    else if (button == button3Button)
        clickedControl = BUTTON_3;

    
    mainComponentRef.setEliteControlsSettingsDisplay(clickedControl);
    
}

void EliteControlsComponent::turnOffButtons()
{
	
	dial1Button->setToggleState(false, false);
	dial2Button->setToggleState(false, false);
	button1Button->setToggleState(false, false);
	button2Button->setToggleState(false, false);
	button3Button->setToggleState(false, false);
	
}

void EliteControlsComponent::updateDisplay (int control, int value)
{
    //dial 1
    if (control == 0)
    {
        if (value > 0 && value < 64)
        {
            dialRotationValue[0] += (0.1 * value);
            
            if (dialRotationValue[0] >= (M_PI * 2))
                dialRotationValue[0] = dialRotationValue[0] - (M_PI * 2); 
        }
        else
        {
            dialRotationValue[0] -= (0.1 * (128 - value));
            
            if (dialRotationValue[0] < 0)
                dialRotationValue[0] = dialRotationValue[0] + (M_PI * 2);
        }
        
        dial1Button->rotateImage (dialRotationValue[0], 0.4, 0.4);
    }
    
    //dial 2
    else if (control == 1)
    {
        if (value > 0 && value < 64)
        {
            dialRotationValue[1] += (0.1 * value);
            
            if (dialRotationValue[1] >= (M_PI * 2))
                dialRotationValue[1] = dialRotationValue[1] - (M_PI * 2); 
        }
        else
        {
            dialRotationValue[1] -= (0.1 * (128 - value));
            
            if (dialRotationValue[1] < 0)
                dialRotationValue[1] = dialRotationValue[1] + (M_PI * 2);
        }
        
        dial2Button->rotateImage (dialRotationValue[1], 0.4, 0.4);
    }
    
    //button 1
    else if (control == 2)
    {
        buttonStatusValue[0] = value;
        repaint();
    }
    
    //button 2
    else if (control == 3)
    {
        buttonStatusValue[1] = value;
        repaint();
    }
    
    //button 3
    else if (control == 4)
    {
        buttonStatusValue[2] = value;
        repaint();
    }
    
}

void EliteControlsComponent::mouseDown (const MouseEvent &e)
{
    //====================================================================================
    //right-click (or ctrl-click on a mac) to bring up the popup menu to allow settings 
    //to be applied to all scenes.
    //====================================================================================
    if (e.mods.isPopupMenu() == true)
    {
        PopupMenu menu;
        menu.addItem(1, translate("Apply settings to all scenes..."));
        
        const int result = menu.show();
        
        if (result == 0)
        {
            // user dismissed the menu without picking anything
        }
        
        else if (result == 1)
        {
            int selectedControl = 0;
            
            if (e.originalComponent == dial1Button)
                selectedControl = DIAL_1; 
            else if (e.originalComponent == dial2Button)
                selectedControl = DIAL_2; 
            else if (e.originalComponent == button1Button)
                selectedControl = BUTTON_1; 
            else if (e.originalComponent == button2Button)
                selectedControl = BUTTON_2; 
            else if (e.originalComponent == button3Button)
                selectedControl = BUTTON_3; 
            
            mainComponentRef.getAppDocumentStateRef().applyEliteSettingsToAllScenes (selectedControl);
        }
        
    }
}

void EliteControlsComponent::mouseUp (const MouseEvent &e)
{
    
}

void EliteControlsComponent::mouseEnter (const MouseEvent &e)
{
    if (dial1Button->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Elite Dial 1.") + " " + translate(CommonInfoBoxText::eliteControl));
    }
    else if (dial2Button->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Elite Dial 2.") + " " + translate(CommonInfoBoxText::eliteControl));
    }
    else if (button1Button->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Elite Button 1.") + " " + translate(CommonInfoBoxText::eliteControl));
    }
    else if (button2Button->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Elite Button 2.") + " " + translate(CommonInfoBoxText::eliteControl));
    }
    else if (button3Button->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Elite Button 3.") + " " + translate(CommonInfoBoxText::eliteControl));
    }
}

void EliteControlsComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}