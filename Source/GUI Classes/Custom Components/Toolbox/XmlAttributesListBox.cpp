//
//  XmlAttributesListBox.cpp
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

#include "XmlAttributesListBox.h"
#include "Toolbox.h"
#include "../../AlphaLiveLookandFeel.h"


XmlAttributesListBox::XmlAttributesListBox (File fileToList, File fileToList2, bool forScales, Toolbox &parent)
:   forScales_(forScales),
    parentRef(parent)
{
     //will have to check tagname outside this class so it can be used for multiple files.
    //should be check its in an xml format here?
    
    StringArray itemStrings;
    xmlData = new XmlElement("data");
    
    //search through the files for the child elements and append them to xmlData
    if (fileToList.exists())
    {
        ScopedPointer <XmlElement> xmlFile(XmlDocument::parse(fileToList));
        
        for (int i = 0; i < xmlFile->getNumChildElements(); i++)
        {
            xmlData->addChildElement(new XmlElement (*xmlFile->getChildElement(i)));
        }
    }
    if (fileToList2.exists())
    {
        ScopedPointer <XmlElement> xmlFile(XmlDocument::parse(fileToList2));
        
        for (int i = 0; i < xmlFile->getNumChildElements(); i++)
        {
            //do some safety checks, as these elements
            //would have been made by the user.
            if (xmlFile->getChildElement(i)->hasAttribute("name") &&
                xmlFile->getChildElement(i)->hasAttribute("layout"))
            {
                xmlData->addChildElement(new XmlElement (*xmlFile->getChildElement(i)));
            }
        }
    }
    
    int numOfItems = xmlData->getNumChildElements();
    for (int i = 0; i < numOfItems; i++)
        itemStrings.add(xmlData->getChildElement(i)->getStringAttribute("name"));
    
    listBoxModel = new AlphaListBoxModel(itemStrings, *this);
    addAndMakeVisible(listBox = new ListBox("list box", listBoxModel));
        
    listBox->setRowHeight(14 + (AlphaTheme::getInstance()->fontSizeAddition * 1.5));
    listBox->updateContent();
  
    
}

XmlAttributesListBox::~XmlAttributesListBox()
{
    delete xmlData;
    delete listBoxModel;
    deleteAllChildren();
}

void XmlAttributesListBox::resized()
{
    listBox->setBounds(0, 0, getWidth(), getHeight());
}

void XmlAttributesListBox::paint(Graphics& g)
{

}

void XmlAttributesListBox::deselectAllRows()
{
    listBox->deselectAllRows();
}

void XmlAttributesListBox::itemSelected (int itemRow)
{
    //called from listBoxModel when an item is double clicked
    String noteLayout = xmlData->getChildElement(itemRow)->getStringAttribute("layout");
    
    parentRef.noteLayoutSelected(noteLayout, forScales_);
  
}