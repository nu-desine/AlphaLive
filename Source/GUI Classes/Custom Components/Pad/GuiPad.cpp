/*
 *  GuiPad.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 19/09/2011.
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
 */

#include "GuiPad.h"
#include "GuiPadLayout.h"
#include "../../../File and Settings/AppSettings.h"
#include "../../AlphaLiveLookandFeel.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

//how often the pad GUI gradient is updated when pressure is applied.
//the smaller the number (milliseconds), the more CPU-heavy
#define UPDATE_TIME 100

//==============================================================================

GuiPad::GuiPad(int padNum, GuiPadLayout &ref) 
                            :   Component ("GuiPad"),
                                padNumber(padNum),
                                somethingIsBeingDraggedOver (false),
                                guiPadLayoutRef(ref)
{
    
	padName << padNum + 1;
    
    //Set the image of the pad depending on the pad number.
    /* //If the number is the start of a row (0, 8, 16, 24, 32, 40) load the white pad image
     if (padNumber == 16)
     image = ImageCache::getFromMemory(PadBinaryData::greypad_png, PadBinaryData::greypad_pngSize);
     else if (padNumber == 24 || padNumber == 8)
     image = ImageCache::getFromMemory(PadBinaryData::greypad1_png, PadBinaryData::greypad1_pngSize);
     else if (padNumber == 32 || padNumber == 0)
     image = ImageCache::getFromMemory(PadBinaryData::greypad2_png, PadBinaryData::greypad2_pngSize);
     else if (padNumber == 40)
     image = ImageCache::getFromMemory(PadBinaryData::greypad3_png, PadBinaryData::greypad3_pngSize);
     else if (padNumber > 40)
     image = ImageCache::getFromMemory(PadBinaryData::pad3_png, PadBinaryData::pad3_pngSize);	
     else if ((padNumber > 0 && padNumber < 8) || (padNumber > 32 && padNumber < 40))
     image = ImageCache::getFromMemory(PadBinaryData::pad2_png, PadBinaryData::pad2_pngSize);
     else if ((padNumber > 24 && padNumber < 32) || (padNumber > 8 && padNumber < 16))
     image = ImageCache::getFromMemory(PadBinaryData::pad1_png, PadBinaryData::pad1_pngSize);
     else 
     image = ImageCache::getFromMemory(PadBinaryData::pad_png, PadBinaryData::pad_pngSize);
     
     */
	
	//midiImage = ImageCache::getFromMemory(BinaryData::midisymbol_png, BinaryData::midisymbol_pngSize);
	//loopImage = ImageCache::getFromMemory(BinaryData::loopsymbol_png, BinaryData::loopsymbol_pngSize);
	//seqImage = ImageCache::getFromMemory(BinaryData::sequencersymbol_png, BinaryData::sequencersymbol_pngSize);
	//controlImage = ImageCache::getFromMemory(BinaryData::controlsymbol_png, BinaryData::controlsymbol_pngSize);
	
	
	
    // path for shapebutton
    
	Path myPath;
	myPath.addEllipse (0.0f, 0.0f, 100, 100);
	
	normalColour = Colours::transparentBlack;
	overColour = Colours::white.withAlpha(0.2f);
	downColour = Colours::white.withAlpha(0.3f);
    
    overlayColour = Colours::transparentBlack;
	
    //the following alpha values are for a gradient ring colour
    //change to 0.5f for solid colours
	samplerColour = Colours::orange.withAlpha(0.2f);
	midiColour = Colours::green.withAlpha(0.2f);
	sequencerColour = Colours::red.withAlpha(0.2f);
	controllerColour = Colours::purple.withAlpha(0.2f);
	
	
	//create shape button with listener reference from guipadlayout
	sb = new ShapeButton (padName, normalColour, overColour, downColour);
	sb->setShape (myPath , true, false, false);
	sb->setClickingTogglesState (true);
	sb->addListener(&guiPadLayoutRef);
    
	addAndMakeVisible (sb, 3);
    
    lastTime = Time::currentTimeMillis();
    
    
    gradientOuterColourAlpha = gradientInnerColourAlpha = 0;
    oscGradientOuterColour = oscGradientInnerColour = Colours::white.withAlpha(0.0f);
	
	modeOpacity = 0.05f;
	
	padColour = Colours::lightgrey.withAlpha(0.6f);
	padOuterColour = Colours::darkgrey.withAlpha(0.9f);
    padInnerColour = Colours::darkgrey.withAlpha(0.3f);
    
    somethingIsBeingDraggedOver = false;
	
    addMouseListener(this, true);
    
    isMouseInPlayMode = false;
    shouldIgnoreValues = false;
    playingStatus = 0;
		
}


