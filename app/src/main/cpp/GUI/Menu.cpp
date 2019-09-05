//
// Created by Admin on 31.07.2019.
//

#include "Menu.h"
#include "Text.h"


Menu::Menu(std::vector<std::pair<wchar_t *, std::function<void(void)>>> items_) {

    GAttachTexture("Textures/effect_canvas.bmp");
    GSaveRatio(true);

    items = items_;
    int i = 0;

    cursor = new MGCanvas();
    cursor->place(0.05, 0.05);
    cursor->setHeight(0.07);
    cursor->setWidth(0.9);
    cursor->GAttachTexture("Textures/effect_canvas.bmp");
    cursor->GSaveRatio(false);
    GAttach(cursor);

    unfold_background = new GCanvas();
    unfold_background->place(0, 1);
    unfold_background->setWidth(1);
    unfold_background->setRatio(1);
    unfold_background->GAttachTexture("Textures/effect_canvas.bmp");
    unfold_background->GSetVisible(false);
    GAttach(unfold_background);

    size = 0;
    for (auto const& item : items){
        auto txt = new Text("Fonts/Roboto-Regular.ttf", item.first);
        txt->place(0.05, 0.05 + i*0.1);
        txt->setHeight(0.1);
        unfold_background->GAttach(txt);
        i++;
        size ++;
    }
    focus = 0;
}

GObject * Menu::GTapEnd() {
    auto midi = &MEngine::getMEngine();
    auto mnames = midi->getDevices();
    for (auto const& name : mnames) {
        wchar_t* wide_string = new wchar_t[ name.length() + 1 ];
        std::copy( name.begin(), name.end(), wide_string );
        wide_string[ name.length() ] = 0;

        items.push_back({wide_string, [midi, name](){midi->connectDevice(name);}});

        midi->connectDevice(name);
        auto txt = new Text("Fonts/Roboto-Regular.ttf", wide_string);
        txt->place(0.05, 0.05 + size*0.1);
        txt->setHeight(0.1);
        unfold_background->GAttach(txt);
        size++;

        unfold_background->changed = true;

        delete [] wide_string;
    }

    return this;
}

void Menu::GLoseFocus() {
    unfold_background->GSetVisible(false);
}

void Menu::GGainFocus(){
    unfold_background->GSetVisible(true);
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

