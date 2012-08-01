//
//  PadSettings.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 15/09/2011.
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

#ifndef H_PADSETTINGS
#define H_PADSETTINGS

//==================================================================
/**
 This class is used to store and retrieve all the settings for a single pad.
 The program should contain 48 objects of this class, one for each pad.
 The class just contains a set of accessor and mutator meothods for each of the classes variables.
 */

//NOTE: WHEN APPLYING SETTINGS INTO HERE FROM XML FILES (PROBABLY BY CALLING THE MUTATOR METHODS) SOMETIMES IT MIGHT TRY AND SET VALUES IN OBJECTS THAT DON'T EXIST.
//IN THIS CASE IT MAY BE A GOOD IDEA TO PASS IN DATA FROM SAMPLER MODE AND SEQUENCE MODE TO KEEP THIS CLASS KNOWING AND UPDATED AT WHAT OBJECTS EXIST OR NOT, AND ONLY APPLY THE SETTINGS IF THEY EXIST (IF OBJECT[PAD] != NULL)
//what settings should be saved into and loaded from xmls files? how should it handle the data on the pads that isn't need yet (e.g. the seq data if the pad is noe set to a sampler)?
//don;t want unncessary data loaded up when switching between presets.

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Functionality Classes/Sampler Mode/ModeSampler.h"
#include "../Functionality Classes/Sequencer Mode/SequencerValues.h"

class AlphaLiveEngine;
class AppSettings;
 
class PadSettings
{
public:
    //==================================================================
    PadSettings (int arrayIndex);
    ~PadSettings();
    
    void setAlphaLiveEngineRef (AlphaLiveEngine *ref);
    void setPadNumber (int value);
    void resetData(int whatToReset);
    
    //==================================================================
    //==================================================================
    //==================================================================
    
    //==================================================================
    //Global
    void setMode (int value);
    void setCurrentState (int value);
    void setCurrentPlayingState (int value);
    void setCurrentValue (int value); 
    void setPressureSensitivityMode (int value);
    void setExclusiveMode (int value);
    void setExclusiveGroup (int value);
    void setQuantizeMode (int value);

    int getMode();
    int getCurrentState();
    int getCurrentValue();
    int getCurrentPlayingState();
    int getPressureSensitivityMode();
    int getExclusiveMode();
    int getExclusiveGroup();
    int getQuantizeMode();
    
    //==================================================================
    //MIDI mode
    void setMidiNote (int value);
    void setMidiVelocity (int value);
    void setMidiChannel (int value);
    void setMidiMinPressureRange (int value);
    void setMidiMaxPressureRange (int value);
    void setMidiPressureMode (int value);
    void setMidiTriggerMode (int value);
    void setMidiIndestructible (int value);
    void setMidiSticky (int value);
    void setMidiPressureStatus (bool value);
    void setMidiNoteStatus (bool value);
    void setMidiCcController (int value);
    
    int getMidiNote();
    int getMidiVelocity();
    int getMidiChannel();
    int getMidiMinPressureRange();
    int getMidiMaxPressureRange();
    int getMidiPressureMode();
    int getMidiTriggerMode();
    int getMidiIndestructible();
    int getMidiSticky();
    bool getMidiPressureStatus();
    bool getMidiNoteStatus();
    int getMidiCcController();
    
    //==================================================================
    //Sampler mode
    void setSamplerAudioFilePath (File value);
    void setSamplerTriggerMode (int value);
    void setSamplerShouldLoop (int value);
    void setSamplerIndestructible (int value);
    void setSamplerShouldFinishLoop (int value);
    void setSamplerSticky (int value);
    void setSamplerEffect (int value);
    void setSamplerPan (float value);
    void setSamplerGain (float value);
    
