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
	
	
	addAndMakeVisible(sliderValue = new Label());
	sliderValue->setFont(Font(11));
	sliderValue->setText(String(getValue()), false);
    sliderValue->setColour(Label::textColourId, Colours::white);
    sliderValue->setColour(Label::backgroundColourId, Colours::transparentBlack);
    sliderValue->setJustificationType(Justification::centred);
    sliderValue->setEditable(false, true, true);
    sliderValue->addListener(this);
	
	i = 0;
	
	arrowUpColour = Colours::grey.withAlpha(0.3f);
	arrowDownColour = Colours::grey.withAlpha(0.3f);
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
	
	if (i == 0) {
		
		sliderValue->setText(String(getValue()), false);
		i = 1;
		
	}
	
	sliderValue->setBounds((getWidth()*0.15), (getHeight()*0.35), (getWidth()*0.7), (getHeight()*0.3));
	
	valueStore = getValue();

}

void AlphaSlider::sliderValueChanged (Slider *slider)

{
	
	sliderValue->setText(String(getValue()), false);
	
	if (getValue() < valueStore) {
		
		arrowUpColour = Colours::grey.withAlpha(0.3f);
		arrowDownColour = Colours::grey.withAlpha(0.8f);

	}
	else if (getValue() > valueStore) {
		
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
	
	setValue(sliderValue->getText().getFloatValue(), true, true);
	
	arrowDownColour = Colours::grey.withAlpha(0.3f);
	arrowUpColour = Colours::grey.withAlpha(0.3f);
	
	repaint();
	
}



bool AlphaSlider::hitTest (int x, int y)
{

	return thePath.contains(x, y);

}