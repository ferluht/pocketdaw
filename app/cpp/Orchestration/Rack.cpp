//
// Created by Admin on 21.09.2019.
//

#include "Rack.h"

bool AMGRack::ARender(double beat, float *lsample, float *rsample){
//    MEffects.MRender(beat);
//    Instr->MRender(beat);
//    Instr->ARender(beat, lsample, rsample);
//    AEffects.MRender(beat);
//    AEffects.ARender(beat, lsample, rsample);
    for (auto const& obj : objects) obj->ARender(beat, lsample, rsample);
    return true;
}

//GUI::GObject * AMGRack::GDragHandler(const ndk_helper::Vec2 &v) {
//    x_offset += (v.x - drag_from.x)/shape->global.s.x;
//    updatePositions();
////    if ((MEffects.shape->local.ratio + Instr->shape->local.ratio + AEffects.shape->local.ratio)/shape->global.ratio + x_offset + 2*padding < 1)
////        x_offset = 1 - (MEffects.shape->local.ratio + Instr->shape->local.ratio + AEffects.shape->local.ratio)/shape->global.ratio - 2*padding;
////    if (x_offset > 0) x_offset = 0;
////    changed = true;
//
//    return this;
//}
//
//GUI::GObject * AMGRack::GDragBegin(const ndk_helper::Vec2 &v) {
//    drag_from = v;
//    return this;
//}