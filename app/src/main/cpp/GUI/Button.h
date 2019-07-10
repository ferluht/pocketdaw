//
// Created by Admin on 06.07.2019.
//

#ifndef PD_BUTTON_H
#define PD_BUTTON_H

#include "GraphicObject.h"

class Button : public GraphicObject{
public:
    float * parameter_;
    std::function<void(void)> callback_;

    Button(const char * texture, float x, float y, std::function<void(void)> callback);

    void SetPosition(float x, float y);
    void SetSize(float x, float y);
    void grender(float dTime) override ;
    void dragHandler(ndk_helper::Vec2 v) override ;
    void dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) override ;
    void dragEnd() override ;
};


#endif //PD_BUTTON_H
