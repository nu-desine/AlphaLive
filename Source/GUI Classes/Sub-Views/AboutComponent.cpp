//
//  AboutComponent.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 22/05/2012.
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

#include "AboutComponent.h"
#include "../Views/MainComponent.h"
#include "../../Application/CommonInfoBoxText.h"
#include "../AlphaLiveLookandFeel.h"
#include "../Binary Data/BinaryDataNew.h"



AboutComponent::AboutComponent(MainComponent &ref)
                            : mainComponentRef(ref)
{
    addAndMakeVisible(titleLabel = new Label("Title", ProjectInfo::projectName));
    titleLabel->setJustificationType(Justification::horizontallyCentred);
    Font newFont1 (40, Font::bold);
    titleLabel->setFont(newFont1);
    
    addAndMakeVisible(versionNumberLabel = new Label("Version Number"));
    versionNumberLabel->setText(translate("Version") + " " + String(ProjectInfo::versionString), false);
    versionNumberLabel->setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(firmwareNumberLabel = new Label("Firmware Number"));
    firmwareNumberLabel->setText(translate("Firmware Version:") + 
                                 " " + 
                                 mainComponentRef.getAlphaLiveEngineRef().getFirmwareVersion(),
                                 false);
    firmwareNumberLabel->setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(deviceSerialLabel = new Label("Device Serial"));
    deviceSerialLabel->setText(translate("Device Serial:") + 
                               " " + 
                               mainComponentRef.getAlphaLiveEngineRef().getDeviceSerial(),
                               false);
    deviceSerialLabel->setJustificationType(Justification::centredTop);
    
    addAndMakeVisible(companyLabel = new Label("Company"));
    companyLabel->setText(translate("Developed by") + " ", false);
    companyLabel->setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(nuDesineLink = new HyperlinkButton("nu desine", URL("http://www.nu-desine.com")));
    //Font hyperlinkFont(18);
    //nuDesineLink->setFont(hyperlinkFont, true);
    
    addAndMakeVisible(githubLink = new HyperlinkButton(translate("An open source application"), 
                                                           URL("https://github.com/nu-desine/AlphaLive")));
    
    addAndMakeVisible(openSourceLabel = new Label("Open Source Label"));
    openSourceLabel->setText(translate("released under the") + " ", false);
    openSourceLabel->setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(gnuLicenseLink = new HyperlinkButton("GNU General Public License v2.0", 
                                                         URL("http://www.opensource.org/licenses/GPL-2.0")));
    
    addAndMakeVisible(copyrightLabel = new Label("Copyright Label"));
    copyrightLabel->setText(translate("Copyright 2011-2013 nu desine."), false);
    copyrightLabel->setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(samplesLabel = new Label("Samples Label"));
    samplesLabel->setText(translate("Audio samples provided by:"), false);
    samplesLabel->setJustificationType(Justification::horizontallyCentred);
    
    //logo buttons
    
    addAndMakeVisible(loopmastersButton = new ImageButton("loopmasters"));
    loopmastersButton->addListener(this);
    Image loopmastersImage = ImageCache::getFromMemory 
    (BinaryDataNew::logoloopmasterssmall_jpg, 
     BinaryDataNew::logoloopmasterssmall_jpgSize);
    loopmastersButton->setImages (true, false, true,
                                  loopmastersImage, 0.9f, Colours::transparentBlack,
                                  loopmastersImage, 1.0f, Colours::transparentBlack,
                                  loopmastersImage, 1.0f, Colours::transparentBlack,
                                  0.5f);
    
    addAndMakeVisible(zeroGButton = new ImageButton("zero-g"));
    zeroGButton->addListener(this);
    Image zeroGImage = ImageCache::getFromMemory 
    (BinaryDataNew::logozerogsmall_png, 
     BinaryDataNew::logozerogsmall_pngSize);
    zeroGButton->setImages (true, false, true,
                            zeroGImage, 0.9f, Colours::transparentBlack,
                            zeroGImage, 1.0f, Colours::transparentBlack,
                            zeroGImage, 1.0f, Colours::transparentBlack,
                            0.5f);
    
    addAndMakeVisible(fivePinMediaButton = new ImageButton("5 pin media"));
    fivePinMediaButton->addListener(this);
    Image fivePinImage = ImageCache::getFromMemory 
    (BinaryDataNew::logo5pinmediasmall_jpg, 
     BinaryDataNew::logo5pinmediasmall_jpgSize);
    fivePinMediaButton->setImages (true, false, true,
                            fivePinImage, 0.9f, Colours::transparentBlack,
                            fivePinImage, 1.0f, Colours::transparentBlack,
                            fivePinImage, 1.0f, Colours::transparentBlack,
                            0.5f);
    
    addAndMakeVisible(closeButton = new TextButton());
    closeButton->setButtonText(translate("Close"));
    closeButton->addListener(this);
    closeButton->addMouseListener(this, true);
    
    //set this component to listen to itself
    addKeyListener(this);
}

