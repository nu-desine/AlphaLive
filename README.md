# AlphaLive #

[AlphaLive](https://www.kvraudio.com/product/alphalive-by-nu-desine) is the [AlphaSphere](https://www.kvraudio.com/product/alphasphere-nexus-by-nu-desine)'s official software application that allows you to program the device into an expressive musical instrument and controller. The software can functions as a MIDI mapping editor, sampler, sequencer, and Open Sound Control converter.

AlphaLive has been programmed using the [JUCE](https://juce.com) cross-platform C++ library, and includes an Xcode project for building on Mac OS X, and a Visual Studio 2010 project for building on Windows. JUCE also easily allows for Linux makefile's to be built.

You can download pre-built binaries of the application for Mac OS X and Windows (32 and 64 bit) from [here](https://github.com/nu-desine/AlphaLive/releases/tag/version_1.3.2).

## Requirements ##

To build AlphaLive on Windows you will need to do the following:

+  Download and install the [Windows Driver Kit (WDK)](http://msdn.microsoft.com/en-gb/library/windows/hardware/gg487428.aspx).
+  Download the [Steinberg ASIO SDK](http://www.steinberg.net/en/company/developer.html) and add to "./Source/Third Party Libraries" within a folder called "ASIOSDK". However AlphaLive can be built without ASIO compatibility by setting the JUCE_ASIO flag in "./JuceLibraryCode/AppConfig.h" to "0" which will not require the ASIO SDK.

For all platforms see "[./Other Files/README - running AlphaLive.txt](https://github.com/nu-desine/AlphaLive/blob/master/Other%20Files/README%20-%20running%20AlphaLive.txt)" for instructions regarding the extra files needed when running the built AlphaLive binary.

If you are familiar with JUCE development, you will probably expect your first port of call to be the projects Introjucer file. However this file has not been updated in some time, so if you currently try and rebuild the Xcode/VS2010 using the Introjucer as it is, it will remove/change some important Xcode/VS2010 project settings. This will be fixed in the near future.

## License ##

AlphaLive is licensed under the GPLv2. See GPL2.txt for details.

AlphaLive and the AlphaSphere have been developed by [nu desine](http://nu-desine.com/).

The following libraries are included in the source code:

+  The [JUCE](http://www.rawmaterialsoftware.com/juce.php) library which is licensed under the GPLv2. 
+  The [oscpack](http://www.rossbencina.com/code/oscpack) library which is licensed under a BSD style license.
+  The [HIDAPI](http://www.signal11.us/oss/hidapi/) library which is licensed under a BSD style license.
+  The [JUCED](http://code.google.com/p/juced/) library which is licensed under the GPLv2. 
+  The [DSPFilters](https://github.com/vinniefalco/DSPFilters) library which is licensed under the MIT License. 

