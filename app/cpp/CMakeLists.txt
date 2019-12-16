# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

project(native-lib LANGUAGES C CXX)

# Ableton Link
set (LINK_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../link)
include(${LINK_DIR}/AbletonLinkConfig.cmake)
include_directories(${LINK_DIR}/include)
include_directories(${LINK_DIR}/modules/asio-standalone/asio/include)
include_directories(src/main/cpp/common/Link/android-ifaddrs)

# set up common compile options
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -fno-exceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Ofast")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ndk_helper ${CMAKE_CURRENT_LIST_DIR}/ndkHelperBin/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

set(BX_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../../bx)
set(BIMG_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../../bimg)
set(BGFX_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../../bgfx)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/common/Link/android-ifaddrs

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
        ${CMAKE_CURRENT_SOURCE_DIR}/common/nanovg/nanovg.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/common/nanovg/nanovg_bgfx.cpp
        )
target_include_directories(nanovg-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common/nanovg)

# add sources
add_library(
        native-lib

        SHARED

        synth/MainActivity.cpp
        synth/Synth.cpp

        common/Link/LinkManager.cpp
        common/Link/android-ifaddrs/ifaddrs.cpp

        common/AudioEffects/AudioEffect.cpp
        common/AudioEffects/Delay.cpp
#        common/AudioEffects/Lissajous.cpp
        common/AudioEffects/MoogFilter.cpp
        common/AudioEffects/Oscilloscope.cpp
#        common/AudioEffects/Spectrum.cpp
        common/AudioEffects/StereoDelay.cpp
        common/AudioEffects/ConvolutionReverb.cpp
        common/AudioEffects/ThreeBandEQ.cpp

        common/AudioEffects/FFTConvolver/AudioFFT.cpp
        common/AudioEffects/FFTConvolver/FFTConvolver.cpp
        common/AudioEffects/FFTConvolver/TwoStageFFTConvolver.cpp
        common/AudioEffects/FFTConvolver/Utilities.cpp

        common/Engine/A.cpp
        common/Engine/G.cpp
        common/Engine/M.cpp
        common/Engine/Engine.cpp

        common/GUI/Button.cpp
        common/GUI/Canvas.cpp
        common/GUI/Encoder.cpp
        common/GUI/Graph.cpp
        common/GUI/IECanvas.cpp
        common/GUI/Knob.cpp
        common/GUI/Led.cpp
        common/GUI/Menu.cpp
        common/GUI/Plot.cpp
        common/GUI/Slider.cpp
#        common/GUI/vectordisplay/vectordisplay.cpp

        common/Instruments/Envelopes/ADSR.cpp
        common/Instruments/Envelopes/ADS.cpp
        common/Instruments/Envelopes/AD.cpp

        common/Instruments/DrumRack.cpp
        common/Instruments/Instrument.cpp
        common/Instruments/Metronome.cpp
#        common/Instruments/Operator.cpp
        common/Instruments/Oscillator.cpp
        common/Instruments/Sampler.cpp
        common/Instruments/SingleTone.cpp
        common/Instruments/Brute.cpp

        common/Instruments/AnalogDrums/Kick.cpp
        common/Instruments/AnalogDrums/Snare.cpp


        common/MidiEffects/MidiEffect.cpp
        common/MidiEffects/Arpeggiator.cpp
#        common/MidiEffects/MidiDelay.cpp
#        common/MidiEffects/Scale.cpp

        common/Orchestration/Chain.cpp
        common/Orchestration/Master.cpp
#        common/Orchestration/Midi.cpp
        common/Orchestration/Rack.cpp
        common/Orchestration/Track.cpp

        common/Utils/AudioFile.cpp

        common/bgfx_utils.cpp
        common/example-glue.cpp

        common/entry/cmd.cpp
        common/entry/dialog.cpp
        common/entry/entry.cpp
        common/entry/input.cpp
        common/entry/entry_android.cpp

        common/imgui/imgui.cpp

#        common/nanovg/nanovg.cpp

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
