/*
 *  AlphaLiveLookandFeel.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 12/06/2012.
 *  Copyright 2012 nu desine. All rights reserved.
 *
 */

#include "AlphaLiveLookandFeel.h"
#include "../File and Settings/StoredSettings.h"
#include "Binary Data/MainBinaryData.h"
#include "Binary Data/ThemeClassicBinaryData.h"
#include "Binary Data/ThemeMateriaBinaryData.h"
#include "Binary Data/ThemeMinimalBinaryData.h"
#include "Binary Data/ThemeCharcoalBinaryData.h"
#include "Binary Data/ThemeAuroraBinaryData.h"
#include "Binary Data/ThemeTransparencyBinaryData.h"

AlphaTheme::AlphaTheme()
{
    fontSizeAddition = 0;
}

AlphaTheme::~AlphaTheme()
{
    clearSingletonInstance();
}

juce_ImplementSingleton (AlphaTheme);


namespace LookAndFeelHelpers
{
    static void createRoundedPath (Path& p,
                                   const float x, const float y,
                                   const float w, const float h,
                                   const float cs,
                                   const bool curveTopLeft, const bool curveTopRight,
                                   const bool curveBottomLeft, const bool curveBottomRight) noexcept
    {
        const float cs2 = 2.0f * cs;
		
        if (curveTopLeft)
        {
            p.startNewSubPath (x, y + cs);
            p.addArc (x, y, cs2, cs2, float_Pi * 1.5f, float_Pi * 2.0f);
        }
        else
        {
            p.startNewSubPath (x, y);
        }
		
        if (curveTopRight)
        {
            p.lineTo (x + w - cs, y);
            p.addArc (x + w - cs2, y, cs2, cs2, 0.0f, float_Pi * 0.5f);
        }
        else
        {
            p.lineTo (x + w, y);
        }
		
        if (curveBottomRight)
        {
            p.lineTo (x + w, y + h - cs);
            p.addArc (x + w - cs2, y + h - cs2, cs2, cs2, float_Pi * 0.5f, float_Pi);
        }
        else
        {
            p.lineTo (x + w, y + h);
        }
		
        if (curveBottomLeft)
        {
            p.lineTo (x + cs, y + h);
            p.addArc (x, y + h - cs2, cs2, cs2, float_Pi, float_Pi * 1.5f);
        }
        else
        {
            p.lineTo (x, y + h);
        }
		
        p.closeSubPath();
    }
	
    static Colour createBaseColour (const Colour& buttonColour,
                                    const bool hasKeyboardFocus,
                                    const bool isMouseOverButton,
                                    const bool isButtonDown) noexcept
    {
        const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
        const Colour baseColour (buttonColour.withMultipliedSaturation (sat));
		
        if (isButtonDown)
            return baseColour.contrasting (0.2f);
        else if (isMouseOverButton)
            return baseColour.contrasting (0.1f);
		
        return baseColour;
    }
	
    static TextLayout layoutTooltipText (const String& text, const Colour& colour) noexcept
    {
        const float tooltipFontSize = 13.0f + AlphaTheme::getInstance()->fontSizeAddition;
        const int maxToolTipWidth = 400;
		
        AttributedString s;
        s.setJustification (Justification::centred);
        s.append (text, Font (tooltipFontSize, Font::bold), colour);
		
        TextLayout tl;
        tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
        return tl;
    }
}

AlphaLiveLookandFeel::AlphaLiveLookandFeel()
{
    setTheme(StoredSettings::getInstance()->interfaceTheme);
}

AlphaLiveLookandFeel::~AlphaLiveLookandFeel()
{
   
}

//======================================================================
//==========================Custom function=============================
//======================================================================
//======================================================================


