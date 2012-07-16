/*
 *  CircleWaveformCache.cpp
 *  AlphaSoft
 *
 *  Created by Sam Davies on 07/11/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "CircleWaveformCache.h"
#include "CircleWaveform.h"


//==============================================================================
struct WaveformCacheEntry
{
    int64 hash;
    uint32 lastUsed;
    MemoryBlock data;
	
};

//==============================================================================
CircleWaveformCache::CircleWaveformCache (const int maxNumThumbsToStore_)
: TimeSliceThread ("thumb cache"),
maxNumThumbsToStore (maxNumThumbsToStore_)
{
    startThread (2);
}

CircleWaveformCache::~CircleWaveformCache()
{
}

WaveformCacheEntry* CircleWaveformCache::findThumbFor (const int64 hash) const
{
    for (int i = thumbs.size(); --i >= 0;)
        if (thumbs.getUnchecked(i)->hash == hash)
            return thumbs.getUnchecked(i);
	
    return nullptr;
}

bool CircleWaveformCache::loadThumb (CircleWaveform& thumb, const int64 hashCode)
{
    WaveformCacheEntry* te = findThumbFor (hashCode);
	
    if (te != nullptr)
    {
        te->lastUsed = Time::getMillisecondCounter();
		
        MemoryInputStream in (te->data, false);
        thumb.loadFrom (in);
        return true;
    }
	
    return false;
}

void CircleWaveformCache::storeThumb (const CircleWaveform& thumb,
                                      const int64 hashCode)
{
    WaveformCacheEntry* te = findThumbFor (hashCode);
	
    if (te == nullptr)
    {
        te = new WaveformCacheEntry();
        te->hash = hashCode;
		
        if (thumbs.size() < maxNumThumbsToStore)
        {
            thumbs.add (te);
        }
        else
        {
            int oldest = 0;
            uint32 oldestTime = Time::getMillisecondCounter() + 1;
			
            for (int i = thumbs.size(); --i >= 0;)
            {
                if (thumbs.getUnchecked(i)->lastUsed < oldestTime)
                {
                    oldest = i;
                    oldestTime = thumbs.getUnchecked(i)->lastUsed;
                }
            }
			
            thumbs.set (oldest, te);
        }
    }
	
    te->lastUsed = Time::getMillisecondCounter();
	
    MemoryOutputStream out (te->data, false);
    thumb.saveTo (out);
}

void CircleWaveformCache::clear()
{
    thumbs.clear();
}