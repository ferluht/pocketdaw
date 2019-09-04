//
// Created by Admin on 31.07.2019.
//

#ifndef PD_MENU_H
#define PD_MENU_H

#include "Canvas.h"
#include "Text.h"

class Menu : public MGCanvas {

public:

//    MGObject * focusObject;
    unsigned int focus;
    unsigned int size;
    GCanvas * cursor;
    std::vector<std::pair<wchar_t *, std::function<void(void)>>> items;

    Menu(std::vector<std::pair<wchar_t *, std::function<void(void)>>> items_);

    void MIn(MData cmd) override ;

    void GTapEnd() override {
        auto midi = &MEngine::getMEngine();
        auto mnames = midi->getDevices();
        for (auto const& name : mnames) {
            wchar_t* wide_string = new wchar_t[ name.length() + 1 ];
            std::copy( name.begin(), name.end(), wide_string );
            wide_string[ name.length() ] = 0;

            items.push_back({wide_string, [midi, name](){midi->connectDevice(name);}});
            changed = true;

            midi->connectDevice(name);
//            auto txt = new Text("Fonts/Roboto-Regular.ttf", items.back().first);
//            txt->place(0.05, 0.05 + size*0.08, 0.2, 0.07);
//            GAttach(txt);
//            size++;

            delete [] wide_string;
        }
    }

};


#endif //PD_MENU_H
