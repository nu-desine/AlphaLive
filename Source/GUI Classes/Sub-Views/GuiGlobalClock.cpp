//
//  GuiGlobalClock.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 09/02/2012.
//  Copyright 2012 nu desine.
//
//  This file is part of AlphaLive.
//
//  AlphaLive is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2, 
//  as published by the Free Software Foundation.
//  
//  AlphaLive is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "GuiGlobalClock.h"
#include "../Views/MainComponent.h"
#include "../../File and Settings/AppSettings.h"
#include "../AlphaLiveLookandFeel.h"
#include "../../Application/CommonInfoBoxText.h"
#include "../Binary Data/MainBinaryData.h"

#define OFFSET_X 479
#define OFFSET_Y 6

GuiGlobalClock::GuiGlobalClock(MainComponent &ref, AlphaLiveEngine &ref2)
:   mainComponentRef(ref),
    alphaLiveEngineRef(ref2)
{
    //tempo slider (IncDecButtons)
    addAndMakeVisible(tempoSlider = new AlphaSlider());
    tempoSlider->setRange(60.0, 360.0, 0.1);
    tempoSlider->addListener(this);
    tempoSlider->setValue(120.0);
    tempoSlider->addMouseListener(this, true);
	
	//beats per bar buttons
    for (int i = 0; i < 6; i++)
    {
        beatsPerBarButtons.insert(i, new AlphaTextButton(String(i+2)));
        beatsPerBarButtons[i]->setClickingTogglesState(true);
        beatsPerBarButtons[i]->addListener(this);
        beatsPerBarButtons[i]->addMouseListener(this, true);
        beatsPerBarButtons[i]->setRadioGroupId (1234);
        
        addAndMakeVisible(beatsPerBarButtons[i]);
    }
	
    beatsPerBarButtons[2]->setToggleState(true, dontSendNotification);
    beatsPerBar_ = 4;
	
	//quantisation buttons
    for (int i = 0; i < 6; i++)
    {
        if (i == 0)
           quantizationValueButtons.insert(i, new SettingsButton("1/4", (238.5 * (M_PI / 180)), (268.5 * (M_PI / 180)), 0.75f, -10, 0.2, 0.28)); 
        else if (i == 1)
            quantizationValueButtons.insert(i, new SettingsButton("1/2", (268.5 * (M_PI / 180)), (298.5 * (M_PI / 180)), 0.75f, -5, 0.2, 0.28));
        else if (i == 2)
            quantizationValueButtons.insert(i, new SettingsButton("1", (298.5 * (M_PI / 180)), (328.5 * (M_PI / 180)), 0.75f, 0, 0.4, 0.28));
        else if (i == 3)
            quantizationValueButtons.insert(i, new SettingsButton("1", (333.5 * (M_PI / 180)), (356 * (M_PI / 180)), 0.75f, -3, 0.38, 0.28));
        else if (i == 4)
            quantizationValueButtons.insert(i, new SettingsButton("2", (356 * (M_PI / 180)), (378.5 * (M_PI / 180)), 0.75f, 0, 0.4, 0.28));
        else if (i == 5)
            quantizationValueButtons.insert(i, new SettingsButton("4", (378.5 * (M_PI / 180)), (401 * (M_PI / 180)), 0.75f, 0, 0.4, 0.28));
        
        quantizationValueButtons[i]->addListener(this);
        quantizationValueButtons[i]->addMouseListener(this, false);
        quantizationValueButtons[i]->setRadioGroupId (43);
        addAndMakeVisible(quantizationValueButtons[i]);
        
    }
    
    quantizationValueButtons[3]->setToggleState(true, dontSendNotification);

    //transport button
	addAndMakeVisible(transportButton = new AlphaTextButton());
    transportButton->setButtonText(translate("START"));
    transportButton->setClickingTogglesState(true);
    transportButton->setToggleState(false, dontSendNotification);
    transportButton->addListener(this);
    transportButton->addMouseListener(this, true);
	
	
    //auto-start switch
	Image *audioStartImage = new Image(ImageCache::getFromMemory(MainBinaryData::autostartsymbol_png, MainBinaryData::autostartsymbol_pngSize));
	addAndMakeVisible(autoStartSwitch = new ModeButton(audioStartImage));
    autoStartSwitch->addListener(this);
    autoStartSwitch->setClickingTogglesState(true);
    autoStartSwitch->addMouseListener(this, true);
     
    //metronome button
    Image *metronomeImage = new Image(ImageCache::getFromMemory(MainBinaryData::metronomeicon_png, MainBinaryData::metronomeicon_pngSize));
	addAndMakeVisible(metronomeButton = new ModeButton(metronomeImage));
    metronomeButton->setClickingTogglesState(true);	
	metronomeButton->addListener(this);
	metronomeButton->addMouseListener(this, true);
    
    //sync label (that hides the tempo control when synced to an external clock)
    addChildComponent(syncLabel = new Label());
    syncLabel->setText(translate("SYNC"), dontSendNotification);
    syncLabel->setFont(Font (11.f + AlphaTheme::getInstance()->fontSizeAddition));
    syncLabel->setJustificationType(Justification::centred);
    
	currentStepNumber = 0;
	
	countGap = (4 * (M_PI / 180));
	countSeg = 0;
	segStart = 0;
	segEnd = 0;  
}

