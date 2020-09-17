# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

project(pocketdaw LANGUAGES C CXX)

# Ableton Link
set (LINK_DIR ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/link-android)
include(${LINK_DIR}/AbletonLinkConfig.cmake)
include_directories(${LINK_DIR}/include)
include_directories(${LINK_DIR}/modules/asio-standalone/asio/include)
include_directories(src/main/cpp/Link/android-ifaddrs)

# set up common compile options
#set(CMAKE_C_FLAGS “${CMAKE_C_FLAGS} ”)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -w -fno-exceptions")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Ofast")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld -Wl,--build-id=sha1")
set(CMAKE_CXX11_EXTENSION_COMPILE_OPTION "-std=c++14")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../helpers/ndk_helper ${CMAKE_CURRENT_LIST_DIR}/../.build/ndkHelper/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

set(BX_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/bx)
set(BIMG_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/bimg)
set(BGFX_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/bgfx)

set(BGFX_SOURCE
        ${BX_DIRECTORY}/src/amalgamated.cpp
        ${BGFX_DIRECTORY}/src/amalgamated.cpp
#        ${BGFX_DIRECTORY}/src/amalgamated.mm  # iOS

        ${BIMG_DIRECTORY}/src/image.cpp
        ${BIMG_DIRECTORY}/src/image_cubemap_filter.cpp
        ${BIMG_DIRECTORY}/src/image_decode.cpp
        ${BIMG_DIRECTORY}/src/image_encode.cpp
        ${BIMG_DIRECTORY}/src/image_gnf.cpp

        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/astc_file.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/codec.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/endpoint_codec.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/footprint.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/integer_sequence_codec.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/intermediate_astc_block.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/logical_astc_block.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/partition.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/physical_astc_block.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/quantization.cc
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/src/decoder/weight_infill.cc
        )

add_library(bgfx STATIC ${BGFX_SOURCE})

target_compile_definitions(bgfx PUBLIC
#         -DBGFX_CONFIG_RENDERER_METAL  # iOS
         -DBGFX_CONFIG_RENDERER_OPENGLES=30
        )

target_compile_options(bgfx PUBLIC -stdlib=libc++)

target_include_directories(bgfx PRIVATE

        ${BX_DIRECTORY}/include
        ${BX_DIRECTORY}/3rdparty

        ${BIMG_DIRECTORY}/include
        ${BIMG_DIRECTORY}/src
        ${BIMG_DIRECTORY}/3rdparty
        ${BIMG_DIRECTORY}/3rdparty/astc-codec
        ${BIMG_DIRECTORY}/3rdparty/astc-codec/include
        ${BIMG_DIRECTORY}/3rdparty/iqa/include

        ${BGFX_DIRECTORY}/include
        ${BGFX_DIRECTORY}/3rdparty/khronos
        )



include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../core
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine
        ${CMAKE_CURRENT_SOURCE_DIR}/../helpers
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty
        ${CMAKE_CURRENT_SOURCE_DIR}/Link/android-ifaddrs

        ${BGFX_DIRECTORY}/include
        ${BGFX_DIRECTORY}/3rdparty
        ${BGFX_DIRECTORY}/3rdparty/dear-imgui

        ${BX_DIRECTORY}/include

        ${BIMG_DIRECTORY}/include

        ${OBOE_DIR}/include
        )

add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(native_app_glue PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue)

add_library(imgui-lib
        STATIC
        ${BGFX_DIRECTORY}/3rdparty/dear-imgui/imgui.cpp
        ${BGFX_DIRECTORY}/3rdparty/dear-imgui/imgui_widgets.cpp
        ${BGFX_DIRECTORY}/3rdparty/dear-imgui/imgui_draw.cpp
        )
target_include_directories(imgui-lib PUBLIC ${BGFX_DIRECTORY}/3rdparty/dear-imgui)

add_library(nanovg-lib
        STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/nanovg/nanovg.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/nanovg/nanovg_bgfx.cpp
        )
target_include_directories(nanovg-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanovg)

### INCLUDE OBOE LIBRARY ###
set (OBOE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/oboe)
add_subdirectory(${OBOE_DIR} ./oboe-bin)

