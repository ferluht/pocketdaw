//
// Created by Admin on 13.08.2019.
//

#ifndef PD_KNOB_H
#define PD_KNOB_H

#include <common/Engine/Engine.h>
#include "Canvas.h"
#include "styles.h"

namespace GUI {

    class Knob : public AMGObject {

        int overlay_type;
        AMGCanvas * properties_window;

    public:

        unsigned int keymap;
        bool mapping_mode;

        Knob(unsigned int shape_type_) : AMGObject(shape_type_) {
            mapping_mode = false;
            overlay_type = shape_type_;
            properties_window = new AMGCanvas();
            properties_window->lPlace({0.1, 0.1});
            properties_window->lSetHeight(0.8);
            properties_window->lSetWidth(0.8);
            properties_window->setColor(DARK);
        }

        void MIn(MData cmd) override {
            if (mapping_mode && ((cmd.status & 0xF0) == 0xB0)) {
                keymap = cmd.data1;
                mapping_mode = false;
            }
        }

        GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override {
            if (visible && contains(point)) {
                trace->push_front(this);
                return this;
            }
            return nullptr;
        }

        virtual GObject *GDoubleTapEnd(const Vec2 &v) override {
            return properties_window;
        }

        virtual GObject *GDragBegin(const Vec2 &v) override {
            mapping_mode = true;
            return this;
        }

        virtual GObject *GDragEnd(const Vec2 &v) override {
            mapping_mode = false;
            return this;
        }

        void GDraw(NVGcontext * nvg) override;

    };

}


#endif //PD_KNOB_H
