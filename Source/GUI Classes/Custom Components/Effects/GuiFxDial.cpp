/*
 *  GuiFxDial.cpp
 *  sdaJuce
 *
 *  Created by Sam Davies on 19/10/2011.
 *  Copyright 2011 __MyCompanyName__.
 *
 */

#include "GuiFxDial.h"
#include "../../Binary Data/PressureBinaryData.h"
#include "../../../File and Settings/AppSettings.h"
#include "../../Views/MainComponent.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNum]
#define SINGLE_PAD (selectedPads.size() == 1)
#define MULTI_PADS (selectedPads.size() > 1)



GuiFxDial::GuiFxDial(MainComponent &ref) :  Component ("GuiFxDial"),
                                            mainComponentRef(ref)

{
	
    addAndMakeVisible(gainAndPan = new GuiGainAndPan(mainComponentRef));
    addChildComponent(lowpassFilter = new GuiLowpassFilter(mainComponentRef));
    addChildComponent(highPassFilter = new GuiHighPassFilter(mainComponentRef));
    addChildComponent(bandPassFilter = new GuiBandPassFilter(mainComponentRef));
    addChildComponent(delay = new GuiDelay(mainComponentRef));
    addChildComponent(reverb = new GuiReverb(mainComponentRef));
    addChildComponent(flanger = new GuiFlanger(mainComponentRef));
    addChildComponent(tremolo = new GuiTremolo(mainComponentRef));
    
    
    //----------------------FX buttons------------------
    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
            fxButtons.insert(i, new SettingsButton("GAIN", (27 * (M_PI / 180)), (54 * (M_PI / 180)), 0.75f, -5, 0.3, 0.3));
        else if (i == 1)
            fxButtons.insert(i, new SettingsButton("LPF", (54 * (M_PI / 180)), (81 * (M_PI / 180)), 0.75f, -5, 0.38, 0.3));
        else if (i == 2)
            fxButtons.insert(i, new SettingsButton("HPF", (81 * (M_PI / 180)), (108 * (M_PI / 180)), 0.75f, -5, 0.35, 0.3));
        else if (i == 3)
            fxButtons.insert(i, new SettingsButton("BPF", (108 * (M_PI / 180)), (135 * (M_PI / 180)), 0.75f, 185, 0.6, 0.37));
        else if (i == 4)
            fxButtons.insert(i, new SettingsButton("DRIVE", (135 * (M_PI / 180)), (162 * (M_PI / 180)), 0.75f, 185, 0.7, 0.37));
        else if (i == 5)
            fxButtons.insert(i, new SettingsButton("CRUSH", (162 * (M_PI / 180)), (189 * (M_PI / 180)), 0.75f,  185, 0.73, 0.37));
        else if (i == 6)
            fxButtons.insert(i, new SettingsButton("DELAY", (189 * (M_PI / 180)), (216 * (M_PI / 180)), 0.75f, 185, 0.7, 0.37));
        else if (i == 7)
            fxButtons.insert(i, new SettingsButton("REVERB", (216 * (M_PI / 180)), (243 * (M_PI / 180)), 0.75f, 188, 0.75, 0.37));
        else if (i == 8)
            fxButtons.insert(i, new SettingsButton("FLANGER", (243 * (M_PI / 180)), (270 * (M_PI / 180)), 0.75f, 188, 0.75, 0.37));
        else if (i == 9)
            fxButtons.insert(i, new SettingsButton("TREMOLO", (270 * (M_PI / 180)), (297 * (M_PI / 180)), 0.75f, 190, 0.85, 0.37));
        
        fxButtons[i]->addListener(this);
        fxButtons[i]->setOpaque(false);
        fxButtons[i]->setRadioGroupId (43);
        fxButtons[i]->addMouseListener(this, false);
        addAndMakeVisible(fxButtons[i]);
                             
    }
    
    fxButtons[0]->setToggleState(true, false);
    

    //TEMPORARY
    fxButtons[4]->setEnabled(false);
    fxButtons[4]->setAlpha(0.6f);
    fxButtons[5]->setEnabled(false);
    fxButtons[5]->setAlpha(0.6f);

   
}

GuiFxDial::~GuiFxDial()
{
    fxButtons.clear();
    
    delete gainAndPan;
    delete lowpassFilter;
    delete highPassFilter;
    delete bandPassFilter;
    delete delay;
    delete reverb;
    delete flanger;
    delete tremolo;
}


