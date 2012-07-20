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


GuiGlobalClock::GuiGlobalClock(MainComponent &ref, AlphaLiveEngine &ref2)
                                        :   mainComponentRef(ref),
                                            alphaLiveEngineRef(ref2)
{
    mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->attach(this);
    animator = new ComponentAnimator();
    
    //tempo slider (IncDecButtons)
    addAndMakeVisible(tempoSlider = new AlphaSlider());
    tempoSlider->setRange(60.0, 360.0, 0.1);
    tempoSlider->addListener(this);
    tempoSlider->setTextBoxStyle(Slider::TextBoxAbove, false, 45, 15);
    tempoSlider->setValue(120.0, false);
    tempoSlider->addMouseListener(this, true);
    
    //beats per bar slider
    addAndMakeVisible(beatsPerBarSlider = new AlphaSlider());
    beatsPerBarSlider->setRange(2, 7, 1);
    beatsPerBarSlider->addListener(this);
    beatsPerBarSlider->setTextBoxStyle(Slider::TextBoxAbove, false, 45, 15);
    beatsPerBarSlider->setValue(4, false);
    beatsPerBarSlider->addMouseListener(this, true);
    
    //quantization menu
    addAndMakeVisible(quantizationMenu = new ComboBox());
    quantizationMenu->addItem("4 Bars", 1);
    quantizationMenu->addItem("2 Bars", 2);
    quantizationMenu->addItem("1 Bar", 3);
    //quantizationMenu->addItem("2 Beats", 4); //this wouldn't work with time signatures other than 4/4
    quantizationMenu->addItem("1 Beat", 5);
    quantizationMenu->addItem("1/2 Beat", 6);
    quantizationMenu->addItem("1/4 Beat", 7);
    quantizationMenu->setSelectedId(3, true);
    quantizationMenu->addListener(this);
    quantizationMenu->addMouseListener(this, true);
    
    //transport button
    addAndMakeVisible(transportButton = new TextButton());
    transportButton->setButtonText("Start");
    transportButton->setClickingTogglesState(true);
    transportButton->setToggleState(false, false);
    transportButton->addListener(this);
    transportButton->addMouseListener(this, true);
    
    //beat indicator
    addAndMakeVisible(beatIndicator = new GuiBeatIndicator());
    beatIndicator->addMouseListener(this, true);
    beatIndicator->setAlpha(0.1);
    
    //bar number label
    addAndMakeVisible(barNumberLabel = new Label());
    barNumberLabel->setText("1", false);
    barNumberLabel->setJustificationType(Justification::centred);
    barNumberLabel->addMouseListener(this, true);
    //barNumberLabel->setColour(Label::backgroundColourId, Colours::grey);
    barNumberLabel->addMouseListener(this, true);
    Font barFont("Arial", 24, Font::plain);
    barNumberLabel->setFont(barFont);
    //beat number label
    addAndMakeVisible(beatNumberLabel = new Label());
    beatNumberLabel->setText("0", false);
    beatNumberLabel->setJustificationType(Justification::centred);
    //beatNumberLabel->setColour(Label::backgroundColourId, Colours::grey);
    beatNumberLabel->addMouseListener(this, true);
    Font beatFont("Arial", 24, Font::bold);
    beatNumberLabel->setFont(beatFont);
    
    //auto-start switch
    addAndMakeVisible(autoStartSwitch = new GuiSwitch());
    autoStartSwitch->addListener(this);
    autoStartSwitch->setClickingTogglesState(true);
    autoStartSwitch->setToggleState(false, false);
    autoStartSwitch->addMouseListener(this, true);
    
                      
    
}

GuiGlobalClock::~GuiGlobalClock()
{
    deleteAllChildren();
    delete animator;
    
    //detach this class from the subject class
    mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->detach(this);
}

void GuiGlobalClock::resized()
{
    
    /*
    // OLD LAYOUT FOR TOP BAR
    tempoSlider->setBounds(10, 2, 45, 35);
    beatsPerBarSlider->setBounds(60, 2, 45, 35);
    quantizationMenu->setBounds(110, 7, 70, 20);
    
    
    beatIndicator->setBounds(getWidth()-165, 5, 25, 25);
    barNumberLabel->setBounds(getWidth()-135, 5, 20, 25);
    beatNumberLabel->setBounds(getWidth()-110, 5, 20, 25);
    transportButton->setBounds(getWidth()-85, 7, 50, 20);
    autoStartSwitch->setBounds(getWidth()-30, 1, 30, 30);
     */
    
    //NEW LAYOUT FOR LEFT SIDE
    transportButton->setBounds(5+2, 7, 50, 20);
    autoStartSwitch->setBounds(60+2, 3, 30, 30);
    barNumberLabel->setBounds(95+2, 5, 20, 25);
    beatNumberLabel->setBounds(120+2, 5, 20, 25);
    beatIndicator->setBounds(145+2, 5, 25, 25);
    
    tempoSlider->setBounds(5, 35+2, 45, 35);
    beatsPerBarSlider->setBounds(55, 35+2, 45, 35);
    quantizationMenu->setBounds(105, 35+7, 70, 20);
    
}