AboutComponent::~AboutComponent()
{
    deleteAllChildren();
}

void AboutComponent::resized()
{
    titleLabel->setBounds((getWidth()/2)-100, (getHeight()/6)+10, 200, 40);
    versionNumberLabel->setBounds((getWidth()/2)-75, (getHeight()/6)+40, 150, 40);
    
    firmwareNumberLabel->setBounds((getWidth()/2)-75, (getHeight()/6)+70, 150, 40);
    deviceSerialLabel->setBounds((getWidth()/2)-75, (getHeight()/6)+100, 150, 40);
    
    companyLabel->setBounds((getWidth()/2)-75, (getHeight()/6)+130, 150, 40);
    nuDesineLink->setBounds((getWidth()/2)-40, (getHeight()/6)+155, 80, 22);
    
    githubLink->setBounds((getWidth()/2)-150, (getHeight()/6)+185, 300, 22);
    openSourceLabel->setBounds((getWidth()/2)-150, (getHeight()/6)+193, 300, 40);
    gnuLicenseLink->setBounds((getWidth()/2)-105, (getHeight()/6)+217, 220, 22);
    copyrightLabel->setBounds(((getWidth()/2)-155)+45, (getHeight()/6)+225, 220, 40);
    
    samplesLabel->setBounds(((getWidth()/2)-155)+45, (getHeight()/6)+260, 220, 40);
    loopmastersButton->setBounds((getWidth()/2)-75, (getHeight()/6)+290, 150, 32);
    zeroGButton->setBounds((getWidth()/2)-75, (getHeight()/6)+320, 150, 32);
    fivePinMediaButton->setBounds((getWidth()/2)-75, (getHeight()/6)+350, 150, 32);
    
    closeButton->setBounds((getWidth()/2)-20, ((getHeight()/6)*5)-70, 40, 35);
}

void AboutComponent::paint (Graphics& g)
{
    g.setColour(Colours::black.withAlpha(0.8f));
    g.fillRect(0, 0, getWidth(), getHeight());
    
    g.setColour(Colours::grey.withAlpha(1.0f));
    //g.fillRoundedRectangle(getWidth()/3, getHeight()/4, getWidth()/3, (getHeight()/2)-30, 10);
    g.fillRoundedRectangle(getWidth()/3, getHeight()/6, getWidth()/3, ((getHeight()/6)*4)-30, 10);
    
}


void AboutComponent::buttonClicked (Button *button)
{
    if (button == closeButton)
    {
        setVisible(false);
    }
    else if (button == loopmastersButton)
    {
        URL url("http://www.loopmasters.com/");
        url.launchInDefaultBrowser();
    }
    else if (button == zeroGButton)
    {
        URL url("http://www.zero-g.co.uk/");
        url.launchInDefaultBrowser();
    }
    else if (button == fivePinMediaButton)
    {
        URL url("http://www.5pinmedia.com/");
        url.launchInDefaultBrowser();
    }
}


void AboutComponent::mouseEnter (const MouseEvent &e)
{
    if (closeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText(translate(CommonInfoBoxText::closeButton));
    }
}

void AboutComponent::mouseExit (const MouseEvent &e)
{
    //remove any text
    mainComponentRef.setInfoTextBoxText (String::empty);
}

bool AboutComponent::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    if (key == KeyPress::escapeKey)
    {
        setVisible(false);
        return true;
    }
    else
        return false; //incase the keypress is shortcut that MainComponent needs to react to.

}

void AboutComponent::setFirmwareDetails (String version, String serial)
{
    firmwareNumberLabel->setText(translate("Firmware Version:") + " " + version, false);
    deviceSerialLabel->setText(translate("Device Serial:") + " " + serial, false);
}
