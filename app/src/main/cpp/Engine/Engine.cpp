//
// Created by Admin on 05.09.2019.
//

#include "Engine.h"

int32_t Engine::HandleInput(android_app *app, AInputEvent *event) {
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
            while (eng->focus.back()->GFindFocusObject(v) == nullptr){
                eng->focus.back()->GLoseFocus();
                eng->focus.pop_back();
            }
            auto focus = eng->focus.back()->GFindFocusObject(v);
            if (focus != eng->focus.back()) {
                focus->GGainFocus();
                eng->focus.push_back(focus);
            }
            auto new_focus = eng->focus.back()->GDragBegin(v);
        } else if (dragState & ndk_helper::GESTURE_STATE_MOVE) {
            ndk_helper::Vec2 v;
            eng->drag_detector_.GetPointer(v);
            auto new_focus = eng->focus.back()->GDragHandler(v);
        } else if (dragState & ndk_helper::GESTURE_STATE_END) {
            auto new_focus = eng->focus.back()->GDragEnd();
            if (eng->focus.size() != 1){
                eng->focus.back()->GLoseFocus();
                eng->focus.pop_back();
            }
            if (new_focus != nullptr) {
                new_focus->GGainFocus();
                eng->focus.push_back(new_focus);
            }
        }

        // Handle drag state
        if (tapState & ndk_helper::GESTURE_STATE_ACTION) {
            // Otherwise, start dragging
            ndk_helper::Vec2 v;
            eng->tap_detector_.GetPointer(v);
            while (eng->focus.back()->GFindFocusObject(v) == nullptr){
                eng->focus.back()->GLoseFocus();
                eng->focus.pop_back();
            }
            auto focus = eng->focus.back()->GFindFocusObject(v);
            if (focus != eng->focus.back()) {
                focus->GGainFocus();
                eng->focus.push_back(focus);
            }
            auto new_focus = eng->focus.back()->GTapEnd();
            if (eng->focus.size() != 1){
                eng->focus.back()->GLoseFocus();
                eng->focus.pop_back();
            }
            if (new_focus != nullptr) {
                new_focus->GGainFocus();
                eng->focus.push_back(new_focus);
            }
        }

        return 1;
    }
    return 0;
}

void Engine::HandleCmd(struct android_app *app, int32_t cmd) {
    auto eng = (GEngine *) app->userData;
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