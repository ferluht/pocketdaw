//
// Created by ibelikov on 11.07.19.
//

#include <GUI/Encoder.h>
#include "Oscilloscope.h"

Oscilloscope::Oscilloscope() : AudioEffect("Oscill")
{

    setRatio(1.5);

    plot = new GUI::Plot<GUI::TimeGraph>(200);
    plot->lPlace({0.01, 0.01});
    plot->lSetHeight(0.7);
    plot->lSetWidth(0.98);
    GAttach(plot);

    trig = new GUI::Encoder("trig", 0, 0, -1, 1);
    trig->lPlace({0.05, 0.72});
    trig->lSetHeight(0.26);
    GAttach(trig);
    MConnect(trig);

    time = new GUI::Encoder("time", 100, 0, 0, 100);
    time->lPlace({0.2, 0.72});
    time->lSetHeight(0.26);
    GAttach(time);
    MConnect(time);

    scale = new GUI::Encoder("scale", 1, 0, 0, 5);
    scale->lPlace({0.35, 0.72});
    scale->lSetHeight(0.26);
    GAttach(scale);
    MConnect(scale);
}

bool Oscilloscope::ARender(double beat, float *lsample, float *rsample) {

    if (*rsample > *trig / *scale) after_trig = 0;

    if (after_trig < graph_points) {
        if (sample_counter < *time) {
            sample_counter++;
        } else {
            plot->graph->update(*rsample * *scale);
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