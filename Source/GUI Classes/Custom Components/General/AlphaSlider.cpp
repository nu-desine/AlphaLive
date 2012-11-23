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


AlphaSlider::AlphaSlider()
{
	
	this->setSliderStyle(RotaryVerticalDrag);
	this->setTextBoxStyle(NoTextBox, false, 80, 40);
	this->setColour(textBoxBackgroundColourId , Colours::transparentBlack);
	this->setColour(textBoxOutlineColourId  , Colours::transparentBlack);
	this->setColour(textBoxHighlightColourId, Colours::white);
	this->setColour(textBoxTextColourId, Colours::white);
	addListener(this);
	
	addAndMakeVisible(sliderValueLabel = new Label());
	sliderValueLabel->setFont(Font(11));
	sliderValueLabel->setText(String(getValue()), false);
    sliderValueLabel->setColour(Label::textColourId, Colours::white);
    sliderValueLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
    sliderValueLabel->setJustificationType(Justification::centred);
    sliderValueLabel->setEditable(false, true, true);
    sliderValueLabel->setInterceptsMouseClicks(false, false);
    sliderValueLabel->addListener(this);
	
	i = 0;
	
	arrowUpColour = arrowDownColour = Colours::grey.withAlpha(0.3f);
}

AlphaSlider::~AlphaSlider()
{
	
	deleteAllChildren();
    
}


void AlphaSlider::paint(Graphics& g)
{
	thePath.addEllipse(0, 0, getWidth(), getHeight());
	
	g.setColour(Colours::black);
	g.fillPath(thePath, getTransform());
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse((getWidth()*0.1), (getHeight()*0.1), (getWidth()*0.8), (getHeight()*0.8), 1.0f);
	
	g.setColour(arrowUpColour);
	Path incButtonPath;
	incButtonPath.addTriangle((getWidth()*0.4),(getHeight()*0.25),(getWidth()*0.6),(getHeight()*0.25), (getWidth()*0.5), (getHeight() *0.15));
	g.fillPath(incButtonPath, getTransform());
	
	AffineTransform rotatePath (AffineTransform::rotation((180 * (M_PI / 180)), (getWidth() * 0.5), (getHeight() * 0.5)));
	g.setColour(arrowDownColour);
	g.fillPath(incButtonPath, rotatePath);
	
	if (i == 0) 
    {
		sliderValueLabel->setText(String(getValue()), false);
		i = 1;
	}
	
	sliderValueLabel->setBounds((getWidth()*0.15), (getHeight()*0.35), (getWidth()*0.7), (getHeight()*0.3));
	valueStore = getValue();

}

void AlphaSlider::sliderValueChanged (Slider *slider)
{
	
	sliderValueLabel->setText(String(getValue()), false);
	
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
        sliderValueLabel->setText(String(labelValue), false);
    }
    else if (labelValue > getMaximum())
    {
        labelValue = getMaximum();
        sliderValueLabel->setText(String(labelValue), false);
    }
    
    Slider::setValue(labelValue, sendNotification);
	
	arrowDownColour = Colours::grey.withAlpha(0.3f);
	arrowUpColour = Colours::grey.withAlpha(0.3f);
	
	repaint();
	
}

void AlphaSlider::setComponentValue (double value)
{
    
    
}

void AlphaSlider::setValue (double value, int sendNotification)
{
    if (value != -999)
    {
        Slider::setValue(value, dontSendNotification);
        
        //the below alg. needs changing as what if we want to display things to 2 decimal places? Though will that currently fit?
        if (getInterval() >= 1)
        {
            sliderValueLabel->setText(String(value), false);
        }
        else
        {
            sliderValueLabel->setText(String(value, 1), false);
        }
    }
    else //just set a default display without changing the actual slider value
    {
        sliderValueLabel->setText("-", false);
    }
}


bool AlphaSlider::hitTest (int x, int y)
{
	return thePath.contains(x, y);
}

void AlphaSlider::mouseDown(const MouseEvent &e)
{
    if (e.getNumberOfClicks() == 2)
    {
        sliderValueLabel->showEditor();
        sliderValueLabel->getCurrentTextEditor()->setInputRestrictions(0, "1234567890.-");
    }
    else
    {
        Slider::mouseDown(e);
    }
}