GuiGlobalClock::~GuiGlobalClock()
{
    beatsPerBarButtons.clear();
    quantizationValueButtons.clear();
    
    deleteAllChildren();
}

void GuiGlobalClock::resized()
{
    transportButton->setBounds(644-OFFSET_X, 42, 56, 56);
    tempoSlider->setBounds(550-OFFSET_X, 12, 50, 50);
    syncLabel->setBounds(555-OFFSET_X, 29, 40, 15);
    metronomeButton->setBounds (489-OFFSET_X, 8, 16, 16);
    autoStartSwitch->setBounds (518-OFFSET_X, 10, 24, 24);
	
	beatsPerBarButtons[0]->setBounds(631-OFFSET_X, 111, 17, 17);
	beatsPerBarButtons[1]->setBounds(659-OFFSET_X, 121, 17, 17);
	beatsPerBarButtons[2]->setBounds(689-OFFSET_X, 115, 17, 17);
	beatsPerBarButtons[3]->setBounds(712-OFFSET_X, 95, 17, 17);
	beatsPerBarButtons[4]->setBounds(722-OFFSET_X, 67, 17, 17);
	beatsPerBarButtons[5]->setBounds(716-OFFSET_X, 37, 17, 17);
	
    for (int i = 0; i < 6; i++)
        quantizationValueButtons[i]->setBounds(606-OFFSET_X, 6, 131, 131);
	
}

