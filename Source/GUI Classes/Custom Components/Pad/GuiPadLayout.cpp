/*
 *  GuiPadLayout.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 22/09/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "GuiPadLayout.h"
#include "PadBinaryData.h"
#include "MainComponent.h"

GuiPadLayout::GuiPadLayout(AlphaLiveEngine &subject, MainComponent &ref)
                            :   Component ("GuiPadLayout"),
                                mSubject(subject), //set the classes subject to the alphaLiveEngine ref
                                mainComponentRef(ref) //take in a reference of MainComponent
                                

{
    
    //attach this class to the subject class
    mSubject.attach(this);
    
    //adds buttons to select rows using custom button GuiPadRow
    row6Sb =  new GuiPadRow();
    row5Sb =  new GuiPadRow();
    row4Sb =  new GuiPadRow();
    row3Sb =  new GuiPadRow();
    row2Sb =  new GuiPadRow();
    row1Sb =  new GuiPadRow();
    
    row1Sb->addListener(this);
	addAndMakeVisible(row1Sb);
	//row1Sb->toBehind(globalSb);
    row1Sb->addMouseListener(this, true);
	
	row2Sb->addListener(this);
	addAndMakeVisible(row2Sb);
	//row2Sb->toBehind(row1Sb);
    row2Sb->addMouseListener(this, true);
	
	row3Sb->addListener(this);
	addAndMakeVisible(row3Sb);
	//row3Sb->toBehind(row2Sb);
    row3Sb->addMouseListener(this, true);
	
	row4Sb->addListener(this);
	addAndMakeVisible(row4Sb);
	//row4Sb->toBehind(row3Sb);
    row4Sb->addMouseListener(this, true);
	
	row5Sb->addListener(this);
	addAndMakeVisible(row5Sb);
	//row5Sb->toBehind(row4Sb);
    row5Sb->addMouseListener(this, true);
	
	row6Sb->addListener(this);
	addAndMakeVisible(row6Sb);
	//row6Sb->toBehind(row5Sb);
    row6Sb->addMouseListener(this, true);
    
    globalSb = new GuiPadRow();
	globalSb->addListener(this);
	addAndMakeVisible(globalSb);
	//globalSb->toBehind(pads[0]);
    globalSb->addMouseListener(this, true);

	
	//adds 48 instances of GuiPad component with toggle-state active
	for (int i = 0; i <= 47; i++) 
    {
		pads.insert(i, new GuiPad(i, *this));
		addAndMakeVisible(pads[i]);
		pads[i]->getButton()->setClickingTogglesState(true);
        pads[i]->addMouseListener(this, true);
	}
	
	//path for hit test - DO WE NEED A HIT TEST FOR THIS COMPONENT?
	hitPath.addEllipse (230.0f, 30.0f, 590.0f, 590.0f);
	
	pStore = 0;
	rotateValue = 0;
	pivotX = 0;
	pivotY = 0;
	s = 0;	
	
	sin1 = 0.3826834324;
	sin2 = 0.9238795325;
	sinN1 = -sin1;
	sinN2 = -sin2;
	
	sin3 = 0.7071067812;
	sinN3 = -sin3;
	
	sin4 = 0.2588190451;
	sin5 = 0.8660254038;
	sin6 = 0.9659258263;
	sin7 = 0.5;
	sinN4 = -sin4;
	sinN5 = -sin5;
	sinN6 = -sin6;
	sinN7 = -sin7;
	
	sin8 = 0.4539904997;
	sin9 = 0.9510565163;
	sin10 = 0.8910065242;
	sin11 = 0.3090169944;
	sinN8 = -sin8;
	sinN9 = -sin9;
	sinN10 = -sin10;
	sinN11 = -sin11;
	
	
	cos1 = sin2;
	cos2 = sin1;
	cosN1 = -sin2;
	cosN2 = -sin1;
	
	cos3 = sin3;
	cosN3 = -sin3;
	
	cos4 = -sin6;
	cos5 = -sin7;
	cos6 = -sin4;
	cos7 = -sin5;
	cosN4 = -cos4;
	cosN5 = -cos5;
	cosN6 = -cos6;
	cosN7 = -cos7;
	
	cos8 = -sin10;
	cos9 = -sin11;
	cos10 = -sin8;
	cos11 = -sin9;
	cosN8 = -cos8;
	cosN9 = -cos9;
	cosN10 = -cos10;
	cosN11 = -cos11;
	
    //trigger the global button being clicked to set a default set of selected pads!
	//globalSb->triggerClick();
    
    shouldDisplaySettings = false;
    //currentlySelectedPad = 99;
    
    selectedPads.clear(); //should I call clearQuick() instead?
	
}

GuiPadLayout::~GuiPadLayout()
{
	
	delete globalSb;
	delete row1Sb;
	delete row2Sb;
	delete row3Sb;
	delete row4Sb;
	delete row5Sb;
	delete row6Sb;
    
    //detach this class from the subject class
    mSubject.detach(this);

}


void GuiPadLayout::resized()
{
	
	globalSb->centreWithSize(58, 58);
    
    row6Sb->centreWithSize(113, 113);
	row5Sb->centreWithSize(173, 173);
	row4Sb->centreWithSize(259, 259);
	row3Sb->centreWithSize(377, 377);
	row2Sb->centreWithSize(466, 466);
	row1Sb->centreWithSize(528, 528);
	
	
    //adds 48 pads using an equation that takes centre point, distance from centre and angle
    pads[45]->setBounds((((getWidth() *0.5) - 19) + (50 * sin1)), (((getHeight() *0.5) - 19) + (50 * cos1)) ,38, 38);
	pads[46]->setBounds((((getWidth() *0.5) - 19) + (50 * sin2)), (((getHeight() *0.5) - 19) + (50 * cos2)) ,38, 38);	
	pads[47]->setBounds((((getWidth() *0.5) - 19) + (50 * sin2)), (((getHeight() *0.5) - 19) + (50 * cosN2)) ,38, 38);
	pads[40]->setBounds((((getWidth() *0.5) - 19) + (50 * sin1)), (((getHeight() *0.5) - 19) + (50 * cosN1)) ,38, 38);	
	pads[41]->setBounds((((getWidth() *0.5) - 19) + (50 * sinN1)), (((getHeight() *0.5) - 19) + (50 * cosN1)) ,38, 38);
	pads[42]->setBounds((((getWidth() *0.5) - 19) + (50 * sinN2)), (((getHeight() *0.5) - 19) + (50 * cosN2)) ,38, 38);
	pads[43]->setBounds((((getWidth() *0.5) - 19) + (50 * sinN2)), (((getHeight() *0.5) - 19) + (50 * cos2)) ,38, 38);
	pads[44]->setBounds((((getWidth() *0.5) - 19) + (50 * sinN1)), (((getHeight() *0.5) - 19) + (50 * cos1)) ,38, 38);
	
	pads[36]->setBounds((((getWidth() *0.5) - 24) + (86 * sin3)), (((getHeight() *0.5) - 24) + (86 * cos3)) ,48, 48);
	pads[37]->setBounds((((getWidth() *0.5) - 24) + (86 * 1)), (((getHeight() *0.5) - 24) + (86 * 0)) ,48, 48);
	pads[38]->setBounds((((getWidth() *0.5) - 24) + (86 * sin3)), (((getHeight() *0.5) - 24) + (86 * cosN3)) ,48, 48);
	pads[39]->setBounds((((getWidth() *0.5) - 24) + (86 * 0)), (((getHeight() *0.5) - 24) + (86 * -1)) ,48, 48);
	pads[32]->setBounds((((getWidth() *0.5) - 24) + (86 * sinN3)), (((getHeight() *0.5) - 24) + (86 * cosN3)) ,48, 48);
	pads[33]->setBounds((((getWidth() *0.5) - 24) + (86 * -1)), (((getHeight() *0.5) - 24) + (86 * 0)) ,48, 48);
	pads[34]->setBounds((((getWidth() *0.5) - 24) + (86 * sinN3)), (((getHeight() *0.5) - 24) + (86 * cos3)) ,48, 48);
	pads[35]->setBounds((((getWidth() *0.5) - 24) + (86 * 0)), (((getHeight() *0.5) - 24) + (86 * 1)) ,48, 48);
	
	pads[27]->setBounds((((getWidth() *0.5) - 36) + (127 * sin2)), (((getHeight() *0.5) - 36) + (127 * cos2)) ,72, 72);
	pads[28]->setBounds((((getWidth() *0.5) - 36) + (127 * sin2)), (((getHeight() *0.5) - 36) + (127 * cosN2)) ,72, 72);
	pads[29]->setBounds((((getWidth() *0.5) - 36) + (127 * sin1)), (((getHeight() *0.5) - 36) + (127 * cosN1)) ,72, 72);
	pads[30]->setBounds((((getWidth() *0.5) - 36) + (127 * sinN1)), (((getHeight() *0.5) - 36) + (127 * cosN1)) ,72, 72);
	pads[31]->setBounds((((getWidth() *0.5) - 36) + (127 * sinN2)), (((getHeight() *0.5) - 36) + (127 * cosN2)) ,72, 72);
	pads[24]->setBounds((((getWidth() *0.5) - 36) + (127 * sinN2)), (((getHeight() *0.5) - 36) + (127 * cos2)) ,72, 72);
	pads[25]->setBounds((((getWidth() *0.5) - 36) + (127 * sinN1)), (((getHeight() *0.5) - 36) + (127 * cos1)) ,72, 72);
	pads[26]->setBounds((((getWidth() *0.5) - 36) + (127 * sin1)), (((getHeight() *0.5) - 36) + (127 * cos1)) ,72, 72);
	
	pads[18]->setBounds((((getWidth() *0.5) - 43) + (185 * 1)), (((getHeight() *0.5) - 43) + (185 * 0)) ,86, 86);
	pads[19]->setBounds((((getWidth() *0.5) - 43) + (185 * sin3)), (((getHeight() *0.5) - 43) + (185 * cosN3)) ,86, 86);
	pads[20]->setBounds((((getWidth() *0.5) - 43) + (185 * 0)), (((getHeight() *0.5) - 43) + (185 * -1)) ,86, 86);
	pads[21]->setBounds((((getWidth() *0.5) - 43) + (185 * sinN3)), (((getHeight() *0.5) - 43) + (185 * cosN3)) ,86, 86);
	pads[22]->setBounds((((getWidth() *0.5) - 43) + (185 * -1)), (((getHeight() *0.5) - 43) + (185 * 0)) ,86, 86);
	pads[23]->setBounds((((getWidth() *0.5) - 43) + (185 * sinN3)), (((getHeight() *0.5) - 43) + (185 * cos3)) ,86, 86);
	pads[16]->setBounds((((getWidth() *0.5) - 43) + (185 * 0)), (((getHeight() *0.5) - 43) + (185 * 1)) ,86, 86);
	pads[17]->setBounds((((getWidth() *0.5) - 43) + (185 * sin3)), (((getHeight() *0.5) - 43) + (185 * cos3)) ,86, 86);
	
	pads[9]->setBounds((((getWidth() *0.5) - 36) + (230 * sin2)), (((getHeight() *0.5) - 36) + (230 * cosN2)) ,72, 72);
	pads[10]->setBounds((((getWidth() *0.5) - 36) + (230 * sin1)), (((getHeight() *0.5) - 36) + (230 * cosN1)) ,72, 72);
	pads[11]->setBounds((((getWidth() *0.5) - 36) + (230 * sinN1)), (((getHeight() *0.5) - 36) + (230 * cosN1)) ,72, 72);
	pads[12]->setBounds((((getWidth() *0.5) - 36) + (230 * sinN2)), (((getHeight() *0.5) - 36) + (230 * cosN2)) ,72, 72);
	pads[13]->setBounds((((getWidth() *0.5) - 36) + (230 * sinN2)), (((getHeight() *0.5) - 36) + (230 * cos2)) ,72, 72);
	pads[14]->setBounds((((getWidth() *0.5) - 36) + (230 * sinN1)), (((getHeight() *0.5) - 36) + (230 * cos1)) ,72, 72);
	pads[15]->setBounds((((getWidth() *0.5) - 36) + (230 * sin1)), (((getHeight() *0.5) - 36) + (230 * cos1)) ,72, 72);
	pads[8]->setBounds((((getWidth() *0.5) - 36) + (230 * sin2)), (((getHeight() *0.5) - 36) + (230 * cos2)) ,72, 72);
 
	pads[0]->setBounds((((getWidth() *0.5) - 24) + (260 * sin3)), (((getHeight() *0.5) - 24) + (260 * cosN3)) ,48, 48);
	pads[1]->setBounds((((getWidth() *0.5) - 24) + (260 * 0)), (((getHeight() *0.5) - 24) + (260 * -1)) ,48, 48);
	pads[2]->setBounds((((getWidth() *0.5) - 24) + (260 * sinN3)), (((getHeight() *0.5) - 24) + (260 * cosN3)) ,48, 48);
	pads[3]->setBounds((((getWidth() *0.5) - 24) + (260 * -1)), (((getHeight() *0.5) - 24) + (260 * 0)) ,48, 48);
	pads[4]->setBounds((((getWidth() *0.5) - 24) + (260 * sinN3)), (((getHeight() *0.5) - 24) + (260 * cos3)) ,48, 48);
	pads[5]->setBounds((((getWidth() *0.5) - 24) + (260 * 0)), (((getHeight() *0.5) - 24) + (260 * 1)) ,48, 48);
	pads[6]->setBounds((((getWidth() *0.5) - 24) + (260 * sin3)), (((getHeight() *0.5) - 24) + (260 * cos3)) ,48, 48);
	pads[7]->setBounds((((getWidth() *0.5) - 24) + (260 * 1)), (((getHeight() *0.5) - 24) + (260 * 0)) ,48, 48);
	
}


//observers update function, called everytime an OSC message is recieved
bool GuiPadLayout::update(const Subject& theChangedSubject)
{
    if (&theChangedSubject == &mSubject)
    {
        if (mSubject.getGuiUpdateFlag() == 0) //update gradient from pressure
        {
            //if 'show pad settings when pressed' control is set to on, the pad settings are currently
            //not being displayed, display current settings of the pressed pad
            /*
            if (shouldDisplaySettings == true)
            {
                
                if (pStore != mSubject.getRecievedPad()) 
                {
                    turnOff(pStore);
                    pStore = mSubject.getRecievedPad();
                    mainComponentRef.setCurrentlySelectedPad(pStore);
                
                    pads[mSubject.getRecievedPad()]->turnOn();
                    pads[mSubject.getRecievedPad()]->toggleChange();
                }
            }
             */
            
            //update gradient
            pads[mSubject.getRecievedPad()]->setGradient(mSubject.getRecievedValue());
            
            
        }
        else if (mSubject.getGuiUpdateFlag() == 1) //update pad playing status visual signifier
        {
            pads[mSubject.getPadNumberForPlayingStatus()]->setPadPlayingState(mSubject.getPlayingStatus());
            mSubject.setGuiUpdateFlag(0);   // this is needed so that the pad gradient can be set to 0
                                            // when the playing status is being changed at the same time.
                                            // Without this line the pad will hang orange.
            //However is strange how this doesn't apply when using alt-click to emulate mouse clicks!!

        }
        
    }
    
    return true;
}


