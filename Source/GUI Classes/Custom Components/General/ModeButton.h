/*
 *  ModeButton.h
 *  sdaJuce
 *
 *  Created by Sam Davies on 14/09/2011.
 *  Copyright 2011 __NuDesine__.
 *
 */

#ifndef MODEBUTTON_H
#define MODEBUTTON_H

#include "../../../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 A class that creates a custom imagebutton with an image as an argument.

 */

class ModeButton :	public Button
					
					
{
public:
    
    //==============================================================================
    /** Creates an imagebutton toggle.
     
     @param image               the image to be applied to the button
          
     */
    ModeButton(Image* modeSymbol);
    
    /** Destructor. */
    ~ModeButton();
	
	
	
	
	/** Returns the currently set 'normal' image. */
    Image getNormalImage() const;
	
    /** Returns the image that's drawn when the mouse is over the button.
	 
	 If a valid 'over' image has been set, this will return it; otherwise it'll
	 just return the normal image.
	 */
    Image getOverImage() const;
	
    /** Returns the image that's drawn when the button is held down.
	 
	 If a valid 'down' image has been set, this will return it; otherwise it'll
	 return the 'over' image or normal image, depending on what's available.
	 */
    Image getDownImage() const;
	
	/** Returns the image that's drawn when the button is on.
	 
	 If a valid 'on' image has been set, this will return it; otherwise it'll
	 return the 'over' image or normal image, depending on what's available.
	 */
    Image getOnImage() const;
    
	void resized();
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
    void clicked();
    void buttonStateChanged();
	bool hitTest (int x, int y);
	

    
private:
    //==============================================================================
    
    unsigned char alphaThreshold;
	int imageX, imageY, imageW, imageH;
	Image OffImage, OverImage, OnImage, ClickImage;
	float normalOpacity, overOpacity, downOpacity, onOpacity;
    Colour normalOverlay, overOverlay, downOverlay, onOverlay;
	Image* symVar;

	Image getCurrentImage() const;
	
	
   
};    


#endif //MODEBUTTON_H