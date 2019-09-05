//
// Created by ibelikov on 11.07.19.
//

#ifndef PD_TEXT_H
#define PD_TEXT_H


#include "Canvas.h"
#include "NDKHelper.h"
#include <wchar.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

class Text : public GCanvas{
public:

    const char * ttf_file;
    wchar_t * text;

    FT_Face face = 0;

    int width;
    int height;

    FT_Library ft_library;

    Text(const char * font, wchar_t * text_);

    void generateTexture();

    float init ( wchar_t *text, int fontSize, int align, int valign, int space, uint8_t r, uint8_t g, uint8_t b );

    void GInit() override;
};


#endif //PD_TEXT_H
