//
//  GuiEliteControlsSettings.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 17/09/2012.
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

#ifndef H_GUIELITECONTROLSETTINGS
#define H_GUIELITECONTROLSETTINGS

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Custom Components/General/AlphaSlider.h"

class MainComponent;


class GuiEliteControlsSettings :    public Component,
                                    public Button::Listener,
                                    public Slider::Listener,
                                    public ComboBox::Listener,
                                    public Label::Listener
{
public:
	
	GuiEliteControlsSettings(MainComponent &ref);
    ~GuiEliteControlsSettings();
    
    void resized();
	void paint (Graphics& g);
    
    void buttonClicked (Button* button);
    void sliderValueChanged (Slider* slider);
    void comboBoxChanged (ComboBox* comboBox);
    void labelTextChanged (Label* labelThatHasChanged);	
    
    void setDisplay (int controlNumber);
	
	void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
	
private:
    
    int currentlySelectedControl;
    MainComponent &mainComponentRef;
    
    ScopedPointer <ComboBox> dialsMenu, buttonsMenu;
    
    ScopedPointer <AlphaSlider> dialMidiCcNumber, dialMidiChannel, dialMidiMinRange, dialMidiMaxRange;
    ScopedPointer <AlphaSlider> dialOscPortNumber, dialOscMinRange, dialOscMaxRange;
    ScopedPointer <Label> dialOscIpAddressEditor;
    
    ScopedPointer <AlphaSlider> buttonSceneNumber, buttonMidiCcNumber, buttonMidiChannel, buttonMidiOffNumber, buttonMidiOnNumber;
    ScopedPointer <AlphaSlider> buttonOscPortNumber, buttonOscOffNumber, buttonOscOnNumber;
    ScopedPointer <Label> buttonOscIpAddressEditor;
	
};

#endif