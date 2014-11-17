//
//  AlphaListBoxModel.cpp
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

#include "AlphaListBoxModel.h"
#include "XmlAttributesListBox.h"


AlphaListBoxModel::AlphaListBoxModel(StringArray itemText, XmlAttributesListBox &parent)
: parentRef(parent)
{
    text = itemText;
}

AlphaListBoxModel::~AlphaListBoxModel()
{
    
}

void AlphaListBoxModel::listBoxItemDoubleClicked (int row, const MouseEvent &e)
{
    parentRef.itemSelected (row);
}

int AlphaListBoxModel::getNumRows()
{
    return text.size();
}

void AlphaListBoxModel::paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected)
{
    
    Colour textColour (LookAndFeel::getDefaultLookAndFeel().findColour (PopupMenu::textColourId));
        
        if (rowIsSelected)
        {
            g.setColour (LookAndFeel::getDefaultLookAndFeel().findColour (PopupMenu::highlightedBackgroundColourId));
            g.fillRect (1, 1, width - 2, height - 2);
            
            g.setColour (LookAndFeel::getDefaultLookAndFeel().findColour (PopupMenu::highlightedTextColourId));
        }
        else
        {
            g.setColour (textColour);
        }
        
        
        
        Font font (LookAndFeel::getDefaultLookAndFeel().getPopupMenuFont());
        
        if (font.getHeight() > height / 1.3f)
            font.setHeight (height / 1.3f);
        
        g.setFont (font);
        
        const int leftBorder = (height * 5) / 4;
        const int rightBorder = 4;
        
        
        g.drawFittedText (translate(text[rowNumber]),
                          leftBorder, 0,
                          width - (leftBorder + rightBorder), height,
                          Justification::centredLeft, 1);
        
    
}
