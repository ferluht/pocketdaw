//
// Created by Admin on 31.07.2019.
//

#ifndef PD_MENU_H
#define PD_MENU_H

#include <functional>
#include "Canvas.h"
#include "Text.h"
#include "Knob.h"

class Menu;

struct MenuItem {

    Menu * menu;
    std::function<void(void)> callback;
    int centering;

};

class Menu : public Knob {

    float item_height = 0.15;

public:

    int focus;
    unsigned int size;

    bool unfold;

    GCanvas * unfold_background;
    GCanvas * cursor;

    Text * label;

    ndk_helper::Vec2 last_touch;

    std::vector<std::pair<Text *, MenuItem *>> items;

    Menu(wchar_t * label_);

    void addItem(const wchar_t * text_, std::function<void(void)> callback_);
    void addSubmenu(const wchar_t * text_, Menu * menu);

    void MIn(MData cmd) override ;

//    GObject * GDragBegin(const ndk_helper::Vec2& v) override;
    GObject * GDragHandler(const ndk_helper::Vec2& v) override;
    GObject * GDragEnd(const ndk_helper::Vec2& v) override;

//    GObject * GTapEnd(const ndk_helper::Vec2& v) override;

    void GGainFocus() override;
    void GLoseFocus() override;

    void GSetVisible(bool visible_) override {
        Knob::GSetVisible(visible_);
        unfold_background->GSetVisible(unfold);
    }
};


#endif //PD_MENU_H
