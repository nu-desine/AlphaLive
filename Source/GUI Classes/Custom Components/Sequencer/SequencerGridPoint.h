//
//  SequencerGridPoint.h
//  sdaJuce
//
//  Created by Liam Meredith-Lacey on 14/10/2011.
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
#ifndef H_SEQUENCERGRIDPOINT
#define H_SEQUENCERGRIDPOINT

#include "../../../../JuceLibraryCode/JuceHeader.h"

class SequencerGrid;

class SequencerGridPoint  :     public Component
{	
public:
	//==============================================================================
	SequencerGridPoint(int rowNumber, int columnNumber, SequencerGrid &ref); 
	~SequencerGridPoint();
	//==============================================================================
	void resized();
    void paint (Graphics &g);
    void mouseDown (const MouseEvent &e);
    void mouseDrag (const MouseEvent &e);
    void mouseUp (const MouseEvent &e);
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    int getStatus();
    void setStatus (int value);
    
    bool hitTest (int x, int y);
    
	//==============================================================================
private:
	//==============================================================================
	//Add data members here
    
    int status; //this value is now in the range 0-127 which refers to the note velocity. 0 is 'note-off'
    int row, column;
    SequencerGrid &parent;
    
    bool shouldChangeVelocity; //true when an alt-click to change velocity has been performed
    int initialVelocity;

    Path thePath;
	float startRadians, endRadians, theWidth;
    
};



#endif // H_SEQUENCERGRIDPOINT