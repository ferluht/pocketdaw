//
// Created by Admin on 12.06.2019.
//

#ifndef PD_METRONOME_H
#define PD_METRONOME_H

#include "Sine.h"

class Metronome : public Sine{

public:

    Metronome() : Sine(L"metr", 1) {
        *enc_level = 0;
        *enc_attack = -1;
        *enc_sustain = -1;
        *enc_decay = -0.98;
    }

    void tic(){
        MData md;
        md.status = 0x80;
        md.data1 = 90;
        md.data2 = 100;
        MIn(md);
    }

    void tac(){
        MData md;
        md.status = 0x80;
        md.data1 = 95;
        md.data2 = 100;
        MIn(md);
    }

};


#endif //PD_METRONOME_H
