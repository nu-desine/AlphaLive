//
//  AbstractSubjectAndObserver.h
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
/*  
    This file contains the abstract classes for creating the Observer Design Pattern.
    Code was adapted from http://codewrangler.home.comcast.net/~codewrangler/tech_info/patterns_code.html
    The following page describes the Observer Design Pattern:
    http://www.cs.clemson.edu/~malloy/courses/patterns/observer.html
*/

#include <list>


//=====================================================================
class Subject;

/*  The abstract Observer class of the Observer Design Pattern.
 Any class that need to 'observe' the subject class must inherit from this class
 and provide implementation for the pure virtual update() function
 */
#ifndef H_OBSERVER
#define H_OBSERVER

class Observer 
{
public:
    virtual ~Observer();
    // Observer (const Observer& ); // implicit
    // Observer& operator= (const Observer& ); // implicit
    
    virtual bool update(const Subject& theChangedSubject) = 0;
    
protected:
    Observer(); // protected default ctor
};

#endif //H_OBSERVER

//=====================================================================

/*  The abstract Subject class of the Observer Design Pattern.
    Any class that has 'observers' and need to notify them must 
    inherit from this class.
 */

#ifndef H_SUBJECT
#define H_SUBJECT

class Subject 
{
public:
    virtual ~Subject();
    // Subject (const Subject& ); // implicit
    // Subject& operator= (const Subject& ); // implicit
    
    virtual void attach(Observer*);
    virtual void detach(Observer*);
    virtual bool notifyObs(); // bool return for a failure condition
protected:
    Subject(); // protected default ctor
private:
    typedef std::list<Observer*> ObsList;
    typedef ObsList::iterator ObsListIter;
    ObsList mObservers;
};

#endif //H_SUBJECT

//=====================================================================