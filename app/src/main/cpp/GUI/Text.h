//
// Created by ibelikov on 11.07.19.
//

#ifndef PD_TEXT_H
#define PD_TEXT_H


#include "Canvas.h"
#include "Font.h"
#include "NDKHelper.h"

class Text : public Canvas{
public:

    Font * f;
    const char * font_file;
    wchar_t * text_;

    Text(const char * font, wchar_t * text, float x_, float y_, float z_, float h_);

    void init() override;
};


#endif //PD_TEXT_H
