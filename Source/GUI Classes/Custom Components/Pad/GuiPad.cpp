/*
 *  GuiPad.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 19/09/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "GuiPad.h"
#include "PadBinaryData.h"
#include "GuiPadLayout.h"
#include "AppSettings.h"

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
    //If the number is the start of a row (0, 8, 16, 24, 32, 40) load the white pad image
    if (padNumber == 0 || padNumber == 8 || padNumber == 16 || padNumber == 24 || padNumber == 32 || padNumber == 40)
        image = ImageCache::getFromMemory(PadBinaryData::pad2_png, PadBinaryData::pad2_pngSize);
    else 
        image = ImageCache::getFromMemory(PadBinaryData::pad_png, PadBinaryData::pad_pngSize);

	
// path for shapebutton
	Path myPath;
	myPath.addEllipse (0.0f, 0.0f, 100.0f, 100.0f);
	
	normalColour = Colours::transparentBlack;
	overColour = Colours::white.withAlpha(0.3f);
	downColour = Colours::white.withAlpha(0.6f);
    
    overlayColour = Colours::transparentBlack;
	
    //the following alpha values are for a gradient ring colour
    //change to 0.5f for solid colours
	looperColour = Colours::green.withAlpha(0.7f);
	midiColour = Colours::yellow.withAlpha(0.7f);
	seqColour = Colours::red.withAlpha(0.7f);
	controllerColour = Colours::blue.withAlpha(0.7f);
	
	
	//create shape button with listener reference from guipadlayout
	sb = new ShapeButton (padName, normalColour, overColour, downColour);
	sb->setShape (myPath , true, false, false);
	sb->setClickingTogglesState (true);
	sb->addListener(&guiPadLayoutRef);

	addAndMakeVisible (sb, 3);

    lastTime = Time::currentTimeMillis();

    gradient = new ColourGradient(g1, (getWidth()*0.5),(getHeight()*0.5), g2, (getWidth()*0.8),(getHeight()*0.8), true);
    gradientOuterColourAlpha = gradientInnerColourAlpha = 0;
    oscGradientOuterColour = oscGradientInnerColour = Colours::red.withAlpha(0.0f);
    
    somethingIsBeingDraggedOver = false;
	
    addMouseListener(this, true);
    
    isMouseInPlayMode = false;
    shouldIgnoreValues = false;
    playingStatus = 0;
		
}


GuiPad::~GuiPad()
{
	deleteAllChildren();
    delete gradient;
}



//==============================================================================
void GuiPad::resized()
{
    hitPath.addEllipse(0.0f, 0.0f, getWidth(), getHeight());
	sb->setBounds (0, 0, getWidth(), getHeight());
}




void GuiPad::paint (Graphics& g)
{
    //std::cout << "Paint pad...";
	
    //main image
    g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 0, 100, 100);
    
    //button 'on' overlay
    g.setColour(overlayColour);
    g.fillEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84));
    
    //text
    g.setColour(Colours::white);
    g.setFont(9);
    g.drawFittedText(padName, (getWidth()*0.1), 0, (getWidth()*0.8), getHeight(), Justification::centred, 1);
    
    //mode ring
    //ColourGradient ringGradient(Colours::transparentWhite, (getWidth()/3)*2, (getHeight()/3)*2, modeColour, 0, 0, false);
    //g.setColour(modeColour);
    
    ColourGradient ringGradient(Colours::transparentWhite, getWidth(), getHeight(), modeColour, 0, 0, false);
    g.setGradientFill(ringGradient);
    
    g.drawEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84), 3);
    
    //osc gradient (should this be in a seperate component so that repainting is less CPU-heavy)?
    oscGradientOuterColour = Colours::orange.withAlpha(float(gradientOuterColourAlpha));
    oscGradientInnerColour = Colours::orange.withAlpha(float(gradientInnerColourAlpha));
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
        g.setColour(Colours::red.withAlpha(0.7f));
        //g.fillEllipse((getWidth()*0.08), (getHeight()*0.08), (getWidth()*0.84), (getHeight()*0.84));
        g.drawEllipse(getWidth()*0.25, getHeight()*0.25, getWidth()*0.51, getHeight()*0.51, getWidth()/8);
    }
    else if (playingStatus == 2) //waiting to play
    {
        //visually signify that the pad is waiting to play
        g.setColour(Colours::yellow.withAlpha(0.7f));
        g.drawEllipse(getWidth()*0.25, getHeight()*0.25, getWidth()*0.51, getHeight()*0.51, getWidth()/8);
    }
    else if (playingStatus == 3) //waiting to stop
    {
        //visually signify that the pad is waiting to stop
        g.setColour(Colours::brown.withAlpha(0.7f));
        g.drawEllipse(getWidth()*0.25, getHeight()*0.25, getWidth()*0.51, getHeight()*0.51, getWidth()/8);
    }


}


void GuiPad::setGradient (int oscValue)
{
    
    int currentTime = Time::currentTimeMillis();
   
    //update the GUI at time intervals
    //updating everytime there is an OSC message would be too CPU extensive and would create series of commands the GUI couldn't keep up with
    if (currentTime >= (lastTime + UPDATE_TIME)) 
    {
        gradientInnerColourAlpha = oscValue * (1.0/float(511.0));
        gradientOuterColourAlpha = 1 - (oscValue * (1.0/float(511.0)));
        
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
    if (oscValue == 511)
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
		modeColour = looperColour;
	
	else if (value == 2)
		modeColour = midiColour;
	
	else if (value == 3)
		modeColour = seqColour;
	
	else if (value == 4)
		modeColour = controllerColour;
    
    
    setPadText();
	
	repaint();
}	


void GuiPad::toggleChange()
{
	//function to be called when togglestate changes to apply overlay colour
	i = sb->getToggleState();
	
	if (i == 0) {
		
		sb->setClickingTogglesState (true);
		
		overlayColour = Colours::transparentBlack;
		g1 = Colours::transparentBlack;
		g2 = Colours::transparentBlack;
		gradient->setColour(0, g1);
		gradient->setColour(1, g2);
	}
	else {
		
		sb->setClickingTogglesState (false);
		
		overlayColour = Colours::blue.withAlpha(0.6f);
		g1 = Colours::darkblue;
		g2 = Colours::aqua;
		gradient->setColour(0, g1);
		gradient->setColour(1, g2);
		
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
        
        else if (PAD_SETTINGS->getMode() == 2) //Looper
        {
            File newFile(PAD_SETTINGS->getLooperAudioFilePath());
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
    
    //std::cout << droppedFile.getFullPathName() << std::endl;
    
    //'put' audiofile into pad
    PAD_SETTINGS->setLooperAudioFilePath(droppedFile);
    
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
        menu.addItem(1, "Copy pad settings...");
        menu.addItem(2, "Paste pad settings...");
        
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
        //guiPadLayoutRef.getAlphaLiveEngine().playPadFromMouse(padNumber, pressureValue);
        guiPadLayoutRef.getAlphaLiveEngine().inputData(padNumber, pressureValue);
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
        
        if (pressureValue > 511)
            pressureValue = 511;
        else
            shouldIgnoreValues = false;
        
        if (shouldIgnoreValues == false)
        {
            //emulate pressure change
            //guiPadLayoutRef.getAlphaLiveEngine().playPadFromMouse(padNumber, pressureValue);
            guiPadLayoutRef.getAlphaLiveEngine().inputData(padNumber, pressureValue);
            
            if (pressureValue == 511)
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
        guiPadLayoutRef.getAlphaLiveEngine().inputData(padNumber, 1);
        guiPadLayoutRef.getAlphaLiveEngine().inputData(padNumber, 0);
        //guiPadLayoutRef.getAlphaLiveEngine().playPadFromMouse(padNumber, 1);
        //guiPadLayoutRef.getAlphaLiveEngine().playPadFromMouse(padNumber, 0);
        
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



