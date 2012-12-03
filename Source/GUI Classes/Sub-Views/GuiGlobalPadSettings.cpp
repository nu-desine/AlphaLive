/*
 *  GuiGlobalPadSettings.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 20/06/2012.
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
 */

#include "GuiGlobalPadSettings.h"
#include "../../File and Settings/AppSettings.h"
#include "../Views/MainComponent.h"
#include "../Binary Data/BinaryDataNew.h"
#include "../../Application/CommonInfoBoxText.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)

GuiGlobalPadSettings::GuiGlobalPadSettings(MainComponent &ref)
: mainComponentRef(ref)

{
	expoImage = new Image(ImageCache::getFromMemory(BinaryDataNew::expoicon_png, BinaryDataNew::expoicon_pngSize));
	linearImage = new Image(ImageCache::getFromMemory(BinaryDataNew::linearicon_png, BinaryDataNew::linearicon_pngSize));
	logImage = new Image(ImageCache::getFromMemory(BinaryDataNew::logicon_png, BinaryDataNew::logicon_pngSize));
	staticImage = new Image(ImageCache::getFromMemory(BinaryDataNew::staticicon_png, BinaryDataNew::staticicon_pngSize));
    emptyImage = new Image();

	Image *quantiseIcon = new Image(ImageCache::getFromMemory(BinaryDataNew::quantiseicon_png, BinaryDataNew::quantiseicon_pngSize));
	addAndMakeVisible(quantiseButton = new ModeButton(quantiseIcon));
	quantiseButton->setClickingTogglesState(true);
	quantiseButton->setToggleState(false, false);	
	quantiseButton->addListener(this);
	quantiseButton->addMouseListener(this, true);
    
	Image *exclusiveImage = new Image(ImageCache::getFromMemory(BinaryDataNew::exclusiveicon_png, BinaryDataNew::exclusiveicon_pngSize));
	addAndMakeVisible(exclusiveModeButton = new ModeButton(exclusiveImage));
    exclusiveModeButton->addListener(this);
    exclusiveModeButton->addMouseListener(this, true);
    exclusiveModeButton->setClickingTogglesState(true);
    exclusiveModeButton->setToggleState(0, false);
    
    addChildComponent(exclusiveGroupSlider = new AlphaSlider());
    exclusiveGroupSlider->setRange(1, 24, 1);
    exclusiveGroupSlider->addListener(this);
    exclusiveGroupSlider->setValue(1, dontSendNotification);
    exclusiveGroupSlider->addMouseListener(this, true);
    
	/*
    addAndMakeVisible(velocitySlider = new AlphaSlider());
    velocitySlider->setRange(0, 127, 1);
    velocitySlider->addListener(this);
    velocitySlider->setValue(110, false);
    velocitySlider->addMouseListener(this, true);*/
	
	addChildComponent(velocitySlider = new AlphaRotarySlider((220 * (M_PI / 180)), (500 * (M_PI / 180)), 120));
	velocitySlider->setRotaryParameters((220 * (M_PI / 180)), (500 * (M_PI / 180)),true);
    velocitySlider->setRange(0, 127, 1);
    velocitySlider->addListener(this);
    velocitySlider->setValue(110, dontSendNotification);
    velocitySlider->addMouseListener(this, true);
    
	
    pressureCurveMenu = new PopupMenu();
	pressureCurveMenu->addItem(1, translate("Exponential"));
	pressureCurveMenu->addItem(2, translate("Linear"));
	pressureCurveMenu->addItem(3, translate("Logarithmic"));
    
    
    velocityCurveMenu = new PopupMenu();
    velocityCurveMenu->addItem(1, translate("Exponential"));
	velocityCurveMenu->addItem(2, translate("Linear"));
	velocityCurveMenu->addItem(3, translate("Logarithmic"));
    velocityCurveMenu->addItem(4, translate("Static"));
    
	
	addAndMakeVisible(velocityCurveButton = new AlphaPopUpImageButton(linearImage));
    velocityCurveButton->addListener(this);
    velocityCurveButton->addMouseListener(this, true);
		
	addAndMakeVisible(pressureCurveButton = new AlphaPopUpImageButton(linearImage));
    pressureCurveButton->addListener(this);
    pressureCurveButton->addMouseListener(this, true);
	
	//---------------parameter label -------------------------------------
    addChildComponent(parameterHoverLabel = new Label("value label", String::empty));
    parameterHoverLabel->setJustificationType(Justification::centred);
    parameterHoverLabel->setColour(Label::textColourId, AlphaColours::blue);
    parameterHoverLabel->setFont(Font(9));
    parameterHoverLabel->addMouseListener(this, true);
	
}

