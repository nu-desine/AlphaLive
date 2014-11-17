/*
 *  AlphaTextButton.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 03/07/2012.
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
 */

#include "AlphaTextButton.h"

AlphaTextButton::AlphaTextButton()
{
	thePath.addEllipse(0, 0, 100, 100);
}

AlphaTextButton::AlphaTextButton(String buttonText)
{
    thePath.addEllipse(0, 0, 100, 100);
    
    setButtonText(buttonText);
}

AlphaTextButton::~AlphaTextButton()
{		
}

void AlphaTextButton::resized()
{
	thePath.addEllipse(0, 0, getWidth(), getHeight());
}

bool AlphaTextButton::hitTest (int x, int y)
{
	return thePath.contains(x, y);
}