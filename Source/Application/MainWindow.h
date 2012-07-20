/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

//  Created by Liam Meredith-Lacey on 14/09/2011.
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

#ifndef __MAINWINDOW_H_6D70D716__
#define __MAINWINDOW_H_6D70D716__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Gui Classes/Views/MainComponent.h"
#include "../Functionality Classes/AlphaLiveEngine.h"
#include "../File and Settings/AppDocumentState.h"



//==============================================================================
class MainAppWindow   : public DocumentWindow
                        //public ApplicationCommandTarget
{
public:
    //==============================================================================
    MainAppWindow(AlphaLiveEngine &ref, AppDocumentState &ref2);
    ~MainAppWindow();

    void closeButtonPressed();
    
    void setTitleBarText (String projectName);
    
    MainComponent* getMainComponent();


    //==============================================================================
    /*
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);
     */

private:
    //==============================================================================
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)
    
    MainComponent *mainComponent;
    AlphaLiveEngine &alphaLiveEngineRef;
    AppDocumentState &appDocumentStateRef;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)
};


#endif  // __MAINWINDOW_H_6D70D716__