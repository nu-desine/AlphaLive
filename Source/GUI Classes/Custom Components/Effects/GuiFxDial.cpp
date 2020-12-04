/*
 *  GuiFxDial.cpp
 //  AlphaLive
 //
 //  Created by Liam Meredith-Lacey on 22/12/2011.
 //  Copyright 2011 nu desine.
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
 */

#include "GuiFxDial.h"
#include "../../Binary Data/MainBinaryData.h"
#include "../../../File and Settings/AppSettings.h"
#include "../../Views/MainComponent.h"
#include "../../../Application/CommonInfoBoxText.h"

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
	addChildComponent(distortion = new GuiDistortion(mainComponentRef));
	addChildComponent(bitcrusher = new GuiBitcrusher(mainComponentRef));
    
    
    //----------------------FX buttons------------------
	Image *gainIcon = new Image(ImageCache::getFromMemory(MainBinaryData::gainicon_png, MainBinaryData::gainicon_pngSize));
	Image *lpfIcon = new Image(ImageCache::getFromMemory(MainBinaryData::lpficon_png, MainBinaryData::lpficon_pngSize));
	Image *hpfIcon = new Image(ImageCache::getFromMemory(MainBinaryData::hpficon_png, MainBinaryData::hpficon_pngSize));
	Image *bpfIcon = new Image(ImageCache::getFromMemory(MainBinaryData::bpficon_png, MainBinaryData::bpficon_pngSize));
	Image *driveIcon = new Image(ImageCache::getFromMemory(MainBinaryData::overdriveicon_png, MainBinaryData::overdriveicon_pngSize));
	Image *crushIcon = new Image(ImageCache::getFromMemory(MainBinaryData::bitcrushicon_png, MainBinaryData::bitcrushicon_pngSize));
	Image *delayIcon = new Image(ImageCache::getFromMemory(MainBinaryData::delayicon_png, MainBinaryData::delayicon_pngSize));
	Image *reverbIcon = new Image(ImageCache::getFromMemory(MainBinaryData::reverbicon_png, MainBinaryData::reverbicon_pngSize));
	Image *flangerIcon = new Image(ImageCache::getFromMemory(MainBinaryData::flangericon_png, MainBinaryData::flangericon_pngSize));
	Image *tremoloIcon = new Image(ImageCache::getFromMemory(MainBinaryData::tremoloicon_png, MainBinaryData::tremoloicon_pngSize));
		
    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
            fxButtons.insert(i, new SettingsButtonImage(gainIcon, (27 * (M_PI / 180)), (54 * (M_PI / 180)), 0.75f, 1.0f));
        else if (i == 1)
            fxButtons.insert(i, new SettingsButtonImage(lpfIcon, (54 * (M_PI / 180)), (81 * (M_PI / 180)), 0.75f, 1.0f));
        else if (i == 2)
            fxButtons.insert(i, new SettingsButtonImage(hpfIcon, (81 * (M_PI / 180)), (108 * (M_PI / 180)), 0.75f, 1.0f));
        else if (i == 3)
            fxButtons.insert(i, new SettingsButtonImage(bpfIcon, (108 * (M_PI / 180)), (135 * (M_PI / 180)), 0.75f, 1.0f));
        else if (i == 4)
			fxButtons.insert(i, new SettingsButtonImage(driveIcon, (135 * (M_PI / 180)), (162 * (M_PI / 180)), 0.75f, 1.0f));
        else if (i == 5)
            fxButtons.insert(i, new SettingsButtonImage(crushIcon, (162 * (M_PI / 180)), (189 * (M_PI / 180)), 0.75f, 1.0f));
        else if (i == 6)
            fxButtons.insert(i, new SettingsButtonImage(delayIcon, (189 * (M_PI / 180)), (216 * (M_PI / 180)), 0.75f, 1.0f));
        else if (i == 7)
            fxButtons.insert(i, new SettingsButtonImage(reverbIcon, (216 * (M_PI / 180)), (243 * (M_PI / 180)), 0.75f, 1.0f));
        else if (i == 8)
            fxButtons.insert(i, new SettingsButtonImage(flangerIcon, (243 * (M_PI / 180)), (270 * (M_PI / 180)), 0.75f, 1.0f));
        else if (i == 9)
            fxButtons.insert(i, new SettingsButtonImage(tremoloIcon, (270 * (M_PI / 180)), (297 * (M_PI / 180)), 0.75f, 1.0f));
        
        fxButtons[i]->addListener(this);
        fxButtons[i]->setOpaque(false);
        fxButtons[i]->setRadioGroupId (43);
        fxButtons[i]->addMouseListener(this, false);
        addAndMakeVisible(fxButtons[i]);
                             
    }
    
    fxButtons[0]->setToggleState(true, dontSendNotification);
   
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
	delete distortion;
	delete bitcrusher;
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
	distortion->setBounds(0, 0, getWidth(), getHeight());
	bitcrusher->setBounds(0, 0, getWidth(), getHeight());
    
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
	g.setColour(AlphaTheme::getInstance()->backgroundColour);
	g.fillEllipse(208,208, 38, 38);
	
	Path pieSeg;
	pieSeg.addPieSegment(119, 119, 86, 86, (125 * (M_PI / 180)), (235 * (M_PI / 180)), 0.2f);
	g.fillPath(pieSeg);
	
	g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.3f));
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
                    
                    if (PAD_SETTINGS->getMode() == 2)
                        PAD_SETTINGS->setSamplerEffect(fx+1);
                    else if (PAD_SETTINGS->getMode() == 3)
                        PAD_SETTINGS->setSequencerEffect(fx+1);
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
    else if (button == fxButtons[4]) //Distortion
    {
        distortion->updateDisplay();
		distortion->setVisible(true);
    }
    else if (button == fxButtons[5]) //Bitcrusher
    {
		bitcrusher->updateDisplay();
		bitcrusher->setVisible(true);
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
	distortion->setVisible(false);
	bitcrusher->setVisible(false);
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
	distortion->setCurrentlySelectedPad(selectedPads);
	bitcrusher->setCurrentlySelectedPad(selectedPads);
}

