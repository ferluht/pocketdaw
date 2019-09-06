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

    float maxWidth, maxRatio;

    FT_Face face = 0;

    int twidth;
    int theight;

    FT_Library ft_library;

    Text(const char * font, const wchar_t * text_);

    void generateTexture();

    void setMaxWidth(float maxWidth_);

    void GInit() override;
};


#endif //PD_TEXT_H
