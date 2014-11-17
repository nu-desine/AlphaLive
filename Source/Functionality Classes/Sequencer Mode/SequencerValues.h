//
//  SequencerValues.h
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 14/10/2011.
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


//define everything needed to change the size the dimensions of the grid and values of the sequencer arrays here!

//to change no. of rows and.or columns, just changes the numbers here!
#define SEQ_VERTICAL_STEPS 13.0 //number of grid rows + 1
#define SEQ_HORIZONTAL_STEPS 33.0 //number of grid columns + 1

#define NO_OF_ROWS int(SEQ_VERTICAL_STEPS)-1
#define NO_OF_COLUMNS int(SEQ_HORIZONTAL_STEPS)-1
#define NO_OF_SEQS 8

#define GRID_POINT_W float(getWidth())/(SEQ_HORIZONTAL_STEPS)
#define GRID_POINT_H float(getHeight())/(SEQ_VERTICAL_STEPS)

#define TIME_INTERVAL 15000.0/tempo