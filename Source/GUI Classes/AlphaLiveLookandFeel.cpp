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
#include "Binary Data/BinaryDataNew.h"

AlphaTheme::AlphaTheme()
{
    
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
        const float tooltipFontSize = 13.0f;
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
    jassert (Colours::white == Colour (0xffffffff));
	
    //======================================================================
    //======================================================================
    
    if (theme == 1) //CLASSIC
    {
        // set background images here...
        
        AlphaTheme::getInstance()->mainImage = ImageCache::getFromMemory(BinaryDataNew::interfacemain_png,
                                                                           BinaryDataNew::interfacemain_pngSize);
        AlphaTheme::getInstance()->padsOffImage = ImageCache::getFromMemory(BinaryDataNew::padsoff_png,
                                                                              BinaryDataNew::padsoff_pngSize);
        AlphaTheme::getInstance()->padsOnImage = ImageCache::getFromMemory(BinaryDataNew::padson_png,
                                                                             BinaryDataNew::padson_pngSize);
        AlphaTheme::getInstance()->modeOffImage = ImageCache::getFromMemory(BinaryDataNew::modeoff_png,
                                                                              BinaryDataNew::modeoff_pngSize);
        AlphaTheme::getInstance()->padsBackgroundImage = ImageCache::getFromMemory(BinaryDataNew::padsbg_png,
                                                                                     BinaryDataNew::padsbg_pngSize);
        AlphaTheme::getInstance()->settingsOffImage = ImageCache::getFromMemory(BinaryDataNew::statusoff_png,
                                                                                  BinaryDataNew::statusoff_pngSize);
        
        //set custom and default AlphaLive colours here...
        
        AlphaTheme::getInstance()->colour1_ = 0xff1a54ab; //blue
        AlphaTheme::getInstance()->colour2_ = 0xff3c76c5; //light blue
        AlphaTheme::getInstance()->colour3_ = 0xff181818; //nearly black
        AlphaTheme::getInstance()->colour4_ = 0xff202020; //very dark grey
        
        AlphaTheme::getInstance()->colour1 = Colour(AlphaTheme::getInstance()->colour1_); //blue
        AlphaTheme::getInstance()->colour2 = Colour(AlphaTheme::getInstance()->colour2_); //light blue
        AlphaTheme::getInstance()->colour3 = Colour(AlphaTheme::getInstance()->colour3_); //nearly black
        AlphaTheme::getInstance()->colour4 = Colour(AlphaTheme::getInstance()->colour4_); //very dark grey
        
        // set up the standard set of colours..
        
        const uint32 textButtonColour      = 0xff000000;
        const uint32 textHighlightColour   = 0xffcccccc;
        const uint32 standardOutlineColour = 0xff3c3c3c;
        
        static const uint32 standardColours[] =
        {
            TextButton::buttonColourId,                 textButtonColour,
            TextButton::buttonOnColourId,               0xff1a56ab,
            TextButton::textColourOnId,                 0xffffffff,
            TextButton::textColourOffId,                0xffffffff,
            
            DrawableButton::backgroundColourId,         0x00000000,
            DrawableButton::backgroundOnColourId,       AlphaTheme::getInstance()->colour1_,
            
            
            ComboBox::buttonColourId,                   0xffcccccc,
            ComboBox::outlineColourId,                  standardOutlineColour,
            
            ToggleButton::textColourId,                 0xffffffff,
            
            TextEditor::backgroundColourId,             0x00000000,
            TextEditor::textColourId,                   0xffffffff,
            TextEditor::highlightColourId,              AlphaTheme::getInstance()->colour1_,
            TextEditor::highlightedTextColourId,        0xffffffff,
            TextEditor::outlineColourId,                0x00000000,
            TextEditor::focusedOutlineColourId,         textButtonColour,
            TextEditor::shadowColourId,                 0x38000000,
            
            CaretComponent::caretColourId,              0xff000000,
            
            Label::backgroundColourId,                  0x00000000,
            Label::textColourId,                        0xffffffff,
            Label::outlineColourId,                     0x00000000,
            
            ScrollBar::backgroundColourId,              0x00000000,
            ScrollBar::thumbColourId,                   0xffffffff,
            
            TreeView::linesColourId,                    0x4c000000,
            TreeView::backgroundColourId,               0xff000000,
            TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
            
            PopupMenu::backgroundColourId,              0x00000000,
            PopupMenu::textColourId,                    0xffffffff,
            PopupMenu::headerTextColourId,              0xff000000,
            PopupMenu::highlightedTextColourId,         0xffffffff,
            PopupMenu::highlightedBackgroundColourId,   AlphaTheme::getInstance()->colour1_,
            
            ComboBox::textColourId,                     0xffffffff,
            ComboBox::backgroundColourId,               0x0fcccccc,
            ComboBox::arrowColourId,                    0xffcccccc,
            
            ListBox::backgroundColourId,                0xff000000,
            ListBox::outlineColourId,                   standardOutlineColour,
            ListBox::textColourId,                      0xffffffff,
            
            Slider::backgroundColourId,                 0x00000000,
            Slider::thumbColourId,                      textButtonColour,
            Slider::trackColourId,                      AlphaTheme::getInstance()->colour1_,
            Slider::rotarySliderFillColourId,           AlphaTheme::getInstance()->colour1_,
            Slider::rotarySliderOutlineColourId,        0xff2c2c2c, //0xff3c3c3c
            Slider::textBoxTextColourId,                0xffffffff,
            Slider::textBoxBackgroundColourId,          0x00000000,
            Slider::textBoxHighlightColourId,           textHighlightColour,
            Slider::textBoxOutlineColourId,             standardOutlineColour,
            
            ResizableWindow::backgroundColourId,        0xff777777,
            //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)
            
            AlertWindow::backgroundColourId,            0xffededed,
            AlertWindow::textColourId,                  0xff000000,
            AlertWindow::outlineColourId,               0xff666666,
            
            ProgressBar::backgroundColourId,            AlphaTheme::getInstance()->colour4_,
            ProgressBar::foregroundColourId,            0xff808080, //grey
            
            TooltipWindow::backgroundColourId,          0x00000000,
            TooltipWindow::textColourId,                0xffffffff,
            TooltipWindow::outlineColourId,             0x4c000000,
            
            TabbedComponent::backgroundColourId,        0x00000000,
            TabbedComponent::outlineColourId,           0xff000000,
            TabbedButtonBar::tabOutlineColourId,        0x80000000,
            TabbedButtonBar::frontOutlineColourId,      0x90000000,
            
            Toolbar::backgroundColourId,                0xfff6f8f9,
            Toolbar::separatorColourId,                 0x4c000000,
            Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
            Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
            Toolbar::labelTextColourId,                 0xf0000000,
            Toolbar::editingModeOutlineColourId,        0xffff0000,
            
            HyperlinkButton::textColourId,              0xcc1111ee,
            
            GroupComponent::outlineColourId,            0x66000000,
            GroupComponent::textColourId,               0xffffffff,
            
            DirectoryContentsDisplayComponent::highlightColourId,   AlphaTheme::getInstance()->colour1_, // <-- used for fileList and fileTree item's
            DirectoryContentsDisplayComponent::textColourId,        0xffffffff,
            
            0x1000440, /*LassoComponent::lassoFillColourId*/        0x66dddddd,
            0x1000441, /*LassoComponent::lassoOutlineColourId*/     0x99111111,
            
            0x1005000, /*MidiKeyboardComponent::whiteNoteColourId*/               0xffffffff,
            0x1005001, /*MidiKeyboardComponent::blackNoteColourId*/               0xff000000,
            0x1005002, /*MidiKeyboardComponent::keySeparatorLineColourId*/        0x66000000,
            0x1005003, /*MidiKeyboardComponent::mouseOverKeyOverlayColourId*/     0x80ffff00,
            0x1005004, /*MidiKeyboardComponent::keyDownOverlayColourId*/          0xffb6b600,
            0x1005005, /*MidiKeyboardComponent::textLabelColourId*/               0xff000000,
            0x1005006, /*MidiKeyboardComponent::upDownButtonBackgroundColourId*/  0xffd3d3d3,
            0x1005007, /*MidiKeyboardComponent::upDownButtonArrowColourId*/       0xff000000,
            
            0x1004500, /*CodeEditorComponent::backgroundColourId*/                0xffffffff,
            0x1004502, /*CodeEditorComponent::highlightColourId*/                 textHighlightColour,
            0x1004503, /*CodeEditorComponent::defaultTextColourId*/               0xffffffff,
            
            0x1007000, /*ColourSelector::backgroundColourId*/                     0xff000000,
            0x1007001, /*ColourSelector::labelTextColourId*/                      0xffffffff,
            
            0x100ad00, /*KeyMappingEditorComponent::backgroundColourId*/          0x00000000,
            0x100ad01, /*KeyMappingEditorComponent::textColourId*/                0xff000000,
            
            FileSearchPathListComponent::backgroundColourId,        0x00000000,
            
            FileChooserDialogBox::titleTextColourId,                0xffffffff,
            
            DrawableButton::textColourId,                           0xffffffff,
        };
        
        for (int i = 0; i < numElementsInArray (standardColours); i += 2)
            setColour (standardColours [i], Colour ((uint32) standardColours [i + 1]));
    }
    
    //======================================================================
    //======================================================================
    
    else if (theme == 2) //MATERIA
    {
        //set background images here...
        
        AlphaTheme::getInstance()->mainImage = ImageCache::getFromMemory(BinaryDataNew::materia_interfacemain_png,
                                                                           BinaryDataNew::materia_interfacemain_pngSize);
        AlphaTheme::getInstance()->padsOffImage = ImageCache::getFromMemory(BinaryDataNew::materia_padsoff_png,
                                                                              BinaryDataNew::materia_padsoff_pngSize);
        AlphaTheme::getInstance()->padsOnImage = ImageCache::getFromMemory(BinaryDataNew::materia_padson_png,
                                                                             BinaryDataNew::materia_padson_pngSize);
        AlphaTheme::getInstance()->modeOffImage = ImageCache::getFromMemory(BinaryDataNew::materia_modeoff_png,
                                                                              BinaryDataNew::materia_modeoff_pngSize);
        AlphaTheme::getInstance()->padsBackgroundImage = ImageCache::getFromMemory(BinaryDataNew::materia_padsbg_png,
                                                                                     BinaryDataNew::materia_padsbg_pngSize);
        AlphaTheme::getInstance()->settingsOffImage = ImageCache::getFromMemory(BinaryDataNew::materia_statusoff_png,
                                                                                  BinaryDataNew::materia_statusoff_pngSize);
        
        //set custom and default AlphaLive colours here...
        
        AlphaTheme::getInstance()->colour1_ = 0xff1a54ab; //blue
        AlphaTheme::getInstance()->colour2_ = 0xff3c76c5; //light blue
        AlphaTheme::getInstance()->colour3_ = 0xff181818; //nearly black
        AlphaTheme::getInstance()->colour4_ = 0xff202020; //very dark grey
        
        AlphaTheme::getInstance()->colour1 = Colour(AlphaTheme::getInstance()->colour1_); //blue
        AlphaTheme::getInstance()->colour2 = Colour(AlphaTheme::getInstance()->colour2_); //light blue
        AlphaTheme::getInstance()->colour3 = Colour(AlphaTheme::getInstance()->colour3_); //nearly black
        AlphaTheme::getInstance()->colour4 = Colour(AlphaTheme::getInstance()->colour4_); //very dark grey
        
        // set up the standard set of colours..
        const uint32 textButtonColour      = 0xff000000;
        const uint32 textHighlightColour   = 0xffcccccc;
        const uint32 standardOutlineColour = 0xff3c3c3c;
        
        static const uint32 standardColours[] =
        {
            TextButton::buttonColourId,                 textButtonColour,
            TextButton::buttonOnColourId,               0xff1a56ab,
            TextButton::textColourOnId,                 0xffffffff,
            TextButton::textColourOffId,                0xffffffff,
            
            DrawableButton::backgroundColourId,         0x00000000,
            DrawableButton::backgroundOnColourId,       AlphaTheme::getInstance()->colour1_,
            
            
            ComboBox::buttonColourId,                   0xffcccccc,
            ComboBox::outlineColourId,                  standardOutlineColour,
            
            ToggleButton::textColourId,                 0xffffffff,
            
            TextEditor::backgroundColourId,             0x00000000,
            TextEditor::textColourId,                   0xffffffff,
            TextEditor::highlightColourId,              AlphaTheme::getInstance()->colour1_,
            TextEditor::highlightedTextColourId,        0xffffffff,
            TextEditor::outlineColourId,                0x00000000,
            TextEditor::focusedOutlineColourId,         textButtonColour,
            TextEditor::shadowColourId,                 0x38000000,
            
            CaretComponent::caretColourId,              0xff000000,
            
            Label::backgroundColourId,                  0x00000000,
            Label::textColourId,                        0xffffffff,
            Label::outlineColourId,                     0x00000000,
            
            ScrollBar::backgroundColourId,              0x00000000,
            ScrollBar::thumbColourId,                   0xffffffff,
            
            TreeView::linesColourId,                    0x4c000000,
            TreeView::backgroundColourId,               0xff000000,
            TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
            
            PopupMenu::backgroundColourId,              0x00000000,
            PopupMenu::textColourId,                    0xffffffff,
            PopupMenu::headerTextColourId,              0xff000000,
            PopupMenu::highlightedTextColourId,         0xffffffff,
            PopupMenu::highlightedBackgroundColourId,   AlphaTheme::getInstance()->colour1_,
            
            ComboBox::textColourId,                     0xffffffff,
            ComboBox::backgroundColourId,               0x0fcccccc,
            ComboBox::arrowColourId,                    0xffcccccc,
            
            ListBox::backgroundColourId,                0xff000000,
            ListBox::outlineColourId,                   standardOutlineColour,
            ListBox::textColourId,                      0xffffffff,
            
            Slider::backgroundColourId,                 0x00000000,
            Slider::thumbColourId,                      textButtonColour,
            Slider::trackColourId,                      AlphaTheme::getInstance()->colour1_,
            Slider::rotarySliderFillColourId,           AlphaTheme::getInstance()->colour1_,
            Slider::rotarySliderOutlineColourId,        0xff2c2c2c, //0xff3c3c3c
            Slider::textBoxTextColourId,                0xffffffff,
            Slider::textBoxBackgroundColourId,          0x00000000,
            Slider::textBoxHighlightColourId,           textHighlightColour,
            Slider::textBoxOutlineColourId,             standardOutlineColour,
            
            ResizableWindow::backgroundColourId,        0xff777777,
            //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)
            
            AlertWindow::backgroundColourId,            0xffededed,
            AlertWindow::textColourId,                  0xff000000,
            AlertWindow::outlineColourId,               0xff666666,
            
            ProgressBar::backgroundColourId,            AlphaTheme::getInstance()->colour4_,
            ProgressBar::foregroundColourId,            0xff808080, //grey
            
            TooltipWindow::backgroundColourId,          0x00000000,
            TooltipWindow::textColourId,                0xffffffff,
            TooltipWindow::outlineColourId,             0x4c000000,
            
            TabbedComponent::backgroundColourId,        0x00000000,
            TabbedComponent::outlineColourId,           0xff000000,
            TabbedButtonBar::tabOutlineColourId,        0x80000000,
            TabbedButtonBar::frontOutlineColourId,      0x90000000,
            
            Toolbar::backgroundColourId,                0xfff6f8f9,
            Toolbar::separatorColourId,                 0x4c000000,
            Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
            Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
            Toolbar::labelTextColourId,                 0xf0000000,
            Toolbar::editingModeOutlineColourId,        0xffff0000,
            
            HyperlinkButton::textColourId,              0xcc1111ee,
            
            GroupComponent::outlineColourId,            0x66000000,
            GroupComponent::textColourId,               0xffffffff,
            
            DirectoryContentsDisplayComponent::highlightColourId,   AlphaTheme::getInstance()->colour1_, // <-- used for fileList and fileTree item's
            DirectoryContentsDisplayComponent::textColourId,        0xffffffff,
            
            0x1000440, /*LassoComponent::lassoFillColourId*/        0x66dddddd,
            0x1000441, /*LassoComponent::lassoOutlineColourId*/     0x99111111,
            
            0x1005000, /*MidiKeyboardComponent::whiteNoteColourId*/               0xffffffff,
            0x1005001, /*MidiKeyboardComponent::blackNoteColourId*/               0xff000000,
            0x1005002, /*MidiKeyboardComponent::keySeparatorLineColourId*/        0x66000000,
            0x1005003, /*MidiKeyboardComponent::mouseOverKeyOverlayColourId*/     0x80ffff00,
            0x1005004, /*MidiKeyboardComponent::keyDownOverlayColourId*/          0xffb6b600,
            0x1005005, /*MidiKeyboardComponent::textLabelColourId*/               0xff000000,
            0x1005006, /*MidiKeyboardComponent::upDownButtonBackgroundColourId*/  0xffd3d3d3,
            0x1005007, /*MidiKeyboardComponent::upDownButtonArrowColourId*/       0xff000000,
            
            0x1004500, /*CodeEditorComponent::backgroundColourId*/                0xffffffff,
            0x1004502, /*CodeEditorComponent::highlightColourId*/                 textHighlightColour,
            0x1004503, /*CodeEditorComponent::defaultTextColourId*/               0xffffffff,
            
            0x1007000, /*ColourSelector::backgroundColourId*/                     0xff000000,
            0x1007001, /*ColourSelector::labelTextColourId*/                      0xffffffff,
            
            0x100ad00, /*KeyMappingEditorComponent::backgroundColourId*/          0x00000000,
            0x100ad01, /*KeyMappingEditorComponent::textColourId*/                0xff000000,
            
            FileSearchPathListComponent::backgroundColourId,        0x00000000,
            
            FileChooserDialogBox::titleTextColourId,                0xffffffff,
            
            DrawableButton::textColourId,                           0xffffffff,
        };
        
        for (int i = 0; i < numElementsInArray (standardColours); i += 2)
            setColour (standardColours [i], Colour ((uint32) standardColours [i + 1]));
    }
    
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
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
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
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
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
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
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
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
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
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
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
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
            break;
        }
			
		default:
			break;
    }
	
}

