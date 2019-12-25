//
// Created by Admin on 13.08.2019.
//

#ifndef PD_KNOB_H
#define PD_KNOB_H

#include <common/Engine/Engine.h>

namespace GUI {

    class Knob : public MGObject {

        int overlay_type;

    public:

        unsigned int keymap;
        bool mapping_mode;

        Knob() {
            mapping_mode = false;
            overlay_type = BOX;
        }

        inline void setOverlayType(int overlay_type_) {
            overlay_type = overlay_type_;
        }

        void MIn(MData cmd) override {
            if (mapping_mode && ((cmd.status & 0xF0) == 0xB0)) {
                keymap = cmd.data1;
                mapping_mode = false;
            }
        }

        GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override {
            if (visible && shape->contains(point)) {
                trace->push_front(this);
                return this;
            }
            return nullptr;
        }

//        virtual GObject *GDoubleTapEnd(const Vec2 &v) override {
//            mapping_mode = !mapping_mode;
//            return this;
//        }

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
