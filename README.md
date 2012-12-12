# AlphaLive #

AlphaLive is the [AlphaSphere's](http://www.alphasphere.com/) official software application that allows you to program the device into an expressive musical instrument and controller. The software can functions as a MIDI mapping editor, sampler, sequencer, and Open Sound Control converter.


## Requirements ##

To build AlphaLive on Windows you will need to do the following:

+  Download and install the [Windows Driver Kit (WDK)](http://msdn.microsoft.com/en-gb/library/windows/hardware/gg487428.aspx).
+  Download the [Steinberg ASIO SDK](http://www.steinberg.net/en/company/developer.html) and add to "./Third Party Libraries". However AlphaLive can be built without ASIO compatibility by setting the JUCE_ASIO flag in "./JuceLibraryCode/AppConfig.h" to "0" which will not require the ASIO SDK.

See "./Other Files/README - running AlphaLive.txt" for instructions regarding the extra files needed when running the built AlphaLive binary. 


## License ##

AlphaLive is licensed under the GPLv2. See GPL2.txt for details.

AlphaLive and the AlphaSphere have been developed by [nu desine](http://nu-desine.com/).

The following libraries are included in the source code:

+  The [JUCE](http://www.rawmaterialsoftware.com/juce.php) library which is licensed under the GPLv2. 
+  The [oscpack](http://www.rossbencina.com/code/oscpack) library which is licensed under a BSD style license.
+  The [HIDAPI](http://www.signal11.us/oss/hidapi/) library which is licensed under a BSD style license.
+  The [JUCED](http://code.google.com/p/juced/) library which is licensed under the GPLv2. 
+  The [DSPFilters](https://github.com/vinniefalco/DSPFilters) library which is licensed under the MIT License. 

