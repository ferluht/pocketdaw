//
// Created by Admin on 31.07.2019.
//

#include "Menu.h"
#include "Text.h"
#include <Utils/Utils.h>

Menu::Menu(wchar_t * label_) {

    GAttachTexture("Textures/effect_canvas.bmp");
    GSaveRatio(true);

    int i = 0;

    label = new Text("Fonts/Roboto-Regular.ttf", label_);
    label->place(0.05, 0.05);
    label->setHeight(0.7);
//    label->setMaxWidth(0.9);
    GAttach(label);

    unfold_background = new GCanvas();
    unfold_background->place(0, 1);
    unfold_background->setWidth(1);
    unfold_background->setRatio(1);
    unfold_background->GAttachTexture("Textures/effect_canvas.bmp");
    GAttach(unfold_background);
    unfold_background->GSetVisible(false);

    cursor = new GCanvas();
    cursor->place(0.05, 0.05);
    cursor->setHeight(item_height);
    cursor->setWidth(0.9);
    cursor->GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
    cursor->GSetColor(1, 0, 0, 0.2);
    unfold_background->GAttach(cursor);

    size = 0;
    focus = -1;

    unfold = false;
}

void Menu::addItem(const wchar_t * text_, std::function<void(void)> callback_) {

    for (auto item = items.begin(); item != items.end(); ++item){
        if (wcscmp(item->first->text, text_) == 0) {
            //delete item->second->callback;
            item->second->callback = callback_;
            if (item->second->menu){
                delete item->second->menu;
                item->second->menu = nullptr;
            }
            return;
        }
    }

    auto txt = new Text("Fonts/Roboto-Regular.ttf", text_);
    txt->place(0.1, 0.05 + size*item_height);
    txt->setHeight(item_height);
    txt->setMaxWidth(0.8);

    auto item = new MenuItem();
    item->callback = callback_;

    items.push_back({txt, item});
    unfold_background->GAttach(txt);
    unfold_background->changed = true;
    size ++;
}

void Menu::addSubmenu(const wchar_t * text_, Menu * menu) {

    for (auto item = items.begin(); item != items.end(); ++item){
        if (wcscmp(item->first->text, text_) == 0) {
            if (item->second->menu){
                delete item->second->menu;
                item->second->menu = nullptr;
            }
            item->second->menu = menu;
            return;
        }
    }

    auto txt = new Text("Fonts/Roboto-Regular.ttf", text_);
    txt->place(0.1, 0.05 + size*item_height);
    txt->setHeight(item_height);
    txt->setMaxWidth(0.8);

    auto item = new MenuItem();
    item->menu = menu;

    items.push_back({txt, item});
    unfold_background->GAttach(txt);
    unfold_background->changed = true;
    size ++;
}

GObject * Menu::GDragHandler(const ndk_helper::Vec2 &v){

    if (unfold_background->globalPosition.contains(v)) {
        int new_focus = (int)((abs(v.y_ - unfold_background->globalPosition.y) - 0.05*unfold_background->globalPosition.height)/item_height/unfold_background->globalPosition.height);
        if (new_focus > items.size() - 1) new_focus = -1;
        if (new_focus != focus) {
            if ((new_focus > -1) && items[new_focus].second->menu) {
                unfold_background->GAttach(items[new_focus].second->menu);
                items[new_focus].second->menu->place(1, 0.05 + new_focus * item_height);
                items[new_focus].second->menu->setHeight(item_height);
                items[new_focus].second->menu->setRatio(globalPosition.ratio);
                items[new_focus].second->menu->GGainFocus();
            }
            if ((focus > -1) && items[focus].second->menu) {
                unfold_background->GDetach(items[focus].second->menu);
                items[focus].second->menu->GLoseFocus();
            }
        }
        focus = new_focus;
        if (focus > -1) {
            cursor->place(0.05, 0.05 + focus * item_height);
            cursor->GSetVisible(true);
        }
        else cursor->GSetVisible(false);
    } else if (focus > -1) {
        if (items[focus].second->menu) {
            items[focus].second->menu->GDragHandler(v);
        }
    }

    last_touch = v;

    return this;
}

GObject * Menu::GDragEnd(const ndk_helper::Vec2 &v) {

    if (cursor->globalPosition.contains(last_touch)) {
        if (items[focus].second->menu == nullptr)
            items[focus].second->callback();
    } else {
        if ((focus > -1) && items[focus].second->menu) {
            items[focus].second->menu->GDragEnd(v);
        }
    }

    focus = -1;

    return nullptr;
}

//GObject * Menu::GTapEnd(const ndk_helper::Vec2& v) {
//    auto midi = &MEngine::getMEngine();
//    auto mnames = midi->getDevices();
//    for (auto const& name : mnames) {
//        std::wstring wide_string = utils::UTF8toUnicode(name);
//        addItem(wide_string.c_str(), [midi, name](){midi->connectDevice(name);});
//    }
//
//    return this;
//}

void Menu::GLoseFocus() {
    unfold = false;
    unfold_background->GSetVisible(unfold);
    if ((focus > -1) && items[focus].second->menu) {
        unfold_background->GDetach(items[focus].second->menu);
        items[focus].second->menu->GLoseFocus();
    }
}

void Menu::GGainFocus(){

    auto midi = &MEngine::getMEngine();
    auto mnames = midi->getDevices();
    for (auto const& name : mnames) {
        std::wstring wide_string = utils::UTF8toUnicode(name);
        addItem(wide_string.c_str(), [midi, name](){midi->connectDevice(name);});
    }

    unfold = true;
    unfold_background->GSetVisible(unfold);
}

void Menu::MIn(MData cmd)
{
    if (cmd.status == 0xb0){
        auto geng = &GEngine::getGEngine();
        switch (cmd.data1){
            case 0x01:
                focus = (float)cmd.data2/127.f*((float)items.size()-1);
                cursor->place(0.05, 0.05 + focus * 0.08);
                break;
            case 0x0A:
                geng->focusOn(this);
//                items[focus].second();
//                parent->GSetVisible(true);
//                parent->focusObject = nullptr;
//                GSetVisible(false);
                break;
            default:
                break;
        }
    }

//    MidiQueue.push(md);
}

