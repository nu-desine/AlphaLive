//
//  LayoutsAndScales.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 26/09/2011.
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

//#include "LayoutsAndScales.h"

namespace Layouts 
{
    //the following layout is the new osc-to-pad layout where the index is in incoming OSC pad number (0-47) 
    //and the number at each index is the new pad number (0-47). 
    //Each number is then incremented by 1 for the GUI so the user is working with pads in the range 1-48
    const int padArrangementLayout[48] = {45,46,47,40,41,42,43,44,  36,37,38,39,32,33,34,35,  27,28,29,30,31,24,25,26,  18,19,20,21,22,23,16,17,  9,10,11,12,13,14,15,8,  0,1,2,3,4,5,6,7};
    
    //the following layout is the default CC controller layout for the pad numbers
    const int ccLayout[48] = {12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,52,53,54,55,56,57,58,59,60,61,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119};
    
    //how to add a layout to the application - enter instructions here!
    
    const int major[48] = {0,2,4,5,7,9,11,12,  0+12,2+12,4+12,5+12,7+12,9+12,11+12,12+12,  0+24,2+24,4+24,5+24,7+24,9+24,11+24,12+24,  0+36,2+36,4+36,5+36,7+36,9+36,11+36,12+36,  0+48,2+48,4+48,5+48,7+48,9+48,11+48,12+48, 0+60,2+60,4+60,5+60,7+60,9+60,11+60,12+60};
    const int minor[48] = {0,2,3,5,7,8,10,12,  0+12,2+12,3+12,5+12,7+12,8+12,10+12,12+12,  0+24,2+24,3+24,5+24,7+24,8+24,10+24,12+24,  0+36,2+36,3+36,5+36,7+36,8+36,10+36,12+36,  0+48,2+48,3+48,5+48,7+48,8+48,10+48,12+48,  0+60,2+60,3+60,5+60,7+60,8+60,10+60,12+60};  
    const int melodicMinor[48] = {0,2,3,5,7,9,11,12,  0+12,2+12,3+12,5+12,7+12,9+12,11+12,12+12,  0+24,2+24,3+24,5+24,7+24,9+24,11+24,12+24,  0+36,2+36,3+36,5+36,7+36,9+36,11+36,12+36,  0+48,2+48,3+48,5+48,7+48,9+48,11+48,12+48,  0+60,2+60,3+60,5+60,7+60,9+60,11+60,12+60};
    const int harmonicMinor[48] = {0,2,3,5,7,8,11,12,  0+12,2+12,3+12,5+12,7+12,8+12,11+12,12+12,  0+24,2+24,3+24,5+24,7+24,8+24,11+24,12+24,  0+36,2+36,3+36,5+36,7+36,8+36,11+36,12+36,  0+48,2+48,3+48,5+48,7+48,8+48,11+48,12+48,  0+60,2+60,3+60,5+60,7+60,8+60,11+60,12+60};
    const int rockNRoll[48] = {0,3,4,5,7,9,10,12,  0+12,3+12,4+12,5+12,7+12,9+12,10+12,12+12,  0+24,3+24,4+24,5+24,7+24,9+24,10+24,12+24,  0+36,3+36,4+36,5+36,7+36,9+36,10+36,12+36,  0+48,3+48,4+48,5+48,7+48,9+48,10+48,12+48,  0+60,3+60,4+60,5+60,7+60,9+60,10+60,12+60};
    
    //The following layouts don't follow the same logic as the scale layout
    
    //Simple chromatic layout that spirals up the sphere
    const int chromatic[48] = {0,1,2,3,4,5,6,7,  15,8,9,10,11,12,13,14, 22,23,16,17,18,19,20,21,  29,30,31,24,25,26,27,28,  36,37,38,39,32,33,34,35,  43,44,45,46,47,40,41,42};
    
    //Our two-row chromatic layout - the chromatic scale is split over two rows at a time
    const int chromaticTwoRow[48] = {0,2,4,6,8,10,12,14,  13,15,1,3,5,7,9,11,  26,28,30,16,18,20,22,24,  23,25,27,29,31,17,19,21,  36,38,40,42,44,46,32,34,  33,35,37,39,41,43,45,47};
    
    //official harmonic table layout - http://en.wikipedia.org/wiki/Harmonic_table_note_layout
    const int harmonicTable1[48] = {36,37,38,39,40,41,42,43,  46,39,40,41,42,43,44,45,  48,49,42,43,44,45,46,47,  50,51,52,45,46,47,48,49,  52,53,54,55,48,49,50,51, 54,55,56,57,58,51,52,53}; 
    
    //Adams HT layout which is shifted anti-clockwise - 
    //http://www.watershed.co.uk/ished/mediasandbox/projects/2010/alphasphere/journal/mapping-the-sphere
    const int harmonicTable2[48] = {60,69,66,63,60,69,66,63,  70,67,64,61,70,67,64,61,  68,65,62,71,68,65,62,71,  66,63,60,69,66,63,60,69,  64,61,70,67,64,61,70,67,  62,71,68,65,62,71,68,65}; 
    //is this right? Does it work?
    
    //Wicki-Hayden note layout - http://en.wikipedia.org/wiki/Wicki-Hayden_note_layout
    const int wickiHayden[48] = {36,38,40,42,44,46,48,50,  55,57,43,45,47,49,51,53,  58,60,62,48,50,52,54,56,  61,63,65,67,69,55,57,59,  64,66,68,70,72,74,60,62,  67,69,71,73,75,77,79,81};
    
    //Based on this monome layout - http://www.youtube.com/watch?v=uQm3xbTxJRc&feature=related
    const int perfectFourths[48] = {0,5,10,15,20,25,30,35,  30+1,35+1,0+1,5+1,10+1,15+1,20+1,25+1,  25+2,30+2,35+2,0+2,5+2,10+2,15+2,20+2,  15+3,20+3,25+3,30+3,35+3,0+3,5+3,10+3,  10+4,15+4,20+4,25+4,30+4,35+4,0+4,5+4,  0+5,5+5,10+5,15+5,20+5,25+5,30+5,35+5};
}   

namespace Scales 
{
    //how to add a scale to the application - enter instructions here!
    
    const int chromatic[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
    const int major[12] = {0,2,4,5,7,9,11,12,14,16,17,19};
    const int majorPentatonic[12] = {0,2,4,7,9,12,14,16,19,21,24,26};
    const int minor[12] = {0,2,3,5,7,8,10,12,14,15,17,19};
    const int minorPentatonic[12] = {0,3,5,7,10,12,15,17,19,22,24,26};
    const int melodicMinor[12] = {0,2,3,5,7,9,11,12,14,15,17,19};
    const int harmonicMinor[12] = {0,2,3,5,7,8,11,12,14,15,17,19};
    const int blues[12] = {0,3,5,6,7,10,12,15,17,18,19,22};
    const int rockNRoll[12] = {0,3,4,5,7,9,10,12,15,16,17,19};
    const int hungarianGypsy[12] = {0,2,3,6,7,8,11,12,14,15,18,19};
    const int ragaMadhuri[12] = {0,4,5,7,9,10,11,12,16,17,19,21};
    const int alphaAdam[12] = {0,2,4,6,7,9,11,13,14,16,18,20}; //???
    
}

