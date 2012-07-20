//
//  SequenceAudioFilePlayer.cpp
//  AlphaSoft
//
//  Created by Liam Meredith-Lacey on 19/10/2011.
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

#include "SequenceAudioFilePlayer.h"
#include "../../File and Settings/AppSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

SequenceAudioFilePlayer::SequenceAudioFilePlayer(int padNumber_, int rowNumber_, TimeSliceThread *audioTransportSourceThread_)
                                                :   padNumber(padNumber_),
                                                    rowNumber(rowNumber_)
{
    audioTransportSourceThread = audioTransportSourceThread_;
    
    currentAudioFileSource = NULL;
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
    
    //call this here so that the default drum sounds are loaded in correcntly before this
    //object is intialised. This will only be temporary though.
    setAudioFile(PAD_SETTINGS->getSequencerSamplesAudioFilePath(rowNumber));
    
}

SequenceAudioFilePlayer::~SequenceAudioFilePlayer()
{
    fileSource.setSource(0);//unload the current file
	deleteAndZero(currentAudioFileSource);//delete the current file
    
}



//called from either the constructor, or setSequencerSamplesAudioFilePath() in PadSettings
void SequenceAudioFilePlayer::setAudioFile (File audioFile_)
{
    if (audioFile_ != File::nonexistent)
    {
        
        //passes in pads audio file
        File audioFile (audioFile_);
        
        //if the audio file is different from the previous one, stop and load in the new file
        if (audioFile != currentFile)
        {
            // unload the previous file source and delete it..
            fileSource.stop();
            fileSource.setSource (0);
            deleteAndZero (currentAudioFileSource);
            
            // create a new file source from the file..
            // get a format manager and set it up with the basic types (wav, ogg and aiff).
            AudioFormatManager formatManager;
            formatManager.registerBasicFormats();
            
            AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
            
            if (reader != 0)
            {
                currentFile = audioFile;
                currentAudioFileSource = new AudioFormatReaderSource (reader, true);
                
                //set read ahead buffer size
                int bufferSize = currentAudioFileSource->getTotalLength()/2;
                //restrict buffer size value
                if (bufferSize > 48000)
                    bufferSize = 48000;
                else if (bufferSize < 8000)
                    bufferSize = 8000;
                
                // ..and plug it into our transport source
                fileSource.setSource (currentAudioFileSource,
                                      bufferSize, // tells it to buffer this many samples ahead
                                      audioTransportSourceThread,
                                      reader->sampleRate);
                
                
            }
            
        }  
    }
}



void SequenceAudioFilePlayer::playAudioFile (float gain)
{
    fileSource.setGain(gain); //ideal the gain should be mapped logorythmitically (?)
    //there is a bug here in that when the gain is changed to a low value, it doesn't seem to change in time
    //most likely this is a juce bug that would have been fixed in the latest version?

    fileSource.setPosition (0.0);
    fileSource.start();
}


void SequenceAudioFilePlayer::stopAudioFile()
{
    fileSource.stop();
    //fileSource.setPosition (0.0);
}



void SequenceAudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    fileSource.getNextAudioBlock(bufferToFill);
}

void SequenceAudioFilePlayer::prepareToPlay (int samplesPerBlockExpected,double sampleRate)
{
    fileSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}



void SequenceAudioFilePlayer::releaseResources()
{
    fileSource.releaseResources();
}