void AlphaLiveLookandFeel::setTheme (int theme)
{
    /*
     ====== HOW TO CREATE A NEW THEME =====
     
     1. Put the image files of the theme into a new folder called "Theme_[ThemeName]", and then
        place that folder into the BinaryData folder in the root directory
        of this project. Name the images in the format [themeName]_interfaceMAIN.png, 
        [themeName]_modeoff.png, etc... (See existing theme image folders and files for examples).
     
     2. Put the theme's images into a NEW binary data class using the JUCE BinaryBuilder command
        line application. Store the class files in "Source/Gui Classes/Binary Data", and give it
        the following name - Theme[ThemeName]BinaryData (see existing theme binary data files
        for examples).
     
     3. Add the new binary data files to the IDE project, adding them to the Binary Data group.
        You may also need to manually link the .cpp file to the compiler if you are getting
        linker errors when compiling. You do this by adding the file to the "Compile Sources" list
        under the "Build Phases" section of the "Targets" section.
     
     4. #include the .h file in the AlphaLiveLookandFeel.cpp file.
     
     5. In the PreferencesComponent.cpp file (Source/GUI Classes/Sub-Views), add a new item
        to the interfaceThemeMenu Combobox with a unique ID.
     
     6. In this setTheme() function below copy the entire contents of the "else" statement
        (which sets the theme to CLASSIC) and put it into a new "if else()" above that 
        checks if "theme" is equal to the new theme's ID.
     
     7. Change the AlphaTheme image variables to the corresponding new ones within the new
        binary data class.
     
     8. Change the AlphaTheme uint32 variables that store ARGB colour values. See the AlphaTheme
        class at the top of AlphaLiveLookandFeel.h to see where each of the colours are used
        throughout the application.
     
     9. You don't need to change the standardColours[] array; just changing the AlphaTheme
        colour values should create a sufficent colour scheme. Though there will be no issues
        if you want to change these values.
     
     */
    
    /*
     Colours that can't be changed here:
     - Various colours within the pads. You don't want to change the mode/ring colours.
     - Various whites/silvers that are used for mouse-over highlighting as well as
     shadowing/highlighting buttons/components. We preferably don't want to change this
     - record button colour
     */
    
    jassert (Colours::white == Colour (0xffffffff));
	
    //======================================================================
    //======================================================================
    if (theme == 101) //MATERIA
    {
        //set background images here...
        
        AlphaTheme::getInstance()->mainImage = ImageCache::getFromMemory(ThemeMateriaBinaryData::materia_interfacemain_png,
                                                                         ThemeMateriaBinaryData::materia_interfacemain_pngSize);
        AlphaTheme::getInstance()->padsOffImage = ImageCache::getFromMemory(ThemeMateriaBinaryData::materia_padsoff_png,
                                                                            ThemeMateriaBinaryData::materia_padsoff_pngSize);
        AlphaTheme::getInstance()->padsOnImage = ImageCache::getFromMemory(ThemeMateriaBinaryData::materia_padson_png,
                                                                           ThemeMateriaBinaryData::materia_padson_pngSize);
        AlphaTheme::getInstance()->modeOffImage = ImageCache::getFromMemory(ThemeMateriaBinaryData::materia_modeoff_png,
                                                                            ThemeMateriaBinaryData::materia_modeoff_pngSize);
        AlphaTheme::getInstance()->padsBackgroundImage = ImageCache::getFromMemory(ThemeMateriaBinaryData::materia_padsbg_png,
                                                                                   ThemeMateriaBinaryData::materia_padsbg_pngSize);
        AlphaTheme::getInstance()->settingsOffImage = ImageCache::getFromMemory(ThemeMateriaBinaryData::materia_statusoff_png,
                                                                                ThemeMateriaBinaryData::materia_statusoff_pngSize);
        
        //set custom and default AlphaLive colours here...
        
        AlphaTheme::getInstance()->mainColour_ = 0xff316491; //blue
        AlphaTheme::getInstance()->mainColourLighter_ = 0xff448ecf; //light blue
        AlphaTheme::getInstance()->childBackgroundColour_ = 0xff181818; //nearly black
        AlphaTheme::getInstance()->childBackgroundColourLighter_ = 0xff21212a; //very dark grey
        AlphaTheme::getInstance()->textColour_ = 0xffb6b797; // materia cream
        AlphaTheme::getInstance()->backgroundColour_ = 0xff021221; //very dark blue
        AlphaTheme::getInstance()->foregroundColour_ = 0xff808080; //grey
        AlphaTheme::getInstance()->foregroundColourLighter_ = 0xffb6b797; //materia cream
        AlphaTheme::getInstance()->foregroundColourDarker_ = 0xff46505a; //dark blue/grey
        AlphaTheme::getInstance()->iconColour_ = 0xffb6b797; // materia cream
        AlphaTheme::getInstance()->iconOnColour_ = 0xffb6b797;
        AlphaTheme::getInstance()->tabColour_ = 0xff46505a;
        AlphaTheme::getInstance()->padColourBackground_ = 0xffb6b797; //light grey
        AlphaTheme::getInstance()->padColour_ = 0xff555555; //dark grey
        AlphaTheme::getInstance()->padColour2_ = 0xff808080; //grey
        AlphaTheme::getInstance()->padTextColour_ = 0xffb6b797;
        
        
        // set up the standard set of colours..
        static const uint32 standardColours[] =
        {
            TextButton::buttonColourId,                 AlphaTheme::getInstance()->backgroundColour_,
            TextButton::buttonOnColourId,               AlphaTheme::getInstance()->mainColour_,
            TextButton::textColourOnId,                 AlphaTheme::getInstance()->textColour_,
            TextButton::textColourOffId,                AlphaTheme::getInstance()->textColour_,
            
            DrawableButton::backgroundColourId,         0x00000000,
            DrawableButton::backgroundOnColourId,       AlphaTheme::getInstance()->mainColour_,
            
            
            ComboBox::buttonColourId,                   0xffcccccc,
            ComboBox::outlineColourId,                  AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ComboBox::textColourId,                     AlphaTheme::getInstance()->textColour_,
            ComboBox::backgroundColourId,               AlphaTheme::getInstance()->childBackgroundColour_,
            ComboBox::arrowColourId,                    AlphaTheme::getInstance()->textColour_,
            
            ToggleButton::textColourId,                 AlphaTheme::getInstance()->textColour_,
            
            TextEditor::backgroundColourId,             AlphaTheme::getInstance()->childBackgroundColour_,
            TextEditor::textColourId,                   AlphaTheme::getInstance()->textColour_,
            TextEditor::highlightColourId,              AlphaTheme::getInstance()->mainColour_,
            TextEditor::highlightedTextColourId,        AlphaTheme::getInstance()->textColour_,
            TextEditor::outlineColourId,                AlphaTheme::getInstance()->childBackgroundColourLighter_,
            TextEditor::focusedOutlineColourId,         AlphaTheme::getInstance()->foregroundColourDarker_,
            TextEditor::shadowColourId,                 0x38000000,
            
            CaretComponent::caretColourId,              AlphaTheme::getInstance()->textColour_,
            
            Label::backgroundColourId,                  0x00000000,
            Label::textColourId,                        AlphaTheme::getInstance()->textColour_,
            Label::outlineColourId,                     0x00000000,
            
            ScrollBar::backgroundColourId,              0x00000000,
            ScrollBar::thumbColourId,                   AlphaTheme::getInstance()->textColour_,
            
            TreeView::linesColourId,                    0x00000000,
            TreeView::backgroundColourId,               AlphaTheme::getInstance()->backgroundColour_,
            TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
            
            PopupMenu::backgroundColourId,              AlphaTheme::getInstance()->childBackgroundColour_,
            PopupMenu::textColourId,                    AlphaTheme::getInstance()->textColour_,
            PopupMenu::headerTextColourId,              AlphaTheme::getInstance()->foregroundColour_,
            PopupMenu::highlightedTextColourId,         AlphaTheme::getInstance()->textColour_,
            PopupMenu::highlightedBackgroundColourId,   AlphaTheme::getInstance()->mainColour_,
            
            ListBox::backgroundColourId,                AlphaTheme::getInstance()->backgroundColour_,
            ListBox::outlineColourId,                   AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ListBox::textColourId,                      AlphaTheme::getInstance()->textColour_,
            
            Slider::backgroundColourId,                 0x00000000,
            Slider::thumbColourId,                      AlphaTheme::getInstance()->backgroundColour_,
            Slider::trackColourId,                      AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderFillColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderOutlineColourId,        0xff2c2c2c, //0xff3c3c3c
            Slider::textBoxTextColourId,                AlphaTheme::getInstance()->textColour_,
            Slider::textBoxBackgroundColourId,          0x00000000,
            Slider::textBoxHighlightColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::textBoxOutlineColourId,             AlphaTheme::getInstance()->childBackgroundColourLighter_,
            
            ResizableWindow::backgroundColourId,        0xff777777,
            //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)
            
            AlertWindow::backgroundColourId,            AlphaTheme::getInstance()->backgroundColour_,
            AlertWindow::textColourId,                  AlphaTheme::getInstance()->textColour_,
            AlertWindow::outlineColourId,               0xff666666,
            
            ProgressBar::backgroundColourId,            AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ProgressBar::foregroundColourId,            AlphaTheme::getInstance()->foregroundColour_,
            
            TooltipWindow::backgroundColourId,          0x00000000,
            TooltipWindow::textColourId,                AlphaTheme::getInstance()->textColour_,
            TooltipWindow::outlineColourId,             0x4c000000,
            
            TabbedComponent::backgroundColourId,        0x00000000,
            TabbedComponent::outlineColourId,           AlphaTheme::getInstance()->backgroundColour_,
            TabbedButtonBar::tabOutlineColourId,        0x80000000,
            TabbedButtonBar::frontOutlineColourId,      0x90000000,
            TabbedButtonBar::tabTextColourId,           AlphaTheme::getInstance()->textColour_,
            TabbedButtonBar::frontTextColourId,         AlphaTheme::getInstance()->textColour_,
            
            Toolbar::backgroundColourId,                0xfff6f8f9,
            Toolbar::separatorColourId,                 0x4c000000,
            Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
            Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
            Toolbar::labelTextColourId,                 0xf0000000,
            Toolbar::editingModeOutlineColourId,        0xffff0000,
            
            HyperlinkButton::textColourId,              0xcc1111ee,
            
            GroupComponent::outlineColourId,            AlphaTheme::getInstance()->textColour_,
            GroupComponent::textColourId,               AlphaTheme::getInstance()->textColour_,
            
            DirectoryContentsDisplayComponent::highlightColourId,   AlphaTheme::getInstance()->mainColour_, // <-- used for fileList and fileTree item's
            DirectoryContentsDisplayComponent::textColourId,        AlphaTheme::getInstance()->textColour_,
            
            FileSearchPathListComponent::backgroundColourId,        0x00000000,
            
            FileChooserDialogBox::titleTextColourId,                AlphaTheme::getInstance()->textColour_,
            DrawableButton::textColourId,                           AlphaTheme::getInstance()->textColour_,
            
            ColourSelector::backgroundColourId,         AlphaTheme::getInstance()->childBackgroundColour_,
            ColourSelector::labelTextColourId,          AlphaTheme::getInstance()->textColour_,
        };
        
        for (int i = 0; i < numElementsInArray (standardColours); i += 2)
            setColour (standardColours [i], Colour ((uint32) standardColours [i + 1]));
    }
    
    
    //======================================================================
    //======================================================================
    
    
    else if (theme == 2) //minimal theme
    {
        // set background images here...
        
        AlphaTheme::getInstance()->mainImage = ImageCache::getFromMemory(ThemeMinimalBinaryData::minimal_interfacemain_png,
                                                                         ThemeMinimalBinaryData::minimal_interfacemain_pngSize);
        AlphaTheme::getInstance()->padsOffImage = ImageCache::getFromMemory(ThemeMinimalBinaryData::minimal_padsoff_png,
                                                                            ThemeMinimalBinaryData::minimal_padsoff_pngSize);
        AlphaTheme::getInstance()->padsOnImage = ImageCache::getFromMemory(ThemeMinimalBinaryData::minimal_padson_png,
                                                                           ThemeMinimalBinaryData::minimal_padson_pngSize);
        AlphaTheme::getInstance()->modeOffImage = ImageCache::getFromMemory(ThemeMinimalBinaryData::minimal_modeoff_png,
                                                                            ThemeMinimalBinaryData::minimal_modeoff_pngSize);
        AlphaTheme::getInstance()->padsBackgroundImage = ImageCache::getFromMemory(ThemeMinimalBinaryData::minimal_padsbg_png,
                                                                                   ThemeMinimalBinaryData::minimal_padsbg_pngSize);
        AlphaTheme::getInstance()->settingsOffImage = ImageCache::getFromMemory(ThemeMinimalBinaryData::minimal_statusoff_png,
                                                                                ThemeMinimalBinaryData::minimal_statusoff_pngSize);
        
        //set custom and default AlphaLive colours here...
        
        AlphaTheme::getInstance()->mainColour_ = 0xff195fad; //blue
        AlphaTheme::getInstance()->mainColourLighter_ = 0xff6ec8e8; //light blue
        AlphaTheme::getInstance()->childBackgroundColour_ = 0xffebebeb; //light grey
        AlphaTheme::getInstance()->childBackgroundColourLighter_ = 0xfff0f0f0; //dark white
        AlphaTheme::getInstance()->textColour_ = 0xff777777; //dark grey
        AlphaTheme::getInstance()->backgroundColour_ = 0xffffffff; //white
        AlphaTheme::getInstance()->foregroundColour_ = 0xffaeaeae; //grey
        AlphaTheme::getInstance()->foregroundColourLighter_ = 0xffdddddd; //light grey
        AlphaTheme::getInstance()->foregroundColourDarker_ = 0xff666666; //dark grey
        AlphaTheme::getInstance()->iconColour_ = 0xff999999; //dark grey (same as text colour)
        AlphaTheme::getInstance()->iconOnColour_ = 0xffffffff;
        AlphaTheme::getInstance()->padColourBackground_ = 0xffd3d3d3; //light grey
        AlphaTheme::getInstance()->padColour_ = 0xfff0f0f0;
        AlphaTheme::getInstance()->padColour2_ = 0xffc2c2c2; //grey
        AlphaTheme::getInstance()->padTextColour_ = 0xffffffff;
        AlphaTheme::getInstance()->tabColour_ = 0xffffffff;
        
        
        // set up the standard set of colours..
        static const uint32 standardColours[] =
        {
            TextButton::buttonColourId,                 AlphaTheme::getInstance()->backgroundColour_,
            TextButton::buttonOnColourId,               AlphaTheme::getInstance()->mainColour_,
            TextButton::textColourOnId,                 AlphaTheme::getInstance()->textColour_,
            TextButton::textColourOffId,                AlphaTheme::getInstance()->textColour_,
            
            DrawableButton::backgroundColourId,         0x00000000,
            DrawableButton::backgroundOnColourId,       AlphaTheme::getInstance()->mainColour_,
            
            
            ComboBox::buttonColourId,                   0xffcccccc,
            ComboBox::outlineColourId,                  AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ComboBox::textColourId,                     AlphaTheme::getInstance()->textColour_,
            ComboBox::backgroundColourId,               AlphaTheme::getInstance()->childBackgroundColour_,
            ComboBox::arrowColourId,                    AlphaTheme::getInstance()->textColour_,
            
            ToggleButton::textColourId,                 AlphaTheme::getInstance()->textColour_,
            
            TextEditor::backgroundColourId,             AlphaTheme::getInstance()->childBackgroundColour_,
            TextEditor::textColourId,                   AlphaTheme::getInstance()->textColour_,
            TextEditor::highlightColourId,              AlphaTheme::getInstance()->mainColour_,
            TextEditor::highlightedTextColourId,        AlphaTheme::getInstance()->textColour_,
            TextEditor::outlineColourId,                AlphaTheme::getInstance()->childBackgroundColourLighter_,
            TextEditor::focusedOutlineColourId,         AlphaTheme::getInstance()->foregroundColourDarker_,
            TextEditor::shadowColourId,                 0x38000000,
            
            CaretComponent::caretColourId,              AlphaTheme::getInstance()->textColour_,
            
            Label::backgroundColourId,                  0x00000000,
            Label::textColourId,                        AlphaTheme::getInstance()->textColour_,
            Label::outlineColourId,                     0x00000000,
            
            ScrollBar::backgroundColourId,              0x00000000,
            ScrollBar::thumbColourId,                   AlphaTheme::getInstance()->textColour_,
            
            TreeView::linesColourId,                    0x00000000,
            TreeView::backgroundColourId,               AlphaTheme::getInstance()->backgroundColour_,
            TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
            
            PopupMenu::backgroundColourId,              AlphaTheme::getInstance()->childBackgroundColour_,
            PopupMenu::textColourId,                    AlphaTheme::getInstance()->textColour_,
            PopupMenu::headerTextColourId,              AlphaTheme::getInstance()->foregroundColour_,
            PopupMenu::highlightedTextColourId,         AlphaTheme::getInstance()->textColour_,
            PopupMenu::highlightedBackgroundColourId,   AlphaTheme::getInstance()->mainColour_,
            
            ListBox::backgroundColourId,                AlphaTheme::getInstance()->backgroundColour_,
            ListBox::outlineColourId,                   AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ListBox::textColourId,                      AlphaTheme::getInstance()->textColour_,
            
            Slider::backgroundColourId,                 0x00000000,
            Slider::thumbColourId,                      AlphaTheme::getInstance()->backgroundColour_,
            Slider::trackColourId,                      AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderFillColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderOutlineColourId,        AlphaTheme::getInstance()->childBackgroundColour_, //0xff3c3c3c
            Slider::textBoxTextColourId,                AlphaTheme::getInstance()->textColour_,
            Slider::textBoxBackgroundColourId,          0x00000000,
            Slider::textBoxHighlightColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::textBoxOutlineColourId,             AlphaTheme::getInstance()->childBackgroundColourLighter_,
            
            ResizableWindow::backgroundColourId,        0xff777777,
            //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)
            
            AlertWindow::backgroundColourId,            AlphaTheme::getInstance()->backgroundColour_,
            AlertWindow::textColourId,                  AlphaTheme::getInstance()->textColour_,
            AlertWindow::outlineColourId,               0xff666666,
            
            ProgressBar::backgroundColourId,            AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ProgressBar::foregroundColourId,            AlphaTheme::getInstance()->foregroundColour_,
            
            TooltipWindow::backgroundColourId,          0x00000000,
            TooltipWindow::textColourId,                AlphaTheme::getInstance()->textColour_,
            TooltipWindow::outlineColourId,             0x4c000000,
            
            TabbedComponent::backgroundColourId,        0x00000000,
            TabbedComponent::outlineColourId,           AlphaTheme::getInstance()->backgroundColour_,
            TabbedButtonBar::tabOutlineColourId,        AlphaTheme::getInstance()->foregroundColour_,
            TabbedButtonBar::frontOutlineColourId,      AlphaTheme::getInstance()->foregroundColour_,
            TabbedButtonBar::tabTextColourId,           AlphaTheme::getInstance()->textColour_,
            TabbedButtonBar::frontTextColourId,         AlphaTheme::getInstance()->textColour_,
            
            Toolbar::backgroundColourId,                0xfff6f8f9,
            Toolbar::separatorColourId,                 0x4c000000,
            Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
            Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
            Toolbar::labelTextColourId,                 0xf0000000,
            Toolbar::editingModeOutlineColourId,        0xffff0000,
            
            HyperlinkButton::textColourId,              0xcc1111ee,
            
            GroupComponent::outlineColourId,            AlphaTheme::getInstance()->textColour_,
            GroupComponent::textColourId,               AlphaTheme::getInstance()->textColour_,
            
            DirectoryContentsDisplayComponent::highlightColourId,   AlphaTheme::getInstance()->mainColour_, // <-- used for fileList and fileTree item's
            DirectoryContentsDisplayComponent::textColourId,        AlphaTheme::getInstance()->textColour_,
            
            FileSearchPathListComponent::backgroundColourId,        0x00000000,
            
            FileChooserDialogBox::titleTextColourId,                AlphaTheme::getInstance()->textColour_,
            DrawableButton::textColourId,                           AlphaTheme::getInstance()->textColour_,
            
            ColourSelector::backgroundColourId,         AlphaTheme::getInstance()->childBackgroundColour_,
            ColourSelector::labelTextColourId,          AlphaTheme::getInstance()->textColour_,
        };
        
        for (int i = 0; i < numElementsInArray (standardColours); i += 2)
            setColour (standardColours [i], Colour ((uint32) standardColours [i + 1]));
    }

    
    
    //======================================================================
    //======================================================================
    
    else if (theme == 3) //charcoal theme
    {
        // set background images here...
        
        AlphaTheme::getInstance()->mainImage = ImageCache::getFromMemory(ThemeCharcoalBinaryData::charcoal_interfacemain_png,
                                                                         ThemeCharcoalBinaryData::charcoal_interfacemain_pngSize);
        AlphaTheme::getInstance()->padsOffImage = ImageCache::getFromMemory(ThemeCharcoalBinaryData::charcoal_padsoff_png,
                                                                            ThemeCharcoalBinaryData::charcoal_padsoff_pngSize);
        AlphaTheme::getInstance()->padsOnImage = ImageCache::getFromMemory(ThemeCharcoalBinaryData::charcoal_padson_png,
                                                                           ThemeCharcoalBinaryData::charcoal_padson_pngSize);
        AlphaTheme::getInstance()->modeOffImage = ImageCache::getFromMemory(ThemeCharcoalBinaryData::charcoal_modeoff_png,
                                                                            ThemeCharcoalBinaryData::charcoal_modeoff_pngSize);
        AlphaTheme::getInstance()->padsBackgroundImage = ImageCache::getFromMemory(ThemeCharcoalBinaryData::charcoal_padsbg_png,
                                                                                   ThemeCharcoalBinaryData::charcoal_padsbg_pngSize);
        AlphaTheme::getInstance()->settingsOffImage = ImageCache::getFromMemory(ThemeCharcoalBinaryData::charcoal_statusoff_png,
                                                                                ThemeCharcoalBinaryData::charcoal_statusoff_pngSize);
        
        //set custom and default AlphaLive colours here...
        
        AlphaTheme::getInstance()->mainColour_ = 0xff244458; //turqoise
        AlphaTheme::getInstance()->mainColourLighter_ = 0xff35a296; //green
        AlphaTheme::getInstance()->childBackgroundColour_ = 0xff262c38; //dark blue
        AlphaTheme::getInstance()->childBackgroundColourLighter_ = 0xff2e343f; //light grey
        AlphaTheme::getInstance()->textColour_ = 0xff999999; //dark grey
        AlphaTheme::getInstance()->backgroundColour_ = 0xff1c212a; //bg
        AlphaTheme::getInstance()->foregroundColour_ = 0xff6B6B6B; //grey
        AlphaTheme::getInstance()->foregroundColourLighter_ = 0xffcccccc; //light grey
        AlphaTheme::getInstance()->foregroundColourDarker_ = 0xff606060; //dark grey
        AlphaTheme::getInstance()->iconColour_ = 0xff999999; //dark grey (same as text colour)
        AlphaTheme::getInstance()->iconOnColour_ = 0xffffffff;
        AlphaTheme::getInstance()->padColourBackground_ = 0xff999999;
        AlphaTheme::getInstance()->padColour_ = 0xff2e343f;
        AlphaTheme::getInstance()->padColour2_ = 0xff585D65; //grey
        AlphaTheme::getInstance()->padTextColour_ = 0xff999999;
        AlphaTheme::getInstance()->tabColour_ = 0xff2e343f;
        
        
        // set up the standard set of colours..
        static const uint32 standardColours[] =
        {
            TextButton::buttonColourId,                 AlphaTheme::getInstance()->childBackgroundColourLighter_,
            TextButton::buttonOnColourId,               AlphaTheme::getInstance()->mainColour_,
            TextButton::textColourOnId,                 AlphaTheme::getInstance()->iconOnColour_,
            TextButton::textColourOffId,                AlphaTheme::getInstance()->textColour_,
            
            DrawableButton::backgroundColourId,         AlphaTheme::getInstance()->childBackgroundColourLighter_,
            DrawableButton::backgroundOnColourId,       AlphaTheme::getInstance()->mainColour_,
            
            
            ComboBox::buttonColourId,                   0xffcccccc,
            ComboBox::outlineColourId,                  AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ComboBox::textColourId,                     AlphaTheme::getInstance()->textColour_,
            ComboBox::backgroundColourId,               AlphaTheme::getInstance()->childBackgroundColour_,
            ComboBox::arrowColourId,                    AlphaTheme::getInstance()->textColour_,
            
            ToggleButton::textColourId,                 AlphaTheme::getInstance()->textColour_,
            
            TextEditor::backgroundColourId,             AlphaTheme::getInstance()->childBackgroundColour_,
            TextEditor::textColourId,                   AlphaTheme::getInstance()->textColour_,
            TextEditor::highlightColourId,              AlphaTheme::getInstance()->mainColour_,
            TextEditor::highlightedTextColourId,        AlphaTheme::getInstance()->textColour_,
            TextEditor::outlineColourId,                AlphaTheme::getInstance()->childBackgroundColourLighter_,
            TextEditor::focusedOutlineColourId,         AlphaTheme::getInstance()->foregroundColourDarker_,
            TextEditor::shadowColourId,                 0x38000000,
            
            CaretComponent::caretColourId,              AlphaTheme::getInstance()->textColour_,
            
            Label::backgroundColourId,                  0x00000000,
            Label::textColourId,                        AlphaTheme::getInstance()->textColour_,
            Label::outlineColourId,                     0x00000000,
            
            ScrollBar::backgroundColourId,              0x00000000,
            ScrollBar::thumbColourId,                   AlphaTheme::getInstance()->textColour_,
            
            TreeView::linesColourId,                    0x00000000,
            TreeView::backgroundColourId,               AlphaTheme::getInstance()->backgroundColour_,
            TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
            
            PopupMenu::backgroundColourId,              AlphaTheme::getInstance()->childBackgroundColour_,
            PopupMenu::textColourId,                    AlphaTheme::getInstance()->textColour_,
            PopupMenu::headerTextColourId,              AlphaTheme::getInstance()->foregroundColour_,
            PopupMenu::highlightedTextColourId,         AlphaTheme::getInstance()->textColour_,
            PopupMenu::highlightedBackgroundColourId,   AlphaTheme::getInstance()->mainColour_,
            
            ListBox::backgroundColourId,                AlphaTheme::getInstance()->backgroundColour_,
            ListBox::outlineColourId,                   AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ListBox::textColourId,                      AlphaTheme::getInstance()->textColour_,
            
            Slider::backgroundColourId,                 0x00000000,
            Slider::thumbColourId,                      AlphaTheme::getInstance()->backgroundColour_,
            Slider::trackColourId,                      AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderFillColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderOutlineColourId,        AlphaTheme::getInstance()->foregroundColour_, //0xff3c3c3c
            Slider::textBoxTextColourId,                AlphaTheme::getInstance()->textColour_,
            Slider::textBoxBackgroundColourId,          0x00000000,
            Slider::textBoxHighlightColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::textBoxOutlineColourId,             AlphaTheme::getInstance()->childBackgroundColourLighter_,
            
            ResizableWindow::backgroundColourId,        0xff777777,
            //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)
            
            AlertWindow::backgroundColourId,            AlphaTheme::getInstance()->backgroundColour_,
            AlertWindow::textColourId,                  AlphaTheme::getInstance()->textColour_,
            AlertWindow::outlineColourId,               0xff666666,
            
            ProgressBar::backgroundColourId,            AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ProgressBar::foregroundColourId,            AlphaTheme::getInstance()->foregroundColour_,
            
            TooltipWindow::backgroundColourId,          0x00000000,
            TooltipWindow::textColourId,                AlphaTheme::getInstance()->textColour_,
            TooltipWindow::outlineColourId,             0x4c000000,
            
            TabbedComponent::backgroundColourId,        0x00000000,
            TabbedComponent::outlineColourId,           AlphaTheme::getInstance()->backgroundColour_,
            TabbedButtonBar::tabOutlineColourId,        AlphaTheme::getInstance()->foregroundColour_,
            TabbedButtonBar::frontOutlineColourId,      AlphaTheme::getInstance()->foregroundColour_,
            TabbedButtonBar::tabTextColourId,           AlphaTheme::getInstance()->textColour_,
            TabbedButtonBar::frontTextColourId,         AlphaTheme::getInstance()->textColour_,
            
            Toolbar::backgroundColourId,                0xfff6f8f9,
            Toolbar::separatorColourId,                 0x4c000000,
            Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
            Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
            Toolbar::labelTextColourId,                 0xf0000000,
            Toolbar::editingModeOutlineColourId,        0xffff0000,
            
            HyperlinkButton::textColourId,              0xcc1111ee,
            
            GroupComponent::outlineColourId,            AlphaTheme::getInstance()->textColour_,
            GroupComponent::textColourId,               AlphaTheme::getInstance()->textColour_,
            
            DirectoryContentsDisplayComponent::highlightColourId,   AlphaTheme::getInstance()->mainColour_, // <-- used for fileList and fileTree item's
            DirectoryContentsDisplayComponent::textColourId,        AlphaTheme::getInstance()->textColour_,
            
            FileSearchPathListComponent::backgroundColourId,        0x00000000,
            
            FileChooserDialogBox::titleTextColourId,                AlphaTheme::getInstance()->textColour_,
            DrawableButton::textColourId,                           AlphaTheme::getInstance()->textColour_,
            
            ColourSelector::backgroundColourId,         AlphaTheme::getInstance()->childBackgroundColour_,
            ColourSelector::labelTextColourId,          AlphaTheme::getInstance()->textColour_,
        };
        
        for (int i = 0; i < numElementsInArray (standardColours); i += 2)
            setColour (standardColours [i], Colour ((uint32) standardColours [i + 1]));
    }
    
    //======================================================================
    //======================================================================
    
    else if (theme == 4) //aurora theme
    {
        // set background images here...
        
        AlphaTheme::getInstance()->mainImage = ImageCache::getFromMemory(ThemeAuroraBinaryData::aurora_interfacemain_png,
                                                                         ThemeAuroraBinaryData::aurora_interfacemain_pngSize);
        AlphaTheme::getInstance()->padsOffImage = ImageCache::getFromMemory(ThemeAuroraBinaryData::aurora_padsoff_png,
                                                                            ThemeAuroraBinaryData::aurora_padsoff_pngSize);
        AlphaTheme::getInstance()->padsOnImage = ImageCache::getFromMemory(ThemeAuroraBinaryData::aurora_padson_png,
                                                                           ThemeAuroraBinaryData::aurora_padson_pngSize);
        AlphaTheme::getInstance()->modeOffImage = ImageCache::getFromMemory(ThemeAuroraBinaryData::aurora_modeoff_png,
                                                                            ThemeAuroraBinaryData::aurora_modeoff_pngSize);
        AlphaTheme::getInstance()->padsBackgroundImage = ImageCache::getFromMemory(ThemeAuroraBinaryData::aurora_padsbg_png,
                                                                                   ThemeAuroraBinaryData::aurora_padsbg_pngSize);
        AlphaTheme::getInstance()->settingsOffImage = ImageCache::getFromMemory(ThemeAuroraBinaryData::aurora_statusoff_png,
                                                                                ThemeAuroraBinaryData::aurora_statusoff_pngSize);
        
        //set custom and default AlphaLive colours here...
        
        AlphaTheme::getInstance()->mainColour_ = 0xeea4709c; //purple
        AlphaTheme::getInstance()->mainColourLighter_ = 0xffee95ac; //pink
        AlphaTheme::getInstance()->childBackgroundColour_ = 0xff1c1c1c; //dark blue
        AlphaTheme::getInstance()->childBackgroundColourLighter_ = 0xff1e1e1e; //light grey
        AlphaTheme::getInstance()->textColour_ = 0xff999999; //dark grey
        AlphaTheme::getInstance()->backgroundColour_ = 0xff1a1a1a; //bg
        AlphaTheme::getInstance()->foregroundColour_ = 0xff505050; //grey
        AlphaTheme::getInstance()->foregroundColourLighter_ = 0xff999999; //light grey
        AlphaTheme::getInstance()->foregroundColourDarker_ = 0xff3a3a3a; //dark grey
        AlphaTheme::getInstance()->iconColour_ = 0xff999999; //dark grey (same as text colour)
        AlphaTheme::getInstance()->iconOnColour_ = 0xffffffff;
        AlphaTheme::getInstance()->padColourBackground_ = 0x484848;
        AlphaTheme::getInstance()->padColour_ = 0xff1e1e1e; //dark grey
        AlphaTheme::getInstance()->padColour2_ = 0xff2e2e2e;
        AlphaTheme::getInstance()->padTextColour_ = 0xff999999;
        AlphaTheme::getInstance()->tabColour_ = 0xff262626; //0xff1e1e1e
        
        
        // set up the standard set of colours..
        static const uint32 standardColours[] =
        {
            TextButton::buttonColourId,                 AlphaTheme::getInstance()->childBackgroundColourLighter_,
            TextButton::buttonOnColourId,               AlphaTheme::getInstance()->mainColour_,
            TextButton::textColourOnId,                 AlphaTheme::getInstance()->childBackgroundColour_,
            TextButton::textColourOffId,                AlphaTheme::getInstance()->textColour_,
            
            DrawableButton::backgroundColourId,         AlphaTheme::getInstance()->childBackgroundColourLighter_,
            DrawableButton::backgroundOnColourId,       AlphaTheme::getInstance()->mainColour_,
            
            
            ComboBox::buttonColourId,                   0xffcccccc,
            ComboBox::outlineColourId,                  AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ComboBox::textColourId,                     AlphaTheme::getInstance()->textColour_,
            ComboBox::backgroundColourId,               AlphaTheme::getInstance()->childBackgroundColour_,
            ComboBox::arrowColourId,                    AlphaTheme::getInstance()->textColour_,
            
            ToggleButton::textColourId,                 AlphaTheme::getInstance()->textColour_,
            
            TextEditor::backgroundColourId,             AlphaTheme::getInstance()->childBackgroundColour_,
            TextEditor::textColourId,                   AlphaTheme::getInstance()->textColour_,
            TextEditor::highlightColourId,              AlphaTheme::getInstance()->mainColour_,
            TextEditor::highlightedTextColourId,        AlphaTheme::getInstance()->textColour_,
            TextEditor::outlineColourId,                AlphaTheme::getInstance()->childBackgroundColourLighter_,
            TextEditor::focusedOutlineColourId,         AlphaTheme::getInstance()->foregroundColourDarker_,
            TextEditor::shadowColourId,                 0x38000000,
            
            CaretComponent::caretColourId,              AlphaTheme::getInstance()->textColour_,
            
            Label::backgroundColourId,                  0x00000000,
            Label::textColourId,                        AlphaTheme::getInstance()->textColour_,
            Label::outlineColourId,                     0x00000000,
            
            ScrollBar::backgroundColourId,              0x00000000,
            ScrollBar::thumbColourId,                   AlphaTheme::getInstance()->textColour_,
            
            TreeView::linesColourId,                    0x00000000,
            TreeView::backgroundColourId,               AlphaTheme::getInstance()->backgroundColour_,
            TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
            
            PopupMenu::backgroundColourId,              AlphaTheme::getInstance()->childBackgroundColour_,
            PopupMenu::textColourId,                    AlphaTheme::getInstance()->textColour_,
            PopupMenu::headerTextColourId,              AlphaTheme::getInstance()->foregroundColour_,
            PopupMenu::highlightedTextColourId,         AlphaTheme::getInstance()->textColour_,
            PopupMenu::highlightedBackgroundColourId,   AlphaTheme::getInstance()->mainColour_,
            
            ListBox::backgroundColourId,                AlphaTheme::getInstance()->backgroundColour_,
            ListBox::outlineColourId,                   AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ListBox::textColourId,                      AlphaTheme::getInstance()->textColour_,
            
            Slider::backgroundColourId,                 0x00000000,
            Slider::thumbColourId,                      AlphaTheme::getInstance()->backgroundColour_,
            Slider::trackColourId,                      AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderFillColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderOutlineColourId,        AlphaTheme::getInstance()->foregroundColour_, //0xff3c3c3c
            Slider::textBoxTextColourId,                AlphaTheme::getInstance()->textColour_,
            Slider::textBoxBackgroundColourId,          0x00000000,
            Slider::textBoxHighlightColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::textBoxOutlineColourId,             AlphaTheme::getInstance()->childBackgroundColourLighter_,
            
            ResizableWindow::backgroundColourId,        0xff777777,
            //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)
            
            AlertWindow::backgroundColourId,            AlphaTheme::getInstance()->backgroundColour_,
            AlertWindow::textColourId,                  AlphaTheme::getInstance()->textColour_,
            AlertWindow::outlineColourId,               0xff666666,
            
            ProgressBar::backgroundColourId,            AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ProgressBar::foregroundColourId,            AlphaTheme::getInstance()->foregroundColour_,
            
            TooltipWindow::backgroundColourId,          0x00000000,
            TooltipWindow::textColourId,                AlphaTheme::getInstance()->textColour_,
            TooltipWindow::outlineColourId,             0x4c000000,
            
            TabbedComponent::backgroundColourId,        0x00000000,
            TabbedComponent::outlineColourId,           AlphaTheme::getInstance()->backgroundColour_,
            TabbedButtonBar::tabOutlineColourId,        AlphaTheme::getInstance()->foregroundColour_,
            TabbedButtonBar::frontOutlineColourId,      AlphaTheme::getInstance()->foregroundColour_,
            TabbedButtonBar::tabTextColourId,           AlphaTheme::getInstance()->textColour_,
            TabbedButtonBar::frontTextColourId,         AlphaTheme::getInstance()->textColour_,
            
            Toolbar::backgroundColourId,                0xfff6f8f9,
            Toolbar::separatorColourId,                 0x4c000000,
            Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
            Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
            Toolbar::labelTextColourId,                 0xf0000000,
            Toolbar::editingModeOutlineColourId,        0xffff0000,
            
            HyperlinkButton::textColourId,              0xcc1111ee,
            
            GroupComponent::outlineColourId,            AlphaTheme::getInstance()->textColour_,
            GroupComponent::textColourId,               AlphaTheme::getInstance()->textColour_,
            
            DirectoryContentsDisplayComponent::highlightColourId,   AlphaTheme::getInstance()->mainColour_, // <-- used for fileList and fileTree item's
            DirectoryContentsDisplayComponent::textColourId,        AlphaTheme::getInstance()->textColour_,
            
            FileSearchPathListComponent::backgroundColourId,        0x00000000,
            
            FileChooserDialogBox::titleTextColourId,                AlphaTheme::getInstance()->textColour_,
            DrawableButton::textColourId,                           AlphaTheme::getInstance()->textColour_,
            
            ColourSelector::backgroundColourId,         AlphaTheme::getInstance()->childBackgroundColour_,
            ColourSelector::labelTextColourId,          AlphaTheme::getInstance()->textColour_,
        };
        
        for (int i = 0; i < numElementsInArray (standardColours); i += 2)
            setColour (standardColours [i], Colour ((uint32) standardColours [i + 1]));
    }


    
    //======================================================================
    //======================================================================
    
    else if (theme == 5) //transparency theme (NOT CURRENTLY BEING USED)
    {
        // set background images here...
        
        AlphaTheme::getInstance()->mainImage = ImageCache::getFromMemory(ThemeTransparencyBinaryData::transparency_interfacemain_png,
                                                                         ThemeTransparencyBinaryData::transparency_interfacemain_pngSize);
        AlphaTheme::getInstance()->padsOffImage = ImageCache::getFromMemory(ThemeTransparencyBinaryData::transparency_padsoff_png,
                                                                            ThemeTransparencyBinaryData::transparency_padsoff_pngSize);
        AlphaTheme::getInstance()->padsOnImage = ImageCache::getFromMemory(ThemeTransparencyBinaryData::transparency_padson_png,
                                                                           ThemeTransparencyBinaryData::transparency_padson_pngSize);
        AlphaTheme::getInstance()->modeOffImage = ImageCache::getFromMemory(ThemeTransparencyBinaryData::transparency_modeoff_png,
                                                                            ThemeTransparencyBinaryData::transparency_modeoff_pngSize);
        AlphaTheme::getInstance()->padsBackgroundImage = ImageCache::getFromMemory(ThemeTransparencyBinaryData::transparency_padsbg_png,
                                                                                   ThemeTransparencyBinaryData::transparency_padsbg_pngSize);
        AlphaTheme::getInstance()->settingsOffImage = ImageCache::getFromMemory(ThemeTransparencyBinaryData::transparency_statusoff_png,
                                                                                ThemeTransparencyBinaryData::transparency_statusoff_pngSize);
        
        //set custom and default AlphaLive colours here...
        
        AlphaTheme::getInstance()->mainColour_ = 0xffb67dad; //purple
        AlphaTheme::getInstance()->mainColourLighter_ = 0xffee95ac; //pink
        AlphaTheme::getInstance()->childBackgroundColour_ = 0xff1c1c1c; //dark blue
        AlphaTheme::getInstance()->childBackgroundColourLighter_ = 0xff1e1e1e; //light grey
        AlphaTheme::getInstance()->textColour_ = 0xffffffff; //dark grey
        AlphaTheme::getInstance()->backgroundColour_ = 0x1affffff; //bg
        AlphaTheme::getInstance()->foregroundColour_ = 0xff505050; //grey
        AlphaTheme::getInstance()->foregroundColourLighter_ = 0xff999999; //light grey
        AlphaTheme::getInstance()->foregroundColourDarker_ = 0xff3a3a3a; //dark grey
        AlphaTheme::getInstance()->iconColour_ = 0xffffffff; //dark grey (same as text colour)
        AlphaTheme::getInstance()->iconOnColour_ = 0xffffffff;
        AlphaTheme::getInstance()->padColour_ = 0xff1e1e1e; //dark grey
        AlphaTheme::getInstance()->padTextColour_ = 0xff999999;
        AlphaTheme::getInstance()->tabColour_ = 0xff1e1e1e;
        
        
        // set up the standard set of colours..
        static const uint32 standardColours[] =
        {
            TextButton::buttonColourId,                 AlphaTheme::getInstance()->childBackgroundColourLighter_,
            TextButton::buttonOnColourId,               AlphaTheme::getInstance()->mainColour_,
            TextButton::textColourOnId,                 AlphaTheme::getInstance()->childBackgroundColour_,
            TextButton::textColourOffId,                AlphaTheme::getInstance()->textColour_,
            
            DrawableButton::backgroundColourId,         AlphaTheme::getInstance()->childBackgroundColourLighter_,
            DrawableButton::backgroundOnColourId,       AlphaTheme::getInstance()->mainColour_,
            
            
            ComboBox::buttonColourId,                   0xffcccccc,
            ComboBox::outlineColourId,                  AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ComboBox::textColourId,                     AlphaTheme::getInstance()->textColour_,
            ComboBox::backgroundColourId,               AlphaTheme::getInstance()->childBackgroundColour_,
            ComboBox::arrowColourId,                    AlphaTheme::getInstance()->textColour_,
            
            ToggleButton::textColourId,                 AlphaTheme::getInstance()->textColour_,
            
            TextEditor::backgroundColourId,             AlphaTheme::getInstance()->childBackgroundColour_,
            TextEditor::textColourId,                   AlphaTheme::getInstance()->textColour_,
            TextEditor::highlightColourId,              AlphaTheme::getInstance()->mainColour_,
            TextEditor::highlightedTextColourId,        AlphaTheme::getInstance()->textColour_,
            TextEditor::outlineColourId,                AlphaTheme::getInstance()->childBackgroundColourLighter_,
            TextEditor::focusedOutlineColourId,         AlphaTheme::getInstance()->foregroundColourDarker_,
            TextEditor::shadowColourId,                 0x38000000,
            
            CaretComponent::caretColourId,              AlphaTheme::getInstance()->textColour_,
            
            Label::backgroundColourId,                  0x00000000,
            Label::textColourId,                        AlphaTheme::getInstance()->textColour_,
            Label::outlineColourId,                     0x00000000,
            
            ScrollBar::backgroundColourId,              0x00000000,
            ScrollBar::thumbColourId,                   AlphaTheme::getInstance()->textColour_,
            
            TreeView::linesColourId,                    0x00000000,
            TreeView::backgroundColourId,               AlphaTheme::getInstance()->backgroundColour_,
            TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
            
            PopupMenu::backgroundColourId,              AlphaTheme::getInstance()->childBackgroundColour_,
            PopupMenu::textColourId,                    AlphaTheme::getInstance()->textColour_,
            PopupMenu::headerTextColourId,              AlphaTheme::getInstance()->foregroundColour_,
            PopupMenu::highlightedTextColourId,         AlphaTheme::getInstance()->textColour_,
            PopupMenu::highlightedBackgroundColourId,   AlphaTheme::getInstance()->mainColour_,
            
            ListBox::backgroundColourId,                AlphaTheme::getInstance()->backgroundColour_,
            ListBox::outlineColourId,                   AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ListBox::textColourId,                      AlphaTheme::getInstance()->textColour_,
            
            Slider::backgroundColourId,                 0x00000000,
            Slider::thumbColourId,                      AlphaTheme::getInstance()->backgroundColour_,
            Slider::trackColourId,                      AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderFillColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderOutlineColourId,        AlphaTheme::getInstance()->foregroundColour_, //0xff3c3c3c
            Slider::textBoxTextColourId,                AlphaTheme::getInstance()->textColour_,
            Slider::textBoxBackgroundColourId,          0x00000000,
            Slider::textBoxHighlightColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::textBoxOutlineColourId,             AlphaTheme::getInstance()->childBackgroundColourLighter_,
            
            ResizableWindow::backgroundColourId,        0xff777777,
            //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)
            
            AlertWindow::backgroundColourId,            AlphaTheme::getInstance()->backgroundColour_,
            AlertWindow::textColourId,                  AlphaTheme::getInstance()->textColour_,
            AlertWindow::outlineColourId,               0xff666666,
            
            ProgressBar::backgroundColourId,            AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ProgressBar::foregroundColourId,            AlphaTheme::getInstance()->foregroundColour_,
            
            TooltipWindow::backgroundColourId,          0x00000000,
            TooltipWindow::textColourId,                AlphaTheme::getInstance()->textColour_,
            TooltipWindow::outlineColourId,             0x4c000000,
            
            TabbedComponent::backgroundColourId,        0x00000000,
            TabbedComponent::outlineColourId,           AlphaTheme::getInstance()->backgroundColour_,
            TabbedButtonBar::tabOutlineColourId,        0x80000000,
            TabbedButtonBar::frontOutlineColourId,      0x90000000,
            TabbedButtonBar::tabTextColourId,           AlphaTheme::getInstance()->textColour_,
            TabbedButtonBar::frontTextColourId,         AlphaTheme::getInstance()->textColour_,
            
            Toolbar::backgroundColourId,                0xfff6f8f9,
            Toolbar::separatorColourId,                 0x4c000000,
            Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
            Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
            Toolbar::labelTextColourId,                 0xf0000000,
            Toolbar::editingModeOutlineColourId,        0xffff0000,
            
            HyperlinkButton::textColourId,              0xcc1111ee,
            
            GroupComponent::outlineColourId,            AlphaTheme::getInstance()->textColour_,
            GroupComponent::textColourId,               AlphaTheme::getInstance()->textColour_,
            
            DirectoryContentsDisplayComponent::highlightColourId,   AlphaTheme::getInstance()->mainColour_, // <-- used for fileList and fileTree item's
            DirectoryContentsDisplayComponent::textColourId,        AlphaTheme::getInstance()->textColour_,
            
            FileSearchPathListComponent::backgroundColourId,        0x00000000,
            
            FileChooserDialogBox::titleTextColourId,                AlphaTheme::getInstance()->textColour_,
            DrawableButton::textColourId,                           AlphaTheme::getInstance()->textColour_,
            
            ColourSelector::backgroundColourId,         AlphaTheme::getInstance()->childBackgroundColour_,
            ColourSelector::labelTextColourId,          AlphaTheme::getInstance()->textColour_,
        };
        
        for (int i = 0; i < numElementsInArray (standardColours); i += 2)
            setColour (standardColours [i], Colour ((uint32) standardColours [i + 1]));
    }

    
    
    else //Theme either equals 1 or has an unspecified value, so set it to CLASSIC
    {
        // set background images here...
        
        AlphaTheme::getInstance()->mainImage = ImageCache::getFromMemory(ThemeClassicBinaryData::classic_interfacemain_png,
                                                                           ThemeClassicBinaryData::classic_interfacemain_pngSize);
        AlphaTheme::getInstance()->padsOffImage = ImageCache::getFromMemory(ThemeClassicBinaryData::classic_padsoff_png,
                                                                              ThemeClassicBinaryData::classic_padsoff_pngSize);
        AlphaTheme::getInstance()->padsOnImage = ImageCache::getFromMemory(ThemeClassicBinaryData::classic_padson_png,
                                                                             ThemeClassicBinaryData::classic_padson_pngSize);
        AlphaTheme::getInstance()->modeOffImage = ImageCache::getFromMemory(ThemeClassicBinaryData::classic_modeoff_png,
                                                                              ThemeClassicBinaryData::classic_modeoff_pngSize);
        AlphaTheme::getInstance()->padsBackgroundImage = ImageCache::getFromMemory(ThemeClassicBinaryData::classic_padsbg_png,
                                                                                     ThemeClassicBinaryData::classic_padsbg_pngSize);
        AlphaTheme::getInstance()->settingsOffImage = ImageCache::getFromMemory(ThemeClassicBinaryData::classic_statusoff_png,
                                                                                  ThemeClassicBinaryData::classic_statusoff_pngSize);
        
        //set custom and default AlphaLive colours here...
        
        AlphaTheme::getInstance()->mainColour_ = 0xff1a54ab; //blue
        AlphaTheme::getInstance()->mainColourLighter_ = 0xff3c76c5; //light blue
        AlphaTheme::getInstance()->childBackgroundColour_ = 0xff181818; //nearly black
        AlphaTheme::getInstance()->childBackgroundColourLighter_ = 0xff202020; //very dark grey
        AlphaTheme::getInstance()->textColour_ = 0xffffffff; //white
        AlphaTheme::getInstance()->backgroundColour_ = 0xff000000; //black
        AlphaTheme::getInstance()->foregroundColour_ = 0xff808080; //grey
        AlphaTheme::getInstance()->foregroundColourLighter_ = 0xffd3d3d3; //light grey
        AlphaTheme::getInstance()->foregroundColourDarker_ = 0xff555555; //dark grey
        AlphaTheme::getInstance()->iconColour_ = 0xffffffff; //white (same as text colour)
        AlphaTheme::getInstance()->iconOnColour_ = 0xffffffff;
        AlphaTheme::getInstance()->padColourBackground_ = 0xffd3d3d3; //light grey
        AlphaTheme::getInstance()->padColour_ = 0xff555555; //dark grey
        AlphaTheme::getInstance()->padColour2_ = 0xff808080; //grey
        AlphaTheme::getInstance()->padTextColour_ = 0xffffffff;
        AlphaTheme::getInstance()->tabColour_ = 0xff555555; //dark grey
        
        
        // set up the standard set of colours..
        static const uint32 standardColours[] =
        {
            TextButton::buttonColourId,                 AlphaTheme::getInstance()->backgroundColour_,
            TextButton::buttonOnColourId,               AlphaTheme::getInstance()->mainColour_,
            TextButton::textColourOnId,                 AlphaTheme::getInstance()->textColour_,
            TextButton::textColourOffId,                AlphaTheme::getInstance()->textColour_,
            
            DrawableButton::backgroundColourId,         0x00000000,
            DrawableButton::backgroundOnColourId,       AlphaTheme::getInstance()->mainColour_,
            
            
            ComboBox::buttonColourId,                   0xffcccccc,
            ComboBox::outlineColourId,                  AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ComboBox::textColourId,                     AlphaTheme::getInstance()->textColour_,
            ComboBox::backgroundColourId,               AlphaTheme::getInstance()->childBackgroundColour_,
            ComboBox::arrowColourId,                    AlphaTheme::getInstance()->textColour_,
            
            ToggleButton::textColourId,                 AlphaTheme::getInstance()->textColour_,
            
            TextEditor::backgroundColourId,             AlphaTheme::getInstance()->childBackgroundColour_,
            TextEditor::textColourId,                   AlphaTheme::getInstance()->textColour_,
            TextEditor::highlightColourId,              AlphaTheme::getInstance()->mainColour_,
            TextEditor::highlightedTextColourId,        AlphaTheme::getInstance()->textColour_,
            TextEditor::outlineColourId,                AlphaTheme::getInstance()->childBackgroundColourLighter_,
            TextEditor::focusedOutlineColourId,         AlphaTheme::getInstance()->foregroundColourDarker_,
            TextEditor::shadowColourId,                 0x38000000,
            
            CaretComponent::caretColourId,              AlphaTheme::getInstance()->textColour_,
            
            Label::backgroundColourId,                  0x00000000,
            Label::textColourId,                        AlphaTheme::getInstance()->textColour_,
            Label::outlineColourId,                     0x00000000,
            
            ScrollBar::backgroundColourId,              0x00000000,
            ScrollBar::thumbColourId,                   AlphaTheme::getInstance()->textColour_,
            
            TreeView::linesColourId,                    0x00000000,
            TreeView::backgroundColourId,               AlphaTheme::getInstance()->backgroundColour_,
            TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
            
            PopupMenu::backgroundColourId,              AlphaTheme::getInstance()->childBackgroundColour_,
            PopupMenu::textColourId,                    AlphaTheme::getInstance()->textColour_,
            PopupMenu::headerTextColourId,              AlphaTheme::getInstance()->foregroundColour_,
            PopupMenu::highlightedTextColourId,         AlphaTheme::getInstance()->textColour_,
            PopupMenu::highlightedBackgroundColourId,   AlphaTheme::getInstance()->mainColour_,
            
            ListBox::backgroundColourId,                AlphaTheme::getInstance()->backgroundColour_,
            ListBox::outlineColourId,                   AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ListBox::textColourId,                      AlphaTheme::getInstance()->textColour_,
            
            Slider::backgroundColourId,                 0x00000000,
            Slider::thumbColourId,                      AlphaTheme::getInstance()->backgroundColour_,
            Slider::trackColourId,                      AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderFillColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::rotarySliderOutlineColourId,        0xff2c2c2c, //0xff3c3c3c
            Slider::textBoxTextColourId,                AlphaTheme::getInstance()->textColour_,
            Slider::textBoxBackgroundColourId,          0x00000000,
            Slider::textBoxHighlightColourId,           AlphaTheme::getInstance()->mainColour_,
            Slider::textBoxOutlineColourId,             AlphaTheme::getInstance()->childBackgroundColourLighter_,
            
            ResizableWindow::backgroundColourId,        0xff777777,
            //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)
            
            AlertWindow::backgroundColourId,            AlphaTheme::getInstance()->backgroundColour_,
            AlertWindow::textColourId,                  AlphaTheme::getInstance()->textColour_,
            AlertWindow::outlineColourId,               0xff666666,
            
            ProgressBar::backgroundColourId,            AlphaTheme::getInstance()->childBackgroundColourLighter_,
            ProgressBar::foregroundColourId,            AlphaTheme::getInstance()->foregroundColour_,
            
            TooltipWindow::backgroundColourId,          0x00000000,
            TooltipWindow::textColourId,                AlphaTheme::getInstance()->textColour_,
            TooltipWindow::outlineColourId,             0x4c000000,
            
            TabbedComponent::backgroundColourId,        0x00000000,
            TabbedComponent::outlineColourId,           AlphaTheme::getInstance()->backgroundColour_,
            TabbedButtonBar::tabOutlineColourId,        0x80000000,
            TabbedButtonBar::frontOutlineColourId,      0x90000000,
            TabbedButtonBar::tabTextColourId,           AlphaTheme::getInstance()->textColour_,
            TabbedButtonBar::frontTextColourId,         AlphaTheme::getInstance()->textColour_,
            
            Toolbar::backgroundColourId,                0xfff6f8f9,
            Toolbar::separatorColourId,                 0x4c000000,
            Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
            Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
            Toolbar::labelTextColourId,                 0xf0000000,
            Toolbar::editingModeOutlineColourId,        0xffff0000,
            
            HyperlinkButton::textColourId,              0xcc1111ee,
            
            GroupComponent::outlineColourId,            AlphaTheme::getInstance()->textColour_,
            GroupComponent::textColourId,               AlphaTheme::getInstance()->textColour_,
            
            DirectoryContentsDisplayComponent::highlightColourId,   AlphaTheme::getInstance()->mainColour_, // <-- used for fileList and fileTree item's
            DirectoryContentsDisplayComponent::textColourId,        AlphaTheme::getInstance()->textColour_,
            
            FileSearchPathListComponent::backgroundColourId,        0x00000000,
            
            FileChooserDialogBox::titleTextColourId,                AlphaTheme::getInstance()->textColour_,
            DrawableButton::textColourId,                           AlphaTheme::getInstance()->textColour_,
            
            ColourSelector::backgroundColourId,         AlphaTheme::getInstance()->childBackgroundColour_,
            ColourSelector::labelTextColourId,          AlphaTheme::getInstance()->textColour_,
        };
        
        for (int i = 0; i < numElementsInArray (standardColours); i += 2)
            setColour (standardColours [i], Colour ((uint32) standardColours [i + 1]));
    }
    
    //======================================================================
    //======================================================================
   
    AlphaTheme::getInstance()->mainColour = Colour(AlphaTheme::getInstance()->mainColour_);
    AlphaTheme::getInstance()->mainColourLighter = Colour(AlphaTheme::getInstance()->mainColourLighter_);
    AlphaTheme::getInstance()->childBackgroundColour = Colour(AlphaTheme::getInstance()->childBackgroundColour_);
    AlphaTheme::getInstance()->childBackgroundColourLighter = Colour(AlphaTheme::getInstance()->childBackgroundColourLighter_);
    AlphaTheme::getInstance()->textColour = Colour(AlphaTheme::getInstance()->textColour_);
    AlphaTheme::getInstance()->backgroundColour = Colour(AlphaTheme::getInstance()->backgroundColour_);
    AlphaTheme::getInstance()->foregroundColour = Colour(AlphaTheme::getInstance()->foregroundColour_);
    AlphaTheme::getInstance()->foregroundColourLighter = Colour(AlphaTheme::getInstance()->foregroundColourLighter_);
    AlphaTheme::getInstance()->foregroundColourDarker = Colour(AlphaTheme::getInstance()->foregroundColourDarker_);
    AlphaTheme::getInstance()->iconColour = Colour(AlphaTheme::getInstance()->iconColour_);
    AlphaTheme::getInstance()->iconOnColour = Colour(AlphaTheme::getInstance()->iconOnColour_);
    AlphaTheme::getInstance()->padColourBackground = Colour(AlphaTheme::getInstance()->padColourBackground_);
    AlphaTheme::getInstance()->padColour = Colour(AlphaTheme::getInstance()->padColour_);
    AlphaTheme::getInstance()->padColour2 = Colour(AlphaTheme::getInstance()->padColour2_);
    AlphaTheme::getInstance()->padTextColour = Colour(AlphaTheme::getInstance()->padTextColour_);
    AlphaTheme::getInstance()->tabColour = Colour(AlphaTheme::getInstance()->tabColour_);
    
}