# add sources
add_library(
        pocketdaw

        SHARED

        MainActivity.cpp
        Synth.cpp
#        Effector.cpp

        Link/LinkManager.cpp
        Link/android-ifaddrs/ifaddrs.cpp

        AudioEffects/AudioEffect.cpp
        AudioEffects/Delay.cpp
#        AudioEffects/Lissajous.cpp
        AudioEffects/MoogFilter.cpp
        AudioEffects/Oscilloscope.cpp
#        AudioEffects/Spectrum.cpp
        AudioEffects/StereoDelay.cpp
        AudioEffects/ConvolutionReverb.cpp
        AudioEffects/ThreeBandEQ.cpp
        AudioEffects/Saturator.cpp
        AudioEffects/Calibrator.cpp

        AudioEffects/FFTConvolver/AudioFFT.cpp
        AudioEffects/FFTConvolver/FFTConvolver.cpp
        AudioEffects/FFTConvolver/TwoStageFFTConvolver.cpp
        AudioEffects/FFTConvolver/Utilities.cpp

        ../engine/A.cpp
        ../engine/G.cpp
        ../engine/M.cpp
        ../engine/AMGEngine.cpp
        ../engine/FullDuplexStream.cpp

        GUI/Button.cpp
        GUI/Canvas.cpp
        GUI/Encoder.cpp
        GUI/Graph.cpp
        GUI/IECanvas.cpp
        GUI/Knob.cpp
        GUI/Led.cpp
        GUI/Text.cpp
        GUI/Menu.cpp
        GUI/Plot.cpp
        GUI/Slider.cpp
        GUI/EncoderButton.cpp
        GUI/FullscreenWindow.cpp
        GUI/Jack.cpp
        GUI/Wire.cpp
#        GUI/AnalogEncoder.cpp
#        GUI/vectordisplay/vectordisplay.cpp

        Instruments/Envelopes/ADSR.cpp
        Instruments/Envelopes/ADS.cpp
        Instruments/Envelopes/AD.cpp

        Instruments/DrumRack.cpp
        Instruments/Instrument.cpp
        Instruments/Metronome.cpp
        Instruments/Operator.cpp
        Instruments/Oscillator.cpp
        Instruments/Sampler.cpp
        Instruments/SingleTone.cpp
        Instruments/Brute.cpp
        Instruments/SoundObject5.cpp
        Instruments/SplineSynth.cpp
        Instruments/DoubleSplineSynth.cpp

        Instruments/AnalogDrums/AnalogDrum.cpp
        Instruments/AnalogDrums/Kick.cpp
        Instruments/AnalogDrums/Snare.cpp

        MidiEffects/LFO.cpp
        MidiEffects/MidiEffect.cpp
        MidiEffects/Arpeggiator.cpp
        MidiEffects/MidiMonitor.cpp
#        MidiEffects/MidiDelay.cpp
#        MidiEffects/Scale.cpp

        Orchestration/Chain.cpp
        Orchestration/Master.cpp
        Orchestration/Midi.cpp
        Orchestration/Rack.cpp
        Orchestration/Track.cpp
        MidiEffects/Sequencer.cpp

        Utils/AudioFile.cpp

        ../helpers/bgfx_utils/bgfx_utils.cpp
        ../helpers/bgfx_utils/example-glue.cpp

        ../helpers/entry/cmd.cpp
        ../helpers/entry/dialog.cpp
        ../helpers/entry/entry.cpp
        ../helpers/entry/input.cpp
        ../helpers/entry/entry_android.cpp

        ../3rdparty/imgui/imgui.cpp

        )

target_compile_definitions(pocketdaw PRIVATE ANDROID=1 LINK_PLATFORM_LINUX=1 PD=1 ABL_LINK_OFFSET_MS=23)
#target_compile_options(pocketdaw PRIVATE -std=c++14)

find_library(log-lib log)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS "-u ANativeActivity_onCreate")

target_link_libraries(
        pocketdaw
        android
#        aaudio
        oboe
        native_app_glue
        EGL
        GLESv3
        imgui-lib
        nanovg-lib
        ${log-lib}
        Ableton::Link
        NdkHelper
        bgfx
        )
