//
//  XmlAttributesListBox.h
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

#ifndef H_XMLATTRIBUTESLISTBOX
#define H_XMLATTRIBUTESLISTBOX

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "NoteLayoutListBoxModel.h"


class XmlAttributesListBox : public Component
{
public:
    //==============================================================================
    XmlAttributesListBox(File fileToList);
    ~XmlAttributesListBox();
    
    void resized();
    void paint(Graphics& g);
    
  
private:
    //==============================================================================
    NoteLayoutListBoxModel *listBoxModel;
    ListBox *listBox;
    File xmlFile;
    
    
};


#endif // H_XMLATTRIBUTESLISTBOX
