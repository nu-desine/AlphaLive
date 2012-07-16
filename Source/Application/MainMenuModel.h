//
//  MainMenuModel.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 14/06/2012.
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

#ifndef H_MAINMENUMODEL
#define H_MAINMENUMODEL

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../File and Settings/AppDocumentState.h"


class MainMenuModel  : public MenuBarModel
{
public:
    MainMenuModel(AppDocumentState &ref);
    ~MainMenuModel();
    
    StringArray getMenuBarNames();
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
    void menuItemSelected (int menuItemID, int topLevelMenuIndex);
    
private:
    AppDocumentState &appDocumentStateRef;
    
    
    
};




#endif //H_MAINMENUMODEL