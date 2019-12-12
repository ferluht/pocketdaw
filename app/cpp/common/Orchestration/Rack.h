//
// Created by Admin on 21.09.2019.
//

#ifndef PD_RACK_H
#define PD_RACK_H

#include <GUI/Canvas.h>
#include <common/MidiEffects/MidiEffect.h>
#include <common/Instruments/Instrument.h>
#include <common/AudioEffects/AudioEffect.h>
#include "Chain.h"
#include <algorithm>

class AMGRack : public GUI::AMGCanvas{

private:

    MObject mout;

public:

    const float padding = 0.01;

    float x_offset = 0;
    float max_x_offset = 0;

    std::list<GUI::IECanvas *> objects;
    int midi_devices;

    Vec2 drag_from;

    AMGRack(){

        midi_devices = 0;
//        Instr = new GUI::IECanvas("dummy");
//        MEffects.MConnect(Instr);
//        MEffects.MConnect(&mout);
//        Instr->MConnect(&AEffects);
//
//        GAttach(&MEffects);
//        GAttach(&AEffects);
    }

    AMGRack(GUI::IECanvas * instr_) : AMGRack() {
//        RAttachInsrument(instr_);
    }

    void RAdd(GUI::IECanvas * obj){
        obj->shape->lSetHeight(1);

        if (isAudioEffect(obj)) {
            if (!objects.empty()) objects.back()->MConnect(obj);
            objects.push_back(obj);
        }
        if (isMidiEffect(obj)) {
            if (!objects.empty()) obj->MConnect(objects.front());
            objects.push_front(obj);
        }
        if (isInstrument(obj)) {

            auto it = objects.begin();
            for (it; (it != objects.end()) && isMidiEffect(*it); ++it);
            if (isInstrument(*it)) {
                objects.erase(it);
                it = objects.begin();
                for (it; (it != objects.end()) && isMidiEffect(*it); ++it);
            }

            if (it != objects.begin()) (*std::prev(it, 1))->MConnect(obj);
            if (it != objects.end()) obj->MConnect(*std::next(it, 1));

            objects.insert(it, obj);
        }

        updatePositions();

        GAttach(obj);
    }

    void RDel(GUI::IECanvas * obj){
        auto it = std::find(objects.begin(), objects.end(), obj);

        if (it != objects.begin()) (*std::prev(it, 1))->MDisconnect(obj);
        if (it != objects.end()) (*it)->MDisconnect(*std::next(it, 1));

        objects.remove(obj);
    }

    void updatePositions() {
        float ratio = 0;
        for (auto const& obj : objects) ratio += obj->shape->local.ratio;

        float x = padding;
        for (auto const& obj : objects) {
            obj->shape->lPlace({x + x_offset, padding});
            x += obj->shape->local.ratio / shape->global.ratio + padding;
        }

        max_x_offset = - x + 1;
    }

    inline void MIn(MData cmd) override {
        if (objects.size()) objects.front()->MIn(cmd);
    }

    inline void MEnableMapping(bool state) override {
        for (auto const& obj : objects) obj->MEnableMapping(state);
    }

    inline void MRender(double beat) override {
        for (auto const& obj : objects) obj->MRender(beat);
    }

//    void MConnect(MObject * mo) override ;
//    void MDisconnect(MObject * mo) override ;

    bool ARender(double beat, float * lsample, float * rsample) override ;

//    GUI::GObject * GDragHandler(const ndk_helper::Vec2& v) override ;
//    GUI::GObject * GDragBegin(const ndk_helper::Vec2& v) override ;
};


#endif //PD_RACK_H
