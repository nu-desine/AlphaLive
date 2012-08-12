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
#include "AlphaLiveLookandFeel.h"

#define OFFSET_X 479
#define OFFSET_Y 6


GuiGlobalClock::GuiGlobalClock(MainComponent &ref, AlphaLiveEngine &ref2)
:   mainComponentRef(ref),
    alphaLiveEngineRef(ref2)
{
    mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->attach(this);
    
    //tempo slider (IncDecButtons)
    addAndMakeVisible(tempoSlider = new AlphaSlider());
    tempoSlider->setRange(60.0, 360.0, 0.1);
    tempoSlider->addListener(this);
    tempoSlider->setComponentValue(120.0);
    tempoSlider->addMouseListener(this, true);
	
	//beats per bar buttons
	addAndMakeVisible(twoFour = new AlphaTextButton());
    twoFour->setButtonText("2");
    twoFour->setClickingTogglesState(true);
    twoFour->setToggleState(false, false);
    twoFour->addListener(this);
    twoFour->addMouseListener(this, true);
	twoFour->setRadioGroupId (1234);
	twoFour->setOpaque(false);
	
	addAndMakeVisible(threeFour = new AlphaTextButton());
    threeFour->setButtonText("3");
    threeFour->setClickingTogglesState(true);
    threeFour->setToggleState(false, false);
    threeFour->addListener(this);
    threeFour->addMouseListener(this, true);
	threeFour->setRadioGroupId (1234);
	threeFour->setOpaque(false);
	
	addAndMakeVisible(fourFour = new AlphaTextButton());
    fourFour->setButtonText("4");
    fourFour->setClickingTogglesState(true);
    fourFour->setToggleState(true, false);
    fourFour->addListener(this);
    fourFour->addMouseListener(this, true);
	fourFour->setRadioGroupId (1234);
	fourFour->setOpaque(false);
	
	addAndMakeVisible(fiveFour = new AlphaTextButton());
    fiveFour->setButtonText("5");
    fiveFour->setClickingTogglesState(true);
    fiveFour->setToggleState(false, false);
    fiveFour->addListener(this);
    fiveFour->addMouseListener(this, true);
	fiveFour->setRadioGroupId (1234);
	fiveFour->setOpaque(false);
	
	addAndMakeVisible(sixFour = new AlphaTextButton());
    sixFour->setButtonText("6");
    sixFour->setClickingTogglesState(true);
    sixFour->setToggleState(false, false);
    sixFour->addListener(this);
    sixFour->addMouseListener(this, true);
	sixFour->setRadioGroupId (1234);
	sixFour->setOpaque(false);
	
	addAndMakeVisible(sevenFour = new AlphaTextButton());
    sevenFour->setButtonText("7");
    sevenFour->setClickingTogglesState(true);
    sevenFour->setToggleState(false, false);
    sevenFour->addListener(this);
    sevenFour->addMouseListener(this, true);
	sevenFour->setRadioGroupId (1234);
	sevenFour->setOpaque(false);
	
	//quantisation buttons
	addAndMakeVisible(fourBar = new SettingsButton("4", (378.5 * (M_PI / 180)), (401 * (M_PI / 180)), 0.75f, 0, 0.4, 0.28));
	fourBar->addListener(this);
	fourBar->setOpaque(false);
	fourBar->setRadioGroupId (43);
	fourBar->addMouseListener(this, false);	
	
	addAndMakeVisible(twoBar = new SettingsButton("2", (356 * (M_PI / 180)), (378.5 * (M_PI / 180)), 0.75f, 0, 0.4, 0.28));
	twoBar->addListener(this);
	twoBar->setOpaque(false);
	twoBar->setRadioGroupId (43);
	twoBar->addMouseListener(this, false);
	
	addAndMakeVisible(oneBar = new SettingsButton("1", (333.5 * (M_PI / 180)), (356 * (M_PI / 180)), 0.75f, -3, 0.38, 0.28));
	oneBar->addListener(this);
	oneBar->setOpaque(false);
	oneBar->setRadioGroupId (43);
	oneBar->addMouseListener(this, false);
	//oneBar->triggerClick();
    oneBar->setToggleState(true, false);
	
	addAndMakeVisible(oneBeat = new SettingsButton("1", (298.5 * (M_PI / 180)), (328.5 * (M_PI / 180)), 0.75f, 0, 0.4, 0.28));
	oneBeat->addListener(this);
	oneBeat->setOpaque(false);
	oneBeat->setRadioGroupId (43);
	oneBeat->addMouseListener(this, false);	
	
	addAndMakeVisible(halfBeat = new SettingsButton("1/2", (268.5 * (M_PI / 180)), (298.5 * (M_PI / 180)), 0.75f, -5, 0.2, 0.28));
	halfBeat->addListener(this);
	halfBeat->setOpaque(false);
	halfBeat->setRadioGroupId (43);
	halfBeat->addMouseListener(this, false);	
	
	addAndMakeVisible(quarterBeat = new SettingsButton("1/4", (238.5 * (M_PI / 180)), (268.5 * (M_PI / 180)), 0.75f, -10, 0.2, 0.28));
	quarterBeat->addListener(this);
	quarterBeat->setOpaque(false);
	quarterBeat->setRadioGroupId (43);
	quarterBeat->addMouseListener(this, false);	
    
    
    //transport button
	addAndMakeVisible(transportButton = new AlphaTextButton());
    transportButton->setButtonText("START");
    transportButton->setClickingTogglesState(true);
    transportButton->setToggleState(false, false);
    transportButton->addListener(this);
    transportButton->addMouseListener(this, true);
	
	
     /*
     //auto-start switch
     addAndMakeVisible(autoStartSwitch = new GuiSwitch());
     autoStartSwitch->addListener(this);
     autoStartSwitch->setClickingTogglesState(true);
     autoStartSwitch->setToggleState(false, false);
     autoStartSwitch->addMouseListener(this, true);
     */
    
	currentBeatNumber = 0;
	currentBeatStore = 0;
	
	countGap = (4 * (M_PI / 180));
	countSeg = 0;
	segStart = 0;
	segEnd = 0;
    
    
    
}

