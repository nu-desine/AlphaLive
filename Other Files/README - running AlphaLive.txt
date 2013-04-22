This directory contains most of the extra files and folders needed when running AlphaLive. 
When AlphaLive is running it expects its executable/application file to be living in a folder alongside the contents of this directory. 
Therefore once you have built AlphaLive move the binary into a folder containing these extra files.
AlphaLive can run without these files, however features such as Toolbox items, language translation, and firmware updating will not be available.

Extra files that you will need to add manually:
* The Audio Library contents - place in "./Library/Audio Library". You will only have access to these audio samples if you have purchased an AlphaSphere.
* firmwareUpdater executable/application - place in "./Application Data". You can find this within the AlphaLive download bundle available at [???], or you can build it yourself using the source code available at https://github.com/nu-desine/SphereWare/tree/master/Bootloader/HostLoaderApp.
* AlphaLive Updater executable/application - place in "./Application Data". You can find this within the AlphaLive download bundle available at [???], or you can build it yourself using the source code available at [???].
