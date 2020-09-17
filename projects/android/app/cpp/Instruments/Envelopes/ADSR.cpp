//
// Created by Admin on 13.06.2019.
//

#include "ADSR.h"
#include <math.h>

bool ADSR::ARender(double beat, float *lsample, float *rsample) {
    float old_level = level;
    if (release_beat > 0){
        auto time = (float)(beat - release_beat);
        level = release_level * (1 - time / R);
        if (level < 0) {
            level = 0;
            *lsample = 0;
            *rsample = 0;
            return false;
        }
    } else {
        auto time = (float)(beat - attack_beat + attack_level*A);
        if (time < A) {
            level = time / A;
        } else if (time < A + D) {
            level = (A + D - time) / D + S * (time - A) / D;
        } else {
            level = S;
        }
    }
    *lsample *= level;
    *rsample *= level;
    return true;
}