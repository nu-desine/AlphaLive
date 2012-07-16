//
//  AbstractSubjectAndObserver.cpp
//  Observer Pattern Example
//
//  Created by Liam Meredith-Lacey on 23/09/2011.
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

#include "AbstractSubjectAndObserver.h"


//=====================================================================
/*
    Observer Class function definitions
*/

Observer::Observer()
{
    
}

Observer::~Observer()
{
    
}

//=====================================================================
/*
 Subject Class function definitions
 */

Subject::Subject()
{
}

Subject::~Subject()
{
}

void Subject::attach (Observer* obs) 
{
    mObservers.push_back(obs);
}

void Subject::detach (Observer* obs) 
{
    mObservers.remove(obs);
}

bool Subject::notifyObs() 
{
    ObsList detachList;
    for (ObsListIter i (mObservers.begin()); i != mObservers.end(); ++i) {
        
        if (!(*i)->update(*this)) 
        {
            detachList.push_back(*i);
        }
    }
    for (ObsListIter j (detachList.begin()); j != detachList.end(); ++j) 
    {
        detach(*j);
    }
    return true; // always return true, but may be different logic in future
}

//=====================================================================