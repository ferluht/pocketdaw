//
// Created by Admin on 31.07.2019.
//

#ifndef PD_MENU_H
#define PD_MENU_H

#include <functional>
#include "Canvas.h"
#include "Button.h"
#include "styles.h"

namespace GUI {

    class Menu;

    struct MenuItem {

        Menu *menu;
        std::function<void(void)> callback;
        int centering;

    };

    class Menu : public AMGCanvas {

        float item_height = 0.15;
        float padding = 0.1;
        float button_height = 0.1;
        float button_width = 0.5;
        float button_spacing = 0.01;

    public:

        int focus;
        unsigned int size;

        std::list<Button *> buttons;

        bool unfold;

        ndk_helper::Vec2 last_touch;

        std::vector<std::pair<char *, MenuItem *>> items;

        Menu(char *label_);

        void setGainCallback(std::function<void(void)> gainCallback_);

        std::function<void(void)> gainCallback;

        void addButton(Button * b) {
            b->GPlace({0.5f - button_width / 2,
                       (buttons.size() + 1) * (button_height + button_spacing)});
            b->GSetHeight(button_height);
            b->GSetWidth(button_width);
            GAttach(b);
            buttons.push_back(b);
        }

        void clear(){
            for (auto const &b : buttons) {
                GDetach(b);
                delete b;
            }
            buttons.clear();
        }

//        void addItem(const char *text_, std::function<void(void)> callback_);
//
//        void addSubmenu(const wchar_t *text_, Menu *menu);
//
//        void MIn(MData cmd) override;

//    GObject * GDragBegin(const ndk_helper::Vec2& v) override;
//        GObject *GDragHandler(const ndk_helper::Vec2 &v) override;
//
//        GObject *GDragEnd(const ndk_helper::Vec2 &v) override;

//    GObject * GTapEnd(const ndk_helper::Vec2& v) override;

//    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override;

        void GGainFocus() override {
            open(true);
            infocus = true;
        }

        void GLoseFocus() override {
            open(false);
            infocus = false;
        }

        void open(bool open_) { GSetVisible(open_); }

        void GDraw(NVGcontext *nvg);
    };

}

#endif //PD_MENU_H
