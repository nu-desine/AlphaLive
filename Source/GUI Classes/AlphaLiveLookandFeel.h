/*
 *  AlphaLiveLookandFeel.h
 *  AlphaLive
 *
 *  Created by Sam Davies on 12/06/2012.
 *  Copyright 2012 nu desine. All rights reserved.
 *
 */

#ifndef H_ALPHACOLOURS
#define H_ALPHACOLOURS

#include "./../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 A singleton to hold custom colours for AlphaLive.
 This used to be a namespace, however I changed it to a singleton
 so that the colour variables could be dynamically changed
 to allow for the colour scheme to be changed by the user.
 I'm using a singleton as it seemed to be the easiest way
 of integrating changable colours into the application which
 was originally designed to have static colours. Therefore
 it is a bit hacky.
 */
class AlphaColours
{
public:
    //==============================================================================
    AlphaColours();
    ~AlphaColours();
    
    juce_DeclareSingleton (AlphaColours, false);
    
    uint32 colour1_, colour2_, colour3_, colour4_;
    Colour colour1, colour2, colour3, colour4;
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaColours);
};

#endif


#ifndef ALPHALIVELOOKANDFEEL_H
#define ALPHALIVELOOKANDFEEL_H

#include "../JuceLibraryCode/JuceHeader.h"

class AlphaLiveLookandFeel : public LookAndFeel
{
    
public:
	AlphaLiveLookandFeel();
	~AlphaLiveLookandFeel();
    
    //===custom fuctions====
    
    void setStandardColours (int theme);
    
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