//======================================================================
//=========================Overridden function==========================
//======================================================================
//======================================================================


void AlphaLiveLookandFeel::drawButtonBackground (Graphics& g,
                                        Button& button,
                                        const Colour& backgroundColour,
                                        bool isMouseOverButton,
                                        bool isButtonDown)
{
    
	switch (button.getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.6), Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), AlphaTheme::getInstance()->foregroundColourDarker, (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
            break;
        }
			
		case 1:
        {
            g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
            ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.9), backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), AlphaTheme::getInstance()->foregroundColourDarker, (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
			
            break;
        }
			
		case 2:
        {
			g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.4), backgroundColour, (button.getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), AlphaTheme::getInstance()->foregroundColourDarker, (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
			
            break;
        }
			
		case 3:
        {
			g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.7), backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), AlphaTheme::getInstance()->foregroundColourDarker, (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
			
			
            break;
        }
			
		case 4:
        {
			g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.9), backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), AlphaTheme::getInstance()->foregroundColourDarker, (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));			
			
            break;
        }
			
		case 5:
        {
			g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.8), backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), AlphaTheme::getInstance()->foregroundColourDarker, (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
            break;
        }
			
		default:
			break;
    }
	
}

Font AlphaLiveLookandFeel::getTextButtonFont (TextButton& button, int buttonHeight)
{
    return Font (jmin (15.0f, buttonHeight * 0.6f));
}

