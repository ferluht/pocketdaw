//
// Created by Admin on 15.09.2019.
//

#ifndef PD_IECANVAS_H
#define PD_IECANVAS_H

#include "Button.h"

class IECanvas : public AMGCanvas{

    const float onoff_button_padding = 0.02;
    const float onoff_button_height = 0.7;
    const float onoff_button_ratio = 3.2;
    const float name_padding = 0.01;
    const float name_height = 0.5;
    const float header_height = 0.1;

    GObject * body;
    bool attach_to_body = false;

    GCanvas * header;

public:

    Button * isOn;

    IECanvas(const wchar_t * name_){

        header = new GCanvas();
        header->GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
        header->GSetColor(0.8, 0.8, 1, 0.2);
        header->place(0, 0);
        header->setHeight(header_height);
        header->setWidth(1);
        GAttach(header);

        auto name = new Text("Fonts/Roboto-Regular.ttf", name_);
        name->place(name_padding, (1-name_height)/2);
        name->setHeight(name_height);
        header->GAttach(name);

        GAttachTexture("Textures/effect_canvas.bmp");
        isOn = new Button(L"ON/OFF", [](bool state){});
        isOn->setHeight(onoff_button_height);
        isOn->setRatio(onoff_button_ratio);
        header->GAttach(isOn);

        body = new GObject();
        body->place(0, header_height);
        body->setHeight(0.9);
        body->setWidth(1);
        GAttach(body);

        attach_to_body = true;

        *isOn = true;
    }

    void NoHeader(){
        header->visible = false;
        body->place(0, 0);
        body->setHeight(1);
        body->setWidth(1);
    }

    void GAttach(GObject * go) override
    {
        if (attach_to_body) {
            body->GAttach(go);
        } else {
            AMGCanvas::GAttach(go);
        }
    }

    void GInit() override
    {
        isOn->place(1 - onoff_button_padding - onoff_button_height*onoff_button_ratio/ratio*header_height, (1 - onoff_button_height)/2);
    }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override
    {
        if (visible && body->globalPosition.contains(point)){
            for (auto const &gr : body->Graphics) {
                auto fo = gr->GFindFocusObject(point);
                if (fo) return fo;
            }
            return this->parent;
        }

        if (visible && header->globalPosition.contains(point)){
            if (isOn->globalPosition.contains(point)) return isOn;
            return this;
        }

        return nullptr;
    }
};


#endif //PD_IECANVAS_H
