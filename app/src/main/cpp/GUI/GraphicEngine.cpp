//
// Created by Admin on 06.07.2019.
//

#include "GraphicEngine.h"

//-------------------------------------------------------------------------
// Ctor
//-------------------------------------------------------------------------
GraphicEngine::GraphicEngine(Master * master_)
        : initialized_resources_(false),
          has_focus_(false),
          app_(NULL),
          sensor_manager_(NULL),
          accelerometer_sensor_(NULL),
          sensor_event_queue_(NULL) {
    gl_context_ = ndk_helper::GLContext::GetInstance();
    this->master = master_;
}

//-------------------------------------------------------------------------
// Dtor
//-------------------------------------------------------------------------
GraphicEngine::~GraphicEngine() {}

/**
 * Unload resources
 */
void GraphicEngine::UnloadResources() { }


void GraphicEngine::EnableRender(bool enr) {
    render = enr;
}

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

    master->Init();

    ShowUI();

    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Note that screen size might have been changed
    glViewport(0, 0, gl_context_->GetScreenWidth(),
               gl_context_->GetScreenHeight());
    master->Update();

    return 0;
}


/**
 * Just the current frame in the display.
 */
void GraphicEngine::DrawFrame() {
    float fps;
    if (monitor_.Update(fps)) {
        UpdateFPS(fps);
    }

    if (render) {

        // Just fill the screen with a color.
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        master->grender_(monitor_.GetCurrentTime());

        // Swap
        if (EGL_SUCCESS != gl_context_->Swap()) {
            UnloadResources();
        }
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
// Sensor handlers
//-------------------------------------------------------------------------
void GraphicEngine::InitSensors() {
    sensor_manager_ = ndk_helper::AcquireASensorManagerInstance(app_);
    accelerometer_sensor_ = ASensorManager_getDefaultSensor(
            sensor_manager_, ASENSOR_TYPE_ACCELEROMETER);
    sensor_event_queue_ = ASensorManager_createEventQueue(
            sensor_manager_, app_->looper, LOOPER_ID_USER, NULL, NULL);
}

void GraphicEngine::ProcessSensors(int32_t id) {
    // If a sensor has data, process it now.
    if (id == LOOPER_ID_USER) {
        if (accelerometer_sensor_ != NULL) {
            ASensorEvent event;
            while (ASensorEventQueue_getEvents(sensor_event_queue_, &event, 1) > 0) {
            }
        }
    }
}

void GraphicEngine::ResumeSensors() {
    // When our app gains focus, we start monitoring the accelerometer.
    if (accelerometer_sensor_ != NULL) {
        ASensorEventQueue_enableSensor(sensor_event_queue_, accelerometer_sensor_);
        // We'd like to get 60 events per second (in us).
        ASensorEventQueue_setEventRate(sensor_event_queue_, accelerometer_sensor_,
                                       (1000L / 60) * 1000);
    }
}

void GraphicEngine::SuspendSensors() {
    // When our app loses focus, we stop monitoring the accelerometer.
    // This is to avoid consuming battery while not being used.
    if (accelerometer_sensor_ != NULL) {
        ASensorEventQueue_disableSensor(sensor_event_queue_, accelerometer_sensor_);
    }
}

//-------------------------------------------------------------------------
// Misc
//-------------------------------------------------------------------------
void GraphicEngine::SetState(android_app *state) {
    app_ = state;
    doubletap_detector_.SetConfiguration(app_->config);
    drag_detector_.SetConfiguration(app_->config);
    pinch_detector_.SetConfiguration(app_->config);
}

bool GraphicEngine::IsReady() {
    if (has_focus_) return true;

    return false;
}

void GraphicEngine::TransformPosition(ndk_helper::Vec2 &vec) {
    vec = ndk_helper::Vec2(2.0f, 2.0f) * vec /
          ndk_helper::Vec2(gl_context_->GetScreenWidth(),
                           gl_context_->GetScreenHeight()) -
          ndk_helper::Vec2(1.f, 1.f);
}

void GraphicEngine::ShowUI() {
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, NULL);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "showUI", "()V");
    jni->CallVoidMethod(app_->activity->clazz, methodID);

    app_->activity->vm->DetachCurrentThread();
    return;
}

void GraphicEngine::UpdateFPS(float fFPS) {
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, NULL);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "updateFPS", "(F)V");
    jni->CallVoidMethod(app_->activity->clazz, methodID, fFPS);

    app_->activity->vm->DetachCurrentThread();
    return;
}