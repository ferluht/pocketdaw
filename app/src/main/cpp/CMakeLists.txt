# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

project(native-lib LANGUAGES C CXX)

set (LINK_DIR /home/ibelikov/link)

include(${LINK_DIR}/AbletonLinkConfig.cmake)

# set up common compile options
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -fno-exceptions -fno-rtti")

# build the ndk-helper library
get_filename_component(commonDir ${CMAKE_CURRENT_SOURCE_DIR}/../../../../common ABSOLUTE)
get_filename_component(ndkHelperSrc ${commonDir}/ndk_helper ABSOLUTE)
add_subdirectory(${ndkHelperSrc} ${commonDir}/ndkHelperBin/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

get_filename_component(freetypeSrc ${commonDir}/freetype-2.10.0 ABSOLUTE)
add_subdirectory(${freetypeSrc} ${commonDir}/freetypeBin/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

include_directories(src/main/cpp/Link/android-ifaddrs)
include_directories(${LINK_DIR}/include)
include_directories(${LINK_DIR}/modules/asio-standalone/asio/include)
include_directories(src/main/cpp)
include_directories(${freetypeSrc}/include)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)



# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        MainActivity.cpp
        GUI/GraphicObject.cpp
        GUI/GraphicEngine.cpp
        GUI/Button.cpp
        GUI/Encoder.cpp
        GUI/Font.cpp
        GUI/Text.cpp
        # Provides a relative path to your source file(s).

        Link/LinkManager.cpp
        Link/android-ifaddrs/ifaddrs.cpp

        Instruments/ModularSynth/VCO.cpp
        Instruments/ModularSynth/Module.cpp
        Instruments/ModularSynth/Modular.cpp
        Orchestration/Master.cpp
        Orchestration/Track.cpp
        Instruments/Instrument.cpp
        Instruments/Metronome.cpp
        Orchestration/Midi.cpp
        MidiEffects/MidiEffect.cpp
        AudioEffects/AudioEffect.cpp
        AudioEffects/Delay.cpp
        Instruments/ADSR.cpp
        MidiEffects/Arpeggiator.cpp
        AudioEngine/AudioEngine.cpp)

target_compile_definitions(native-lib PRIVATE
        ANDROID=1 LINK_PLATFORM_LINUX=1 PD=1 ABL_LINK_OFFSET_MS=23)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set_target_properties(${PROJECT_NAME}
        PROPERTIES  LINK_FLAGS "-u ANativeActivity_onCreate")

target_link_libraries( # Specifies the target library.
        native-lib
        aaudio
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        native_app_glue
        Ableton::Link
        freetype
        NdkHelper)