void GuiFxDial::updateDisplay()
{
    hideAllFx();
    
    int currentEffect = 0;
    
    //if an individual pad number is currently selected
    if(SINGLE_PAD)
    {
        int padNum = selectedPads[0];
        
        if (PAD_SETTINGS->getMode() == 2)
            currentEffect = PAD_SETTINGS->getSamplerEffect() - 1;
        else if (PAD_SETTINGS->getMode() == 3)
            currentEffect = PAD_SETTINGS->getSequencerEffect() - 1;
    }

    else if(MULTI_PADS)
    {
        if (AppSettings::Instance()->padSettings[selectedPads[0]]->getMode() == 2)
        {
            int effect_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSamplerEffect();
            for (int i = 1; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                if (PAD_SETTINGS->getSamplerEffect() != effect_)
                {
                    for (int i = 0; i < 10; i++)
                        fxButtons[i]->setToggleState(0, dontSendNotification);
                    
                    currentEffect = 0;
                    break;
                }
                if (i == selectedPads.size()-1)
                {
                    currentEffect = effect_-1;
                }
            }
        }
        else if (AppSettings::Instance()->padSettings[selectedPads[0]]->getMode() == 3)
        {
            int effect_ = AppSettings::Instance()->padSettings[selectedPads[0]]->getSequencerEffect();
            for (int i = 1; i < selectedPads.size(); i++)
            {
                int padNum = selectedPads[i];
                if (PAD_SETTINGS->getSequencerEffect() != effect_)
                {
                    for (int i = 0; i < 10; i++)
                        fxButtons[i]->setToggleState(0, dontSendNotification);
                    
                    currentEffect = 0;
                    break;
                }
                if (i == selectedPads.size()-1)
                {
                    currentEffect = effect_-1;
                }
            }
        }
    }
    
    
    if (currentEffect == 0) //Gain and Pan
    {
        gainAndPan->updateDisplay();
        gainAndPan->setVisible(true);
    }
    else if (currentEffect == 1) //LPF
    {
        lowpassFilter->updateDisplay();
        lowpassFilter->setVisible(true);
    }
    else if (currentEffect == 2) //HPF
    {
        highPassFilter->updateDisplay();
        highPassFilter->setVisible(true);
    }
    else if (currentEffect == 3) //BPF
    {
        bandPassFilter->updateDisplay();
        bandPassFilter->setVisible(true);
    }
	else if (currentEffect == 4) //Distortion
    {
        distortion->updateDisplay();
        distortion->setVisible(true);
    }
	else if (currentEffect == 5) //Bitcrusher
    {
        bitcrusher->updateDisplay();
        bitcrusher->setVisible(true);
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
    else if (currentEffect == 8) //Flanger
    {
        flanger->updateDisplay();
        flanger->setVisible(true);
    }
    else if (currentEffect == 9) //Tremolo
    {
        tremolo->updateDisplay();
        tremolo->setVisible(true);
    }
  
    if (currentEffect >= 0)
        fxButtons[currentEffect]->setToggleState(true, dontSendNotification);
    
    //repaint(); //repaint with bounds! Need to call repaint here?

}

void GuiFxDial::mouseEnter (const MouseEvent &e)
{
    
    if (fxButtons[0]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Gain and Pan. Allows the pressure of the selected pads to manipulate the gain and pan of the audio."));
    }
    else if (fxButtons[1]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Low-pass Filter.") + " " + translate(CommonInfoBoxText::effectsButtons));
    }
    else if (fxButtons[2]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("High-pass Filter.") + " " + translate(CommonInfoBoxText::effectsButtons));
    }
    else if (fxButtons[3]->isMouseOver(true))
    {
       mainComponentRef.setInfoTextBoxText(translate("Band-pass Filter.") + " " + translate(CommonInfoBoxText::effectsButtons));
    }
    else if (fxButtons[4]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Distortion.") + " " + translate(CommonInfoBoxText::effectsButtons));
    }
    else if (fxButtons[5]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Bitcrusher Distortion.") + " " + translate(CommonInfoBoxText::effectsButtons));
    }
    else if (fxButtons[6]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Delay/Echo.") + " " + translate(CommonInfoBoxText::effectsButtons));
    }
    else if (fxButtons[7]->isMouseOver(true))
    {
       mainComponentRef.setInfoTextBoxText(translate("Reverb.") + " " + translate(CommonInfoBoxText::effectsButtons));
    }
    else if (fxButtons[8]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Flanger/Phaser.") + " " + translate(CommonInfoBoxText::effectsButtons));
    }
    else if (fxButtons[9]->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate("Tremolo.") + " " + translate(CommonInfoBoxText::effectsButtons));
    }
    
     
}

void GuiFxDial::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String()); 
}

void GuiFxDial::mouseDown(const MouseEvent &e)
{
	
	if (e.mods.isPopupMenu() == true)
    {
		PopupMenu menu;
		
		if (selectedPads.size() == 1) 
		{
			menu.addItem(1, translate("Export Effect Settings..."));
		}
		else {
			menu.addItem(1, translate("Export Effect Settings..."), false);
		}
        
        const int result = menu.show();
        
        if (result == 0)
        {
            // user dismissed the menu without picking anything
        }
        
        else if (result == 1)
        {
            mainComponentRef.getAppDocumentStateRef().saveEffect(selectedPads[0]);
        }
    }

}