void AlphaLiveLookandFeel::drawButtonText (Graphics& g, TextButton& button,
                                  bool isMouseOverButton, bool isButtonDown)
{
    Font font (getTextButtonFont (button, button.getHeight()));
    g.setFont (10 + AlphaTheme::getInstance()->fontSizeAddition);
    g.setColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId
									: TextButton::textColourOffId)
				 .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));
	
    const int yIndent = jmin (4, button.proportionOfHeight (0.3f));
    const int cornerSize = jmin (button.getHeight(), button.getWidth()) / 2;
	
    const int fontHeight = roundToInt (font.getHeight() * 0.3f);
    const int leftIndent  = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
	
    g.drawFittedText (button.getButtonText(),
                      leftIndent,
                      yIndent,
                      button.getWidth() - leftIndent - rightIndent,
                      button.getHeight() - yIndent * 2,
                      Justification::centred, 2);
}


void AlphaLiveLookandFeel::drawComboBox (Graphics& g, int width, int height,
                                const bool isButtonDown,
                                int buttonX, int buttonY,
                                int buttonW, int buttonH,
                                ComboBox& box)
{
    g.fillAll (box.findColour (ComboBox::backgroundColourId));
	
	Path outline;
	LookAndFeelHelpers::createRoundedPath (outline, 0, 0, box.getWidth(), box.getHeight(), (box.getHeight()* 0.5),
										   true,
										   true,
										   true,
										   true);
	
    if (box.isEnabled() && box.hasKeyboardFocus (false))
    {
        g.setColour (box.findColour (ComboBox::outlineColourId));
		
		g.drawRect(0, 0, box.getWidth(), box.getHeight());
		
        //g.strokePath (outline, PathStrokeType (1.0f));
    }
    else
    {
        g.setColour (box.findColour (ComboBox::outlineColourId));
		
		g.drawRect(0, 0, box.getWidth(), box.getHeight());
		
        //g.strokePath (outline, PathStrokeType (1.0f));
    }
	
	
    if (box.isEnabled())
    {
       // const float arrowX = 0.2f;
        const float arrowH = 0.2f;
		
        Path p;
        p.addTriangle (buttonX + buttonW * 0.4f,            buttonY + buttonH * (0.45f - arrowH),
                       buttonX + buttonW * 0.6f, buttonY + buttonH * 0.45f,
                       buttonX + buttonW * 0.2f,          buttonY + buttonH * 0.45f);
		
        p.addTriangle (buttonX + buttonW * 0.4f,            buttonY + buttonH * (0.55f + arrowH),
                       buttonX + buttonW * 0.6f, buttonY + buttonH * 0.55f,
                       buttonX + buttonW * 0.2f,          buttonY + buttonH * 0.55f);
		
        g.setColour (box.findColour (ComboBox::arrowColourId));
        g.fillPath (p);
    }
}

