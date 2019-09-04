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
    cursor->place(0.05, 0.05, 0.07, 0.9);
    cursor->GAttachTexture("Textures/effect_canvas.bmp");
    cursor->GSaveRatio(false);
    GAttach(cursor);

    size = 0;
    for (auto const& item : items){
        auto txt = new Text("Fonts/Roboto-Regular.ttf", item.first);
        txt->place(0.05, 0.05 + i*0.08, 2, 0.07);
        GAttach(txt);
        i++;
        size ++;
    }
    focus = 0;
}

void Menu::MIn(MData cmd)
{
    if (cmd.status == 0xb0){
        switch (cmd.data1){
            case 0x01:
                focus = (float)cmd.data2/127.f*((float)items.size()-1);
                cursor->place(0.05, 0.05 + focus * 0.08, 0.07, 0.9);
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

