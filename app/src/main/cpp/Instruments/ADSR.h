//
// Created by Admin on 13.06.2019.
//

#ifndef PD_ADSR_H
#define PD_ADSR_H


#include <Engine/A.h>

class ADSR : public AObject{

public:

    bool released;

    std::function<void(bool)> callback;

    float A, D, S, R;
    int position;
    float amp;
    float ad, ds, sr;

    ADSR();
    void Attack(float A_, float D_, float S_, float R_);
    void Release(std::function<void(bool)> callback_);
    void ARender(double beat, float * lsample, float * rsample) override ;

    operator float() const {return amp;}
};


#endif //PD_ADSR_H
