//
// Created by Admin on 15.09.2019.
//

#ifndef PD_IECANVAS_H
#define PD_IECANVAS_H

#include "Button.h"
#include "Canvas.h"

namespace GUI {

    class Header : public GCanvas {

        char * label;

        const float onoff_button_padding = 0.01;
        const float onoff_button_height = 0.9;
        const float onoff_button_ratio = 3.2;
        const float name_height = 0.9;

    public:

        Button *isOn;

        Header(const char * label_) {
            setColor(GREY);
            size_t len = strlen(label_);
            label = new char[len + 1];
            strncpy(label, label_, len);
            label[len] = 0;

            isOn = new Button("ON/OFF", [](bool state) {});
            isOn->shape->lPlace({onoff_button_padding, (1-onoff_button_height)/2});
            isOn->shape->lSetHeight(onoff_button_height);
            isOn->shape->setRatio(onoff_button_ratio);
            GAttach(isOn);
            *isOn = true;
        }

        void GDraw(NVGcontext *nvg) override {
            GCanvas::GDraw(nvg);

            nvgBeginPath(nvg);
            nvgFontSize(nvg, shape->global.s.y * name_height);
            nvgFontFace(nvg, "sans");
            nvgTextAlign(nvg,NVG_ALIGN_MIDDLE|NVG_ALIGN_MIDDLE);

            nvgFillColor(nvg, BLACK);
            nvgText(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + shape->global.s.y/2, label, NULL);
            nvgClosePath(nvg);
        };

        GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override {
            auto fo = isOn->GFindFocusObject(point, trace);
            if (fo) {
                trace->push_front(this);
                return fo;
            }
            if (visible && shape->contains(point)) {
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
            header->shape->lPlace({0, 0});
            header->shape->lSetHeight(header_height);
            header->shape->lSetWidth(1);
            AMGCanvas::GAttach(header);

            body = new GObject();
            body->setShapeType(BOX);
            body->shape->lPlace({0, header_height});
            body->shape->lSetHeight(0.9);
            body->shape->lSetWidth(1);
            AMGCanvas::GAttach(body);

            attach_to_body = true;
        }

        inline bool enabled() {
            return *header->isOn;
        }

        void NoHeader() {
            header->visible = false;
            no_header = true;
            body->shape->lPlace({0, 0});
            body->shape->lSetHeight(1);
            body->shape->lSetWidth(1);
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
                    shape->global.c.x,
                    shape->global.c.y,
                    shape->global.s.x, shape->global.s.y);
            nvgFillColor(nvg, DARK);
            nvgFill(nvg);
            nvgStrokeColor(nvg, BLACK);
            nvgStroke(nvg);
            nvgClosePath(nvg);
        };
    };

}

#endif //PD_IECANVAS_H