    File getSamplerAudioFilePath();
    bool getSamplerIsLibraryFile();
    int getSamplerTriggerMode();
    int getSamplerShouldLoop();
    int getSamplerIndestructible();
    int getSamplerShouldFinishLoop();
    int getSamplerSticky();
    int getSamplerEffect();
    float getSamplerPan();
    float getSamplerGain();
    
    //----Sampler FX----
    //Gain&Pan
    void setSamplerFxGainPanGain (float value);
    void setSamplerFxGainPanPan (float value);
    void setSamplerFxGainPanAlphaTouch (int value);
    void setSamplerFxGainPanAtReverse (int value);
    void setSamplerFxGainPanAtIntensity (float value);
    float getSamplerFxGainPanGain();
    float getSamplerFxGainPanPan();
    int getSamplerFxGainPanAlphaTouch();
    int getSamplerFxGainPanAtReverse();
    float getSamplerFxGainPanAtIntensity();
    //LPF
    void setSamplerFxLpfMix (float value);
    void setSamplerFxLpfFreq (float value);
    void setSamplerFxLpfBandwidth (float value);
    void setSamplerFxLpfAlphaTouch (int value);
    void setSamplerFxLpfAtReverse (int value);
    void setSamplerFxLpfAtIntensity (float value);
    float getSamplerFxLpfMix();
    float getSamplerFxLpfFreq();
    float getSamplerFxLpfBandwidth();
    int getSamplerFxLpfAlphaTouch();
    int getSamplerFxLpfAtReverse();
    float getSamplerFxLpfAtIntensity();
    //HPF
    void setSamplerFxHpfMix (float value);
    void setSamplerFxHpfFreq (float value);
    void setSamplerFxHpfBandwidth (float value);
    void setSamplerFxHpfAlphaTouch (int value);
    void setSamplerFxHpfAtReverse (int value);
    void setSamplerFxHpfAtIntensity (float value);
    float getSamplerFxHpfMix();
    float getSamplerFxHpfFreq();
    float getSamplerFxHpfBandwidth();
    int getSamplerFxHpfAlphaTouch();
    int getSamplerFxHpfAtReverse();
    float getSamplerFxHpfAtIntensity();
    //BPF
    void setSamplerFxBpfMix (float value);
    void setSamplerFxBpfFreq (float value);
    void setSamplerFxBpfBandwidth (float value);
    void setSamplerFxBpfAlphaTouch (int value);
    void setSamplerFxBpfAtReverse (int value);
    void setSamplerFxBpfAtIntensity (float value);
    float getSamplerFxBpfMix();
    float getSamplerFxBpfFreq();
    float getSamplerFxBpfBandwidth();
    int getSamplerFxBpfAlphaTouch();
    int getSamplerFxBpfAtReverse();
    float getSamplerFxBpfAtIntensity();
    //delay
    void setSamplerFxDelayMix (float value);
    void setSamplerFxDelayTime (float value);
    void setSamplerFxDelayFeedback (float value);
    void setSamplerFxDelayLpfFreq (float value);
    void setSamplerFxDelayHpfFreq (float value);
    void setSamplerFxDelaySync (int value);
    void setSamplerFxDelayTimeMenu (int value);
    void setSamplerFxDelayAlphaTouch (int value);
    void setSamplerFxDelayAtReverse (int value);
    void setSamplerFxDelayAtIntensity (float value);
    float getSamplerFxDelayMix();
    float getSamplerFxDelayTime();
    float getSamplerFxDelayFeedback();
    float getSamplerFxDelayLpfFreq();
    float getSamplerFxDelayHpfFreq();
    int getSamplerFxDelaySync();
    int getSamplerFxDelayTimeMenu();
    int getSamplerFxDelayAlphaTouch();
    int getSamplerFxDelayAtReverse();
    float getSamplerFxDelayAtIntensity();
    //reverb
    void setSamplerFxReverbMix (float value);
    void setSamplerFxReverbRoomSize (float value);
    void setSamplerFxReverbDamping (float value);
    void setSamplerFxReverbWidth (float value);
    void setSamplerFxReverbFreezeMode (float value);
    void setSamplerFxReverbAlphaTouch (int value);
    void setSamplerFxReverbAtReverse (int value);
    void setSamplerFxReverbAtIntensity (float value);
    float getSamplerFxReverbMix();
    float getSamplerFxReverbRoomSize();
    float getSamplerFxReverbDamping();
    float getSamplerFxReverbWidth();
    float getSamplerFxReverbFreezeMode();
    int getSamplerFxReverbAlphaTouch();
    int getSamplerFxReverbAtReverse();
    float getSamplerFxReverbAtIntensity();
    //flanger
    void setSamplerFxFlangerMix (double value);
    void setSamplerFxFlangerRate(double value);
    void setSamplerFxFlangerFeedback (double value);
    void setSamplerFxFlangerIntensity (double value);
    void setSamplerFxFlangerSync (int value);
    void setSamplerFxFlangerRateMenu (int value);
    void setSamplerFxFlangerAlphaTouch (int value);
    void setSamplerFxFlangerAtReverse (int value);
    void setSamplerFxFlangerAtIntensity (double value);
    double getSamplerFxFlangerMix();
    double getSamplerFxFlangerRate();
    double getSamplerFxFlangerFeedback();
    double getSamplerFxFlangerIntensity();
    int getSamplerFxFlangerSync();
    int getSamplerFxFlangerRateMenu();
    int getSamplerFxFlangerAlphaTouch();
    int getSamplerFxFlangerAtReverse();
    double getSamplerFxFlangerAtIntensity();
    //tremolo
    void setSamplerFxTremoloDepth (double value);
    void setSamplerFxTremoloRate (double value);
    void setSamplerFxTremoloShape (int value);
    void setSamplerFxTremoloSync (int value);
    void setSamplerFxTremoloRateMenu (int value);
    void setSamplerFxTremoloAlphaTouch (int value);
    void setSamplerFxTremoloAtReverse (int value);
    void setSamplerFxTremoloAtIntensity (double value);
    double getSamplerFxTremoloDepth();
    double getSamplerFxTremoloRate();
    int getSamplerFxTremoloShape();
    int getSamplerFxTremoloSync();
    int getSamplerFxTremoloRateMenu();
    int getSamplerFxTremoloAlphaTouch();
    int getSamplerFxTremoloAtReverse();
    double getSamplerFxTremoloAtIntensity();
    
    
    //==================================================================
    //Sequencer mode
    void setSequencerMode (int value);
    void setSequencerData (int sequenceNumber, int rowNumber, int columnNumber, int value, bool shouldUpdateSeqDataString=true);
    void setSequencerNumberOfSequences (int value);
    void setSequencerTriggerMode (int value);
    void setSequencerShouldLoop (int value);
    void setSequencerIndestructible (int value);
    void setSequencerShouldFinishLoop (int value);
    void setSequencerSticky (int value);
    void setSequencerLength (int value);
    void setSequencerRelativeTempoMode (int value);
    void setSequencerDynamicMode (int value);
    