GuiGlobalPadSettings::~GuiGlobalPadSettings()
{
    delete expoImage;
    delete linearImage;
    delete logImage;
    delete staticImage;
    delete emptyImage;
	delete pressureCurveMenu;
    delete velocityCurveMenu;
	deleteAllChildren();
}

void GuiGlobalPadSettings::resized()
{
    quantiseButton->setBounds(681, 288,32, 32); 
	
	exclusiveModeButton->setBounds(823, 275, 42, 42);
	velocityCurveButton->setBounds(924 , 456,58,58);
	pressureCurveButton->setBounds(706 , 456,58,58);
	
    exclusiveGroupSlider->setBounds(816, 393, 58, 58);
	
    velocitySlider->setBounds(785, 362, 120, 120);
	
	 parameterHoverLabel->setBounds(825, 468, 40, 10);
	
}

void GuiGlobalPadSettings::paint (Graphics& g)
{
	
	ColourGradient fillGradient(AlphaColours::nearlyblack,845 , 461, Colours::black, 845 , 383, false);
	g.setGradientFill(fillGradient);
	
	g.fillEllipse(802, 379, 86, 86);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	
	Path trianglePath;
	trianglePath.addTriangle(844, 288, 963, 493, 726, 493);
	g.strokePath(trianglePath, PathStrokeType(1.0f));
	
	g.setColour(Colours::black);
	
	g.fillEllipse(678,285, 38, 38);
	//g.fillEllipse(820, 272, 48, 48);
	g.fillEllipse(815, 267, 58, 58);
	//g.fillEllipse(816, 393, 58, 58);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	
	g.drawEllipse(678,285, 38, 38, 1.0);
	g.drawEllipse(820, 272, 48, 48, 1.0f);
	
	
	
	
}

void GuiGlobalPadSettings::setCurrentlySelectedPad (Array<int> selectedPads_)
{
	
	selectedPads = selectedPads_;
}

void GuiGlobalPadSettings::buttonClicked (Button* button)
{
    if (button == exclusiveModeButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setExclusiveMode(button->getToggleState());
        }
        
        if (button->getToggleState() == true)
            exclusiveGroupSlider->setVisible(true);
        else
            exclusiveGroupSlider->setVisible(false); 
    }
    
    else if(button == quantiseButton)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setQuantizeMode(button->getToggleState());
        }
    }
	
	else if(button == velocityCurveButton)
	{
	
		const int result = velocityCurveMenu->show();
        
        if (result != 0) //if the user selects something
        {
            switch (result)
            {
                   
                case 1: // Exponential
                    
                    velocityCurveButton->setImage(expoImage);
                    break;
                case 2: // Linear
                    
                    velocityCurveButton->setImage(linearImage);
                    break;
                case 3: // Logarithmic
                    
                    velocityCurveButton->setImage(logImage);
                    break;
                case 4: // Static
                    
                    velocityCurveButton->setImage(staticImage);
                    break;
				default: // Linear
                    
                    velocityCurveButton->setImage(linearImage);
                    break;
            }
            
            velocityCurveValue = result;
            
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
				PAD_SETTINGS->setVelocityCurve(result);
                
            }
            
            if (result == 4 
                && (AppSettings::Instance()->padSettings[selectedPads[0]]->getMode() == 1
                || AppSettings::Instance()->padSettings[selectedPads[0]]->getMode() == 4))
                velocitySlider->setVisible(true);
            else
                velocitySlider->setVisible(false);
			
			
        }
		
	}
	
	else if(button == pressureCurveButton)
	{
		
		const int result = pressureCurveMenu->show();
        
        if (result != 0) //if the user selects something
        {
            switch (result)
            {
					
				case 1: // Exponential
                    
                    pressureCurveButton->setImage(expoImage);
                    break;
                case 2: // Linear
                    
                    pressureCurveButton->setImage(linearImage);
                    break;
                case 3: // Logarithmic
                    
                    pressureCurveButton->setImage(logImage);
                    break;
				default: // Linear
                    
                     pressureCurveButton->setImage(linearImage);
                    break;
            }
            
            pressureCurveValue = result;
            
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
				PAD_SETTINGS->setPressureCurve(result);
            }
			
			
        }
		
	}
        
}