GuiPad::~GuiPad()
{
	deleteAllChildren();
    //delete gradient;
}



//==============================================================================
void GuiPad::resized()
{
    hitPath.addEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84));
	sb->setBounds ((getWidth()*0.05), (getHeight()*0.05), (getWidth()*0.9), (getHeight()*0.9));
}




void GuiPad::paint (Graphics& g)
{
    //Rectangle<int> rect = g.getClipBounds();
    //std::cout << "Painting pad " << padNumber << ": " << rect.getX() << " " << rect.getY() << " " << rect.getRight() << " " << rect.getBottom() << std::endl;
	
    //main image
    //g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 0, image.getWidth(), image.getHeight());
	g.setColour(padColour);
	g.drawEllipse(getWidth()*0.05, getHeight()*0.05, getWidth()*0.9, getHeight()*0.9, 2.0f);
    ColourGradient padGradient(padInnerColour, (getWidth()*0.5),(getHeight()*0.5), padOuterColour, (getWidth()*0.8),(getHeight()*0.8), true);
    g.setGradientFill(padGradient);
    
	g.fillEllipse((getWidth()*0.05), (getHeight()*0.05), (getWidth()*0.9), (getHeight()*0.9));
    
    
    //button 'on' overlay
	g.setColour(overlayColour);
    g.fillEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84));
    
    //text
    g.setColour(Colours::white);
    g.setFont(9);
    g.drawFittedText(padName, (getWidth()*0.1), 0, (getWidth()*0.8), getHeight(), Justification::centred, 1);
    
    //mode ring
	//g.setColour(modeColour);
    //	g.drawEllipse(getWidth()*0.03, getHeight()*0.03, getWidth()*0.94, getHeight()*0.94, 2.0f);
	
    //ColourGradient ringGradient(Colours::transparentWhite, (getWidth()/3)*2, (getHeight()/3)*2, modeColour, 0, 0, false);
    //g.setColour(modeColour);
    
    ColourGradient ringGradient(Colours::transparentWhite, (getWidth()*0.5), (getHeight()*0.6), modeColour, (getWidth()*0.5), 0, false);
    g.setGradientFill(ringGradient);
    g.fillEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84));
    
    //osc gradient (should this be in a seperate component so that repainting is less CPU-heavy)?
    oscGradientOuterColour = Colours::white.withAlpha(float(gradientOuterColourAlpha));
    oscGradientInnerColour = Colours::white.withAlpha(float(gradientInnerColourAlpha));
    ColourGradient oscGradient(oscGradientInnerColour, (getWidth()*0.5),(getHeight()*0.5), oscGradientOuterColour, (getWidth()*0.8),(getHeight()*0.8), true);
    g.setGradientFill(oscGradient);
    g.fillEllipse((getWidth()*0.05), (getHeight()*0.05), (getWidth()*0.9), (getHeight()*0.9));
	
    if (somethingIsBeingDraggedOver == true)
    {
        //draw a 'highlight' eclipse. Another option would be to draw an image of a downwards arrow signifying 'drop here'?
        g.setColour(Colours::white.withAlpha(0.5f));
        g.fillEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84));
    }
    
    
    if (playingStatus == 1) //playing
    {
        //visually signify that the pad is playing
        g.setColour(Colours::red.withAlpha(0.2f));
        //g.fillEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84));
        g.drawEllipse((getWidth()*0.12), (getHeight()*0.12), (getWidth()*0.76), (getHeight()*0.76), getWidth()/8);
    }
    else if (playingStatus == 2) //waiting to play
    {
        //visually signify that the pad is waiting to play
        g.setColour(Colours::yellow.withAlpha(0.2f));
        g.drawEllipse((getWidth()*0.12), (getHeight()*0.12), (getWidth()*0.76), (getHeight()*0.76), getWidth()/8);
    }
    else if (playingStatus == 3) //waiting to stop
    {
        //visually signify that the pad is waiting to stop
        g.setColour(Colours::red.withAlpha(0.1f));
        g.drawEllipse((getWidth()*0.12), (getHeight()*0.12), (getWidth()*0.76), (getHeight()*0.76), getWidth()/8);
    }



}


