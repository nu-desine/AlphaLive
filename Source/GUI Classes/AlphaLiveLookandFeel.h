/*
 *  AlphaLiveLookandFeel.h
 *  AlphaLive
 *
 *  Created by Sam Davies on 12/06/2012.
 *  Copyright 2012 nu desine. All rights reserved.
 *
 */


#ifndef ALPHALIVELOOKANDFEEL_H
#define ALPHALIVELOOKANDFEEL_H

#include "../JuceLibraryCode/JuceHeader.h"

namespace AlphaColours
{
    //Create a uint32 and a Juce::Colour of any 
    //custom colours for the application here.
    
    //colours as uint32
    const uint32 blue_ = 0xff1a54ab;
    const uint32 lightblue_ = 0xff3c76c5;
	const uint32 nearlyblack_ = 0xff181818;
	const uint32 verydarkgrey_ = 0xff202020;
    
    //colours as Juce::Colour
    static Colour blue(blue_);
    static Colour lightblue(lightblue_);
	static Colour nearlyblack(nearlyblack_);
	static Colour verydarkgrey(verydarkgrey_);
    
}

class AlphaLiveLookandFeel : public LookAndFeel


{
public:
	
	AlphaLiveLookandFeel();
	
	~AlphaLiveLookandFeel();
	
	void drawButtonBackground (Graphics& g,
                                       Button& button,
                                       const Colour& backgroundColour,
                                       bool isMouseOverButton,
                                       bool isButtonDown);
	
	const Font getFontForTextButton (TextButton& button);
	
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
    
    
	
	const Font getPopupMenuFont();
    
    void drawPopupMenuBackground (Graphics &g, int width, int height);
    
    /*
    void drawPopupMenuItem (Graphics& g,
                            int width, int height,
                            bool isSeparator,
                            bool isActive,
                            bool isHighlighted,
                            bool isTicked,
                            bool hasSubMenu,
                            const String& text,
                            const String& shortcutKeyText,
                            Image* image,
                            const Colour* const textColour);
    
    void getIdealPopupMenuItemSize (const String& text,
                                    bool isSeparator,
                                    int standardMenuItemHeight,
                                    int& idealWidth,
                                    int& idealHeight);
	
	
    */
    
	int getDefaultScrollbarWidth(); //<-- why is this not working properly for the fileListComponent? need to scroll it first to adjust the size. Think its working now.
                                    
   	
private:
    //==============================================================================
   	
    Array <int> colourIds;
    Array <Colour> colours;
	
};    


#endif   // ALPHALIVELOOKANDFEEL_H
