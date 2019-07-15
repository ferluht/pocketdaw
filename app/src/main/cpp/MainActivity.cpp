//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <cerrno>

#include "Orchestration/Master.h"
#include "Orchestration/Midi.h"
#include "AudioEngine/AudioEngine.h"
#include "GUI/GraphicEngine.h"

#include <NDKHelper.h>

constexpr auto HELPER_CLASS_NAME="com/pdaw/pd/helper/NDKHelper";  // Class name of helper function

/**
 * Process the next input event.
 */
int32_t HandleInput(android_app *app, AInputEvent *event) {
    auto eng = (GraphicEngine *) app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        ndk_helper::GESTURE_STATE tapState = eng->tap_detector_.Detect(event);
        ndk_helper::GESTURE_STATE doubleTapState = eng->doubletap_detector_.Detect(event);
        ndk_helper::GESTURE_STATE dragState = eng->drag_detector_.Detect(event);
        ndk_helper::GESTURE_STATE pinchState = eng->pinch_detector_.Detect(event);

        // Handle drag state
        if (dragState & ndk_helper::GESTURE_STATE_START) {
            // Otherwise, start dragging
            ndk_helper::Vec2 v;
            eng->drag_detector_.GetPointer(v);
            eng->focus_object = eng->master->findFocusObject(v);
            eng->focus_object->dragBegin(v);
        } else if (dragState & ndk_helper::GESTURE_STATE_MOVE) {
            ndk_helper::Vec2 v;
            eng->drag_detector_.GetPointer(v);
            eng->focus_object->dragHandler(v);
        } else if (dragState & ndk_helper::GESTURE_STATE_END) {
            eng->focus_object->dragEnd();
            eng->focus_object = nullptr;
        }
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
void HandleCmd(struct android_app *app, int32_t cmd) {
    auto eng = (GraphicEngine *) app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != nullptr) {
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
            // Start animation
            eng->has_focus_ = true;
            break;
        case APP_CMD_LOST_FOCUS:
            // Also stop animating.
            eng->has_focus_ = false;
            eng->DrawFrame();
            break;
        case APP_CMD_LOW_MEMORY:
            // Free up GL resources
            eng->TrimMemory();
            break;
        default:
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

    // Init helper functions
    ndk_helper::JNIHelper::Init(state->activity, HELPER_CLASS_NAME);

    state->userData = graphicEngine;
    state->onAppCmd = HandleCmd;
    state->onInputEvent = HandleInput;

#ifdef USE_NDK_PROFILER
    monstartup("libMainActivity.so");
#endif

    // Prepare to monitor accelerometer
    audioEngine->start();

    // loop waiting for stuff to do.
    while (true) {
        // Read all pending events.
        int id;
        int events;
        android_poll_source *source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((id = ALooper_pollAll(graphicEngine->IsReady() ? 0 : -1, nullptr, &events,
                                     (void **) &source)) >= 0) {
            // Process this event.
            if (source != nullptr) source->process(state, source);

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
    MidiData md((unsigned char)status_byte, (unsigned char)data_byte_1, (unsigned char)data_byte_2);
    graphicEngine->master->receiveMIDI(md);
}

}

