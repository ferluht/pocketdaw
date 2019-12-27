//
// Created by ibelikov on 11.12.19.
//

#ifndef PD_MIDIEFFECT_H
#define PD_MIDIEFFECT_H


#include <GUI/IECanvas.h>

class MidiEffect : public GUI::IECanvas{

public:

    MidiEffect(const char * name_) : GUI::IECanvas(name_){

    }

};

static auto isMidiEffect = GUI::IsType<MidiEffect>;

#endif //PD_MIDIEFFECT_H
