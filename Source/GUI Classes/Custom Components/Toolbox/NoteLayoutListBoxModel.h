//
//  NoteLayoutListBoxModel.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 25/08/2012.
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

#ifndef H_NOTELAYOUTLISTBOXMODEL
#define H_NOTELAYOUTLISTBOXMODEL

#include "../../../../JuceLibraryCode/JuceHeader.h"


class NoteLayoutListBoxModel : public ListBoxModel
{
public:
    //==============================================================================
    NoteLayoutListBoxModel(StringArray itemText);
    ~NoteLayoutListBoxModel();
    
    int getNumRows();
    void paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected);
    
    
private:
    //==============================================================================
    
    StringArray text;
};


#endif // H_NOTELAYOUTLISTBOXMODEL