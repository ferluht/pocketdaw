//
// Created by Admin on 13.06.2019.
//

#ifndef PD_ARPEGGIATOR_H
#define PD_ARPEGGIATOR_H

#include <map>
#include <random>
#include <GUI/IECanvas.h>

class Arpeggiator : public IECanvas{

public:

    double scale;

    float rate;
    float gate;

    int cycles;

    bool isplaying;

    double last_played_beat;

    std::map<unsigned char, MData> notes;
    unsigned char last_played_note;

    Arpeggiator();
    Arpeggiator(double scale_);

    void MIn(MData cmd) override ;
    void MRender(double beat) override ;
};


#endif //PD_ARPEGGIATOR_H