void GuiGlobalClock::paint (Graphics &g)
{
    Path quantiseBg, barsBg;
    
    //Draw semi-circle background for quantise buttons
	quantiseBg.addPieSegment(603-OFFSET_X, 3, 137, 137, (235.5 * (M_PI / 180)), (404 * (M_PI / 180)), 0.5f);
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	g.fillPath(quantiseBg, getTransform());
	
    //Draw circle behind clock display segments
	g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.2f));
	g.fillEllipse(634-OFFSET_X, 32, 76, 76);
	
    //Draw clock display segments background
	barsBg.addPieSegment(626-OFFSET_X, 24, 93, 93, 0, (90 * (M_PI / 180)), 0.5f);
	barsBg.addPieSegment(626-OFFSET_X, 24, 93, 93, (180 * (M_PI / 180)), (270 * (M_PI / 180)), 0.5f);
	g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.2f));
	g.fillPath(barsBg, getTransform());
	
    //Draw circles around the buttons
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	g.fillEllipse(639-OFFSET_X, 37, 66, 66);
	g.fillEllipse(515-OFFSET_X, 7, 30, 30);
	g.fillEllipse(486-OFFSET_X, 5, 22, 22);
	
    //=====Draw clock display segments=====
    //We are now drawing each beat/segment every time, using the
    //currentStepNumber value to determine how many segments to draw.
    //This is an improvement over the previous method of drawing
    //one at a time for two reasons:
    // - If for some reason this class misses a beat (which seems to not
    // happen so much (if at all?) with this new method), it will now
    // correct itself on the next beat instead of being one beat for
    // the duration of the clocks runtime.
    // - If the beats per bar value is changed, the display is correctly
    // updated the next time a beat is displayed as opposed to only when
    // we get back to the beginning of the clock like before.
    
    barCount.clear();
    countSeg = ((M_PI/2) - (countGap * beatsPerBar_)) / beatsPerBar_;
    
    for (int i = 0; i < currentStepNumber; i++)
    {
        if (i == 0)
        {
            segStart = countGap * 0.5;
            segEnd = segStart + countSeg; 
        }
        else
        {
            segStart = segEnd + countGap;
            segEnd = segStart + countSeg;
        }
        
        barCount.addPieSegment(627-OFFSET_X, 26, 90, 90, segStart, segEnd, 0.9f);
    }
    
    g.setColour(AlphaTheme::getInstance()->mainColour);
	g.fillPath(barCount, getTransform());
    
    //set colours of components here so they repaint when the theme changes
    syncLabel->setColour(Label::backgroundColourId, AlphaTheme::getInstance()->childBackgroundColour);
}

void GuiGlobalClock::updateClockDisplay (int beatNumber, int barNumber, int beatsPerBar)
{
    beatsPerBar_ = beatsPerBar;
    
    currentStepNumber = (beatsPerBar * barNumber) - (beatsPerBar - beatNumber);
    
    repaint(606-OFFSET_X, 6, 131, 131);
}

void GuiGlobalClock::updateTransportButtonDisplay (bool status)
{
    if (AppSettings::Instance()->getMidiClockValue() == 3)
    {
        transportButton->setButtonText(translate("EXTERNAL") + "\n" + translate("SYNC"));
        syncLabel->setVisible(true);
    }
    else if (status == true)
    {
        transportButton->setToggleState(true, dontSendNotification);
        transportButton->setButtonText(translate("STOP"));
        
        syncLabel->setVisible(false);
    }
    else
    {
        transportButton->setToggleState(false, dontSendNotification);
        transportButton->setButtonText(translate("START"));
        
        syncLabel->setVisible(false);
    }
}

void GuiGlobalClock::updateTempoDisplay (float value)
{
    tempoSlider->setValue(value);
}

void GuiGlobalClock::sliderValueChanged (Slider* slider)
{
    if (slider == tempoSlider)
    {
        AppSettings::Instance()->setGlobalTempo(tempoSlider->getValue());
    }
}

void GuiGlobalClock::buttonClicked (Button* button)
{
    
    for (int i = 0; i < 6; i++)
    {
       if (button == beatsPerBarButtons[i]) 
           AppSettings::Instance()->setBeatsPerBar(i+2);
        
        else if(button == quantizationValueButtons[i])
            AppSettings::Instance()->setQuantizationValue(6-i);
    }
    
	
    if (button == transportButton)
    {
        //don't allow the button press to do anything if set to sync to an external MIDI Clock
        if (AppSettings::Instance()->getMidiClockValue() != 3)
        {
            if (transportButton->getToggleState() == true)
            {
                alphaLiveEngineRef.getGlobalClock()->startClock();
                transportButton->setButtonText(translate("STOP"));
                mainComponentRef.setIsClockRunning(true);
            }
            else
            {
                alphaLiveEngineRef.getGlobalClock()->stopClock();
                transportButton->setButtonText(translate("START"));
                mainComponentRef.setIsClockRunning(false);
            }
        }
        else
        {
            int state = 0;
            if (button->getToggleState() == 0)
                state = 1;
            
            button->setToggleState(state, dontSendNotification);
        }
    }
    
    else if (button == metronomeButton)
    {
        AppSettings::Instance()->setMetronomeStatus(metronomeButton->getToggleState());
    }
    
    else if (button == autoStartSwitch)
    {
        AppSettings::Instance()->setAutoStartClock(autoStartSwitch->getToggleState());
    }
     
}

