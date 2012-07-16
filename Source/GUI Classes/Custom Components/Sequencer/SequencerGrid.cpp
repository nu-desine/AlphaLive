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
#include "AppSettings.h"
#include "SequencerBinaryData.h"
#include "GuiSequencerMode.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[currentlySelectedPad]
#define PAD_SETTINGS_i AppSettings::Instance()->padSettings[i]




SequencerGrid::SequencerGrid(ModeSequencer &ref, GuiSequencerMode &ref2)
                                        :   mSubject(ref),
                                            guiSequencerMode(ref2)
{
    backgroundImage = ImageFileFormat::loadFrom(SequencerBinaryData::seqgridbg_png, SequencerBinaryData::seqgridbg_pngSize);
    
    //init playHead
    addAndMakeVisible(playHead = new SequencerGridPlayHead());
    
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
    
    addAndMakeVisible(endBlock = new SequencerGridEndBlock());
    
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
    
    //arrange all gridPoints
    for (int column = 0; column <= NO_OF_COLUMNS-1; column++) //i iterates through the column numbers from left to right
    {
        int rowReverse = 0; //k iterates upwards to set the vertical position 
        for (int row = NO_OF_ROWS-1; row >= 0; row--) //j iterates through row numbers from bottom to top
        {
            gridPoint[row][column]->setBounds((GRID_POINT_W*float(column)) + (GRID_POINT_W/2.0), (GRID_POINT_H*float(rowReverse)) + (GRID_POINT_H/2.0), GRID_POINT_W, GRID_POINT_H);
            rowReverse++;
        }
        
    }
}

void SequencerGrid::setCurrentlySelectedPad (int value)
{
    currentlySelectedPad = value;
    
    //if it is a single pad, get the padsettings seq data array and put it into the local array.
    //then display the first one
    //if it is a row or 'all' pads, 
    
    //if individual pad number is selected
    if(currentlySelectedPad < 99)
    {
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
    
    //if current pad selected is 'all pads'
    if (currentlySelectedPad == 99)
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
    
    //if current pad selected is a row
    if (currentlySelectedPad > 99)
    {
        //get row number
        int alphaRow = currentlySelectedPad - 99; 
        
        for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
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
    
    //if current pad selected is a single pad
    if (currentlySelectedPad < 99)
    {
        PAD_SETTINGS->setSequencerData(currentSequenceNumber-1, row, column, status);
    }
    
    //if current pad selected is 'all pads'
    if (currentlySelectedPad == 99)
    {
        for (int i = 0; i <= 47; i++)
        {
            PAD_SETTINGS_i->setSequencerData(currentSequenceNumber-1, row, column, status);
            
        }
    }
    
    //if current pad selected is a row
    if (currentlySelectedPad > 99)
    {
        //get row number
        int alphaRow = currentlySelectedPad - 99; 
        
        for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
        {
            //i = pad number
            PAD_SETTINGS_i->setSequencerData(currentSequenceNumber-1, row, column, status);
        }
    }
    
    //set the local sequencer data array
    sequencerData[currentSequenceNumber-1][row][column] = status;


    
    
}


void SequencerGrid::paint (Graphics &g)
{
    //draw background
    //g.setColour(Colours::white);
    //g.fillRect(0, 0, getWidth(), getHeight());
    g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
    
    //draw vertical lines
    for (int i = 0, j = 0; i <=SEQ_HORIZONTAL_STEPS; i++)
    {
        //j is used as a counter to different lines after every 1 regular lines
        
        if (j == 1)
            g.setColour(Colours::orange);
        else
           g.setColour(Colours::darkgrey);
        
        g.drawLine((GRID_POINT_W)*float(i), 0, (GRID_POINT_W)*float(i), getHeight());
        
        j++;
        if (j == 5)
            j = 1;
    }
    
    //draw horizontal lines
    for (int i = 0, j = 0; i <=SEQ_VERTICAL_STEPS; i++)
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
  
    //resize end block
    endBlock->setBounds(float(PAD_SETTINGS->getSequencerLength()+1)*GRID_POINT_W, 0, getWidth()-(float(PAD_SETTINGS->getSequencerLength()+1)*GRID_POINT_W), getHeight());
}


void SequencerGrid::clear()
{
    //set all grid points to 0, and send 'clear' sequencerData arrays
    for (int row = 0; row <= NO_OF_ROWS-1; row++)
    {
        for (int column = 0; column <= NO_OF_COLUMNS-1; column++)
        {
            //if current pad selected is a single pad
            if (currentlySelectedPad < 99)
            {
                PAD_SETTINGS->setSequencerData(currentSequenceNumber-1, row, column, 0, false); 
                PAD_SETTINGS->clearSequencerDataString(currentSequenceNumber-1);
            }
            
            //if current pad selected is 'all pads'
            if (currentlySelectedPad == 99)
            {
                for (int i = 0; i <= 47; i++)
                {
                    PAD_SETTINGS_i->setSequencerData(currentSequenceNumber-1, row, column, 0, false);
                    PAD_SETTINGS_i->clearSequencerDataString(currentSequenceNumber-1);
                    
                }
            }
            
            //if current pad selected is a row
            if (currentlySelectedPad > 99)
            {
                //get row number
                int alphaRow = currentlySelectedPad - 99; 
                
                for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
                {
                    //i = pad number
                    PAD_SETTINGS_i->setSequencerData(currentSequenceNumber-1, row, column, 0, false);
                    PAD_SETTINGS_i->clearSequencerDataString(currentSequenceNumber-1);
                }
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
                //if current pad selected is a single pad
                if (currentlySelectedPad < 99)
                {
                    PAD_SETTINGS->setSequencerData(seq, row, column, 0, false);
                    PAD_SETTINGS->clearSequencerDataString(seq);
                }
                
                //if current pad selected is 'all pads'
                if (currentlySelectedPad == 99)
                {
                    for (int i = 0; i <= 47; i++)
                    {
                        PAD_SETTINGS_i->setSequencerData(seq, row, column, 0, false);
                        PAD_SETTINGS_i->clearSequencerDataString(seq);
                        
                    }
                }
                
                //if current pad selected is a row
                if (currentlySelectedPad > 99)
                {
                    //get row number
                    int alphaRow = currentlySelectedPad - 99; 
                    
                    for(int i = (alphaRow*8)-8; i <= (alphaRow*8)-1; i++) 
                    {
                        //i = pad number
                        PAD_SETTINGS_i->setSequencerData(seq, row, column, 0, false);
                        PAD_SETTINGS_i->clearSequencerDataString(seq);
                    }
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
                currentColumnNumber = PAD_SETTINGS->getSequencerLength(); //weird thing to get it displaying right
            }
            
            playHead->setLinePostion(float(currentColumnNumber)*GRID_POINT_W);
        }
    }
    
    return true;
}


void SequencerGrid::setVelocityLabelText (String velocity)
{
    guiSequencerMode.setVelocityLabelText(velocity);
}