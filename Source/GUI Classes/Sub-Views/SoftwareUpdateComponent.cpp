//
//  SoftwareUpdateComponent.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 20/03/2013.
//  Copyright 2013 nu desine.
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

#include "SoftwareUpdateComponent.h"
#include "../AlphaLiveLookandFeel.h"
#include "../Views/MainComponent.h"

#if JUCE_MAC || JUCE_LINUX
#include <sys/stat.h>
#endif

//Dimensions for centre text box
#define BOX_X (getWidth()/9)*3
#define BOX_Y (getHeight()/9)*3
#define BOX_W (getWidth()/9)*3
#define BOX_H (getHeight()/9)*3

SoftwareUpdateComponent::SoftwareUpdateComponent (MainComponent &ref)
                            :   Thread ("software downloader thread"),
                                mainComponentRef(ref)
{
    addAndMakeVisible (infoLabel = new Label());
    infoLabel->setJustificationType(Justification::centred);
    infoLabel->setText(translate("Downloading update.") +
                       "\n" + 
                       translate("This may take several minutes..."), 
                       dontSendNotification);
    
    progress = -1;
    addAndMakeVisible (progressBar = new ProgressBar (progress));

}

SoftwareUpdateComponent::~SoftwareUpdateComponent()
{
    stopThread(4010);
}

void SoftwareUpdateComponent::resized()
{
    infoLabel->setBounds(BOX_X+20, BOX_Y+20, BOX_W-40, BOX_H-40);
    progressBar->setBounds((getWidth()/2)-75, (BOX_Y + BOX_H) - 40, 150, 20);
}

void SoftwareUpdateComponent::paint (Graphics& g)
{
    g.setColour(AlphaTheme::getInstance()->backgroundColour.withAlpha(0.8f));
    g.fillRect(0, 0, getWidth(), getHeight());
    
    g.setColour(AlphaTheme::getInstance()->foregroundColourDarker.withAlpha(0.7f));
    g.fillRoundedRectangle(BOX_X, BOX_Y, BOX_W, BOX_H, 10);
    
    g.setColour(AlphaTheme::getInstance()->foregroundColour.withAlpha(0.6f));
    int border = 5;
    g.fillRoundedRectangle(BOX_X+border, 
                           BOX_Y+border, 
                           BOX_W-(border*2), 
                           BOX_H-(border*2), 
                           10);
}