GuiGlobalClock::~GuiGlobalClock()
{
    deleteAllChildren();
    //delete animator;
    
    //detach this class from the subject class
    mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->detach(this);
}

void GuiGlobalClock::resized()
{
    
    transportButton->setBounds(644-OFFSET_X, 42, 56, 56);
    
    tempoSlider->setBounds(550-OFFSET_X, 12, 50, 50);
	
	twoFour->setBounds(631-OFFSET_X, 111, 17, 17);
	threeFour->setBounds(659-OFFSET_X, 121, 17, 17);
	fourFour->setBounds(689-OFFSET_X, 115, 17, 17);
	fiveFour->setBounds(712-OFFSET_X, 95, 17, 17);
	sixFour->setBounds(722-OFFSET_X, 67, 17, 17);
	sevenFour->setBounds(716-OFFSET_X, 37, 17, 17);
	
	fourBar->setBounds(606-OFFSET_X, 6, 131, 131);
	twoBar->setBounds(606-OFFSET_X, 6, 131, 131);
	oneBar->setBounds(606-OFFSET_X, 6, 131, 131);
	oneBeat->setBounds(606-OFFSET_X, 6, 131, 131);
	halfBeat->setBounds(606-OFFSET_X, 6, 131, 131);
	quarterBeat->setBounds(606-OFFSET_X, 6, 131, 131);
    
    
}