void GuiGlobalPadSettings::sliderValueChanged (Slider* slider)
{
    if (slider == exclusiveGroupSlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setExclusiveGroup(slider->getValue());
        }
    }
    else if (slider == velocitySlider)
    {
        for (int i = 0; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            PAD_SETTINGS->setStaticVelocity(slider->getValue());
			parameterHoverLabel->setText(String(slider->getValue()), false);
        }
    }
}

void GuiGlobalPadSettings::comboBoxChanged (ComboBox* comboBox)
{

}

void GuiGlobalPadSettings::updateDisplay()
{
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        exclusiveModeButton->setToggleState(PAD_SETTINGS->getExclusiveMode(), false);
        exclusiveGroupSlider->setValue(PAD_SETTINGS->getExclusiveGroup());
        quantiseButton->setToggleState(PAD_SETTINGS->getQuantizeMode(), false);
        velocitySlider->setValue(PAD_SETTINGS->getStaticVelocity());
		parameterHoverLabel->setText(String(PAD_SETTINGS->getStaticVelocity()), false);
		pressureCurveValue = PAD_SETTINGS->getPressureCurve();
        velocityCurveValue = PAD_SETTINGS->getVelocityCurve();
        
        //if sequencer mode, dissable velocity curve menu as this would cause confusion
        if (PAD_SETTINGS->getMode() == 3)
        {
            velocityCurveButton->setEnabled(false);
            velocityCurveButton->setAlpha(0.3);
        }
        else
        {
            velocityCurveButton->setEnabled(true);
            velocityCurveButton->setAlpha(1.0);
        }
      
    }
    else if(MULTI_PADS)
    {
        
        //==================================================================================================
        int quantiseMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getQuantizeMode();
        //loop through all selected pads expect for the first one
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            //if setting of this pad does NOT match setting of last pad, set default and break
            if (PAD_SETTINGS->getQuantizeMode() != quantiseMode_)
            {
                quantiseButton->setToggleState(0, false);
                break;
            }
            //if this is the last 'natural' iteraction, displayed the setting that matches all the pads
            if (i == selectedPads.size()-1)
                quantiseButton->setToggleState(quantiseMode_, false);
        }
        //==================================================================================================
        int exclusiveMode_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getExclusiveMode();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getExclusiveMode() != exclusiveMode_)
            {
                exclusiveModeButton->setToggleState(0, false);
                break;
            }
            if (i == selectedPads.size()-1)
                exclusiveModeButton->setToggleState(exclusiveMode_, false);
        }
        
        //==================================================================================================
        int exclusiveGroup_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getExclusiveGroup();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getExclusiveGroup() != exclusiveGroup_)
            {
                exclusiveGroupSlider->setValue(-999);
                break;
            }
            if (i == selectedPads.size()-1)
                exclusiveGroupSlider->setValue(exclusiveGroup_);
        }
        
        //==================================================================================================
        int pressureCurve_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getPressureCurve();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getPressureCurve() != pressureCurve_)
            {
                pressureCurveValue = 0;
                break;
            }
            if (i == selectedPads.size()-1)
                pressureCurveValue = pressureCurve_;
        }
        
        //==================================================================================================
        int velocityCurve_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getVelocityCurve();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getVelocityCurve() != velocityCurve_)
            {
                velocityCurveValue = 0;
                break;
            }
            if (i == selectedPads.size()-1)
				velocityCurveValue = velocityCurve_;
        }
        
        //==================================================================================================
        int velocity_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getStaticVelocity();
        for (int i = 1; i < selectedPads.size(); i++)
        {
            int padNum = selectedPads[i];
            if (PAD_SETTINGS->getStaticVelocity() != velocity_)
            {
                velocitySlider->setValue(-999);
                break;
            }
            if (i == selectedPads.size()-1)
                velocitySlider->setValue(velocity_);
			parameterHoverLabel->setText(String(velocity_), false);
        }
        
    }
    
    if (exclusiveModeButton->getToggleState() == true)
        exclusiveGroupSlider->setVisible(true);
    else
        exclusiveGroupSlider->setVisible(false);
    
    
    
    switch (pressureCurveValue)
    {
        case 0: // Empty
            
            pressureCurveButton->setImage(emptyImage);
            break;
            
        case 1: // Exponential
            
            pressureCurveButton->setImage(expoImage);
            break;
        case 2: // Linear
            
            pressureCurveButton->setImage(linearImage);
            break;
        case 3: // Logarithmic
            
            pressureCurveButton->setImage(logImage);
            break;
        default: // Linear
            
            pressureCurveButton->setImage(linearImage);
            break;
    }
    
    
    switch (velocityCurveValue)
    {
        case 0: // Empty
            
            velocityCurveButton->setImage(emptyImage);
            break;
            
        case 1: // Exponential
            
            velocityCurveButton->setImage(expoImage);
            break;
        case 2: // Linear
            
            velocityCurveButton->setImage(linearImage);
            break;
        case 3: // Logarithmic
            
            velocityCurveButton->setImage(logImage);
            break;
        case 4: // Static
            
            velocityCurveButton->setImage(staticImage);
            break;
        default: // Linear
            
            velocityCurveButton->setImage(linearImage);
            break;
    }
    
    
    //static velocity and in MIDI mode or controller mode
    if (velocityCurveValue == 4 
        && (AppSettings::Instance()->padSettings[selectedPads[0]]->getMode() == 1
        || AppSettings::Instance()->padSettings[selectedPads[0]]->getMode() == 4))
    {
        velocitySlider->setVisible(true);
    }
    else
    {
        velocitySlider->setVisible(false);
    }
}


void GuiGlobalPadSettings::mouseEnter (const MouseEvent &e)
{
    if (exclusiveModeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Exclusive Mode Button. If this button is set to 'on' the selected pads will be set to 'Exclusive Mode' - only a single pad can be playing at any time for each exclusive group."));
    }
    else if (exclusiveGroupSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Exclusive Group Selector. Sets and displays the exclusive group number for the selected pads."));
        
    }
    else if (pressureCurveButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Pressure Curve Menu. Use this menu to select the curve for the pressure mapping for the selected pads."));
    }
    else if (quantiseButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::quantizeButton));
    }
    else if (velocityCurveButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Velocity Curve Menu. Use this menu to select the curve for the velocity mapping for the selected pads. If 'Static' is selected a seperate control will be displayed to set the static velocity value."));
    }
    else if (velocitySlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Static Velocity Selector. Sets and displays the static velocity for a MIDI note or sample."));
		parameterHoverLabel->setText(String(velocitySlider->getValue()), false);
		parameterHoverLabel->setVisible(true);
    }
}

void GuiGlobalPadSettings::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
	
	if(e.eventComponent == velocitySlider)
	{
        parameterHoverLabel->setText(String::empty, false);
		parameterHoverLabel->setVisible(false);
	}
}

