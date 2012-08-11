//
//  Toolbox.cpp
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

#include "Toolbox.h"

Toolbox::Toolbox() : 
                    fileTreeComp (0),
                    thread ("file tree thread"),
                    loopsDirectoryList (0, thread),
                    scalesDirectoryList(0, thread)
{
    //loopsDir = "/Users/Liam/Desktop/AlphaSphere Software Dev/Audio Samples/Library/Loops"; //what if it doesn't exist?
    loopsDir = File::getSpecialLocation(File::userMusicDirectory);
    loopsDirectoryList.setDirectory (loopsDir, true, true);
    //scalesDir = "/Users/Liam/Desktop/AlphaSphere Software Dev/Audio Samples/Library/Scales"; //what if it doesn't exist?
    scalesDir = File::getSpecialLocation(File::userDocumentsDirectory);
    scalesDirectoryList.setDirectory (scalesDir, true, true);
    thread.startThread();
    
    //By default FileTreeComponent's and FileListComponent's display file extention, file size, and file modifcation date
    //To remove these attributes, need to hack FileListTreeItem::paintItem in juce_FileTreeComponent.cpp
    //and FileListItemComponent::paint() in juce_FileListComponent.cpp
    //could this be done using LookAndFeel instead?
    
    fileTreeComp = new FileTreeComponent (loopsDirectoryList);
    fileTreeComp->addListener(this);
    fileTreeComp->setIndentSize(10);
    //to change the row height, need to hack FileTreeListItem::getItemHeight() in juce_FileTreeComponent.cpp
    //could this be done using LookAndFeel instead?
    
    fileListComp = new FileListComponent (scalesDirectoryList);
    fileListComp->addListener(this);
    fileListComp->setRowHeight(16);
    
    
    //create tabbed component and add tabs/child components
    addAndMakeVisible(tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    tabbedComponent->addTab("Scales", Colours::darkgrey, fileListComp, true);
    tabbedComponent->addTab("Sample Library", Colours::darkgrey, fileTreeComp, true);
    tabbedComponent->setTabBarDepth(22);
    
    //tabbedComponent->setCurrentTabIndex(1, false); // can do this to sort out the scroll bar resizing bug
}


Toolbox::~Toolbox()
{
    thread.stopThread(50);
    deleteAllChildren();
}

//==============================================================================

void Toolbox::resized()
{
    tabbedComponent->setBounds(5, 5, getWidth()-10, getHeight()-10);
    
}

void Toolbox::paint(Graphics& g)
{
    g.setColour(Colours::grey.withAlpha(0.2f));
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 10);
}


void Toolbox::selectionChanged()
{
    std::cout << "selection changed...\n";
}

void Toolbox::fileClicked (const File& file, const MouseEvent& e)
{
    std::cout << "file clicked...\n";
    
}

void Toolbox::fileDoubleClicked (const File& file)
{
    std::cout << "file double clicked...\n";
}

void Toolbox::browserRootChanged (const File&)
{
    std::cout << "browser root changed...\n";
}
