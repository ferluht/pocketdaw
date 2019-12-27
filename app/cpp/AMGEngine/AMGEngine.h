//
// Created by Admin on 08.08.2019.
//

#ifndef PD_META_H
#define PD_META_H

#include "A.h"
#include "G.h"
#include "M.h"

class AMGObject : public AObject, public GUI::GObject, public MObject {
public:
    AMGObject(unsigned int shape_type_) : GUI::GObject(shape_type_) {}
};


class AMGEngine : MObject{

public:

    AEngine * audio;
    MEngine * midi;
    GUI::GEngine * graphic;

    AMGEngine(AMGObject* root_){
        audio = &AEngine::getAEngine();
        midi = &MEngine::getMEngine();
        graphic = &GUI::GEngine::getGEngine();

        audio->setRoot(root_);
        midi->MConnect(this);
        MConnect(root_);
        graphic->setRoot(root_);
    }

    void MIn(MData cmd) override {
        MOut(cmd);
    }

};

#endif //PD_META_H
