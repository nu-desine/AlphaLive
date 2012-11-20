//
//  FxGuiFlanger.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 14/07/2012.
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

#ifndef H_FXGUIFLANGER
#define H_FXGUIFLANGER

#include "../../../../../JuceLibraryCode/JuceHeader.h"
#include "../../General/AlphaRotarySlider.h"
#include "../../General/AlphaPopUpButton.h"
#include "../../General/AlphaTextButton.h"
#include "../../General/ModeButton.h"


class MainComponent;

class GuiFlanger :  public Component,
                    public Slider::Listener,
                    public ComboBox::Listener,
                    public Button::Listener
{
public:
    GuiFlanger(MainComponent &ref);
    ~GuiFlanger();
    
    void resized();
	void paint (Graphics& g);
    void sliderValueChanged (Slider *slider);
    void comboBoxChanged (ComboBox *comboBox);
    void buttonClicked (Button *button);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    
    void setTempo (float value);
    
private:
    MainComponent &mainComponentRef;
    
    AlphaRotarySlider *mixSlider, *rateSlider, *feedbackSlider, *flangerIntensitySlider;
    
    ComboBox *alphaTouchMenu;
	AlphaPopUpButton *rateMenu;
	ScopedPointer <ModeButton> reverseButton, syncButton;
    AlphaRotarySlider *intensitySlider;
    Label *parameterHoverLabel;
    
    Array<int> selectedPads;
    float tempo;
    
};

#endif