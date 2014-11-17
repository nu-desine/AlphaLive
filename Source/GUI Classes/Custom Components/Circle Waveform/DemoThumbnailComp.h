//
//  DemoThumbnailComp.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 08/12/2011.
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

#ifndef H_DEMOTHUMBNAILCOMP
#define H_DEMOTHUMBNAILCOMP

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "CircleWaveform.h"


class DemoThumbnailComp  :  public Component,
                            public ChangeListener
{
public:
    DemoThumbnailComp();
    ~DemoThumbnailComp();
   
	
    void setFile (const File& file);
    void setZoomFactor (double amount);
    //void mouseWheelMove (const MouseEvent&, float wheelIncrementX, float wheelIncrementY);
    void paint (Graphics& g);
    void changeListenerCallback (ChangeBroadcaster*);
    	
private: 

    AudioFormatManager formatManager;
    CircleWaveformCache thumbnailCache;
    CircleWaveform thumbnail;
    double startTime, endTime;
};

#endif //H_DEMOTHUMBNAILCOMP


