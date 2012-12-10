//
//  AboutComponent.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 22/05/2012.
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

#ifndef H_ABOUTCOMPONENT
#define H_ABOUTCOMPONENT

#include "../../../JuceLibraryCode/JuceHeader.h"

class MainComponent;

class AboutComponent :  public Component,
                        public Button::Listener,
                        public KeyListener
{
public:
    AboutComponent(MainComponent &ref);
    ~AboutComponent();
    
    void resized();
    void paint (Graphics& g);
    
    void buttonClicked (Button *button);
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    bool keyPressed (const KeyPress &key, Component *originatingComponent);
    
private:
    MainComponent &mainComponentRef;
    TextButton *closeButton;
    Label *titleLabel, *versionNumberLabel, *companyLabel, *openSourceLabel, *copyrightLabel, *samplesLabel;
    HyperlinkButton *nuDesineLink, *gnuLicenseLink, *githubLink;
    ImageButton *loopmastersButton, *zeroGButton, *fivePinMediaButton;
    
};

#endif
