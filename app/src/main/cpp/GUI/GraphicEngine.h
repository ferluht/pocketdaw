//
// Created by Admin on 06.07.2019.
//

#ifndef PD_GraphicEngine_H
#define PD_GraphicEngine_H


/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <cerrno>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>

#include "../Orchestration/Master.h"
#include "../Orchestration/Midi.h"
#include "../AudioEngine/AudioEngine.h"

#include "GraphicObject.h"
#include "NDKHelper.h"

//-------------------------------------------------------------------------
// Preprocessor
//-------------------------------------------------------------------------
#define HELPER_CLASS_NAME \
  "com/pdaw/pd/helper/NDKHelper"  // Class name of helper function
//-------------------------------------------------------------------------
// Shared state for our app.
//-------------------------------------------------------------------------
struct android_app;

class GraphicEngine {

public:

    Master * master;

    AudioEngine *audioEngine;

    ndk_helper::GLContext *gl_context_;

    bool initialized_resources_;
    bool has_focus_;

    ndk_helper::DoubletapDetector doubletap_detector_;
    ndk_helper::PinchDetector pinch_detector_;
    ndk_helper::DragDetector drag_detector_;
    ndk_helper::TapDetector tap_detector_;
    ndk_helper::PerfMonitor monitor_;

    ndk_helper::TapCamera tap_camera_;

    android_app *app_;

    GraphicObject * focus_object;

    bool render = true;
    float pinchv;

    ASensorManager *sensor_manager_;
    const ASensor *accelerometer_sensor_;
    ASensorEventQueue *sensor_event_queue_;

    void UpdateFPS(float fFPS);

    void ShowUI();

    void TransformPosition(ndk_helper::Vec2 &vec);

public:

    GraphicEngine(Master * master);

    ~GraphicEngine();

    void SetState(android_app *app);

    int InitDisplay(android_app *app);

    void UnloadResources();

    void DrawFrame();

    void TermDisplay();

    void TrimMemory();

    bool IsReady();

    void InitSensors();

    void ProcessSensors(int32_t id);

    void SuspendSensors();

    void ResumeSensors();

    void EnableRender(bool enr);
};


#endif //PD_GraphicEngine_H
