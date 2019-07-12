//
// Created by Admin on 12.06.2019.
//

#ifndef PD_AUDIOEFFECT_H
#define PD_AUDIOEFFECT_H

#include "../GUI/GraphicObject.h"

class AudioEffect : public GraphicObject{

public:

    AudioEffect();
    AudioEffect(const char * tex, const char * vsh, const char * fsh);

    void grender(float dTime) override ;

    virtual float apply(float sample);
};


#endif //PD_AUDIOEFFECT_H
