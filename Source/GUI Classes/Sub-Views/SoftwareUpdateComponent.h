//
//  SoftwareUpdateComponent.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 20/03/2013.
//  Copyright 2013 nu desine.
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

#ifndef H_SOFTWAREUPDATECOMPONENT
#define H_SOFTWAREUPDATECOMPONENT

class MainComponent;

#include "../../../JuceLibraryCode/JuceHeader.h"

class SoftwareUpdateComponent :     public Component,
                                    public Thread,
                                    public AsyncUpdater
{
public:
    SoftwareUpdateComponent (MainComponent &ref);
    ~SoftwareUpdateComponent();
    
    void resized();
    void paint (Graphics& g);
    
    void run();
    void handleAsyncUpdate();
    
private:
    
    MainComponent &mainComponentRef;
    
    File alphaLiveDirectory;
    
    ScopedPointer <Label> infoLabel;
    
    ScopedPointer <ProgressBar> progressBar;
    double progress;
};

#endif // H_SOFTWAREUPDATECOMPONENT