//
// Created by ibelikov on 11.07.19.
//

#include <GUI/Encoder.h>
#include "Waveform.h"
#include "../GUI/Text.h"

Waveform::Waveform(float n, float x_, float y_) : AudioEffect(x_,y_,0,0){

    window = n;

    attach(new Text("Fonts/Roboto-Regular.ttf", L"waveform", 0.05, 0.05, 2, 0.07));

    attach(new Encoder(0, 0.05, 0.7, 2, 0.25, "Textures/encoder.bmp",
                       [this](float state){
                           this->window = (state/2 + 0.5f) * 200 + 1;
                       }));

    graph = new TimeGraph(300, 0.05, 0.5, 3, 0.9, 0.9);
    attach(graph);
}

void Waveform::apply(float *lsample, float *rsample) {
    if (ai < window){
        accumulator += *rsample;
        ai ++;
    } else {
        ai = 0;
        graph->update(accumulator / sqrt(window) / 2);
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