//
//  AlphaAudioSettingsComponent.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 17/11/2011.
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
//    THIS HEADER FILE ALONG WITH ITS .CPP FILE ARE IDENTICAL TO THE JUCE_AUDIODEVICESELECTORCOMPONENT FILES
//      HOWEVER WITH A FEW GUI CHANGES TO SUIT THE APP. ALL THE FUNCTIONALITY IS EXACTLY THE SAME.

/*
 ==============================================================================
 
 This file is part of the JUCE library - "Jules' Utility Class Extensions"
 Copyright 2004-11 by Raw Material Software Ltd.
 
 ------------------------------------------------------------------------------
 
 JUCE can be redistributed and/or modified under the terms of the GNU General
 Public License (Version 2), as published by the Free Software Foundation.
 A copy of the license is included in the JUCE distribution, or can be found
 online at www.gnu.org/licenses.
 
 JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses JUCE, commercial licenses are
 available: visit www.rawmaterialsoftware.com/juce for more information.
 
 ==============================================================================
 */

#ifndef __JUCE_AlphaAudioSettingsComponent_JUCEHEADER__
#define __JUCE_AlphaAudioSettingsComponent_JUCEHEADER__

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../../../Functionality Classes/AlphaLiveEngine.h"

//==============================================================================
/**
 A component containing controls to let the user change the audio settings of
 an AudioDeviceManager object.
 
 Very easy to use - just create one of these and show it to the user.
 
 @see AudioDeviceManager
 */
class JUCE_API  AlphaAudioSettingsComponent  : public Component,
public ComboBoxListener, // (can't use ComboBox::Listener due to idiotic VC2005 bug)
public ButtonListener,
public ChangeListener
{
public:
    //==============================================================================
    /** Creates the component.
     
     If your app needs only output channels, you might ask for a maximum of 0 input
     channels, and the component won't display any options for choosing the input
     channels. And likewise if you're doing an input-only app.
     
     @param deviceManager            the device manager that this component should control
     @param minAudioInputChannels    the minimum number of audio input channels that the application needs
     @param maxAudioInputChannels    the maximum number of audio input channels that the application needs
     @param minAudioOutputChannels   the minimum number of audio output channels that the application needs
     @param maxAudioOutputChannels   the maximum number of audio output channels that the application needs
     @param showMidiInputOptions     if true, the component will allow the user to select which midi inputs are enabled
     @param showMidiOutputSelector   if true, the component will let the user choose a default midi output device
     @param showChannelsAsStereoPairs    if true, channels will be treated as pairs; if false, channels will be
     treated as a set of separate mono channels.
     @param hideAdvancedOptionsWithButton    if true, only the minimum amount of UI components
     are shown, with an "advanced" button that shows the rest of them
     */
    AlphaAudioSettingsComponent (AudioDeviceManager& deviceManager,
                                  const int minAudioInputChannels,
                                  const int maxAudioInputChannels,
                                  const int minAudioOutputChannels,
                                  const int maxAudioOutputChannels,
                                  const bool showMidiInputOptions,
                                  const bool showMidiOutputSelector,
                                  const bool showChannelsAsStereoPairs,
                                  const bool hideAdvancedOptionsWithButton,
                                AlphaLiveEngine &ref);
    
    /** Destructor */
    ~AlphaAudioSettingsComponent();
    
    
    //==============================================================================
    /** @internal */
    void resized();
    /** @internal */
    void comboBoxChanged (ComboBox*);
    /** @internal */
    void buttonClicked (Button*);
    /** @internal */
    void changeListenerCallback (ChangeBroadcaster*);
    /** @internal */
    void childBoundsChanged (Component*);
    
private:
    //==============================================================================
    AudioDeviceManager& deviceManager;
    ScopedPointer<ComboBox> deviceTypeDropDown;
    ScopedPointer<Label> deviceTypeDropDownLabel;
    ScopedPointer<Component> audioDeviceSettingsComp;
    String audioDeviceSettingsCompType;
    const int minOutputChannels, maxOutputChannels, minInputChannels, maxInputChannels;
    const bool showChannelsAsStereoPairs;
    const bool hideAdvancedOptionsWithButton;
    
    class MidiInputSelectorComponentListBox;
    friend class ScopedPointer<MidiInputSelectorComponentListBox>;
    ScopedPointer<MidiInputSelectorComponentListBox> midiInputsList;
    ScopedPointer<ComboBox> midiOutputSelector;
    ScopedPointer<Label> midiInputsLabel, midiOutputLabel;
    
    void updateAllControls();
    
    AlphaLiveEngine &alphaLiveEngineRef;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaAudioSettingsComponent);
};


#endif   // __JUCE_AlphaAudioSettingsComponent_JUCEHEADER__
