//
// Created by Admin on 15.09.2019.
//

#ifndef PD_IECANVAS_H
#define PD_IECANVAS_H

#include "Button.h"
#include "Canvas.h"

namespace GUI {

    class Header : public AMGCanvas {

        char * label;

        const float onoff_button_padding = 0.01;
        const float onoff_button_height = 0.9;
        const float onoff_button_ratio = 2;
        const float name_height = 0.5;

    public:

        Button *isOn;

        Header(const char * label_) {
            setColor(GREY);
            size_t len = strlen(label_);
            label = new char[len + 1];
            strncpy(label, label_, len);
            label[len] = 0;

            isOn = new Button("ON", "OFF", [](bool state) {});
            isOn->GPlace({onoff_button_padding, (1 - onoff_button_height) / 2});
            isOn->GSetHeight(onoff_button_height);
            isOn->GSetRatio(onoff_button_ratio);
            GAttach(isOn);
            *isOn = true;
        }

        void GDraw(NVGcontext *nvg) override {
            AMGCanvas::GDraw(nvg);

            nvgBeginPath(nvg);
            nvgFontSize(nvg, global.s.y * name_height);
            nvgFontFace(nvg, "sans");
            nvgTextAlign(nvg,NVG_ALIGN_MIDDLE|NVG_ALIGN_MIDDLE);

            nvgFillColor(nvg, BLACK);
            nvgText(nvg, global.c.x + global.s.x/2, global.c.y + global.s.y/2, label, NULL);
            nvgClosePath(nvg);
        };

        GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override {
            auto fo = isOn->GFindFocusObject(point, trace);
            if (fo) {
                trace->push_front(this);
                return fo;
            }
            if (visible && GContains(point)) {
                return this;
            }

            return nullptr;
        }

    };

    class IECanvas : public AMGCanvas {

        const float header_height = 0.08;

        GObject *body;
        Header *header;
        bool attach_to_body = false;

        bool no_header = false;

    public:


        IECanvas(const char *name_) {

            header = new Header(name_);
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
            return *header->isOn;
        }

        void NoHeader() {
            header->visible = false;
            no_header = true;
            body->GPlace({0, 0});
            body->GSetHeight(1);
            body->GSetWidth(1);
        }

        GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override {
            auto fo = body->GFindFocusObject(point, trace);
            if (fo) {
                trace->push_front(this);
                return fo;
            }
            fo = header->GFindFocusObject(point, trace);
            if (fo == header) {
                return this;
            }
            return nullptr;
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
            nvgFillColor(nvg, DARK);
            nvgFill(nvg);
            nvgStrokeColor(nvg, BLACK);
            nvgStroke(nvg);
            nvgClosePath(nvg);
        };
    };

}

#endif //PD_IECANVAS_H
