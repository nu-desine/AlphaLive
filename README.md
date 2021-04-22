# AlphaLive #

[DOWNLOAD HERE](https://github.com/nu-desine/AlphaLive/releases/tag/version_1.4.0)

[AlphaLive](https://alphasphere.com/alphalive/) is the [AlphaSphere](https://alphasphere.com)'s official software application that allows you to program the device into an expressive musical instrument and controller. The software can functions as a MIDI mapping editor, sampler, sequencer, and Open Sound Control converter.

AlphaLive has been programmed using the [JUCE](https://juce.com) cross-platform C++ library. It includes an Xcode project for building on macOS, and a Visual Studio 2019 project for building on Windows. JUCE also easily allows for Linux makefile's to be built.

You can download the latest pre-built binaries of the application for macOS and Windows from [here](https://github.com/nu-desine/AlphaLive/releases/tag/version_1.4.0). This is a 64 bit version compatible with macOS 10.9 (Mavericks) and above, and Windows 10 and above. For older OS versions (macOS 10.5 (Leopard) up to 10.8 (Mountain Lion), Windows 7 and 8) which are both 32 and 64 bit, see [here](https://github.com/nu-desine/AlphaLive/releases/tag/version_1.3.2).

## Build Requirements ##

To build AlphaLive on macOS you will need to download and install [Xcode](https://developer.apple.com/xcode/).

To build AlphaLive on Windows you will need to do the following:

+  Download and install [Visual Studio](https://visualstudio.microsoft.com/downloads/) with the C++ SDK.
+  Download and install the [Windows Driver Kit (WDK)](https://docs.microsoft.com/en-gb/windows-hardware/drivers/download-the-wdk).
+  Download the [Steinberg ASIO SDK](http://www.steinberg.net/en/company/developer.html) and add to "./Source/Third Party Libraries" within a folder called "ASIOSDK". However AlphaLive can be built without ASIO compatibility by setting the JUCE_ASIO option in the Projucer project (Modules -> juce_audio_devices) to 'Disabled', which will allow the project to be built without the ASIO SDK.

## Building ##

For all platforms, use the AlphaLive.jucer Projucer file to configure the project for your system.

On Windows, the application must be built to [run using administrator permissions](https://docs.microsoft.com/en-us/cpp/build/reference/manifestuac-embeds-uac-information-in-manifest?view=msvc-160) in order for the software updating functionality to work correctly.

To build the AlphaLive installers, please use the installer generator projects found in the InstallerGenerators directory.
To build the macOS installer you will need to download and install [Packages](http://s.sudre.free.fr/Software/Packages/about.html).
To build the Windows installer you will need to download and install [Inno Setup](https://jrsoftware.org/isinfo.php).
It is recommended to install AlphaLive using these installers to ensure that the application is installed to the correct location along with all the needed external files. Please see the installer generator projects for information on the file hierarchy.

## Other ##

This repository also contains some pre-built binaries of some utility applications that are required for certain features of AlphaLive. The source code for these can be found at:
+  [AlphaLive Updater](https://github.com/nu-desine/AlphaLive-Updater)
+  [firmwareUpdater](https://github.com/nu-desine/SphereWare/tree/master/Bootloader/HostLoaderApp)

## License ##

AlphaLive is licensed under the GPLv3. See gpl-3.0.txt for details.

AlphaLive and the AlphaSphere have been developed by [nu desine](https://www.watershed.co.uk/studio/residents/nu-desine).

The following libraries are included in the source code:

+  The [JUCE](https://juce.com) library which is licensed under the GPLv3. 
+  The [oscpack](http://www.rossbencina.com/code/oscpack) library which is licensed under a BSD style license.
+  The [HIDAPI](https://github.com/signal11/hidapi) library which is licensed under a BSD style license.
+  The [JUCED](http://code.google.com/p/juced/) library which is licensed under the GPLv2. 
+  The [DSPFilters](https://github.com/vinniefalco/DSPFilters) library which is licensed under the MIT License. 

