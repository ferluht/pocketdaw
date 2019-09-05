#include <jni.h>
#include <cerrno>

#include "Engine/Engine.h"
#include "Synth.h"

constexpr auto HELPER_CLASS_NAME="com/pdaw/pd/helper/NDKHelper";  // Class name of helper function

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(android_app *state) {

    auto master = new Synth();
    auto engine = new Engine(master);

    engine->graphic->SetState(state);
    engine->midi->attachApp(state);

    // Init helper functions
    ndk_helper::JNIHelper::Init(state->activity, HELPER_CLASS_NAME);

    state->userData = engine->graphic;
    state->onAppCmd = engine->HandleCmd;
    state->onInputEvent = engine->HandleInput;

#ifdef USE_NDK_PROFILER
    monstartup("libMainActivity.so");
#endif

    engine->audio->start();

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
        }
    }
}