void GuiPad::setGradient (int oscValue)
{
    
    int currentTime = Time::currentTimeMillis();
   
    //update the GUI at time intervals
    //updating everytime there is an OSC message would be too CPU extensive and would create series of commands the GUI couldn't keep up with
    if (currentTime >= (lastTime + UPDATE_TIME)) 
    {
        gradientInnerColourAlpha = oscValue * (1.0/float(MAX_PRESSURE));
        gradientOuterColourAlpha = 1 - (oscValue * (1.0/float(MAX_PRESSURE)));
        
        //repaint to re-apply gradientChange
        repaint();

        lastTime = currentTime;
    }
    
    //following if statement needed as using a timer above means that
    //the pad isn't likely to be set to 0 when depressed
    if (oscValue == 0)
    {
        gradientInnerColourAlpha = gradientOuterColourAlpha = 0;
        
        repaint();
    }
    
    //following if statement needed as using a timer above means that
    //the pad isn't likely to be set to 0 when depressed
    if (oscValue == MAX_PRESSURE)
    {
        gradientInnerColourAlpha = 1;
        gradientOuterColourAlpha = 0;
        
        repaint();
    }

}



void GuiPad::modeChange(int value)
{
	if (value == 0) 
		modeColour = normalColour;
	
	else if (value == 1)
		modeColour = midiColour;
	
	else if (value == 2)
		modeColour = samplerColour;
	
	else if (value == 3)
		modeColour = sequencerColour;
	
	else if (value == 4)
		modeColour = controllerColour;
    
    
    setPadText();
	
	repaint();
}	


void GuiPad::toggleChange()
{
	//function to be called when togglestate changes to apply overlay colour
	i = sb->getToggleState();
	
	if (i == 0) 
    {
		
		sb->setClickingTogglesState (true);
		
		overlayColour = Colours::transparentBlack;
		
	}
	else 
    {
		
		sb->setClickingTogglesState (false);
		
		overlayColour = AlphaColours::blue.withAlpha(0.6f);
		
		
	}
	repaint();
}
	
void GuiPad::turnOff()
{
	//function that forces toggle to false
    sb->setToggleState(false, false);
}

void GuiPad::turnOn()
{
	//function that forces toggle to true
	sb->setToggleState(true, false);
}

bool GuiPad::hitTest (int x, int y)
{
	//returns true when mouse is within path hitpath
	return hitPath.contains(x, y);
}

Button* GuiPad::getButton()
{
	//returns pointer to shapebutton
    return sb;
}


//called from 'mode change' above
//finish this feature at a later date.
//think about how the pad text updates when contents are updated directly by the user...
//...Where should this function be called to do this?
//relevent combobox in maincomponent is hidden for now
void GuiPad::setPadText()
{
    if (AppSettings::Instance()->getPadDisplayTextMode() == 1) //pad numbers
    {
        padName = String(padNumber+1);
    }
    
    else if (AppSettings::Instance()->getPadDisplayTextMode() == 2) //pad contents
    {
        if (PAD_SETTINGS->getMode() == 0) //off
        {
            padName = String::empty; 
        }
        
        else if (PAD_SETTINGS->getMode() == 1) //MIDI
        {
            padName = "Note " + String(PAD_SETTINGS->getMidiNote()); 
        }
        
        else if (PAD_SETTINGS->getMode() == 2) //Sampler
        {
            File newFile(PAD_SETTINGS->getSamplerAudioFilePath());
            padName = newFile.getFileNameWithoutExtension();
        }
        
        else if (PAD_SETTINGS->getMode() == 3) //Sequencer
        {
            padName = String::empty; 
        }
        
        else if (PAD_SETTINGS->getMode() == 4) //Controller
        {
            padName = String::empty; 
        }
    }
    
    repaint();
}



bool GuiPad::isInterestedInFileDrag (const StringArray& files)
{
	File selectedFile(files[0]);
			
	String extension = selectedFile.getFileExtension();
	//std::cout << extension << std::endl;
			
	//Only allow .wav and .aiff files to be dropped. Ignore any other files
    //is there a way to accept any common audio files?
	if (extension == ".wav" || extension == ".aiff" || extension == ".aif")
	{
		return true;
	}
	else
		return false;
	 
}
					  
void GuiPad::fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/)
{
	somethingIsBeingDraggedOver = true;
	repaint();
}
					  
void GuiPad::fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/)
{
	
}
					  
void GuiPad::fileDragExit (const StringArray& /*files*/)
{
	somethingIsBeingDraggedOver = false;
	repaint();
}
	


void GuiPad::filesDropped (const StringArray& files, int /*x*/, int /*y*/)
{
	//string of filepath
	message = files.joinIntoString ("\n");
	//string reduced to only include characters after last "/"
	//message = message.fromLastOccurrenceOf(File::separatorString, false, true);
	
    //file of filepath
    File droppedFile (message);
    
    //'put' audiofile into pad
    PAD_SETTINGS->setSamplerAudioFilePath(droppedFile);
    guiPadLayoutRef.getMainComponent().getGuiSamplerMode()->setAudioFileDisplay(droppedFile);
    
	somethingIsBeingDraggedOver = false;
	repaint();
	 
}

