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
//don;t want unncessary data loaded up when switching between scenes.

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
    void setPressureSensitivityMode (int value);
    void setExclusiveMode (int value);
    void setExclusiveGroup (int value);
    void setQuantizeMode (int value);

    int getMode();
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
    void setSamplerAttackTime (double value);
    void setSamplerReleaseTime (double value);
    
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
    double getSamplerAttackTime();
    double getSamplerReleaseTime();
    
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
    void setSequencerEffect (int value);
    void setSequencerPan (float value);
    void setSequencerGain (float value);
    void setSequencerSamplesAttackTime (double value);
    
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
    int getSequencerEffect();
    float getSequencerPan();
    float getSequencerGain();
    double getSequencerSamplesAttackTime();
    
    //used to encode the sequence data into a more friendly form for saving to xml
    void seqDataToString();
    void stringToSeqData (String savedString, int seqNumber);
    void stringToSeqDataFormatConversion (String savedString, int seqNumber);   //function for converting seq data of the old 
                                                                                //format to the new format
                                                                       
    String getSequencerDataString (int seqNumber);
    void clearSequencerDataString (int seqNumber);
    
    void setSequencerRecordEnabled (bool value);
    bool getSequencerRecordEnabled();
    
    //see private members section below for notes on other needed Sequencer methods
    
    //==================================================================
    //Controller Mode
    void setControllerControl (int value);
    void setControllerSceneNumber (int value);
    void setControllerOscIpAddress (String value);
    void setControllerOscPort (int value);
    void setControllerMidiProgramChangeNumber (int value);
    void setControllerMidiProgramChangeChannel (int value);
    
    int getControllerControl();
    int getControllerSceneNumber();
    String getControllerOscIpAddress();
    int getControllerOscPort();
    int getControllerMidiProgramChangeNumber();
    int getControllerMidiProgramChangeChannel();

    
    //==================================================================
    //Pad effects
    
    //Gain&Pan
    void setPadFxGainPanGain (float value);
    void setPadFxGainPanPan (float value);
    void setPadFxGainPanAlphaTouch (int value);
    void setPadFxGainPanAtReverse (int value);
    void setPadFxGainPanAtIntensity (float value);
    float getPadFxGainPanGain();
    float getPadFxGainPanPan();
    int getPadFxGainPanAlphaTouch();
    int getPadFxGainPanAtReverse();
    float getPadFxGainPanAtIntensity();
    //LPF
    void setPadFxLpfMix (float value);
    void setPadFxLpfFreq (float value);
    void setPadFxLpfBandwidth (float value);
    void setPadFxLpfAlphaTouch (int value);
    void setPadFxLpfAtReverse (int value);
    void setPadFxLpfAtIntensity (float value);
    float getPadFxLpfMix();
    float getPadFxLpfFreq();
    float getPadFxLpfBandwidth();
    int getPadFxLpfAlphaTouch();
    int getPadFxLpfAtReverse();
    float getPadFxLpfAtIntensity();
    //HPF
    void setPadFxHpfMix (float value);
    void setPadFxHpfFreq (float value);
    void setPadFxHpfBandwidth (float value);
    void setPadFxHpfAlphaTouch (int value);
    void setPadFxHpfAtReverse (int value);
    void setPadFxHpfAtIntensity (float value);
    float getPadFxHpfMix();
    float getPadFxHpfFreq();
    float getPadFxHpfBandwidth();
    int getPadFxHpfAlphaTouch();
    int getPadFxHpfAtReverse();
    float getPadFxHpfAtIntensity();
    //BPF
    void setPadFxBpfMix (float value);
    void setPadFxBpfFreq (float value);
    void setPadFxBpfBandwidth (float value);
    void setPadFxBpfAlphaTouch (int value);
    void setPadFxBpfAtReverse (int value);
    void setPadFxBpfAtIntensity (float value);
    float getPadFxBpfMix();
    float getPadFxBpfFreq();
    float getPadFxBpfBandwidth();
    int getPadFxBpfAlphaTouch();
    int getPadFxBpfAtReverse();
    float getPadFxBpfAtIntensity();
    //delay
    void setPadFxDelayMix (float value);
    void setPadFxDelayTime (float value);
    void setPadFxDelayFeedback (float value);
    void setPadFxDelayLpfFreq (float value);
    void setPadFxDelayHpfFreq (float value);
    void setPadFxDelaySync (int value);
    void setPadFxDelayTimeMenu (int value);
    void setPadFxDelayAlphaTouch (int value);
    void setPadFxDelayAtReverse (int value);
    void setPadFxDelayAtIntensity (float value);
    float getPadFxDelayMix();
    float getPadFxDelayTime();
    float getPadFxDelayFeedback();
    float getPadFxDelayLpfFreq();
    float getPadFxDelayHpfFreq();
    int getPadFxDelaySync();
    int getPadFxDelayTimeMenu();
    int getPadFxDelayAlphaTouch();
    int getPadFxDelayAtReverse();
    float getPadFxDelayAtIntensity();
    //reverb
    void setPadFxReverbMix (float value);
    void setPadFxReverbRoomSize (float value);
    void setPadFxReverbDamping (float value);
    void setPadFxReverbWidth (float value);
    void setPadFxReverbFreezeMode (float value);
    void setPadFxReverbAlphaTouch (int value);
    void setPadFxReverbAtReverse (int value);
    void setPadFxReverbAtIntensity (float value);
    float getPadFxReverbMix();
    float getPadFxReverbRoomSize();
    float getPadFxReverbDamping();
    float getPadFxReverbWidth();
    float getPadFxReverbFreezeMode();
    int getPadFxReverbAlphaTouch();
    int getPadFxReverbAtReverse();
    float getPadFxReverbAtIntensity();
    //flanger
    void setPadFxFlangerMix (double value);
    void setPadFxFlangerRate(double value);
    void setPadFxFlangerFeedback (double value);
    void setPadFxFlangerIntensity (double value);
    void setPadFxFlangerSync (int value);
    void setPadFxFlangerRateMenu (int value);
    void setPadFxFlangerAlphaTouch (int value);
    void setPadFxFlangerAtReverse (int value);
    void setPadFxFlangerAtIntensity (double value);
    double getPadFxFlangerMix();
    double getPadFxFlangerRate();
    double getPadFxFlangerFeedback();
    double getPadFxFlangerIntensity();
    int getPadFxFlangerSync();
    int getPadFxFlangerRateMenu();
    int getPadFxFlangerAlphaTouch();
    int getPadFxFlangerAtReverse();
    double getPadFxFlangerAtIntensity();
    //tremolo
    void setPadFxTremoloDepth (double value);
    void setPadFxTremoloRate (double value);
    void setPadFxTremoloShape (int value);
    void setPadFxTremoloSync (int value);
    void setPadFxTremoloRateMenu (int value);
    void setPadFxTremoloAlphaTouch (int value);
    void setPadFxTremoloAtReverse (int value);
    void setPadFxTremoloAtIntensity (double value);
    double getPadFxTremoloDepth();
    double getPadFxTremoloRate();
    int getPadFxTremoloShape();
    int getPadFxTremoloSync();
    int getPadFxTremoloRateMenu();
    int getPadFxTremoloAlphaTouch();
    int getPadFxTremoloAtReverse();
    double getPadFxTremoloAtIntensity();
    
    //==================================================================
    //==================================================================
    //==================================================================
    
