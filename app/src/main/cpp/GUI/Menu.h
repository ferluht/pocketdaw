//
// Created by Admin on 31.07.2019.
//

#ifndef PD_MENU_H
#define PD_MENU_H

#include "Canvas.h"
#include "Text.h"

class Menu : public MGCanvas {

public:

    unsigned int focus;

    GCanvas * unfold_background;

    unsigned int size;
    GCanvas * cursor;
    std::vector<std::pair<wchar_t *, std::function<void(void)>>> items;

    Menu(std::vector<std::pair<wchar_t *, std::function<void(void)>>> items_);

    void MIn(MData cmd) override ;

    GObject * GTapEnd() override;

    void GGainFocus() override;
    void GLoseFocus() override;
};


#endif //PD_MENU_H
