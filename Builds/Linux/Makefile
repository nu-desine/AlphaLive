# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DJUCER_LINUX_MAKE_7346DA2A=1" "-DJUCE_APP_VERSION=1.4.0" "-DJUCE_APP_VERSION_HEX=0x10400" $(shell pkg-config --cflags alsa freetype2 libcurl webkit2gtk-4.0 gtk+-x11-3.0) -pthread -I../../JuceLibraryCode -I../../JuceLibraryCode/modules $(CPPFLAGS)
  JUCE_CPPFLAGS_APP :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_TARGET_APP := AlphaLive

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa freetype2 libcurl) -fvisibility=hidden -lrt -ldl -lpthread -lGL $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DJUCER_LINUX_MAKE_7346DA2A=1" "-DJUCE_APP_VERSION=1.4.0" "-DJUCE_APP_VERSION_HEX=0x10400" $(shell pkg-config --cflags alsa freetype2 libcurl webkit2gtk-4.0 gtk+-x11-3.0) -pthread -I../../JuceLibraryCode -I../../JuceLibraryCode/modules $(CPPFLAGS)
  JUCE_CPPFLAGS_APP :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_TARGET_APP := AlphaLive

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -Os $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa freetype2 libcurl) -fvisibility=hidden -lrt -ldl -lpthread -lGL $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_APP := \
  $(JUCE_OBJDIR)/AbstractSubjectAndObserver_822e078b.o \
  $(JUCE_OBJDIR)/Main_70df61c1.o \
  $(JUCE_OBJDIR)/MainMenuModel_a5abb513.o \
  $(JUCE_OBJDIR)/MainWindow_3e8328d1.o \
  $(JUCE_OBJDIR)/BandPassFilter_ef72c13d.o \
  $(JUCE_OBJDIR)/Bitcrusher_39b803ba.o \
  $(JUCE_OBJDIR)/Delay_53cfb0ee.o \
  $(JUCE_OBJDIR)/Distortion_ece7b05c.o \
  $(JUCE_OBJDIR)/Flanger_450c9ec.o \
  $(JUCE_OBJDIR)/GainAndPan_5d65b364.o \
  $(JUCE_OBJDIR)/HighPassFilter_4c9d95ea.o \
  $(JUCE_OBJDIR)/LowpassFilter_c72d89a8.o \
  $(JUCE_OBJDIR)/Reverb_5b61dc11.o \
  $(JUCE_OBJDIR)/Tremolo_7eb54637.o \
  $(JUCE_OBJDIR)/Oscillator_932081b3.o \
  $(JUCE_OBJDIR)/AppDocumentState_54d66add.o \
  $(JUCE_OBJDIR)/AppSettings_2996bf26.o \
  $(JUCE_OBJDIR)/PadSettings_34eda98.o \
  $(JUCE_OBJDIR)/StoredSettings_ba4846e.o \
  $(JUCE_OBJDIR)/AlphaLiveEngine_9007592f.o \
  $(JUCE_OBJDIR)/ModeController_a5c344e.o \
  $(JUCE_OBJDIR)/EliteControls_56ceee4c.o \
  $(JUCE_OBJDIR)/GlobalClock_43ec288e.o \
  $(JUCE_OBJDIR)/ModeMidi_a7f7d4b8.o \
  $(JUCE_OBJDIR)/LayoutsAndScales_ee32a39d.o \
  $(JUCE_OBJDIR)/TriggerModes_6d09eb5e.o \
  $(JUCE_OBJDIR)/AudioFilePlayer_99014466.o \
  $(JUCE_OBJDIR)/ModeSampler_b2754758.o \
  $(JUCE_OBJDIR)/ModeSequencer_895b3178.o \
  $(JUCE_OBJDIR)/SequenceAudioFilePlayer_590714bc.o \
  $(JUCE_OBJDIR)/SequencePlayer_c045cf02.o \
  $(JUCE_OBJDIR)/AlphaLiveLookandFeel_3871d7ab.o \
  $(JUCE_OBJDIR)/MainBinaryData_897b98a1.o \
  $(JUCE_OBJDIR)/ThemeAuroraBinaryData_fc06b461.o \
  $(JUCE_OBJDIR)/ThemeCharcoalBinaryData_1f0b77ee.o \
  $(JUCE_OBJDIR)/ThemeClassicBinaryData_baf95571.o \
  $(JUCE_OBJDIR)/ThemeMateriaBinaryData_c448e784.o \
  $(JUCE_OBJDIR)/ThemeMinimalBinaryData_7e2e2440.o \
  $(JUCE_OBJDIR)/ThemeTransparencyBinaryData_5a7acd99.o \
  $(JUCE_OBJDIR)/GuiKeys_601b14fa.o \
  $(JUCE_OBJDIR)/GuiPiano_2d804f4d.o \
  $(JUCE_OBJDIR)/CircleWaveform_ed1ccd70.o \
  $(JUCE_OBJDIR)/CircleWaveformCache_1722c33c.o \
  $(JUCE_OBJDIR)/DemoThumbnailComp_d9ef2fdf.o \
  $(JUCE_OBJDIR)/FxGuiBandPassFilter_3148afdc.o \
  $(JUCE_OBJDIR)/FxGuiBitcrusher_b1d419d9.o \
  $(JUCE_OBJDIR)/FxGuiDelay_e638036f.o \
  $(JUCE_OBJDIR)/FxGuiDistortion_6503c67b.o \
  $(JUCE_OBJDIR)/FxGuiFlanger_9dee802d.o \
  $(JUCE_OBJDIR)/FxGuiHighPassFilter_8e738489.o \
  $(JUCE_OBJDIR)/FxGuiLowPassFilter_d7e69749.o \
  $(JUCE_OBJDIR)/FxGuiPanAndGain_b33dda77.o \
  $(JUCE_OBJDIR)/FxGuiReverb_1603d9b0.o \
  $(JUCE_OBJDIR)/FxGuiTremolo_1852fc78.o \
  $(JUCE_OBJDIR)/GuiFxDial_334fe79.o \
  $(JUCE_OBJDIR)/AlphaPopUpButton_a4331dc8.o \
  $(JUCE_OBJDIR)/AlphaPopUpImageButton_333be861.o \
  $(JUCE_OBJDIR)/AlphaRotarySlider_8b49ded2.o \
  $(JUCE_OBJDIR)/AlphaSlider_e04a0b01.o \
  $(JUCE_OBJDIR)/AlphaSquareButton_3402818f.o \
  $(JUCE_OBJDIR)/AlphaTextButton_a43b885f.o \
  $(JUCE_OBJDIR)/DeviceStatusComponent_aa8053d7.o \
  $(JUCE_OBJDIR)/FileDropButton_bc0a5ac5.o \
  $(JUCE_OBJDIR)/GuiSwitch_49506eb1.o \
  $(JUCE_OBJDIR)/ImageSlider_e336eabe.o \
  $(JUCE_OBJDIR)/ModeButton_d9248d5d.o \
  $(JUCE_OBJDIR)/SettingsButton_31587e5d.o \
  $(JUCE_OBJDIR)/SettingsButtonImage_15c903c8.o \
  $(JUCE_OBJDIR)/GuiCircleBackground_5ddb3356.o \
  $(JUCE_OBJDIR)/GuiPad_f2b1bd75.o \
  $(JUCE_OBJDIR)/GuiPadLayout_491fdfbf.o \
  $(JUCE_OBJDIR)/GuiPadRow_1eeef4af.o \
  $(JUCE_OBJDIR)/SceneComponent_7d4571d5.o \
  $(JUCE_OBJDIR)/SceneSlot_69715b30.o \
  $(JUCE_OBJDIR)/SequencerGrid_57dfa242.o \
  $(JUCE_OBJDIR)/SequencerGridEndBlock_18b2cd4.o \
  $(JUCE_OBJDIR)/SequencerGridPlayHead_79c67296.o \
  $(JUCE_OBJDIR)/SequencerGridPoint_bd9e10b8.o \
  $(JUCE_OBJDIR)/AlphaListBoxModel_467db547.o \
  $(JUCE_OBJDIR)/Toolbox_142920a0.o \
  $(JUCE_OBJDIR)/XmlAttributesListBox_fe07db9c.o \
  $(JUCE_OBJDIR)/LoadingComponent_3e0142d3.o \
  $(JUCE_OBJDIR)/LoadingWindow_4ea998a4.o \
  $(JUCE_OBJDIR)/AboutComponent_e7aa8635.o \
  $(JUCE_OBJDIR)/EliteControlsComponent_9e7340d9.o \
  $(JUCE_OBJDIR)/GuiEliteControlsSettings_e03168f6.o \
  $(JUCE_OBJDIR)/GuiGlobalClock_82c10835.o \
  $(JUCE_OBJDIR)/GuiGlobalPadSettings_90e3f87d.o \
  $(JUCE_OBJDIR)/PreferencesComponent_7ad04aa.o \
  $(JUCE_OBJDIR)/ProjectSettingsComponent_5867fbc6.o \
  $(JUCE_OBJDIR)/SoftwareUpdateComponent_26b0dc32.o \
  $(JUCE_OBJDIR)/GuiControllerMode_ef8d52cc.o \
  $(JUCE_OBJDIR)/GuiMidiMode_729c4751.o \
  $(JUCE_OBJDIR)/GuiSamplerMode_47e395c8.o \
  $(JUCE_OBJDIR)/GuiSequencerMode_f6396b91.o \
  $(JUCE_OBJDIR)/MainComponent_c2744e16.o \
  $(JUCE_OBJDIR)/HidComms_b8543e7c.o \
  $(JUCE_OBJDIR)/OscInput_88f649e3.o \
  $(JUCE_OBJDIR)/OscOutput_f9e72b52.o \
  $(JUCE_OBJDIR)/Bessel_f99b9af7.o \
  $(JUCE_OBJDIR)/Biquad_dd73553b.o \
  $(JUCE_OBJDIR)/Butterworth_a95d25cb.o \
  $(JUCE_OBJDIR)/Cascade_a132f6f1.o \
  $(JUCE_OBJDIR)/ChebyshevI_b8f1a4d9.o \
  $(JUCE_OBJDIR)/ChebyshevII_66a728da.o \
  $(JUCE_OBJDIR)/Custom_7830c51e.o \
  $(JUCE_OBJDIR)/Design_d4b60ceb.o \
  $(JUCE_OBJDIR)/Documentation_6e1999f7.o \
  $(JUCE_OBJDIR)/Elliptic_3436afaf.o \
  $(JUCE_OBJDIR)/Filter_79e5a9a5.o \
  $(JUCE_OBJDIR)/Legendre_32a21f2d.o \
  $(JUCE_OBJDIR)/Param_49c4b64a.o \
  $(JUCE_OBJDIR)/PoleFilter_d4475b7d.o \
  $(JUCE_OBJDIR)/RBJ_5217577.o \
  $(JUCE_OBJDIR)/RootFinder_e9dcfdd5.o \
  $(JUCE_OBJDIR)/State_58f9c04e.o \
  $(JUCE_OBJDIR)/hid_339907bd.o \
  $(JUCE_OBJDIR)/IpEndpointName_4c00b6d0.o \
  $(JUCE_OBJDIR)/NetworkingUtils_fa583732.o \
  $(JUCE_OBJDIR)/UdpSocket_5a0f1429.o \
  $(JUCE_OBJDIR)/OscOutboundPacketStream_483c40e8.o \
  $(JUCE_OBJDIR)/OscPrintReceivedElements_c864615f.o \
  $(JUCE_OBJDIR)/OscReceivedElements_7f52db28.o \
  $(JUCE_OBJDIR)/OscTypes_fdfe8f13.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o \
  $(JUCE_OBJDIR)/include_juce_core_f26d17db.o \
  $(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o \
  $(JUCE_OBJDIR)/include_juce_events_fd7d695.o \
  $(JUCE_OBJDIR)/include_juce_graphics_f817e147.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o \
  $(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o \

.PHONY: clean all strip

all : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : $(OBJECTS_APP) $(RESOURCES)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl
	@echo Linking "AlphaLive - App"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OBJDIR)/AbstractSubjectAndObserver_822e078b.o: ../../Source/Application/AbstractSubjectAndObserver.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AbstractSubjectAndObserver.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_70df61c1.o: ../../Source/Application/Main.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainMenuModel_a5abb513.o: ../../Source/Application/MainMenuModel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainMenuModel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainWindow_3e8328d1.o: ../../Source/Application/MainWindow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BandPassFilter_ef72c13d.o: ../../Source/Audio\ Processing/DSP\ Effects/BandPassFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BandPassFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Bitcrusher_39b803ba.o: ../../Source/Audio\ Processing/DSP\ Effects/Bitcrusher.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Bitcrusher.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Delay_53cfb0ee.o: ../../Source/Audio\ Processing/DSP\ Effects/Delay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Delay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Distortion_ece7b05c.o: ../../Source/Audio\ Processing/DSP\ Effects/Distortion.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Distortion.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Flanger_450c9ec.o: ../../Source/Audio\ Processing/DSP\ Effects/Flanger.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Flanger.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GainAndPan_5d65b364.o: ../../Source/Audio\ Processing/DSP\ Effects/GainAndPan.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GainAndPan.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/HighPassFilter_4c9d95ea.o: ../../Source/Audio\ Processing/DSP\ Effects/HighPassFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling HighPassFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LowpassFilter_c72d89a8.o: ../../Source/Audio\ Processing/DSP\ Effects/LowpassFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LowpassFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Reverb_5b61dc11.o: ../../Source/Audio\ Processing/DSP\ Effects/Reverb.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Reverb.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Tremolo_7eb54637.o: ../../Source/Audio\ Processing/DSP\ Effects/Tremolo.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Tremolo.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Oscillator_932081b3.o: ../../Source/Audio\ Processing/Oscillator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Oscillator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppDocumentState_54d66add.o: ../../Source/File\ and\ Settings/AppDocumentState.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppDocumentState.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppSettings_2996bf26.o: ../../Source/File\ and\ Settings/AppSettings.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppSettings.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PadSettings_34eda98.o: ../../Source/File\ and\ Settings/PadSettings.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PadSettings.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StoredSettings_ba4846e.o: ../../Source/File\ and\ Settings/StoredSettings.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StoredSettings.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AlphaLiveEngine_9007592f.o: ../../Source/Functionality\ Classes/AlphaLiveEngine.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AlphaLiveEngine.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModeController_a5c344e.o: ../../Source/Functionality\ Classes/Controller\ Mode/ModeController.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModeController.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EliteControls_56ceee4c.o: ../../Source/Functionality\ Classes/EliteControls.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EliteControls.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GlobalClock_43ec288e.o: ../../Source/Functionality\ Classes/GlobalClock.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GlobalClock.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModeMidi_a7f7d4b8.o: ../../Source/Functionality\ Classes/MIDI\ Mode/ModeMidi.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModeMidi.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LayoutsAndScales_ee32a39d.o: ../../Source/Functionality\ Classes/Other/LayoutsAndScales.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LayoutsAndScales.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerModes_6d09eb5e.o: ../../Source/Functionality\ Classes/Other/TriggerModes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerModes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioFilePlayer_99014466.o: ../../Source/Functionality\ Classes/Sampler\ Mode/AudioFilePlayer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioFilePlayer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModeSampler_b2754758.o: ../../Source/Functionality\ Classes/Sampler\ Mode/ModeSampler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModeSampler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModeSequencer_895b3178.o: ../../Source/Functionality\ Classes/Sequencer\ Mode/ModeSequencer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModeSequencer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceAudioFilePlayer_590714bc.o: ../../Source/Functionality\ Classes/Sequencer\ Mode/SequenceAudioFilePlayer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceAudioFilePlayer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequencePlayer_c045cf02.o: ../../Source/Functionality\ Classes/Sequencer\ Mode/SequencePlayer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequencePlayer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AlphaLiveLookandFeel_3871d7ab.o: ../../Source/GUI\ Classes/AlphaLiveLookandFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AlphaLiveLookandFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainBinaryData_897b98a1.o: ../../Source/GUI\ Classes/Binary\ Data/MainBinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ThemeAuroraBinaryData_fc06b461.o: ../../Source/GUI\ Classes/Binary\ Data/ThemeAuroraBinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ThemeAuroraBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ThemeCharcoalBinaryData_1f0b77ee.o: ../../Source/GUI\ Classes/Binary\ Data/ThemeCharcoalBinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ThemeCharcoalBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ThemeClassicBinaryData_baf95571.o: ../../Source/GUI\ Classes/Binary\ Data/ThemeClassicBinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ThemeClassicBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ThemeMateriaBinaryData_c448e784.o: ../../Source/GUI\ Classes/Binary\ Data/ThemeMateriaBinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ThemeMateriaBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ThemeMinimalBinaryData_7e2e2440.o: ../../Source/GUI\ Classes/Binary\ Data/ThemeMinimalBinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ThemeMinimalBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ThemeTransparencyBinaryData_5a7acd99.o: ../../Source/GUI\ Classes/Binary\ Data/ThemeTransparencyBinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ThemeTransparencyBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiKeys_601b14fa.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Piano/GuiKeys.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiKeys.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiPiano_2d804f4d.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Piano/GuiPiano.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiPiano.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CircleWaveform_ed1ccd70.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Waveform/CircleWaveform.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CircleWaveform.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CircleWaveformCache_1722c33c.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Waveform/CircleWaveformCache.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CircleWaveformCache.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DemoThumbnailComp_d9ef2fdf.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Waveform/DemoThumbnailComp.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DemoThumbnailComp.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiBandPassFilter_3148afdc.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiBandPassFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiBandPassFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiBitcrusher_b1d419d9.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiBitcrusher.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiBitcrusher.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiDelay_e638036f.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiDelay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiDelay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiDistortion_6503c67b.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiDistortion.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiDistortion.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiFlanger_9dee802d.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiFlanger.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiFlanger.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiHighPassFilter_8e738489.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiHighPassFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiHighPassFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiLowPassFilter_d7e69749.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiLowPassFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiLowPassFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiPanAndGain_b33dda77.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiPanAndGain.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiPanAndGain.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiReverb_1603d9b0.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiReverb.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiReverb.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FxGuiTremolo_1852fc78.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiTremolo.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FxGuiTremolo.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiFxDial_334fe79.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/GuiFxDial.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiFxDial.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AlphaPopUpButton_a4331dc8.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaPopUpButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AlphaPopUpButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AlphaPopUpImageButton_333be861.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaPopUpImageButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AlphaPopUpImageButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AlphaRotarySlider_8b49ded2.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaRotarySlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AlphaRotarySlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AlphaSlider_e04a0b01.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AlphaSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AlphaSquareButton_3402818f.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaSquareButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AlphaSquareButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AlphaTextButton_a43b885f.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaTextButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AlphaTextButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DeviceStatusComponent_aa8053d7.o: ../../Source/GUI\ Classes/Custom\ Components/General/DeviceStatusComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DeviceStatusComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FileDropButton_bc0a5ac5.o: ../../Source/GUI\ Classes/Custom\ Components/General/FileDropButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FileDropButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiSwitch_49506eb1.o: ../../Source/GUI\ Classes/Custom\ Components/General/GuiSwitch.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiSwitch.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ImageSlider_e336eabe.o: ../../Source/GUI\ Classes/Custom\ Components/General/ImageSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ImageSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModeButton_d9248d5d.o: ../../Source/GUI\ Classes/Custom\ Components/General/ModeButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModeButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsButton_31587e5d.o: ../../Source/GUI\ Classes/Custom\ Components/General/SettingsButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsButtonImage_15c903c8.o: ../../Source/GUI\ Classes/Custom\ Components/General/SettingsButtonImage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsButtonImage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiCircleBackground_5ddb3356.o: ../../Source/GUI\ Classes/Custom\ Components/Graphics/GuiCircleBackground.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiCircleBackground.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiPad_f2b1bd75.o: ../../Source/GUI\ Classes/Custom\ Components/Pad/GuiPad.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiPad.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiPadLayout_491fdfbf.o: ../../Source/GUI\ Classes/Custom\ Components/Pad/GuiPadLayout.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiPadLayout.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiPadRow_1eeef4af.o: ../../Source/GUI\ Classes/Custom\ Components/Pad/GuiPadRow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiPadRow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SceneComponent_7d4571d5.o: ../../Source/GUI\ Classes/Custom\ Components/Scene/SceneComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SceneComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SceneSlot_69715b30.o: ../../Source/GUI\ Classes/Custom\ Components/Scene/SceneSlot.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SceneSlot.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequencerGrid_57dfa242.o: ../../Source/GUI\ Classes/Custom\ Components/Sequencer/SequencerGrid.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequencerGrid.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequencerGridEndBlock_18b2cd4.o: ../../Source/GUI\ Classes/Custom\ Components/Sequencer/SequencerGridEndBlock.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequencerGridEndBlock.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequencerGridPlayHead_79c67296.o: ../../Source/GUI\ Classes/Custom\ Components/Sequencer/SequencerGridPlayHead.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequencerGridPlayHead.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequencerGridPoint_bd9e10b8.o: ../../Source/GUI\ Classes/Custom\ Components/Sequencer/SequencerGridPoint.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequencerGridPoint.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AlphaListBoxModel_467db547.o: ../../Source/GUI\ Classes/Custom\ Components/Toolbox/AlphaListBoxModel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AlphaListBoxModel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Toolbox_142920a0.o: ../../Source/GUI\ Classes/Custom\ Components/Toolbox/Toolbox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Toolbox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/XmlAttributesListBox_fe07db9c.o: ../../Source/GUI\ Classes/Custom\ Components/Toolbox/XmlAttributesListBox.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling XmlAttributesListBox.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LoadingComponent_3e0142d3.o: ../../Source/GUI\ Classes/Initial\ Loading\ Window/LoadingComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LoadingComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LoadingWindow_4ea998a4.o: ../../Source/GUI\ Classes/Initial\ Loading\ Window/LoadingWindow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LoadingWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AboutComponent_e7aa8635.o: ../../Source/GUI\ Classes/Sub-Views/AboutComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AboutComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EliteControlsComponent_9e7340d9.o: ../../Source/GUI\ Classes/Sub-Views/EliteControlsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EliteControlsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiEliteControlsSettings_e03168f6.o: ../../Source/GUI\ Classes/Sub-Views/GuiEliteControlsSettings.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiEliteControlsSettings.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiGlobalClock_82c10835.o: ../../Source/GUI\ Classes/Sub-Views/GuiGlobalClock.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiGlobalClock.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiGlobalPadSettings_90e3f87d.o: ../../Source/GUI\ Classes/Sub-Views/GuiGlobalPadSettings.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiGlobalPadSettings.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PreferencesComponent_7ad04aa.o: ../../Source/GUI\ Classes/Sub-Views/PreferencesComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PreferencesComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProjectSettingsComponent_5867fbc6.o: ../../Source/GUI\ Classes/Sub-Views/ProjectSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProjectSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SoftwareUpdateComponent_26b0dc32.o: ../../Source/GUI\ Classes/Sub-Views/SoftwareUpdateComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SoftwareUpdateComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiControllerMode_ef8d52cc.o: ../../Source/GUI\ Classes/Views/GuiControllerMode.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiControllerMode.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiMidiMode_729c4751.o: ../../Source/GUI\ Classes/Views/GuiMidiMode.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiMidiMode.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiSamplerMode_47e395c8.o: ../../Source/GUI\ Classes/Views/GuiSamplerMode.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiSamplerMode.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GuiSequencerMode_f6396b91.o: ../../Source/GUI\ Classes/Views/GuiSequencerMode.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiSequencerMode.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponent_c2744e16.o: ../../Source/GUI\ Classes/Views/MainComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/HidComms_b8543e7c.o: ../../Source/My\ IO\ Classes/HidComms.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling HidComms.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OscInput_88f649e3.o: ../../Source/My\ IO\ Classes/OscInput.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OscInput.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OscOutput_f9e72b52.o: ../../Source/My\ IO\ Classes/OscOutput.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OscOutput.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Bessel_f99b9af7.o: ../../Source/Third\ Party\ Libraries/DspFilters/Bessel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Bessel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Biquad_dd73553b.o: ../../Source/Third\ Party\ Libraries/DspFilters/Biquad.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Biquad.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Butterworth_a95d25cb.o: ../../Source/Third\ Party\ Libraries/DspFilters/Butterworth.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Butterworth.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Cascade_a132f6f1.o: ../../Source/Third\ Party\ Libraries/DspFilters/Cascade.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Cascade.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ChebyshevI_b8f1a4d9.o: ../../Source/Third\ Party\ Libraries/DspFilters/ChebyshevI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ChebyshevI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ChebyshevII_66a728da.o: ../../Source/Third\ Party\ Libraries/DspFilters/ChebyshevII.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ChebyshevII.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Custom_7830c51e.o: ../../Source/Third\ Party\ Libraries/DspFilters/Custom.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Custom.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Design_d4b60ceb.o: ../../Source/Third\ Party\ Libraries/DspFilters/Design.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Design.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Documentation_6e1999f7.o: ../../Source/Third\ Party\ Libraries/DspFilters/Documentation.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Documentation.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Elliptic_3436afaf.o: ../../Source/Third\ Party\ Libraries/DspFilters/Elliptic.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Elliptic.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Filter_79e5a9a5.o: ../../Source/Third\ Party\ Libraries/DspFilters/Filter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Filter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Legendre_32a21f2d.o: ../../Source/Third\ Party\ Libraries/DspFilters/Legendre.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Legendre.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Param_49c4b64a.o: ../../Source/Third\ Party\ Libraries/DspFilters/Param.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Param.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PoleFilter_d4475b7d.o: ../../Source/Third\ Party\ Libraries/DspFilters/PoleFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PoleFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RBJ_5217577.o: ../../Source/Third\ Party\ Libraries/DspFilters/RBJ.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RBJ.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RootFinder_e9dcfdd5.o: ../../Source/Third\ Party\ Libraries/DspFilters/RootFinder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RootFinder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/State_58f9c04e.o: ../../Source/Third\ Party\ Libraries/DspFilters/State.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling State.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/hid_339907bd.o: ../../Source/Third\ Party\ Libraries/HIDAPI/hid.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling hid.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IpEndpointName_4c00b6d0.o: ../../Source/Third\ Party\ Libraries/oscpack/ip/IpEndpointName.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IpEndpointName.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NetworkingUtils_fa583732.o: ../../Source/Third\ Party\ Libraries/oscpack/ip/posix/NetworkingUtils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NetworkingUtils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UdpSocket_5a0f1429.o: ../../Source/Third\ Party\ Libraries/oscpack/ip/posix/UdpSocket.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UdpSocket.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OscOutboundPacketStream_483c40e8.o: ../../Source/Third\ Party\ Libraries/oscpack/osc/OscOutboundPacketStream.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OscOutboundPacketStream.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OscPrintReceivedElements_c864615f.o: ../../Source/Third\ Party\ Libraries/oscpack/osc/OscPrintReceivedElements.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OscPrintReceivedElements.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OscReceivedElements_7f52db28.o: ../../Source/Third\ Party\ Libraries/oscpack/osc/OscReceivedElements.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OscReceivedElements.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OscTypes_fdfe8f13.o: ../../Source/Third\ Party\ Libraries/oscpack/osc/OscTypes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OscTypes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o: ../../JuceLibraryCode/include_juce_audio_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o: ../../JuceLibraryCode/include_juce_audio_devices.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o: ../../JuceLibraryCode/include_juce_audio_formats.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o: ../../JuceLibraryCode/include_juce_audio_processors.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o: ../../JuceLibraryCode/include_juce_audio_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_f26d17db.o: ../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o: ../../JuceLibraryCode/include_juce_cryptography.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_cryptography.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o: ../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_fd7d695.o: ../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_f817e147.o: ../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o: ../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o: ../../JuceLibraryCode/include_juce_gui_extra.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o: ../../JuceLibraryCode/include_juce_opengl.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_opengl.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

clean:
	@echo Cleaning AlphaLive
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping AlphaLive
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_APP:%.o=%.d)
