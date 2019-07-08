//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <cerrno>

#include "Orchestration/Master.h"
#include "Orchestration/Midi.h"
#include "AudioEngine/AudioEngine.h"

#include "GUI/GraphicEngine.h"
#include "NDKHelper.h"


#define CLASS_NAME "android/app/NativeActivity"
#define APPLICATION_CLASS_NAME "com/sample/teapot/TeapotApplication"

/**
 * Process the next input event.
 */
int32_t HandleInput(android_app *app, AInputEvent *event) {
    GraphicEngine *eng = (GraphicEngine *) app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
//        ndk_helper::GESTURE_STATE tapState = eng->tap_detector_.Detect(event);
//        ndk_helper::GESTURE_STATE doubleTapState = eng->doubletap_detector_.Detect(event);
        ndk_helper::GESTURE_STATE dragState = eng->drag_detector_.Detect(event);
//        ndk_helper::GESTURE_STATE pinchState = eng->pinch_detector_.Detect(event);




//        // Double tap detector has a priority over other detectors
//        if (doubleTapState == ndk_helper::GESTURE_STATE_ACTION) {
//            // Detect double tap
//            eng->tap_camera_.Reset(true);
//        } else {
        // Handle drag state
        if (dragState & ndk_helper::GESTURE_STATE_START) {
            // Otherwise, start dragging
            ndk_helper::Vec2 v;
            eng->drag_detector_.GetPointer(v);
            eng->focus_object = eng->master->findDragHandler(v, 1.f, 1.f);
        } else if (dragState & ndk_helper::GESTURE_STATE_MOVE) {
            ndk_helper::Vec2 v;
            eng->drag_detector_.GetPointer(v);
            eng->focus_object->dragHandler(v);
        } else if (dragState & ndk_helper::GESTURE_STATE_END) {
            eng->focus_object->dragEnd();
            eng->focus_object = nullptr;
        }

//            // Handle pinch state
//            if (pinchState & ndk_helper::GESTURE_STATE_START) {
//                // Start new pinch
//                ndk_helper::Vec2 v1;
//                ndk_helper::Vec2 v2;
//                eng->pinch_detector_.GetPointers(v1, v2);
//                eng->pinchv = (v1 - v2).Length();
//                eng->EnableRender(true);
////        eng->TransformPosition(v1);
////        eng->TransformPosition(v2);
////        eng->tap_camera_.BeginPinch(v1, v2);
//            } else if (pinchState & ndk_helper::GESTURE_STATE_MOVE) {
//                // Multi touch
//                // Start new pinch
//                ndk_helper::Vec2 v1;
//                ndk_helper::Vec2 v2;
//                eng->pinch_detector_.GetPointers(v1, v2);
//                eng->master->window = eng->master->window * eng->pinchv / (v1 - v2).Length();
//                if (eng->master->window < 1) eng->master->window = 1;
//                if (eng->master->window > 10000) eng->master->window = 10000;
//                eng->EnableRender(true);
//        eng->TransformPosition(v1);
//        eng->TransformPosition(v2);
//        eng->tap_camera_.Pinch(v1, v2);
//            }

//        if (pinchState & ndk_helper::GESTURE_STATE_START) {
//            // Start new pinch
//            ndk_helper::Vec2 v1;
//            ndk_helper::Vec2 v2;
//            eng->pinch_detector_.GetPointers(v1, v2);
//            eng->TransformPosition(v1);
//            eng->TransformPosition(v2);
//            eng->tap_camera_.BeginPinch(v1, v2);
//        } else if (pinchState & ndk_helper::GESTURE_STATE_MOVE) {
//            // Multi touch
//            // Start new pinch
//            ndk_helper::Vec2 v1;
//            ndk_helper::Vec2 v2;
//            eng->pinch_detector_.GetPointers(v1, v2);
//            eng->TransformPosition(v1);
//            eng->TransformPosition(v2);
//            eng->tap_camera_.Pinch(v1, v2);
//        }

//        }
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
void HandleCmd(struct android_app *app, int32_t cmd) {
    GraphicEngine *eng = (GraphicEngine *) app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != NULL) {
                eng->InitDisplay(app);
                eng->has_focus_ = true;
                eng->DrawFrame();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            eng->TermDisplay();
            eng->has_focus_ = false;
            break;
        case APP_CMD_STOP:
            break;
        case APP_CMD_GAINED_FOCUS:
            eng->ResumeSensors();
            // Start animation
            eng->has_focus_ = true;
            break;
        case APP_CMD_LOST_FOCUS:
            eng->SuspendSensors();
            // Also stop animating.
            eng->has_focus_ = false;
            eng->DrawFrame();
            break;
        case APP_CMD_LOW_MEMORY:
            // Free up GL resources
            eng->TrimMemory();
            break;
    }
}

auto master = new Master();
auto graphicEngine = new GraphicEngine(master);
auto audioEngine = new AudioEngine(master);

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(android_app *state) {

    graphicEngine->SetState(state);
    audioEngine->start();

    // Init helper functions
    ndk_helper::JNIHelper::Init(state->activity, HELPER_CLASS_NAME);

    state->userData = graphicEngine;
    state->onAppCmd = HandleCmd;
    state->onInputEvent = HandleInput;

#ifdef USE_NDK_PROFILER
    monstartup("libMainActivity.so");
#endif

    // Prepare to monitor accelerometer
    graphicEngine->InitSensors();

    // loop waiting for stuff to do.
    while (1) {
        // Read all pending events.
        int id;
        int events;
        android_poll_source *source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((id = ALooper_pollAll(graphicEngine->IsReady() ? 0 : -1, NULL, &events,
                                     (void **) &source)) >= 0) {
            // Process this event.
            if (source != NULL) source->process(state, source);

            graphicEngine->ProcessSensors(id);

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                graphicEngine->TermDisplay();
                return;
            }
        }

        if (graphicEngine->IsReady()) {
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            graphicEngine->DrawFrame();
        }
    }
}


extern "C" {

// Data callback stuff
JavaVM *theJvm;
jobject dataCallbackObj;
jmethodID midDataCallback;

JNIEXPORT void JNICALL
Java_com_pdaw_pd_MainActivity_midiEvent(JNIEnv *env, jobject obj, jbyte status_byte,
                                        jbyte data_byte_1, jbyte data_byte_2) {
    MidiData md(status_byte, data_byte_1, data_byte_2);
    graphicEngine->master->receiveMIDI(md);
}

}

