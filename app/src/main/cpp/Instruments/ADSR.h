//
// Created by Admin on 13.06.2019.
//

#ifndef PD_ADSR_H
#define PD_ADSR_H


#include <Engine/A.h>

class ADSR : public AObject{

    double attack_beat;
    double release_beat;

    float attack_level;
    float release_level;

    float level;

public:

    float A, D, S, R;

    ADSR() {
        level = 0;
    }

    inline void attack(double beat) {
        attack_beat = beat;
        attack_level = level;
        release_beat = -1;
    }

    inline void release(double beat) {
        release_beat = beat;
        release_level = level;
    }

    bool ARender(double beat, float * lsample, float * rsample) override ;
};


#endif //PD_ADSR_H
