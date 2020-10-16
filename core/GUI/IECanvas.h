//
// Created by Admin on 15.09.2019.
//

#ifndef PD_IECANVAS_H
#define PD_IECANVAS_H

#include "Button.h"
#include "Canvas.h"

namespace GUI {

    class IEHeader : public Button {

    public:

        IEHeader(const char * label_) : Button(label_, [](bool state) {}){
            state = true;
        }

    };

    static auto isIEHeader = GUI::IsType<IEHeader>;

    class IECanvas : public AMGCanvas {

        const float header_height = 0.08;

        GObject *body;
        IEHeader *header;
        bool attach_to_body = false;

        bool no_header = false;

    public:


        IECanvas(const char *name_) {

            header = new IEHeader(name_);
            header->GPlace({0, 0});
            header->GSetHeight(header_height);
            header->GSetWidth(1);
            AMGCanvas::GAttach(header);

            body = new GObject(BOX);
            body->GPlace({0, header_height});
            body->GSetHeight(0.9);
            body->GSetWidth(1);
            AMGCanvas::GAttach(body);

            attach_to_body = true;
        }

        inline bool enabled() {
            return *header;
        }

        void NoHeader() {
            header->visible = false;
            no_header = true;
            body->GPlace({0, 0});
            body->GSetHeight(1);
            body->GSetWidth(1);
        }

        void GAttach(GObject * go) override {
            body->GAttach(go);
        }

        void GDetach(GObject * go) override {
            body->GDetach(go);
        }

        void GSetVisible(bool visible_) override {
            if (no_header) {
                body->GSetVisible(visible_);
            } else {
                AMGCanvas::GSetVisible(visible_);
            }
            visible = visible_;
        }

        void GDraw(NVGcontext *nvg) override {
            nvgBeginPath(nvg);
            nvgRect(nvg,
                    global.c.x,
                    global.c.y,
                    global.s.x, global.s.y);
            nvgFillColor(nvg, GEngine::ui_theme->IECANVAS_BODY_COLOR);
            nvgFill(nvg);
            nvgStrokeColor(nvg, BLACK);
            nvgStroke(nvg);
            nvgClosePath(nvg);
        };

        virtual int BRender(uint8_t * data) {
            return 0;
        }
    };

    static auto isIECanvas = GUI::IsType<IECanvas>;

}

#endif //PD_IECANVAS_H