void GuiPad::setPadPlayingState (int playingState)
{
    playingStatus = playingState;
    repaint();
}


void GuiPad::mouseDown (const MouseEvent &e)
{
    //====================================================================================
    //right-click (or ctrl-click on a mac) to bring up the popup menu to allow pad settings 
    //to be copied and pasted
    //====================================================================================
    if (e.mods.isPopupMenu() == true)
    {
        PopupMenu menu;
        menu.addItem(1, translate("Copy pad settings..."));
        menu.addItem(2, translate("Paste pad settings..."));
        
        const int result = menu.show();
        
        if (result == 0)
        {
            // user dismissed the menu without picking anything
        }
        
        else if (result == 1)
        {
            copyPadSettings();
        }
        
        else if (result == 2)
        {
            pastePadSettings();
            
        }
    }
    
    //====================================================================================
    //Alt-click to emulate a physical pad being pressed.
    //====================================================================================
    else if (e.mods.isAltDown() == true)
    {
        //dissable the component so that that the mouse-up event that 
        //will follow doesn't trigger the shape button to be 'listened' to
        //(so it doesn't call buttonClicked() is GuiPadLayout)
        setEnabled(false);
        
        isMouseInPlayMode = true;
        
        //emulate a pad press with a pressure value of the distance of the Y position
        //of the mouse click from the centre of the component/pad, mutipled by 10 to put
        //the distance into a more suited range, and added by 1 so that the distance cannot
        //be 0 (no pressure).
        int pressureValue = abs((getHeight()/2) - e.y) * 10 + 1;
        //emulate pad press
        guiPadLayoutRef.getAlphaLiveEngine().hidInputCallback(padNumber, pressureValue, 110);
        //guiPadLayoutRef.getAlphaLiveEngine().playPadFromMouse(padNumber, pressureValue);
    }
    
}



void GuiPad::mouseDrag (const MouseEvent &e)
{
    //Dragging the mouse in 'play mode'
    if (isMouseInPlayMode == true)
    {
        //Drag is centred around the centre point of the pad/component in only
        //the y dimension (as its less fiddly to find the lowest pressure value using just one
        //dimension)
        
        int pressureValue = abs((getHeight()/2) - e.y) * 10 + 1;
        
        if (pressureValue > MAX_PRESSURE)
            pressureValue = MAX_PRESSURE;
        else
            shouldIgnoreValues = false;
        
        if (shouldIgnoreValues == false)
        {
            //emulate pressure change
            guiPadLayoutRef.getAlphaLiveEngine().hidInputCallback(padNumber, pressureValue, 110);
            //guiPadLayoutRef.getAlphaLiveEngine().playPadFromMouse(padNumber, pressureValue);
            
            if (pressureValue == MAX_PRESSURE)
                shouldIgnoreValues = true;
        }
    }
}
 


void GuiPad::mouseUp (const MouseEvent &e)
{
    //if the mouse state is currently being used to emulate a pad press
    //, re-enable the component and send a pad 'off' message.
    if (isMouseInPlayMode == true)
    {
        setEnabled(true);
        //why can't i just sent a 'pressure value' of 0?
        //for some reason i need to send something before that
        //for most of the triggerModes to work. This should probably be fixed!!
        //guiPadLayoutRef.getAlphaLiveEngine().playPadFromMouse(padNumber, 1);
        //guiPadLayoutRef.getAlphaLiveEngine().playPadFromMouse(padNumber, 0);
        guiPadLayoutRef.getAlphaLiveEngine().hidInputCallback(padNumber, 1, 110);
        guiPadLayoutRef.getAlphaLiveEngine().hidInputCallback(padNumber, 0, 110);
        
        isMouseInPlayMode = false;
    }
        
}



void GuiPad::copyPadSettings()
{
    // stored pad settings into memory
    AppSettings::Instance()->copyPadSettings(padNumber);
}


void GuiPad::pastePadSettings()
{
    // paste pad settings from memory into pad
    AppSettings::Instance()->pastePadSettings(padNumber);
    
    // set the mode ring colour
    modeChange(PAD_SETTINGS->getMode());
    
    //if the pasted pad is currently selected, update its display

    if (AppSettings::Instance()->getCurrentlySelectedPad().size() == 1)
    {
        if (AppSettings::Instance()->getCurrentlySelectedPad()[0] == padNumber)
            guiPadLayoutRef.updateCurrentlySelectedPad();
    }
    
    //ideally, users should only be allowed to paste once something has been copied
    //what about when pasting between projects when the audio file doesn't exist?
    //reset the clipbaord when opening a new project? or when copying, note down any audio files?
    
}



