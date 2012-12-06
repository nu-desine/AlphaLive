/*
 *  GuiGlobalPadSettings.h
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

#ifndef H_GUIGLOBALPADSETTINGS
#define H_GUIGLOBALPADSETTINGS

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Custom Components/General/AlphaSlider.h"
#include "../Custom Components/General/AlphaRotarySlider.h"
#include "../Custom Components/General/ModeButton.h"
#include "../Custom Components/General/AlphaPopUpImageButton.h"

class MainComponent;


class GuiGlobalPadSettings :    public Component,
                                public Button::Listener,
                                public Slider::Listener,
                                public ComboBox::Listener
{
	public:
	
	GuiGlobalPadSettings(MainComponent &ref);
    ~GuiGlobalPadSettings();
    
    void resized();
	void paint (Graphics& g);
    
    void buttonClicked (Button* button);
    void sliderValueChanged (Slider* slider);
    void comboBoxChanged (ComboBox* comboBox);
	
	void setCurrentlySelectedPad (Array<int> selectedPads_);
    
    void updateDisplay();
	
	void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
	
	private:
    
    Array <int> selectedPads;
    MainComponent &mainComponentRef;
    
    ModeButton *exclusiveModeButton;
    AlphaSlider *exclusiveGroupSlider;
    AlphaRotarySlider *velocitySlider;
	AlphaPopUpImageButton *pressureCurveButton, *velocityCurveButton;
    ModeButton *quantiseButton;
	Label *parameterHoverLabel;
	Image *expoImage, *linearImage, *logImage, *staticImage, *emptyImage;
	PopupMenu *pressureCurveMenu, *velocityCurveMenu;
    int pressureCurveValue, velocityCurveValue;
	
};

#endif
	
	