    void setSequencerMidiNote (int value, int arrayIndex);
    void setSequencerMidiVelocity (int value); //NO LONGER NEED THIS
    void setSequencerMidiChannel (int value);
    void setSequencerMidiNoteLength (int value);
    void setSequencerMidiMinPressureRange (int value);
    void setSequencerMidiMaxPressureRange (int value);
    void setSequencerMidiPressureMode (int value);
    void setSequencerMidiPressureStatus (bool value);
    void setSequencerMidiCcController (int value);
    
    void setSequencerSamplesAudioFilePath (File value, int sequencerRow);
    void setSequencerPan (float value);
    void setSequencerGain (float value);
    
    int getSequencerMode();
    int getSequencerData (int sequenceNumber, int rowNumber, int columnNumber);
    int getSequencerNumberOfSequences();
    int getSequencerTriggerMode();
    int getSequencerShouldLoop();
    int getSequencerIndestructible();
    int getSequencerShouldFinishLoop();
    int getSequencerSticky();
    int getSequencerLength();
    int getSequencerRelativeTempoMode();
    int getSequencerDynamicMode();
    
    int getSequencerMidiNote(int arrayIndex);
    int getSequencerMidiVelocity(); //NO LONGER NEED THIS
    int getSequencerMidiChannel();
    int getSequencerMidiNoteLength();
    int getSequencerMidiMinPressureRange();
    int getSequencerMidiMaxPressureRange();
    int getSequencerMidiPressureMode();
    bool getSequencerMidiPressureStatus();
    int getSequencerMidiCcController();
    