void GuiGlobalClock::paint (Graphics &g)
{
    //std::cout << "painting clock... ";
    g.setColour(Colours::darkgrey.withAlpha(0.5f));
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 10);
    
    //create a rounded shape for the bar and beat number labels
    g.setColour(Colours::grey);
    g.fillRoundedRectangle(95+2, 5, 45, 25, 10);

}

bool GuiGlobalClock::update(const Subject& theChangedSubject)
{
    if (&theChangedSubject == mainComponentRef.getAlphaLiveEngineRef().getGlobalClock())
    {
        //if beat indicator needs updating
        if (mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->getGuiUpdateFlag() == 1)
        {
            //update the beat label number
            beatNumberLabel->setText(String(mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->getBeatNumber()), true);
            
            //update the bar label number
            barNumberLabel->setText(String(mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->getBarNumber()), true);
            
            //update the beat indicator display
            //is using this animator the most CPU friendly way? What about just triggering the component on and off like
            //originally indented?
            beatIndicator->setAlpha(1.0);
            animator->animateComponent(beatIndicator, beatIndicator->getBounds(), 0.1f, 10, false, 1.0, 1.0);
        }
        //if transport button needs updating
        else if (mainComponentRef.getAlphaLiveEngineRef().getGlobalClock()->getGuiUpdateFlag() == 2)
        {
            transportButton->setToggleState(true, false);
            transportButton->setButtonText("Stop");
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
    if (slider == beatsPerBarSlider)
    {
        AppSettings::Instance()->setBeatsPerBar(beatsPerBarSlider->getValue());
    }
}

void GuiGlobalClock::buttonClicked (Button* button)
{
    if (button == transportButton)
    {
        if (transportButton->getToggleState() == true)
        {
            alphaLiveEngineRef.getGlobalClock()->startClock();
            transportButton->setButtonText("Stop");
            mainComponentRef.setIsClockRunning(true);
        }
        else
        {
            alphaLiveEngineRef.getGlobalClock()->stopClock();
            transportButton->setButtonText("Start");
            mainComponentRef.setIsClockRunning(false);
        }
    }
    
    else if (button == autoStartSwitch)
    {
        if (autoStartSwitch->getToggleState() == true)
        {
            AppSettings::Instance()->setAutoStartClock(1);
        }
        else
            AppSettings::Instance()->setAutoStartClock(0);
    }
}

void GuiGlobalClock::comboBoxChanged(ComboBox *comboBox)
{
   if (comboBox == quantizationMenu)
   {
       AppSettings::Instance()->setQuantizationValue(quantizationMenu->getSelectedId());
   }
    
}

void GuiGlobalClock::toggleTransportButtonOff()
{
    if (transportButton->getToggleState() == true)
    {
        transportButton->setToggleState(false, false);
        transportButton->setButtonText("Start");
        mainComponentRef.setIsClockRunning(false);
    }
    
}

void GuiGlobalClock::triggerTransportButton()
{
    transportButton->triggerClick();
}

void GuiGlobalClock::updateDisplay()
{
    tempoSlider->setValue(AppSettings::Instance()->getGlobalTempo(), false);
    quantizationMenu->setSelectedId(AppSettings::Instance()->getQuantizationValue(), true);
    beatsPerBarSlider->setValue(AppSettings::Instance()->getBeatsPerBar(), false);
    autoStartSwitch->setToggleState(AppSettings::Instance()->getAutoStartClock(), false);
}

void GuiGlobalClock::mouseEnter (const MouseEvent &e)
{
    if (tempoSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText ("Global Tempo Selector. Sets and displays the tempo which controls the playback speed of the global clock and sequences.");
    }
    else if (beatsPerBarSlider->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText ("Beats-Per-Bar Selector. This is similar to a time signature option however it does not take into account note value/duration. Therefore this changes the upper numeral of the time signature but the bottom numeral is always set to 4.");
    }
    else if (quantizationMenu->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText ("Quantization Menu. Sets and displays the quantization value which is used to determine the start time of loops and sequences when set to Quantization Trigger Mode.");
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
    else if (transportButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText ("Clock Start/Stop Button. Use this button to manually start and stop the global clock.");
    }
    else if (autoStartSwitch->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText ("Clock Auto-Start Switch. If set to true the clock will automatically start the first time a loop or sequence is played, else the clock will need to be manually started using the Start/Stop button.");
    }
}

void GuiGlobalClock::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}
