//
//  FxGuiPanAndGain.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 22/12/2011.
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
//
#ifndef H_FXGUIGAINANDPAN
#define H_FXGUIGAINANDPAN

#include "../../../../../JuceLibraryCode/JuceHeader.h"
#include "../../General/AlphaImageKnob.h"

class MainComponent;

class GuiGainAndPan :   public Component,
                        public Slider::Listener,
                        public ComboBox::Listener,
                        public Button::Listener
{
public:
    GuiGainAndPan(MainComponent &ref);
    ~GuiGainAndPan();
    
    void resized();
    void sliderValueChanged (Slider *slider);
    void comboBoxChanged (ComboBox *comboBox);
    void buttonClicked (Button *button);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    
private:
    MainComponent &mainComponentRef;
    
    AlphaImageKnob *gainSlider, *panSlider;
    
    ComboBox *alphaTouchMenu;
    TextButton *reverseButton;
    AlphaImageKnob *intensitySlider;
    
    //int currentlySelectedPad;
    Array<int> selectedPads;
    
};

#endif