#include "G.h"

namespace GUI {

    float GEngine::screen_width;
    float GEngine::screen_height;
    float GEngine::screen_ratio;

    struct Theme * GEngine::ui_theme;

    void Shape::GUpdateGlobalPosition(Shape *parent) {
        if (parent) {
            global.c = parent->global.c + local.c * parent->global.s;

            if (local.s.x > 0) {
                if (local.s.y > 0) {
                    global.s = local.s * parent->global.s;
                } else {
                    global.s.x = local.s.x * parent->global.s.x;
                    global.s.y = global.s.x / local.ratio;
                }
            } else {
                if (local.s.y > 0) {
                    global.s.y = local.s.y * parent->global.s.y;
                    global.s.x = global.s.y * local.ratio;
                } else {
                    global.s = parent->global.s;
                }
            }
            global.ratio = global.s.x / global.s.y;
        } else {
            vecmath::Vec2 screen_size = {GEngine::screen_width, GEngine::screen_height};
            global.c = local.c * screen_size;
            global.s = local.s * screen_size;
            global.ratio = global.s.x / global.s.y;
        }
    }

}