//
// Created by Admin on 12.06.2019.
//

#include <chrono>
#include "Track.h"

GUI::GObject * AMGTrack::GDragHandler(const ndk_helper::Vec2 &v) {
    x_offset += (v.x - drag_from.x)/global.s.x;
    if ((max_x_offset > 0) || (x_offset > 0)) x_offset = 0;
    if ((max_x_offset < 0) && (x_offset < max_x_offset)) x_offset = max_x_offset;
    updatePositions();
    drag_from = v;
    return this;
}

GUI::GObject * AMGTrack::GDragBegin(const ndk_helper::Vec2 &v) {
    drag_from = v;
    return this;
}