void GuiPadLayout::rotated(int val)
{
	//function is called once the pad rotate slider drag has ended to rotate individual pads to original angle 
	rotateValue = val;
	rotateValue = -rotateValue;
	repaint();
}

void GuiPadLayout::paint (Graphics& g)
{
		
	if (s == 1) 
    {
		//used to create image snapshot of component and children
		paintEntireComponent(g, false);
	}
	
	for (int i = 0; i <= 47; i++) 
    {
		//variables to find centre point of individual pads
		pivotX = pads[i]->getX() + (pads[i]->getWidth() * 0.5);
		pivotY = pads[i]->getY() + (pads[i]->getHeight() * 0.5);
		
		//transformation to rotate pads back to original angle
		pads[i]->setTransform (AffineTransform::rotation ((float) (rotateValue / (180.0 / double_Pi)), pivotX , pivotY));	
		
		}
	
	
}


bool GuiPadLayout::hitTest (int x, int y)
{
    //Do we need a custom hit test for this component? - CPU usage is minimal (if at all different)
    //all the child components have their own custom hit tests,
    //which should be enough.
    
	//returns true if mouse is within path hitpath
	return hitPath.contains(x, y);

}

 
void GuiPadLayout::buttonClicked(Button *button)
{
	//function to listen for button clicks and turn on whichever pad or row has been clicked on and turn off the previous pad or row
    
    //get modifier key so we can handle cmd-clicks and shift-clicks when selecting pads
    ModifierKeys modifier = ModifierKeys::getCurrentModifiers();
    
    //if a regular click is performed, clear the selectedPads array and clear all pads
    if (modifier.isCommandDown() == false && modifier.isShiftDown() == false)
    {
        selectedPads.clear(); //should I call clearQuick() instead?
        for (int i = 0; i <=47; i++)
            turnOff(i);
    }
	
    //individual pads
    for (int i = 0; i <=47; i++)
    {
        if (button == pads[i]->getButton())
        {
            //if shift is down....
            if (modifier.isShiftDown() == true)
            {
                //select and turn on all the pads within the range if the 
                //newly selected pad and the last selected pad
                
                int newPad = i;
                int lastPad = selectedPads.getLast(); //this will return 0 if the array is empty (which is also a pad number),
                                                        //but the array should never be empty at this point, right?
                
                if (newPad > lastPad)
                {
                    for (int i = lastPad; i <= newPad; i++)
                    {
                        selectedPads.addIfNotAlreadyThere(i);
                        turnOn(i); 
                    }
                }
                else if (newPad < lastPad)
                {
                    for (int i = lastPad; i >= newPad; i--)
                    {
                        selectedPads.addIfNotAlreadyThere(i);
                        turnOn(i); 
                    }
                }
                //if newPad == lastPad, do nothing as that pad would have already been selected
            }
            //else, cmd-click or regular click...
            else
            {
                if (modifier.isCommandDown() == true && selectedPads.contains(i))
                {
                    //if cmd-click and pad is already selected, unselected
                    selectedPads.removeFirstMatchingValue(i);
                    turnOff(i);
                }
                else
                {
                    //select and turn on pad
                    selectedPads.addIfNotAlreadyThere(i);
                    turnOn(i);
                }
            }
            
            break; // so it doesn't check for other pads afterwards
        }
    }
    
	
    
    //row buttons...
	if (button == row1Sb || button == row2Sb || button == row3Sb || button == row4Sb ||button == row5Sb || button == row6Sb) 
    {
        //for shift click stuff
        int lastPad = selectedPads.getLast();
        int newPad;
        
        int row; 
        
        //get row based on clicked button...
        if (button == row1Sb)
            row = 1;
        else if (button == row2Sb)
            row = 2;
        else if (button == row3Sb)
            row = 3;
        else if (button == row4Sb)
            row = 4;
        else if (button == row5Sb)
            row = 5;
        else if (button == row6Sb)
            row = 6;
               
        //select and turn on pads is selected row
        for(int i = (row*8)-8; i <= (row*8)-1; i++) 
        {
            if (modifier.isCommandDown() == true && selectedPads.contains(i))
            {
                selectedPads.removeFirstMatchingValue(i);
                turnOff(i);
                newPad = i; //for shift-click stuff
            }
            else
            {
                selectedPads.addIfNotAlreadyThere(i);
                turnOn(i);
                newPad = i;
            }
        }
        
        
        //if shift is down....
        if (modifier.isShiftDown() == true)
        {
            if (newPad > lastPad)
            {
                for (int i = lastPad; i <= newPad; i++)
                {
                    selectedPads.addIfNotAlreadyThere(i);
                    turnOn(i); 
                }
            }
            else if (newPad < lastPad)
            {
                for (int i = lastPad; i >= newPad; i--)
                {
                    selectedPads.addIfNotAlreadyThere(i);
                    turnOn(i); 
                }
            }
        }
	}
    
    
    //global button...
	if (button == globalSb)
    {
		for (int i = 0; i <=47; i++) 
        {
            if (modifier.isCommandDown() == true && selectedPads.contains(i))
            {
                selectedPads.removeFirstMatchingValue(i);
                turnOff(i);
            }
            else
            {
                selectedPads.addIfNotAlreadyThere(i);
                turnOn(i);
            }
		}
	}
	
    if (selectedPads.size() > 0)
    {
        //pass on the selected pads to mainComponent, which then passes it further into the application..
        mainComponentRef.setCurrentlySelectedPad(selectedPads);
    }
	
}

