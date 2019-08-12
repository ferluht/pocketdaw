//
// Created by Admin on 31.07.2019.
//

#ifndef PD_MENU_H
#define PD_MENU_H

#include "Canvas.h"
//#include <Orchestration/Midi.h>
//#include "Basic/Basic.h"

class Menu : public Canvas, MObject {

public:

//    MGObject * focusObject;
    unsigned int focus;
    Canvas * cursor;
    std::vector<std::pair<wchar_t *, std::function<void(void)>>> items;

    Menu(std::vector<std::pair<wchar_t *, std::function<void(void)>>> items_);

    void MIn(MData cmd) override ;

};


#endif //PD_MENU_H
