//
//  SequencerGrid.h
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

#ifndef H_SEQUENCERGRID
#define H_SEQUENCERGRID

#include "../JuceLibraryCode/JuceHeader.h"
#include "SequencerGridPoint.h"
#include "SequencerGridPlayHead.h"
#include "SequencerValues.h"
#include "AbstractSubjectAndObserver.h"
#include "ModeSequencer.h"
#include "SequencerGridEndBlock.h"

class GuiSequencerMode;

class SequencerGrid  :      public Component,
                            public Observer //so this class can 'observe' modeSequencer
{	
public:
	//==============================================================================
	SequencerGrid(ModeSequencer &ref, GuiSequencerMode &ref2);
	~SequencerGrid();
	//==============================================================================
	void resized();
    void paint (Graphics &g);
    void gridClicked (int row, int column, int status);
    void clear();
    void clearAll();
    void resetGridGui(); //clears GUI but not sequencerData
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void setCurrentSequenceNumber (int value);
    void setSequencerData (int seqNumber, int rowNumber, int columnNumber, int status);
    
    void setPlayHeadPos (float position);
    
    //override the Observer virtual update function
    bool update(const Subject& theChangedSubject);
    
    void setVelocityLabelText (String velocity);
    
	//==============================================================================
private:
	//==============================================================================
	//array that holds the grid point components
    SequencerGridPoint *gridPoint[NO_OF_ROWS][NO_OF_COLUMNS];
    
    SequencerGridPlayHead *playHead;
    
    //array that holds the status data for each point (of all sequences in one sequencer)
    //do we really need this? should we just read direct from padsettings instead of using this
    int sequencerData[NO_OF_SEQS][NO_OF_ROWS][NO_OF_COLUMNS];
    
    int currentSequenceNumber;
    //int currentlySelectedPad;
    Array<int> selectedPads;
    
    //create a reference to the subject that you want this class to observe
    ModeSequencer &mSubject;
    
    Image backgroundImage;
    
    //this needs to be a seperate component and not just a grey rectangle drawn in this component
    //as it allows no interaction with seq grid point components 'underneath' the rectangle.
    //I'm sure there are other ways of setting a region of a component to not respond to mouse clicks
    //however this seems like the most straightforward way.
    SequencerGridEndBlock *endBlock;
    
    int lastTime;
    
    GuiSequencerMode &guiSequencerMode;
        	
};



#endif // H_SEQUENCERGRID