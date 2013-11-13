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
    
	padName = String(padNum + 1);

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
    gradientOuterColour = gradientInnerColour = Colours::white;
	
	modeOpacity = 0.05f;
	
    if (padNum == 0 || padNum == 8 || padNum == 16 || padNum == 24 || padNum == 32 || padNum == 40)
    {
        padOuterColour = Colours::grey.withAlpha(0.9f);
        padInnerColour = Colours::grey.withAlpha(0.3f);
    }
    else
    {
        padOuterColour = Colours::darkgrey.withAlpha(0.9f);
        padInnerColour = Colours::darkgrey.withAlpha(0.3f);
    }
    
//    padOuterColour = Colours::darkgrey.withAlpha(0.9f);
//    padInnerColour = Colours::darkgrey.withAlpha(0.3f);
    padColour = Colours::lightgrey.withAlpha(0.6f);
    
    somethingIsBeingDraggedOver = false;
	
    addMouseListener(this, true);
    
    isMouseInPlayMode = false;
    shouldIgnoreValues = false;
    playingStatus = 0;
		
}


GuiPad::~GuiPad()
{
	deleteAllChildren();
}



//==============================================================================
void GuiPad::resized()
{
    hitPath.addEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84));
	sb->setBounds ((getWidth()*0.05), (getHeight()*0.05), (getWidth()*0.9), (getHeight()*0.9));
}




void GuiPad::paint (Graphics& g)
{
    //main image
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
    g.setFont(9 + AlphaTheme::getInstance()->fontSizeAddition);
    g.drawFittedText(padName, (getWidth()*0.1), 0, (getWidth()*0.8), getHeight(), Justification::centred, 1);
    
    ColourGradient ringGradient(Colours::transparentWhite, (getWidth()*0.5), (getHeight()*0.6), modeColour, (getWidth()*0.5), 0, false);
    g.setGradientFill(ringGradient);
    g.fillEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84));
    
    //pressure gradient (should this be in a seperate component so that repainting is less CPU-heavy)?
    gradientOuterColour = gradientOuterColour.withAlpha(float(gradientOuterColourAlpha));
    gradientInnerColour = gradientInnerColour.withAlpha(float(gradientInnerColourAlpha));
    ColourGradient gradient(gradientInnerColour, (getWidth()*0.5),(getHeight()*0.5), gradientOuterColour, (getWidth()*0.8),(getHeight()*0.8), true);
    g.setGradientFill(gradient);
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


void GuiPad::setGradient (int pressureValue, int minPressureValue)
{
    
    int currentTime = Time::currentTimeMillis();
   
    //update the GUI at time intervals
    //updating everytime there is an OSC message would be too CPU extensive and would create series of commands the GUI couldn't keep up with
    if (currentTime >= (lastTime + UPDATE_TIME)) 
    {
        gradientInnerColourAlpha = pressureValue * (1.0/float(MAX_PRESSURE));
        gradientOuterColourAlpha = 1 - (pressureValue * (1.0/float(MAX_PRESSURE)));
        
        //repaint to re-apply gradientChange
        repaint();

        lastTime = currentTime;
    }
    
    //following if statement needed as using a timer above means that
    //the pad isn't likely to be set to 0 when depressed
    if (pressureValue == 0)
    {
        gradientInnerColourAlpha = gradientOuterColourAlpha = 0;
        
        repaint();
    }
    
    //following if statement needed as using a timer above means that
    //the pad isn't likely to be set to 0 when depressed
    else if (pressureValue == MAX_PRESSURE)
    {
        gradientInnerColourAlpha = 1;
        gradientOuterColourAlpha = 0;
        
        repaint();
    }
    
    else if (pressureValue == minPressureValue)
    {
        gradientInnerColourAlpha = pressureValue * (1.0/float(MAX_PRESSURE));
        gradientOuterColourAlpha = 1 - (pressureValue * (1.0/float(MAX_PRESSURE)));

        repaint();
    }

}

void GuiPad::setGradientColour (bool pressureIsLatched)
{
    Colour gradientColour;
    
    if (pressureIsLatched == true)
        gradientColour = Colours::white.overlaidWith (AlphaTheme::getInstance()->mainColour.withAlpha(0.7f));
    else
        gradientColour = Colours::white;
    
    gradientInnerColour = gradientOuterColour = gradientColour;
    
    repaint();
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
		overlayColour = AlphaTheme::getInstance()->mainColour.withAlpha(0.6f);	
	}
	repaint();
}
	
void GuiPad::turnOff()
{
	//function that forces toggle to false
    sb->setToggleState(false, dontSendNotification);
}

void GuiPad::turnOn()
{
	//function that forces toggle to true
	sb->setToggleState(true, dontSendNotification);
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
	if (PAD_SETTINGS->getMode() == 2 &&
        (extension == ".wav" || extension == ".aiff" || extension == ".aif"))
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
        //dissable the component so that that the mouse-up event that
        //will follow doesn't trigger the shape button to be 'listened' to
        //(so it doesn't call buttonClicked() is GuiPadLayout)
        setEnabled(false);
        
        PopupMenu menu;
        menu.addItem(1, translate("Copy pad settings..."));
        menu.addItem(2, translate("Paste pad settings..."));
        
        if (PAD_SETTINGS->getMode() == 2)
        {
            menu.addSeparator();
            menu.addItem(3, translate("Set audio sample..."));
        }
        
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
        
        else if (result == 3)
        {
            FileChooser myChooser (translate("Please select an audio file to load..."),
                                   AppSettings::Instance()->getLastAudioSampleDirectory(),
                                   "*.wav;*.aif;*.aiff");
            
            if (myChooser.browseForFileToOpen() == true)
            {
                File selectedAudioFile (myChooser.getResult());
                PAD_SETTINGS->setSamplerAudioFilePath(selectedAudioFile);
                
                guiPadLayoutRef.getMainComponent().getGuiSamplerMode()->setAudioFileDisplay(selectedAudioFile);
                
                AppSettings::Instance()->setLastAudioSampleDirectory(selectedAudioFile.getParentDirectory());
            }
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
    //if the mouse state is currently being used to emulate a pad press,
    //send a pad 'off' message.
    if (isMouseInPlayMode == true)
    {
        //emulate pad release
        //guiPadLayoutRef.getAlphaLiveEngine().playPadFromMouse(padNumber, 0);
        guiPadLayoutRef.getAlphaLiveEngine().hidInputCallback(padNumber, 0, 110);
        
        isMouseInPlayMode = false;
    }
    
    //re-enable the component after dissabling it in mouseDown()
    setEnabled(true);
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



