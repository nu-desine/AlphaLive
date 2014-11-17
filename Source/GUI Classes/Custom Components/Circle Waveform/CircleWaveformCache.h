/*
 *  CircleWaveformCache.h
 *  AlphaSoft
 *
 *  Created by Sam Davies on 07/11/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */
#ifndef CIRCLEWAVEFORMCACHE_H
#define CIRCLEWAVEFORMCACHE_H

#include "../../../../JuceLibraryCode/JuceHeader.h"
//#include "CircleWaveform.h"



struct WaveformCacheEntry;


//==============================================================================
/**
 An instance of this class is used to manage multiple AudioThumbnail objects.
 
 The cache runs a single background thread that is shared by all the thumbnails
 that need it, and it maintains a set of low-res previews in memory, to avoid
 having to re-scan audio files too often.
 
 @see AudioThumbnail
 */

class CircleWaveform;

class CircleWaveformCache   : public TimeSliceThread
{
public:
    //==============================================================================
    /** Creates a cache object.
	 
	 The maxNumThumbsToStore parameter lets you specify how many previews should
	 be kept in memory at once.
	 */
    explicit CircleWaveformCache (int maxNumThumbsToStore);
	
    /** Destructor. */
    ~CircleWaveformCache();
	
    //==============================================================================
    /** Clears out any stored thumbnails.
	 */
    void clear();
	
    /** Reloads the specified thumb if this cache contains the appropriate stored
	 data.
	 
	 This is called automatically by the AudioThumbnail class, so you shouldn't
	 normally need to call it directly.
	 */
    bool loadThumb (CircleWaveform& thumb, int64 hashCode);
	
    /** Stores the cachable data from the specified thumb in this cache.
	 
	 This is called automatically by the AudioThumbnail class, so you shouldn't
	 normally need to call it directly.
	 */
    void storeThumb (const CircleWaveform& thumb, int64 hashCode);
	
	
private:
    //==============================================================================
    OwnedArray <WaveformCacheEntry> thumbs;
    int maxNumThumbsToStore;
	
    WaveformCacheEntry* findThumbFor (int64 hash) const;
	
   
};


#endif   // CIRCLEWAVEFORMCACHE_H
