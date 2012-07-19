//
//  GuiLooperMode.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 25/09/2011.
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

#ifndef H_GUILOOPERMODE
#define H_GUILOOPERMODE

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Custom Components/Effects/GuiFxDial.h"
#include "../Custom Components/General/ImageSlider.h"
#include "../Custom Components/Graphics/GuiCircleBackground.h"
#include "../Custom Components/Circle Waveform/DemoThumbnailComp.h"
#include "../Custom Components/General/AlphaImageKnob.h"


class MainComponent;

class GuiLooperMode :   public Component,
                            public ComboBox::Listener,
                            public Slider::Listener,
                            public FilenameComponentListener
{
public:
    GuiLooperMode(MainComponent &ref);
    ~GuiLooperMode();
    
    void resized();
	
    
    //derived Listener functions
    void comboBoxChanged (ComboBox* comboBox);
    void sliderValueChanged (Slider* slider);
    void filenameComponentChanged (FilenameComponent* filenameComponent);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);

    
private:
    
    FilenameComponent *fileChooser;
    ComboBox *playStateMenu, *triggerModeMenu, *pressureModeMenu;
    
    //int currentlySelectedPad;
    Array<int> selectedPads;
    
    MainComponent &mainComponentRef;
    
    //New GUI stuff!
    GuiFxDial *fxDial;
    ImageSlider *channelSlider;
    AlphaImageKnob *gainSlider;
	AlphaImageKnob *panSlider;
    GuiCircleBackground *circleBackgroundRight;
	
	DemoThumbnailComp *waveform;
	
	
	
	
	double startTime, endTime;
    
};

#endif