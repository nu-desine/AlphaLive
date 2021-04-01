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
    
    // used for tab colours as well as background colour for pop up views and lots of outlines
    uint32 foregroundColourDarker_;
    
    // colour for button images/icons. Works best when this is the same as textColour_
    uint32 iconColour_;
    
    // colour for button images/icons when button is on.
    uint32 iconOnColour_;
    
    // background/outline colour for pads
    uint32 padColourBackground_;
    
    // main foreground colour for pads
    uint32 padColour_;
    
    // foreground colour for first pad of each row
    uint32 padColour2_;
    
    // colour for pads text
    uint32 padTextColour_;
    
    // colour for toolbox tabs
    uint32 tabColour_;
    
    Colour mainColour, mainColourLighter, childBackgroundColour, childBackgroundColourLighter, textColour;
    Colour backgroundColour, foregroundColour, foregroundColourLighter, foregroundColourDarker, iconColour;
    Colour padColourBackground, padColour, padColour2, padTextColour, tabColour, iconOnColour;
    
    //=============================================
    // background images
    Image mainImage, padsOffImage, padsOnImage, modeOffImage, padsBackgroundImage, settingsOffImage;
    
    // the following variable is changed when the localisation is changed.
    // is doesn't actually have anything to do with themes, but this seems like
    // the most appropriate place for the variable to live.
    float fontSizeAddition;
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaTheme);
};

#endif //H_ALPHATHEME


#ifndef ALPHALIVELOOKANDFEEL_H
#define ALPHALIVELOOKANDFEEL_H

#include "../JuceLibraryCode/JuceHeader.h"

class AlphaLiveLookandFeel : public LookAndFeel_V2
{
    
public:
	AlphaLiveLookandFeel();
	~AlphaLiveLookandFeel();
    
    //===custom fuctions====
    
    void setTheme (int theme);
    
    //===overridden functions===
	
    //=====================================================================
	void drawButtonBackground (Graphics& g,
                                       Button& button,
                                       const Colour& backgroundColour,
                                       bool isMouseOverButton,
                                       bool isButtonDown) override;
	
	Font getTextButtonFont (TextButton& button, int buttonHeight) override;
	
	void drawButtonText (Graphics& g, TextButton& button,
						 bool isMouseOverButton, bool isButtonDown) override;
	
    //=====================================================================
	void drawComboBox (Graphics& g, int width, int height,
											 const bool isButtonDown,
											 int buttonX, int buttonY,
											 int buttonW, int buttonH,
											 ComboBox& box) override;
    
    Font getComboBoxFont (ComboBox&) override;
    Label* createComboBoxTextBox (ComboBox&) override;
	
	//void positionComboBoxText (ComboBox& box, Label& label) override;
	
	//=====================================================================
	void layoutFilenameComponent (FilenameComponent& filenameComp,
								  ComboBox* filenameBox,
								  Button* browseButton) override;
     
	//=====================================================================
	void drawLabel (Graphics& g, Label& label) override;
	
    //=====================================================================
	void drawRotarySlider (Graphics& g,
										int x, int y,
										int width, int height,
										float sliderPos,
										const float rotaryStartAngle,
										const float rotaryEndAngle,
										Slider& slider) override;
    
    
	//=====================================================================
	Font getPopupMenuFont() override;
    
    void drawPopupMenuBackground (Graphics &g, int width, int height) override;
    
    //=====================================================================
    
	int getDefaultScrollbarWidth() override;

    //=====================================================================
	int getDefaultMenuBarHeight() override;
	void drawMenuBarBackground (Graphics& g, int width, int height, bool, MenuBarComponent& menuBar) override;
    
    //=====================================================================
    
    void drawFileBrowserRow (Graphics&, int width, int height,
                             const File& file, const String& filename, Image* icon,
                             const String& fileSizeDescription, const String& fileTimeDescription,
                             bool isDirectory, bool isItemSelected, int itemIndex,
                             DirectoryContentsDisplayComponent&) override;
                                    
   	
private:
    //==============================================================================
   	
    Array <int> colourIds;
    Array <Colour> colours;
	
};    


#endif   // ALPHALIVELOOKANDFEEL_H
