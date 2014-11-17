//
//  AppSettings.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 22/09/2011.
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

#ifndef H_ELITEDIALDATA
#define H_ELITEDIALDATA

#include "../../JuceLibraryCode/JuceHeader.h"

struct EliteDialData 
{
    double prevValue;
    int status;
    int control;
    
    int midiControllerType; //0 = absolute, 1 = relative
    int midiCcNumber;
    int midiChannel;
    int midiMinRange;
    int midiMaxRange;
    
    int oscPortNumber;
    String oscIpAddress;
    double oscMinRange;
    double oscMaxRange;
    double oscStepValue;
    
};

#endif

#ifndef H_ELITEBUTTONDATA
#define H_ELITEBUTTONDATA

struct EliteButtonData 
{
    int status;
    int control;
    
    int sceneMode;
    int sceneNumber;
    
    int midiCcNumber;
    int midiChannel;
    int midiOffNumber;
    int midiOnNumber;
    
    int oscPortNumber;
    String oscIpAddress;
    double oscOffNumber;
    double oscOnNumber;
    
};

#endif

#ifndef H_APPSETTINGS
#define H_APPSETTINGS

//==================================================================
/**
 Warning: this is a Singlton class!
 Code adapted from http://www.informit.com/guides/content.aspx?g=cplusplus&seqNum=148
 (with a breif look at and play with http://www.daniweb.com/software-development/cpp/threads/58955
 and http://www.bombaydigital.com/arenared/2005/10/25/1 )
 This class is used to store and retrieve all the application settings.
 */

#include "../../JuceLibraryCode/JuceHeader.h"
#include "PadSettings.h"
#include "../Functionality Classes/AlphaLiveEngine.h"

class AppSettings
{
public:
    //==================================================================
    
    // Implement the logic here to instantiate the class for the first time by validating the
    // member pointer. If member pointer is already pointing to some valid memory it means
    // that the first object is created and it should not allow for the next instantiation
    // so simply return the member pointer without calling its ctor.
    static AppSettings* Instance();
    //==================================================================

    void setAlphaLiveEngineRef (AlphaLiveEngine *ref);
    void resetData();
    void resetProjectSettingsData();
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    
    void setGlobalPan (float value);
    void setGlobalGain (float value);
    
    void setGlobalTempo (float value);
    void setQuantizationValue (int value);
    void setBeatsPerBar (int value);
    void setAutoStartClock (int value);
    void setMetronomeStatus (bool value);
    
    void setCopyExternalFiles (bool value);
    void setMidiClockValue (int value);
    void setMidiClockStartMessage (int value);
    void setMidiClockMessageFilter (int value);
    
    void setReceiveMidiProgramChangeMessages (bool value);

    void setSceneName (int sceneNumber, String value);
    void setHardwareLedMode (int value);
    void setHardwareLedStatus (int value);
    void setHardwareLedPressureStatus (int value);
    void setHardwareLedClockStatus (int value);
    
    void setDisablePressureFeedback (bool value);
    
    Array<int> getCurrentlySelectedPad();
    
    float getGlobalPan();
    float getGlobalGain();
    
    float getGlobalTempo();
    int getQuantizationValue();
    int getBeatsPerBar();
    int getAutoStartClock();
    bool getMetronomeStatus();
    
    bool getCopyExternalFiles();
    int getMidiClockValue();
    int getMidiClockStartMessage();
    int getMidiClockMessageFilter();
    
    bool getReceiveMidiProgramChangeMessages();
    
    String getSceneName(int sceneNumber);
    int getHardwareLedMode();
    int getHardwareLedStatus();
    int getHardwareLedPressureStatus();
    int getHardwareLedClockStatus();
    
    bool getDisablePressureFeedback();
    
    //Elite Controls Stuff
    void setEliteDialPrevValue(double value, int dialNumber);
    void setEliteDialControl(int value, int dialNumber);
    void setEliteDialStatus(int value, int dialNumber);
    void setEliteDialMidiControllerType(int value, int dialNumber);
    void setEliteDialMidiCcNumber(int value, int dialNumber);
    void setEliteDialMidiChannel(int value, int dialNumber);
    void setEliteDialMidiMinRange(int value, int dialNumber);
    void setEliteDialMidiMaxRange(int value, int dialNumber);
    void setEliteDialOscPortNumber(int value, int dialNumber);
    void setEliteDialOscIpAddress(String value, int dialNumber);
    void setEliteDialOscMinRange(double value, int dialNumber);
    void setEliteDialOscMaxRange(double value, int dialNumber);
    void setEliteDialOscStepValue(double value, int dialNumber);
    
