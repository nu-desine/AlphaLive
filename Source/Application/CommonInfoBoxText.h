//
//  CommonInfoBoxText.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 23/08/2012.
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

// The namespace below contains a set of Strings which are 
// display in the Info Box for multiple controls throughout the software.
// For some reason the translate() call in the Strings don't actually translate
// The text when the software is running, so where the strings appear in the
// rest of the code are wrapped in the translate() function.
// However the translate() functions are still needed below so my translation file
// generator app will grab the text from the code.


namespace CommonInfoBoxText
{
    //==== Mode specific controls ====
    static String quantizeButton(translate("Quantize Button. If this button is set to 'on' the selected pads will be quantized to global quantization value set at the top of the interface. Quantization allows pads to be synchronized by setting the points in time when a pad will start and stop."));
    
    static String triggerModeButtons(translate("This set of Pad Behaviour buttons determine how interaction with a pad controls the playback of an audio sample, sequence, or MIDI note."));
    
    static String triggerModeStandard(translate("'Standard' Mode - press the pad to play the note/sample/sequence and release to stop."));
    
    static String triggerModeToggle(translate("'Toggle' Mode - press the pad to play the note/sample/sequence and press again to stop."));
    
    static String triggerModeLatch(translate("'Latch' Mode - press the pad to play the note/sample/sequence and press to the pads full depth and release to stop."));
    
    static String triggerModeTrigger(translate("'Trigger' Mode - press the pad to play the note/sample/sequence, press again to re-trigger, and press to the pads full depth and release to stop."));
    
    static String midiChannelButtons(translate("MIDI Channel Buttons. Sets and displays the MIDI channel for the selected pads. Click this button to set the selected pads to channel"));
    
    static String triggerSettingsButton(translate("Trigger Settings. Click here to display the trigger settings controls. Trigger settings determine the parameters and playback of a MIDI note, audio sample, or sequence."));
    
    static String pressureSettingsButton(translate("Pressure Settings. Click here to display the pressure settings controls. Pressure settings determine the how the pads pressure manipulates an audio sample or sequence, or sets continuous MIDI data parameters."));
    
    static String midiPressureModes(translate("This set of buttons determines the type of MIDI data that is created from the selected pads pressure."));
    
    static String pressureChannelAT(translate("Channel Aftertouch."));
    static String pressureModWheel(translate("Modulation Wheel."));
    static String pressureCC(translate("CC Messages."));
    static String pressurePitchUp(translate("Pitch Bend (Up)."));
    static String pressurePitchDown(translate("Pitch Bend (Down)."));
    
    static String stickyButton(translate("Sticky Button. If this button is set to 'on' the selected pads will be set to 'Sticky' - the pressure value will 'stick' to the highest pressed value until the pad is released and pressed again."));
    
    static String indestructibleButton(translate("Indestructible Button. If this button is set to 'on' the selected pads will be set to 'Indestructible' - the pads can only be turned off via the Exclusive Group feature or using the kill-switch."));
    
    static String loopButton(translate("Loop Button. If this button is set to 'on' the audio sample or sequence of the selected pads will loop."));
    
    static String finishLoopButton(translate("Finish Loop Button. If this button is set to 'on' the audio sample or sequence of the selected pads will finish its currently loop first when instructed to stop."));
    
    static String pressureStatusButton(translate("Pressure Status Switch. If this switch is set to 'off' pressure data from the selected pads will be ignored."));
    
    static String midiCcSlider(translate("MIDI CC Number Selector. Sets and displays the MIDI control change number for the pressure of the  selected pads."));
    
    static String midiMinPressureRangeSlider(translate("Pressure Minimum Range Selector. Along with the Maximum Range Selector it sets and displays the MIDI pressure data range for the selected pads."));
    
    static String midiMaxPressureRangeSlider(translate("Pressure Maximum Range Selector. Along with the Minimum Range Selector it sets and displays the MIDI pressure data range for the selected pads."));
    
    static String oscIpAddressEditor(translate("IP Address Editor. Sets and displays the IP address of the device that you want the selected pads to send OSC messages to. Set to 127.0.0.1 to send OSC messages somewhere on the same computer."));
    
    static String oscportNumberSlider(translate("Port Number Selector. Sets and displays the UDP/TCP port number that you want the selected pads to send OSC messages over."));
    
    
    // ==== global clock controls ====
    static String quantizationValueButtons(translate("When a pad is set to 'quantized' its start/stop command will be triggered at the next point in time relevant to the global quantization value which is set with this set of buttons."));
    
    // ==== FX dial and views ====
    static String effectsButtons(translate("Applies the effect to the audio of the selected pads whose parameters can then be controlled via the pads pressure."));
    
    static String alphaTouchMenu(translate("AlphaTouch Menu. Sets and displays the effect parameter that the selected pads pressure will control."));
    
    static String inverseButton(translate("AlphaTouch Inverse Button. Activate this button to invert the direction of the modulation created by the pressure of the selected pads."));
    
    static String intensitySlider(translate("AlphaTouch Intensity Control. Sets and displays the intensity/range of modulation created by the pressure of the selected pads."));
    
    // ==== Other ====
    static String closeButton (translate("Click to go back to the applications main interface. Pressing ESC also triggers this command."));
    
    static String eliteControl (translate("Click this button to set and display the settings for this elite control."));
    
    static String eliteControlMenu (translate("Elite Control Type Menu. Sets and displays the function of the selected elite control."));
    
    static String eliteMidiCcNumber (translate("MIDI CC Selector. Sets and displays the MIDI CC number for the selected elite control."));
    
    static String eliteMidiChannel (translate("MIDI Channel Buttons. Sets and displays the MIDI channel for the selected elite control. Click this button to set the selected control to channel"));
    
    static String eliteOscPortNumber (translate("OSC Port Number Selector. Sets and displays the UDP/TCP port number that you want the selected elite control to send OSC messages over."));
    
    static String eliteOscIpAddress (translate("OSC IP Address Editor. Sets and displays the IP address of the device that you want the selected elite control to send OSC messages to. Set to 127.0.0.1 to send OSC messages somewhere on the same computer."));
    
    
}