    File getSequencerSamplesAudioFilePath (int sequencerRow);
    bool getSequencerIsLibraryFile (int sequencerRow);
    float getSequencerPan();
    float getSequencerGain();
    
    //used to encode the sequence data into a more friendly form for saving to xml
    void seqDataToString();
    void stringToSeqData (String savedString, int seqNumber);
    void stringToSeqDataFormatConversion (String savedString, int seqNumber);   //function for converting seq data of the old 
                                                                                //format to the new format
                                                                       
    String getSequencerDataString (int seqNumber);
    void clearSequencerDataString (int seqNumber);
    
    //see private members section below for notes on other needed Sequencer methods
    
    //==================================================================
    //Controller Mode
    void setControllerControl (int value);
    void setControllerPresetNumber (int value);
    void setControllerOscIpAddress (String value);
    void setControllerOscPort (int value);
    void setControllerMidiProgramChangeNumber (int value);
    void setControllerMidiProgramChangeChannel (int value);
    
    int getControllerControl();
    int getControllerPresentNumber();
    String getControllerOscIpAddress();
    int getControllerOscPort();
    int getControllerMidiProgramChangeNumber();
    int getControllerMidiProgramChangeChannel();

    
    //==================================================================
    //==================================================================
    //==================================================================
    
private:
    
    int padNumber; //this should be private surely?! Test it out
    AlphaLiveEngine *alphaLiveEngineRef;
    
    //==================================================================
    //Global
    int mode; // 0 = off, 1 = midi, 2 = sampler, 3 = sequencer, 4 = controller
    int prevMode;
    int currentState; //Hods the current physical state. 1 = pressed, 0 = not pressed
    int currentPlayingState; //Holds the current PLAYING state. 0 = off, 1 = on
    int currentValue; //Holds the current modified pressire value
    
    int pressureSensitivityMode;
    int exclusiveMode;
    int exclusiveGroup;
    int quantizeMode;
    
    //==================================================================
    //MIDI mode
    int midiNote;
    int midiVelocity;
    int midiChannel;
    int midiMinPressureRange;
    int midiMaxPressureRange;
    int midiPressureMode;
    int midiTriggerMode;
    int midiIndestructible;
    int midiSticky;
    bool midiPressureStatus;
    bool midiNoteStatus;
    int midiCcController;
    
    //==================================================================
    //Sampler mode
    String samplerAudioFilePath;
    int samplerTriggerMode;
    int samplerShouldLoop;
    int samplerIndestructible;
    int samplerShouldFinishLoop;
    int samplerSticky;
    int samplerEffect;
    float samplerPan;
    float samplerGain;
    
