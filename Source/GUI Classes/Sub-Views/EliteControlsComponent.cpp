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
    
   /* addAndMakeVisible (dial1Button = new ShapeButton("Dial 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    dial1Button->setShape(dialShape, true, true, false);
    dial1Button->addListener(this);
    dial1Button->addMouseListener(this, true);
    addAndMakeVisible (dial2Button = new ShapeButton("Dial 2", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    dial2Button->setShape(dialShape, true, true, false);
    dial2Button->addListener(this);
    dial2Button->addMouseListener(this, true);
	
    
    addAndMakeVisible (button1Button = new ShapeButton("Button 1", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    button1Button->setShape(buttonShape, true, true, false);
    button1Button->addListener(this);
    button1Button->addMouseListener(this, true);
    addAndMakeVisible (button2Button = new ShapeButton("Button 2", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    button2Button->setShape(buttonShape, true, true, false);
    button2Button->addListener(this);
    button2Button->addMouseListener(this, true);
    addAndMakeVisible (button3Button = new ShapeButton("Button 3", AlphaColours::blue, AlphaColours::lightblue, AlphaColours::blue));
    button3Button->setShape(buttonShape, true, true, false);
    button3Button->addListener(this);
    button3Button->addMouseListener(this, true);
	
	*/
	
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
    
}

EliteControlsComponent::~EliteControlsComponent()
{

}

void EliteControlsComponent::resized()
{
    dial1Button->setBounds(58, 545, 32, 32);
    dial2Button->setBounds(102, 545, 32, 32);
    
    button1Button->setBounds(52, 593, 20, 20);
    button2Button->setBounds(86, 593, 20, 20);
    button3Button->setBounds(120, 593, 20, 20);
}

void EliteControlsComponent::paint (Graphics& g)
{
	
	g.setColour(Colours::black);
	
	g.fillRoundedRectangle(49, 590, 26, 26, 2);
	g.fillRoundedRectangle(83, 590, 26, 26, 2);
	g.fillRoundedRectangle(117, 590, 26, 26, 2);
	g.fillEllipse(55,542, 38, 38);
	g.fillEllipse(99,542, 38, 38);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	
	g.drawRoundedRectangle(49, 590, 26, 26, 2, 1.0f);
	g.drawRoundedRectangle(83, 590, 26, 26, 2, 1.0f);
	g.drawRoundedRectangle(117, 590, 26, 26, 2, 1.0f);
	g.drawEllipse(55,542, 38, 38, 1.0f);
	g.drawEllipse(99,542, 38,38, 1.0f);
	
    
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

//void EliteControlsComponent::updateDisplay()
//{
//    
//}

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