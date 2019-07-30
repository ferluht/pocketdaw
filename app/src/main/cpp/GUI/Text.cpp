//
// Created by ibelikov on 11.07.19.
//

#include "Text.h"

Text::Text(const char * font, wchar_t * text, float x_, float y_, float z_, float h_)
        : Canvas(x_, y_, z_, h_, h_, 0, nullptr, "Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh", true){

    font_file = font;
    text_ = text;
}

void Text::init(){
    f = new Font(font_file);
    ratio = f->init ( text_, 40, 1, 4, 4, 0, 0, 0 );
    texture = f->textureid;
}