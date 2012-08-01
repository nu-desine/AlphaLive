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
	image = ImageFileFormat::loadFrom(PressureBinaryData::circle_png, PressureBinaryData::circle_pngSize);
	
	im = ImageFileFormat::loadFrom(PressureBinaryData::b1bg_png, PressureBinaryData::b1bg_pngSize);
	
	keyOffIm.insert(0, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b1off_png, PressureBinaryData::b1off_pngSize)));
	keyOffIm.insert(1, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b2off_png, PressureBinaryData::b2off_pngSize)));
	keyOffIm.insert(2, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b3off_png, PressureBinaryData::b3off_pngSize)));
	keyOffIm.insert(3, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b4off_png, PressureBinaryData::b4off_pngSize)));
	keyOffIm.insert(4, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b5off_png, PressureBinaryData::b5off_pngSize)));
	keyOffIm.insert(5, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b6off_png, PressureBinaryData::b6off_pngSize)));
	keyOffIm.insert(6, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b7off_png, PressureBinaryData::b7off_pngSize)));
	keyOffIm.insert(7, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b8off_png, PressureBinaryData::b8off_pngSize)));
	keyOffIm.insert(8, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b9off_png, PressureBinaryData::b9off_pngSize)));
	keyOffIm.insert(9, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b10off_png, PressureBinaryData::b10off_pngSize)));
	keyOffIm.insert(10, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b11off_png, PressureBinaryData::b11off_pngSize)));
	keyOffIm.insert(11, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b12off_png, PressureBinaryData::b12off_pngSize)));
	keyOffIm.insert(12, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b13off_png, PressureBinaryData::b13off_pngSize)));
    keyOverIm.insert(0, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b1over_png, PressureBinaryData::b1over_pngSize)));
	keyOverIm.insert(1, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b2over_png, PressureBinaryData::b2over_pngSize)));
	keyOverIm.insert(2, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b3over_png, PressureBinaryData::b3over_pngSize)));
	keyOverIm.insert(3, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b4over_png, PressureBinaryData::b4over_pngSize)));
	keyOverIm.insert(4, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b5over_png, PressureBinaryData::b5over_pngSize)));
	keyOverIm.insert(5, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b6over_png, PressureBinaryData::b6over_pngSize)));
	keyOverIm.insert(6, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b7over_png, PressureBinaryData::b7over_pngSize)));
	keyOverIm.insert(7, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b8over_png, PressureBinaryData::b8over_pngSize)));
	keyOverIm.insert(8, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b9over_png, PressureBinaryData::b9over_pngSize)));
	keyOverIm.insert(9, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b10over_png, PressureBinaryData::b10over_pngSize)));
    keyOverIm.insert(10, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b11over_png, PressureBinaryData::b11over_pngSize)));
	keyOverIm.insert(11, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b12over_png, PressureBinaryData::b12over_pngSize)));
	keyOverIm.insert(12, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b13over_png, PressureBinaryData::b13over_pngSize)));
	keyDownIm.insert(0, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b1down_png, PressureBinaryData::b1down_pngSize)));
	keyDownIm.insert(1, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b2down_png, PressureBinaryData::b2down_pngSize)));
	keyDownIm.insert(2, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b3down_png, PressureBinaryData::b3down_pngSize)));
	keyDownIm.insert(3, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b4down_png, PressureBinaryData::b4down_pngSize)));
	keyDownIm.insert(4, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b5down_png, PressureBinaryData::b5down_pngSize)));
	keyDownIm.insert(5, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b6down_png, PressureBinaryData::b6down_pngSize)));
	keyDownIm.insert(6, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b7down_png, PressureBinaryData::b7down_pngSize)));
	keyDownIm.insert(7, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b8down_png, PressureBinaryData::b8down_pngSize)));
	keyDownIm.insert(8, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b9down_png, PressureBinaryData::b9down_pngSize)));
	keyDownIm.insert(9, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b10down_png, PressureBinaryData::b10down_pngSize)));
	keyDownIm.insert(10, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b11down_png, PressureBinaryData::b11down_pngSize)));
	keyDownIm.insert(11, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b12down_png, PressureBinaryData::b12down_pngSize)));
	keyDownIm.insert(12, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b13down_png, PressureBinaryData::b13down_pngSize)));
	bgIm.insert(0, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b1bg_png, PressureBinaryData::b1bg_pngSize)));
	bgIm.insert(1, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b2bg_png, PressureBinaryData::b2bg_pngSize)));
	bgIm.insert(2, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b3bg_png, PressureBinaryData::b3bg_pngSize)));
	bgIm.insert(3, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b4bg_png, PressureBinaryData::b4bg_pngSize)));
	bgIm.insert(4, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b5bg_png, PressureBinaryData::b5bg_pngSize)));
	bgIm.insert(5, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b6bg_png, PressureBinaryData::b6bg_pngSize)));
	bgIm.insert(6, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b7bg_png, PressureBinaryData::b7bg_pngSize)));
	bgIm.insert(7, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b8bg_png, PressureBinaryData::b8bg_pngSize)));
	bgIm.insert(8, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b9bg_png, PressureBinaryData::b9bg_pngSize)));
	bgIm.insert(9, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b10bg_png, PressureBinaryData::b10bg_pngSize)));
	bgIm.insert(10, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b11bg_png, PressureBinaryData::b11bg_pngSize)));
	bgIm.insert(11, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b12bg_png, PressureBinaryData::b12bg_pngSize)));
	bgIm.insert(12, new Image(ImageFileFormat::loadFrom(PressureBinaryData::b13bg_png, PressureBinaryData::b13bg_pngSize)));
	
	for (int i = 0; i <= 12; i++) 
    {
		buttons.insert(i, new GuiFxButtons(keyOffIm[i], keyOverIm[i], keyDownIm[i]));
		addAndMakeVisible(buttons[i]);
		buttons[i]->setRadioGroupId (800);
		buttons[i]->addListener(this);
        buttons[i]->addMouseListener(this, true);
		buttons[i]->setOpaque(false);
		
		//if (i == 0)
		//{
            //do we actually need to do anything here???
			//buttons[i]->triggerClick();
		//}
        
	}
    
    //TEMPORARY -DONT FORGET TO CHANGE THIS!
    buttons[5]->setEnabled(false);
    buttons[5]->setAlpha(0.4f);
    buttons[8]->setEnabled(false);
    buttons[8]->setAlpha(0.4f);
    buttons[11]->setEnabled(false);
    buttons[11]->setAlpha(0.4f);
    buttons[12]->setEnabled(false);
    buttons[12]->setAlpha(0.4f);
    
	
	hitPath.addEllipse (0.0f, 0.0f, 230.0f, 230.0f);
    
    addAndMakeVisible(gainAndPan = new GuiGainAndPan(mainComponentRef));
    addAndMakeVisible(lowpassFilter = new GuiLowpassFilter(mainComponentRef));
    addAndMakeVisible(highPassFilter = new GuiHighPassFilter(mainComponentRef));
    addAndMakeVisible(bandPassFilter = new GuiBandPassFilter(mainComponentRef));
    addAndMakeVisible(delay = new GuiDelay(mainComponentRef));
    addAndMakeVisible(reverb = new GuiReverb(mainComponentRef));
    addAndMakeVisible(flanger = new GuiFlanger(mainComponentRef));
    addAndMakeVisible(tremolo = new GuiTremolo(mainComponentRef));
     
    //hide all fx Gui's to start with...
    hideAllFx();
    
    //currentlySelectedPad = 99;

    
}

