//
// Created by ibelikov on 12.12.19.
//

#include "AD.h"

bool AD::ARender(double beat, float *lsample, float *rsample) {

    auto time = (float)(beat - attack_beat + attack_level*A);
    if (time < A) {
        level = time / A;
    } else if (time < A + D) {
        level = (1 - time / D);
    } else {
        level = 0;
        *lsample = 0;
        *rsample = 0;
        return false;
    }

    *lsample *= level;
    *rsample *= level;
    return true;
}