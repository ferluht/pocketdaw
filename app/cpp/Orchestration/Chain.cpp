//
// Created by Admin on 21.09.2019.
//

#include "Chain.h"

GUI::GObject * AMGChain::GDragHandler(const ndk_helper::Vec2 &v) {

    auto old_moving_to = moving_to;

    while ((moving_to != IEObjects.begin()) && (v.x < (*moving_to)->global.c.x)){
        moving_to --;
    }
    while ((moving_to != IEObjects.end() - 2) && (v.x > (*moving_to)->global.c.x + (*moving_to)->global.s.x)){
        moving_to ++;
    }

    if (!(*moving_to)->GContains(v)) moving_overlay->GSetVisible(false);

    if (old_moving_to != moving_to){
//        auto position = (*moving_to)->globalPosition.toRelative(this->globalPosition);
//        moving_overlay->place(position.x, position.y, (*moving_to)->z);
//        moving_overlay->setHeight(position.height);
//        moving_overlay->setWidth(position.width);
//        moving_overlay->GSetVisible(true);
    }

    last_touch = v;

    return this;
}

GUI::GObject * AMGChain::GDragBegin(const ndk_helper::Vec2 &v) {
    std::list<GObject *> trace;
    auto go = AMGObject::GFindFocusObject(v, &trace);
    moving_from = std::find(IEObjects.begin(), IEObjects.end(), go);
    moving_to = moving_from;
//    moving_overlay->GSetVisible(true);
    return this;
}

GUI::GObject * AMGChain::GDragEnd(const ndk_helper::Vec2 &v) {
//    if ((*moving_to)->globalPosition.GContains(last_touch)){
//        auto go = (*moving_from);
//        AMGChainDel(moving_from - AMGObjects.begin());
//        AMGChainInsert(go, moving_to - AMGObjects.begin());
//    }
//    moving_overlay->GSetVisible(false);
    return nullptr;
}