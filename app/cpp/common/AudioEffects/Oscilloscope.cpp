//
// Created by ibelikov on 11.07.19.
//

#include <GUI/Encoder.h>
#include "Oscilloscope.h"

Oscilloscope::Oscilloscope() : AudioEffect("Oscill")
{

    shape->setRatio(1.5);

    graph = new GUI::TimeGraph(200);
    graph->shape->lPlace({0.01, 0.01});
    graph->shape->lSetHeight(0.7);
    graph->shape->lSetWidth(0.98);
    GAttach(graph);

    trig = new GUI::Encoder("trig", 0, 0, -1, 1);
    trig->shape->lPlace({0.05, 0.72});
    trig->shape->lSetHeight(0.26);
    GAttach(trig);
    MConnect(trig);

    time = new GUI::Encoder("time", 100, 0, 0, 100);
    time->shape->lPlace({0.2, 0.72});
    time->shape->lSetHeight(0.26);
    GAttach(time);
    MConnect(time);

    scale = new GUI::Encoder("scale", 1, 0, 0, 5);
    scale->shape->lPlace({0.35, 0.72});
    scale->shape->lSetHeight(0.26);
    GAttach(scale);
    MConnect(scale);
}

bool Oscilloscope::ARender(double beat, float *lsample, float *rsample) {

    if (*rsample > *trig / *scale) after_trig = 0;

    if (after_trig < graph_points) {
        if (sample_counter < *time) {
            sample_counter++;
        } else {
            graph->update(*rsample * *scale);
            sample_counter = 0;
            after_trig ++;
        }
    }
    return true;
}

//void Oscilloscope::dragHandler(ndk_helper::Vec2 v) {
////    relativePosition.x = (v.x_ - drag_from.x_)*drag_xscale + relative_position_backup.x;
////    relativePosition.y = (v.y_ - drag_from.y_)*drag_yscale + relative_position_backup.y;
////
////    Update();
//}
//
//void Oscilloscope::dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) {
////    drag_from = v;
////    relative_position_backup = relativePosition;
////    drag_xscale = xscale;
////    drag_yscale = yscale;
//}
//
//void Oscilloscope::dragEnd() {}