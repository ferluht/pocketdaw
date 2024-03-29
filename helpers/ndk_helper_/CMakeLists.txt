# build native_app_glue as a static lib
cmake_minimum_required(VERSION 3.4.1)

#include(AndroidNdkModules)
#android_ndk_import_module_native_app_glue()

add_library(native_app_glue_NdkHelper STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(native_app_glue_NdkHelper PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)

add_library(NdkHelper
  STATIC
    gl3stub.cpp
    GLContext.cpp
    interpolator.cpp
    JNIHelper.cpp
    perfMonitor.cpp
    sensorManager.cpp
    shader.cpp
    tapCamera.cpp
    texture.cpp
)
set_target_properties(NdkHelper
  PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:NdkHelper,INCLUDE_DIRECTORIES>
)
target_include_directories(NdkHelper
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(NdkHelper
  PUBLIC
    native_app_glue_NdkHelper
    GLESv2
    EGL
    log
    android
    atomic
)
