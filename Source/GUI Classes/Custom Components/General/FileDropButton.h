//
//  AlphaDropButton.h
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

#ifndef H_FILEDROPBUTTON
#define H_FILEDROPBUTTON

#include "../../../../JuceLibraryCode/JuceHeader.h"

class FileDropButton :  public TextButton,
                        public FileDragAndDropTarget

{
public:
    FileDropButton (int rowNumber_);
    FileDropButton(String buttonText, int rowNumber_);
    ~FileDropButton();
	void resized();
	bool hitTest (int x, int y);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    
    // These methods implement the FileDragAndDropTarget interface, and allow the component
    // to accept drag-and-drop of files..
    bool isInterestedInFileDrag (const StringArray& files);
    void fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/);
    void fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/);
    void fileDragExit (const StringArray& /*files*/);
    void filesDropped (const StringArray& files, int /*x*/, int /*y*/);
	
	
	
private:
	
    Array<int> selectedPads;
    
	Path thePath;
	int rowNumber;
    bool somethingIsBeingDraggedOver;
};

#endif