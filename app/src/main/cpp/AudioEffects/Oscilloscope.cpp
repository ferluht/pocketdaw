//
// Created by ibelikov on 11.07.19.
//

#include <GUI/Encoder.h>
#include "Oscilloscope.h"

Oscilloscope::Oscilloscope(float n) : AudioEffect(L"Oscill")
{

    window = n;

    setRatio(1.5);

    graph = new TimeGraph(n);
    graph->place(0.05, 0.05);
    graph->setHeight(0.9);
    graph->setWidth(0.9);
    GAttach(graph);
}

bool Oscilloscope::ARender(double beat, float *lsample, float *rsample) {
    if (ai < window){
        accumulator += *rsample;
        ai ++;
    } else {
        ai = 0;
        graph->update(accumulator / window);
        accumulator = 0;
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