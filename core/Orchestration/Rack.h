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

    const float padding = 0; //0.004;

    float x_offset = 0;
    float max_x_offset = 0;

    std::list<GUI::IECanvas *> objects;
    vecmath::Vec2 obj_space_c;
    vecmath::Vec2 obj_space_s;

    MObject * m_in, * m_out;
    int midi_devices;

    vecmath::Vec2 drag_from;
    GUI::IECanvas * dragging_obj;
    std::mutex drawLock;

    enum {
        RACK_NO_ACTION,
        RACK_ACTION_DRAG,
        RACK_ACTION_MOVE,
        RACK_ACTION_DELETE
    };

    int action;
    float move_border = 0.4;
    float delete_border = 0.8;

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

        GSetDragBeginCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            drag_from = v;
            return this;
        });

        GSetDragHandlerCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            x_offset += (v.x - drag_from.x)/global.s.x;
            if ((max_x_offset > 0) || (x_offset > 0)) x_offset = 0;
            if ((max_x_offset < 0) && (x_offset < max_x_offset)) x_offset = max_x_offset;
            updatePositions();
            drag_from = v;
            if (v.y - global.c.y > global.s.y * move_border) {
                if (v.y - global.c.y > global.s.y * delete_border) {
                    action = RACK_ACTION_DELETE;
                } else {
                    action = RACK_ACTION_MOVE;
                }
            } else {
                action = RACK_NO_ACTION;
            }
            return this;
        });

        GSetDragEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            if (action == RACK_ACTION_DELETE) {
                RDel(dragging_obj);
                dragging_obj = nullptr;
                action = RACK_NO_ACTION;
            }
            return this;
        });
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
        drawLock.lock();
        auto it = std::find(objects.begin(), objects.end(), obj);

        if (it != objects.begin()) (*std::prev(it, 1))->MDisconnect(obj);
        if (it != objects.end()) (*it)->MDisconnect(*std::next(it, 1));

        objects.remove(obj);
        GDetach(obj);
        delete(obj);
        updatePositions();
        drawLock.unlock();
    }

    void updatePositions() {
        float ratio = 0;
        for (auto const& obj : objects) ratio += obj->local.ratio;

        float x = padding;
        for (auto const& obj : objects) {
            obj->GPlace({x + x_offset, obj_space_c.y + padding * obj_space_s.y});
            obj->GSetHeight(obj_space_s.y);
            x += obj->local.ratio / (global.ratio * obj_space_s.x / obj_space_s.y) +
                    padding * obj_space_s.y / obj_space_s.x;
        }

        max_x_offset = - x + 1;
    }

    GObject *GFindFocusObject(const vecmath::Vec2 &point, std::list<GObject *> * trace) override {
        auto fo = GUI::AMGCanvas::GFindFocusObject(point, trace);
        if (GUI::isIEHeader(fo)) {
            while (trace->back() != this) {
                if (GUI::isIECanvas(trace->back()))
                    dragging_obj = dynamic_cast<GUI::IECanvas *>(trace->back());
                trace->pop_back();
            }
            return this;
        }
        return fo;
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

    void GDraw(NVGcontext * nvg) override {
        drawLock.lock();
        GUI::AMGCanvas::GDraw(nvg);
        if (action == RACK_ACTION_DELETE) {
            nvgBeginPath(nvg);
            nvgRect(nvg, global.c.x,
                    global.c.y + global.s.y * delete_border,
                    global.s.x, global.s.y * (1 - delete_border));
            NVGcolor c = RED;
            c.a = 0.2;
            nvgFillColor(nvg, c);
            nvgFill(nvg);
            nvgClosePath(nvg);
        }
        drawLock.unlock();
    }

    bool ARender(double beat, float * lsample, float * rsample) override ;
};


#endif //PD_RACK_H
