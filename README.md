# AlphaLive #

[AlphaLive](https://alphasphere.com/alphalive/) is the [AlphaSphere](https://alphasphere.com)'s official software application that allows you to program the device into an expressive musical instrument and controller. The software can functions as a MIDI mapping editor, sampler, sequencer, and Open Sound Control converter.

AlphaLive has been programmed using the [JUCE](https://juce.com) cross-platform C++ library, and includes an Xcode project for building on macOS, and a Visual Studio 2019 project for building on Windows. JUCE also easily allows for Linux makefile's to be built.

You can download the latest pre-built binaries of the application for macOS and Windows from (TODO: create tag when ready) [here](https://github.com/nu-desine/AlphaLive/releases/tag/version_1.4.0). This is a 64 bit version compatible with macOS 10.9 (Mavericks) and above and Windows 10 and above. For older OS versions (macOS 10.5 (Leopard) up to 10.9 (Mavericks), Windows 7 and 8) which are both 32 and 64 bit, see [here](https://github.com/nu-desine/AlphaLive/releases/tag/version_1.3.2).

## Requirements ##

To build AlphaLive on Windows you will need to do the following:

+  Download and install the [Windows Driver Kit (WDK)](https://docs.microsoft.com/en-gb/windows-hardware/drivers/download-the-wdk).
+  Download the [Steinberg ASIO SDK](http://www.steinberg.net/en/company/developer.html) and add to "./Source/Third Party Libraries" within a folder called "ASIOSDK". However AlphaLive can be built without ASIO compatibility by setting the JUCE_ASIO flag in "./JuceLibraryCode/AppConfig.h" to "0" which will not require the ASIO SDK.

For all platforms see "[./Other Files/README - running AlphaLive.txt](https://github.com/nu-desine/AlphaLive/blob/master/Other%20Files/README%20-%20running%20AlphaLive.txt)" for instructions regarding the extra files needed when running the built AlphaLive binary.

Use the builds Projucer files to configure the projects.

## License ##

AlphaLive is licensed under the GPLv3. See gpl-3.0.txt for details.

AlphaLive and the AlphaSphere have been developed by [nu desine](https://www.watershed.co.uk/studio/residents/nu-desine).

The following libraries are included in the source code:

+  The [JUCE](https://juce.com) library which is licensed under the GPLv3. 
+  The [oscpack](http://www.rossbencina.com/code/oscpack) library which is licensed under a BSD style license.
+  The [HIDAPI](https://github.com/signal11/hidapi) library which is licensed under a BSD style license.
+  The [JUCED](http://code.google.com/p/juced/) library which is licensed under the GPLv2. 
+  The [DSPFilters](https://github.com/vinniefalco/DSPFilters) library which is licensed under the MIT License. 

