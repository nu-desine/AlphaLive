//
//  FxGuiDelay.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/07/2012.
//  Copyright 2012 nu desine. 
//
//  Created by Liam Meredith-Lacey on 13/12/2011.
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

#ifndef H_FXGUIDELAY
#define H_FXGUIDELAY

#include "../../../../../JuceLibraryCode/JuceHeader.h"
#include "../../General/AlphaImageKnob.h"

class MainComponent;

class GuiDelay :    public Component,
                    public Slider::Listener,
                    public ComboBox::Listener,
                    public Button::Listener
{
public:
    GuiDelay(MainComponent &ref);
    ~GuiDelay();
    
    void resized();
    void sliderValueChanged (Slider *slider);
    void comboBoxChanged (ComboBox *comboBox);
    void buttonClicked (Button *button);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (int padNumber);
    void updateDisplay();
    
    void setTempo (float value);
    
private:
    MainComponent &mainComponentRef;
    
    AlphaImageKnob *wetMixSlider, *feedbackSlider, *delayTimeSlider, *lpfFrequencySlider, *hpfFrequencySlider;
    
    ComboBox *alphaTouchMenu, *delayTimeMenu;
    TextButton *reverseButton, *syncButton;
    AlphaImageKnob *intensitySlider;
    
    int currentlySelectedPad;
    float tempo;
    
};

#endif