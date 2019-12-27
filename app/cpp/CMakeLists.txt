# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

project(native-lib LANGUAGES C CXX)

# Ableton Link
set (LINK_DIR ${CMAKE_CURRENT_LIST_DIR}/AMGEngine/link)
include(${LINK_DIR}/AbletonLinkConfig.cmake)
include_directories(${LINK_DIR}/include)
include_directories(${LINK_DIR}/modules/asio-standalone/asio/include)
include_directories(src/main/cpp/Link/android-ifaddrs)

# set up common compile options
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -fno-exceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Ofast")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/AMGEngine/ndk_helper ${CMAKE_CURRENT_LIST_DIR}/.build/ndkHelper/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

set(BX_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/AMGEngine/bx)
set(BIMG_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/AMGEngine/bimg)
set(BGFX_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/AMGEngine/bgfx)

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
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Link/android-ifaddrs

        ${BGFX_DIRECTORY}/include
        ${BGFX_DIRECTORY}/3rdparty
        ${BGFX_DIRECTORY}/3rdparty/dear-imgui

        ${BX_DIRECTORY}/include

        ${BIMG_DIRECTORY}/include
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
        ${CMAKE_CURRENT_SOURCE_DIR}/AMGEngine/nanovg/nanovg.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AMGEngine/nanovg/nanovg_bgfx.cpp
        )
target_include_directories(nanovg-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanovg)

# add sources
add_library(
        native-lib

        SHARED

        MainActivity.cpp
        Synth.cpp

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

        AudioEffects/FFTConvolver/AudioFFT.cpp
        AudioEffects/FFTConvolver/FFTConvolver.cpp
        AudioEffects/FFTConvolver/TwoStageFFTConvolver.cpp
        AudioEffects/FFTConvolver/Utilities.cpp

        AMGEngine/A.cpp
        AMGEngine/G.cpp
        AMGEngine/M.cpp
        AMGEngine/AMGEngine.cpp

        GUI/Button.cpp
        GUI/Canvas.cpp
        GUI/Encoder.cpp
        GUI/Graph.cpp
        GUI/IECanvas.cpp
        GUI/Knob.cpp
        GUI/Led.cpp
        GUI/Menu.cpp
        GUI/Plot.cpp
        GUI/Slider.cpp
        GUI/EncoderButton.cpp
#        GUI/vectordisplay/vectordisplay.cpp

        Instruments/Envelopes/ADSR.cpp
        Instruments/Envelopes/ADS.cpp
        Instruments/Envelopes/AD.cpp

        Instruments/DrumRack.cpp
        Instruments/Instrument.cpp
        Instruments/Metronome.cpp
#        Instruments/Operator.cpp
        Instruments/Oscillator.cpp
        Instruments/Sampler.cpp
        Instruments/SingleTone.cpp
        Instruments/Brute.cpp

        Instruments/AnalogDrums/AnalogDrum.cpp
        Instruments/AnalogDrums/Kick.cpp
        Instruments/AnalogDrums/Snare.cpp


        MidiEffects/MidiEffect.cpp
        MidiEffects/Arpeggiator.cpp
#        MidiEffects/MidiDelay.cpp
#        MidiEffects/Scale.cpp

        Orchestration/Chain.cpp
        Orchestration/Master.cpp
#        Orchestration/Midi.cpp
        Orchestration/Rack.cpp
        Orchestration/Track.cpp
        MidiEffects/Sequencer.cpp

        Utils/AudioFile.cpp

        AMGEngine/bgfx_utils/bgfx_utils.cpp
        AMGEngine/bgfx_utils/example-glue.cpp

        AMGEngine/entry/cmd.cpp
        AMGEngine/entry/dialog.cpp
        AMGEngine/entry/entry.cpp
        AMGEngine/entry/input.cpp
        AMGEngine/entry/entry_android.cpp

        AMGEngine/imgui/imgui.cpp

        )

target_compile_definitions(native-lib PRIVATE ANDROID=1 LINK_PLATFORM_LINUX=1 PD=1 ABL_LINK_OFFSET_MS=23)

find_library(log-lib log)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS "-u ANativeActivity_onCreate")

target_link_libraries(
        native-lib
        android
        aaudio
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
