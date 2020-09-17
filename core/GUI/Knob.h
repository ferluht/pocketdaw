//
// Created by Admin on 13.08.2019.
//

#ifndef PD_KNOB_H
#define PD_KNOB_H

#include <AMGEngine.h>
#include "Canvas.h"
#include "styles.h"
#include "FullscreenWindow.h"

namespace GUI {

    class Knob : public AMGObject {

        int overlay_type;
        FullscreenWindow * properties_window;

    public:

        unsigned int keymap;
        bool mapping_mode;

        Knob(unsigned int shape_type_) : AMGObject(shape_type_) {
            mapping_mode = false;
            overlay_type = shape_type_;
            properties_window = new FullscreenWindow("Properties");
            properties_window->setColor(DARK);

            GSetDoubleTapEndCallback([this](const Vec2& v) -> GUI::GObject * {
                return properties_window;
            });

            GSetDragBeginCallback([this](const Vec2& v) -> GUI::GObject * {
                mapping_mode = true;
                return this;
            });

            GSetDragEndCallback([this](const Vec2& v) -> GUI::GObject * {
                mapping_mode = false;
                return this;
            });
        }

        void MIn(MData cmd) override {
            if (mapping_mode && ((cmd.status & 0xF0) == 0xB0)) {
                keymap = cmd.data1;
                mapping_mode = false;
            }
        }

        GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override {
            if (visible && GContains(point)) {
                trace->push_front(this);
                return this;
            }
            return nullptr;
        }

        void GDraw(NVGcontext * nvg) override;

    };

}


#endif //PD_KNOB_H