void GuiFxDial::resized()
{
	hitPath.addEllipse(0.0f, 0.0f, getWidth(), getHeight());
    
    gainAndPan->setBounds(0, 0, getWidth(), getHeight());
    lowpassFilter->setBounds(0, 0, getWidth(), getHeight());
    highPassFilter->setBounds(0, 0, getWidth(), getHeight());
    bandPassFilter->setBounds(0, 0, getWidth(), getHeight());
    delay->setBounds(0, 0, getWidth(), getHeight());
    reverb->setBounds(0, 0, getWidth(), getHeight());
    flanger->setBounds(0, 0, getWidth(), getHeight());
    tremolo->setBounds(0, 0, getWidth(), getHeight());
    
    //can we give the below more specific bounds?
    //if not the below can be put into a for loop
    fxButtons[0]->setBounds(0, 0, getWidth(), getHeight());
	fxButtons[1]->setBounds(0, 0, getWidth(), getHeight());
	fxButtons[2]->setBounds(0, 0, getWidth(), getHeight());
	fxButtons[3]->setBounds(0, 0, getWidth(), getHeight());
	fxButtons[4]->setBounds(0, 0, getWidth(), getHeight());
	fxButtons[5]->setBounds(0, 0, getWidth(), getHeight());
	fxButtons[6]->setBounds(0, 0, getWidth(), getHeight());
	fxButtons[7]->setBounds(0, 0, getWidth(), getHeight());
	fxButtons[8]->setBounds(0, 0, getWidth(), getHeight());
	fxButtons[9]->setBounds(0, 0, getWidth(), getHeight());


}

void GuiFxDial::paint (Graphics& g)
{
	g.setColour(Colours::black);
	g.fillEllipse(208,208, 38, 38);
	
	Path pieSeg;
	pieSeg.addPieSegment(119, 119, 86, 86, (125 * (M_PI / 180)), (235 * (M_PI / 180)), 0.2f);
	g.fillPath(pieSeg);
	
	g.setColour(Colours::grey.withAlpha(0.3f));
	g.drawEllipse(208,208, 38, 38, 1.0f);
	
}


void GuiFxDial::buttonClicked(Button *button)
{
    hideAllFx();
    
	for (int fx = 0; fx < 10; fx++)
    {
        if (button == fxButtons[fx])
			{
                for (int i = 0; i < selectedPads.size(); i++)
                {
                    int padNum = selectedPads[i];
                    PAD_SETTINGS->setSamplerEffect(fx);
                }
                
                //repaint(); // repaint with bounds!! what is ths repaint? do we need it anymore?
			}
	}
    
    if (button == fxButtons[0]) //Gain and Pan
    {
        gainAndPan->updateDisplay();
        gainAndPan->setVisible(true);
    }
    else if (button == fxButtons[1]) //LPF
    {
        lowpassFilter->updateDisplay();
        lowpassFilter->setVisible(true);
    }
    else if (button == fxButtons[2]) //HPF
    {
        highPassFilter->updateDisplay();
        highPassFilter->setVisible(true);
    }
    else if (button == fxButtons[3]) //BPF
    {
        bandPassFilter->updateDisplay();
        bandPassFilter->setVisible(true);
    }
    else if (button == fxButtons[4]) //Overdrive
    {
        
    }
    else if (button == fxButtons[5]) //Bitcrusher
    {
        
    }
    else if (button == fxButtons[6]) //Delay
    {
        delay->updateDisplay();
        delay->setVisible(true);
    }
    else if (button == fxButtons[7]) //Reverb
    {
        reverb->updateDisplay();
        reverb->setVisible(true);
    }
    else if (button == fxButtons[8]) //Flanger
    {
        flanger->updateDisplay();
        flanger->setVisible(true);
    }
    else if (button == fxButtons[9]) //Tremolo
    {
        tremolo->updateDisplay();
        tremolo->setVisible(true);
    }
}


void GuiFxDial::sliderValueChanged (Slider *slider)
{
    
}

bool GuiFxDial::hitTest (int x, int y)
{
	return hitPath.contains(x, y);
}


void GuiFxDial::hideAllFx()
{
    //all Fx Guis should be hiden here
    gainAndPan->setVisible(false);
    lowpassFilter->setVisible(false);
    highPassFilter->setVisible(false);
    bandPassFilter->setVisible(false);
    delay->setVisible(false);
    reverb->setVisible(false);
    flanger->setVisible(false);
    tremolo->setVisible(false);
}

