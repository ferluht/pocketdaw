//
// Created by Admin on 13.08.2019.
//

#ifndef PD_KNOB_H
#define PD_KNOB_H

#include <common/Engine/Engine.h>

namespace GUI {

    class Knob : public MGObject {

    public:

        unsigned int keymap;

        Knob() {

        }

        inline bool MMap(MData cmd) override {
            if (cmd.status == 0xB0) {
                keymap = cmd.data1;
                return true;
            }
            return false;
        }

        inline void MEnableMapping(bool state) override {

        }

        GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override {
            if (visible && shape->contains(point)) {
                trace->push_front(this);
                return this;
            }
            return nullptr;
        }
    };

}


#endif //PD_KNOB_H