Font AlphaLiveLookandFeel::getTextButtonFont (TextButton& button)
{
    return button.getFont();
}

void AlphaLiveLookandFeel::drawButtonText (Graphics& g, TextButton& button,
                                  bool isMouseOverButton, bool isButtonDown)
{
    Font font (getTextButtonFont (button));
    g.setFont (10);
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


void AlphaLiveLookandFeel::positionComboBoxText (ComboBox& box, Label& label)
{
    label.setBounds (5, 3,
                     box.getWidth() - box.getHeight(),
                     box.getHeight() - 6);
	
    label.setFont (10);
    
}



void AlphaLiveLookandFeel::layoutFilenameComponent (FilenameComponent& filenameComp,
                                           ComboBox* filenameBox,
                                           Button* browseButton)
{
    browseButton->setColour(TextButton::buttonColourId, AlphaTheme::getInstance()->colour1);
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
                          label.getHorizontalBorderSize(),
                          label.getVerticalBorderSize(),
                          label.getWidth() - 2 * label.getHorizontalBorderSize(),
                          label.getHeight() - 2 * label.getVerticalBorderSize(),
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
        if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        else
            g.setColour (Colour (0x80808080));
		
        const float thickness = (1 - 0.2) + (0.2 * (radius * 0.0055));
		
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
		
        g.strokePath (outlineArc, 1.0f);
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
    return Font (13.0f);
}

void AlphaLiveLookandFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
    const Colour background (findColour (PopupMenu::backgroundColourId));
    
    g.fillAll (background);
    
	//overlays colour/shape
    g.setColour (background.overlaidWith (Colour (AlphaTheme::getInstance()->colour3_).withAlpha(1.0f)));
    g.fillRect (0, 0, width, height);
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
	ColourGradient barGradient(Colours::darkgrey, 0, 0, Colours::black, 0, height, false);
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
        g.setColour(Colours::darkgrey.withAlpha(0.3f));
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
        g.setColour (Colours::darkgrey);
        
    }
    else
    {
        g.drawFittedText (filenameMod,
                          5, 0, width - x, height,
                          Justification::centredLeft, 1);
        
    }
}
