//
// Created by Admin on 12.06.2019.
//

#ifndef PD_METRONOME_H
#define PD_METRONOME_H

#include "Oscillator.h"

class Metronome : public Oscillator{

public:

    Metronome() : Oscillator("metr", 1) {
        *A = 0.05;
        *D = 0.1;
        *S = 0.02;
        *R = 0.02;
        *level = 0.8;
    }

    void tic(double beat){
        MData md;
        md.status = 0x80;
        md.data1 = 80;
        md.data2 = 100;
        md.beat = beat;
        MIn(md);
    }

    void tac(double beat){
        MData md;
        md.status = 0x80;
        md.data1 = 85;
        md.data2 = 100;
        md.beat = beat;
        MIn(md);
    }

};


#endif //PD_METRONOME_H
