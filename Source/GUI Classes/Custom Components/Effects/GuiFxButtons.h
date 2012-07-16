/*
 *  GuiFxButtons.h
 *  sdaJuce
 *
 *  Created by Sam Davies on 19/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#ifndef GUIFXBUTTONS_H
#define GUIFXBUTTONS_H

#include "../../../../JuceLibraryCode/JuceHeader.h"


class GuiFxButtons :	public Button


{
public:
    
	
    GuiFxButtons(Image* offIm, Image* overIm, Image* downIm);
    
    /** Destructor. */
    ~GuiFxButtons();
	
	
	
	
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
	
    
	void resized();
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
    void clicked();
    void buttonStateChanged();
	bool hitTest (int x, int y);
	
	
    
private:
    //==============================================================================
    
    unsigned char alphaThreshold;
	int imageX, imageY, imageW, imageH;
	Image OffImage, OverImage, ClickImage;
	
	Image getCurrentImage() const;
	
	
	
};    


#endif //GUIFXBUTTONS_H