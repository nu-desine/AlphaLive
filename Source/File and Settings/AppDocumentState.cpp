//
//  AppDocumentState.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 07/11/2011.
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

#include "AppDocumentState.h"
#include "AppSettings.h"
#include "../Functionality Classes/Sequencer Mode/SequencerValues.h"
#include "../Application/MainWindow.h"
#include "StoredSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[i]
#define PAD_SETTINGS_pad AppSettings::Instance()->padSettings[currentlySelectedPad]
#define PAD_SETTINGS_pads AppSettings::Instance()->padSettings[padNum]

AppDocumentState::AppDocumentState()
{
	#if JUCE_WINDOWS
	//set windows file association
	WindowsRegistry::registerFileAssociation(	".alphalive", 
												"alphalive", 
												"AlphaLive Project File", 
												File::getSpecialLocation(File::currentExecutableFile), 
												106, //This is the document_icon file ID that can be found by going to File->Opem->File->.exe
												true);
	#endif

    for (int i = 0; i <= NO_OF_SCENES-1; i++)
    {
        sceneData.insert(i, new XmlElement("SCENE_" + String(i)));
    }
    
    projectData = new XmlElement("PROJECT_SETTINGS");
    
    guiUpdateFlag = 0;
    sceneToUpdate = 0;
    sceneStatus = 0;
    
    currentProjectFile = File::nonexistent;
    
    mainAppWindowRef = nullptr;
    
    shouldDisplayAlertWindow = true;
    currentlySelectedScene = 0;
    
    numOfFilesAtStart = 0;
}


AppDocumentState::~AppDocumentState()
{

}

void AppDocumentState::setMainAppWindowRef (MainAppWindow *ref)
{
    mainAppWindowRef = ref;
}

void AppDocumentState::savePadSettings (int padNumber, XmlElement *padData)
{
    int i = padNumber;
    
    //put data into this the temp XmlElement
    padData->setAttribute("mode", PAD_SETTINGS->getMode());
    padData->setAttribute("pressureCurve", PAD_SETTINGS->getPressureCurve());
    padData->setAttribute("exclusiveMode", PAD_SETTINGS->getExclusiveMode());
    padData->setAttribute("exclusiveGroup", PAD_SETTINGS->getExclusiveGroup());
    padData->setAttribute("quantizeMode", PAD_SETTINGS->getQuantizeMode());
    padData->setAttribute("velocityCurve", PAD_SETTINGS->getVelocityCurve());
    padData->setAttribute("velocity", PAD_SETTINGS->getStaticVelocity());
    
    //only save whats necessary
    if (PAD_SETTINGS->getMode() == 1) //midi mode
    {
        padData->setAttribute("midiNote", PAD_SETTINGS->getMidiNote());
        padData->setAttribute("midiChannel", PAD_SETTINGS->getMidiChannel());
        padData->setAttribute("midiMinPressureRange", PAD_SETTINGS->getMidiMinPressureRange());
        padData->setAttribute("midiMaxPressureRange", PAD_SETTINGS->getMidiMaxPressureRange());
        padData->setAttribute("midiPressureMode", PAD_SETTINGS->getMidiPressureMode());
        padData->setAttribute("midiTriggerMode", PAD_SETTINGS->getMidiTriggerMode());
        padData->setAttribute("midiIndestructible", PAD_SETTINGS->getMidiIndestructible());
        padData->setAttribute("midiSticky", PAD_SETTINGS->getMidiSticky());
        padData->setAttribute("midiPressureStatus", PAD_SETTINGS->getMidiPressureStatus());
        padData->setAttribute("midiNoteStatus", PAD_SETTINGS->getMidiNoteStatus());
        padData->setAttribute("midiCcController", PAD_SETTINGS->getMidiCcController());
    }
    else if (PAD_SETTINGS->getMode() == 2) //sampler mode
    {
        
        if (AppSettings::Instance()->getCopyExternalFiles() == true)
        {
            //if project currently allows audio files to be copied to project, only save the file name
            padData->setAttribute("samplerAudioFilePath", PAD_SETTINGS->getSamplerAudioFilePath().getFileName());
            
        }
        else if (AppSettings::Instance()->getCopyExternalFiles() == false)
        {
            //else save the full pathname
            padData->setAttribute("samplerAudioFilePath", PAD_SETTINGS->getSamplerAudioFilePath().getFullPathName());
        }
        
        padData->setAttribute("samplerTriggerMode", PAD_SETTINGS->getSamplerTriggerMode());
        padData->setAttribute("samplerShouldLoop", PAD_SETTINGS->getSamplerShouldLoop());
        padData->setAttribute("samplerIndestructible", PAD_SETTINGS->getSamplerIndestructible());
        padData->setAttribute("samplerShouldFinishLoop", PAD_SETTINGS->getSamplerShouldFinishLoop());
        padData->setAttribute("samplerSticky", PAD_SETTINGS->getSamplerSticky());
        padData->setAttribute("samplerEffect", PAD_SETTINGS->getSamplerEffect());
        padData->setAttribute("samplerPan", PAD_SETTINGS->getSamplerPan());
        padData->setAttribute("samplerGain", PAD_SETTINGS->getSamplerGain());
        padData->setAttribute("samplerAttackTime", PAD_SETTINGS->getSamplerAttackTime());
        padData->setAttribute("samplerReleaseTime", PAD_SETTINGS->getSamplerReleaseTime());
        
    }
    else if (PAD_SETTINGS->getMode() == 3) //sequencer mode
    {
        padData->setAttribute("sequencerMode", PAD_SETTINGS->getSequencerMode());
        for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
        {
            padData->setAttribute("newSequencerData"+String(seq), PAD_SETTINGS->getSequencerDataString(seq));
        }
        
        padData->setAttribute("sequencerNumberOfSequences", PAD_SETTINGS->getSequencerNumberOfSequences());
        padData->setAttribute("sequencerTriggerMode", PAD_SETTINGS->getSequencerTriggerMode());
        padData->setAttribute("sequencerShouldLoop", PAD_SETTINGS->getSequencerShouldLoop());
        padData->setAttribute("sequencerIndestructible", PAD_SETTINGS->getSequencerIndestructible());
        padData->setAttribute("sequencerShouldFinishLoop", PAD_SETTINGS->getSequencerShouldFinishLoop());
        padData->setAttribute("sequencerSticky", PAD_SETTINGS->getSequencerSticky());
        padData->setAttribute("sequencerLength", PAD_SETTINGS->getSequencerLength());
        padData->setAttribute("sequencerRelativeTempoMode", PAD_SETTINGS->getSequencerRelativeTempoMode());
        padData->setAttribute("sequencerDynamicMode", PAD_SETTINGS->getSequencerDynamicMode());
        
        if (PAD_SETTINGS->getSequencerMode() == 1) //sequencer midi mode
        {
            for (int row = 0; row <= NO_OF_ROWS-1; row++)
            {
                padData->setAttribute("sequencerMidiNote"+String(row), PAD_SETTINGS->getSequencerMidiNote(row));
            }
            padData->setAttribute("sequencerMidiChannel", PAD_SETTINGS->getSequencerMidiChannel());
            padData->setAttribute("sequencerMidiNoteLength", PAD_SETTINGS->getSequencerMidiNoteLength());
            padData->setAttribute("sequencerMidiMinPressureRange", PAD_SETTINGS->getSequencerMidiMinPressureRange());
            padData->setAttribute("sequencerMidiMaxPressureRange", PAD_SETTINGS->getSequencerMidiMaxPressureRange());
            padData->setAttribute("sequencerMidiPressureMode", PAD_SETTINGS->getSequencerMidiPressureMode());
            padData->setAttribute("sequencerMidiPressureStatus", PAD_SETTINGS->getSequencerMidiPressureStatus());
            padData->setAttribute("sequencerMidiCcController", PAD_SETTINGS->getSequencerMidiCcController());
            
        }
        else if (PAD_SETTINGS->getSequencerMode() == 2) //sequencer samples mode
        {
            for (int row = 0; row <= NO_OF_ROWS-1; row++)
            {
                if (AppSettings::Instance()->getCopyExternalFiles() == true)
                {
                    //if project currently allows external audio files to be copied to project, only save the file name
                    padData->setAttribute("sequencerSamplesAudioFilePath"+String(row), PAD_SETTINGS->getSequencerSamplesAudioFilePath(row).getFileName());
                }
                else
                {
                    //else save the full pathname
                    padData->setAttribute("sequencerSamplesAudioFilePath"+String(row), PAD_SETTINGS->getSequencerSamplesAudioFilePath(row).getFullPathName());
                }
            }
            padData->setAttribute("sequencerEffect", PAD_SETTINGS->getSequencerEffect());
            padData->setAttribute("sequencerPan", PAD_SETTINGS->getSequencerPan());
            padData->setAttribute("sequencerGain", PAD_SETTINGS->getSequencerGain());
            padData->setAttribute("sequencerSamplesAttack", PAD_SETTINGS->getSequencerSamplesAttackTime());
        }
    }
    
    else if (PAD_SETTINGS->getMode() == 4) //controller mode
    {
        padData->setAttribute("controllerControl", PAD_SETTINGS->getControllerControl());
        padData->setAttribute("controllerSceneNumber", PAD_SETTINGS->getControllerSceneNumber());
        padData->setAttribute("controllerOscIpAddress", PAD_SETTINGS->getControllerOscIpAddress());
        padData->setAttribute("controllerOscPortNumber", PAD_SETTINGS->getControllerOscPort());
        padData->setAttribute("controllerMidiProgramChangeNumber", PAD_SETTINGS->getControllerMidiProgramChangeNumber());
        padData->setAttribute("controllerMidiProgramChangeChannel", PAD_SETTINGS->getControllerMidiProgramChangeChannel());
    }
    
    int modeCheck = PAD_SETTINGS->getMode();
    int effect = 0;
    if (modeCheck == 2)
        effect = PAD_SETTINGS->getSamplerEffect();
    else if (modeCheck == 3)
    {
        if (PAD_SETTINGS->getSequencerMode() == 2)
            effect = PAD_SETTINGS->getSequencerEffect();
    }
    
    if (effect == 1) //Gain and Pan
    {
        padData->setAttribute("padFxGainPanGain", PAD_SETTINGS->getPadFxGainPanGain());
        padData->setAttribute("padFxGainPanPan", PAD_SETTINGS->getPadFxGainPanPan());
        padData->setAttribute("padFxGainPanAlphaTouch", PAD_SETTINGS->getPadFxGainPanAlphaTouch());
        padData->setAttribute("padFxGainPanAtReverse", PAD_SETTINGS->getPadFxGainPanAtReverse());
        padData->setAttribute("padFxGainPanAtIntensity", PAD_SETTINGS->getPadFxGainPanAtIntensity());
    }
    else if (effect == 2) //LPF
    {
        padData->setAttribute("padFxLpfMix", PAD_SETTINGS->getPadFxLpfMix());
        padData->setAttribute("padFxLpfFreq", PAD_SETTINGS->getPadFxLpfFreq());
        padData->setAttribute("padFxLpfBandwidth", PAD_SETTINGS->getPadFxLpfBandwidth());
        padData->setAttribute("padFxLpfAlphaTouch", PAD_SETTINGS->getPadFxLpfAlphaTouch());
        padData->setAttribute("padFxLpfAtReverse", PAD_SETTINGS->getPadFxLpfAtReverse());
        padData->setAttribute("padFxLpfAtIntensity", PAD_SETTINGS->getPadFxLpfAtIntensity());
    }
    else if (effect == 3) //HPF
    {
        padData->setAttribute("padFxHpfMix", PAD_SETTINGS->getPadFxHpfMix());
        padData->setAttribute("padFxHpfFreq", PAD_SETTINGS->getPadFxHpfFreq());
        padData->setAttribute("padFxHpfBandwidth", PAD_SETTINGS->getPadFxHpfBandwidth());
        padData->setAttribute("padFxHpfAlphaTouch", PAD_SETTINGS->getPadFxHpfAlphaTouch());
        padData->setAttribute("padFxHpfAtReverse", PAD_SETTINGS->getPadFxHpfAtReverse());
        padData->setAttribute("padFxHpfAtIntensity", PAD_SETTINGS->getPadFxHpfAtIntensity());
    }
    else if (effect == 4) //BPF
    {
        padData->setAttribute("padFxBpfMix", PAD_SETTINGS->getPadFxBpfMix());
        padData->setAttribute("padFxBpfFreq", PAD_SETTINGS->getPadFxBpfFreq());
        padData->setAttribute("padFxBpfBandwidth", PAD_SETTINGS->getPadFxBpfBandwidth());
        padData->setAttribute("padFxBpfAlphaTouch", PAD_SETTINGS->getPadFxBpfAlphaTouch());
        padData->setAttribute("padFxBpfAtReverse", PAD_SETTINGS->getPadFxBpfAtReverse());
        padData->setAttribute("padFxBpfAtIntensity", PAD_SETTINGS->getPadFxBpfAtIntensity());
    }
	else if (effect == 5) //Distortion
    {
        padData->setAttribute("padFxDistortionInputGain", PAD_SETTINGS->getPadFxDistortionInputGain());
        padData->setAttribute("padFxDistortionDrive", PAD_SETTINGS->getPadFxDistortionDrive());
		padData->setAttribute("padFxDistortionWetDryMix", PAD_SETTINGS->getPadFxDistortionWetDryMix());
        padData->setAttribute("padFxDistortionTone", PAD_SETTINGS->getPadFxDistortionTone());
        padData->setAttribute("padFxDistortionTypeMenu", PAD_SETTINGS->getPadFxDistortionTypeMenu());
        padData->setAttribute("padFxDistortionAlphaTouch", PAD_SETTINGS->getPadFxDistortionAlphaTouch());
        padData->setAttribute("padFxDistortionAtReverse", PAD_SETTINGS->getPadFxDistortionAtReverse());
        padData->setAttribute("padFxDistortionAtIntensity", PAD_SETTINGS->getPadFxDistortionAtIntensity());
    }
	else if (effect == 6) //Bitcrusher
    {
        padData->setAttribute("padFxBitcrusherInputGain", PAD_SETTINGS->getPadFxBitcrusherInputGain());
		padData->setAttribute("padFxBitcrusherDownsample", PAD_SETTINGS->getPadFxBitcrusherDownsample());
		padData->setAttribute("padFxBitcrusherCrush", PAD_SETTINGS->getPadFxBitcrusherCrush());
		padData->setAttribute("padFxBitcrusherSmoothing", PAD_SETTINGS->getPadFxBitcrusherSmoothing());
		padData->setAttribute("padFxBitcrusherWetDryMix", PAD_SETTINGS->getPadFxBitcrusherWetDryMix());
        padData->setAttribute("padFxBitcrusherAlphaTouch", PAD_SETTINGS->getPadFxBitcrusherAlphaTouch());
        padData->setAttribute("padFxBitcrusherAtReverse", PAD_SETTINGS->getPadFxBitcrusherAtReverse());
        padData->setAttribute("padFxBitcrusherAtIntensity", PAD_SETTINGS->getPadFxBitcrusherAtIntensity());
    }
    else if (effect == 7) //Delay
    {
        padData->setAttribute("padFxDelayMix", PAD_SETTINGS->getPadFxDelayMix());
        padData->setAttribute("padFxDelayTime", PAD_SETTINGS->getPadFxDelayTime());
        padData->setAttribute("padFxDelayFeedback", PAD_SETTINGS->getPadFxDelayFeedback());
        padData->setAttribute("padFxDelayLpfFreq", PAD_SETTINGS->getPadFxDelayLpfFreq());
        padData->setAttribute("padFxDelayHpfFreq", PAD_SETTINGS->getPadFxDelayHpfFreq());
        padData->setAttribute("padFxDelaySync", PAD_SETTINGS->getPadFxDelaySync());
        padData->setAttribute("padFxDelayTimeMenu", PAD_SETTINGS->getPadFxDelayTimeMenu());
        padData->setAttribute("padFxDelayAlphaTouch", PAD_SETTINGS->getPadFxDelayAlphaTouch());
        padData->setAttribute("padFxDelayAtReverse", PAD_SETTINGS->getPadFxDelayAtReverse());
        padData->setAttribute("padFxDelayAtIntensity", PAD_SETTINGS->getPadFxDelayAtIntensity());
    }
    else if (effect == 8) //Reverb
    {
        padData->setAttribute("padFxReverbMix", PAD_SETTINGS->getPadFxReverbMix());
        padData->setAttribute("padFxReverbRoomSize", PAD_SETTINGS->getPadFxReverbRoomSize());
        padData->setAttribute("padFxReverbDamping", PAD_SETTINGS->getPadFxReverbDamping());
        padData->setAttribute("padFxReverbWidth", PAD_SETTINGS->getPadFxReverbWidth());
        padData->setAttribute("padFxReverbFreezeMode", PAD_SETTINGS->getPadFxReverbFreezeMode());
        padData->setAttribute("padFxReverbAlphaTouch", PAD_SETTINGS->getPadFxReverbAlphaTouch());
        padData->setAttribute("padFxReverbAtReverse", PAD_SETTINGS->getPadFxReverbAtReverse());
        padData->setAttribute("padFxReverbAtIntensity", PAD_SETTINGS->getPadFxReverbAtIntensity());
    }
    else if (effect == 9) //Flanger
    {
        padData->setAttribute("padFxFlangerMix", PAD_SETTINGS->getPadFxFlangerMix());
        padData->setAttribute("padFxFlangerRate", PAD_SETTINGS->getPadFxFlangerRate());
        padData->setAttribute("padFxFlangerFeedback", PAD_SETTINGS->getPadFxFlangerFeedback());
        padData->setAttribute("padFxFlangerIntensity", PAD_SETTINGS->getPadFxFlangerIntensity());
        padData->setAttribute("padFxFlangerRateMenu", PAD_SETTINGS->getPadFxFlangerRateMenu());
        padData->setAttribute("padFxFlangerSync", PAD_SETTINGS->getPadFxFlangerSync());
        padData->setAttribute("padFxFlangerAlphaTouch", PAD_SETTINGS->getPadFxFlangerAlphaTouch());
        padData->setAttribute("padFxFlangerAtReverse", PAD_SETTINGS->getPadFxFlangerAtReverse());
        padData->setAttribute("padFxFlangerAtIntensity", PAD_SETTINGS->getPadFxFlangerAtIntensity());
    }
    else if (effect == 10) //Tremolo
    {
        padData->setAttribute("padFxTremoloDepth", PAD_SETTINGS->getPadFxTremoloDepth());
        padData->setAttribute("padFxTremoloRate", PAD_SETTINGS->getPadFxTremoloRate());
        padData->setAttribute("padFxTremoloShape", PAD_SETTINGS->getPadFxTremoloShape());
        padData->setAttribute("padFxTremoloSync", PAD_SETTINGS->getPadFxTremoloSync());
        padData->setAttribute("padFxTremoloRateMenu", PAD_SETTINGS->getPadFxTremoloRateMenu());
        padData->setAttribute("padFxTremoloAlphaTouch", PAD_SETTINGS->getPadFxTremoloAlphaTouch());
        padData->setAttribute("padFxTremoloAtReverse", PAD_SETTINGS->getPadFxTremoloAtReverse());
        padData->setAttribute("padFxTremoloAtIntensity", PAD_SETTINGS->getPadFxTremoloAtIntensity());
    }
}







