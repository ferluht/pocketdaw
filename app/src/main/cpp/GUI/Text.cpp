//
// Created by ibelikov on 11.07.19.
//

#include "Text.h"

Text::Text(const char * font, wchar_t * text){
    GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh");
    GSaveRatio(true);
    font_file = font;
    text_ = text;
}

void Text::GInit(){
    f = new Font(font_file);
    ratio = f->init ( text_, 40, 1, 4, 4, 0, 0, 0 );
    texture = f->textureid;
}