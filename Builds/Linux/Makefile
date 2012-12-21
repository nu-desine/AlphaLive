# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(TARGET_ARCH),)
  TARGET_ARCH := -march=native
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  TARGET := AlphaLive
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  TARGET := AlphaLive
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/AbstractSubjectAndObserver_822e078b.o \
  $(OBJDIR)/Main_70df61c1.o \
  $(OBJDIR)/MainMenuModel_a5abb513.o \
  $(OBJDIR)/MainWindow_3e8328d1.o \
  $(OBJDIR)/BandPassFilter_ef72c13d.o \
  $(OBJDIR)/Bitcrusher_39b803ba.o \
  $(OBJDIR)/Delay_53cfb0ee.o \
  $(OBJDIR)/Distortion_ece7b05c.o \
  $(OBJDIR)/Flanger_450c9ec.o \
  $(OBJDIR)/GainAndPan_5d65b364.o \
  $(OBJDIR)/HighPassFilter_4c9d95ea.o \
  $(OBJDIR)/LowpassFilter_c72d89a8.o \
  $(OBJDIR)/Reverb_5b61dc11.o \
  $(OBJDIR)/Tremolo_7eb54637.o \
  $(OBJDIR)/Oscillator_932081b3.o \
  $(OBJDIR)/AppDocumentState_54d66add.o \
  $(OBJDIR)/AppSettings_2996bf26.o \
  $(OBJDIR)/PadSettings_34eda98.o \
  $(OBJDIR)/StoredSettings_ba4846e.o \
  $(OBJDIR)/AlphaLiveEngine_9007592f.o \
  $(OBJDIR)/ModeController_a5c344e.o \
  $(OBJDIR)/EliteControls_56ceee4c.o \
  $(OBJDIR)/GlobalClock_43ec288e.o \
  $(OBJDIR)/ModeMidi_a7f7d4b8.o \
  $(OBJDIR)/LayoutsAndScales_ee32a39d.o \
  $(OBJDIR)/PlayStates_c50a229c.o \
  $(OBJDIR)/TriggerModes_6d09eb5e.o \
  $(OBJDIR)/AudioFilePlayer_99014466.o \
  $(OBJDIR)/ModeSampler_b2754758.o \
  $(OBJDIR)/ModeSequencer_895b3178.o \
  $(OBJDIR)/SequenceAudioFilePlayer_590714bc.o \
  $(OBJDIR)/SequencePlayer_c045cf02.o \
  $(OBJDIR)/AlphaLiveLookandFeel_3871d7ab.o \
  $(OBJDIR)/BinaryDataNew_8f48cf82.o \
  $(OBJDIR)/GuiKeys_601b14fa.o \
  $(OBJDIR)/GuiPiano_2d804f4d.o \
  $(OBJDIR)/CircleWaveform_ed1ccd70.o \
  $(OBJDIR)/CircleWaveformCache_1722c33c.o \
  $(OBJDIR)/DemoThumbnailComp_d9ef2fdf.o \
  $(OBJDIR)/FxGuiBandPassFilter_3148afdc.o \
  $(OBJDIR)/FxGuiBitcrusher_b1d419d9.o \
  $(OBJDIR)/FxGuiDelay_e638036f.o \
  $(OBJDIR)/FxGuiDistortion_6503c67b.o \
  $(OBJDIR)/FxGuiFlanger_9dee802d.o \
  $(OBJDIR)/FxGuiHighPassFilter_8e738489.o \
  $(OBJDIR)/FxGuiLowPassFilter_d7e69749.o \
  $(OBJDIR)/FxGuiPanAndGain_b33dda77.o \
  $(OBJDIR)/FxGuiReverb_1603d9b0.o \
  $(OBJDIR)/FxGuiTremolo_1852fc78.o \
  $(OBJDIR)/GuiFxDial_334fe79.o \
  $(OBJDIR)/AlphaAudioSettingsComponent_d91f2584.o \
  $(OBJDIR)/AlphaPopUpButton_a4331dc8.o \
  $(OBJDIR)/AlphaPopUpImageButton_333be861.o \
  $(OBJDIR)/AlphaRotarySlider_8b49ded2.o \
  $(OBJDIR)/AlphaSlider_e04a0b01.o \
  $(OBJDIR)/AlphaSquareButton_3402818f.o \
  $(OBJDIR)/AlphaTextButton_a43b885f.o \
  $(OBJDIR)/FileDropButton_bc0a5ac5.o \
  $(OBJDIR)/GuiSwitch_49506eb1.o \
  $(OBJDIR)/ImageSlider_e336eabe.o \
  $(OBJDIR)/ModeButton_d9248d5d.o \
  $(OBJDIR)/SettingsButton_31587e5d.o \
  $(OBJDIR)/SettingsButtonImage_15c903c8.o \
  $(OBJDIR)/GuiCircleBackground_5ddb3356.o \
  $(OBJDIR)/GuiPad_f2b1bd75.o \
  $(OBJDIR)/GuiPadLayout_491fdfbf.o \
  $(OBJDIR)/GuiPadRow_1eeef4af.o \
  $(OBJDIR)/SceneComponent_7d4571d5.o \
  $(OBJDIR)/SceneSlot_69715b30.o \
  $(OBJDIR)/SequencerGrid_57dfa242.o \
  $(OBJDIR)/SequencerGridEndBlock_18b2cd4.o \
  $(OBJDIR)/SequencerGridPlayHead_79c67296.o \
  $(OBJDIR)/SequencerGridPoint_bd9e10b8.o \
  $(OBJDIR)/AlphaListBoxModel_467db547.o \
  $(OBJDIR)/Toolbox_142920a0.o \
  $(OBJDIR)/XmlAttributesListBox_fe07db9c.o \
  $(OBJDIR)/LoadingComponent_3e0142d3.o \
  $(OBJDIR)/LoadingWindow_4ea998a4.o \
  $(OBJDIR)/AboutComponent_e7aa8635.o \
  $(OBJDIR)/EliteControlsComponent_9e7340d9.o \
  $(OBJDIR)/GuiEliteControlsSettings_e03168f6.o \
  $(OBJDIR)/GuiGlobalClock_82c10835.o \
  $(OBJDIR)/GuiGlobalPadSettings_90e3f87d.o \
  $(OBJDIR)/PreferencesComponent_7ad04aa.o \
  $(OBJDIR)/ProjectSettingsComponent_5867fbc6.o \
  $(OBJDIR)/GuiControllerMode_ef8d52cc.o \
  $(OBJDIR)/GuiMidiMode_729c4751.o \
  $(OBJDIR)/GuiSamplerMode_47e395c8.o \
  $(OBJDIR)/GuiSequencerMode_f6396b91.o \
  $(OBJDIR)/MainComponent_c2744e16.o \
  $(OBJDIR)/HidComms_b8543e7c.o \
  $(OBJDIR)/OscInput_88f649e3.o \
  $(OBJDIR)/OscOutput_f9e72b52.o \
  $(OBJDIR)/asio_d823bb2f.o \
  $(OBJDIR)/asiodrvr_69d128f9.o \
  $(OBJDIR)/combase_6dcdaea5.o \
  $(OBJDIR)/debugmessage_8f6f3cab.o \
  $(OBJDIR)/dllentry_f2fcac65.o \
  $(OBJDIR)/register_c8e9967a.o \
  $(OBJDIR)/asiosmpl_30e3ee2b.o \
  $(OBJDIR)/macnanosecs_3f0b0cb2.o \
  $(OBJDIR)/mactimer_1f841ed3.o \
  $(OBJDIR)/makesamp_b358b65c.o \
  $(OBJDIR)/wintimer_92c206e6.o \
  $(OBJDIR)/ASIOConvertSamples_3c4848c8.o \
  $(OBJDIR)/asiodrivers_7fd6a703.o \
  $(OBJDIR)/asioshlib_cb35b9c8.o \
  $(OBJDIR)/codefragments_c8f97fe6.o \
  $(OBJDIR)/asiolist_936af822.o \
  $(OBJDIR)/hostsample_a9a00a27.o \
  $(OBJDIR)/Bessel_f99b9af7.o \
  $(OBJDIR)/Biquad_dd73553b.o \
  $(OBJDIR)/Butterworth_a95d25cb.o \
  $(OBJDIR)/Cascade_a132f6f1.o \
  $(OBJDIR)/ChebyshevI_b8f1a4d9.o \
  $(OBJDIR)/ChebyshevII_66a728da.o \
  $(OBJDIR)/Custom_7830c51e.o \
  $(OBJDIR)/Design_d4b60ceb.o \
  $(OBJDIR)/Documentation_6e1999f7.o \
  $(OBJDIR)/Elliptic_3436afaf.o \
  $(OBJDIR)/Filter_79e5a9a5.o \
  $(OBJDIR)/Legendre_32a21f2d.o \
  $(OBJDIR)/Param_49c4b64a.o \
  $(OBJDIR)/PoleFilter_d4475b7d.o \
  $(OBJDIR)/RBJ_5217577.o \
  $(OBJDIR)/RootFinder_e9dcfdd5.o \
  $(OBJDIR)/State_58f9c04e.o \
  $(OBJDIR)/hid_339907bd.o \
  $(OBJDIR)/IpEndpointName_4c00b6d0.o \
  $(OBJDIR)/NetworkingUtils_fa583732.o \
  $(OBJDIR)/UdpSocket_5a0f1429.o \
  $(OBJDIR)/OscOutboundPacketStream_483c40e8.o \
  $(OBJDIR)/OscPrintReceivedElements_c864615f.o \
  $(OBJDIR)/OscReceivedElements_7f52db28.o \
  $(OBJDIR)/OscTypes_fdfe8f13.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_audio_basics_2442e4ea.o \
  $(OBJDIR)/juce_audio_devices_a4c8a728.o \
  $(OBJDIR)/juce_audio_formats_d349f0c8.o \
  $(OBJDIR)/juce_audio_processors_44a134a2.o \
  $(OBJDIR)/juce_audio_utils_f63b12e8.o \
  $(OBJDIR)/juce_core_aff681cc.o \
  $(OBJDIR)/juce_cryptography_25c7e826.o \
  $(OBJDIR)/juce_data_structures_bdd6d488.o \
  $(OBJDIR)/juce_events_79b2840.o \
  $(OBJDIR)/juce_graphics_c8f1e7a4.o \
  $(OBJDIR)/juce_gui_basics_a630dd20.o \
  $(OBJDIR)/juce_gui_extra_7767d6a8.o \
  $(OBJDIR)/juce_opengl_c7e3506c.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking AlphaLive
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning AlphaLive
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