void AppDocumentState::loadPadSettings (int padNumber, XmlElement *padData)
{
    int i = padNumber;
    
    if (padData->hasAttribute("mode")) //WHICH IT SHOULD ALWAYS HAVE!
        PAD_SETTINGS->setMode(padData->getIntAttribute("mode"));
    
    if (padData->hasAttribute("pressureSensitivityMode")) //old attribute name
        PAD_SETTINGS->setPressureCurve(padData->getIntAttribute("pressureSensitivityMode"));
    else if(padData->hasAttribute("pressureCurve")) //new attribute name
        PAD_SETTINGS->setPressureCurve(padData->getIntAttribute("pressureCurve"));
    
    if (padData->hasAttribute("exclusiveMode"))
        PAD_SETTINGS->setExclusiveMode(padData->getIntAttribute("exclusiveMode"));
    if (padData->hasAttribute("exclusiveGroup"))
        PAD_SETTINGS->setExclusiveGroup(padData->getIntAttribute("exclusiveGroup"));
    if (padData->hasAttribute("quantizeMode"))
        PAD_SETTINGS->setQuantizeMode(padData->getIntAttribute("quantizeMode"));
    if (padData->hasAttribute("velocityCurve"))
        PAD_SETTINGS->setVelocityCurve(padData->getIntAttribute("velocityCurve"));
    if (padData->hasAttribute("velocity"))
        PAD_SETTINGS->setStaticVelocity(padData->getIntAttribute("velocity"));
    
    //only load needed data to reduce loading times and CPU usage, plus
    //can not load settings into seq and sampler modes where the pads player objects don't exist (yet)
    
    //midi mode
    if (PAD_SETTINGS->getMode() == 1)
    {
        if (padData->hasAttribute("midiNote"))
            PAD_SETTINGS->setMidiNote(padData->getIntAttribute("midiNote"));
        if (padData->hasAttribute("midiChannel"))
            PAD_SETTINGS->setMidiChannel(padData->getIntAttribute("midiChannel"));
        if (padData->hasAttribute("midiMinPressureRange"))
            PAD_SETTINGS->setMidiMinPressureRange(padData->getIntAttribute("midiMinPressureRange"));
        if (padData->hasAttribute("midiMaxPressureRange"))
            PAD_SETTINGS->setMidiMaxPressureRange(padData->getIntAttribute("midiMaxPressureRange"));
        if (padData->hasAttribute("midiPressureMode"))
            PAD_SETTINGS->setMidiPressureMode(padData->getIntAttribute("midiPressureMode"));
        if (padData->hasAttribute("midiTriggerMode"))
            PAD_SETTINGS->setMidiTriggerMode(padData->getIntAttribute("midiTriggerMode"));
        if (padData->hasAttribute("midiIndestructible"))
            PAD_SETTINGS->setMidiIndestructible(padData->getIntAttribute("midiIndestructible"));
        if (padData->hasAttribute("midiSticky"))
            PAD_SETTINGS->setMidiSticky(padData->getIntAttribute("midiSticky"));
        if (padData->hasAttribute("midiPressureStatus"))
            PAD_SETTINGS->setMidiPressureStatus(padData->getBoolAttribute("midiPressureStatus"));
        if (padData->hasAttribute("midiNoteStatus"))
            PAD_SETTINGS->setMidiNoteStatus(padData->getBoolAttribute("midiNoteStatus"));
        if (padData->hasAttribute("midiCcController"))
            PAD_SETTINGS->setMidiCcController(padData->getIntAttribute("midiCcController"));
        
    }
    
    //sampler mode
    else if (PAD_SETTINGS->getMode() == 2)
    {
        File newFile;
        String newFileString(String::empty);
        
        if (padData->hasAttribute("samplerAudioFilePath"))
        {
            newFileString = padData->getStringAttribute("samplerAudioFilePath");
            
            if (newFileString != String::empty) //to prevent trying to load in a file if there's nothing to load
            {
                if (File::isAbsolutePath(newFileString) == false)
                {
                    //check if the saved audio file path is just the file name
                    //if so, get it from the working directory and apply the full pathname to it
                    newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + newFileString;
                }
                else if (File::isAbsolutePath(newFileString) == true)
                {
                    //else, it should be the full path name already
                    newFile = newFileString;
                }
                
                PAD_SETTINGS->setSamplerAudioFilePath(newFile);
            }
            else
                PAD_SETTINGS->setSamplerAudioFilePath(File::nonexistent);
        }
        
        if (padData->hasAttribute("samplerTriggerMode"))
            PAD_SETTINGS->setSamplerTriggerMode(padData->getIntAttribute("samplerTriggerMode"));
        if (padData->hasAttribute("samplerShouldLoop"))
            PAD_SETTINGS->setSamplerShouldLoop(padData->getIntAttribute("samplerShouldLoop"));
        if (padData->hasAttribute("samplerIndestructible"))
            PAD_SETTINGS->setSamplerIndestructible(padData->getIntAttribute("samplerIndestructible"));
        if (padData->hasAttribute("samplerShouldFinishLoop"))
            PAD_SETTINGS->setSamplerShouldFinishLoop(padData->getIntAttribute("samplerShouldFinishLoop"));
        if (padData->hasAttribute("samplerSticky"))
            PAD_SETTINGS->setSamplerSticky(padData->getIntAttribute("samplerSticky"));
        if (padData->hasAttribute("samplerEffect"))
            PAD_SETTINGS->setSamplerEffect(padData->getIntAttribute("samplerEffect"));
        if (padData->hasAttribute("samplerPan"))
            PAD_SETTINGS->setSamplerPan(padData->getDoubleAttribute("samplerPan"));
        if (padData->hasAttribute("samplerGain"))
            PAD_SETTINGS->setSamplerGain(padData->getDoubleAttribute("samplerGain"));
        if (padData->hasAttribute("samplerAttackTime"))
            PAD_SETTINGS->setSamplerAttackTime(padData->getDoubleAttribute("samplerAttackTime"));
        if (padData->hasAttribute("samplerReleaseTime"))
            PAD_SETTINGS->setSamplerReleaseTime(padData->getDoubleAttribute("samplerReleaseTime"));
        
    }
    
    //sequencer mode
    else if (PAD_SETTINGS->getMode() == 3)
    {
        if (padData->hasAttribute("sequencerMode")) //WHICH IT SHOULD ALWAYS HAVE
            PAD_SETTINGS->setSequencerMode(padData->getIntAttribute("sequencerMode"));
        
        //should probably change this now as there's no 'new' data - just data
        
        if (padData->hasAttribute("newSequencerData0") == true) //new seq data format (0-127)
        {
            for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
            {
                PAD_SETTINGS->stringToSeqData(padData->getStringAttribute("newSequencerData"+String(seq)), seq);
            }
        }
        
        /*
         else if (padData->hasAttribute("sequencerData0") == true) //old seq data format (0-1)
         {
         for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
         {
         //convert any '1s' in the string to '110'
         PAD_SETTINGS->stringToSeqDataFormatConversion(padData->getStringAttribute("sequencerData"+String(seq)), seq);
         }
         }
         */
        
        if (padData->hasAttribute("sequencerNumberOfSequences"))
            PAD_SETTINGS->setSequencerNumberOfSequences(padData->getIntAttribute("sequencerNumberOfSequences"));
        if (padData->hasAttribute("sequencerTriggerMode"))
            PAD_SETTINGS->setSequencerTriggerMode(padData->getIntAttribute("sequencerTriggerMode"));
        if (padData->hasAttribute("sequencerShouldLoop"))
            PAD_SETTINGS->setSequencerShouldLoop(padData->getIntAttribute("sequencerShouldLoop"));
        if (padData->hasAttribute("sequencerIndestructible"))
            PAD_SETTINGS->setSequencerIndestructible(padData->getIntAttribute("sequencerIndestructible"));
        if (padData->hasAttribute("sequencerShouldFinishLoop"))
            PAD_SETTINGS->setSequencerShouldFinishLoop(padData->getIntAttribute("sequencerShouldFinishLoop"));
        if (padData->hasAttribute("sequencerSticky"))
            PAD_SETTINGS->setSequencerSticky(padData->getIntAttribute("sequencerSticky"));
        if (padData->hasAttribute("sequencerLength"))
            PAD_SETTINGS->setSequencerLength(padData->getIntAttribute("sequencerLength"));
        if (padData->hasAttribute("sequencerRelativeTempoMode"))
            PAD_SETTINGS->setSequencerRelativeTempoMode(padData->getIntAttribute("sequencerRelativeTempoMode"));
        if (padData->hasAttribute("sequencerDynamicMode"))
            PAD_SETTINGS->setSequencerDynamicMode(padData->getIntAttribute("sequencerDynamicMode"));
        
        //sequencer midi mode
        if (padData->getIntAttribute("sequencerMode") == 1)
        {
            for (int row = 0; row <= NO_OF_ROWS-1; row++)
            {
                if (padData->hasAttribute("sequencerMidiNote"+String(row)))
                    PAD_SETTINGS->setSequencerMidiNote(padData->getIntAttribute("sequencerMidiNote"+String(row)), row);
            }
            
            if (padData->hasAttribute("sequencerMidiChannel"))
                PAD_SETTINGS->setSequencerMidiChannel(padData->getIntAttribute("sequencerMidiChannel"));
            if (padData->hasAttribute("sequencerMidiNoteLength"))
                PAD_SETTINGS->setSequencerMidiNoteLength(padData->getIntAttribute("sequencerMidiNoteLength"));
            if (padData->hasAttribute("sequencerMidiMinPressureRange"))
                PAD_SETTINGS->setSequencerMidiMinPressureRange(padData->getIntAttribute("sequencerMidiMinPressureRange"));
            if (padData->hasAttribute("sequencerMidiMaxPressureRange"))
                PAD_SETTINGS->setSequencerMidiMaxPressureRange(padData->getIntAttribute("sequencerMidiMaxPressureRange"));
            if (padData->hasAttribute("sequencerMidiPressureMode"))
                PAD_SETTINGS->setSequencerMidiPressureMode(padData->getIntAttribute("sequencerMidiPressureMode"));
            if (padData->hasAttribute("sequencerMidiPressureStatus"))
                PAD_SETTINGS->setSequencerMidiPressureStatus(padData->getBoolAttribute("sequencerMidiPressureStatus"));
            if (padData->hasAttribute("sequencerMidiCcController"))
                PAD_SETTINGS->setSequencerMidiCcController(padData->getIntAttribute("sequencerMidiCcController"));
        }
        
        //sequencer samples mode
        else if (padData->getIntAttribute("sequencerMode") == 2)
        {
            for (int row = 0; row <= NO_OF_ROWS-1; row++)
            {
                File newFile;
                String newFileString(String::empty);
                
                if (padData->hasAttribute("sequencerSamplesAudioFilePath"+String(row)))
                {
                    newFileString = padData->getStringAttribute("sequencerSamplesAudioFilePath"+String(row));
                    
                    if (newFileString != String::empty) //to prevent trying to load in a file if there's nothing to load
                    {
                        if (File::isAbsolutePath(newFileString) == false)
                        {
                            //check if the saved audio file path is just the file name
                            //if so, get it from the working directory and apply the full pathname to it
                            newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + newFileString;
                        }
                        else
                        {
                            //else, it should be the full path name already
                            newFile = newFileString;
                        }
                        
                        PAD_SETTINGS->setSequencerSamplesAudioFilePath(newFile, row);
                    }
                    else
                        PAD_SETTINGS->setSequencerSamplesAudioFilePath(File::nonexistent, row);
                }
            }
            
            if (padData->hasAttribute("sequencerEffect"))
                PAD_SETTINGS->setSequencerEffect(padData->getIntAttribute("sequencerEffect"));
            if (padData->hasAttribute("sequencerPan"))
                PAD_SETTINGS->setSequencerPan(padData->getDoubleAttribute("sequencerPan"));
            if (padData->hasAttribute("sequencerGain"))
                PAD_SETTINGS->setSequencerGain(padData->getDoubleAttribute("sequencerGain"));
            if (padData->hasAttribute("sequencerSamplesAttack"))
                 PAD_SETTINGS->setSequencerSamplesAttackTime(padData->getDoubleAttribute("sequencerSamplesAttack"));
        }
    }
    
    //controller mode
    else if (PAD_SETTINGS->getMode() == 4)
    {
        if (padData->hasAttribute("controllerControl"))
            PAD_SETTINGS->setControllerControl(padData->getIntAttribute("controllerControl"));
        if (padData->hasAttribute("controllerSceneNumber"))
            PAD_SETTINGS->setControllerSceneNumber(padData->getIntAttribute("controllerSceneNumber"));
        
        if (padData->hasAttribute("controllerOscIpAddress"))
            PAD_SETTINGS->setControllerOscIpAddress(padData->getStringAttribute("controllerOscIpAddress"));
        if (padData->hasAttribute("controllerOscPortNumber"))
            PAD_SETTINGS->setControllerOscPort(padData->getIntAttribute("controllerOscPortNumber"));
        if (padData->hasAttribute("controllerMidiProgramChangeNumber"))
            PAD_SETTINGS->setControllerMidiProgramChangeNumber(padData->getIntAttribute("controllerMidiProgramChangeNumber"));
        if (padData->hasAttribute("controllerMidiProgramChangeChannel"))
            PAD_SETTINGS->setControllerMidiProgramChangeChannel(padData->getIntAttribute("controllerMidiProgramChangeChannel"));
    }
    
    int modeCheck = PAD_SETTINGS->getMode();
    int effect = 0;
    if (modeCheck == 2)
    {
        if (padData->hasAttribute("effect")) //if padData is from an effects preset file
            PAD_SETTINGS->setSamplerEffect(padData->getIntAttribute("effect"));
    
        effect = PAD_SETTINGS->getSamplerEffect();
    }
    else if (modeCheck == 3)
    {
        if (padData->hasAttribute("effect"))
            PAD_SETTINGS->setSequencerEffect(padData->getIntAttribute("effect"));
        
        if (PAD_SETTINGS->getSequencerMode() == 2)
            effect = PAD_SETTINGS->getSequencerEffect();
    }
    
    if (padData->hasAttribute("samplerEffect") || 
        padData->hasAttribute("sequencerEffect") || 
        padData->hasAttribute("effect"))
    {
        if (effect == 1) //Gain and Pan
        {
            PAD_SETTINGS->setPadFxGainPanGain(padData->getDoubleAttribute("padFxGainPanGain"));
            PAD_SETTINGS->setPadFxGainPanPan(padData->getDoubleAttribute("padFxGainPanPan"));
            PAD_SETTINGS->setPadFxGainPanAlphaTouch(padData->getIntAttribute("padFxGainPanAlphaTouch"));
            PAD_SETTINGS->setPadFxGainPanAtReverse(padData->getIntAttribute("padFxGainPanAtReverse"));
            PAD_SETTINGS->setPadFxGainPanAtIntensity(padData->getDoubleAttribute("padFxGainPanAtIntensity"));
        }
        else if (effect == 2) //LPF
        {
            PAD_SETTINGS->setPadFxLpfMix(padData->getDoubleAttribute("padFxLpfMix"));
            PAD_SETTINGS->setPadFxLpfFreq(padData->getDoubleAttribute("padFxLpfFreq"));
            PAD_SETTINGS->setPadFxLpfBandwidth(padData->getDoubleAttribute("padFxLpfBandwidth"));
            PAD_SETTINGS->setPadFxLpfAlphaTouch(padData->getIntAttribute("padFxLpfAlphaTouch"));
            PAD_SETTINGS->setPadFxLpfAtReverse(padData->getIntAttribute("padFxLpfAtReverse"));
            PAD_SETTINGS->setPadFxLpfAtIntensity(padData->getDoubleAttribute("padFxLpfAtIntensity"));
            
        }
        else if (effect == 3) //HPF
        {
            PAD_SETTINGS->setPadFxHpfMix(padData->getDoubleAttribute("padFxHpfMix"));
            PAD_SETTINGS->setPadFxHpfFreq(padData->getDoubleAttribute("padFxHpfFreq"));
            PAD_SETTINGS->setPadFxHpfBandwidth(padData->getDoubleAttribute("padFxHpfBandwidth"));
            PAD_SETTINGS->setPadFxHpfAlphaTouch(padData->getIntAttribute("padFxHpfAlphaTouch"));
            PAD_SETTINGS->setPadFxHpfAtReverse(padData->getIntAttribute("padFxHpfAtReverse"));
            PAD_SETTINGS->setPadFxHpfAtIntensity(padData->getDoubleAttribute("padFxHpfAtIntensity"));
        }
		else if (effect == 4) //BPF
        {
            PAD_SETTINGS->setPadFxBpfMix(padData->getDoubleAttribute("padFxBpfMix"));
            PAD_SETTINGS->setPadFxBpfFreq(padData->getDoubleAttribute("padFxBpfFreq"));
            PAD_SETTINGS->setPadFxBpfBandwidth(padData->getDoubleAttribute("padFxBpfBandwidth"));
            PAD_SETTINGS->setPadFxBpfAlphaTouch(padData->getIntAttribute("padFxBpfAlphaTouch"));
            PAD_SETTINGS->setPadFxBpfAtReverse(padData->getIntAttribute("padFxBpfAtReverse"));
            PAD_SETTINGS->setPadFxBpfAtIntensity(padData->getDoubleAttribute("padFxBpfAtIntensity"));
        }
        else if (effect == 5) //Distortion
        {
            PAD_SETTINGS->setPadFxDistortionInputGain(padData->getDoubleAttribute("padFxDistortionInputGain"));
			PAD_SETTINGS->setPadFxDistortionDrive(padData->getDoubleAttribute("padFxDistortionDrive"));
			PAD_SETTINGS->setPadFxDistortionWetDryMix(padData->getDoubleAttribute("padFxDistortionWetDryMix"));
			PAD_SETTINGS->setPadFxDistortionTone(padData->getDoubleAttribute("padFxDistortionTone"));
			PAD_SETTINGS->setPadFxDistortionTypeMenu(padData->getIntAttribute("padFxDistortionTypeMenu"));
            PAD_SETTINGS->setPadFxDistortionAlphaTouch(padData->getIntAttribute("padFxDistortionAlphaTouch"));
            PAD_SETTINGS->setPadFxDistortionAtReverse(padData->getIntAttribute("padFxDistortionAtReverse"));
            PAD_SETTINGS->setPadFxDistortionAtIntensity(padData->getDoubleAttribute("padFxDistortionAtIntensity"));
        }
		else if (effect == 6) //Bitcrusher
        {
            PAD_SETTINGS->setPadFxBitcrusherInputGain(padData->getDoubleAttribute("padFxBitcrusherInputGain"));
			PAD_SETTINGS->setPadFxBitcrusherDownsample(padData->getDoubleAttribute("padFxBitcrusherDownsample"));
			PAD_SETTINGS->setPadFxBitcrusherCrush(padData->getDoubleAttribute("padFxBitcrusherCrush"));
			PAD_SETTINGS->setPadFxBitcrusherSmoothing(padData->getDoubleAttribute("padFxBitcrusherSmoothing"));
			PAD_SETTINGS->setPadFxBitcrusherWetDryMix(padData->getDoubleAttribute("padFxBitcrusherWetDryMix"));
            PAD_SETTINGS->setPadFxBitcrusherAlphaTouch(padData->getIntAttribute("padFxBitcrusherAlphaTouch"));
            PAD_SETTINGS->setPadFxBitcrusherAtReverse(padData->getIntAttribute("padFxBitcrusherAtReverse"));
            PAD_SETTINGS->setPadFxBitcrusherAtIntensity(padData->getDoubleAttribute("padFxBitcrusherAtIntensity"));
        }
        else if (effect == 7) //Delay
        {
            PAD_SETTINGS->setPadFxDelayMix(padData->getDoubleAttribute("padFxDelayMix"));
            PAD_SETTINGS->setPadFxDelayTime(padData->getDoubleAttribute("padFxDelayTime"));
            PAD_SETTINGS->setPadFxDelayFeedback(padData->getDoubleAttribute("padFxDelayFeedback"));
            PAD_SETTINGS->setPadFxDelayLpfFreq(padData->getDoubleAttribute("padFxDelayLpfFreq"));
            PAD_SETTINGS->setPadFxDelayHpfFreq(padData->getDoubleAttribute("padFxDelayHpfFreq"));
            PAD_SETTINGS->setPadFxDelaySync(padData->getIntAttribute("padFxDelaySync"));
            PAD_SETTINGS->setPadFxDelayTimeMenu(padData->getIntAttribute("padFxDelayTimeMenu"));
            PAD_SETTINGS->setPadFxDelayAlphaTouch(padData->getIntAttribute("padFxDelayAlphaTouch"));
            PAD_SETTINGS->setPadFxDelayAtReverse(padData->getIntAttribute("padFxDelayAtReverse"));
            PAD_SETTINGS->setPadFxDelayAtIntensity(padData->getDoubleAttribute("padFxDelayAtIntensity"));
        }
        else if (effect == 8) //Reverb
        {
            PAD_SETTINGS->setPadFxReverbMix(padData->getDoubleAttribute("padFxReverbMix"));
            PAD_SETTINGS->setPadFxReverbRoomSize(padData->getDoubleAttribute("padFxReverbRoomSize"));
            PAD_SETTINGS->setPadFxReverbDamping(padData->getDoubleAttribute("padFxReverbDamping"));
            PAD_SETTINGS->setPadFxReverbWidth(padData->getDoubleAttribute("padFxReverbWidth"));
            PAD_SETTINGS->setPadFxReverbFreezeMode(padData->getDoubleAttribute("padFxReverbFreezeMode"));
            PAD_SETTINGS->setPadFxReverbAlphaTouch(padData->getIntAttribute("padFxReverbAlphaTouch"));
            PAD_SETTINGS->setPadFxReverbAtReverse(padData->getIntAttribute("padFxReverbAtReverse"));
            PAD_SETTINGS->setPadFxReverbAtIntensity(padData->getDoubleAttribute("padFxReverbAtIntensity"));
        }
        else if (effect == 9) //Flanger
        {
            PAD_SETTINGS->setPadFxFlangerMix(padData->getDoubleAttribute("padFxFlangerMix"));
            PAD_SETTINGS->setPadFxFlangerRate(padData->getDoubleAttribute("padFxFlangerRate"));
            PAD_SETTINGS->setPadFxFlangerFeedback(padData->getDoubleAttribute("padFxFlangerFeedback"));
            PAD_SETTINGS->setPadFxFlangerIntensity(padData->getDoubleAttribute("padFxFlangerIntensity"));
            PAD_SETTINGS->setPadFxFlangerSync(padData->getIntAttribute("padFxFlangerSync"));
            PAD_SETTINGS->setPadFxFlangerRateMenu(padData->getIntAttribute("padFxFlangerRateMenu"));
            PAD_SETTINGS->setPadFxFlangerAlphaTouch(padData->getIntAttribute("padFxFlangerAlphaTouch"));
            PAD_SETTINGS->setPadFxFlangerAtReverse(padData->getIntAttribute("padFxFlangerAtReverse"));
            PAD_SETTINGS->setPadFxFlangerAtIntensity(padData->getDoubleAttribute("padFxFlangerAtIntensity"));
        }
        
        else if (effect == 10) //Tremolo
        {
            PAD_SETTINGS->setPadFxTremoloDepth(padData->getDoubleAttribute("padFxTremoloDepth"));
            PAD_SETTINGS->setPadFxTremoloRate(padData->getDoubleAttribute("padFxTremoloRate"));
            PAD_SETTINGS->setPadFxTremoloShape(padData->getIntAttribute("padFxTremoloShape"));
            PAD_SETTINGS->setPadFxTremoloSync(padData->getIntAttribute("padFxTremoloSync"));
            PAD_SETTINGS->setPadFxTremoloRateMenu(padData->getIntAttribute("padFxTremoloRateMenu"));
            PAD_SETTINGS->setPadFxTremoloAlphaTouch(padData->getIntAttribute("padFxTremoloAlphaTouch"));
            PAD_SETTINGS->setPadFxTremoloAtReverse(padData->getIntAttribute("padFxTremoloAtReverse"));
            PAD_SETTINGS->setPadFxTremoloAtIntensity(padData->getDoubleAttribute("padFxTremoloAtIntensity"));
        }
    }
}