Font AlphaLiveLookandFeel::getComboBoxFont (ComboBox& box)
{
    return Font (jmin (15.0f + AlphaTheme::getInstance()->fontSizeAddition,
                       box.getHeight() * 0.85f));
}

Label* AlphaLiveLookandFeel::createComboBoxTextBox (ComboBox&)
{
    return new Label (String(), String());
}

//Using this existing function with latest code version (app v1.4.0)
//causes combobox popupmenu text to be very small / badly positioned
//in certain instances.
//void AlphaLiveLookandFeel::positionComboBoxText (ComboBox& box, Label& label)
//{
//    label.setBounds (5, 3,
//                     box.getWidth() - box.getHeight(),
//                     box.getHeight() - 6);
//	
//    label.setFont (10 + AlphaTheme::getInstance()->fontSizeAddition);
//    
//}

void AlphaLiveLookandFeel::layoutFilenameComponent (FilenameComponent& filenameComp,
                                           ComboBox* filenameBox,
                                           Button* browseButton)
{
    browseButton->setColour(TextButton::buttonColourId, AlphaTheme::getInstance()->mainColour);
    browseButton->setSize (20, filenameComp.getHeight());
    
    //if (TextButton* const tb = dynamic_cast <TextButton*> (browseButton))
    //    tb->changeWidthToFitText();
    
    filenameBox->setBounds (browseButton->getWidth()+5, 0, filenameComp.getWidth()-(browseButton->getWidth()+5), filenameComp.getHeight());
     
}
 

