//
// FxGuiDistortion.h
// AlphaLive
//
// Created by Felix Godden on 25/09/2012.
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

#ifndef H_FXGUIDISTORTION
#define H_FXGUIDISTORTION

#include "../../../../../JuceLibraryCode/JuceHeader.h"
#include "../../General/AlphaRotarySlider.h"
#include "../../General/AlphaTextButton.h"
#include "../../General/ModeButton.h"
#include "../../General/AlphaPopUpButton.h"

class MainComponent;

class GuiDistortion		:	public Component,
							public Slider::Listener,
							public ComboBox::Listener,
							public Button::Listener
{
public:
    GuiDistortion(MainComponent &ref);
    ~GuiDistortion();
    
    void resized();
    void paint (Graphics &g);
    void sliderValueChanged (Slider *slider);
    void comboBoxChanged (ComboBox *comboBox);
    void buttonClicked (Button *button);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (Array<int> selectedPads);
    void updateDisplay();
    
    void setDistortionTypeMenu (int selectedItem);

    
private:
    MainComponent &mainComponentRef;
    
    AlphaRotarySlider *inputGainSlider, *driveSlider, *toneSlider, *wetDryMixSlider;
    
    ComboBox *alphaTouchMenu;
	ModeButton *reverseButton;
    AlphaTextButton *syncButton, *toneSwitchButton;
    AlphaRotarySlider *intensitySlider;
    Label *parameterHoverLabel;
    AlphaPopUpButton *distortionTypeMenu;
    
    Array<int> selectedPads;
    
};

#endif