//
// Created by Admin on 21.09.2019.
//

#ifndef PD_RACK_H
#define PD_RACK_H

#include <GUI/Canvas.h>
#include "Chain.h"

class AMGRack : public AMGCanvas{

private:

    MObject mout;

public:

    const float padding = 0.01;

    float x_offset = 0;

    AMGChain MEffects;
    AMGObject * Instr;
    AMGChain AEffects;

    AMGRack(){
        Instr = new AMGObject();
        MEffects.MConnect(Instr);
        MEffects.MConnect(&mout);
        Instr->MConnect(&AEffects);

        GAttachTexture("Textures/track_canvas.bmp");

        GAttach(&MEffects);
        GAttach(&AEffects);
    }

    AMGRack(AMGObject * instr_) : AMGRack() {
        RAttachInsrument(instr_);
    }

    inline void RAttachInsrument(AMGObject * instr_) {
        MEffects.MDisconnect(Instr);
        Instr->MDisconnect(&AEffects);
        GDetach(Instr);
//        delete Instr;

        Instr = instr_;
        GAttach(Instr);

        Instr->MConnect(&AEffects);
        MEffects.MConnect(Instr);
        changed = true;
    }

    void GDraw() override {
        MEffects.place(x_offset + padding, padding);
        MEffects.setHeight(1 - 2*padding);
        Instr->place(x_offset + (MEffects.ratio + padding)/globalPosition.ratio + padding, padding);
        Instr->setHeight(1 - 2*padding);
        AEffects.place(x_offset + (MEffects.ratio + Instr->ratio + 2*padding)/globalPosition.ratio + padding, padding);
        AEffects.setHeight(1 - 2*padding);
        AMGCanvas::GDraw();
    }

    inline void RDetachInsrument() {
        MEffects.MDisconnect(Instr);
        Instr->MDisconnect(&AEffects);
        Instr = new AMGObject();
        GDetach(Instr);
        MEffects.MConnect(Instr);
        Instr->MConnect(&AEffects);
    }

    inline void RAddMidiEffect(AMGObject * meffect){
        MEffects.AMGChainPushBack(meffect);
        changed = true;
    }

    inline void RDelMidiEffect(){

    }

    inline void RAddAudioEffect(AMGObject * aeffect){
        AEffects.AMGChainPushBack(aeffect);
        changed = true;
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