    double getEliteDialPrevValue(int dialNumber);
    int getEliteDialControl(int dialNumber);
    int getEliteDialStatus(int dialNumber);
    int getEliteDialMidiControllerType(int dialNumber);
    int getEliteDialMidiCcNumber(int dialNumber);
    int getEliteDialMidiChannel(int dialNumber);
    int getEliteDialMidiMinRange(int dialNumber);
    int getEliteDialMidiMaxRange(int dialNumber);
    int getEliteDialOscPortNumber(int dialNumber);
    String getEliteDialOscIpAddress(int dialNumber);
    double getEliteDialOscMinRange(int dialNumber);
    double getEliteDialOscMaxRange(int dialNumber);
    double getEliteDialOscStepValue(int dialNumber);
    
    void setEliteButtonControl(int value, int buttonNumber);
    void setEliteButtonStatus(int value, int buttonNumber);
    void setEliteButtonSceneMode(int value, int buttonNumber);
    void setEliteButtonSceneNumber(int value, int buttonNumber);
    void setEliteButtonMidiCcNumber(int value, int buttonNumber);
    void setEliteButtonMidiChannel(int value, int buttonNumber);
    void setEliteButtonMidiOffNumber(int value, int buttonNumber);
    void setEliteButtonMidiOnNumber(int value, int buttonNumber);
    void setEliteButtonOscPortNumber(int value, int buttonNumber);
    void setEliteButtonOscIpAddress(String value, int buttonNumber);
    void setEliteButtonOscOffNumber(double value, int buttonNumber);
    void setEliteButtonOscOnNumber(double value, int buttonNumber);
    
    int getEliteButtonControl (int buttonNumber);
    int getEliteButtonStatus(int buttonNumber);
    int getEliteButtonSceneMode(int buttonNumber);
    int getEliteButtonSceneNumber(int buttonNumber);
    int getEliteButtonMidiCcNumber(int buttonNumber);
    int getEliteButtonMidiChannel(int buttonNumber);
    int getEliteButtonMidiOffNumber(int buttonNumber);
    int getEliteButtonMidiOnNumber(int buttonNumber);
    int getEliteButtonOscPortNumber(int buttonNumber);
    String getEliteButtonOscIpAddress(int buttonNumber);
    double getEliteButtonOscOffNumber(int buttonNumber);
    double getEliteButtonOscOnNumber(int buttonNumber);
    
    
    //would be nice if this could be private, but without having to access every 
    //PadSettings method through another individual method in this class
    OwnedArray<PadSettings> padSettings;
    
    //functions for the copy/paste pad settings feature
    void copyPadSettings (int padNumber);
    void pastePadSettings (int padNumber);
    
    void copySequenceData (int value, int row, int column);
    int pasteSequenceData (int row, int column);
    
    void setLastAudioSampleDirectory (File directory);
    File getLastAudioSampleDirectory ();
    
    
    //~AppSettings();
protected:
    //==================================================================
    // Default ctor should be as protected so that nobody
    // from outside can call those functions and instantiate it
    // No destructor
    AppSettings();
    //==================================================================
    
private:
    //==================================================================
    // A member pointer to itself which point to the firstly created object and this should be
    // returned if another object is instantiated that means the second object what you
    // thought of created is nothind but the first instance
    static AppSettings* pInstance;
    //==================================================================
    AlphaLiveEngine *alphaLiveEngineRef;
    
    //int currentlySelectedPad;
    Array<int> selectedPads;
    
    float globalPan;
    float globalGain;
    
    float globalTempo;
    int quantizationValue; //value represents an item in the GUI quantization menu
    int beatsPerBar; //similar to time signature
    int autoStartClock; //when 1, clock will start when the first midi/loop/seq pad is pressed.
    bool metronomeStatus;
    
    //Elite Controls stuff
    EliteDialData eliteDial[2];
    EliteButtonData eliteButton[3];
    
    //variable used to store copied pad settings
    ScopedPointer<PadSettings> copiedPadSettings;
    
    int copiedSequencerData[NO_OF_ROWS][NO_OF_COLUMNS]; //[row][column]
    
    File lastAudioSampleDirectory; //this variable isn't ever saved or reset
    
    //========================================================================================================
    //variables which are global to the whole project (project settings) and aren't changeable between individual scenes.
    //Would it make more sense to not have them in AppSettings?
    //========================================================================================================
    
    bool copyExternalFiles;
    int midiClockValue, midiClockStartMessage, midiClockMessageFilter;
    bool receiveMidiProgramChangeMessages;
    
    int hardwareLedMode;            // 0 - normal, 1 - MIDI CC controlled
    int hardwareLedStatus;          // 0 - off, 1 - on
    int hardwareLedPressureStatus;  // 0 - off, 1 - on
    int hardwareLedClockStatus;     // 0 - off, 1 - on

    String sceneName[20];
    
    bool disablePressureFeedback;
    
};

#endif // H_APPSETTINGS
