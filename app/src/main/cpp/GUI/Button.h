//
// Created by Admin on 06.07.2019.
//

#ifndef PD_BUTTON_H
#define PD_BUTTON_H

#include "GraphicObject.h"

class Button : public GraphicObject{
public:
    float * parameter_;

    Button(const char * texture, float x, float y, float * parameter);

    void SetPosition(float x, float y);
    void SetSize(float x, float y);
    void grender(float dTime) override ;
    void dragHandler(ndk_helper::Vec2 v) override ;
};


#endif //PD_BUTTON_H
