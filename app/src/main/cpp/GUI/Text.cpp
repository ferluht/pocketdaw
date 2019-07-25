//
// Created by ibelikov on 11.07.19.
//

#include "Text.h"

Text::Text(const char * font, wchar_t * text, float x, float y)
        : Canvas(x, y, 100, 100, nullptr, "Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh"){

    font_file = font;
    text_ = text;
}

void Text::init(){
    f = new Font(font_file);
    ratio = f->init ( text_, 200, 1, 4, 4, 0, 0, 0 );
    texture = f->textureid;
}