void AppDocumentState::saveProjectSettings()
{
    //reset/clear XmlElement.
    if (projectData != nullptr) //if there is data in the scene's XmlElement
        projectData->removeAllAttributes();
    
    projectData->setAttribute("copyExternalFiles", AppSettings::Instance()->getCopyExternalFiles());
    
}

void AppDocumentState::loadProjectSettings()
{
    if (projectData->hasAttribute("copyExternalFiles") == true)
        AppSettings::Instance()->setCopyExternalFiles(projectData->getIntAttribute("copyExternalFiles"));
    else
        AppSettings::Instance()->setCopyExternalFiles(true); //default value
    
}







void AppDocumentState::saveToScene (int sceneNumber)
{
    //reset/clear XmlElement.
    clearScene(sceneNumber);
    
    //===global settings===
    XmlElement *globalData  = new XmlElement ("GLOBAL_DATA");
    globalData->setAttribute("globalGain", AppSettings::Instance()->getGlobalGain());
    globalData->setAttribute("globalPan", AppSettings::Instance()->getGlobalPan());
    
    globalData->setAttribute("globalTempo", AppSettings::Instance()->getGlobalTempo());
    globalData->setAttribute("quantizationValue", AppSettings::Instance()->getQuantizationValue());
    globalData->setAttribute("beatsPerBar", AppSettings::Instance()->getBeatsPerBar());
    globalData->setAttribute("autoStartClock", AppSettings::Instance()->getAutoStartClock());
    
    //==========elite dials=============
    for (int i = 0; i < 2; i++)
    {
        globalData->setAttribute("eliteDialPrevValue" + String(i), AppSettings::Instance()->getEliteDialPrevValue(i));
        globalData->setAttribute("eliteDialControl" + String(i), AppSettings::Instance()->getEliteDialControl(i));
        globalData->setAttribute("eliteDialStatus" + String(i), AppSettings::Instance()->getEliteDialStatus(i));
        
        if (AppSettings::Instance()->getEliteDialControl(i) == 4) //midi cc
        {
            globalData->setAttribute("eliteDialMidiControllerType" + String(i), AppSettings::Instance()->getEliteDialMidiControllerType(i));
            globalData->setAttribute("eliteDialMidiCcNumber" + String(i), AppSettings::Instance()->getEliteDialMidiCcNumber(i));
            globalData->setAttribute("eliteDialMidiChannel" + String(i), AppSettings::Instance()->getEliteDialMidiChannel(i));
            globalData->setAttribute("eliteDialMidiMinRange" + String(i), AppSettings::Instance()->getEliteDialMidiMinRange(i));
            globalData->setAttribute("eliteDialMidiMaxRange" + String(i), AppSettings::Instance()->getEliteDialMidiMaxRange(i));
        }
        else if (AppSettings::Instance()->getEliteDialControl(i) == 5) //osc
        {
            globalData->setAttribute("eliteDialOscPortNumber" + String(i), AppSettings::Instance()->getEliteDialOscPortNumber(i));
            globalData->setAttribute("eliteDialOscMinRange" + String(i), AppSettings::Instance()->getEliteDialOscMinRange(i));
            globalData->setAttribute("eliteDialOscMaxRange" + String(i), AppSettings::Instance()->getEliteDialOscMaxRange(i));
            globalData->setAttribute("eliteDialOscIpAddress" + String(i), AppSettings::Instance()->getEliteDialOscIpAddress(i));
            globalData->setAttribute("eliteDialOscStepValue" + String(i), AppSettings::Instance()->getEliteDialOscStepValue(i));
        }
    }
    //==========elite buttons=============
    for (int i = 0; i < 3; i++)
    {
        globalData->setAttribute("eliteButtonControl" + String(i), AppSettings::Instance()->getEliteButtonControl(i));
        globalData->setAttribute("eliteButtonStatus" + String(i), AppSettings::Instance()->getEliteButtonStatus(i));
        
        if (AppSettings::Instance()->getEliteButtonControl(i) == 2) //scene switcher
        {
            globalData->setAttribute("eliteButtonSceneNumber" + String(i), AppSettings::Instance()->getEliteButtonSceneNumber(i));
        }
        else if (AppSettings::Instance()->getEliteButtonControl(i) == 4) //midi cc
        {
            globalData->setAttribute("eliteButtonMidiCcNumber" + String(i), AppSettings::Instance()->getEliteButtonMidiCcNumber(i));
            globalData->setAttribute("eliteButtonMidiChannel" + String(i), AppSettings::Instance()->getEliteButtonMidiChannel(i));
            globalData->setAttribute("eliteButtonMidiOffNumber" + String(i), AppSettings::Instance()->getEliteButtonMidiOffNumber(i));
            globalData->setAttribute("eliteButtonMidiOnNumber" + String(i), AppSettings::Instance()->getEliteButtonMidiOnNumber(i));
        }
        else if (AppSettings::Instance()->getEliteButtonControl(i) == 5) //osc
        {
            globalData->setAttribute("eliteButtonOscPortNumber" + String(i), AppSettings::Instance()->getEliteButtonOscPortNumber(i));
            globalData->setAttribute("eliteButtonOscOffNumber" + String(i), AppSettings::Instance()->getEliteButtonOscOffNumber(i));
            globalData->setAttribute("eliteButtonOscOnNumber" + String(i), AppSettings::Instance()->getEliteButtonOscOnNumber(i));
            globalData->setAttribute("eliteButtonOscIpAddress" + String(i), AppSettings::Instance()->getEliteButtonOscIpAddress(i));
        }
    }
    
    //===pad settings (pad number = i)
    for (int i = 0; i <= 47; i++)
    {
        //create a 'temp' XmlElement to store data for a single pad
        XmlElement *padData  = new XmlElement ("PAD_DATA_" + String(i));
        
        savePadSettings (i, padData);
        
        //add temp xmlElement as a child element of the main scene XmlElement
        sceneData[sceneNumber]->addChildElement(new XmlElement(*padData));
        
        delete padData; 
        
    }
    
    //add globalData as child element.
    //WHY DO I HAVE TO DO THIS HERE AND NOT BEFORE DOING THE PADDATA STUFF?
    //caused hours of confusion and crashing if this was done first
    //UPDATE 29/8/12 - does the above comment still apply now?
    sceneData[sceneNumber]->addChildElement(new XmlElement(*globalData));
    
    delete globalData;
}




