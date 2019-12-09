//
// Created by Admin on 06.07.2019.
//

#ifndef PD_BUTTON_H
#define PD_BUTTON_H

#include <functional>
#include "Knob.h"
#include "styles.h"

namespace GUI {

    class Button : public Knob {

    public:

        const float textwidth = 0.86;
        const float textheight = 0.8;

        char * label;

        bool state;
        std::function<void(bool)> callback;

        Button(const char *label_, std::function<void(bool)> callback_);

        virtual GObject *GTapEnd(const ndk_helper::Vec2 &v) override;

        operator bool() const { return state; }

        Button &operator=(const bool &state_) {
            state = state_;
            callback(state);
            return *this;
        }

        void GDraw(NVGcontext * nvg) override;

        void GSetVisible(bool visible_) override {
            Knob::GSetVisible(visible_);
        }
    };

    class ProgressButton : public Button {

        float percent;

    public:

        ProgressButton(char *label, std::function<void(bool)> callback_) :
                Button(label, callback_) {
            percent = 0;
        }

        void progress(float percent_) {
            percent = percent_;
        }

        void GDraw(NVGcontext * nvg) override;

    };

    class TexturedMultiButton : public Knob {

        const float textwidth = 0.86;
        const float textheight = 0.8;

        const char **textures;
        unsigned int num_states;

    public:

        unsigned int state;
        std::function<void(unsigned int)> callback;

        TexturedMultiButton(unsigned int num_states_, const char **textures_);

        TexturedMultiButton(std::function<void(unsigned int)> callback_, unsigned int num_states_,
                            const char **textures_);

        GObject *GTapEnd(const ndk_helper::Vec2 &v) override;

        operator unsigned int() const { return state; }

        TexturedMultiButton &operator=(const unsigned int &state_) {
            state = state_;
            callback(state);
            return *this;
        }

        void GSetVisible(bool visible_) override {
            Knob::GSetVisible(visible_);
        }
    };

    class FocusButton : public Button {

        GObject * focus_object;

    public:

        FocusButton(const char *label, std::function<void(bool)> callback_, GObject * focus_object_) :
                Button(label, callback_) {
            focus_object = focus_object_;
        }

        virtual GObject *GTapEnd(const ndk_helper::Vec2 &v) override {
            Button::GTapEnd(v);
            if (state)
                return focus_object;
            return nullptr;
        }

        virtual void GLoseFocus() override {
            Button::GLoseFocus();
            state = false;
        }

    };
}

#endif //PD_BUTTON_H
