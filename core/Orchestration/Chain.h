//
// Created by Admin on 21.09.2019.
//

#ifndef PD_CHAIN_H
#define PD_CHAIN_H

#include <AMGEngine.h>
#include <GUI/Canvas.h>
#include <GUI/styles.h>
#include <GUI/IECanvas.h>

class AMGChain : public AMGObject {

private:

    std::vector<GUI::IECanvas *> IEObjects;
    const float spacing = 0.01;
    std::vector<GUI::IECanvas *>::iterator moving_from, moving_to;
    GUI::AMGCanvas * moving_overlay;
    int moving_focus = 0;
    vecmath::Vec2 last_touch;

public:

    AMGChain() : AMGObject(GUI::BOX) {

//        moving_overlay = new GCanvas();
//        moving_overlay->GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
//        moving_overlay->GSetColor(1, 1, 1, 0.2);
//        moving_overlay->GSetVisible(false);
//        GAttach(moving_overlay);

        auto dummy = new GUI::IECanvas("dummy");
        IEObjects.push_back(dummy);

        GSetDragBeginCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            std::list<GObject *> trace;
            auto go = AMGObject::GFindFocusObject(v, &trace);
            moving_from = std::find(IEObjects.begin(), IEObjects.end(), go);
            moving_to = moving_from;
//    moving_overlay->GSetVisible(true);
            return this;
        });

        GSetDragHandlerCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
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
        });

        GSetDragEndCallback([](const vecmath::Vec2& v) -> GUI::GObject * {
            //    if ((*moving_to)->globalPosition.GContains(last_touch)){
//        auto go = (*moving_from);
//        AMGChainDel(moving_from - AMGObjects.begin());
//        AMGChainInsert(go, moving_to - AMGObjects.begin());
//    }
//    moving_overlay->GSetVisible(false);
            return nullptr;
        });
    }

    inline bool ARender(double beat, float * lsample, float * rsample) override {
        for (auto const &ieo : IEObjects) {
            ieo->ARender(beat, lsample, rsample);
        }
        return true;
    }

    inline void MIn(MData cmd) override {
        IEObjects.front()->MIn(cmd);
    }

    inline void MConnect(MObject * mo) override {
        IEObjects.back()->MConnect(mo);
    }

    inline void MDisconnect(MObject * mo) override {
        IEObjects.back()->MDisconnect(mo);
    }

    void GDraw(NVGcontext *nvg) override {

        if (changed) {

            float cur_ratio = 0;
            int i = 0;
            for (auto const &obj : IEObjects) {
                obj->GPlace({cur_ratio / local.ratio + spacing * i, 0});
                obj->GSetHeight(1);
                cur_ratio += obj->local.ratio;
                i++;
            }
        }

//        nvgBeginPath(nvg);
//        nvgRect(nvg,
//                global.c.x,
//                global.c.y,
//                global.s.x, global.s.y);
//        nvgFillColor(nvg, BLUE);
//        nvgFill(nvg);
    }

    inline void AMGChainPushBack(GUI::IECanvas * ieo) {
        auto size = IEObjects.size();
        if (size > 1) {
            IEObjects[size - 2]->MDisconnect(IEObjects.back());
            IEObjects[size - 2]->MConnect(ieo);
        }
        ieo->MConnect(IEObjects.back());
        GSetRatio(local.ratio + ieo->local.ratio + spacing);
        IEObjects.insert(IEObjects.end() - 1, ieo);
        GAttach(ieo);
        changed = true;
    }

    inline void AMGChainPushFront(GUI::IECanvas * ieo) {
        ieo->MConnect(IEObjects[0]);
        IEObjects.insert(IEObjects.begin(), ieo);
        GSetRatio(local.ratio + ieo->local.ratio + spacing);
        GAttach(ieo);
        changed = true;
    }

    inline void AMGChainInsert(GUI::IECanvas * ieo, int pos) {
        auto size = IEObjects.size();
        if (pos > size) pos = size;
        if (pos < 0) pos = 0;
        if (pos == size - 1) AMGChainPushBack(ieo);
        else if (pos == 0) AMGChainPushFront(ieo);
        else {
            IEObjects[pos - 1]->MDisconnect(IEObjects[pos]);
            IEObjects[pos - 1]->MConnect(ieo);
            ieo->MConnect(IEObjects[pos]);
            GSetRatio(local.ratio + ieo->local.ratio + spacing);
            IEObjects.insert(IEObjects.begin() + pos, ieo);
            GAttach(ieo);
            changed = true;
        }
    }

    inline void AMGChainDel(int pos) {
        auto size = IEObjects.size();
        GSetRatio(local.ratio - IEObjects[pos]->local.ratio);
        if (pos > 0 && pos < size - 1) {
            IEObjects[pos]->MDisconnect(IEObjects[pos + 1]);
            IEObjects[pos - 1]->MDisconnect(IEObjects[pos]);
            IEObjects[pos - 1]->MConnect(IEObjects[pos + 1]);
            GDetach(IEObjects[pos]);
            IEObjects.erase(IEObjects.begin() + pos);
        } else if (pos == 0 && size > 1) {
            IEObjects[0]->MDisconnect(IEObjects[1]);
            GDetach(IEObjects[pos]);
            IEObjects.erase(IEObjects.begin() + pos);
        }
        changed = true;
    }

    inline void MRender(double beat) override {
        for (auto const& mo : IEObjects) mo->MRender(beat);
    }

//    GObject * GFindFocusObject(const vecmath::Vec2& point) override
//    {
//        if (visible && globalPosition.GContains(point)){
//            for (auto const &gr : Graphics) {
//                auto fo = gr->GFindFocusObject(point);
//                if (fo){
//                    if (fo == this) return parent;
//                    if (fo == gr) return this;
//                    return fo;
//                }
//            }
//        }
//
//        return nullptr;
//    }

    virtual GObject *GFindFocusObject(const vecmath::Vec2 &point, std::list<GObject *> * trace) override {
        for (auto const &gr : Graphics) {
            auto fo = gr->GFindFocusObject(point, trace);
            if (fo) {
                trace->push_front(this);
                if (fo == this) return parent;
                if (fo == gr) return this;
                return fo;
            }
        }
        if (visible && GContains(point)) {
            trace->push_front(this);
            return this;
        }
        return nullptr;
    }

    void GSetVisible(bool visible_) override {
        AMGObject::GSetVisible(visible_);
//        moving_overlay->GSetVisible(false);
    }
};


#endif //PD_CHAIN_H
