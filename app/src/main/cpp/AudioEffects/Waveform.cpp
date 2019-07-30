//
// Created by ibelikov on 11.07.19.
//

#include <GUI/Encoder.h>
#include "Waveform.h"
#include "../GUI/Text.h"

Waveform::Waveform(float n, float x_, float y_) : AudioEffect(x_,y_,0,0){

    window = n;
    graph = new SimpleGraph(300, 0.05, 0.5, 3, 0.9, 0.9);
    attach(graph);

    attach(new Encoder(0, 0.02, 0.8, 2, 0.15, "Textures/encoder.bmp",
                       [this](float state){
                           this->window = (state/2 + 0.5f) * 200 + 1;
                       }));
}

void Waveform::apply(float *lsample, float *rsample) {
    if (ai < window){
        accumulator += *rsample;
        ai ++;
    } else {
        ai = 0;
        graph->update(accumulator / sqrt(window) / 2);
        prev_acc = accumulator;
        accumulator = 0;
    }
}

//void Waveform::dragHandler(ndk_helper::Vec2 v) {
////    relativePosition.x = (v.x_ - drag_from.x_)*drag_xscale + relative_position_backup.x;
////    relativePosition.y = (v.y_ - drag_from.y_)*drag_yscale + relative_position_backup.y;
////
////    Update();
//}
//
//void Waveform::dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) {
////    drag_from = v;
////    relative_position_backup = relativePosition;
////    drag_xscale = xscale;
////    drag_yscale = yscale;
//}
//
//void Waveform::dragEnd() {}