    //----fx variables------
    //Gain&Pan
    float samplerFxGainPanGain;
    float samplerFxGainPanPan;
    int samplerFxGainPanAlphaTouch;
    int samplerFxGainPanAtReverse;
    float samplerFxGainPanAtIntensity;
    //LPF
    float samplerFxLpfMix;
    float samplerFxLpfFreq;
    float samplerFxLpfBandwidth;
    int samplerFxLpfAlphaTouch;
    int samplerFxLpfAtReverse;
    float samplerFxLpfAtIntensity;
    //HPF
    float samplerFxHpfMix;
    float samplerFxHpfFreq;
    float samplerFxHpfBandwidth;
    int samplerFxHpfAlphaTouch;
    int samplerFxHpfAtReverse;
    float samplerFxHpfAtIntensity;
    //BPF
    float samplerFxBpfMix;
    float samplerFxBpfFreq;
    float samplerFxBpfBandwidth;
    int samplerFxBpfAlphaTouch;
    int samplerFxBpfAtReverse;
    float samplerFxBpfAtIntensity;
    //Delay
    float samplerFxDelayMix;
    float samplerFxDelayTime;
    float samplerFxDelayFeedback;
    float samplerFxDelayLpfFreq;
    float samplerFxDelayHpfFreq;
    int samplerFxDelaySync;
    int samplerFxDelayTimeMenu;
    int samplerFxDelayAlphaTouch;
    int samplerFxDelayAtReverse;
    float samplerFxDelayAtIntensity;
    
    //Reverb
    float samplerFxReverbMix;
    float samplerFxReverbRoomSize;
    float samplerFxReverbDamping;
    float samplerFxReverbWidth;
    float samplerFxReverbFreezeMode;
    int samplerFxReverbAlphaTouch;
    int samplerFxReverbAtReverse;
    float samplerFxReverbAtIntensity;
    
    //Flanger
    double samplerFxFlangerMix;
    double samplerFxFlangerRate;
    double samplerFxFlangerFeedback;
    double samplerFxFlangerIntensity;
    int samplerFxFlangerSync;
    int samplerFxFlangerRateMenu;
    int samplerFxFlangerAlphaTouch;
    int samplerFxFlangerAtReverse;
    double samplerFxFlangerAtIntensity;
    
    //Tremolo
    double samplerFxTremoloDepth;
    double samplerFxTremoloRate;
    int samplerFxTremoloShape;
    int samplerFxTremoloSync;
    int samplerFxTremoloRateMenu;
    int samplerFxTremoloAlphaTouch;
    int samplerFxTremoloAtReverse;
    float samplerFxTremoloAtIntensity;
    
    
    
    
    //==================================================================
    //Sequencer mode
    int sequencerMode; //Midi or Samples
    int sequencerData[NO_OF_SEQS][NO_OF_ROWS][NO_OF_COLUMNS]; //[sequence][row][column]
    int sequencerNumberOfSequences;
    int sequencerTriggerMode;
    int sequencerShouldLoop;
    int sequencerIndestructible;
    int sequencerShouldFinishLoop;
    int sequencerSticky;
    int sequencerLength;
    int sequencerRelativeTempoMode;
    int sequencerDynamicMode;
    
    int sequencerMidiNote[NO_OF_ROWS]; //assuming there are 12 rows to a sequencer
    int sequencerMidiVelocity; //NO LONGER NEED THIS
    int sequencerMidiChannel;
    int sequencerMidiNoteLength;
    int sequencerMidiMinPressureRange;
    int sequencerMidiMaxPressureRange;
    int sequencerMidiPressureMode;
    int sequencerMidiCcController;
    bool sequencerMidiPressureStatus;
    
    String sequencerSamplesAudioFilePath[NO_OF_ROWS];
    float sequencerPan;
    float sequencerGain;
    
    //this is the sequence data variable that is saved to xml, NOT the 3D int array above!
    String sequencerDataString[NO_OF_SEQS];
    
    
    //possible extra settings - gain and pan for each individual samples
    
    //==================================================================
    //Controller Mode
    int controllerControl; //type of control
    int controllerPresetNumber; //if control is set to preset switcher, this selects the preset number
    String controllerOscIpAddress;
    int controllerOscPort;
    int controllerMidiProgramChangeNumber;
    int controllerMidiProgramChangeChannel;
    
};


#endif // H_PADSETTINGS



