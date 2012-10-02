//
//  AlphaDropButton.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 22/08/2012.
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

#include "FileDropButton.h"
#include "../../../File and Settings/AppSettings.h"

FileDropButton::FileDropButton(int rowNumber_)
: rowNumber(rowNumber_)
{
	thePath.addEllipse(0, 0, 100, 100);
    
    somethingIsBeingDraggedOver = false;
}

FileDropButton::FileDropButton(String buttonText, int rowNumber_)
: rowNumber(rowNumber_)
{
    thePath.addEllipse(0, 0, 100, 100);
    
    setButtonText(buttonText);
}

FileDropButton::~FileDropButton()
{		
    selectedPads.clear();
}

void FileDropButton::resized()
{
	thePath.addEllipse(0, 0, getWidth(), getHeight());
}


/*
void FileDropButton::paint (Graphics &g)
{
    if (somethingIsBeingDraggedOver == true)
    {
        //draw a 'highlight' eclipse. Another option would be to draw an image of a downwards arrow signifying 'drop here'?
        g.setColour(Colours::white.withAlpha(0.3f));
        g.fillEllipse(0, 0, getWidth(), getHeight());
    }
}
 */

bool FileDropButton::hitTest (int x, int y)
{
	return thePath.contains(x, y);
}

void FileDropButton::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    
}


bool FileDropButton::isInterestedInFileDrag (const StringArray& files)
{
	File selectedFile(files[0]);
    
	String extension = selectedFile.getFileExtension();
    
	//Only allow .wav and .aiff files to be dropped. Ignore any other files
    //is there a way to accept any common audio files?
	if (extension == ".wav" || extension == ".aiff" || extension == ".aif")
	{
		return true;
	}
	else
		return false;
    
}

void FileDropButton::fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/)
{
	//somethingIsBeingDraggedOver = true;
	//repaint();
}

void FileDropButton::fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/)
{
	
}

void FileDropButton::fileDragExit (const StringArray& /*files*/)
{
	//somethingIsBeingDraggedOver = false;
	//repaint();
}



void FileDropButton::filesDropped (const StringArray& files, int /*x*/, int /*y*/)
{
	//string of filepath
	String message = files.joinIntoString ("\n");
	
    //file of filepath
    File droppedFile (message);
    
    //'put' audiofile into rowNumber of selectedPads
    for (int i = 0; i < selectedPads.size(); i++)
    {
        int padNum = selectedPads[i];
        AppSettings::Instance()->padSettings[padNum]->setSequencerSamplesAudioFilePath(droppedFile, rowNumber);
    }
    
    
	//somethingIsBeingDraggedOver = false;
	//repaint();
    
}