void GuiGlobalClock::toggleTransportButtonOff()
{
    if (transportButton->getToggleState() == true)
    {
        transportButton->setToggleState(false, dontSendNotification);
        transportButton->setButtonText(translate("START"));
        mainComponentRef.setIsClockRunning(false);
    }
    
}

void GuiGlobalClock::triggerTransportButton()
{
    transportButton->triggerClick();
}

void GuiGlobalClock::updateDisplay()
{
    tempoSlider->setValue(AppSettings::Instance()->getGlobalTempo());
	
    beatsPerBarButtons[AppSettings::Instance()->getBeatsPerBar()-2]->setToggleState(true, dontSendNotification);
    quantizationValueButtons[6-AppSettings::Instance()->getQuantizationValue()]->setToggleState(true, dontSendNotification);
    autoStartSwitch->setToggleState(AppSettings::Instance()->getAutoStartClock(), dontSendNotification);
    
    updateTransportButtonDisplay(transportButton->getToggleState());
        

}

Slider* GuiGlobalClock::getTempoSlider()
{
    return tempoSlider;
}

Button* GuiGlobalClock::getMetronomeButton()
{
    return metronomeButton;
}

void GuiGlobalClock::mouseEnter (const MouseEvent &e)
{
    if (tempoSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText (translate("Global Tempo Selector. Sets the tempo of the global clock and any sequences."));
    }
    
    else if (transportButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText (translate("Clock Start/Stop. This button is used to manually start and stop the global clock. Please note that this button will be disabled if the current project is set to sync with an external MIDI clock - here the global clock can only controlled with the external clock."));
    }
    
     else if (autoStartSwitch->isMouseOver(true))
     {
     mainComponentRef.setInfoTextBoxText (translate("Clock Auto-Start Button. When enabled, the clock will start automatically when a note, sample or sequence is played. If this is off the clock will need to be started manually using the Start/Stop button."));
     }
     else if (metronomeButton->isMouseOver(true))
     {
         mainComponentRef.setInfoTextBoxText (translate("Metronome. When enabled, a click track will be played in time with the global clock."));
     }
    
    else if (quantizationValueButtons[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText (translate("Quarter Beat Quantization.") + " " + translate(CommonInfoBoxText::quantizationValueButtons));
    }
    
    else if (quantizationValueButtons[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText (translate("Half Beat Quantization.") + " " + translate(CommonInfoBoxText::quantizationValueButtons));
    }
    else if (quantizationValueButtons[2]->isMouseOver(true))
    {
       mainComponentRef.setInfoTextBoxText (translate("One Beat Quantization.") + " " + translate(CommonInfoBoxText::quantizationValueButtons));
    }
    else if (quantizationValueButtons[3]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText (translate("One Bar Quantization.") + " " + translate(CommonInfoBoxText::quantizationValueButtons));
    }
    else if (quantizationValueButtons[4]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText (translate("Two Bars Quantization.") + " " + translate(CommonInfoBoxText::quantizationValueButtons));
    }
    else if (quantizationValueButtons[5]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText (translate("Four Bars Quantization.") + " " + translate(CommonInfoBoxText::quantizationValueButtons));
    }
    
    for (int i = 0; i < 6; i++)
    {
        if (beatsPerBarButtons[i]->isMouseOver(true))
        {
            mainComponentRef.setInfoTextBoxText (String (i+2) + " " + translate("Beats-Per-Bar. This set of buttons allows a basic time signature for the global clock to be set."));
        }
    }
    
}

void GuiGlobalClock::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String());
}
