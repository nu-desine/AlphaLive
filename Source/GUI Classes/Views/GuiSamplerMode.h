//
//  GuiSamplerMode.h
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

#ifndef H_ALPHAFILENAMECOMPONENT
#define H_ALPHAFILENAMECOMPONENT

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../File and Settings/AppSettings.h"

class AlphaFilenameComponent: public FilenameComponent
{
public:
    AlphaFilenameComponent(const String& name,
                           const File& currentFile,
                           bool canEditFilename,
                           bool isDirectory,
                           bool isForSaving,
                           const String& fileBrowserWildcard,
                           const String& enforcedSuffix,
                           const String& textWhenNothingSelected)
    : FilenameComponent (name,
                         currentFile,
                         canEditFilename,
                         isDirectory,
                         isForSaving,
                         fileBrowserWildcard,
                         enforcedSuffix,
                         textWhenNothingSelected)
    {

    }

    File getLocationToBrowse()
    {
        return AppSettings::Instance()->getLastAudioSampleDirectory();
    }
};

#endif



#ifndef H_GUISAMPLERMODE
#define H_GUISAMPLERMODE

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Custom Components/Effects/GuiFxDial.h"
#include "../Custom Components/General/ImageSlider.h"
#include "../Custom Components/Graphics/GuiCircleBackground.h"
#include "../Custom Components/Circle Waveform/DemoThumbnailComp.h"
#include "../Custom Components/General/AlphaTextButton.h"
#include "../Custom Components/General/SettingsButton.h"
#include "../Custom Components/General/GuiSwitch.h"
#include "../Custom Components/General/ModeButton.h"
#include "../Custom Components/General/SettingsButtonImage.h"
#include "../Custom Components/General/AlphaRotarySlider.h"


class MainComponent;

class GuiSamplerMode :   public Component,
                            public ComboBox::Listener,
                            public Slider::Listener,
                            public FilenameComponentListener,
                            public Button::Listener
{
public:
    GuiSamplerMode(MainComponent &ref);
    ~GuiSamplerMode();
    
    void resized();
    void paint (Graphics& g);
	
    //derived Listener functions
    void comboBoxChanged (ComboBox* comboBox);
    void sliderValueChanged (Slider* slider);
    void filenameComponentChanged (FilenameComponent* filenameComponent);
    void buttonClicked (Button* button);
    
    void setAudioFileDisplay(File file);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    void setParameterLabelText (String value);
    void setRotaryControlDisplay();
    void changeView (int view);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);

    
private:
    
    void setDisplay(int settingsType); //sets whether the trigger (1) or pressure (2) settings are display. 
    
    Array<int> selectedPads;
    MainComponent &mainComponentRef;
    
    ScopedPointer <AlphaFilenameComponent> fileChooser;
    ScopedPointer <ModeButton> loopButton, indestructibleButton, finishLoopButton, stickyButton;
    ScopedPointer <ModeButton> triggerSettingsButton, pressureSettingsButton, quantiseButton; //quantise button should probably now be global.

    ScopedPointer <GuiSwitch> pressureStatusButton;
    OwnedArray<SettingsButtonImage> triggerModeButtons;
    ScopedPointer <GuiFxDial> fxDial;
    ScopedPointer <GuiCircleBackground> notSelected;
	
	ScopedPointer <DemoThumbnailComp> waveform;
	
    int controlDisplayId; //used to set which rotary dial is display in the centre
	ScopedPointer <SettingsButton> plusButton, minusButton;
    ScopedPointer <AlphaRotarySlider> gainSlider, panSlider, attackSlider, releaseSlider, polyphonySlider;
    ScopedPointer <Label> parameterLabel, currentParameterLabel;
	
	double startTime, endTime;
    
};

#endif