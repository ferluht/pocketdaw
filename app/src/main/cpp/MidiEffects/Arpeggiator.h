//
// Created by Admin on 13.06.2019.
//

#ifndef PD_ARPEGGIATOR_H
#define PD_ARPEGGIATOR_H

#include <map>
#include <random>
#include <GUI/Canvas.h>

class Arpeggiator : public AMGCanvas{

public:

    double scale;

    int cycles;

    std::map<unsigned char, MData> notes;
    unsigned char last_played_note;

    Arpeggiator();
    Arpeggiator(double scale_);

    void MIn(MData cmd) override ;
    void MRender(double beat) override ;
};


#endif //PD_ARPEGGIATOR_H
