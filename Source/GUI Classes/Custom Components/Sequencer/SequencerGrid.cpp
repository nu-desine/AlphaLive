//
//  SequencerGrid.cpp
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

#include "SequencerGrid.h"
#include "../../../File and Settings/AppSettings.h"
#include "../../Views/GuiSequencerMode.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)




SequencerGrid::SequencerGrid(ModeSequencer &ref, GuiSequencerMode &ref2)
                                        :   mSubject(ref),
                                            guiSequencerMode(ref2)
{
    //backgroundImage = ImageFileFormat::loadFrom(SequencerBinaryData::seqgridbg_png, SequencerBinaryData::seqgridbg_pngSize);
    
    //init playHead
    addAndMakeVisible(playHead = new SequencerGridPlayHead());
    
    theAngle = 0;
    
    //init all grid points
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
        {
            addAndMakeVisible(gridPoint[row][column] = new SequencerGridPoint(row, column, *this));
    
        }
    }
    
    //init all indexes of the sequencerData 3D array to 0.
    for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
    {
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
            {
                sequencerData[seq][row][column] = 0;
            }
        }
    }
    
    //addAndMakeVisible(endBlock = new SequencerGridEndBlock());
    
    currentSequenceNumber = 1;
    
    //attach this class to the subject class
    mSubject.attach(this);
    
}

SequencerGrid::~SequencerGrid()
{
    deleteAllChildren();
    
    //detach this class from the subject class
    mSubject.detach(this);
}

void SequencerGrid::resized()
{
    playHead->setBounds(0, 0, getWidth(), getHeight());
    
	for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
		int gaps = ((NO_OF_ROWS-1) - row) * 9;
		
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
        {
            gridPoint[row][column]->setBounds(gaps, gaps, (getWidth() - (gaps*2)), (getHeight()- (gaps*2)));
        }
    }
}

void SequencerGrid::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    
    //if it is a single pad, get the padsettings seq data array and put it into the local array.
    //then display the first one
    //if it is a row or 'all' pads, 
    
    //if individual pad number is selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
        {
            for (int row = 0; row <= NO_OF_ROWS-1; row++)
            {
                for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
                {
                    sequencerData[seq][row][column] = PAD_SETTINGS->getSequencerData(seq, row, column);
                }
            }
        }
        
    }
    
    
    else if (MULTI_PADS)
    {
        for (int i = 0; i <= 47; i++)
        {
            for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
            {
                for (int row = 0; row <= NO_OF_ROWS-1; row++)
                {
                    for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
                    {
                        sequencerData[seq][row][column] = 0;
                    }
                }
            }

            
        }
    }
    
    
    //setCurrentSequenceNumber(1);

}

void SequencerGrid::setSequencerData (int seqNumber, int rowNumber, int columnNumber, int status)
{
    //is this function not needed?
    
}


//this function gets called everytime a gridPoint is 'created' or 'deleted'
void SequencerGrid::gridClicked (int row, int column, int status)
{
    //std::cout << currentSequenceNumber-1 << " " << row << " " << column << " " << status << std::endl;
    
    //PAD_SETTINGS->setSequencerData(currentSequenceNumber-1, row, column, status);
    //sequencerData[currentSequenceNumber-1][row][column] = status;
    
    //NEW STUFF
    
    for (int i = 0; i < selectedPads.size(); i++)
    {
        int padNum = selectedPads[i];
        PAD_SETTINGS->setSequencerData(currentSequenceNumber-1, row, column, status);
    }
    
    //set the local sequencer data array
    sequencerData[currentSequenceNumber-1][row][column] = status;


    
    
}


