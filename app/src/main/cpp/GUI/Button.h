//
// Created by Admin on 06.07.2019.
//

#ifndef PD_BUTTON_H
#define PD_BUTTON_H

#include "Knob.h"
#include "Text.h"

class Button : public Knob{

    const float textwidth = 0.86;

public:

    bool state;
    Text * text;
    std::function<void(bool)> callback;

    Button(wchar_t * label, std::function<void(bool)> callback_);

    GObject * GTapEnd(const ndk_helper::Vec2& v) override ;

    operator bool() const { return state; }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override {
        if (visible && globalPosition.contains(point)) return this;
        return nullptr;
    }

    void GInit() override ;

    void GSetVisible(bool visible_) override {
        Knob::GSetVisible(visible_);
        info_overlay.GSetVisible(state);
    }
};


#endif //PD_BUTTON_H