void GuiGlobalClock::paint (Graphics &g)
{
	
    quantiseBg.clear(); // <- without this, the CPU level slowly increases.
	barsBg.clear();
    
	quantiseBg.addPieSegment(603-OFFSET_X, 3, 137, 137, (235.5 * (M_PI / 180)), (404 * (M_PI / 180)), 0.5f);
	
	g.setColour(Colours::black);
	g.fillPath(quantiseBg, getTransform());
	
	g.setColour(Colours::grey.withAlpha(0.2f));
	g.fillEllipse(634-OFFSET_X, 32, 76, 76);
	
	barsBg.addPieSegment(626-OFFSET_X, 24, 93, 93, 0, (90 * (M_PI / 180)), 0.5f);
	barsBg.addPieSegment(626-OFFSET_X, 24, 93, 93, (180 * (M_PI / 180)), (270 * (M_PI / 180)), 0.5f);
	g.setColour(Colours::grey.withAlpha(0.2f));
	g.fillPath(barsBg, getTransform());
	
	g.setColour(Colours::black);
	g.fillEllipse(639-OFFSET_X, 37, 66, 66);
	
	countSeg = ((M_PI/2) - (countGap * AppSettings::Instance()->getBeatsPerBar())) / AppSettings::Instance()->getBeatsPerBar();
	
	
	if (currentBeatNumber == 1) 
    {
		barCount.clear();
		
		segStart = countGap * 0.5;
		segEnd = segStart + countSeg;
		
		barCount.addPieSegment(627-OFFSET_X, 26, 90, 90, segStart, segEnd, 0.9f);
		
	}
	
	
	if (currentBeatNumber != currentBeatStore && currentBeatNumber != 1)
	{
		
		segStart = segEnd + countGap;
		segEnd = segStart + countSeg;
		
		barCount.addPieSegment(627-OFFSET_X, 26, 90, 90, segStart, segEnd, 0.9f);
        
		currentBeatStore = currentBeatNumber;	
		
	}	
	
	g.setColour(AlphaColours::blue);
	g.fillPath(barCount, getTransform());
    
	
}


bool GuiGlobalClock::update(const Subject& theChangedSubject)
{
    if (&theChangedSubject == mainComponentRef.getAlphaLiveEngineRef().getGlobalClock())
    {
        //if beat indicator needs updating
        if (mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->getGuiUpdateFlag() == 1)
        {
            //update the beat label number
            //beatNumberLabel->setText(String(mainComponentRef.getOscRoutingRef().getGlobalClock()->getBeatNumber()), true);
            int beat = mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->getBeatNumber();
            //update the bar label number
            //barNumberLabel->setText(String(mainComponentRef.getOscRoutingRef().getGlobalClock()->getBarNumber()), true);
            int bar = mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->getBarNumber();
			
			currentBeatNumber = (bar * beat) + (beat -1);
			
            repaint(606-OFFSET_X, 6, 131, 131);
			
			
        }
        //if transport button needs updating
        else if (mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->getGuiUpdateFlag() == 2)
        {
            transportButton->setToggleState(true, false);
            transportButton->setButtonText("STOP");
        }
        
        
    }
	
    
    return true;
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
	
	if (button == twoFour) {
		
		AppSettings::Instance()->setBeatsPerBar(2);
		
	}
	
	if (button == threeFour) {
		
		AppSettings::Instance()->setBeatsPerBar(3);
		
	}
	
	if (button == fourFour) {
		
		AppSettings::Instance()->setBeatsPerBar(4);
		
	}
	
	if (button == fiveFour) {
		
		AppSettings::Instance()->setBeatsPerBar(5);
		
	}
	
	if (button == sixFour) {
		
		AppSettings::Instance()->setBeatsPerBar(6);
		
	}
	
	if (button == sevenFour) {
		
		AppSettings::Instance()->setBeatsPerBar(7);
		
	}
	
	if (button == fourBar)
	{
		AppSettings::Instance()->setQuantizationValue(1);
	}
	if (button == twoBar)
	{
		AppSettings::Instance()->setQuantizationValue(2);
	}
	if (button == oneBar)
	{
		AppSettings::Instance()->setQuantizationValue(3);
	}
	if (button == oneBeat)
	{
		AppSettings::Instance()->setQuantizationValue(5);
	}
	if (button == halfBeat)
	{
		AppSettings::Instance()->setQuantizationValue(6);
	}
	if (button == quarterBeat)
	{
		AppSettings::Instance()->setQuantizationValue(7);
	}
	
    if (button == transportButton)
    {
        if (transportButton->getToggleState() == true)
        {
            alphaLiveEngineRef.getGlobalClock()->startClock();
            transportButton->setButtonText("STOP");
            mainComponentRef.setIsClockRunning(true);
        }
        else
        {
            alphaLiveEngineRef.getGlobalClock()->stopClock();
            transportButton->setButtonText("START");
            mainComponentRef.setIsClockRunning(false);
        }
    }
    
    /*
     else if (button == autoStartSwitch)
     {
     if (autoStartSwitch->getToggleState() == true)
     {
     AppSettings::Instance()->setAutoStartClock(1);
     }
     else
     AppSettings::Instance()->setAutoStartClock(0);
     
     }
     */
}

