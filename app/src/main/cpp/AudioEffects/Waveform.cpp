//
// Created by ibelikov on 11.07.19.
//

#include <GUI/Encoder.h>
#include "Waveform.h"
#include "../GUI/Text.h"

Waveform::Waveform(float n) {

    window = n;

//    GAttach(new Text("Fonts/Roboto-Regular.ttf", L"waveform", 0.05, 0.05, 2, 0.07));

    graph = new TimeGraph(300);
    graph->place(0.05, 0.5, 0.9, 0.9);
    GAttach(graph);

    auto enc = new Encoder(L"scale", 0, [this](float value){
        this->window = (value/2 + 0.5f) * 200 + 1;
    });
    enc->place(0, 0.05, 0.7, 0.25);
    GAttach(enc);
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