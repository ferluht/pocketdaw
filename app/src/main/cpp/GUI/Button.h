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

    Button(float x, float y, float h, float w, const char * texture, std::function<void(bool)> callback_);

    void tapEnd() override ;
};


#endif //PD_BUTTON_H
