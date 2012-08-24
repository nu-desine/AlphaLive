//
//  TestGuiControllerMode.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 29/09/2011.
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

#ifndef H_TESTGUICONTROLLERMODE
#define H_TESTGUICONTROLLERMODE

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Custom Components/Graphics/GuiSpeaker.h"
#include "../Custom Components/Graphics/GuiCircleBackground.h"
#include "../Custom Components/General/AlphaSlider.h"
#include "../Custom Components/General/SettingsButton.h"
#include "../Custom Components/General/AlphaTextButton.h"

class MainComponent;

class GuiControllerMode :   public Component,
                                public Button::Listener,
                                public Slider::Listener,
                                public ComboBox::Listener,
                                public TextEditor::Listener, //don't currently need this
                                public Label::Listener
{
public:
    //==============================================================================
    GuiControllerMode(MainComponent &ref);
    ~GuiControllerMode();
    
    void resized();
    void paint (Graphics& g);
    
    void drawButtonsBackground (Graphics& g);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    void setDisplay (int controlSelected);
    
    //==============================================================================
    //derived Listener functions
    void comboBoxChanged (ComboBox* comboBox);
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    void labelTextChanged (Label* labelThatHasChanged);	
    void textEditorReturnKeyPressed (TextEditor& editor); //don't currently need this
    void textEditorTextChanged (TextEditor& editor); //don't currently need this
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    //==============================================================================
private:
    
    Array<int> selectedPads;
    MainComponent &mainComponentRef;
    
    OwnedArray <SettingsButton> controlButtons;
    
    AlphaSlider *sceneNumberSlider, *oscPortNumberSlider;
    AlphaSlider *midiProgramChangeNumberSlider;
    OwnedArray<AlphaTextButton> midiChannelButtons;
    Label *oscIpAddressEditor;
    
    int drawButtons;
    
};

#endif