strip:
	@echo Stripping AlphaLive
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/AbstractSubjectAndObserver_822e078b.o: ../../Source/Application/AbstractSubjectAndObserver.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AbstractSubjectAndObserver.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_70df61c1.o: ../../Source/Application/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainMenuModel_a5abb513.o: ../../Source/Application/MainMenuModel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainMenuModel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainWindow_3e8328d1.o: ../../Source/Application/MainWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BandPassFilter_ef72c13d.o: ../../Source/Audio\ Processing/DSP\ Effects/BandPassFilter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BandPassFilter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Bitcrusher_39b803ba.o: ../../Source/Audio\ Processing/DSP\ Effects/Bitcrusher.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Bitcrusher.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Delay_53cfb0ee.o: ../../Source/Audio\ Processing/DSP\ Effects/Delay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Delay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Distortion_ece7b05c.o: ../../Source/Audio\ Processing/DSP\ Effects/Distortion.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Distortion.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Flanger_450c9ec.o: ../../Source/Audio\ Processing/DSP\ Effects/Flanger.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Flanger.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GainAndPan_5d65b364.o: ../../Source/Audio\ Processing/DSP\ Effects/GainAndPan.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GainAndPan.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/HighPassFilter_4c9d95ea.o: ../../Source/Audio\ Processing/DSP\ Effects/HighPassFilter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling HighPassFilter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LowpassFilter_c72d89a8.o: ../../Source/Audio\ Processing/DSP\ Effects/LowpassFilter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LowpassFilter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Reverb_5b61dc11.o: ../../Source/Audio\ Processing/DSP\ Effects/Reverb.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Reverb.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Tremolo_7eb54637.o: ../../Source/Audio\ Processing/DSP\ Effects/Tremolo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Tremolo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Oscillator_932081b3.o: ../../Source/Audio\ Processing/Oscillator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Oscillator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AppDocumentState_54d66add.o: ../../Source/File\ and\ Settings/AppDocumentState.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AppDocumentState.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AppSettings_2996bf26.o: ../../Source/File\ and\ Settings/AppSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AppSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PadSettings_34eda98.o: ../../Source/File\ and\ Settings/PadSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PadSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/StoredSettings_ba4846e.o: ../../Source/File\ and\ Settings/StoredSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling StoredSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaLiveEngine_9007592f.o: ../../Source/Functionality\ Classes/AlphaLiveEngine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaLiveEngine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ModeController_a5c344e.o: ../../Source/Functionality\ Classes/Controller\ Mode/ModeController.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ModeController.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EliteControls_56ceee4c.o: ../../Source/Functionality\ Classes/EliteControls.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EliteControls.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GlobalClock_43ec288e.o: ../../Source/Functionality\ Classes/GlobalClock.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GlobalClock.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ModeMidi_a7f7d4b8.o: ../../Source/Functionality\ Classes/MIDI\ Mode/ModeMidi.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ModeMidi.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LayoutsAndScales_ee32a39d.o: ../../Source/Functionality\ Classes/Other/LayoutsAndScales.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LayoutsAndScales.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PlayStates_c50a229c.o: ../../Source/Functionality\ Classes/Other/PlayStates.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PlayStates.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TriggerModes_6d09eb5e.o: ../../Source/Functionality\ Classes/Other/TriggerModes.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TriggerModes.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioFilePlayer_99014466.o: ../../Source/Functionality\ Classes/Sampler\ Mode/AudioFilePlayer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioFilePlayer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ModeSampler_b2754758.o: ../../Source/Functionality\ Classes/Sampler\ Mode/ModeSampler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ModeSampler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ModeSequencer_895b3178.o: ../../Source/Functionality\ Classes/Sequencer\ Mode/ModeSequencer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ModeSequencer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequenceAudioFilePlayer_590714bc.o: ../../Source/Functionality\ Classes/Sequencer\ Mode/SequenceAudioFilePlayer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequenceAudioFilePlayer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequencePlayer_c045cf02.o: ../../Source/Functionality\ Classes/Sequencer\ Mode/SequencePlayer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequencePlayer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaLiveLookandFeel_3871d7ab.o: ../../Source/GUI\ Classes/AlphaLiveLookandFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaLiveLookandFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryDataNew_8f48cf82.o: ../../Source/GUI\ Classes/Binary\ Data/BinaryDataNew.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryDataNew.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiKeys_601b14fa.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Piano/GuiKeys.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiKeys.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiPiano_2d804f4d.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Piano/GuiPiano.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiPiano.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CircleWaveform_ed1ccd70.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Waveform/CircleWaveform.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CircleWaveform.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CircleWaveformCache_1722c33c.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Waveform/CircleWaveformCache.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CircleWaveformCache.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DemoThumbnailComp_d9ef2fdf.o: ../../Source/GUI\ Classes/Custom\ Components/Circle\ Waveform/DemoThumbnailComp.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DemoThumbnailComp.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiBandPassFilter_3148afdc.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiBandPassFilter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiBandPassFilter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiBitcrusher_b1d419d9.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiBitcrusher.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiBitcrusher.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiDelay_e638036f.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiDelay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiDelay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiDistortion_6503c67b.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiDistortion.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiDistortion.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiFlanger_9dee802d.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiFlanger.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiFlanger.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiHighPassFilter_8e738489.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiHighPassFilter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiHighPassFilter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiLowPassFilter_d7e69749.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiLowPassFilter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiLowPassFilter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiPanAndGain_b33dda77.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiPanAndGain.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiPanAndGain.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiReverb_1603d9b0.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiReverb.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiReverb.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FxGuiTremolo_1852fc78.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/Effects\ GUIs/FxGuiTremolo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FxGuiTremolo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiFxDial_334fe79.o: ../../Source/GUI\ Classes/Custom\ Components/Effects/GuiFxDial.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiFxDial.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaAudioSettingsComponent_d91f2584.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaAudioSettingsComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaAudioSettingsComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaPopUpButton_a4331dc8.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaPopUpButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaPopUpButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaPopUpImageButton_333be861.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaPopUpImageButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaPopUpImageButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaRotarySlider_8b49ded2.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaRotarySlider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaRotarySlider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaSlider_e04a0b01.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaSlider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaSlider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaSquareButton_3402818f.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaSquareButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaSquareButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaTextButton_a43b885f.o: ../../Source/GUI\ Classes/Custom\ Components/General/AlphaTextButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaTextButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FileDropButton_bc0a5ac5.o: ../../Source/GUI\ Classes/Custom\ Components/General/FileDropButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FileDropButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiSwitch_49506eb1.o: ../../Source/GUI\ Classes/Custom\ Components/General/GuiSwitch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiSwitch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ImageSlider_e336eabe.o: ../../Source/GUI\ Classes/Custom\ Components/General/ImageSlider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ImageSlider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ModeButton_d9248d5d.o: ../../Source/GUI\ Classes/Custom\ Components/General/ModeButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ModeButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SettingsButton_31587e5d.o: ../../Source/GUI\ Classes/Custom\ Components/General/SettingsButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SettingsButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SettingsButtonImage_15c903c8.o: ../../Source/GUI\ Classes/Custom\ Components/General/SettingsButtonImage.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SettingsButtonImage.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiCircleBackground_5ddb3356.o: ../../Source/GUI\ Classes/Custom\ Components/Graphics/GuiCircleBackground.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiCircleBackground.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiPad_f2b1bd75.o: ../../Source/GUI\ Classes/Custom\ Components/Pad/GuiPad.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiPad.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiPadLayout_491fdfbf.o: ../../Source/GUI\ Classes/Custom\ Components/Pad/GuiPadLayout.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiPadLayout.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiPadRow_1eeef4af.o: ../../Source/GUI\ Classes/Custom\ Components/Pad/GuiPadRow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiPadRow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SceneComponent_7d4571d5.o: ../../Source/GUI\ Classes/Custom\ Components/Scene/SceneComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SceneComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SceneSlot_69715b30.o: ../../Source/GUI\ Classes/Custom\ Components/Scene/SceneSlot.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SceneSlot.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequencerGrid_57dfa242.o: ../../Source/GUI\ Classes/Custom\ Components/Sequencer/SequencerGrid.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequencerGrid.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequencerGridEndBlock_18b2cd4.o: ../../Source/GUI\ Classes/Custom\ Components/Sequencer/SequencerGridEndBlock.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequencerGridEndBlock.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequencerGridPlayHead_79c67296.o: ../../Source/GUI\ Classes/Custom\ Components/Sequencer/SequencerGridPlayHead.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequencerGridPlayHead.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequencerGridPoint_bd9e10b8.o: ../../Source/GUI\ Classes/Custom\ Components/Sequencer/SequencerGridPoint.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequencerGridPoint.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AlphaListBoxModel_467db547.o: ../../Source/GUI\ Classes/Custom\ Components/Toolbox/AlphaListBoxModel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AlphaListBoxModel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Toolbox_142920a0.o: ../../Source/GUI\ Classes/Custom\ Components/Toolbox/Toolbox.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Toolbox.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/XmlAttributesListBox_fe07db9c.o: ../../Source/GUI\ Classes/Custom\ Components/Toolbox/XmlAttributesListBox.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling XmlAttributesListBox.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LoadingComponent_3e0142d3.o: ../../Source/GUI\ Classes/Initial\ Loading\ Window/LoadingComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LoadingComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LoadingWindow_4ea998a4.o: ../../Source/GUI\ Classes/Initial\ Loading\ Window/LoadingWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LoadingWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AboutComponent_e7aa8635.o: ../../Source/GUI\ Classes/Sub-Views/AboutComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AboutComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EliteControlsComponent_9e7340d9.o: ../../Source/GUI\ Classes/Sub-Views/EliteControlsComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EliteControlsComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiEliteControlsSettings_e03168f6.o: ../../Source/GUI\ Classes/Sub-Views/GuiEliteControlsSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiEliteControlsSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiGlobalClock_82c10835.o: ../../Source/GUI\ Classes/Sub-Views/GuiGlobalClock.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiGlobalClock.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiGlobalPadSettings_90e3f87d.o: ../../Source/GUI\ Classes/Sub-Views/GuiGlobalPadSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiGlobalPadSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PreferencesComponent_7ad04aa.o: ../../Source/GUI\ Classes/Sub-Views/PreferencesComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PreferencesComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ProjectSettingsComponent_5867fbc6.o: ../../Source/GUI\ Classes/Sub-Views/ProjectSettingsComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ProjectSettingsComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiControllerMode_ef8d52cc.o: ../../Source/GUI\ Classes/Views/GuiControllerMode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiControllerMode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiMidiMode_729c4751.o: ../../Source/GUI\ Classes/Views/GuiMidiMode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiMidiMode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiSamplerMode_47e395c8.o: ../../Source/GUI\ Classes/Views/GuiSamplerMode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiSamplerMode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GuiSequencerMode_f6396b91.o: ../../Source/GUI\ Classes/Views/GuiSequencerMode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GuiSequencerMode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainComponent_c2744e16.o: ../../Source/GUI\ Classes/Views/MainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/HidComms_b8543e7c.o: ../../Source/My\ IO\ Classes/HidComms.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling HidComms.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscInput_88f649e3.o: ../../Source/My\ IO\ Classes/OscInput.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OscInput.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscOutput_f9e72b52.o: ../../Source/My\ IO\ Classes/OscOutput.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OscOutput.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/asio_d823bb2f.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/common/asio.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling asio.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/asiodrvr_69d128f9.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/common/asiodrvr.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling asiodrvr.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/combase_6dcdaea5.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/common/combase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling combase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/debugmessage_8f6f3cab.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/common/debugmessage.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling debugmessage.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dllentry_f2fcac65.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/common/dllentry.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dllentry.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/register_c8e9967a.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/common/register.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling register.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/asiosmpl_30e3ee2b.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/driver/asiosample/asiosmpl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling asiosmpl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/macnanosecs_3f0b0cb2.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/driver/asiosample/macnanosecs.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling macnanosecs.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/mactimer_1f841ed3.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/driver/asiosample/mactimer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling mactimer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/makesamp_b358b65c.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/driver/asiosample/makesamp.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling makesamp.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wintimer_92c206e6.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/driver/asiosample/wintimer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wintimer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ASIOConvertSamples_3c4848c8.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/host/ASIOConvertSamples.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ASIOConvertSamples.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/asiodrivers_7fd6a703.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/host/asiodrivers.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling asiodrivers.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/asioshlib_cb35b9c8.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/host/mac/asioshlib.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling asioshlib.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/codefragments_c8f97fe6.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/host/mac/codefragments.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling codefragments.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/asiolist_936af822.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/host/pc/asiolist.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling asiolist.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/hostsample_a9a00a27.o: ../../Source/Third\ Party\ Libraries/ASIOSDK2/host/sample/hostsample.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling hostsample.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Bessel_f99b9af7.o: ../../Source/Third\ Party\ Libraries/DspFilters/Bessel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Bessel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Biquad_dd73553b.o: ../../Source/Third\ Party\ Libraries/DspFilters/Biquad.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Biquad.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Butterworth_a95d25cb.o: ../../Source/Third\ Party\ Libraries/DspFilters/Butterworth.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Butterworth.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Cascade_a132f6f1.o: ../../Source/Third\ Party\ Libraries/DspFilters/Cascade.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Cascade.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ChebyshevI_b8f1a4d9.o: ../../Source/Third\ Party\ Libraries/DspFilters/ChebyshevI.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ChebyshevI.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ChebyshevII_66a728da.o: ../../Source/Third\ Party\ Libraries/DspFilters/ChebyshevII.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ChebyshevII.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Custom_7830c51e.o: ../../Source/Third\ Party\ Libraries/DspFilters/Custom.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Custom.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Design_d4b60ceb.o: ../../Source/Third\ Party\ Libraries/DspFilters/Design.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Design.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Documentation_6e1999f7.o: ../../Source/Third\ Party\ Libraries/DspFilters/Documentation.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Documentation.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Elliptic_3436afaf.o: ../../Source/Third\ Party\ Libraries/DspFilters/Elliptic.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Elliptic.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Filter_79e5a9a5.o: ../../Source/Third\ Party\ Libraries/DspFilters/Filter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Filter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Legendre_32a21f2d.o: ../../Source/Third\ Party\ Libraries/DspFilters/Legendre.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Legendre.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Param_49c4b64a.o: ../../Source/Third\ Party\ Libraries/DspFilters/Param.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Param.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PoleFilter_d4475b7d.o: ../../Source/Third\ Party\ Libraries/DspFilters/PoleFilter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PoleFilter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RBJ_5217577.o: ../../Source/Third\ Party\ Libraries/DspFilters/RBJ.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RBJ.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RootFinder_e9dcfdd5.o: ../../Source/Third\ Party\ Libraries/DspFilters/RootFinder.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RootFinder.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/State_58f9c04e.o: ../../Source/Third\ Party\ Libraries/DspFilters/State.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling State.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/hid_339907bd.o: ../../Source/Third\ Party\ Libraries/HIDAPI/hid.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling hid.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/IpEndpointName_4c00b6d0.o: ../../Source/Third\ Party\ Libraries/oscpack/ip/IpEndpointName.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling IpEndpointName.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/NetworkingUtils_fa583732.o: ../../Source/Third\ Party\ Libraries/oscpack/ip/posix/NetworkingUtils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling NetworkingUtils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UdpSocket_5a0f1429.o: ../../Source/Third\ Party\ Libraries/oscpack/ip/posix/UdpSocket.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UdpSocket.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscOutboundPacketStream_483c40e8.o: ../../Source/Third\ Party\ Libraries/oscpack/osc/OscOutboundPacketStream.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OscOutboundPacketStream.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscPrintReceivedElements_c864615f.o: ../../Source/Third\ Party\ Libraries/oscpack/osc/OscPrintReceivedElements.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OscPrintReceivedElements.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscReceivedElements_7f52db28.o: ../../Source/Third\ Party\ Libraries/oscpack/osc/OscReceivedElements.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OscReceivedElements.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscTypes_fdfe8f13.o: ../../Source/Third\ Party\ Libraries/oscpack/osc/OscTypes.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OscTypes.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_2442e4ea.o: ../../JuceLibraryCode/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_a4c8a728.o: ../../JuceLibraryCode/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_d349f0c8.o: ../../JuceLibraryCode/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_44a134a2.o: ../../JuceLibraryCode/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_f63b12e8.o: ../../JuceLibraryCode/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_aff681cc.o: ../../JuceLibraryCode/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_25c7e826.o: ../../JuceLibraryCode/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_bdd6d488.o: ../../JuceLibraryCode/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_79b2840.o: ../../JuceLibraryCode/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_c8f1e7a4.o: ../../JuceLibraryCode/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_a630dd20.o: ../../JuceLibraryCode/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_7767d6a8.o: ../../JuceLibraryCode/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_c7e3506c.o: ../../JuceLibraryCode/modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
