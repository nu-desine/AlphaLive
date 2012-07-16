/*
 *  GuiPianoKeys.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 28/09/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "GuiPianoKeys.h"



//==============================================================================

GuiPianoKeys::GuiPianoKeys (Image* offIm, Image* overIm, Image* downIm, Image* onIm)
: Button ("GuiPianoKeys"),
alphaThreshold (0),
imageX (0),
imageY (0),
imageW (0),
imageH (0),
OffImage (*offIm),
OverImage (*overIm),
OnImage (*onIm),
ClickImage (*downIm)
{
    
	
	setClickingTogglesState(true);
	
	imageW = OffImage.getWidth();
	imageH = OffImage.getHeight();
	
	setSize (imageW, imageH);
	
	
	alphaThreshold = 100.0f;
	
	
}

GuiPianoKeys::~GuiPianoKeys()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]
	
	
	
    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

Image GuiPianoKeys::getCurrentImage() const
{
    if (isDown() || getToggleState())
        return getDownImage();
	
    if (isOver())
        return getOverImage();
	
    return getNormalImage();
}

Image GuiPianoKeys::getNormalImage() const
{
    return OffImage;
}

Image GuiPianoKeys::getOverImage() const
{
    return OverImage.isValid() ? OverImage
	: OffImage;
}

Image GuiPianoKeys::getDownImage() const
{
    return ClickImage.isValid() ? ClickImage
	: getOverImage();
}

Image GuiPianoKeys::getOnImage() const
{
    return OnImage.isValid() ? OnImage
	: getOverImage();
}

//==============================================================================
void GuiPianoKeys::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}


void GuiPianoKeys::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	
	Image im (getCurrentImage());
	
	if (im.isValid())
    {
        const int iw = im.getWidth();
        const int ih = im.getHeight();
        imageW = getWidth();
        imageH = getHeight();
        imageX = (imageW - iw) >> 1;
        imageY = (imageH - ih) >> 1;
		
		int newW, newH;
		const float imRatio = ih / (float)iw;
		const float destRatio = imageH / (float)imageW;
		
		if (imRatio > destRatio)
		{
			newW = roundToInt (imageH / imRatio);
			newH = imageH;
		}
		else
		{
			newW = imageW;
			newH = roundToInt (imageW * imRatio);
		}
		
		imageX = (imageW - newW) / 2;
		imageY = (imageH - newH) / 2;
		imageW = newW;
		imageH = newH;
		
		
	}
	
	
	switch (getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            
            g.drawImage (OffImage,
                         imageX, imageY, imageW, imageH,
                         0, 0, 230, 230, false);
			
            break;
        }
			
		case 1:
        {
            
            g.drawImage (OverImage,
                         imageX, imageY, imageW, imageH,
                         0, 0, 230, 230, false);
			
			
            break;
        }
			
		case 2:
        {
			
            g.drawImage (ClickImage,
                         imageX, imageY, imageW, imageH,
                         0, 0, 230, 230, false);
			
			
            break;
        }
			
		case 3:
        {
			
            g.drawImage (OnImage,
						 imageX, imageY, imageW, imageH,
                         0, 0, 230, 230, false);
			
			
            break;
        }
			
		case 4:
        {
			
            g.drawImage (OnImage,
                         imageX, imageY, imageW, imageH,
                         0, 0, 230, 230, false);
			
			
            break;
        }
			
		case 5:
        {
			
            g.drawImage (OnImage,
						 imageX, imageY, imageW, imageH,
                         0, 0, 230, 230,false);
		
			
            break;
        }
			
		default:
			break;
    }
	
	
}



void GuiPianoKeys::clicked()
{
    //[UserCode_clicked] -- Add your code here...
    //[/UserCode_clicked]
	
	
}

void GuiPianoKeys::buttonStateChanged()
{
	//
	
}

bool GuiPianoKeys::hitTest (int x, int y)
{
    if (alphaThreshold == 0)
        return true;
	
    Image im (getCurrentImage());
	
    return im.isNull() || (imageW > 0 && imageH > 0
						   && alphaThreshold < im.getPixelAt (((x - imageX) * im.getWidth()) / imageW,
															  ((y - imageY) * im.getHeight()) / imageH).getAlpha());
}
