//
//  AlphaSlider.cpp
//  AlphaLive
//
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


#include "AlphaSlider.h"
#include "../../AlphaLiveLookandFeel.h"


AlphaSlider::AlphaSlider()
{
	addAndMakeVisible(sliderValueLabel = new Label());
	sliderValueLabel->setFont(Font(11));
	sliderValueLabel->setText(String(getValue()), dontSendNotification);
    sliderValueLabel->setColour(Label::textColourId, Colours::white);
    sliderValueLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
    sliderValueLabel->setJustificationType(Justification::centred);
    sliderValueLabel->setEditable(false, true, true);
    sliderValueLabel->setInterceptsMouseClicks(false, false);
    sliderValueLabel->addListener(this);
    
    setSliderStyle(RotaryVerticalDrag);
	setTextBoxStyle(NoTextBox, false, 80, 40);
	setColour(textBoxBackgroundColourId , Colours::transparentBlack);
	setColour(textBoxOutlineColourId  , Colours::transparentBlack);
	setColour(textBoxHighlightColourId, Colours::white);
	setColour(textBoxTextColourId, Colours::white);
	addListener(this);
	
	i = 0;
	arrowUpColour = arrowDownColour = Colours::grey.withAlpha(0.3f);
}

AlphaSlider::~AlphaSlider()
{
	
	deleteAllChildren();
    
}

void AlphaSlider::resized()
{
    sliderValueLabel->setBounds((getWidth()*0.15), (getHeight()*0.35), (getWidth()*0.7), (getHeight()*0.3));
}

void AlphaSlider::paint(Graphics& g)
{
    thePath.clear();
    upButtonPath.clear();
    downButtonPath.clear();
    upButtonPathBig.clear();
    downButtonPathBig.clear();
    
	thePath.addEllipse(0, 0, getWidth(), getHeight());
	
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	g.fillPath(thePath);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
	
	g.setColour(arrowUpColour);;
	upButtonPath.addTriangle((getWidth()*0.4),(getHeight()*0.3),(getWidth()*0.6),(getHeight()*0.3), (getWidth()*0.5), (getHeight() *0.15));
	g.fillPath(upButtonPath);
	
    g.setColour(arrowDownColour);
    downButtonPath = upButtonPath;
	AffineTransform rotatePath (AffineTransform::rotation((180 * (M_PI / 180)), (getWidth() * 0.5), (getHeight() * 0.5)));
    downButtonPath.applyTransform(rotatePath);
	g.fillPath(downButtonPath);
	
	if (i == 0) 
    {
		sliderValueLabel->setText(String(getValue()), dontSendNotification);
		i = 1;
	}
	
	valueStore = getValue();
    
    //invisible paths for the user to click on to control the arrows
    upButtonPathBig.addTriangle((getWidth()*0.2),(getHeight()*0.4),(getWidth()*0.8),(getHeight()*0.4), (getWidth()*0.5), (getHeight() *0.1));
    downButtonPathBig = upButtonPathBig;
    downButtonPathBig.applyTransform(rotatePath);
}

void AlphaSlider::sliderValueChanged (Slider *slider)
{
	
	sliderValueLabel->setText(String(getValue()), dontSendNotification);
	
	if (getValue() < valueStore) 
    {
		arrowUpColour = Colours::grey.withAlpha(0.3f);
		arrowDownColour = Colours::grey.withAlpha(0.8f);
	}
	else if (getValue() > valueStore) 
    {
		arrowUpColour = Colours::grey.withAlpha(0.8f);
		arrowDownColour = Colours::grey.withAlpha(0.3f);
	}
	
	repaint();
}

void AlphaSlider::sliderDragEnded (Slider *slider)
{
	arrowDownColour = Colours::grey.withAlpha(0.3f);
	arrowUpColour = Colours::grey.withAlpha(0.3f);
	
	repaint();
}

void AlphaSlider::labelTextChanged (Label* labelThatHasChanged)

{
    double labelValue = sliderValueLabel->getText().getFloatValue();
    
    //if the entered value is out of range,
    //stick it in a range and update the label
    //This is already handled when you set the slider and sent
    //a notification, but not if you repetively set an
    //out of range value. Is that something to do with
    //my 'setValue()' function within this class?
    //However having this here means that if the user enters
    //-999 it won't set the label to '-'.
    if (labelValue < getMinimum())
    {
        labelValue = getMinimum();
        sliderValueLabel->setText(String(labelValue), dontSendNotification);
    }
    else if (labelValue > getMaximum())
    {
        labelValue = getMaximum();
        sliderValueLabel->setText(String(labelValue), dontSendNotification);
    }
    
    Slider::setValue(labelValue, sendNotification);
	
	arrowDownColour = Colours::grey.withAlpha(0.3f);
	arrowUpColour = Colours::grey.withAlpha(0.3f);
	
	repaint();
	
}


void AlphaSlider::setValue (double value, int sendNotification_)    //whats the point of sendNotification here? Is it just so the function matches the Slider one?
                                                                    //I think i should be use the notification type rather an int so then sendNotification_ can be used below
{
    if (value != -999)
    {
        Slider::setValue(value, dontSendNotification); //should we be using sendNotification_ here?
        
        //the below alg. needs changing as what if we want to display things to 2 decimal places? Though will that currently fit?
        if (getInterval() >= 1)
        {
            sliderValueLabel->setText(String(value), dontSendNotification);
        }
        else
        {
            sliderValueLabel->setText(String(value, 1), dontSendNotification);
        }
    }
    else //just set a default display without changing the actual slider value
    {
        sliderValueLabel->setText("-", dontSendNotification);
    }
}


bool AlphaSlider::hitTest (int x, int y)
{
	return thePath.contains(x, y);
}

void AlphaSlider::mouseDown(const MouseEvent &e)
{
    if (upButtonPathBig.contains(e.x, e.y))
    {
        Slider::setValue(Slider::getValue() + Slider::getInterval(), sendNotification);
        sliderValueLabel->setText(String(Slider::getValue()), dontSendNotification);
        
        arrowUpColour = Colours::grey.withAlpha(0.8f);
        repaint();
    }
    else if (downButtonPathBig.contains(e.x, e.y))
    {
        Slider::setValue(Slider::getValue() - Slider::getInterval(), sendNotification);
        sliderValueLabel->setText(String(Slider::getValue()), dontSendNotification);
        
        arrowDownColour = Colours::grey.withAlpha(0.8f);
        repaint();
    }
    else if (e.getNumberOfClicks() == 2)
    {
        sliderValueLabel->showEditor();
        sliderValueLabel->getCurrentTextEditor()->setInputRestrictions(0, "1234567890.-");
    }
    else
    {
        Slider::mouseDown(e);
    }
}

void AlphaSlider::mouseUp(const MouseEvent &e)
{
    arrowDownColour = Colours::grey.withAlpha(0.3f);
	arrowUpColour = Colours::grey.withAlpha(0.3f);
	
	repaint();
}

