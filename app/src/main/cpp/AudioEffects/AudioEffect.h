//
// Created by Admin on 12.06.2019.
//

#ifndef PD_AUDIOEFFECT_H
#define PD_AUDIOEFFECT_H

#include <GUI/Button.h>
#include "GUI/IECanvas.h"

class AudioEffect : public IECanvas{

public:

    AudioEffect(wchar_t * name_) : IECanvas(name_){

    }
};


#endif //PD_AUDIOEFFECT_H