//==============================================================================
void AlphaLiveLookandFeel::drawLabel (Graphics& g, Label& label)
{
    //following line is a hack to set the colour of a text editor
    //text while it is being typed in. This issue is desribed here:
    //http://www.rawmaterialsoftware.com/viewtopic.php?f=2&t=8311
    label.setColour (TextEditor::textColourId, Colours::white);
    
    //HACK - added this to set the combobox text colour - for some reason the offical way isn't working! << it is now!! :)
   // label.setColour (Label::textColourId, Colours::white);
    
    g.fillAll (label.findColour (Label::backgroundColourId));
    
    if (! label.isBeingEdited())
    {
        const float alpha = label.isEnabled() ? 1.0f : 0.5f;
        
        g.setColour (label.findColour (Label::textColourId).withMultipliedAlpha (alpha));
        g.setFont (label.getFont());
        g.drawFittedText (label.getText(),
                          label.getBorderSize().getLeft(),
                          label.getBorderSize().getTop(),
                          label.getWidth() - 2 * label.getBorderSize().getLeft(),
                          label.getHeight() - 2 * label.getBorderSize().getTop(),
                          label.getJustificationType(),
                          jmax (1, (int) (label.getHeight() / label.getFont().getHeight())),
                          label.getMinimumHorizontalScale());
        
        g.setColour (label.findColour (Label::outlineColourId).withMultipliedAlpha (alpha));
        g.drawRect (0, 0, label.getWidth(), label.getHeight());
    }
    else if (label.isEnabled())
    {
        g.setColour (label.findColour (Label::outlineColourId));
        g.drawRect (0, 0, label.getWidth(), label.getHeight());
    }
}

