//
// Created by Admin on 06.07.2019.
//

#ifndef PD_GraphicEngine_H
#define PD_GraphicEngine_H


//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <cerrno>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>

#include "Orchestration/Master.h"
#include "Orchestration/Midi.h"
#include "AudioEngine/AudioEngine.h"

#include "GraphicObject.h"
#include "NDKHelper.h"

struct SHADER {
    GLuint program_;
    GLint param_view_;
    GLint param_texture_angle_;
};

enum SHADERS{
    SHADER_BASIC,
    SHADER_ALPHA
};

//-------------------------------------------------------------------------
// Shared state for our app.
//-------------------------------------------------------------------------
struct android_app;

class GraphicEngine {

public:

    Master * master;

    ndk_helper::GLContext *gl_context_;

    SHADER shaders[3];

    bool initialized_resources_;
    bool has_focus_;

    ndk_helper::Mat4 mat_projection_;
    ndk_helper::Mat4 mat_view_;

    ndk_helper::DoubletapDetector doubletap_detector_;
    ndk_helper::PinchDetector pinch_detector_;
    ndk_helper::DragDetector drag_detector_;
    ndk_helper::TapDetector tap_detector_;
    ndk_helper::PerfMonitor monitor_;

    android_app *app_;

    GraphicObject * focus_object;

    static GraphicEngine& GetGraphicEngine()
    {
        static GraphicEngine engine;
        return engine;
    }

    void UpdateFPS(float fFPS);

    void ShowUI();

//    GraphicEngine(Master * master);

    void SetState(android_app *app);

    int InitDisplay(android_app *app);

    void UnloadResources();

    void DrawFrame();

    void TermDisplay();

    void TrimMemory();

    bool IsReady();

    static SHADER CreateShaderProgram(const char *vsh, const char *fsh);

    void SetupView();


private:
    // Private constructor
    GraphicEngine() {
        gl_context_ = ndk_helper::GLContext::GetInstance();
    }
    ~GraphicEngine() {}
    GraphicEngine(const GraphicEngine&);                 // Prevent copy-construction
    GraphicEngine& operator=(const GraphicEngine&);      // Prevent assignment
};


#endif //PD_GraphicEngine_H