void AppDocumentState::loadFromScene (int sceneNumber)
{
    
    if (sceneData[sceneNumber] != nullptr && sceneData[sceneNumber]->hasTagName("SCENE_"+String(sceneNumber)))
    {
        //===global settings===
        XmlElement *globalData = new XmlElement(*sceneData[sceneNumber]->getChildByName("GLOBAL_DATA"));
        
        AppSettings::Instance()->setGlobalGain(globalData->getDoubleAttribute("globalGain"));
        AppSettings::Instance()->setGlobalPan(globalData->getDoubleAttribute("globalPan"));
        
        AppSettings::Instance()->setGlobalTempo(globalData->getDoubleAttribute("globalTempo"));
        AppSettings::Instance()->setQuantizationValue(globalData->getIntAttribute("quantizationValue"));
        AppSettings::Instance()->setBeatsPerBar(globalData->getIntAttribute("beatsPerBar"));
        AppSettings::Instance()->setAutoStartClock(globalData->getIntAttribute("autoStartClock"));
        
        //==========elite dials=============
        for (int i = 0; i < 2; i++)
        {
            AppSettings::Instance()->setEliteDialPrevValue(globalData->getDoubleAttribute("eliteDialPrevValue" + String(i)), i);
            AppSettings::Instance()->setEliteDialControl(globalData->getIntAttribute("eliteDialControl" + String(i)), i);
            AppSettings::Instance()->setEliteDialStatus(globalData->getIntAttribute("eliteDialStatus" + String(i)), i);
            
            if (AppSettings::Instance()->getEliteDialControl(i) == 4) //midi cc
            {
                AppSettings::Instance()->setEliteDialMidiControllerType(globalData->getIntAttribute("eliteDialMidiControllerType" + String(i)), i);
                AppSettings::Instance()->setEliteDialMidiCcNumber(globalData->getIntAttribute("eliteDialMidiCcNumber" + String(i)), i);
                AppSettings::Instance()->setEliteDialMidiChannel(globalData->getIntAttribute("eliteDialMidiChannel" + String(i)), i);
                AppSettings::Instance()->setEliteDialMidiMinRange(globalData->getIntAttribute("eliteDialMidiMinRange" + String(i)), i);
                AppSettings::Instance()->setEliteDialMidiMaxRange(globalData->getIntAttribute("eliteDialMidiMaxRange" + String(i)), i);
            }
            else if (AppSettings::Instance()->getEliteDialControl(i) == 5) //osc
            {
                AppSettings::Instance()->setEliteDialOscPortNumber(globalData->getIntAttribute("eliteDialOscPortNumber" + String(i)), i);
                AppSettings::Instance()->setEliteDialOscMinRange(globalData->getDoubleAttribute("eliteDialOscMinRange" + String(i)), i);
                AppSettings::Instance()->setEliteDialOscMaxRange(globalData->getDoubleAttribute("eliteDialOscMaxRange" + String(i)), i);
                AppSettings::Instance()->setEliteDialOscIpAddress(globalData->getStringAttribute("eliteDialOscIpAddress" + String(i)), i);
                AppSettings::Instance()->setEliteDialOscStepValue(globalData->getDoubleAttribute("eliteDialOscStepValue" + String(i)), i);
            }
        }
        //==========elite buttons=============
        for (int i = 0; i < 3; i++)
        {
            AppSettings::Instance()->setEliteButtonControl(globalData->getIntAttribute("eliteButtonControl" + String(i)), i);
            AppSettings::Instance()->setEliteButtonStatus(globalData->getIntAttribute("eliteButtonStatus" + String(i)), i);
            
            if (AppSettings::Instance()->getEliteButtonControl(i) == 2) //scene switcher
            {
                AppSettings::Instance()->setEliteButtonSceneNumber(globalData->getIntAttribute("eliteButtonSceneNumber" + String(i)), i);
            }
            else if (AppSettings::Instance()->getEliteButtonControl(i) == 4) //midi cc
            {
                AppSettings::Instance()->setEliteButtonMidiCcNumber(globalData->getIntAttribute("eliteButtonMidiCcNumber" + String(i)), i);
                AppSettings::Instance()->setEliteButtonMidiChannel(globalData->getIntAttribute("eliteButtonMidiChannel" + String(i)), i);
                AppSettings::Instance()->setEliteButtonMidiOffNumber(globalData->getIntAttribute("eliteButtonMidiOffNumber" + String(i)), i);
                AppSettings::Instance()->setEliteButtonMidiOnNumber(globalData->getIntAttribute("eliteButtonMidiOnNumber" + String(i)), i);
            }
            else if (AppSettings::Instance()->getEliteButtonControl(i) == 5) //osc
            {
                AppSettings::Instance()->setEliteButtonOscPortNumber(globalData->getIntAttribute("eliteButtonOscPortNumber" + String(i)), i);
                AppSettings::Instance()->setEliteButtonOscOffNumber(globalData->getDoubleAttribute("eliteButtonOscOffNumber" + String(i)), i);
                AppSettings::Instance()->setEliteButtonOscOnNumber(globalData->getDoubleAttribute("eliteButtonOscOnNumber" + String(i)), i);
                AppSettings::Instance()->setEliteButtonOscIpAddress(globalData->getStringAttribute("eliteButtonOscIpAddress" + String(i)), i);
            }
        }
        
        delete globalData;
        
        //===pad settings (pad number = i)
        for (int i = 0; i <= 47; i++)
        {
            //creates a deep copy of sceneData, not just a copy of the pointer
            XmlElement *padData = new XmlElement(*sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))); 
            
            loadPadSettings(i, padData);
            
            delete padData;
        }
        
    }
    
    //update GUI - calls update() in mainComponent
    notifyObs();
}


void AppDocumentState::clearScene (int sceneNumber)
{
    
    //reset/clear XmlElement.
    if (sceneData[sceneNumber] != nullptr) //if there is data in the scene's XmlElement
    {
        sceneData[sceneNumber]->deleteAllChildElements();
    }
}









void AppDocumentState::createNewProject()
{
    //========== ask user if they would like to save the current project first =========
    int modeCheck = 0; //don't show 'save?' alert
    int shouldSave = 2; //don't save
    
    //check to see if the user might want to save anything first by looking for a pad
    //with a mode set to it
    //IDEALLY WE NEED A BETTER METHOD OF CHECKING WHETHER THE USER MIGHT WANT TO SAVE
    for (int i = 0; i <= 47; i++)
    {
        modeCheck = PAD_SETTINGS->getMode();
        if (modeCheck > 0)
            break;
    }
    
    //if found a pad with a mode set to it, ask if user wants to save first
    if (modeCheck != 0)
    {
        shouldSave = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon, translate("Create New Project"), translate("Would you like to save the current project first?"));
    }
    
    //if the user didn't press cancel on the alertwindow ('cancel load command')
    if (shouldSave != 0) 
    {
        
        if (shouldSave == 1) //'yes'
        {
            shouldDisplayAlertWindow = false;
            saveProject();
        }
        
        
        //============= reset all settings =================
        
        AppSettings::Instance()->resetData();
        
        for (int i = 0; i <= 47; i++)
            PAD_SETTINGS->resetData(0);
        
        currentProjectFile = File::nonexistent;
        
        File::getSpecialLocation(File::tempDirectory).deleteRecursively();
        File::getSpecialLocation(File::tempDirectory).setAsCurrentWorkingDirectory();
        numOfFilesAtStart = 0;
        
        //========= clear all XmlElement objects and update the sceneComponent display ===========
        
        //let the observer know it will need to update the sceneComponent GUI next time it is 'notified'
        guiUpdateFlag = 1;
        
        projectData->removeAllAttributes();
        
        for (int i = 0; i <= NO_OF_SCENES-1; i++)
        {
            //accessed by observer in order to update the relevent scene slot's GUI
            sceneToUpdate = i;
            
            //reset the scene data by saving the default settings to the scene
            saveToScene(i);
            
            //display GUI scene slot as empty
            sceneStatus = 0;
        
            //set the first scene to be display as 'selected'
            if (i == 0)
            {
                sceneStatus = 2;
                setCurrentlySelectedScene(0);
            }
            
            //update display
            notifyObs();
        }
        
        //let the observer know it will need to update the settings display next time it is 'notified'
        guiUpdateFlag = 0;
        
        //save the reset settings, then call update the main display
        saveToScene(0);
        notifyObs();
        
        //change the window title bar text
        mainAppWindowRef->setTitleBarText(translate("untitled"));
        
    }
}


void AppDocumentState::saveProject()
{
    if (currentProjectFile == File::nonexistent) //if no performance has been saved yet
    {
        saveProjectAs();
    }
    
    else //replace currentProjectFile
    {
        //first, need to save the current project and scene settings
        saveProjectSettings();
        saveToScene(currentlySelectedScene);
        
        currentProjectFile.deleteFile();
        currentProjectFile.create(); //create the file

        XmlElement performanceSettings("ALPHALIVE_PROJECT_VERSION_1");

        performanceSettings.addChildElement(projectData);
        
        for (int i = 0; i <= NO_OF_SCENES-1; i++)
        {
            performanceSettings.addChildElement(sceneData[i]);
        }
        
        String xmlDoc = performanceSettings.createDocument(String::empty, false);
        currentProjectFile.appendText(xmlDoc);
        
        //remove projectData and sceneData child elements from performanceSettings so that they aren't deleted when
        //performanceSettings goes out of scope.
        
        performanceSettings.removeChildElement(projectData, false);
        
        for (int i = 0; i <= NO_OF_SCENES-1; i++)
        {
            performanceSettings.removeChildElement(sceneData[i], false);
        }
        
        /*
        if (shouldDisplayAlertWindow == true)
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Project Saved", "The project settings have been successfully saved to file");
        }
        shouldDisplayAlertWindow = true;
         */
        
        
        //add the file to the 'recent files' list
        registerRecentFile (currentProjectFile);
        
    }
}


void AppDocumentState::saveProjectAs()
{
    //navigate to app directory
    FileChooser saveFileChooser(translate("Create a AlphaLive project to save..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphalive");
    
    if (saveFileChooser.browseForFileToSave(false))
    {
        //create a project directory
        File savedDirectory (saveFileChooser.getResult());
        
        //create folder to hold the projects audio files (if it doesn't already exist, which it shouldnt (?))
        File audioFileDirectory = (savedDirectory.getFullPathName() + File::separatorString + "Audio Files");
        
        if (AppSettings::Instance()->getCopyExternalFiles() == true)
        {
            //copy current working directory to the audio files directory
            File::getCurrentWorkingDirectory().copyDirectoryTo(audioFileDirectory);
        }
        else
            audioFileDirectory.createDirectory();  
        //set the audio files directory as the new working directory so when audio files are imported they go straight into here
        audioFileDirectory.setAsCurrentWorkingDirectory();
        
        //create file
        File savedFile (savedDirectory.getFullPathName() + File::separatorString + savedDirectory.getFileName()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".alphalive"; //append an extension name to the filepath name
        savedFile = (stringFile); //set the file to this name
        
        bool overwrite = true; //by default true
        
        //how do i check for overriding here?
        
        //delete the file if it exists & write the new data
        if (savedFile.exists())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("This File Already Exists!"), translate("Are you sure you want to overwrite this file?"));
        }
        
        if (overwrite == true)
        {
            //first, need to save the current project and scene settings
            saveToScene(currentlySelectedScene);
            saveProjectSettings();
            
            savedDirectory.createDirectory();
            savedFile.deleteFile();
            savedFile.create(); //create the file
    
            XmlElement performanceSettings("ALPHALIVE_PROJECT_VERSION_1");
            
            performanceSettings.addChildElement(projectData);
            
            for (int i = 0; i <= NO_OF_SCENES-1; i++)
            {
                performanceSettings.addChildElement(sceneData[i]);
            }
            
            String xmlDoc = performanceSettings.createDocument(String::empty, false);
            savedFile.appendText(xmlDoc);
            
            //remove projectData and sceneData child elements from performanceSettings so that they aren't deleted when
            //performanceSettings goes out of scope.
            
            performanceSettings.removeChildElement(projectData, false);
            
            for (int i = 0; i <= NO_OF_SCENES-1; i++)
            {
                performanceSettings.removeChildElement(sceneData[i], false);
            }
            
            //set the current open document to the document just saved
            currentProjectFile = savedFile;
            //change the window title bar text
            mainAppWindowRef->setTitleBarText(currentProjectFile.getFileNameWithoutExtension());
            
            /*
            if (shouldDisplayAlertWindow == true)
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Project Saved", "The project settings have been successfully saved to file");
            shouldDisplayAlertWindow = true;
             */
            
            //add the file to the 'recent files' list
            registerRecentFile (currentProjectFile);
            
        }
        
    }
    
}

void AppDocumentState::loadProject (bool openBrowser, File fileToOpen)
{
    //openBrower will be true when the 'Load' button is clicked, and false when a .alphalive file is clicked
    //fileToOpen will be equal to File::nonexistent when Load is click, and the file path when a .alphalive
    //file is clicked
    
    //========== NEW: ask user if they would like to save the current project first =========
    int modeCheck = 0; //don't show 'save?' alert
    int shouldSave = 2; //don't save
    
    //check to see if the user might want to save anything first by looking for a pad
    //with a mode set to it
    //IDEALLY WE NEED A BETTER METHOD OF CHECKING WHETHER THE USER MIGHT WANT TO SAVE
    for (int i = 0; i <= 47; i++)
    {
        modeCheck = PAD_SETTINGS->getMode();
        if (modeCheck > 0)
            break;
    }
    
    //if found a pad with a mode set to it, ask if user wants to save first
    if (modeCheck != 0)
    {
        shouldSave = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon, translate("Load New Project"), translate("Would you like to save the current project first?"));
    }
    
    //if the user didn't press cancel on the alertwindow ('cancel load command')
    if (shouldSave != 0) 
    {
        
        if (shouldSave == 1) //'yes'
        {
            shouldDisplayAlertWindow = false;
            saveProject();
        }
        
        // ========================== 'LOAD PROJECT' CODE ==================================
        
        //navigate to app directory
        FileChooser loadFileChooser(translate("Select a .alphalive file to open..."), 
                                    StoredSettings::getInstance()->appProjectDir, 
                                    "*.alphalive");
        
        bool shouldLoad;
        
        if (openBrowser == true)
            shouldLoad = loadFileChooser.browseForFileToOpen(); //open file browser
        
        if (shouldLoad || openBrowser == false)
        {
            //let the observer know it will need to update the sceneComponent GUI next time it is 'notified'
            guiUpdateFlag = 1;
            
            //get file
            File loadedFile;
            if (openBrowser == true)
                loadedFile = loadFileChooser.getResult();
            else
                loadedFile = fileToOpen;
            
            //parse file into xml file
            ScopedPointer<XmlElement> loadedXml (XmlDocument::parse(loadedFile));
            
            
            if (loadedXml != nullptr && loadedXml->hasTagName("ALPHALIVE_PROJECT_VERSION_1"))
            {
                
                //get the folder that holds the projects audio files
                File audioFileDirectory = (loadedFile.getParentDirectory().getFullPathName() + File::separatorString + "Audio Files");
                
                //if so some strange reason the directory doesn't exist, create it
                if (audioFileDirectory.exists() == false)
                    audioFileDirectory.createDirectory();
                
                //set the Audio Files directory as the new working directory so loaded audio files can be found
                audioFileDirectory.setAsCurrentWorkingDirectory();
                
                //get number of included audio files (for use at close when auto cleaning project)
                numOfFilesAtStart = File::getCurrentWorkingDirectory().getNumberOfChildFiles(2);
                
                //=====================load projectData settings=======================
                
                //reset/clear XmlElement.
                if (projectData != nullptr)
                    projectData->removeAllAttributes();
                
                //put the loaded xml data into the xmlelement for the project settings
                XmlElement *projSettingsXml = loadedXml->getChildByName("PROJECT_SETTINGS");
                
                //check to see if the project settings child element actually exists (it won't within older AlphaLive Projects)
                if (loadedXml->containsChildElement(projSettingsXml) == true)
                    projectData = new XmlElement(*projSettingsXml);
                
                //apply the settings to AppSettings variables, even if "PROJECT_SETTINGS" doesn't exist (old file format), as default values should then be set
                loadProjectSettings();
                
                
                //WHY DON'T I NEED TO delete or remove projSettingsXml here?
                //I think it's because above I created a hard copy of projSettingsXml when applying it to projectData,
                //therefore when loadedXml goes out of scope and delete's projSettingsXml, it doesn't effect projectData
                //in anyway.
                
                //=========load the child elements of loadedXml and put them in the sceneData objects===========
                for (int scene = 0; scene <= NO_OF_SCENES-1; scene++)
                {
                    //accessed by observer in order to update the relevent scene slot's GUI
                    sceneToUpdate = scene;
                    
                    //clear the xmlelement for the current scene number
                    clearScene(scene);
                    
                    //put the loaded xml data into the xmlelement for the current scene
                    XmlElement* childToInsert = loadedXml->getChildByName("SCENE_" + String(scene));
                    sceneData.insert (scene, childToInsert);
                    //remove sceneData childelement from loadedXml so it isn't deleted when loadedXml goes out of scope!
                    loadedXml->removeChildElement (childToInsert, false);
                    
                    //determine the status of the scene
                    for (int i = 0; i < 48; i++)
                    {
                        sceneStatus = 0;
                        
                        if (sceneData[scene]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") != 0)
                        {
                            sceneStatus = 1;
                            break;
                        }
                    }
                     
                    //set the first scene to be display as 'selected'
                    if (scene == 0)
                    {
                        sceneStatus = 2;
                        setCurrentlySelectedScene(0);
                    }
                    
                    //update display
                    notifyObs();
                }
                
                //let the observer know it will need to update the settings display next time it is 'notified'
                guiUpdateFlag = 0;
                
                //call loadFromScene to load the settings of scene 0 into the application
                loadFromScene(0);
                
                currentProjectFile = loadedFile;
                
                //=====================================================
                //==============NEW - reset unused mode settings=======
                //=====================================================
                /*
                 Here, the settings of the modes that aren't being used for each pad are reset to their default values.
                 */
                for (int i = 0; i <=47; i++)
                {
                    PAD_SETTINGS->resetData(PAD_SETTINGS->getMode());
                }
                //=====================================================
                
                //change the window title bar text
                mainAppWindowRef->setTitleBarText(currentProjectFile.getFileNameWithoutExtension());
                
                //add the file to the 'recent files' list
                registerRecentFile (currentProjectFile);
            }
            else if (loadedXml != nullptr && loadedXml->hasTagName("PERFORMANCE"))
            {
               AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, translate("Cannot Open File"), translate("The selected AlphaLive Project file cannot be opened with this version of AlphaLive."));  
            }
            else
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, translate("Cannot Open File"), translate("The selected AlphaLive Project file seems to be corrupt."));
            }
            
        }
        
    }
     
}



