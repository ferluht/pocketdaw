//
// Created by Admin on 21.09.2019.
//

#ifndef PD_RACK_H
#define PD_RACK_H

#include <GUI/Canvas.h>
#include <MidiEffects/MidiEffect.h>
#include <Instruments/Instrument.h>
#include <AudioEffects/AudioEffect.h>
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
    Vec2 obj_space_c;
    Vec2 obj_space_s;

    MObject * m_in, * m_out;
    int midi_devices;

    Vec2 drag_from;

    AMGRack(){

        midi_devices = 0;
        m_in = new MObject;
        m_out = new MObject;

        obj_space_c = {0, 0};
        obj_space_s = {1, 1};
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
        obj->GSetHeight(obj_space_s.y * (1 - 2 * padding));

        if (objects.empty()) m_in->MDisconnect(m_out);

        if (isAudioEffect(obj)) {
            if (!objects.empty()) {
                objects.back()->MDisconnect(m_out);
                objects.back()->MConnect(obj);
            }
            objects.push_back(obj);
            objects.back()->MConnect(m_out);
        }
        if (isMidiEffect(obj)) {
            if (!objects.empty()) {
                m_in->MDisconnect(objects.front());
                obj->MConnect(objects.front());
            }
            objects.push_front(obj);
            m_in->MConnect(objects.front());
        }
        if (isInstrument(obj)) {

            auto it = objects.begin();
            for (it; (it != objects.end()) && isMidiEffect(*it); ++it);
            if (isInstrument(*it)) {
                auto instr = *it;
                objects.erase(it);
                delete(instr);
                it = objects.begin();
                for (it; (it != objects.end()) && isMidiEffect(*it); ++it);
            }

            if (it == objects.begin()) {
                if (!objects.empty()) {
                    m_in->MDisconnect(objects.front());
                    obj->MConnect(objects.front());
                } else {
                    m_in->MDisconnect(m_out);
                    obj->MConnect(m_out);
                }
                m_in->MConnect(obj);
            } else if (it == objects.end()) {
                if (!objects.empty()) {
                    objects.back()->MDisconnect(m_out);
                    objects.back()->MConnect(obj);
                } else {
                    m_in->MDisconnect(m_out);
                    m_in->MConnect(obj);
                }
                obj->MConnect(m_out);
            } else {
                (*std::prev(it, 1))->MDisconnect(*it);
                (*std::prev(it, 1))->MConnect(obj);
                obj->MConnect(*it);
            }

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
        for (auto const& obj : objects) ratio += obj->local.ratio;

        float x = padding;
        for (auto const& obj : objects) {
            obj->GPlace({x + x_offset, obj_space_c.y + padding * obj_space_s.y});
            x += obj->local.ratio / (global.ratio * obj_space_s.x / obj_space_s.y) +
                    padding * obj_space_s.y / obj_space_s.x;
        }

        max_x_offset = - x + 1;
    }

    inline void MIn(MData cmd) override {
        m_in->MIn(cmd);
    }

    inline void MRender(double beat) override {
        for (auto const& obj : objects) obj->MRender(beat);
    }

    void MConnect(MObject * mo) override {
        m_out->MConnect(mo);
    }

    void MDisconnect(MObject * mo) override {
        m_out->MDisconnect(mo);
    }

    bool ARender(double beat, float * lsample, float * rsample) override ;
};


#endif //PD_RACK_H