void GuiPadLayout::turnOn(int pad)
{
    pads[pad]->turnOn();
    pads[pad]->toggleChange();
}

void GuiPadLayout::turnOff(int pad)
{
    pads[pad]->turnOff();
    pads[pad]->toggleChange();
}


void GuiPadLayout::modeChange(int padNumber, int modeNumber)
{
    pads[padNumber]->modeChange(modeNumber);
}


Image GuiPadLayout::snapshot()
{
	//returns an image of a complete snapshot of this component, including children
	
	s = 1;
	repaint();
	s = 0;
	snap = createComponentSnapshot(Rectangle<int>(0, 0, getWidth(), getHeight()), true);
	return snap;
	
}

/*
MainComponent& GuiPadLayout::getMainComponent()
{
    return mainComponentRef;
}
 */

AlphaLiveEngine& GuiPadLayout::getAlphaLiveEngine()
{
    return mSubject;
}

MainComponent& GuiPadLayout::getMainComponent()
{
    return mainComponentRef;
}

void GuiPadLayout::updateCurrentlySelectedPad (int padNumber)
{
    mainComponentRef.setCurrentlySelectedPad(selectedPads);
    
}

void GuiPadLayout::setShouldDisplaySettings (bool value)
{
    shouldDisplaySettings = value;
}

