//
// Created by Admin on 13.06.2019.
//

#ifndef PD_ADSR_H
#define PD_ADSR_H


class ADSR {

public:
    int A, D, R;
    float S;
    int position;
    float amp;
    float amp_inc;

    ADSR();
    float apply(float sample);
    void attack();
    void release();
};


#endif //PD_ADSR_H
