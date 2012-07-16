/*
 *  ModeButton.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 14/09/2011.
 *  Copyright 2011 __Nu Desine__.
 *
 */

#include "ModeButton.h"
#include "../../Binary Data/BinaryData.h"

//==============================================================================

ModeButton::ModeButton (Image* modeSymbol)
: Button ("ModeButton"),
alphaThreshold (0),
imageX (0),
imageY (0),
imageW (0),
imageH (0),
symVar(modeSymbol)


{
    OffImage = ImageFileFormat::loadFrom (BinaryData::modebuttonoff_png, BinaryData::modebuttonoff_pngSize);
    OverImage = ImageFileFormat::loadFrom (BinaryData::modebuttonover_png, BinaryData::modebuttonover_pngSize);
    ClickImage = ImageFileFormat::loadFrom (BinaryData::modebuttonclick_png, BinaryData::modebuttonclick_pngSize);
    OnImage = ImageFileFormat::loadFrom (BinaryData::modebuttonon_png, BinaryData::modebuttonon_pngSize);
	
	setClickingTogglesState(true);
	
	imageW = OffImage.getWidth();
	imageH = OffImage.getHeight();
	
	setSize (imageW, imageH);
	
	normalOpacity = 1.0f;
    normalOverlay = Colours::transparentBlack;
    overOpacity   = 0.5f;
    overOverlay   = Colours::blue;
    downOpacity   = 0.5f;
    downOverlay   = Colours::white;
	onOpacity   = 0.5f;
    onOverlay   = Colours::orange;
	
	alphaThreshold = 100.0f;
	
	
}

ModeButton::~ModeButton()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]
	
	delete symVar;
	
    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

Image ModeButton::getCurrentImage() const
{
    if (isDown() || getToggleState())
        return getDownImage();
	
    if (isOver())
        return getOverImage();
	
    return getNormalImage();
}

Image ModeButton::getNormalImage() const
{
    return OffImage;
}

Image ModeButton::getOverImage() const
{
    return OverImage.isValid() ? OverImage
	: OffImage;
}

Image ModeButton::getDownImage() const
{
    return ClickImage.isValid() ? ClickImage
	: getOverImage();
}

Image ModeButton::getOnImage() const
{
    return OnImage.isValid() ? OnImage
	: getOverImage();
}

//==============================================================================
void ModeButton::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}


void ModeButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
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
                         0, 0, 100, 100, false);
			
			g.drawImage (* symVar,
                        imageX, imageY, imageW, imageH,
                        0, 0, 100, 100, false);
            break;
        }
			
		case 1:
        {
            
            g.drawImage (OverImage,
                         imageX, imageY, imageW, imageH,
                         0, 0, 100, 100, false);
			
			g.setColour(overOverlay);
			g.drawImage (* symVar,
						 imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, false);
			g.setOpacity(overOpacity);
			g.drawImage (* symVar,
						imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, true);
		
            break;
        }
			
		case 2:
        {
           
            g.drawImage (ClickImage,
                         imageX, imageY, imageW, imageH,
                         0, 0, 100, 100, false);
			
			g.setColour(downOverlay);
			g.drawImage (* symVar,
						 imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, false);
			g.setOpacity(downOpacity);
			g.drawImage (* symVar,
						imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, true);
			
            break;
        }
			
		case 3:
        {
           
            g.drawImage (OnImage,
                       imageX, imageY, imageW, imageH,
                         0, 0, 100, 100, false);
			
			g.setColour(onOverlay);
			g.drawImage (* symVar,
						 imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, false);
			
			g.setOpacity(onOpacity);
			g.drawImage (* symVar,
						imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, true);
			
			
            break;
        }
			
		case 4:
        {
           
            g.drawImage (OnImage,
                         imageX, imageY, imageW, imageH,
                         0, 0, 100, 100, false);
			
			g.setColour(onOverlay);
			g.drawImage (* symVar,
						 imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, false);
			g.setOpacity(onOpacity);
			g.drawImage (* symVar,
						 imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, true);
			
            break;
        }
			
		case 5:
        {
       
            g.drawImage (OnImage,
                       imageX, imageY, imageW, imageH,
                         0, 0, 100, 100,false);
			
			g.setColour(onOverlay);
			g.drawImage (* symVar,
						 imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, false);
			g.setOpacity(onOpacity);
			g.drawImage (* symVar,
						 imageX, imageY, imageW, imageH,
						 0, 0, 100, 100, true);
			
            break;
        }
			
		default:
			break;
    }
	 
	
}



void ModeButton::clicked()
{
    //[UserCode_clicked] -- Add your code here...
    //[/UserCode_clicked]
	

}

void ModeButton::buttonStateChanged()
{
   //

}

bool ModeButton::hitTest (int x, int y)
{
    if (alphaThreshold == 0)
        return true;
	
    Image im (getCurrentImage());
	
    return im.isNull() || (imageW > 0 && imageH > 0
						   && alphaThreshold < im.getPixelAt (((x - imageX) * im.getWidth()) / imageW,
															  ((y - imageY) * im.getHeight()) / imageH).getAlpha());
}




