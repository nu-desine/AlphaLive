//
//  GuiBeatIndicator.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/02/2012.
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

#include "GuiBeatIndicator.h"

GuiBeatIndicator::GuiBeatIndicator()
{
    status = 0;
}

GuiBeatIndicator::~GuiBeatIndicator()
{
    
}

void GuiBeatIndicator::resized()
{
    
}

void GuiBeatIndicator::paint (Graphics& g)
{
    if (status == 0)
        g.setColour(Colours::white);
    else
        g.setColour(Colours::red);
    
    g.fillEllipse(0, 0, getWidth(), getHeight());
}

