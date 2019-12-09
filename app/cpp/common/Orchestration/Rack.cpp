//
// Created by Admin on 21.09.2019.
//

#include "Rack.h"

bool AMGRack::ARender(double beat, float *lsample, float *rsample){
//    MEffects.MRender(beat);
//    Instr->MRender(beat);
    Instr->ARender(beat, lsample, rsample);
//    AEffects.MRender(beat);
    AEffects.ARender(beat, lsample, rsample);
    return true;
}