void GuiFxDial::setCurrentlySelectedPad (Array<int> selectedPads_)
{
    selectedPads = selectedPads_;
    gainAndPan->setCurrentlySelectedPad(selectedPads);
    lowpassFilter->setCurrentlySelectedPad(selectedPads);
    highPassFilter->setCurrentlySelectedPad(selectedPads);
    bandPassFilter->setCurrentlySelectedPad(selectedPads);
    delay->setCurrentlySelectedPad(selectedPads);
    reverb->setCurrentlySelectedPad(selectedPads);
    flanger->setCurrentlySelectedPad(selectedPads);
    tremolo->setCurrentlySelectedPad(selectedPads);
}

void GuiFxDial::updateDisplay()
{
    //This method is used to set all the components to the currently selected pad's settings,
    //as well as show and hide the relavent components
    
    hideAllFx();
    
    int currentEffect;
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        currentEffect = PAD_SETTINGS->getSamplerEffect();
        
        if (currentEffect == 1) //Gain and Pan
        {
            gainAndPan->updateDisplay();
            gainAndPan->setVisible(true);
        }
        else if (currentEffect == 2) //LPF
        {
            lowpassFilter->updateDisplay();
            lowpassFilter->setVisible(true);
        }
        else if (currentEffect == 3) //HPF
        {
            highPassFilter->updateDisplay();
            highPassFilter->setVisible(true);
        }
        else if (currentEffect == 4) //BPF
        {
            bandPassFilter->updateDisplay();
            bandPassFilter->setVisible(true);
        }
        else if (currentEffect == 6) //Delay
        {
            delay->updateDisplay();
            delay->setVisible(true);
        }
        else if (currentEffect == 7) //Reverb
        {
            reverb->updateDisplay();
            reverb->setVisible(true);
        }
        else if (currentEffect == 9) //Flanger
        {
            flanger->updateDisplay();
            flanger->setVisible(true);
        }
        else if (currentEffect == 10) //Tremolo
        {
            tremolo->updateDisplay();
            tremolo->setVisible(true);
        }
        
    }

    else if(MULTI_PADS)
    {
        currentEffect = 0;
    }
    
    
    fxButtons[currentEffect]->setToggleState(true, false);
    
    //repaint(); //repaint with bounds! Need to call repaint here?

}

void GuiFxDial::mouseEnter (const MouseEvent &e)
{
    /*
    if (buttons[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("No Effect. The pressure of the selected pad/pads will not control any effect.");
    }
    else if (buttons[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Gain and Pan. Allows the pressure of the selected pad/pads to manipulate the gain and pan of the audio.");
    }
    else if (buttons[2]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Low-Pass Filter. Allows the pressure of the selected pad/pads to manipulate audio with a Low-Pass Filter.");
    }
    else if (buttons[3]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("High-Pass Filter. Allows the pressure of the selected pad/pads to manipulate audio with a High-Pass Filter.");
    }
    else if (buttons[4]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Band-Pass Filter. Allows the pressure of the selected pad/pads to manipulate audio with a Band-Pass Filter.");
    }
    else if (buttons[5]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Overdrive Distortion. Allows the pressure of the selected pad/pads to manipulate audio with a overdrive distortion effect. Coming Soon!");
    }
    else if (buttons[6]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Delay. Allows the pressure of the selected pad/pads to manipulate audio with a simple delay effect.");
    }
    else if (buttons[7]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Reverb. Allows the pressure of the selected pad/pads to manipulate audio with a simple reverb effect.");
    }
    else if (buttons[8]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Chorus. Allows the pressure of the selected pad/pads to manipulate audio with a chorus effect. Coming Soon!");
    }
    else if (buttons[9]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Flanger. Allows the pressure of the selected pad/pads to manipulate audio with a flanger effect.");
    }
    else if (buttons[10]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Tremolo. Allows the pressure of the selected pad/pads to manipulate audio with a tremolo effect.");
    }
    else if (buttons[11]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Vibrato. Allows the pressure of the selected pad/pads to manipulate audio with a vibrato effect. Coming Soon!");
    }
    else if (buttons[12]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Pitchshifter. Allows the pressure of the selected pad/pads to manipulate audio with a pitchshifter. Coming Soon!");
    }
     */
}

void GuiFxDial::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty); 
}