private:
    
    int padNumber;
    AlphaLiveEngine *alphaLiveEngineRef;
    
    //==================================================================
    //Global
    int mode; // 0 = off, 1 = midi, 2 = sampler, 3 = sequencer, 4 = controller
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
    double samplerAttackTime;
    double samplerReleaseTime;
    
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
    int sequencerEffect;
    float sequencerPan;
    float sequencerGain;
    double sequencerSamplesAttackTime;
    
    //this is the sequence data variable that is saved to xml, NOT the 3D int array above!
    String sequencerDataString[NO_OF_SEQS];
    
    bool sequencerRecordEnabled;
    
    //possible extra settings - gain and pan for each individual samples
    
    //==================================================================
    //Controller Mode
    int controllerControl; //type of control
    int controllerSceneNumber; //if control is set to scene switcher, this selects the scene number
    String controllerOscIpAddress;
    int controllerOscPort;
    int controllerMidiProgramChangeNumber;
    int controllerMidiProgramChangeChannel;
    
    
    //==================================================================
    //Pad Effects (now applies to Sampler Mode and Sequencer Mode)
    
    //Gain&Pan
    float padFxGainPanGain;
    float padFxGainPanPan;
    int padFxGainPanAlphaTouch;
    int padFxGainPanAtReverse;
    float padFxGainPanAtIntensity;
    //LPF
    float padFxLpfMix;
    float padFxLpfFreq;
    float padFxLpfBandwidth;
    int padFxLpfAlphaTouch;
    int padFxLpfAtReverse;
    float padFxLpfAtIntensity;
    //HPF
    float padFxHpfMix;
    float padFxHpfFreq;
    float padFxHpfBandwidth;
    int padFxHpfAlphaTouch;
    int padFxHpfAtReverse;
    float padFxHpfAtIntensity;
    //BPF
    float padFxBpfMix;
    float padFxBpfFreq;
    float padFxBpfBandwidth;
    int padFxBpfAlphaTouch;
    int padFxBpfAtReverse;
    float padFxBpfAtIntensity;
    //Delay
    float padFxDelayMix;
    float padFxDelayTime;
    float padFxDelayFeedback;
    float padFxDelayLpfFreq;
    float padFxDelayHpfFreq;
    int padFxDelaySync;
    int padFxDelayTimeMenu;
    int padFxDelayAlphaTouch;
    int padFxDelayAtReverse;
    float padFxDelayAtIntensity;
    
    //Reverb
    float padFxReverbMix;
    float padFxReverbRoomSize;
    float padFxReverbDamping;
    float padFxReverbWidth;
    float padFxReverbFreezeMode;
    int padFxReverbAlphaTouch;
    int padFxReverbAtReverse;
    float padFxReverbAtIntensity;
    
    //Flanger
    double padFxFlangerMix;
    double padFxFlangerRate;
    double padFxFlangerFeedback;
    double padFxFlangerIntensity;
    int padFxFlangerSync;
    int padFxFlangerRateMenu;
    int padFxFlangerAlphaTouch;
    int padFxFlangerAtReverse;
    double padFxFlangerAtIntensity;
    
    //Tremolo
    double padFxTremoloDepth;
    double padFxTremoloRate;
    int padFxTremoloShape;
    int padFxTremoloSync;
    int padFxTremoloRateMenu;
    int padFxTremoloAlphaTouch;
    int padFxTremoloAtReverse;
    float padFxTremoloAtIntensity;
    
};


#endif // H_PADSETTINGS