void SequencerGrid::paint (Graphics &g)
{
    //draw background
    //g.setColour(Colours::white);
    //g.fillRect(0, 0, getWidth(), getHeight());
    //g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
    
	//Rectangle<int> rect = g.getClipBounds();
    //std::cout << "Painting sequencer: " << rect.getX() << " " << rect.getY() << " " << rect.getRight() << " " << rect.getBottom() << std::endl;
	
	
    //draw lines
    for (int i = 0, j = 1; i <=SEQ_HORIZONTAL_STEPS; i++)
    {
        //j is used as a counter to different lines after every 1 regular lines
        
        if (j == 1)
            g.setColour(Colours::grey.withAlpha(0.9f));
        else
            g.setColour(Colours::grey.withAlpha(0.3f));
		
		float theAngle = ((2 * M_PI) / (SEQ_HORIZONTAL_STEPS-1)) * i;
		float sinX = sin(theAngle);
		float cosY = -cos(theAngle);
		
		float midRadius = (getWidth() * 0.5) - (9 * (SEQ_VERTICAL_STEPS-1));
		//midRadius = midRadius + (((getWidth() * 0.5) * (1 - theWidth)) * textRadius);
		
		float xCo = (getWidth() * 0.5) + (midRadius * sinX);
		
		float yCo =	(getHeight() * 0.5) + (midRadius * cosY);
		
		float xCo2 = (getWidth() * 0.5) + ((getWidth() * 0.5) * sinX);
		
		float yCo2 = (getHeight() * 0.5) + ((getHeight() * 0.5) * cosY);
        
        g.drawLine(xCo, yCo, xCo2, yCo2, 1.0);
        
        j++;
        if (j == 5)
            j = 1;
    }
    
    //draw circular lines
    /*for (int i = 0, j = 0; i <=SEQ_VERTICAL_STEPS; i++)
     {
     if (j == 0) //as lines are drawn top to bottom, not bottom to top like they should be
     g.setColour(Colours::orange);
     else
     g.setColour(Colours::darkgrey);
     
     g.drawLine(0, (GRID_POINT_H)*float(i), getWidth(), (GRID_POINT_H)*float(i));
     
     j++;
     if (j == 4)
     j = 0;
     }
	 */
	g.setColour(Colours::grey.withAlpha(0.3f));
	
	for (int i = 0, j = 9; i <=SEQ_VERTICAL_STEPS-1; i++)
	{
		int gaps = i * j;
		
		g.drawEllipse(gaps, gaps, (getWidth() - (gaps*2)), (getHeight()- (gaps*2)), 1);
		
	}
	/*
     blockPath.clear();
     g.setColour(Colours::black.withAlpha(0.8f));
     
     theAngle = ((2 * M_PI) / (SEQ_HORIZONTAL_STEPS-1)) * (PAD_SETTINGS->getSequencerLength());
     
     blockPath.addPieSegment(0, 0, getWidth(), getHeight(), theAngle, (2 * M_PI), 0.3f);
     
     g.fillPath(blockPath, getTransform());
	 */
    
    //resize end block
    //endBlock->setBounds(float(PAD_SETTINGS->getSequencerLength()+1)*GRID_POINT_W, 0, getWidth()-(float(PAD_SETTINGS->getSequencerLength()+1)*GRID_POINT_W), getHeight());
    
    
    
    
    /*
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        endBlock->setBounds(float(PAD_SETTINGS->getSequencerLength()+1)*GRID_POINT_W, 0, getWidth()-(float(PAD_SETTINGS->getSequencerLength()+1)*GRID_POINT_W), getHeight());
    }
    else if (MULTI_PADS)
    {
        //draw default end point here!!
        
        //NOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE ME
        //NOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE ME
        //NOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE MENOTICE ME NOTICE ME
    }
     */
}



void SequencerGrid::clear()
{
    //set all grid points to 0, and send 'clear' sequencerData arrays
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
        {
            for (int i = 0; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                PAD_SETTINGS->setSequencerData(currentSequenceNumber-1, row, column, 0, false); 
                PAD_SETTINGS->clearSequencerDataString(currentSequenceNumber-1);
            }
            
            //clear grid GUI and local data array
            gridPoint[row][column]->setStatus(0);
            sequencerData[currentSequenceNumber-1][row][column] = 0;
            
        }
    }
    
}




void SequencerGrid::clearAll()
{
    //set all grid points to 0
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
        {
            gridPoint[row][column]->setStatus(0);
        }
    }
    
    //clear sequencerData arrays
    for (int seq = 0; seq <= NO_OF_SEQS-1; seq++)
    {
        for (int row = 0; row <= NO_OF_ROWS-1; row++)
        {
            for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
            {
                for (int i = 0; i < selectedPads.size(); i++)
                {
                    int padNum = selectedPads[i];
                    PAD_SETTINGS->setSequencerData(seq, row, column, 0, false);
                    PAD_SETTINGS->clearSequencerDataString(seq);
                }
            
                //clear local data array
                sequencerData[seq][row][column] = 0;
                

            }
        }
    }

}


void SequencerGrid::resetGridGui()
{
    //set all grid points to 0
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
        {
            int prevStatus = gridPoint[row][column]->getStatus();
            if (prevStatus == 1) //if it needs clearing to 0...
            {
                gridPoint[row][column]->setStatus(0);
            }
        }
    }
    
}


void SequencerGrid::setCurrentSequenceNumber (int value)
{
    currentSequenceNumber = value;
    
    //display the correct sequence on the grid
    
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
        {
            //only update the gridPoint Status (which triggers a repaint) if the status is different
            //otherwise it will repaint all 12*32 objects each time, which caused lags when rapidly switching seqs using pressure
            int prevStatus = gridPoint[row][column]->getStatus();
            int newStatus = sequencerData[currentSequenceNumber-1][row][column];
            if (prevStatus != newStatus)
            {
                //std::cout << " repainting...";
                
                gridPoint[row][column]->setStatus(newStatus);
                //or should it read directly from PadSettings seqData array? or would this be too CPU estensive?
            }
        }
    }
    
}


void SequencerGrid::setPlayHeadPos (float position)
{
    playHead->setLinePostion(position);
}


//observers update function, called everytime the grids appearence needs changing
bool SequencerGrid::update(const Subject& theChangedSubject)
{
    if (&theChangedSubject == &mSubject)
    {
        if (mSubject.getWhatShouldUpdateFlag() == 1) //if notified by an 'update playhead' call
        {
            //std::cout << "update playhead with columnNumber " << mSubject.getCurrentColumnNumber() << std::endl;
            int currentColumnNumber = mSubject.getCurrentColumnNumber();
            if (currentColumnNumber == 0)
            {
                if (SINGLE_PAD)
                {
                    int padNum = selectedPads[0];
                    currentColumnNumber = PAD_SETTINGS->getSequencerLength(); //weird thing to get it displaying right
                }
            }
            
            playHead->setLinePostion(float(currentColumnNumber));
        }
    }
    
    return true;
}


void SequencerGrid::setVelocityLabelText (String velocity)
{
    guiSequencerMode.setParameterLabelText(velocity);
}