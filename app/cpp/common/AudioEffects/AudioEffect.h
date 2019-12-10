//
// Created by Admin on 12.06.2019.
//

#ifndef PD_AUDIOEFFECT_H
#define PD_AUDIOEFFECT_H

#include <GUI/Button.h>
#include "GUI/IECanvas.h"

class AudioEffect : public GUI::IECanvas{

public:

    AudioEffect(const char * name_) : GUI::IECanvas(name_){

    }
};


#endif //PD_AUDIOEFFECT_H
