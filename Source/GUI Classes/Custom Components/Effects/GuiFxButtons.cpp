/*
 *  GuiFxButtons.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 19/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "GuiFxButtons.h"


GuiFxButtons::GuiFxButtons (Image* offIm, Image* overIm, Image* downIm)
        :   Button ("GuiFxButtons"),
            alphaThreshold (0),
            imageX (0),
            imageY (0),
            imageW (0),
            imageH (0),
            OffImage (*offIm),
            OverImage (*overIm),
            ClickImage (*downIm)
{
	
	setClickingTogglesState(true);
	
	imageW = OffImage.getWidth();
	imageH = OffImage.getHeight();
	
	setSize (imageW, imageH);
	
	alphaThreshold = 100.0f;
	
}

GuiFxButtons::~GuiFxButtons()
{
   
}


Image GuiFxButtons::getCurrentImage() const
{
    if (isDown() || getToggleState())
        return getDownImage();
	
    if (isOver())
        return getOverImage();
	
    return getNormalImage();
}

Image GuiFxButtons::getNormalImage() const
{
    return OffImage;
}

Image GuiFxButtons::getOverImage() const
{
    return OverImage.isValid() ? OverImage
	: OffImage;
}

Image GuiFxButtons::getDownImage() const
{
    return ClickImage.isValid() ? ClickImage
	: getOverImage();
}



//==============================================================================
void GuiFxButtons::resized()
{
    
}


void GuiFxButtons::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
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
            g.drawImage (OffImage, imageX, imageY, imageW, imageH, 0, 0, 230, 230, false);
            break;
			
		case 1:
            g.drawImage (OverImage, imageX, imageY, imageW, imageH, 0, 0, 230, 230, false);
            break;
			
		case 2:
            g.drawImage (ClickImage, imageX, imageY, imageW, imageH, 0, 0, 230, 230, false);
            break;
			
		case 3:
            g.drawImage (ClickImage, imageX, imageY, imageW, imageH, 0, 0, 230, 230, false);
            break;
			
		case 4:
            g.drawImage (ClickImage, imageX, imageY, imageW, imageH, 0, 0, 230, 230, false);
            break;
			
		case 5:
            g.drawImage (ClickImage, imageX, imageY, imageW, imageH, 0, 0, 230, 230,false);
            break;
			
		default:
			break;
    }

}



void GuiFxButtons::clicked()
{
    
}

void GuiFxButtons::buttonStateChanged()
{
	
}

bool GuiFxButtons::hitTest (int x, int y)
{
    if (alphaThreshold == 0)
        return true;
	
    Image im (getCurrentImage());
	
    return im.isNull() || (imageW > 0 && imageH > 0
						   && alphaThreshold < im.getPixelAt (((x - imageX) * im.getWidth()) / imageW,
															  ((y - imageY) * im.getHeight()) / imageH).getAlpha());
}
