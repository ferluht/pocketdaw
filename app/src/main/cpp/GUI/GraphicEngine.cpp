//
// Created by Admin on 06.07.2019.
//

#include "GraphicEngine.h"

////-------------------------------------------------------------------------
//// Ctor
////-------------------------------------------------------------------------
//GraphicEngine::GraphicEngine(Master * master_)
//        : initialized_resources_(false),
//          has_focus_(false),
//          app_(nullptr) {
//    gl_context_ = ndk_helper::GLContext::GetInstance();
//    this->master = master_;
//}
//
///**
// * Unload resources
// */
void GraphicEngine::UnloadResources() { }


/**
 * Initialize an EGL context for the current display.
 */
int GraphicEngine::InitDisplay(android_app *app) {
    if (!initialized_resources_) {
        gl_context_->Init(app_->window);
        initialized_resources_ = true;
    } else if (app->window != gl_context_->GetANativeWindow()) {
        // Re-initialize ANativeWindow.
        // On some devices, ANativeWindow is re-created when the app is resumed
        assert(gl_context_->GetANativeWindow());
        UnloadResources();
        gl_context_->Invalidate();
        app_ = app;
        gl_context_->Init(app->window);
        initialized_resources_ = true;
    } else {
        // initialize OpenGL ES and EGL
        if (EGL_SUCCESS == gl_context_->Resume(app_->window)) {
            UnloadResources();
        } else {
            assert(0);
        }
    }

    master->init();

    SetupView();

    ShowUI();

    // Initialize GL state.
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, gl_context_->GetScreenWidth(), gl_context_->GetScreenHeight());

    return 0;
}

void GraphicEngine::SetupView()
{
    int32_t viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    mat_projection_ = ndk_helper::Mat4::Ortho2D(0, 0, viewport[2], -viewport[3]);

    const float CAM_X = 0.f;
    const float CAM_Y = 0.f;
    const float CAM_Z = 700.f;

    mat_view_ = ndk_helper::Mat4::LookAt(ndk_helper::Vec3(CAM_X, CAM_Y, CAM_Z),
                                         ndk_helper::Vec3(0.f, 0.f, 0.f),
                                         ndk_helper::Vec3(0.f, 1.f, 0.f));
}

/**
 * Just the current frame in the display.
 */
void GraphicEngine::DrawFrame() {
    float fps;
    if (monitor_.Update(fps)) {
        UpdateFPS(fps);
    }

    // Just fill the screen with a color.
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    master->grender_((float)monitor_.GetCurrentTime());

    // Swap
    if (EGL_SUCCESS != gl_context_->Swap()) {
        UnloadResources();
    }
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void GraphicEngine::TermDisplay() { gl_context_->Suspend(); }

void GraphicEngine::TrimMemory() {
    LOGI("Trimming memory");
    gl_context_->Invalidate();
}

//-------------------------------------------------------------------------
// Misc
//-------------------------------------------------------------------------
void GraphicEngine::SetState(android_app *state) {
    app_ = state;
    tap_detector_.SetConfiguration(app_->config);
    doubletap_detector_.SetConfiguration(app_->config);
    drag_detector_.SetConfiguration(app_->config);
    pinch_detector_.SetConfiguration(app_->config);
}

bool GraphicEngine::IsReady() {
    return has_focus_;
}

void GraphicEngine::ShowUI() {
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, nullptr);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "showUI", "()V");
    jni->CallVoidMethod(app_->activity->clazz, methodID);

    app_->activity->vm->DetachCurrentThread();
}

void GraphicEngine::UpdateFPS(float fFPS) {
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, nullptr);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "updateFPS", "(F)V");
    jni->CallVoidMethod(app_->activity->clazz, methodID, fFPS);

    app_->activity->vm->DetachCurrentThread();
}