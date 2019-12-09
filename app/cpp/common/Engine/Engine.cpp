//
// Created by Admin on 05.09.2019.
//

#include "Engine.h"

//int32_t Engine::HandleInput(android_app *app, AInputEvent *event) {
//    auto eng = (GEngine *) app->userData;
//    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
//        ndk_helper::GESTURE_STATE tapState = eng->tap_detector_.Detect(event);
//        ndk_helper::GESTURE_STATE doubleTapState = eng->doubletap_detector_.Detect(event);
//        ndk_helper::GESTURE_STATE dragState = eng->drag_detector_.Detect(event);
//        ndk_helper::GESTURE_STATE pinchState = eng->pinch_detector_.Detect(event);
//
//        // Handle drag state
//        if (dragState & ndk_helper::GESTURE_STATE_START) {
//            // Otherwise, start dragging
//            ndk_helper::Vec2 v;
//            eng->drag_detector_.GetPointer(v);
//            while (eng->focusStack.back()->GFindFocusObject(v) == nullptr) eng->unfocus();
//            eng->focusOn(eng->focusStack.back()->GFindFocusObject(v));
//            auto new_focus = eng->focusStack.back()->GDragBegin(v);
//        } else if (dragState & ndk_helper::GESTURE_STATE_MOVE) {
//            ndk_helper::Vec2 v;
//            eng->drag_detector_.GetPointer(v);
//            auto new_focus = eng->focusStack.back()->GDragHandler(v);
//        } else if (dragState & ndk_helper::GESTURE_STATE_END) {
//            ndk_helper::Vec2 v;
//            eng->drag_detector_.GetPointer(v);
//            auto new_focus = eng->focusStack.back()->GDragEnd(v);
//            eng->unfocus();
//            eng->focusOn(new_focus);
//        }
//
//        // Handle drag state
//        if (tapState & ndk_helper::GESTURE_STATE_ACTION) {
//            // Otherwise, start dragging
//            ndk_helper::Vec2 v;
//            eng->tap_detector_.GetPointer(v);
//            while (eng->focusStack.back()->GFindFocusObject(v) == nullptr) eng->unfocus();
//            eng->focusOn(eng->focusStack.back()->GFindFocusObject(v));
//            auto new_focus = eng->focusStack.back()->GTapEnd(v);
//            eng->unfocus();
//            eng->focusOn(new_focus);
//        }

//        return 1;
//    }
//    return 0;
//}

//void Engine::HandleCmd(struct android_app *app, int32_t cmd) {
//    auto eng = (GEngine *) app->userData;
//    switch (cmd) {
//        case APP_CMD_SAVE_STATE:
//            break;
//        case APP_CMD_INIT_WINDOW:
//            // The window is being shown, get it ready.
//            if (app->window != nullptr) {
//                eng->InitDisplay(app);
//                eng->has_focus_ = true;
//                eng->DrawFrame();
//            }
//            break;
//        case APP_CMD_TERM_WINDOW:
//            // The window is being hidden or closed, clean it up.
//            eng->TermDisplay();
//            eng->has_focus_ = false;
//            break;
//        case APP_CMD_STOP:
//            break;
//        case APP_CMD_GAINED_FOCUS:
//            // Start animation
//            eng->has_focus_ = true;
//            break;
//        case APP_CMD_LOST_FOCUS:
//            // Also stop animating.
//            eng->has_focus_ = false;
//            eng->DrawFrame();
//            break;
//        case APP_CMD_LOW_MEMORY:
//            // Free up GL resources
//            eng->TrimMemory();
//            break;
//        default:
//            break;
//    }
//}