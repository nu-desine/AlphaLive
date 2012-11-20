//
//  EliteControlsComponent.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 17/09/2012.
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

#ifndef H_ELITECOMPONENTSETTINGS
#define H_ELITECOMPONENTSETTINGS

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Custom Components/General/ModeButton.h"
#include "../Custom Components/General/AlphaSquareButton.h"


class MainComponent;
class EliteControlsComponent :  public Component,
                                public Button::Listener

{
public:
	
	EliteControlsComponent(MainComponent &ref);
    ~EliteControlsComponent();
    
    void resized();
	void paint (Graphics& g);
    
    void buttonClicked (Button* button);
	void turnOffButtons();
    //void updateDisplay();
	
	void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
	
private:
    
    MainComponent &mainComponentRef;
    ScopedPointer <ModeButton> dial1Button, dial2Button;
	ScopedPointer <AlphaSquareButton> button1Button, button2Button, button3Button;
	
};

#endif 