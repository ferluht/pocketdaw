//
// Created by ibelikov on 11.07.19.
//

#include <GUI/Encoder.h>
#include "Oscilloscope.h"

Oscilloscope::Oscilloscope() : AudioEffect(L"Oscill")
{

    setRatio(1.5);

    graph = new TimePlot(200);
    graph->place(0.01, 0.01);
    graph->setHeight(0.7);
    graph->setWidth(0.98);
    GAttach(graph);

    trig = new Encoder(L"trig", 0, 0, -1, 1);
    trig->place(0.05, 0.72);
    trig->setHeight(0.26);
    GAttach(trig);
    MConnect(trig);

    time = new Encoder(L"time", 100, 0, 0, 100);
    time->place(0.2, 0.72);
    time->setHeight(0.26);
    GAttach(time);
    MConnect(time);

    scale = new Encoder(L"scale", 1, 0, 0, 5);
    scale->place(0.35, 0.72);
    scale->setHeight(0.26);
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