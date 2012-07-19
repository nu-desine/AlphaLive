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
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void setPadDisplayTextMode (int value);
    
    void setGlobalPan (float value);
    void setGlobalGain (float value);
    
    void setGlobalTempo (float value);
    void setQuantisationValue (int value);
    void setBeatsPerBar (int value);
    void setAutoStartClock (int value);
    
    void setLooperChannelMode (int channelNumber, int value);
    void setSequencerChannelMode (int channelNumber, int value);
    
    void setCopyExternalFiles (bool value);
    
    Array<int> getCurrentlySelectedPad();
    int getPadDisplayTextMode();
    
    float getGlobalPan();
    float getGlobalGain();
    
    float getGlobalTempo();
    int getQuantisationValue();
    int getBeatsPerBar();
    int getAutoStartClock();
    
    int getLooperChannelMode (int channelNumber);
    int getSequencerChannelMode (int channelNumber);
    
    bool getCopyExternalFiles();
    
    
    //would be nice if this could be private, but without having to access every 
    //PadSettings method through another individual method in this class
    OwnedArray<PadSettings> padSettings;
    
    //functions for the copy/paste pad settings feature
    void copyPadSettings (int padNumber);
    void pastePadSettings (int padNumber);
    
    
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
    int padDisplayTextMode; //1 is pad numbers, 2 is pad contents. More will be added!
    
    float globalPan;
    float globalGain;
    
    float globalTempo;
    int quantisationValue; //value represents an item in the GUI quantisation menu
    int beatsPerBar; //similar to time signature
    int autoStartClock; //when 1, clock will start when the first loop/seq pad is pressed. First pad atall?
    
    
    //THIS STUFF is no longer needed.
    //global looper mode
    int looperChannelMode[6];
    //global sequencer mode
    int sequencerChannelMode[6];
    
    //variable used to store copied pad settings
    ScopedPointer<PadSettings> copiedPadSettings;
    
    //========================================================================================================
    //variables which are global to the whole project and aren't changeable between individual presets.
    //Would it make more sense to not have them in AppSettings?
    //========================================================================================================
    
    bool copyExternalFiles;
    
    
};

#endif // H_APPSETTINGS
