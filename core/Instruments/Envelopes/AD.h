//
// Created by ibelikov on 12.12.19.
//

#ifndef PD_AD_H
#define PD_AD_H


#include <A.h>

class AD : public AObject{

    double attack_beat;

    float attack_level;

    float level;

public:

    float A, D;

    AD() {
        level = 0;
    }

    inline void attack(double beat) {
        attack_beat = beat;
        attack_level = level;
    }

    bool ARender(double beat, float * lsample, float * rsample) override ;
};


#endif //PD_AD_H
