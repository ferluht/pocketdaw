//
// Created by ibelikov on 11.07.19.
//

#ifndef PD_FONT_H
#define PD_FONT_H

#include <stdint.h>
#include <ft2build.h>
#include <string>
#include <vector>
#include <NDKHelper.h>
#include <wchar.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

class Font {
public:
    Font ( ) { }
    /* инициализировать библиотеку freetype и загрузить ttf файл. */
    Font ( const char *ttf_file );
    /* задать позицию на экране */
    /* здесь происходит создание текстуры. Вот параметры
     *\1 сам текст в широких символах.
     *\2 размер шрифта.
     *\3 расстояние между шрифтами по горизонтали в пикселях.
     *\4 расстояние между шрифтами по вертикали в пикселях.
     *\5 размер пробела в пикселях.
     *\6 компонент цвет красный.
     *\7 компонент цвет зеленый.
     *\8 компонент цвет синий.
     * ну это значит что можно задать любой цвет тексту */
    float init ( wchar_t *text, int fontSize, int align, int valign, int space, uint8_t r, uint8_t g, uint8_t b );
    /* задать размер текстуры */
    /* рисовать текстуру */

    GLuint textureid;
private:
    FT_Face face = 0;
    /* здесь текстурные координаты */
    /* это размер текстуры : ширина */
    int width;
    /* это размер текстуры : высота */
    int height;
    /* это для шейдера надо */
    /* id текстуры */
    /* координата x */
    /* это замена функции glOrtho */
    /* это для перемещения на экране */
     /* шейдерная программа */
    FT_Library ft_library;
};

#endif //PD_FONT_H
