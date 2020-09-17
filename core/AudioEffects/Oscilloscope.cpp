//
// Created by ibelikov on 11.07.19.
//

#include <GUI/Encoder.h>
#include "Oscilloscope.h"

Oscilloscope::Oscilloscope() : AudioEffect("Oscill")
{

    GSetRatio(0.3f);

    plot = new GUI::Plot<GUI::TimeGraph>(graph_points);
    plot->GPlace({0.01, 0.01});
    plot->GSetHeight(0.98*0.3);
    plot->GSetWidth(0.98);
    GAttach(plot);

    trig = new GUI::Encoder("trig", 0, -1, 1);
    trig->GPlace({0.225, 0.35});
    trig->GSetHeight(0.2);
    GAttach(trig);
    MConnect(trig);

    time = new GUI::Encoder("time", 100, 0, 100);
    time->GPlace({0.225, 0.575});
    time->GSetHeight(0.2);
    GAttach(time);
    MConnect(time);

    scale = new GUI::Encoder("scale", 1, 0, 5);
    scale->GPlace({0.225, 0.8});
    scale->GSetHeight(0.2);
    GAttach(scale);
    MConnect(scale);
}

bool Oscilloscope::ARender(double beat, float *lsample, float *rsample) {

//    if (*rsample > *trig / *scale) after_trig = 0;
    if (*rsample > max) after_trig = 0;

    if (after_trig < graph_points) {
        if (sample_counter < *time) {
            sample_counter++;
        } else {
            plot->graph->update(*rsample * *scale);
            sample_counter = 0;
            after_trig ++;
        }
    }
    if (*rsample > max) max = *rsample;
    max -= 0.00001;
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