void AppDocumentState::saveSceneToDisk (int sceneNumber)
{
    saveToScene(sceneNumber);
    
    //navigate to app directory
    FileChooser saveFileChooser(translate("Create an AlphaLive Scene file to save..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphascene");
    if (saveFileChooser.browseForFileToSave(false))
    {
        //create a project directory
        File savedDirectory (saveFileChooser.getResult());
        File savedDirectoryName = savedDirectory;
        String directoryString = savedDirectory.getFullPathName() + " (SCENE)";
        savedDirectory = directoryString;
        
        File savedFile (savedDirectory.getFullPathName() + File::separatorString + savedDirectoryName.getFileName()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".alphascene"; //append an extension name to the filepath name
        savedFile = stringFile; //set the file to this name
        
        bool overwrite = true; //by default true
        
        //delete the file if it exists &write the new data
        if (savedFile.existsAsFile())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("This File Already Exists!"), translate("Are you sure you want to overwrite this file?"));
        }
        
        if (overwrite == true)
        {
            //------
            //MOVE ANY NEEDED AUDIO FILES INTO THE DEDICATED AUDIO FILES DIRECTORY FOR THIS SCENE
            //DO NOT CHANGE THE WORKING DIRECTORY HERE
            //------
            
            //create folder to hold the projects audio files
            File audioFileDirectory = (savedDirectory.getFullPathName() + File::separatorString + "Audio Files");
            audioFileDirectory.createDirectory();
            
            
            
            
            if (AppSettings::Instance()->getCopyExternalFiles() == true)
            {
                
                //search through all pads looking for audio files that need copying with the scene
                for (int i = 0; i <= 47; i++)
                {
                    //look for sampler audio files
                    if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                    {
                        String newFileName(sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("samplerAudioFilePath"));
                        
                        if (newFileName != String::empty) //if there is 'something'
                        {
                            File originalFile;
                            
                            //Need to check here if the saved file path is just file name, which 
                            //means it exists in the audio files directory and needs copying.
                            //else, nothing needs to be done.
                            
                            if (File::isAbsolutePath(newFileName) == false)
                            {
                                //get the original audio file in question
                                originalFile = File::getCurrentWorkingDirectory().getFullPathName()+ File::separatorString + newFileName;
                                
                                //create an new file in scenes "Audio Files" folder;
                                File newFile (audioFileDirectory.getFullPathName() + File::separatorString + newFileName);
                                
                                if (originalFile.existsAsFile() == true) //if the file exists (which it should)
                                {
                                    if (newFile.existsAsFile() == false) //if it doesn't already exist
                                        originalFile.copyFileTo(newFile);
                                }
                                else
                                {
                                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, translate("File not found!"), newFileName + " " + translate("could not be found."));
                                    //do something here so the loaded data is string::empty and NOT the missing audio file name
                                    //can i do this here or does it need to be done in the loadforscene method?
                                }
                            
                            }
                        }
                    }
                    //look for sequencer audio files
                    else if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                    {
                        for (int row = 0; row <= NO_OF_ROWS-1; row++)
                        {
                            String newFileName(sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row)));
                            
                            if (newFileName != String::empty) //if there is 'something'
                            {
                                File originalFile;
                                
                                //Need to check here if the saved file path is just file name, which 
                                //means it exists in the audio files directory and needs copying.
                                //else, nothing needs to be done.
                                
                                if (File::isAbsolutePath(newFileName) == false)
                                {
                                    //get the original audio file in question
                                    originalFile = File::getCurrentWorkingDirectory().getFullPathName()+ File::separatorString + newFileName;
                                    
                                    //create an new file in scenes "Audio Files" folder;
                                    File newFile (audioFileDirectory.getFullPathName() + File::separatorString + newFileName);
                                    
                                    if (originalFile.existsAsFile() == true) //if the file exists (which it should)
                                    {
                                        if (newFile.existsAsFile() == false) //if it doesn't already exist
                                            originalFile.copyFileTo(newFile);
                                    }
                                    else
                                    {
                                        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, translate("File not found!"), newFileName + " " + translate("could not be found."));
                                        //do something here so the loaded data is string::empty and NOT the missing audio file name
                                        //can i do this here or does it need to be done in the loadforscene method?
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }
             
            //----
            
            //----
            //SAVE THE FILE TO DISK
            //----
            savedFile.deleteFile();
            savedFile.create(); //create the file
            
            //can't just save the sceneData[sceneNumber] xmlelement straight into file, as the tag name needs to be different, so a new xmlelement must be
            //created which imports the child elements from sceneData[sceneNumber]
            
            //create xmlelement to be saved
            XmlElement *toBeSaved = new XmlElement("ALPHALIVE_SCENE_VERSION_1");
            
            //import child elements
            for (int i = 0; i <= 47; i++)
            {
                toBeSaved->addChildElement(new XmlElement(*sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))));
            }
            toBeSaved->addChildElement(new XmlElement(*sceneData[sceneNumber]->getChildByName("GLOBAL_DATA")));
            
            //save to file
            String xmlDoc = toBeSaved->createDocument(String::empty, false);
            savedFile.appendText(xmlDoc);
            
            delete toBeSaved;
            
            //----
            
        }
    }
    
}




bool AppDocumentState::loadSceneFromDisk(int sceneNumber, bool openBrowser, File fileToOpen)
{
    //navigate to app directory
    FileChooser loadFileChooser(translate("Select a .alphascene file to load..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphascene");
    
    bool shouldLoad;
    
    if (openBrowser == true)
        shouldLoad = loadFileChooser.browseForFileToOpen(); //open file browser
    
    if (shouldLoad || openBrowser == false)
    {
        File loadedFile;
        
        if (openBrowser == true)
            loadedFile = loadFileChooser.getResult();
        else
            loadedFile = fileToOpen;
        
        //parse file into xml file
        ScopedPointer<XmlElement> loadedXml (XmlDocument::parse(loadedFile));
        
        if (loadedXml != nullptr && loadedXml->hasTagName("ALPHALIVE_SCENE_VERSION_1"))
        {
            //clear the xmlelement for the currently selected scene number
            clearScene(sceneNumber);
            
            //put the loaded xml data into the xmlelement for the current scene
            //howcome i need to load each child individually here but not anywhere else (where i just load/save first child and it weird does the same for the others)??? (21/8/12 - does this comment still apply now?)
            for ( int i = 0; i <= 47; i++)
            {
                XmlElement* childToInsert = loadedXml->getChildByName("PAD_DATA_"+String(i));
                sceneData[sceneNumber]->addChildElement(childToInsert);
                loadedXml->removeChildElement(childToInsert, false);
            }
            XmlElement* childToInsert = loadedXml->getChildByName("GLOBAL_DATA");
            sceneData[sceneNumber]->addChildElement(childToInsert);
            
            //remove sceneData childelement from loadedXml so it isn't deleted when loadedXml goes out of scope!
            loadedXml->removeChildElement (childToInsert, false);
            
            
            
            //------------
            if (AppSettings::Instance()->getCopyExternalFiles() == true)
            {
                
                //get the folder that holds the projects audio files
                File audioFileDirectory = (loadedFile.getParentDirectory().getFullPathName() + File::separatorString + "Audio Files");
                
                //copy contents of audioFileDirectory into the working directory's Audio Files folder
                //This must be done by searching through the loaded data for audio file names and copying the files individually if they exist
                for (int i = 0; i <= 47; i++)
                {
                    //look for sampler audio files
                    if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                    {
                        String newFileName(sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("samplerAudioFilePath"));
                        
                        if (newFileName != String::empty) //if there is 'something'
                        {
                            File originalFile(File::nonexistent);
                            File newFile(File::nonexistent);
                            
                            //Need to check here if the saved file path is just the file
                            //name or the full path, and handle the situation appropriatly
                            
                            if (File::isAbsolutePath(newFileName) == false)
                            {
                                //File should be an internal file
                                originalFile = audioFileDirectory.getFullPathName()+ File::separatorString + newFileName;
                                newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + newFileName;
                            }
                            
                            else if (File::isAbsolutePath(newFileName) == true)
                            {
                                //file is an external file.
                                originalFile = newFileName;
                                newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + originalFile.getFileName();
                                
                                //re-set the path name within sceneData from the full path name to just the file name
                                sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("samplerAudioFilePath", newFile.getFileName());
                            }
                            
                            
                            if (originalFile.existsAsFile() == true) //if the file exists (which it should)
                            {
                                if (newFile.existsAsFile() == false) //if it doesn't already exist
                                {
                                    originalFile.copyFileTo(newFile); 
                                    std::cout << "Copying audio file from loaded scene's dir to current working dir!\n";
                                }
                            }
                            else
                            {
                                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, translate("File not found!"), newFileName + " " + translate("could not be found."));
                                //do something here so the loaded data is string::empty and NOT the missing audio file name
                            }
                        }
                    }
                    
                    
                    
                    //look for sequencer audio files
                    else if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                    {
                        for (int row = 0; row <= NO_OF_ROWS-1; row++)
                        {
                            
                            String newFileName(sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row)));
                            
                            if (newFileName != String::empty) //if there is 'something'
                            {
                                File originalFile(File::nonexistent);
                                File newFile(File::nonexistent);
                                
                                //Need to check here if the saved file path is just the file
                                //name or the full path, and handle the situation appropriatly
                                
                                if (File::isAbsolutePath(newFileName) == false)
                                {
                                    //File should be an internal file
                                    originalFile = audioFileDirectory.getFullPathName()+ File::separatorString + newFileName;
                                    newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + newFileName;
                                }
                                
                                else if (File::isAbsolutePath(newFileName) == true)
                                {
                                    //file is an external file.
                                    originalFile = newFileName;
                                    newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + originalFile.getFileName();
                                    
                                    //re-set the path name within sceneData from the full path name to just the file name
                                    sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("sequencerSamplesAudioFilePath"+String(row), newFile.getFileName());
                                }
                                
                                
                                if (originalFile.existsAsFile() == true) //if the file exists (which it should)
                                {
                                    if (newFile.existsAsFile() == false) //if it doesn't already exist
                                    {
                                        originalFile.copyFileTo(newFile); 
                                        std::cout << "Copying audio file from loaded scene's dir to current working dir!\n";
                                    }
                                }
                                else
                                {
                                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, translate("File not found!"), newFileName + " " + translate("could not be found."));
                                    //do something here so the loaded data is string::empty and NOT the missing audio file name
                                }
                                
                            }
                        }
                    }
                }
            }
            else if (AppSettings::Instance()->getCopyExternalFiles() == false)
            {
                //This else if statement is needed in case the option to copy 
                //external files is currently false when importing scenes
                //which have their audio files stored in the directory 
                //(and hence only the file NAME (not path) is saved), as the audio files
                //will not be found once in the new project. The names need 
                //to be converted from just the file name to the full path to work
                
                //get the folder that holds the projects audio files
                File audioFileDirectory = (loadedFile.getParentDirectory().getFullPathName() + File::separatorString + "Audio Files");
                
                for (int i = 0; i <= 47; i++)
                {
                    //look for sampler audio files
                    if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                    {
                        String newFileString(sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("samplerAudioFilePath"));
                        
                        if (newFileString != String::empty) //if there is 'something'
                        {
    
                            if (File::isAbsolutePath(newFileString) == false)
                            {
                                //File should be an internal file
                                File newFile(audioFileDirectory.getFullPathName()+ File::separatorString + newFileString); //should be a String?
                                
                                //set the saved file name to be the full path...
                                sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("samplerAudioFilePath", newFile.getFullPathName()); 
                            }
                        }
                    }
                    
                    //look for sequencer audio files
                    else if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                    {
                        for (int row = 0; row <= NO_OF_ROWS-1; row++)
                        {
                            
                            String newFileString(sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row)));
                            if (newFileString != String::empty) //if there is 'something'
                            {
                                //if the string saved is just the file name...
                                if (File::isAbsolutePath(newFileString) == false)
                                {
                                    //... get the full path name of the file...
                                    File newFile(audioFileDirectory.getFullPathName()+ File::separatorString + newFileString); //should be a String?
                                    //... a re-set the path name within sceneData, NOT PadSettings as this data hasn't been loaded
                                    //into PadSettings yet
                                    sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("sequencerSamplesAudioFilePath"+String(row), newFile.getFullPathName());
                                }
                            }
                        }
                    }
                }
                
            }
           
            return true;
        }
        else
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, translate("Cannot Open File"), translate("The selected AlphaLive Scene file seems to be corrupt."));
            
            return false;
        } 
    }
    
    else // 'cancel' button pressed
    {
        return false;
    }
    
}




void AppDocumentState::savePadToDisk (int padNumber)
{
    //This function will be similar to 'saveSceneToDisk()'
    //and will extract the pad data element of sceneData[currentlySelectedScene]
    //and put it int a new XmlElment which is then saved as an external file.
    //The only reason I'm not implementing that right now is because,
    //like with exporting scenes, the audio files need to be copied to
    //an associated audio files directory. Also when we implement this function/feature,
    //the 'loadPadFromDisk()' function will need to import any associated
    //audio files into the current projects audio files directory.
    
    //Like with loadFromScene(), all the code that access padData within saveToScene()
    //has been moved to a seperate function which will be called winthin this function
    //to get the right data from PadSettings.
    
    //Importing and exporting pad data should be accessible from right-clicking
    //on a pad, as well as from the menu bar (within the menu bar export settings
    //will only be selectable when a single pad is selected).
    
}