//SAMS DRAWLABEL() - WHATS CHANGED AND WHY?
/*
void AlphaLiveLookandFeel::drawLabel (Graphics& g, Label& label)
{
    //following line is a hack to set the colour of a text editor
    //text while it is being typed in. This issue is desribed here:
    //http://www.rawmaterialsoftware.com/viewtopic.php?f=2&t=8311
    label.setColour (TextEditor::textColourId, Colours::white);
    
    g.fillAll (label.findColour (Label::backgroundColourId));
	
	
    if (! label.isBeingEdited())
    {
        
        g.setColour (label.findColour (Label::textColourId));
        g.setFont (label.getFont());
        g.drawFittedText (label.getText(),
                          label.getHorizontalBorderSize(),
                          label.getVerticalBorderSize(),
                          label.getWidth() - 2 * label.getHorizontalBorderSize(),
                          label.getHeight() - 2 * label.getVerticalBorderSize(),
                          label.getJustificationType(),
                          jmax (1, (int) (label.getHeight() / label.getFont().getHeight())),
                          label.getMinimumHorizontalScale());
		
    }
   
	g.setColour (label.findColour (Label::outlineColourId));
	
	g.drawRect(0, 0, label.getWidth(), label.getHeight());
	
	
}
*/

void AlphaLiveLookandFeel::drawRotarySlider (Graphics& g,
                                    int x, int y,
                                    int width, int height,
                                    float sliderPos,
                                    const float rotaryStartAngle,
                                    const float rotaryEndAngle,
                                    Slider& slider)
{
    const float radius = jmin (width / 2, height / 2) - 2.0f;;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
	
    if (radius > 12.0f)
    {
        
        const float thickness = (1 - 0.2) + (0.2 * (radius * 0.0055));
        
        g.setColour(AlphaTheme::getInstance()->childBackgroundColour);
        Path backgroundArc;
        backgroundArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
        g.fillPath (backgroundArc);
        
        if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        else
            g.setColour (Colour (0x80808080));
		
        {
            Path filledArc;
            filledArc.addPieSegment (rx, ry, rw, rw,
									 rotaryStartAngle,
									 angle,
									 thickness);
			
            g.fillPath (filledArc);
        }
        
        g.setColour(slider.findColour (Slider::rotarySliderOutlineColourId).withAlpha (isMouseOver ? 1.0f : 0.5f));
        Path outlineArc;
        outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
        outlineArc.closeSubPath();
		
        g.strokePath (outlineArc, PathStrokeType(1.0f));
    }
    else
    {
        if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        else
            g.setColour (Colour (0x80808080));
		
        Path p;
        p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        PathStrokeType (rw * 0.1f).createStrokedPath (p, p);
		
        p.addLineSegment (Line<float> (0.0f, 0.0f, 0.0f, -radius), rw * 0.2f);
		
        g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));
    }
}


