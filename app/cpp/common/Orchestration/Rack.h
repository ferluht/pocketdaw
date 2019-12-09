//
// Created by Admin on 21.09.2019.
//

#ifndef PD_RACK_H
#define PD_RACK_H

#include <GUI/Canvas.h>
#include "Chain.h"

class AMGRack : public GUI::AMGCanvas{

private:

    MObject mout;

public:

    const float padding = 0.01;

    float x_offset = 0;

    AMGChain MEffects;
    GUI::IECanvas * Instr;
    AMGChain AEffects;

    AMGRack(){
        Instr = new GUI::IECanvas("dummy");
        MEffects.MConnect(Instr);
        MEffects.MConnect(&mout);
        Instr->MConnect(&AEffects);

        GAttach(&MEffects);
        GAttach(&AEffects);
    }

    AMGRack(GUI::IECanvas * instr_) : AMGRack() {
        RAttachInsrument(instr_);
    }

    inline void RAttachInsrument(GUI::IECanvas * instr_) {
        MEffects.MDisconnect(Instr);
        Instr->MDisconnect(&AEffects);
        GDetach(Instr);
//        delete Instr;

        Instr = instr_;
        Instr->shape->lPlace({x_offset + (MEffects.shape->local.ratio + padding)/shape->global.ratio + padding, padding});
        Instr->shape->lSetHeight(1 - 2*padding);
        GAttach(Instr);

        Instr->MConnect(&AEffects);
        MEffects.MConnect(Instr);
//        changed = true;
    }

    void GDraw(NVGcontext *nvg) override {
        MEffects.shape->lPlace({x_offset + padding, padding});
        MEffects.shape->lSetHeight(1 - 2*padding);
        Instr->shape->lPlace({x_offset + (MEffects.shape->local.ratio + padding)/shape->global.ratio + padding, padding});
        Instr->shape->lSetHeight(1 - 2*padding);
        AEffects.shape->lPlace({x_offset + (MEffects.shape->local.ratio + Instr->shape->global.ratio + 2*padding)/shape->global.ratio + padding, padding});
        AEffects.shape->lSetHeight(1 - 2*padding);
//        GUI::AMGCanvas::GDraw(nvg);
    }

    inline void RDetachInsrument() {
        MEffects.MDisconnect(Instr);
        Instr->MDisconnect(&AEffects);
        Instr = new GUI::IECanvas("dummy");
        GDetach(Instr);
        MEffects.MConnect(Instr);
        Instr->MConnect(&AEffects);
    }

    inline void RAddMidiEffect(GUI::IECanvas * meffect){
        MEffects.AMGChainPushBack(meffect);
//        changed = true;
    }

    inline void RDelMidiEffect(){

    }

    inline void RAddAudioEffect(GUI::IECanvas * aeffect){
        AEffects.AMGChainPushBack(aeffect);
//        changed = true;
    }

    inline void RDelAudioEffect(){

    }

    inline void MIn(MData cmd) override {
        MEffects.MIn(cmd);
    }

    inline void MEnableMapping(bool state) override {
        MEffects.MEnableMapping(state);
        Instr->MEnableMapping(state);
        AEffects.MEnableMapping(state);
    }

    inline void MRender(double beat) override {
        MEffects.MRender(beat);
        Instr->MRender(beat);
        AEffects.MRender(beat);
    }

//    void MConnect(MObject * mo) override ;
//    void MDisconnect(MObject * mo) override ;

    bool ARender(double beat, float * lsample, float * rsample) override ;
};


#endif //PD_RACK_H