void AppDocumentState::loadPadFromDisk (Array<int> selectedPads_, bool openBrowser, File fileToOpen)
{
    //navigate to app directory
    FileChooser loadFileChooser(translate("Select a .alphapad file to load..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphapad");
    
    bool shouldLoad;
    
    if (openBrowser == true)
        shouldLoad = loadFileChooser.browseForFileToOpen(); //open file browser
    
    if (shouldLoad || openBrowser == false)
    {
        File loadedFile;
        
        if (openBrowser == true)
            loadedFile = loadFileChooser.getResult();
        else
            loadedFile = fileToOpen;
        
        //parse file into xml file
        ScopedPointer<XmlElement> loadedXml (XmlDocument::parse(loadedFile));
        
        if (loadedXml != nullptr && loadedXml->hasTagName("ALPHALIVE_PAD_SETTINGS_VERSION_1"))
        {
            //saveToScene(currentlySelectedScene);
            
            XmlElement* padData = loadedXml->getChildByName("PAD_DATA");
            
            for (int i = 0; i < selectedPads_.size(); i++)
            {
                loadPadSettings(selectedPads_[i], padData);
            }
            
            loadedXml->removeChildElement (padData, true);
            
            //update GUI - calls update() in mainComponent
            
            //SHOULD I CREATE A NEW GUIUPDATEFLAG VALUE THAT ONLY ALLOWS A MINIMUM AMOUNT OF
            //THE GUI T0 BE UPDATED? HOWEVER THERE WILL PROBABLY BE A DIFFERENCE BETWEEN WHETHER
            //IF THIS FUNCTION WAS CALLED FROM THE TOOLBOX OR BY MANUALLY LOADING A PADS SETTINGS.
            guiUpdateFlag = 0;
            notifyObs();
        }
        
    }
}







void AppDocumentState::saveSequence (int currentlySelectedSeqNumber, int currentlySelectedPad)
{
    //navigate to app directory
    FileChooser saveFileChooser(translate("Create a single sequence file to save..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphaseq");
    
    if (saveFileChooser.browseForFileToSave(false))
    {
        //Surely there's a easier way to do the following code? FileBasedDocument
        File savedFile (saveFileChooser.getResult()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".alphaseq"; //append an extension name to the filepath name
        savedFile = stringFile; //set the file to this name
        
        bool overwrite = true; //by default true
        
        //delete the file if it exists &write the new data
        if (savedFile.existsAsFile())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("This File Already Exists!"), translate("Are you sure you want to overwrite this file?"));
        }
        
        if (overwrite == true)
        {
            savedFile.deleteFile();
            savedFile.create(); //create the file
            
            XmlElement sequenceDataXml("SEQUENCE_DATA");
            
            //get single sequence data string based on currently selected sequencer number slider value
            sequenceDataXml.setAttribute("sequenceData", PAD_SETTINGS_pad->getSequencerDataString(currentlySelectedSeqNumber));
            
            String xmlDoc = sequenceDataXml.createDocument(String::empty);
            savedFile.appendText(xmlDoc);
            
            std::cout << savedFile.getFullPathName() << std::endl;
            
            //AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Single Sequence Saved", "The sequence has been successfully saved to file");
        }
    }
    
}


void AppDocumentState::loadSequence (int currentlySeletedSeqNumber, 
                                     Array<int> selectedPads_,
                                     bool openBrowser, 
                                     File fileToOpen)
{
    //navigate to app directory
    FileChooser loadFileChooser(translate("Select a .alphaseq file to load..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphaseq");
    
    bool shouldLoad;
    
    if (openBrowser == true)
        shouldLoad = loadFileChooser.browseForFileToOpen(); //open file browser
    
    if (shouldLoad || openBrowser == false)
    {
        //File loadedFile (loadFileChooser.getResult());
        File loadedFile;
        
        if (openBrowser == true)
            loadedFile = loadFileChooser.getResult();
        else
            loadedFile = fileToOpen;
        
        XmlElement* xml = XmlDocument::parse(loadedFile);
        if (xml != nullptr && xml->hasTagName("SEQUENCE_DATA"))
        {
            for (int i = 0; i < selectedPads_.size(); i++)
            {
                int padNum = selectedPads_[i];
                //get the saved string and call stringToSeqData() to convert it back int a stream of int's
                PAD_SETTINGS_pads->stringToSeqData(xml->getStringAttribute("sequenceData"), currentlySeletedSeqNumber);
            }
            

        }
        
        delete xml;
        
        //update GUI
        //SHOULD I CREATE A NEW GUIUPDATEFLAG VALUE THAT ONLY ALLOWS A MINIMUM AMOUNT OF
        //THE GUI T0 BE UPDATED?
        notifyObs();
    }
}




void AppDocumentState::saveSequenceSet(int currentlySelectedPad)
{
    //navigate to app directory
    FileChooser saveFileChooser(translate("Create a sequence set file to save..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphaseqset");
    
    if (saveFileChooser.browseForFileToSave(false))
    {
        //Surely there's a easier way to do the following code? FileBasedDocument
        File savedFile (saveFileChooser.getResult()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".alphaseqset"; //append an extension name to the filepath name
        savedFile = stringFile; //set the file to this name
        
        bool overwrite = true; //by default true
        
        //delete the file if it exists &write the new data
        if (savedFile.existsAsFile())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("This File Already Exists!"), translate("Are you sure you want to overwrite this file?"));
        }
        
        if (overwrite == true)
        {
            savedFile.deleteFile();
            savedFile.create(); //create the file
            
            XmlElement sequenceDataXml("SEQUENCE_DATA");
            
            sequenceDataXml.setAttribute("sequencerNumberOfSequences", PAD_SETTINGS_pad->getSequencerNumberOfSequences());
            
            //get all sequence data strings 
            for (int i = 0; i <= NO_OF_SEQS-1; i++)
            {
                sequenceDataXml.setAttribute("sequenceData"+String(i), PAD_SETTINGS_pad->getSequencerDataString(i));
            }
            
            String xmlDoc = sequenceDataXml.createDocument(String::empty);
            savedFile.appendText(xmlDoc);
            
        }
    }
}





void AppDocumentState::loadSequenceSet(Array<int> selectedPads_,
                                       bool openBrowser,
                                       File fileToOpen)
{
    //navigate to app directory
    FileChooser loadFileChooser(translate("Select a .alphaseqset file to load..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphaseqset");
    
    bool shouldLoad;
    
    if (openBrowser == true)
        shouldLoad = loadFileChooser.browseForFileToOpen(); //open file browser
    
    if (shouldLoad || openBrowser == false)
    {
        //File loadedFile (loadFileChooser.getResult());
        File loadedFile;
        
        if (openBrowser == true)
            loadedFile = loadFileChooser.getResult();
        else
            loadedFile = fileToOpen;
        
        XmlElement* xml = XmlDocument::parse(loadedFile);
        if (xml != nullptr && xml->hasTagName("SEQUENCE_DATA"))
        {
            for (int i = 0; i < selectedPads_.size(); i++)
            {
                int padNum = selectedPads_[i];
                //get the saved string for each of the seq and call stringToSeqData() to convert them back int a stream of int's
                for (int seqNumber = 0; seqNumber <= NO_OF_SEQS-1; seqNumber++)
                {
                    PAD_SETTINGS_pads->stringToSeqData(xml->getStringAttribute("sequenceData"+String(seqNumber)), seqNumber);
                }
                
                if (xml->hasAttribute("sequencerNumberOfSequences"))
                    PAD_SETTINGS_pads->setSequencerNumberOfSequences(xml->getIntAttribute("sequencerNumberOfSequences"));
            }
            
        }
        
        delete xml;
        
        //update GUI
        //SHOULD I CREATE A NEW GUIUPDATEFLAG VALUE THAT ONLY ALLOWS A MINIMUM AMOUNT OF
        //THE GUI T0 BE UPDATED? 
        notifyObs();
        
    }
}


void AppDocumentState::saveEffect (int currentlySelectedPad)
{
    //navigate to app directory
    FileChooser saveFileChooser(translate("Create an effect file to save..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphapad");
    
    if (saveFileChooser.browseForFileToSave(false))
    {
        //Surely there's a easier way to do the following code? FileBasedDocument
        File savedFile (saveFileChooser.getResult()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".alphapad"; //append an extension name to the filepath name
        savedFile = stringFile; //set the file to this name
        
        bool overwrite = true; //by default true
		
		int effect = 0;
		int modeCheck = PAD_SETTINGS_pad->getMode();
        
        //delete the file if it exists &  write the new data
        if (savedFile.existsAsFile())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("This File Already Exists!"), translate("Are you sure you want to overwrite this file?"));
        }
        
        if (overwrite == true)
        {
            savedFile.deleteFile();
            savedFile.create(); //create the file
            
            XmlElement *effectDataXml = new XmlElement("PAD_DATA");
			
			if (modeCheck == 2) 
			{
				effect = PAD_SETTINGS_pad->getSamplerEffect();
			}
			else if (modeCheck == 3)
			{
				effect = PAD_SETTINGS_pad->getSequencerEffect();
			}
			
			effectDataXml->setAttribute("effect", effect);
			
			if (effect == 1) //Gain and Pan
			{
				effectDataXml->setAttribute("padFxGainPanGain", PAD_SETTINGS_pad->getPadFxGainPanGain());
				effectDataXml->setAttribute("padFxGainPanPan", PAD_SETTINGS_pad->getPadFxGainPanPan());
				effectDataXml->setAttribute("padFxGainPanAlphaTouch", PAD_SETTINGS_pad->getPadFxGainPanAlphaTouch());
				effectDataXml->setAttribute("padFxGainPanAtReverse", PAD_SETTINGS_pad->getPadFxGainPanAtReverse());
				effectDataXml->setAttribute("padFxGainPanAtIntensity", PAD_SETTINGS_pad->getPadFxGainPanAtIntensity());
			}
			else if (effect == 2) //LPF
			{
				effectDataXml->setAttribute("padFxLpfMix", PAD_SETTINGS_pad->getPadFxLpfMix());
				effectDataXml->setAttribute("padFxLpfFreq", PAD_SETTINGS_pad->getPadFxLpfFreq());
				effectDataXml->setAttribute("padFxLpfBandwidth", PAD_SETTINGS_pad->getPadFxLpfBandwidth());
				effectDataXml->setAttribute("padFxLpfAlphaTouch", PAD_SETTINGS_pad->getPadFxLpfAlphaTouch());
				effectDataXml->setAttribute("padFxLpfAtReverse", PAD_SETTINGS_pad->getPadFxLpfAtReverse());
				effectDataXml->setAttribute("padFxLpfAtIntensity", PAD_SETTINGS_pad->getPadFxLpfAtIntensity());
			}
			else if (effect == 3) //HPF
			{
				effectDataXml->setAttribute("padFxHpfMix", PAD_SETTINGS_pad->getPadFxHpfMix());
				effectDataXml->setAttribute("padFxHpfFreq", PAD_SETTINGS_pad->getPadFxHpfFreq());
				effectDataXml->setAttribute("padFxHpfBandwidth", PAD_SETTINGS_pad->getPadFxHpfBandwidth());
				effectDataXml->setAttribute("padFxHpfAlphaTouch", PAD_SETTINGS_pad->getPadFxHpfAlphaTouch());
				effectDataXml->setAttribute("padFxHpfAtReverse", PAD_SETTINGS_pad->getPadFxHpfAtReverse());
				effectDataXml->setAttribute("padFxHpfAtIntensity", PAD_SETTINGS_pad->getPadFxHpfAtIntensity());
			}
			else if (effect == 4) //BPF
			{
				effectDataXml->setAttribute("padFxBpfMix", PAD_SETTINGS_pad->getPadFxBpfMix());
				effectDataXml->setAttribute("padFxBpfFreq", PAD_SETTINGS_pad->getPadFxBpfFreq());
				effectDataXml->setAttribute("padFxBpfBandwidth", PAD_SETTINGS_pad->getPadFxBpfBandwidth());
				effectDataXml->setAttribute("padFxBpfAlphaTouch", PAD_SETTINGS_pad->getPadFxBpfAlphaTouch());
				effectDataXml->setAttribute("padFxBpfAtReverse", PAD_SETTINGS_pad->getPadFxBpfAtReverse());
				effectDataXml->setAttribute("padFxBpfAtIntensity", PAD_SETTINGS_pad->getPadFxBpfAtIntensity());
			}
			else if (effect == 5) //Distortion
			{
				effectDataXml->setAttribute("padFxDistortionInputGain", PAD_SETTINGS_pad->getPadFxDistortionInputGain());
				effectDataXml->setAttribute("padFxDistortionDrive", PAD_SETTINGS_pad->getPadFxDistortionDrive());
				effectDataXml->setAttribute("padFxDistortionWetDryMix", PAD_SETTINGS_pad->getPadFxDistortionWetDryMix());
				effectDataXml->setAttribute("padFxDistortionTone", PAD_SETTINGS_pad->getPadFxDistortionTone());
				effectDataXml->setAttribute("padFxDistortionTypeMenu", PAD_SETTINGS_pad->getPadFxDistortionTypeMenu());
				effectDataXml->setAttribute("padFxDistortionAlphaTouch", PAD_SETTINGS_pad->getPadFxDistortionAlphaTouch());
				effectDataXml->setAttribute("padFxDistortionAtReverse", PAD_SETTINGS_pad->getPadFxDistortionAtReverse());
				effectDataXml->setAttribute("padFxDistortionAtIntensity", PAD_SETTINGS_pad->getPadFxDistortionAtIntensity());
			}
			else if (effect == 6) //Bitcrusher
			{
				effectDataXml->setAttribute("padFxBitcrusherInputGain", PAD_SETTINGS_pad->getPadFxBitcrusherInputGain());
				effectDataXml->setAttribute("padFxBitcrusherDownsample", PAD_SETTINGS_pad->getPadFxBitcrusherDownsample());
				effectDataXml->setAttribute("padFxBitcrusherCrush", PAD_SETTINGS_pad->getPadFxBitcrusherCrush());
				effectDataXml->setAttribute("padFxBitcrusherSmoothing", PAD_SETTINGS_pad->getPadFxBitcrusherSmoothing());
				effectDataXml->setAttribute("padFxBitcrusherWetDryMix", PAD_SETTINGS_pad->getPadFxBitcrusherWetDryMix());
				effectDataXml->setAttribute("padFxBitcrusherAlphaTouch", PAD_SETTINGS_pad->getPadFxBitcrusherAlphaTouch());
				effectDataXml->setAttribute("padFxBitcrusherAtReverse", PAD_SETTINGS_pad->getPadFxBitcrusherAtReverse());
				effectDataXml->setAttribute("padFxBitcrusherAtIntensity", PAD_SETTINGS_pad->getPadFxBitcrusherAtIntensity());
			}
			else if (effect == 7) //Delay
			{
				effectDataXml->setAttribute("padFxDelayMix", PAD_SETTINGS_pad->getPadFxDelayMix());
				effectDataXml->setAttribute("padFxDelayTime", PAD_SETTINGS_pad->getPadFxDelayTime());
				effectDataXml->setAttribute("padFxDelayFeedback", PAD_SETTINGS_pad->getPadFxDelayFeedback());
				effectDataXml->setAttribute("padFxDelayLpfFreq", PAD_SETTINGS_pad->getPadFxDelayLpfFreq());
				effectDataXml->setAttribute("padFxDelayHpfFreq", PAD_SETTINGS_pad->getPadFxDelayHpfFreq());
				effectDataXml->setAttribute("padFxDelaySync", PAD_SETTINGS_pad->getPadFxDelaySync());
				effectDataXml->setAttribute("padFxDelayTimeMenu", PAD_SETTINGS_pad->getPadFxDelayTimeMenu());
				effectDataXml->setAttribute("padFxDelayAlphaTouch", PAD_SETTINGS_pad->getPadFxDelayAlphaTouch());
				effectDataXml->setAttribute("padFxDelayAtReverse", PAD_SETTINGS_pad->getPadFxDelayAtReverse());
				effectDataXml->setAttribute("padFxDelayAtIntensity", PAD_SETTINGS_pad->getPadFxDelayAtIntensity());
			}
			else if (effect == 8) //Reverb
			{
				effectDataXml->setAttribute("padFxReverbMix", PAD_SETTINGS_pad->getPadFxReverbMix());
				effectDataXml->setAttribute("padFxReverbRoomSize", PAD_SETTINGS_pad->getPadFxReverbRoomSize());
				effectDataXml->setAttribute("padFxReverbDamping", PAD_SETTINGS_pad->getPadFxReverbDamping());
				effectDataXml->setAttribute("padFxReverbWidth", PAD_SETTINGS_pad->getPadFxReverbWidth());
				effectDataXml->setAttribute("padFxReverbFreezeMode", PAD_SETTINGS_pad->getPadFxReverbFreezeMode());
				effectDataXml->setAttribute("padFxReverbAlphaTouch", PAD_SETTINGS_pad->getPadFxReverbAlphaTouch());
				effectDataXml->setAttribute("padFxReverbAtReverse", PAD_SETTINGS_pad->getPadFxReverbAtReverse());
				effectDataXml->setAttribute("padFxReverbAtIntensity", PAD_SETTINGS_pad->getPadFxReverbAtIntensity());
			}
			else if (effect == 9) //Flanger
			{
				effectDataXml->setAttribute("padFxFlangerMix", PAD_SETTINGS_pad->getPadFxFlangerMix());
				effectDataXml->setAttribute("padFxFlangerRate", PAD_SETTINGS_pad->getPadFxFlangerRate());
				effectDataXml->setAttribute("padFxFlangerFeedback", PAD_SETTINGS_pad->getPadFxFlangerFeedback());
				effectDataXml->setAttribute("padFxFlangerIntensity", PAD_SETTINGS_pad->getPadFxFlangerIntensity());
				effectDataXml->setAttribute("padFxFlangerRateMenu", PAD_SETTINGS_pad->getPadFxFlangerRateMenu());
				effectDataXml->setAttribute("padFxFlangerSync", PAD_SETTINGS_pad->getPadFxFlangerSync());
				effectDataXml->setAttribute("padFxFlangerAlphaTouch", PAD_SETTINGS_pad->getPadFxFlangerAlphaTouch());
				effectDataXml->setAttribute("padFxFlangerAtReverse", PAD_SETTINGS_pad->getPadFxFlangerAtReverse());
				effectDataXml->setAttribute("padFxFlangerAtIntensity", PAD_SETTINGS_pad->getPadFxFlangerAtIntensity());
			}
			else if (effect == 10) //Tremolo
			{
				effectDataXml->setAttribute("padFxTremoloDepth", PAD_SETTINGS_pad->getPadFxTremoloDepth());
				effectDataXml->setAttribute("padFxTremoloRate", PAD_SETTINGS_pad->getPadFxTremoloRate());
				effectDataXml->setAttribute("padFxTremoloShape", PAD_SETTINGS_pad->getPadFxTremoloShape());
				effectDataXml->setAttribute("padFxTremoloSync", PAD_SETTINGS_pad->getPadFxTremoloSync());
				effectDataXml->setAttribute("padFxTremoloRateMenu", PAD_SETTINGS_pad->getPadFxTremoloRateMenu());
				effectDataXml->setAttribute("padFxTremoloAlphaTouch", PAD_SETTINGS_pad->getPadFxTremoloAlphaTouch());
				effectDataXml->setAttribute("padFxTremoloAtReverse", PAD_SETTINGS_pad->getPadFxTremoloAtReverse());
				effectDataXml->setAttribute("padFxTremoloAtIntensity", PAD_SETTINGS_pad->getPadFxTremoloAtIntensity());
			}
			
			//create xmlelement to be saved
            XmlElement *toBeSaved = new XmlElement("ALPHALIVE_PAD_SETTINGS_VERSION_1");
            
			toBeSaved->addChildElement(new XmlElement (*effectDataXml));
			
            //save to file
            String xmlDoc = toBeSaved->createDocument(String::empty, false);
            savedFile.appendText(xmlDoc);
            
            delete toBeSaved;
			delete effectDataXml;
            
            std::cout << savedFile.getFullPathName() << std::endl;
            
            //AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Effect Saved", "The effect has been successfully saved to file");
        }
    }
    
}

void AppDocumentState::exportSampleBank (int currentlySelectedPad)
{
	//navigate to app directory
    FileChooser saveFileChooser(translate("Create a .alphabank file to save..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphabank");
    
    if (saveFileChooser.browseForFileToSave(false))
    {
        //set it so that if the user has selected the toolbox directory, don't create a parent directory. Good idea?
        
		//create a bank directory
        String bankName (saveFileChooser.getResult().getFileName());
        File bankDirectory (saveFileChooser.getResult().getFullPathName() + " (Alphabank)");
        
        //check to see if it already exists and, if so, if it should be overwritten
        bool overwrite = true; 
        
        if (bankDirectory.exists())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("This File Already Exists!"), translate("Are you sure you want to overwrite this file?"));
        }
        
        if (overwrite == true)
        {
            //first delete directory (incase it already exists) and then create it
            bankDirectory.deleteRecursively();
            bankDirectory.createDirectory();
            
            //create folder to hold the banks audio files
            File audioFileDirectory = (bankDirectory.getFullPathName() + File::separatorString + bankName);
            audioFileDirectory.createDirectory();
            
            //create .alphabank file
            File bankFile = (bankDirectory.getFullPathName() + File::separatorString + bankName + ".alphabank");
            bankFile.create();
            
            XmlElement *alphaBankDataXml = new XmlElement("ALPHABANK"); //create Xml before extension name is appended
            alphaBankDataXml->setAttribute("name", String(bankFile.getFileNameWithoutExtension()));
            alphaBankDataXml->setAttribute("numSamples", 12);
            
            for (int i = 0; i < 12; i++)
            {
                File audioFilePath;						  
                
                audioFilePath = PAD_SETTINGS_pad->getSequencerSamplesAudioFilePath(i);
                
                if (audioFilePath != File::nonexistent)
                {
                    //create a new file for the seqs audio file but only if it doesn't already exist!!
                    File audioFileCopy (audioFileDirectory.getFullPathName() + File::separatorString + audioFilePath.getFileName());
                    
                    if (audioFileCopy.existsAsFile() == false) //if it doesn't yet exist
                    {
                        //copy the added audio file into the newly created file
                        audioFilePath.copyFileTo(audioFileCopy);
                    }
                    
                    else if (audioFileCopy.existsAsFile() == true) //if it already exists 
                    {
                        Array<File> matchingFilesArray;
                        String fileWildCard (audioFileCopy.getFileNameWithoutExtension()+"*");
                        bool importedFileNeedsCopying = true;
                        
                        //Find all possible duplicates of the imported file using fileWildCard
                        //and add reference of all possible files to matchingFilesArray.
                        audioFileDirectory.findChildFiles(matchingFilesArray, 2, false, fileWildCard);
                        
                        for (int i = 0; i < matchingFilesArray.size(); i++)
                        {
                            if (audioFilePath.hasIdenticalContentTo(matchingFilesArray[i]) == true)
                            {
                                //if it finds a duplicate, flag that the file doesn't need copying
                                importedFileNeedsCopying = false;
                                //set the file
                                audioFileCopy = matchingFilesArray[i];
                                //break out of the for loop once a duplicate has been found to prevent unnecessary checks
                                break;
                            }
                        }
                        
                        if (importedFileNeedsCopying == true) 
                        {
                            //if no duplicate was found...
                            //... copy the added file with an appended name
                            audioFileCopy = audioFileCopy.getNonexistentSibling();
                            audioFilePath.copyFileTo(audioFileCopy); 
                        }
                    }
                    
                    audioFilePath = audioFileCopy;
					
                    alphaBankDataXml->setAttribute(String("sample" + String(i+1)), 
                                                   File::separatorString + String(audioFilePath.getRelativePathFrom(bankFile)));
                }
            }
            
            //create xmlelement to be saved
            XmlElement *toBeSaved = new XmlElement("ALPHALIVE_SAMPLE_SET");
            
            toBeSaved->addChildElement(new XmlElement (*alphaBankDataXml));
            
            //save to file
            String xmlDoc = toBeSaved->createDocument(String::empty, false);
            bankFile.appendText(xmlDoc);
            
            delete toBeSaved;
            delete alphaBankDataXml;
            
            std::cout << bankFile.getFullPathName() << std::endl;
        }
	}
}


void AppDocumentState::importSampleBank (Array<int> selectedPads_, bool openBrowser, File fileToOpen)
{
    //navigate to app directory
    FileChooser loadFileChooser(translate("Select a .alphabank file to load..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.alphabank");
    
    bool shouldLoad;
    
    if (openBrowser == true)
        shouldLoad = loadFileChooser.browseForFileToOpen(); //open file browser
    
    if (shouldLoad == true || openBrowser == false)
    {
        File loadedFile;
        
        if (openBrowser == true)
            loadedFile = loadFileChooser.getResult();
        else
            loadedFile = fileToOpen;
        
        //parse file into xml file
        ScopedPointer<XmlElement> xmlData (XmlDocument::parse(loadedFile));
        
        //the code here is the same as the relevent code for applying sample banks to sequencer pads from the toolbox.
        //Ideally, the toolbox should just call this function instead of including the same code, however for sampler
        //mode the mainComponent needs updating which cannot be done from here.
        //The arguments of this function are there to allow for such implementation.
        
        if (xmlData != nullptr && xmlData->hasTagName("ALPHALIVE_SAMPLE_SET"))
        {
            int numOfSamples = xmlData->getChildElement(0)->getIntAttribute("numSamples");
            String fileDirPath (loadedFile.getParentDirectory().getFullPathName());
            StringArray sampleFilePaths;
            
            for (int i = 0; i < numOfSamples; i++)
            {
                sampleFilePaths.add(fileDirPath + xmlData->getChildElement(0)->getStringAttribute("sample" + String(i+1)));
            }
            
            for (int i = 0; i < selectedPads_.size(); i++)
            {
                int padNum = selectedPads_[i];
                
                for (int row = 0; row < 12; row++)
                {
                    PAD_SETTINGS_pads->setSequencerSamplesAudioFilePath(File(sampleFilePaths[row]), row);
                }
            }
        }
        
    }
}

void AppDocumentState::createMidiFile (int currentlySelectedSeqNumber, int currentlySelectedPad, int isSeqSet)
{
    //navigate to app directory
    FileChooser saveFileChooser(translate("Create a .mid file to save..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.mid");
    
    if (saveFileChooser.browseForFileToSave(false))
    {
        File savedFile (saveFileChooser.getResult()); //get file that the user has 'saved'
        String stringFile = savedFile.getFullPathName(); //get the filepath name of the file as a string
        stringFile = stringFile + ".mid"; //append an extension name to the filepath name
        savedFile = stringFile; //set the file to this name
        
        bool overwrite = true; //by default true
        
        //delete the file if it exists &write the new data
        if (savedFile.existsAsFile())
        {
            overwrite = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("This File Already Exists!"), translate("Are you sure you want to overwrite this file?"));
        }
        
        if (overwrite == true)
        {
            //on the sequencer grid, a quarter note is 4 columns.
            //therefore, the setTicksPerQuarterNote value would be equal to the length of two sequence 'beats',
            //so one beat would equal noOfTicks/4.
            
            int noOfTicks = 24;
            MidiMessageSequence midiSequence;
            int noteLength;
            int rowNoteNumber[NO_OF_ROWS];
            
            //set note number data and note length depending on mode
            if (PAD_SETTINGS_pad->getSequencerMode() == 1) //midi mode
            {
                noteLength = PAD_SETTINGS_pad->getSequencerMidiNoteLength();
                for (int i = 0; i < NO_OF_ROWS; i++)
                    rowNoteNumber[i] = PAD_SETTINGS_pad->getSequencerMidiNote(i); 
            }
            else //samples mode
            {
                //default value
                noteLength = 1;
                //map GM drum mapping to rowNoteNumber array
                rowNoteNumber[0] = 35;
                rowNoteNumber[1] = 36;
                rowNoteNumber[2] = 37;
                rowNoteNumber[3] = 38;
                rowNoteNumber[4] = 40;
                rowNoteNumber[5] = 41;
                rowNoteNumber[6] = 42;
                rowNoteNumber[7] = 45;
                rowNoteNumber[8] = 46;
                rowNoteNumber[9] = 48;
                rowNoteNumber[10] = 49;
                rowNoteNumber[11] = 51;
            }
            
            
            if (isSeqSet == false)
            {
                //search through sequence data array of current sequence and create midi on/off messages
                for (int row = 0; row <= NO_OF_ROWS-1; row++)
                {
                    if (rowNoteNumber[row] >= 0)
                    {
                        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
                        {
                            int noteData = PAD_SETTINGS_pad->getSequencerData(currentlySelectedSeqNumber, row, column);
                            
                            if (noteData > 0)
                            {
                                double noteStart = column * (noOfTicks/4);
                                double noteEnd = noteStart + (noteLength * (noOfTicks/4));
                                
                                //create note on message
                                //should it export with currently set midi channel?
                                MidiMessage noteOnMessage(MidiMessage::noteOn(1, rowNoteNumber[row], (uint8)noteData));
                                noteOnMessage.setTimeStamp(noteStart);
                                midiSequence.addEvent(noteOnMessage);
                                
                                //create note off message
                                MidiMessage noteOffMessage(MidiMessage::noteOff(1, rowNoteNumber[row]));
                                noteOffMessage.setTimeStamp(noteEnd);
                                midiSequence.addEvent(noteOffMessage);
                                
                                //juce doc says you must call the below after adding note on events,
                                //however when I do this it will cut short any notes if a note of the
                                //same number is added before the previous note is finished, which
                                //ideally is not what we want.
                                //midiSequence.updateMatchedPairs();
                            }
                        }
                    }
                }
            }
            else if (isSeqSet == true)
            {
                int noOfSeqs = PAD_SETTINGS_pad->getSequencerNumberOfSequences();
                
                //search through sequence data array of current sequence and create midi on/off messages
                for (int seq = 0; seq < noOfSeqs; seq++)
                {
                    for (int row = 0; row <= NO_OF_ROWS-1; row++)
                    {
                        if (rowNoteNumber[row] >= 0)
                        {
                            for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
                            {
                                int noteData = PAD_SETTINGS_pad->getSequencerData(seq, row, column);
                                
                                if (noteData > 0)
                                {
                                    double noteStart = (column + (seq * 32)) * (noOfTicks/4); //why can't i use NO_OF_COLUMNS here? it creates weird maths problems and causes out of time notes
                                    double noteEnd = noteStart + (noteLength * (noOfTicks/4));
                                    
                                    //create note on message
                                    //should it export with currently set midi channel?
                                    MidiMessage noteOnMessage(MidiMessage::noteOn(1, rowNoteNumber[row], (uint8)noteData));
                                    noteOnMessage.setTimeStamp(noteStart);
                                    midiSequence.addEvent(noteOnMessage);
                                    
                                    //create note off message
                                    MidiMessage noteOffMessage(MidiMessage::noteOff(1, rowNoteNumber[row]));
                                    noteOffMessage.setTimeStamp(noteEnd);
                                    midiSequence.addEvent(noteOffMessage);
                                    
                                    //juce doc says you must call the below after adding note on events,
                                    //however when I do this it will cut short any notes if a note of the
                                    //same number is added before the previous note is finished, which
                                    //ideally is not what we want.
                                    //midiSequence.updateMatchedPairs();
                                }
                            }
                        }
                    }
                }
            }
            
            
            if (midiSequence.getNumEvents() > 0)
            {
                //create midi file and add data
                MidiFile midiFile;
                midiFile.addTrack(midiSequence);
                midiFile.setTicksPerQuarterNote(noOfTicks);
                
                if (savedFile.exists())
                    savedFile.deleteFile();
                
                FileOutputStream outputStream(savedFile);
                midiFile.writeTo(outputStream);
                
            }
            else
            {
                AlertWindow::showMessageBox(AlertWindow::InfoIcon, translate("MIDI file not created!"), translate("There is no sequence to create a MIDI file from."));
            }
            
            
        }
        
    }    
}





void AppDocumentState::importMidiFile (int currentlySelectedSeqNumber, 
                     Array<int> selectedPads_,
                     int isSeqSet,
                     bool shouldImportNoteData,                  
                     bool openBrowser, 
                     File fileToOpen)
{
	
    //current issues with importing midi files
    // - If importing a big midi file as a single sequence it will determine which
    //note numbers will be imported as well as those note numbers events using the entire
    //midi sequence, even if most of it won't actually be imported. This could result in
    //no events actually be imported as all the events within the first 32 columns could
    //be deleted due to being out of the 'first 12 notes' range. Maybe the midi sequence should
    //be cut to the right length first?
    
    
    //navigate to app directory
    FileChooser loadFileChooser(translate("Select a .mid file to import..."), 
                                StoredSettings::getInstance()->appProjectDir, 
                                "*.mid");
    
    bool shouldLoad;
    
    if (openBrowser == true)
        shouldLoad = loadFileChooser.browseForFileToOpen(); //open file browser
    
    if (shouldLoad || openBrowser == false)
    {
        File loadedFile;
        
        if (openBrowser == true)
            loadedFile = loadFileChooser.getResult();
        else
            loadedFile = fileToOpen;
        
        //read from file and put data into a MidiFile object
        FileInputStream inputStream(loadedFile);
        MidiFile midiFile;
        midiFile.readFrom(inputStream);
        
        int noOfTicks = midiFile.getTimeFormat();
        
        if (noOfTicks >= 0) //time format = ticks per quarter note
        {
            //get the correct MIDI track based on number of tracks
            int trackToImport = 0;
            if (midiFile.getNumTracks() > 1)
                trackToImport = 1;
            
            std::cout << "Midi file no of tracks: " << midiFile.getNumTracks() << std::endl;
            
            //pointer to midiFile content. Will get deleted when midiFile goes out of scope.
            const MidiMessageSequence *midiSequence (midiFile.getTrack(trackToImport)); 
            //as the MidiMessageSequence object above is a pointer to the midiFile content 
            //and needs to be const, we must create a copy of this object to be used
            //to delete all the non note-on events.
            //the other way would be to do the following above: 
            //MidiMessageSequence *newSequence = new MidiMessageSequence(*midiFile.getTrack(trackToImport));
            //to create a deep copy that could be editted (as it won't be const) and then you would
            //only need the one sequence object. But then you would need
            //to manually delete the object.
            MidiMessageSequence newSequence = *midiSequence;
            
            if (midiSequence != 0) //if it got a track from midiFile
            {
                int noOfEvents = midiSequence->getNumEvents();
                std::cout << noOfEvents << std::endl;
                
                if (noOfEvents > 0)
                {
                    //pointers to midiSequence content. Clear array but don't delete containing objects when finshed
                    OwnedArray <MidiMessageSequence::MidiEventHolder> eventHolder;
                    Array <int> noteNumbers;
                    Array <int> eventsToDelete;
                    
                    //Search through all events, flag any non-note-on events to be deleted,
                    //and add the note numbers of any note-on events to the noteNumbers array.
                    for (int i = 0; i < noOfEvents; i++)
                    {
                        eventHolder.insert (i, midiSequence->getEventPointer(i)); 
                        
                        if (eventHolder[i]->message.isNoteOn() == true)
                        {
                            noteNumbers.addIfNotAlreadyThere(eventHolder[i]->message.getNoteNumber());
                        }
                        else
                        {
                            //can't simply delete the event here as the indexes won't match between 
                            //the MidiMessageSequence objects once an event has been removed
                            eventsToDelete.add(i);
                        }
                    }
                    
                    //delete flagged events
                    for (int i = eventsToDelete.size()-1; i >= 0; i--)
                        newSequence.deleteEvent(eventsToDelete[i], false);
                    
                    //update noOfEvents
                    noOfEvents = newSequence.getNumEvents();
                    
                    //sort noteNumbers array then only keep the first twelve
                    DefaultElementComparator<int> sorter;
                    noteNumbers.sort (sorter);
                    for (int i = noteNumbers.size()-1; i > 11; i--)
                        noteNumbers.remove(i);
                    
                    eventHolder.clear(false);
                    eventsToDelete.clear();
                    
                    //Search through all events and flag any events that don't contain a note
                    //number within the noteNumbers array to be deleted
                    for (int i = 0; i < noOfEvents; i++)
                    {
                        eventHolder.insert (i, newSequence.getEventPointer(i));
                        
                        int eventNoteNumber = eventHolder[i]->message.getNoteNumber();
                        
                        if (noteNumbers.contains(eventNoteNumber) == false)
                        {
                            //can't simply delete the event here as the indexes won't match between 
                            //the MidiMessageSequence objects once an event has been removed
                            eventsToDelete.add(i);
                        }
                    }
                    
                    //delete flagged events
                    for (int i = eventsToDelete.size()-1; i >= 0; i--)
                        newSequence.deleteEvent(eventsToDelete[i], false);
                    
                    //update noOfEvents
                    noOfEvents = newSequence.getNumEvents();
                    
                    //reset sequencer grid points
                    if (isSeqSet == false)
                    {
                        for (int i = 0; i < selectedPads_.size(); i++)
                        {
                            int padNum = selectedPads_[i];
                            for (int row = 0; row <= NO_OF_ROWS-1; row++)
                            {
                                for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
                                    PAD_SETTINGS_pads->setSequencerData(currentlySelectedSeqNumber, row, column, 0, false);
                            }
                        }
                    }
                    else //seq set
                    {
                        for (int i = 0; i < selectedPads_.size(); i++)
                        {
                            int padNum = selectedPads_[i];
                            for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
                            {
                                for (int row = 0; row <= NO_OF_ROWS-1; row++)
                                {
                                    for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
                                        PAD_SETTINGS_pads->setSequencerData(seq, row, column, 0, false);
                                }
                            }
                        }
                    }
                    
                    eventHolder.clear(false);
                    
                    //apply data from newSequence to sequencerData within PadSettings
                    //search through all the events, get the time stamp, noteNumber index, and velocity
                    //or each event, and apply it to the correct sequencerData index.
                    for (int i = 0; i < noOfEvents; i++)
                    {
                        eventHolder.insert (i, newSequence.getEventPointer(i));
                        //event message time stamp is used to work oout the column number
                        const int noteColumn = roundToInt((eventHolder[i]->message.getTimeStamp())/(noOfTicks/4.0));
                        //event message note number index in the noteNumber array is used to set the row number
                        const int noteRow = noteNumbers.indexOf(eventHolder[i]->message.getNoteNumber());
                        //event message velocity sets the value of the grid point
                        const int noteValue = eventHolder[i]->message.getVelocity();
                        
                        for (int i = 0; i < selectedPads_.size(); i++)
                        {
                            int padNum = selectedPads_[i];
                            
                            int sequenceLength = PAD_SETTINGS_pads->getSequencerLength();
                            int seqNumber = noteColumn/sequenceLength;
                            
                            //single sequence - apply events to just the currently selected sequence
                            if (isSeqSet == false)
                            {
                                if (noteColumn < sequenceLength)
                                {
                                    PAD_SETTINGS_pads->setSequencerData(currentlySelectedSeqNumber, noteRow, noteColumn, noteValue, false);
                                }
                            }
                            //sequence set - apply to all sequences, splitting the midi sequence after 32 columns each time
                            else if (isSeqSet == true)
                            {
                                if (noteColumn < (sequenceLength * 8)) //why can't I use NO_OF_SEQS? Equals a random number
                                {
                                    int newNoteColumn = noteColumn - (sequenceLength * seqNumber);
                                    PAD_SETTINGS_pads->setSequencerData(seqNumber, noteRow, newNoteColumn, noteValue, false);
                                    
                                    //where can i set this so it isn't called mutiple times? Current way is lazy...
                                    PAD_SETTINGS_pads->setSequencerNumberOfSequences(seqNumber+1);
                                    //std::cout << "Number of seqs: " << seqNumber+1 << std::endl;
                                }
                            }
                        }
                    }
                    
                    eventHolder.clear(false);
                    
                    //convert new sequence data to strings
                    for (int i = 0; i < selectedPads_.size(); i++)
                    {
                        int padNum = selectedPads_[i];
                        PAD_SETTINGS_pads->seqDataToString();
                    }
                    
                    //if needed, apply the noteNumbers array to the pads midi note array
                    if (shouldImportNoteData == true)
                    {
                        for (int i = 0; i < selectedPads_.size(); i++)
                        {
                            int padNum = selectedPads_[i];
                            for(int index = 0; index < noteNumbers.size(); index++)
                            {
                                PAD_SETTINGS_pads->setSequencerMidiNote(noteNumbers[index], index);
                            }
                        }
                    }
                }
            }
        }
        else //time format = SMPTE
        {
            AlertWindow::showMessageBox(AlertWindow::InfoIcon, translate("Cannot Import MIDI File!"), translate("AlphaLive cannot import MIDI files that use the SMPTE time format."));
        }
    }
	
}





void AppDocumentState::removeUneededAudioFiles (bool closingApp)
{
    
    if (currentProjectFile != File::nonexistent) //if there is currently an open project
    {
        bool shouldCleanUp = true;
        
        if (!closingApp)
        {
            shouldCleanUp = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, translate("Clean Up Project"), translate("This command will go through the current projects Audio Files directory and delete any files which aren't currently being used. Over time this will prevent an excessive build-up of redundant data. It was also reset any unused mode settings to default values. Please note that you can not undo this command!"));
        }
        
        if (shouldCleanUp == true)
        {
            //this function must check all the settings of all the sceneData elements,
            //and delete any audio files in the project Audio Files directory that aren't included in these settings.
            //need to move all the currently used audio files somewhere, delete any that are left, and move the used ones back
            //Is is the best way to do it?
            
            //first must save the current settings into the current scene to prevent missing audio files errors once the clean up has been completed.
            //For example, if you imported in a new audio file and then instantly cleaned up without saving the new settings it might delete the current
            //audio file as a reference to it wouldn't be found in any of the sceneData elements, so when the clean up is complete the audio file would 
            //now be missing.
            //instead of saving, you could load up the scene data for the current scene which would delete the current settings that havent been saved. What would be more natural?
            if (!closingApp)
                saveToScene(currentlySelectedScene);
            
            File tempAudioDirectory = File::getCurrentWorkingDirectory().getParentDirectory().getFullPathName() + File::separatorString + "tempDir";
            tempAudioDirectory.createDirectory();
            
            //search through all scenes
            for (int sceneNumber = 0; sceneNumber <= NO_OF_SCENES-1; sceneNumber++)
            {
                
                if (sceneData[sceneNumber]->getNumChildElements() > 0) //if the scene XmlElement contains data
                {
                    
                    //search through all pads looking for audio files 
                    for (int i = 0; i <= 47; i++)
                    {
                        //look for sampler audio files
                        if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                        {
                            String originalFile(sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("samplerAudioFilePath"));
                            
                            if (originalFile != String::empty) //if there is 'something'
                            {
                                File audioFile(File::nonexistent);
                                
                                //Look for internal files, which are saved just by just their file name
                                
                                if (File::isAbsolutePath(originalFile) == false)
                                {
                                    //File should be an internal file
                                    audioFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + originalFile;
                                    
                                    if (audioFile.existsAsFile() == true)
                                    {
                                        File newFile = tempAudioDirectory.getFullPathName()+ File::separatorString + originalFile;
                                        if (newFile.existsAsFile() == false) //if it doesn't already exist
                                        {
                                            audioFile.copyFileTo(newFile);//copy the file
                                        }
                                    }
                                    
                                }
                                else
                                {
                                    //external file.
                                    //check to see if the external file is actually stored internally,
                                    //which could be caused by importing files to project, 
                                    //and then selecting 'don't copy' afterwards.
                                    
                                    audioFile = originalFile;
                                    
                                    if (audioFile.isAChildOf(File::getCurrentWorkingDirectory()) == true)
                                    {
                                        originalFile = audioFile.getFileName();
                                        File newFile = tempAudioDirectory.getFullPathName()+ File::separatorString + originalFile;
                                        if (newFile.existsAsFile() == false) //if it doesn't already exist
                                        {
                                            audioFile.copyFileTo(newFile);//copy the file
                                        } 
                                        
                                        //would it be a good idea to 'fix' the saved file path here so that the file
                                        //is now recignised as being internal instead of external?
                                    }
                                }
                            }
                        }
                        //look for sequencer audio files
                        else if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                        {
                            for (int row = 0; row <= NO_OF_ROWS-1; row++)
                            {
                                String originalFile(sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row)));
                                
                                if (originalFile != String::empty) //if there is 'something'
                                {
                                    File audioFile(File::nonexistent);
                                    
                                    //Look for internal files, which are saved just by just their file name
                                    
                                    if (File::isAbsolutePath(originalFile) == false)
                                    {
                                        //File should be an internal file
                                        audioFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + originalFile;
                                        
                                        if (audioFile.existsAsFile() == true)
                                        {
                                            File newFile = tempAudioDirectory.getFullPathName()+ File::separatorString + originalFile;
                                            if (newFile.existsAsFile() == false) //if it doesn't already exist
                                            {
                                                audioFile.copyFileTo(newFile);//copy the file
                                            }
                                        }
                                        
                                    }
                                    else
                                    {
                                        //external file.
                                        //check to see if the external file is actually stored internally,
                                        //which could be caused by importing files to project, 
                                        //and then selecting 'don't copy' afterwards.
                                        
                                        audioFile = originalFile;
                                        
                                        if (audioFile.isAChildOf(File::getCurrentWorkingDirectory()) == true)
                                        {
                                            originalFile = audioFile.getFileName();
                                            File newFile = tempAudioDirectory.getFullPathName()+ File::separatorString + originalFile;
                                            if (newFile.existsAsFile() == false) //if it doesn't already exist
                                            {
                                                audioFile.copyFileTo(newFile);//copy the file
                                            } 
                                            
                                            //would it be a good idea to 'fix' the saved file path here so that the file
                                            //is now recignised as being internal instead of external?
                                        }
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }
            //create a copy of the working dir (Audio Files dir)
            File audioFileDirectory = File::getCurrentWorkingDirectory();
            //delete the current working dir
            File::getCurrentWorkingDirectory().deleteRecursively();
            //rename the temp dir (which should hold all the needed audio files) to 'Audio Files' so it can be used as the working dir when the project is next loaded up
            tempAudioDirectory.moveFileTo(audioFileDirectory);
            
            if (!closingApp)
            {
                //set the currentWorkingDirectory
                audioFileDirectory.setAsCurrentWorkingDirectory();
                
                //get number of included audio files (for use at close when auto cleaning project)
                numOfFilesAtStart = File::getCurrentWorkingDirectory().getNumberOfChildFiles(2);
                
                // reset unused mode settings
                
                for (int i = 0; i <=47; i++)
                {
                    PAD_SETTINGS->resetData(PAD_SETTINGS->getMode());
                }
                
                //automatically save the new settings
                shouldDisplayAlertWindow = false; // << is this needed anymore?
                saveProject();
                
                
                AlertWindow::showMessageBox(AlertWindow::InfoIcon, translate("Project Cleaned Up!"), translate("All redundant files and settings have been deleted and reset."));
            }
        }
    }
    
    else
    {
        if (!closingApp)
        {
            AlertWindow::showMessageBox(AlertWindow::InfoIcon, translate("No project currently open!"), translate("There is no project open to clean up."));
        }
    }
}


void AppDocumentState::importAudioFiles()
{
    //==============================================================================================
    //this function is called if copyExternalFilesSwitch is set to on/true
    //==============================================================================================
    
    saveToScene(currentlySelectedScene);
    
    //search through all scenes
    for (int sceneNumber = 0; sceneNumber <= NO_OF_SCENES-1; sceneNumber++)
    {
        if (sceneData[sceneNumber]->getNumChildElements() > 0) //if the scene XmlElement contains data
        {
            //search through all pads looking for audio files 
            for (int i = 0; i <= 47; i++)
            {
                //look for sampler audio files
                if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 2)
                {
                    File currentFile;
                    File newFile;
                    String fileName = sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("samplerAudioFilePath");
                    
                    if (fileName != String::empty)
                    {
                        //Here, we are looking for external files. If we find one, we need to copy it
                        if (File::isAbsolutePath(fileName) == true)
                        {
                            //if we are looking for external files and the pads file is an external file...
                            currentFile = fileName;
                        }
                        else
                        {
                            //no match...
                            break;
                        }
                        
                        newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + currentFile.getFileName();
                        
                        if (newFile.existsAsFile() == false) //if it doesn't yet exist
                        {
                            //copy the added audio file into the newly created file
                            currentFile.copyFileTo(newFile);
                        }
                        
                        else if (newFile.existsAsFile() == true) //if it already exists 
                        {
                            Array<File> matchingFilesArray;
                            String fileWildCard (newFile.getFileNameWithoutExtension()+"*");
                            bool importedFileNeedsCopying = true;
                            
                            //Find all possible duplicates of the imported file using fileWildCard
                            //and add reference of all possible files to matchingFilesArray.
                            File::getCurrentWorkingDirectory().findChildFiles(matchingFilesArray, 2, false, fileWildCard);
                            
                            for (int i = 0; i < matchingFilesArray.size(); i++)
                            {
                                if (currentFile.hasIdenticalContentTo(matchingFilesArray[i]) == true)
                                {
                                    //if it finds a duplicate, flag that the file doesn't need copying
                                    importedFileNeedsCopying = false;
                                    //set the file
                                    newFile = matchingFilesArray[i];
                                    //break out of the for loop once a duplicate has been found to prevent unnecessary checks
                                    break;
                                }
                            }
                            
                            if (importedFileNeedsCopying == true) 
                            {
                                //if no duplicate was found...
                                //... copy the added file with an appended name
                                newFile = newFile.getNonexistentSibling();
                                currentFile.copyFileTo(newFile); 
                            }
                            
                        }
                        
                        
                        sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("samplerAudioFilePath", newFile.getFileName());
                        
                        if (sceneNumber == currentlySelectedScene)
                            AppSettings::Instance()->padSettings[i]->setSamplerAudioFilePath(newFile);
                    }
                }
                
                //look for sequencer audio files
                else if (sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getIntAttribute("mode") == 3)
                {
                    for (int row = 0; row <= NO_OF_ROWS-1; row++)
                    {
                        File currentFile;
                        File newFile;
                        String fileName = sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->getStringAttribute("sequencerSamplesAudioFilePath"+String(row));
                        
                        if (fileName != String::empty)
                        {
                            //Here, we are looking for external files. If we find one, we need to copy it
                            if (File::isAbsolutePath(fileName) == true)
                            {
                                //if we are looking for external files and the pads file is an external file...
                                currentFile = fileName;
                            }
                            else
                            {
                                //no match...
                                break;
                            }
                            
                            newFile = File::getCurrentWorkingDirectory().getFullPathName() + File::separatorString + currentFile.getFileName();
                            
                            if (newFile.existsAsFile() == false) //if it doesn't yet exist
                            {
                                //copy the added audio file into the newly created file
                                currentFile.copyFileTo(newFile);
                            }
                            
                            else if (newFile.existsAsFile() == true) //if it already exists 
                            {
                                Array<File> matchingFilesArray;
                                String fileWildCard (newFile.getFileNameWithoutExtension()+"*");
                                bool importedFileNeedsCopying = true;
                                
                                //Find all possible duplicates of the imported file using fileWildCard
                                //and add reference of all possible files to matchingFilesArray.
                                File::getCurrentWorkingDirectory().findChildFiles(matchingFilesArray, 2, false, fileWildCard);
                                
                                for (int i = 0; i < matchingFilesArray.size(); i++)
                                {
                                    if (currentFile.hasIdenticalContentTo(matchingFilesArray[i]) == true)
                                    {
                                        //if it finds a duplicate, flag that the file doesn't need copying
                                        importedFileNeedsCopying = false;
                                        //set the file
                                        newFile = matchingFilesArray[i];
                                        //break out of the for loop once a duplicate has been found to prevent unnecessary checks
                                        break;
                                    }
                                }
                                
                                if (importedFileNeedsCopying == true) 
                                {
                                    //if no duplicate was found...
                                    //... copy the added file with an appended name
                                    newFile = newFile.getNonexistentSibling();
                                    currentFile.copyFileTo(newFile); 
                                }
                                
                            }
                            
                            
                            sceneData[sceneNumber]->getChildByName("PAD_DATA_"+String(i))->setAttribute("sequencerSamplesAudioFilePath"+String(row), newFile.getFileName());
                            
                            if (sceneNumber == currentlySelectedScene)
                                AppSettings::Instance()->padSettings[i]->setSequencerSamplesAudioFilePath(newFile, row);
                        }
                    }
                }
            }
        }
    }
    
    //loadFromScene(currentlySelectedScene);
}

void AppDocumentState::registerRecentFile (const File& file)
{
    RecentlyOpenedFilesList::registerRecentFileNatively (file);
    StoredSettings::getInstance()->recentFiles.addFile (file);
    StoredSettings::getInstance()->flush();
}


void AppDocumentState::setCurrentlySelectedScene(int value)
{
    currentlySelectedScene = value;
    std::cout << "Currently Selected Scene = " << currentlySelectedScene << std::endl;
}


int AppDocumentState::getGuiUpdateFlag()
{
    return guiUpdateFlag;
}
int AppDocumentState::getSceneToUpdate()
{
    return sceneToUpdate;
}
int AppDocumentState::getSceneStatus()
{
    return sceneStatus;
}

int AppDocumentState::getNumOfFilesAtStart()
{
    return numOfFilesAtStart;
}
