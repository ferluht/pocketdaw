//
// Created by ibelikov on 11.07.19.
//

#include <GUI/Encoder.h>
#include "Waveform.h"
#include "../GUI/Text.h"

Waveform::Waveform(float n) {

    window = n;

    setRatio(0.5);
//    GAttach(new Text("Fonts/Roboto-Regular.ttf", L"waveform", 0.05, 0.05, 2, 0.07));

    auto enc = new Encoder(L"scale", 0, [this, n](float value){
        this->window = (value/2 + 0.5f) * n + 1;
    });
    enc->place(0.02, 0.65);
    enc->setHeight(0.25);
    GAttach(enc);
    MConnect(enc);

    graph = new TimeGraph(n);
    graph->place(0.05, 0.05);
    graph->setHeight(0.9);
    graph->setWidth(0.9);
    GAttach(graph);
}

void Waveform::ARender(double beat, float *lsample, float *rsample) {
    if (ai < window){
        accumulator += *rsample;
        ai ++;
    } else {
        ai = 0;
        graph->update(accumulator / window);
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