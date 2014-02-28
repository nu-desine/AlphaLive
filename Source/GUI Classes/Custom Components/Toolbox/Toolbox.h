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

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "XmlAttributesListBox.h"

class MainComponent;

class Toolbox : public Component,
                public FileBrowserListener
{
public:
    //==============================================================================
    Toolbox(MainComponent &parent);
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
    
    void mouseEnter (const MouseEvent &e);
    void mouseExit (const MouseEvent &e);
    
    void setCurrentlySelectedPad (Array<int> selectedPads_);
    void updateDisplay();
    
    void noteLayoutSelected (String layout, bool isScale);
    
    void setTabColour();
    void setCurrentTabIndex (int index);
    
private:
    //==============================================================================
    Array<int> selectedPads;
    MainComponent &mainComponentRef;
    
    //Main object of the component
    ScopedPointer <TabbedComponent> tabbedComponent;
    
    //Each set of items (presets, scales, samples etc...) will have its own tree or list component
    OwnedArray <FileTreeComponent> treeLists;
    OwnedArray <FileListComponent> fileLists;
    int noOfFileLists, noOfTreeLists;
    
    //all fileTree and fileList component need their own of the following object too...
    OwnedArray <DirectoryContentsList> contentLists;
    
    //Below are custom list box components
    ScopedPointer <XmlAttributesListBox> scalesListBox, layoutsListBox;
    
    TimeSliceThread thread; //can i use just the single thread? Probably.
    
    ScopedPointer <WildcardFileFilter> padFileFilter, seqFileFilter, audioFileFilter, bankFileFilter, sceneFileFilter;
    
    int currentList;
    //==============================================================================
};


#endif // H_TOOLBOX