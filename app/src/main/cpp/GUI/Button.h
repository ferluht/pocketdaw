//
// Created by Admin on 06.07.2019.
//

#ifndef PD_BUTTON_H
#define PD_BUTTON_H

#include "Canvas.h"

class Button : public Canvas{
public:

    bool state;
    std::function<void(bool)> callback;

    Button(wchar_t * label, float x, float y, float h, float w, const char * texture, std::function<void(bool)> callback_);

    void tapEnd() override ;

    GObject * findFocusObject(const ndk_helper::Vec2& point) override {return this;}
};


#endif //PD_BUTTON_H
