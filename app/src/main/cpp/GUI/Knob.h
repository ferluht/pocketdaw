//
// Created by Admin on 13.08.2019.
//

#ifndef PD_KNOB_H
#define PD_KNOB_H


#include "Canvas.h"
#include "Text.h"

class Knob : public MGCanvas {

public:

    GCanvas info_overlay;

    unsigned int keymap;

    Knob(){
        info_overlay.GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
        info_overlay.GSetColor(0, 0, 1, 0.2);
        info_overlay.place(0, 0);
        info_overlay.setHeight(1);
        info_overlay.setWidth(1);
        GAttach(&info_overlay);
        info_overlay.GSetVisible(false);
    }

    inline bool MMap(MData cmd) override {
        if (cmd.status == 0xB0) {
            keymap = cmd.data1;
            return true;
        }
        return false;
    }

    inline void MEnableMapping(bool state) override {
        info_overlay.GSetVisible(state);
    }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override {
        if (visible && globalPosition.contains(point)) return this;
        return nullptr;
    }
};


#endif //PD_KNOB_H