void GuiGlobalClock::comboBoxChanged(ComboBox *comboBox)
{
    
}

void GuiGlobalClock::toggleTransportButtonOff()
{
    if (transportButton->getToggleState() == true)
    {
        transportButton->setToggleState(false, false);
        transportButton->setButtonText("START");
        mainComponentRef.setIsClockRunning(false);
    }
    
}

void GuiGlobalClock::triggerTransportButton()
{
    transportButton->triggerClick();
}

void GuiGlobalClock::updateDisplay()
{
    tempoSlider->setComponentValue(AppSettings::Instance()->getGlobalTempo());
	
	switch (AppSettings::Instance()->getBeatsPerBar()) {
		case 2:
            twoFour->setToggleState(true, false);
			break;
		case 3:
            threeFour->setToggleState(true, false);
			break;
		case 4:
            fourFour->setToggleState(true, false);
			break;
		case 5:
            fiveFour->setToggleState(true, false);
			break;
		case 6:
            sixFour->setToggleState(true, false);
			break;
		case 7:
            sevenFour->setToggleState(true, false);
			break;
		default:
			break;
	}
	
	switch (AppSettings::Instance()->getQuantizationValue()) 
    {
		case 1:
            fourBar->setToggleState(true, false);
			break;
		case 2:
            twoBar->setToggleState(true, false);
			break;
		case 3:
            oneBar->setToggleState(true, false);
			break;
		case 5:
            oneBeat->setToggleState(true, false);
			break;
		case 6:
            halfBeat->setToggleState(true, false);
			break;
		case 7:
            quarterBeat->setToggleState(true, false);
			break;
		default:
			break;
	}
}

void GuiGlobalClock::mouseEnter (const MouseEvent &e)
{
    if (tempoSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText ("Global Tempo Selector. Sets and displays the tempo which controls the playback speed of the global clock and sequences.");
    }
    /* else if (beatsPerBarSlider->isMouseOver(true))
     {
     mainComponentRef.setInfoTextBoxText ("Beats-Per-Bar Selector. This is similar to a time signature option however it does not take into account note value/duration. Therefore this changes the upper numeral of the time signature but the bottom numeral is always set to 4.");
     }*/
    /*
     else if (quantisationMenu->isMouseOver(true))
     {
     mainComponentRef.setInfoTextBoxText ("Quantisation Menu. Sets and displays the quantisation value which is used to determine the start time of loops and sequences when set to Quantisation Trigger Mode.");
     }
     else if (beatIndicator->isMouseOver(true))
     {
     mainComponentRef.setInfoTextBoxText ("Beat Indicator. A visual metronome for the global clock.");
     }
     else if (barNumberLabel->isMouseOver(true))
     {
     mainComponentRef.setInfoTextBoxText ("Bar Number Display. Displays the currently bar number of the global clock  in the range of 1-4.");
     }
     else if (beatNumberLabel->isMouseOver(true))
     {
     mainComponentRef.setInfoTextBoxText ("Beat Number Display. Displays the currently beat number of the global clock in the range of 1 to the number of beats per bar.");
     }
     */
    else if (transportButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText ("Clock Start/Stop Button. Use this button to manually start and stop the global clock.");
    }
    /*
     else if (autoStartSwitch->isMouseOver(true))
     {
     mainComponentRef.setInfoTextBoxText ("Clock Auto-Start Switch. If set to true the clock will automatically start the first time a loop or sequence is played, else the clock will need to be manually started using the Start/Stop button.");
     }
     */
}

void GuiGlobalClock::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}
