//
// FxGuiDistortion.h
// AlphaLive
//
// Created by Felix Godden on 02/09/2012.
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

#ifndef H_FXGUIBITCRUSHER
#define H_FXGUIBITCRUSHER

#include "../../../../../JuceLibraryCode/JuceHeader.h"
#include "../../General/AlphaRotarySlider.h"
#include "../../General/AlphaTextButton.h"
#include "../../General/ModeButton.h"

class MainComponent;

class GuiBitcrusher		:		public Component,
								public Slider::Listener,
								public ComboBox::Listener,
								public Button::Listener
{
public:
    GuiBitcrusher(MainComponent &ref);
    ~GuiBitcrusher();
    
    void resized();
    void sliderValueChanged (Slider *slider);
    void comboBoxChanged (ComboBox *comboBox);
    void buttonClicked (Button *button);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (Array<int> selectedPads);
    void updateDisplay();
	
    
private:
    MainComponent &mainComponentRef;
    
    AlphaRotarySlider *inputGainSlider, *crushSlider, *downsampleSlider, *smoothingSlider, *wetDryMixSlider;
    
	ComboBox *alphaTouchMenu;
	ModeButton *reverseButton;
    AlphaTextButton *syncButton;
    AlphaRotarySlider *intensitySlider;
    Label *parameterHoverLabel;
    
    Array<int> selectedPads;
    
};

#endif