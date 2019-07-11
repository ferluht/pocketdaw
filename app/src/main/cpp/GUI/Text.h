//
// Created by ibelikov on 11.07.19.
//

#ifndef PD_TEXT_H
#define PD_TEXT_H


#include "GraphicObject.h"
#include "Font.h"

class Text : public GraphicObject{
public:

    Font * f;
    float ratio;
    const char * font_file;
    wchar_t * text_;

    Text(const char * font, wchar_t * text, float x, float y);

    void SetPosition(float x, float y);
    void SetSize(float x, float y);

    void Init_() override;
    void grender(float dTime) override ;
    void dragHandler(ndk_helper::Vec2 v) override ;
    void dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) override ;
    void dragEnd() override ;
};


#endif //PD_TEXT_H
