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
    focus = 0;

    unfold = false;
}

void Menu::addItem(const wchar_t * text_, std::function<void(void)> callback_) {

    for (auto item = items.begin(); item != items.end(); ++item){
        if (wcscmp(item->first->text, text_) == 0) {
            item->second = callback_;
            return;
        }
    }

    auto txt = new Text("Fonts/Roboto-Regular.ttf", text_);
    txt->place(0.1, 0.05 + size*item_height);
    txt->setHeight(item_height);
    txt->setMaxWidth(0.8);
    items.push_back({txt, callback_});
    unfold_background->GAttach(txt);
    unfold_background->changed = true;
    size ++;
}

GObject * Menu::GDragHandler(const ndk_helper::Vec2 &v){

    if (unfold_background->globalPosition.contains(v)) {
        focus = (int)((abs(v.y_ - unfold_background->globalPosition.y) - 0.05*unfold_background->globalPosition.height)/item_height/unfold_background->globalPosition.height);
        cursor->place(0.05, 0.05 + focus * item_height);
    }

    last_touch = v;

    return this;
}

GObject * Menu::GDragEnd(const ndk_helper::Vec2 &v) {

    if (cursor->globalPosition.contains(last_touch)) {
        items[focus].second();
    }

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
        switch (cmd.data1){
            case 0x01:
                focus = (float)cmd.data2/127.f*((float)items.size()-1);
                cursor->place(0.05, 0.05 + focus * 0.08);
                break;
            case 0x017:
                items[focus].second();
                parent->GSetVisible(true);
                parent->focusObject = nullptr;
                GSetVisible(false);
                break;
            default:
                break;
        }
    }

//    MidiQueue.push(md);
}

