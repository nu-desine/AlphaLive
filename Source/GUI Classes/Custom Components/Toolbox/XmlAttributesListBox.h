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
#include "AlphaListBoxModel.h"

//SHOULD THIS CLASS BE A COMPONENT WITH A LISTBOX OBJECT
//OR A LISTBOX OBJECT ITSELF?

class Toolbox;

class XmlAttributesListBox : public Component
{
public:
    //==============================================================================
    XmlAttributesListBox(File fileToList, bool forScales, Toolbox &parent);
    ~XmlAttributesListBox();
    
    void resized();
    void paint(Graphics& g);
    
    void itemSelected (int itemRow);
    
    void deselectAllRows();
  
private:
    //==============================================================================
    AlphaListBoxModel *listBoxModel;
    ListBox *listBox;
    XmlElement *xmlData;
    
    bool forScales_;
    Toolbox &parentRef;
    
};


#endif // H_XMLATTRIBUTESLISTBOX
