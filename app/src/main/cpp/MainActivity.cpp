#include <jni.h>
#include <cerrno>

#include "Engine/Engine.h"
#include "Synth.h"

constexpr auto HELPER_CLASS_NAME="com/pdaw/pd/helper/NDKHelper";  // Class name of helper function

/**
 * Process the next input event.
 */
int32_t HandleInput(android_app *app, AInputEvent *event) {
    auto eng = (GEngine *) app->userData;
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
            eng->focus_object = eng->root->GFindFocusObject(v);
            eng->focus_object->GDragBegin(v);
        } else if (dragState & ndk_helper::GESTURE_STATE_MOVE) {
            ndk_helper::Vec2 v;
            eng->drag_detector_.GetPointer(v);
            eng->focus_object->GDragHandler(v);
        } else if (dragState & ndk_helper::GESTURE_STATE_END) {
            eng->focus_object->GDragEnd();
            eng->focus_object = nullptr;
        }

        // Handle drag state
        if (tapState & ndk_helper::GESTURE_STATE_ACTION) {
            // Otherwise, start dragging
            ndk_helper::Vec2 v;
            eng->tap_detector_.GetPointer(v);
            eng->focus_object = eng->root->GFindFocusObject(v);
            eng->focus_object->GTapEnd();
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
    auto eng = (GEngine *) app->userData;
    switch (cmd) {
//        case APP_CMD_CONFIG_CHANGED:
//            eng->InitDisplay(app);
//            break;
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

auto master = new Synth();
auto engine = new Engine(master);

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(android_app *state) {

    engine->graphic->SetState(state);
    engine->midi->attachApp(state);

    // Init helper functions
    ndk_helper::JNIHelper::Init(state->activity, HELPER_CLASS_NAME);

    state->userData = engine->graphic;
    state->onAppCmd = HandleCmd;
    state->onInputEvent = HandleInput;

#ifdef USE_NDK_PROFILER
    monstartup("libMainActivity.so");
#endif

    // Prepare to monitor accelerometer
    engine->audio->start();

    int count = 0;

    // loop waiting for stuff to do.
    while (true) {
        // Read all pending events.
        int id;
        int events;
        android_poll_source *source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((id = ALooper_pollAll(engine->graphic->IsReady() ? 0 : -1, nullptr, &events,
                                     (void **) &source)) >= 0) {
            // Process this event.
            if (source != nullptr) source->process(state, source);

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine->graphic->TermDisplay();
                return;
            }
        }

        if (engine->graphic->IsReady()) {
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine->graphic->DrawFrame();
//            if(count == 0) {
//                MData md{
//                        (double) 0,
//                        (unsigned char) 0x80,
//                        (unsigned char) 50,
//                        (unsigned char) 127
//                };
//                engine->midi->MOut(md);
//            }
//            if (count == 60) {
//                MData md{
//                        (double) 0,
//                        (unsigned char) 0x80,
//                        (unsigned char) 50,
//                        (unsigned char) 0
//                };
//                engine->midi->MOut(md);
//            }
//            count = (count + 1)%120;
        }
    }
}

