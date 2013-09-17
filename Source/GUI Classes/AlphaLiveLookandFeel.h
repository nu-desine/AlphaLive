/*
 *  AlphaLiveLookandFeel.h
 *  AlphaLive
 *
 *  Created by Sam Davies on 12/06/2012.
 *  Copyright 2012 nu desine. All rights reserved.
 *
 */

#ifndef H_ALPHATHEME
#define H_ALPHATHEME

#include "./../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 A singleton to hold custom colours and the background images for AlphaLive.
 This used to be a namespace, however I changed it to a singleton
 so that the colour and image variables could be dynamically changed
 to allow for the colour scheme and skin to be changed by the user.
 I'm using a singleton as it seemed to be the easiest way
 of integrating changable colours into the application which
 was originally designed to have static colours. Therefore
 it may be a bit hacky.
 */
class AlphaTheme
{
public:
    //==============================================================================
    AlphaTheme();
    ~AlphaTheme();
    
    juce_DeclareSingleton (AlphaTheme, false);
    
    //=============================================
    // custom/default colours
    
    // colour for selected buttons, sliders, other selected/highlighted components and more
    uint32 mainColour_;
    
    // colour for gradients on buttons amoung a few other things
    uint32 mainColourLighter_;
    
    // background colour of certain child components such as setting buttons
    uint32 childBackgroundColour_;
    
    // mainly used as a component outline colour
    uint32 childBackgroundColourLighter_;
    
    // colour of most text
    uint32 textColour_;
    
    // main background colour for most components and general graphics
    uint32 backgroundColour_;
    
    // main colour for smaller components and various shapes/lines
    uint32 foregroundColour_;
    
    // mainly used as a second text colour. This should be fairly similar to textColour_
    uint32 foregroundColourLighter_;
    
    // used for tab colours as well as background colour for pop up views
    uint32 foregroundColourDarker_;
    
    // colour for button images/icons. Works best when this is the same as textColour_
    uint32 iconColour_;
    
    Colour mainColour, mainColourLighter, childBackgroundColour, childBackgroundColourLighter, textColour;
    Colour backgroundColour, foregroundColour, foregroundColourLighter, foregroundColourDarker, iconColour;
    
    //=============================================
    // background images
    Image mainImage, padsOffImage, padsOnImage, modeOffImage, padsBackgroundImage, settingsOffImage;
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaTheme);
};

#endif //H_ALPHATHEME


#ifndef ALPHALIVELOOKANDFEEL_H
#define ALPHALIVELOOKANDFEEL_H

#include "../JuceLibraryCode/JuceHeader.h"

class AlphaLiveLookandFeel : public LookAndFeel
{
    
public:
	AlphaLiveLookandFeel();
	~AlphaLiveLookandFeel();
    
    //===custom fuctions====
    
    void setTheme (int theme);
    
    //===overridden functions===
	
	void drawButtonBackground (Graphics& g,
                                       Button& button,
                                       const Colour& backgroundColour,
                                       bool isMouseOverButton,
                                       bool isButtonDown);
	
	Font getTextButtonFont (TextButton& button);
	
	void drawButtonText (Graphics& g, TextButton& button,
						 bool isMouseOverButton, bool isButtonDown);
	
	void drawComboBox (Graphics& g, int width, int height,
											 const bool isButtonDown,
											 int buttonX, int buttonY,
											 int buttonW, int buttonH,
											 ComboBox& box);
	
	void positionComboBoxText (ComboBox& box, Label& label);
	
	
	
	void layoutFilenameComponent (FilenameComponent& filenameComp,
								  ComboBox* filenameBox,
								  Button* browseButton);
     
	
	void drawLabel (Graphics& g, Label& label);
	
	void drawRotarySlider (Graphics& g,
										int x, int y,
										int width, int height,
										float sliderPos,
										const float rotaryStartAngle,
										const float rotaryEndAngle,
										Slider& slider);
    
    
	
	Font getPopupMenuFont();
    
    void drawPopupMenuBackground (Graphics &g, int width, int height);
    
    
	int getDefaultScrollbarWidth(); 

	int getDefaultMenuBarHeight();
	void drawMenuBarBackground (Graphics& g, int width, int height, bool, MenuBarComponent& menuBar);
    
    void drawFileBrowserRow (Graphics& g, int width, int height,
                             const String& filename, Image* icon,
                             const String& fileSizeDescription,
                             const String& fileTimeDescription,
                             bool isDirectory,
                             bool isItemSelected,
                             int itemIndex,
                             DirectoryContentsDisplayComponent& component);
                                    
   	
private:
    //==============================================================================
   	
    Array <int> colourIds;
    Array <Colour> colours;
	
};    


#endif   // ALPHALIVELOOKANDFEEL_H
