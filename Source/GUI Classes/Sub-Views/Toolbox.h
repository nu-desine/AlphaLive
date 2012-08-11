//
//  Toolbox.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 18/07/2012.
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

#ifndef H_TOOLBOX
#define H_TOOLBOX

#include "../../../JuceLibraryCode/JuceHeader.h"


class Toolbox : public Component,
                public FileBrowserListener
{
public:
    //==============================================================================
    Toolbox();
    ~Toolbox();
    //==============================================================================
    void resized();
    void paint(Graphics& g);
    //==============================================================================
    // These methods are our FileBrowserListener implementation:
    void selectionChanged();
    void fileClicked (const File& file, const MouseEvent& e);
    void fileDoubleClicked (const File& file);
    void browserRootChanged (const File&);
    
private:
    //==============================================================================
    TabbedComponent *tabbedComponent; //each mode will probably have its own dedicated TabbedComponent
    
    //There will be several of the following components, that each hold a list of files/scales/presets/etc...
    FileTreeComponent* fileTreeComp;
    FileListComponent* fileListComp;
    
    TimeSliceThread thread; //can i use just the single thread? Probably.
    DirectoryContentsList loopsDirectoryList, scalesDirectoryList;
    
    
    File loopsDir, scalesDir;
    //==============================================================================
};


#endif // H_MAINCOMPONENT