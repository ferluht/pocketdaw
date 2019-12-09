//
// Created by Admin on 21.09.2019.
//

#ifndef PD_CHAIN_H
#define PD_CHAIN_H

#include <Engine/Engine.h>
#include <GUI/Canvas.h>
#include <common/GUI/styles.h>
#include <common/GUI/IECanvas.h>

class AMGChain : public AMGObject {

private:

    std::vector<GUI::IECanvas *> IEObjects;
    const float spacing = 0.01;
    std::vector<GUI::IECanvas *>::iterator moving_from, moving_to;
    GUI::GCanvas * moving_overlay;
    int moving_focus = 0;
    ndk_helper::Vec2 last_touch;

public:

    AMGChain() {

        setShapeType(GUI::BOX);
//        moving_overlay = new GCanvas();
//        moving_overlay->GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
//        moving_overlay->GSetColor(1, 1, 1, 0.2);
//        moving_overlay->GSetVisible(false);
//        GAttach(moving_overlay);

        auto dummy = new GUI::IECanvas("dummy");
        IEObjects.push_back(dummy);
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

        if (shape->changed) {

            float cur_ratio = 0;
            int i = 0;
            for (auto const &obj : IEObjects) {
                obj->shape->lPlace({cur_ratio / shape->local.ratio + spacing * i, 0});
                obj->shape->lSetHeight(1);
                cur_ratio += obj->shape->local.ratio;
                i++;
            }
        }

//        nvgBeginPath(nvg);
//        nvgRect(nvg,
//                shape->global.c.x,
//                shape->global.c.y,
//                shape->global.s.x, shape->global.s.y);
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
        shape->setRatio(shape->local.ratio + ieo->shape->local.ratio + spacing);
        IEObjects.insert(IEObjects.end() - 1, ieo);
        GAttach(ieo);
        shape->changed = true;
    }

    inline void AMGChainPushFront(GUI::IECanvas * ieo) {
        ieo->MConnect(IEObjects[0]);
        IEObjects.insert(IEObjects.begin(), ieo);
        shape->setRatio(shape->local.ratio + ieo->shape->local.ratio + spacing);
        GAttach(ieo);
        shape->changed = true;
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
            shape->setRatio(shape->local.ratio + ieo->shape->local.ratio + spacing);
            IEObjects.insert(IEObjects.begin() + pos, ieo);
            GAttach(ieo);
            shape->changed = true;
        }
    }

    inline void AMGChainDel(int pos) {
        auto size = IEObjects.size();
        shape->setRatio(shape->local.ratio - IEObjects[pos]->shape->local.ratio);
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
        shape->changed = true;
    }

    inline void MRender(double beat) override {
        for (auto const& mo : IEObjects) mo->MRender(beat);
    }

    inline void MEnableMapping(bool state) override {
        for (auto const& mo : IEObjects) mo->MEnableMapping(state);
    }

//    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override
//    {
//        if (visible && globalPosition.contains(point)){
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

    virtual GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) {
        for (auto const &gr : Graphics) {
            auto fo = gr->GFindFocusObject(point, trace);
            if (fo) {
                trace->push_front(this);
                if (fo == this) return parent;
                if (fo == gr) return this;
                return fo;
            }
        }
        if (visible && shape->contains(point)) {
            trace->push_front(this);
            return this;
        }
        return nullptr;
    }

    void GSetVisible(bool visible_) override {
        AMGObject::GSetVisible(visible_);
//        moving_overlay->GSetVisible(false);
    }

    GObject * GDragHandler(const ndk_helper::Vec2& v) override ;
    GObject * GDragBegin(const ndk_helper::Vec2& v) override ;
    GObject * GDragEnd(const ndk_helper::Vec2& v) override ;
};


#endif //PD_CHAIN_H
