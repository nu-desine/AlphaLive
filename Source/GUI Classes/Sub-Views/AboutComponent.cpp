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


AboutComponent::AboutComponent(MainComponent &ref)
                            : mainComponentRef(ref)
{
    addAndMakeVisible(titleLabel = new Label("Title", ProjectInfo::projectName));
    titleLabel->setJustificationType(Justification::horizontallyCentred);
    Font newFont1 (40, Font::bold);
    titleLabel->setFont(newFont1);
    
    addAndMakeVisible(versionNumberLabel = new Label("Version Number"));
    versionNumberLabel->setText("Version " + String(ProjectInfo::versionString), false);
    titleLabel->setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(companyLabel = new Label("Company"));
    companyLabel->setText("Developed by ", false);
    companyLabel->setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(nuDesineLink = new HyperlinkButton("nu desine", URL("http://www.nu-desine.com")));
    //Font hyperlinkFont(18);
    //nuDesineLink->setFont(hyperlinkFont, true);
    
    addAndMakeVisible(openSourceLabel = new Label("Open Source Label"));
    openSourceLabel->setText("An Open Source application released under the ", false);
    openSourceLabel->setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(gnuLicenseLink = new HyperlinkButton("GNU General Public License v2.0", 
                                                         URL("http://www.opensource.org/licenses/GPL-2.0")));
    
    addAndMakeVisible(copyrightLabel = new Label("Copyright Label"));
    copyrightLabel->setText("Copyright 2011-2012 nu desine.", false);
    copyrightLabel->setJustificationType(Justification::horizontallyCentred);
    
    addAndMakeVisible(closeButton = new TextButton());
    closeButton->setButtonText("Close");
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
    titleLabel->setBounds((getWidth()/2)-100, (getHeight()/4)+10, 200, 40);
    versionNumberLabel->setBounds((getWidth()/2)-50, (getHeight()/4)+50, 100, 40);
    companyLabel->setBounds((getWidth()/2)-100, (getHeight()/4)+120, 150, 40);
    nuDesineLink->setBounds(((getWidth()/2)-100)+112, (getHeight()/4)+129, 60, 22);
    openSourceLabel->setBounds((getWidth()/2)-150, (getHeight()/4)+160, 300, 40);
    gnuLicenseLink->setBounds(((getWidth()/2)-150)+45, (getHeight()/4)+185, 200, 22);
    copyrightLabel->setBounds((getWidth()/2)-100, (getHeight()/4)+220, 200, 40);
    
    closeButton->setBounds((getWidth()/2)-35, ((getHeight()/4)*3)-60, 70, 20);
}

void AboutComponent::paint (Graphics& g)
{
    g.setColour(Colours::black.withAlpha(0.8f));
    g.fillRect(0, 0, getWidth(), getHeight());
    
    g.setColour(Colours::grey.withAlpha(0.9f));
    g.fillRoundedRectangle(getWidth()/3, getHeight()/4, getWidth()/3, (getHeight()/2)-30, 10);
    
}


void AboutComponent::buttonClicked (Button *button)
{
    if (button == closeButton)
    {
        setVisible(false);
    }
}

void AboutComponent::mouseEnter (const MouseEvent &e)
{
    if (closeButton->isMouseOver(true))
    {
        mainComponentRef.setInfoTextBoxText("Click to go back to the application's main interface. Pressing ESC also triggers this command.");
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