GuiFxDial::~GuiFxDial()
{
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

}

void GuiFxDial::paint (Graphics& g)
{
	g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 0, 800, 800);
	g.drawImage(im, getWidth()*0.05, getHeight()*0.05, getWidth()*0.9, getHeight()*0.9, 0, 0, 230, 230);
	
	for (int i = 0; i <= 12; i++) 
    {
		buttons[i]->setBounds(0.0f, 0.0f, getWidth(), getHeight());
	}
	
}

void GuiFxDial::buttonClicked(Button *button)
{
    hideAllFx();
    
	for (int fx = 0; fx <=12; fx++)
    {
        if (button == buttons[fx])
			{
                for (int i = 0; i < selectedPads.size(); i++)
                {
                    int padNum = selectedPads[i];
                    PAD_SETTINGS->setSamplerEffect(fx);
                }
                
				im = *bgIm[fx];
                repaint(); // repaint with bounds!!
			}
	}
    
    if (button == buttons[1]) //Gain and Pan
    {
        gainAndPan->updateDisplay();
        gainAndPan->setVisible(true);
    }
    else if (button == buttons[2]) //LPF
    {
        lowpassFilter->updateDisplay();
        lowpassFilter->setVisible(true);
    }
    else if (button == buttons[3]) //HPF
    {
        highPassFilter->updateDisplay();
        highPassFilter->setVisible(true);
    }
    else if (button == buttons[4]) //BPF
    {
        bandPassFilter->updateDisplay();
        bandPassFilter->setVisible(true);
    }
    else if (button == buttons[6]) //Delay
    {
        delay->updateDisplay();
        delay->setVisible(true);
    }
    else if (button == buttons[7]) //Reverb
    {
        reverb->updateDisplay();
        reverb->setVisible(true);
    }
    else if (button == buttons[9]) //Flanger
    {
        flanger->updateDisplay();
        flanger->setVisible(true);
    }
    else if (button == buttons[10]) //Tremolo
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
        currentEffect = 0;
    
    //update button state and background image
    buttons[currentEffect]->setToggleState(true, false);
    im = *bgIm[currentEffect];
    repaint(); //repaint with bounds!

}

void GuiFxDial::mouseEnter (const MouseEvent &e)
{
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
}

void GuiFxDial::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty); 
}