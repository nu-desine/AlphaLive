//
//  TestGuiSeqSamplesMode.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 17/10/2011.
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

#ifndef H_TESTGUISEQSAMPLESMODE
#define H_TESTGUISEQSAMPLESMODE

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Functionality Classes/Sequencer Mode/SequencerValues.h"
#include "../Custom Components/General/AlphaImageKnob.h"

class MainComponent;

class GuiSeqSamplesMode :       public Component,
                                    public Button::Listener,
                                    public Slider::Listener,
                                    public ComboBox::Listener,
                                    public FilenameComponentListener
{
public:
    //==============================================================================
    GuiSeqSamplesMode(MainComponent &ref);
    ~GuiSeqSamplesMode();
    
    void resized();
    
    //==============================================================================
    //derived Listener functions
    void comboBoxChanged (ComboBox* comboBox);
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    void filenameComponentChanged (FilenameComponent* filenameComponent);
    
    //==============================================================================
    
    void setComponentsVisible(bool isVisible);
    
    void setCurrentlySelectedPad (int padNumber);
    void updateDisplay();
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
private:
    
    FilenameComponent *audioSampleChooser[NO_OF_ROWS];
    ComboBox *drumBankMenu;
    
    int currentlySelectedPad;
    
    MainComponent &mainComponentRef;
    
    //New GUI stuff!
    AlphaImageKnob *gainSlider, *panSlider;
    
};

#endif