void GuiPadLayout::mouseEnter (const MouseEvent &e)
{
    for (int i = 0; i <= 47; i++) 
    {
        if (pads[i]->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText("Pad " + String(i+1) + ". Click here to view and edit the settings of this pad. Right-click to view copy & paste options. Alt-Click and drag to emulate a pad press. Drag audio files here for the Looper Mode. The outer-ring colour signifies the pad's mode.");
        }
    }
    
    if (globalSb->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Select All Pads. Click here to apply settings to all of the pads on the AlphaSphere.");
    }
    else if (row1Sb->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Select Row 1. Click here to apply settings to the pads on row 1 (bottom row) of the AlphaSphere.");
    }
    else if (row2Sb->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Select Row 2. Click here to apply settings to the pads on row 2 (second from bottom row) of the AlphaSphere.");
    }
    else if (row3Sb->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Select Row 3. Click here to apply settings to the pads on row 3 (bottom-middle row) of the AlphaSphere.");
    }
    else if (row4Sb->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Select Row 4. Click here to apply settings to the pads on row 4 (top-middle row) of the AlphaSphere.");
    }
    else if (row5Sb->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Select Row 5. Click here to apply settings to the pads on row 5 (second from top row) of the AlphaSphere.");
    }
    else if (row6Sb->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Select Row 6. Click here to apply settings to the pads on row 6 (top row) of the AlphaSphere.");
    }
       
    
}

void GuiPadLayout::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty); 
}



void GuiPadLayout::copyPadSettings()
{
    pads[currentlySelectedPad]->copyPadSettings();
}


void GuiPadLayout::pastePadSettings()
{
    pads[currentlySelectedPad]->pastePadSettings();
}


