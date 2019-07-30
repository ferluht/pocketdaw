# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

project(native-lib LANGUAGES C CXX)

# Ableton Link
set (LINK_DIR D:/link)
include(${LINK_DIR}/AbletonLinkConfig.cmake)
include_directories(${LINK_DIR}/include)
include_directories(${LINK_DIR}/modules/asio-standalone/asio/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Link/android-ifaddrs)

# set up common compile options
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -fno-exceptions -fno-rtti")

# 3rd party library dir
get_filename_component(commonDir ${CMAKE_CURRENT_SOURCE_DIR}/../../../../common ABSOLUTE)

# build 3rd parties
add_subdirectory(${commonDir}/ndk_helper ${commonDir}/ndkHelperBin/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})
add_subdirectory(${commonDir}/freetype-2.10.0 ${commonDir}/freetypeBin/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
#        ${CMAKE_CURRENT_SOURCE_DIR}/AudioEffects
#        ${CMAKE_CURRENT_SOURCE_DIR}/AudioEngine
#        ${CMAKE_CURRENT_SOURCE_DIR}/GUI
#        ${CMAKE_CURRENT_SOURCE_DIR}/Instruments
#        ${CMAKE_CURRENT_SOURCE_DIR}/Link
#        ${CMAKE_CURRENT_SOURCE_DIR}/MidiEffects
#        ${CMAKE_CURRENT_SOURCE_DIR}/Orchestration
        )

add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(native_app_glue PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue)

# add sources
add_library(
        native-lib

        SHARED

        MainActivity.cpp

        AudioEffects/AudioEffect.cpp
        AudioEffects/Delay.cpp
        AudioEffects/StereoDelay.cpp
        AudioEffects/Waveform.cpp
#        AudioEffects/Spectrum.cpp

        AudioEngine/AudioEngine.cpp

        GUI/Shader.cpp
        GUI/GraphicObject.cpp
        GUI/GraphicEngine.cpp
        GUI/Button.cpp
        GUI/Encoder.cpp
        GUI/Font.cpp
        GUI/Text.cpp
        GUI/Canvas.cpp
        GUI/Graph.cpp

#        Instruments/ModularSynth/VCO.cpp
#        Instruments/ModularSynth/Module.cpp
#        Instruments/ModularSynth/Modular.cpp
        Instruments/Instrument.cpp
        Instruments/Metronome.cpp
        Instruments/ADSR.cpp

        Link/LinkManager.cpp
        Link/android-ifaddrs/ifaddrs.cpp

        MidiEffects/Arpeggiator.cpp

        Orchestration/Master.cpp
        Orchestration/Track.cpp
        Orchestration/Midi.cpp
        )

target_compile_definitions(native-lib PRIVATE ANDROID=1 LINK_PLATFORM_LINUX=1 PD=1 ABL_LINK_OFFSET_MS=23)

find_library(log-lib log)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS "-u ANativeActivity_onCreate")

target_link_libraries(
        native-lib
        aaudio
        ${log-lib}
        native_app_glue
        Ableton::Link
        freetype
        NdkHelper
        )