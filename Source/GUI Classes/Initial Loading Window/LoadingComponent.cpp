//
//  LoadingComponent.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 17/11/2011.
//  Copyright 2011 nu desine.
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

#include "LoadingComponent.h"
#include "../Binary Data/MainBinaryData.h"


LoadingComponent::LoadingComponent()
{
    backgroundImage = ImageFileFormat::loadFrom(MainBinaryData::loading_png, MainBinaryData::loading_pngSize);
    
    Font newFont (50, Font::bold);
    addAndMakeVisible(loadingLabel = new Label("Loading Label", "AlphaLive\nLoading...")); 
    loadingLabel->setFont(newFont);
    loadingLabel->setJustificationType(Justification::centred);
    
}

LoadingComponent::~LoadingComponent()
{
    deleteAllChildren();
}

void LoadingComponent::resized()
{
    //loadingLabel->setBounds(0, 0, getWidth(), getHeight());
}

void LoadingComponent::paint (Graphics &g)
{
    g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
    
}