Font AlphaLiveLookandFeel::getPopupMenuFont() 
{
    return Font (13.0f + AlphaTheme::getInstance()->fontSizeAddition);
}

void AlphaLiveLookandFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
    g.setColour (findColour (PopupMenu::backgroundColourId));
    g.fillRect (0, 0, width, height);
    
    //g.setColour(findColour(ComboBox::outlineColourId));
    //g.drawRect(0, 0, width, height);
}


int AlphaLiveLookandFeel::getDefaultScrollbarWidth()
{
    return 10;
}

int AlphaLiveLookandFeel::getDefaultMenuBarHeight()
{
    return 20; //or 19, as the menu bar font is equal to 'height * 0.7' = 13.3, which is approx equal to what I have set the popup menu font to be
}

void AlphaLiveLookandFeel::drawMenuBarBackground (Graphics& g, int width, int height,
                                         bool, MenuBarComponent& menuBar)
{
	ColourGradient barGradient(AlphaTheme::getInstance()->foregroundColourDarker, 0, 0, AlphaTheme::getInstance()->backgroundColour, 0, height, false);
    g.setGradientFill(barGradient);
    g.fillRect (0, 0, width, height);

}


void AlphaLiveLookandFeel::drawFileBrowserRow (Graphics& g, int width, int height,
                                      const String& filename, Image* /*icon*/,
                                      const String& /*fileSizeDescription*/,
                                      const String& /*fileTimeDescription*/,
                                      const bool isDirectory,
                                      const bool isItemSelected,
                                      const int /*itemIndex*/,
                                      DirectoryContentsDisplayComponent&)
{
    if (isItemSelected)
        g.fillAll (findColour (DirectoryContentsDisplayComponent::highlightColourId));
    
    //remove the file extension from the filename String
    String filenameMod = filename.upToLastOccurrenceOf(".", false, false);
    
    //highlight the item if it is a directory
    if (isDirectory)
    {
        g.setColour(AlphaTheme::getInstance()->foregroundColourDarker.withAlpha(0.3f));
        g.fillRect(2, 2, width-4, height-4);
    }
    
    const int x = 32;
    g.setColour (findColour (DirectoryContentsDisplayComponent::textColourId));
    g.setFont (height * 0.7f);
    
    if (width > 450 && ! isDirectory)
    {
        const int sizeX = roundToInt (width * 0.7f);
        
        g.drawFittedText (filenameMod,
                          5, 0, sizeX - x, height,
                          Justification::centredLeft, 1);
        
        g.setFont (height * 0.5f);
        g.setColour (AlphaTheme::getInstance()->foregroundColourDarker);
        
    }
    else
    {
        g.drawFittedText (filenameMod,
                          5, 0, width - x, height,
                          Justification::centredLeft, 1);
        
    }
}
