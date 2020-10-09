//
// Created by ibelikov on 27.12.19.
//

#ifndef PD_FULLSCREENWINDOW_H
#define PD_FULLSCREENWINDOW_H

#include "Canvas.h"
#include "styles.h"
#include "Text.h"

namespace GUI {

    class FullscreenWindow : public AMGCanvas {

//        AMGCanvas * close;
        Text * name;

    public:

        vecmath::Vec2 drag_from;
        vecmath::Vec2 last_point;

        FullscreenWindow(const char * name_) {
            GPlace({0, 0});
            GSetHeight(1);
            GSetWidth(1);

            name = new Text(name_);
            name->GPlace({0, 0});
            name->GSetHeight(0.1);
            name->GSetWidth(1);
            GAttach(name);

            last_point = {0, 0};

            GSetDragBeginCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
                GUpdateGlobalPosition(nullptr);
                drag_from = v - global.c;
                return this;
            });

            GSetDragHandlerCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
                if (drag_from.y < global.s.y * 0.1f) GPlace({(v - drag_from).x / global.s.x, (v - drag_from).y / global.s.y});
                last_point = v;
                return this;
            });

            GSetDragEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
                if ((drag_from.y < global.s.y * 0.1f) && (last_point.y > global.s.y * 0.5f)){
                    GEngine::getGEngine().unfocusTo(this);
                    GPlace({0, 0});
                    return nullptr;
                }
                GPlace({0, 0});
                return this;
            });
        }

        virtual GObject *GFindFocusObject(const vecmath::Vec2 &point, std::list<GObject *> * trace) {
            for (auto const &gr : Graphics) {
                if (gr == name) continue;
                auto fo = gr->GFindFocusObject(point, trace);
                if (fo) {
                    trace->push_front(this);
                    return fo;
                }
            }
            if (visible && this->GContains(point)) {
                trace->push_front(this);
                return this;
            }
            return nullptr;
        }

//        virtual GObject *GTapEnd(const vecmath::Vec2 &v) override {
//            if (close->GContains(v)) {
//                GEngine::getGEngine().unfocusTo(this);
//                return nullptr;
//            }
//            return this;
//        }

        void GRender(NVGcontext *nvg, float dTime) {

        }

    };

}

#endif //PD_FULLSCREENWINDOW_H
