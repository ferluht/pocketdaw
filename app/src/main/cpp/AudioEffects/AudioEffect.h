//
// Created by Admin on 12.06.2019.
//

#ifndef PD_AUDIOEFFECT_H
#define PD_AUDIOEFFECT_H


class AudioEffect {

public:

    bool isOn;
    AudioEffect() = default;

    virtual float apply(float sample);
};


#endif //PD_AUDIOEFFECT_H
