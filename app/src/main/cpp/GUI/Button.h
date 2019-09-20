//
// Created by Admin on 06.07.2019.
//

#ifndef PD_BUTTON_H
#define PD_BUTTON_H

#include <functional>
#include "Knob.h"
#include "Text.h"

class Button : public Knob{

    const float textwidth = 0.86;
    const float textheight = 0.8;

public:

    bool state;
    Text * text;
    std::function<void(bool)> callback;

    Button(wchar_t * label, std::function<void(bool)> callback_);

    GObject * GTapEnd(const ndk_helper::Vec2& v) override ;

    operator bool() const { return state; }

    Button& operator=(const bool &state_)
    {
        state = state_;
        info_overlay.GSetVisible(state);
        callback(state);
        return *this;
    }

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

class ProgressButton : public Button{

public:

    ProgressButton(wchar_t * label, std::function<void(bool)> callback_) :
    Button(label, callback_){
        info_overlay.GSetColor(1, 0.8, 0, 0.5);
    }

    void progress(float percent_) {
        info_overlay.setWidth(percent_);
    }

};

class TexturedMultiButton : public Knob{

    const float textwidth = 0.86;
    const float textheight = 0.8;

    const char ** textures;
    unsigned int num_states;

public:

    unsigned int state;
    std::function<void(unsigned int)> callback;

    TexturedMultiButton(unsigned int num_states_, const char ** textures_);
    TexturedMultiButton(std::function<void(unsigned int)> callback_, unsigned int num_states_, const char ** textures_);

    GObject * GTapEnd(const ndk_helper::Vec2& v) override ;

    operator unsigned int() const { return state; }

    TexturedMultiButton& operator=(const unsigned int &state_)
    {
        state = state_;
        info_overlay.GSetVisible(state);
        callback(state);
        return *this;
    }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override {
        if (visible && globalPosition.contains(point)) return this;
        return nullptr;
    }

    void GSetVisible(bool visible_) override {
        Knob::GSetVisible(visible_);
        info_overlay.GSetVisible(state);
    }
};


#endif //PD_BUTTON_H
