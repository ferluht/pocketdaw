//
// Created by Admin on 31.07.2019.
//

#include "Menu.h"
#include "Text.h"


Menu::Menu(std::vector<std::pair<wchar_t *, std::function<void(void)>>> items_)
: Canvas(0, 0, 0, 0, "Textures/background.bmp", false){

    items = items_;
    int i = 0;
    cursor = new Canvas(0.05, 0.05, 0.07, 0.9, "Textures/effect_canvas.bmp", false);
    attach(cursor);
    for (auto const& item : items){
        attach(new Text("Fonts/Roboto-Regular.ttf", item.first, 0.05, 0.05 + i*0.08, 2, 0.07));
        i++;
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
                parent->setVisible(true);
                parent->focusObject = nullptr;
                setVisible(false);
                break;
            default:
                break;
        }
    }

//    MidiQueue.push(md);
}