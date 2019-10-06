//
// Created by Admin on 21.09.2019.
//

#include "Chain.h"

GObject * AMGChain::GDragHandler(const ndk_helper::Vec2 &v) {

    auto old_moving_to = moving_to;

    while ((moving_to != AMGObjects.begin()) && (v.x_ < (*moving_to)->globalPosition.x)){
        moving_to --;
    }
    while ((moving_to != AMGObjects.end() - 2) && (v.x_ > (*moving_to)->globalPosition.x + (*moving_to)->globalPosition.width)){
        moving_to ++;
    }

    if (!(*moving_to)->globalPosition.contains(v)) moving_overlay->GSetVisible(false);

    if (old_moving_to != moving_to){
        auto position = (*moving_to)->globalPosition.toRelative(this->globalPosition);
        moving_overlay->place(position.x, position.y, (*moving_to)->z);
        moving_overlay->setHeight(position.height);
        moving_overlay->setWidth(position.width);
        moving_overlay->GSetVisible(true);
    }

    last_touch = v;

    return this;
}

GObject * AMGChain::GDragBegin(const ndk_helper::Vec2 &v) {
    auto go = AMGObject::GFindFocusObject(v);
    moving_from = std::find(AMGObjects.begin(), AMGObjects.end(), go);
    moving_to = moving_from;
//    moving_overlay->GSetVisible(true);
    return this;
}

GObject * AMGChain::GDragEnd(const ndk_helper::Vec2 &v) {
    if ((*moving_to)->globalPosition.contains(last_touch)){
        auto go = (*moving_from);
        AMGChainDel(moving_from - AMGObjects.begin());
        AMGChainInsert(go, moving_to - AMGObjects.begin());
    }
    moving_overlay->GSetVisible(false);
    return nullptr;
}