void SoftwareUpdateComponent::run()
{
    //first make sure the AlphaLive Updater app exists,
    //and don't run the thread if it doesn't.
    alphaLiveDirectory = File::getSpecialLocation (File::currentApplicationFile).getParentDirectory();
    
    #if JUCE_MAC
    File alphaliveUpdaterApp (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/AlphaLive Updater.app");
    #endif
    #if JUCE_WINDOWS
    File alphaliveUpdaterApp (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/AlphaLive Updater.exe");
    #endif
    #if JUCE_LINUX
    File alphaliveUpdaterApp (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/AlphaLive Updater");
    #endif
    
    if (! alphaliveUpdaterApp.exists())
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          translate("Missing file!"),
                                          translate("The 'AlphaLive Updater' application appears to be missing from the 'AlphaLive/Application Data' directory. Please consult the troubleshooting section of the reference manual."));
        
        signalThreadShouldExit();
    }
    
    //===========================================
    
    while (!threadShouldExit())
    {
        {
            const MessageManagerLock mmLock;
            setVisible(true);
        }
        
        //get AlphaLive zip file from our server
        URL zipUrl ("http://www.alphasphere.com/AlphaLive_Update.zip");
        InputStream* urlStream = zipUrl.createInputStream (true);
        
        //uncompress zip file
        ZipFile zipFile (urlStream, true);
        //should it be downloaded into a temp file instead?
        Result result = zipFile.uncompressTo(alphaLiveDirectory);
        
        File updateDirectory (alphaLiveDirectory.getFullPathName() + File::separatorString + "AlphaLive_Update");
        
        if (result.wasOk() && updateDirectory.exists())
        {
            #if JUCE_MAC
            //when uncompressed, the executable bit of any files has for some been removed,
            //meaning that any application files can't be opened, as found here:
            //http://www.rawmaterialsoftware.com/viewtopic.php?f=2&t=5727
            //To fix the file permissions, you can use the command chmod 775 in terminal
            //or use th chmod() function as documented here:
            //http://www.manpagez.com/man/2/chmod/osx-10.4.php
            
            File exe1 (updateDirectory.getFullPathName() + File::separatorString + "Mac Files/AlphaLive.app/Contents/MacOS/AlphaLive");
            File exe2 (updateDirectory.getFullPathName() + File::separatorString + "Mac Files/AlphaLive Updater.app/Contents/MacOS/AlphaLive Updater");
            File exe3 (updateDirectory.getFullPathName() + File::separatorString + "Mac Files/firmwareUpdater");
            
            chmod (exe1.getFullPathName().toUTF8(), S_IRWXO | S_IRWXU | S_IRWXG);
            if (exe2.exists())
                chmod (exe2.getFullPathName().toUTF8(), S_IRWXO | S_IRWXU | S_IRWXG);
            if (exe3.exists())
                chmod (exe3.getFullPathName().toUTF8(), S_IRWXO | S_IRWXU | S_IRWXG);
            #endif 
            
            //what about on windows?
            
            //==== Move the new version of AlphaLive Updater if there is one ====
            
            #if JUCE_MAC
            File newUpdaterFile (updateDirectory.getFullPathName() + File::separatorString + "Mac Files/AlphaLive Updater.app");
            
            if (newUpdaterFile.exists())
            {
                File oldUpdaterFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/AlphaLive Updater.app");
                oldUpdaterFile.deleteRecursively();
                std::cout << newUpdaterFile.copyFileTo(oldUpdaterFile) << std::endl;
            }
            #endif
            
            #if JUCE_WINDOWS
            if (SystemStats::isOperatingSystem64Bit())
            {
                File newUpdaterFile (updateDirectory.getFullPathName() + File::separatorString +  "Win64 Files/AlphaLive Updater.exe");
                
                if (newUpdaterFile.exists())
                {
                    File oldUpdaterFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/AlphaLive Updater.exe");
                    oldUpdaterFile.deleteRecursively();
                    std::cout << newUpdaterFile.copyFileTo(oldUpdaterFile) << std::endl;
                }
            }
            else
            {
                File newUpdaterFile (updateDirectory.getFullPathName() + File::separatorString +  "Win32 Files/AlphaLive Updater.exe");
                
                if (newUpdaterFile.exists())
                {
                    File oldUpdaterFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/AlphaLive Updater.exe");
                    oldUpdaterFile.deleteRecursively();
                    std::cout << newUpdaterFile.copyFileTo(oldUpdaterFile) << std::endl;
                }
            }
            #endif
            
            {
                const MessageManagerLock mmLock;
                infoLabel->setText (translate("Closing AlphaLive") +
                                    "\n" + 
                                    translate("and launching AlphaLive Updater..."), dontSendNotification);

            }
            
            wait(4000);
            
            {
                const MessageManagerLock mmLock;
                triggerAsyncUpdate();
            }
            
        }
        else if (result.failed() || ! updateDirectory.exists())
        {
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                              translate("Couldn't download update!"),
                                              translate("There was a problem downloading the update. Please check your internet connection or try again later."));
            
            //show an error message from result.getErrorMessage() ???
        }  
        
        signalThreadShouldExit();
    }
    
    {
        const MessageManagerLock mmLock;
        setVisible(false);
    }
   
}

void SoftwareUpdateComponent::handleAsyncUpdate()
{
    //launch AlphaLive Updater
    #if JUCE_MAC
    File alphaliveUpdaterApp (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/AlphaLive Updater.app");
    #endif
    #if JUCE_WINDOWS
    File alphaliveUpdaterApp (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/AlphaLive Updater.exe");
    #endif
    #if JUCE_LINUX
    File alphaliveUpdaterApp (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/AlphaLive Updater");
    #endif
    
    if (alphaliveUpdaterApp.exists())
    {
        alphaliveUpdaterApp.startAsProcess();
        //stop HID thread and close AlphaLive
        mainComponentRef.getAlphaLiveEngineRef().stopThread(100);
        JUCEApplication::quit();
    } 

}
