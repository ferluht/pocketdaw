//
// Created by Admin on 21.09.2019.
//

#ifndef PD_CHAIN_H
#define PD_CHAIN_H

#include <Engine/Engine.h>
#include <GUI/Canvas.h>

class AMGChain : public AMGObject {

private:

    std::vector<AMGObject*> AMGObjects;
    const float spacing = 0.01;
    std::vector<AMGObject*>::iterator moving_from, moving_to;
    GCanvas * moving_overlay;
    int moving_focus = 0;
    ndk_helper::Vec2 last_touch;

public:

    AMGChain() {

        moving_overlay = new GCanvas();
        moving_overlay->GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
        moving_overlay->GSetColor(1, 1, 1, 0.2);
        moving_overlay->GSetVisible(false);
        GAttach(moving_overlay);

        auto dummy = new AMGObject();
        AMGObjects.push_back(dummy);
    }

    inline bool ARender(double beat, float * lsample, float * rsample) override {
        for (auto const &ao : AMGObjects) {
            ao->ARender(beat, lsample, rsample);
        }
        return true;
    }

    inline void MIn(MData cmd) override {
        AMGObjects.front()->MIn(cmd);
    }

    inline void MConnect(MObject * mo) override {
        AMGObjects.back()->MConnect(mo);
    }

    inline void MDisconnect(MObject * mo) override {
        AMGObjects.back()->MDisconnect(mo);
    }

    void GDraw() override {
        float cur_ratio = 0;
        int i = 0;
        for (auto const &obj : AMGObjects) {
            obj->place(cur_ratio/ratio + spacing * i, 0);
            obj->setHeight(1);
            cur_ratio += obj->ratio;
            i ++;
        }
        AMGObject::GDraw();
    }

    inline void AMGChainPushBack(AMGObject * mo) {
        auto size = AMGObjects.size();
        if (size > 1) {
            AMGObjects[size - 2]->MDisconnect(AMGObjects.back());
            AMGObjects[size - 2]->MConnect(mo);
        }
        mo->MConnect(AMGObjects.back());
        setRatio(ratio + mo->ratio + spacing);
        AMGObjects.insert(AMGObjects.end() - 1, mo);
        GAttach(mo);
        changed = true;
    }

    inline void AMGChainPushFront(AMGObject * mo) {
        mo->MConnect(AMGObjects[0]);
        AMGObjects.insert(AMGObjects.begin(), mo);
        setRatio(ratio + mo->ratio + spacing);
        GAttach(mo);
        changed = true;
    }

    inline void AMGChainInsert(AMGObject * mo, int pos) {
        auto size = AMGObjects.size();
        if (pos > size) pos = size;
        if (pos < 0) pos = 0;
        if (pos == size - 1) AMGChainPushBack(mo);
        else if (pos == 0) AMGChainPushFront(mo);
        else {
            AMGObjects[pos - 1]->MDisconnect(AMGObjects[pos]);
            AMGObjects[pos - 1]->MConnect(mo);
            mo->MConnect(AMGObjects[pos]);
            setRatio(ratio + mo->ratio + spacing);
            AMGObjects.insert(AMGObjects.begin() + pos, mo);
            GAttach(mo);
            changed = true;
        }
    }

    inline void AMGChainDel(int pos) {
        auto size = AMGObjects.size();
        setRatio(ratio - AMGObjects[pos]->ratio);
        if (pos > 0 && pos < size - 1) {
            AMGObjects[pos]->MDisconnect(AMGObjects[pos + 1]);
            AMGObjects[pos - 1]->MDisconnect(AMGObjects[pos]);
            AMGObjects[pos - 1]->MConnect(AMGObjects[pos + 1]);
            GDetach(AMGObjects[pos]);
            AMGObjects.erase(AMGObjects.begin() + pos);
        } else if (pos == 0 && size > 1) {
            AMGObjects[0]->MDisconnect(AMGObjects[1]);
            GDetach(AMGObjects[pos]);
            AMGObjects.erase(AMGObjects.begin() + pos);
        }
        changed = true;
    }

    inline void MRender(double beat) override {
        for (auto const& mo : AMGObjects) mo->MRender(beat);
    }

    inline void MEnableMapping(bool state) override {
        for (auto const& mo : AMGObjects) mo->MEnableMapping(state);
    }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override
    {
        if (visible && globalPosition.contains(point)){
            for (auto const &gr : Graphics) {
                auto fo = gr->GFindFocusObject(point);
                if (fo){
                    if (fo == this) return parent;
                    if (fo == gr) return this;
                    return fo;
                }
            }
        }

        return nullptr;
    }

    void GSetVisible(bool visible_) override {
        AMGObject::GSetVisible(visible_);
        moving_overlay->GSetVisible(false);
    }

    GObject * GDragHandler(const ndk_helper::Vec2& v) override ;
    GObject * GDragBegin(const ndk_helper::Vec2& v) override ;
    GObject * GDragEnd(const ndk_helper::Vec2& v) override ;
};


#endif //PD_CHAIN_H
