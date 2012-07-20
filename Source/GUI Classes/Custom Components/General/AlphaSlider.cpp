//
//  AlphaSlider.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 31/10/2011.
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

#include "AlphaSlider.h"


AlphaSlider::AlphaSlider()
{
    this->setSliderStyle(Slider::IncDecButtons);
    this->setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
    this->setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
}

AlphaSlider::~AlphaSlider()
{
    
}