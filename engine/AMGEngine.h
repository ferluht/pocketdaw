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


class AMGEngine {

public:

    AEngine * audio;
    MEngine * midi;
    GUI::GEngine * graphic;

    AMGEngine(AMGObject* root_){
        audio = &AEngine::getAEngine();
        midi = &MEngine::getMEngine();
        graphic = &GUI::GEngine::getGEngine();

        audio->getRootDirectory();

//        audio->getDevices();

        audio->setRoot(root_);
        midi->MConnect(root_);
        graphic->setRoot(root_);
    }

};

#endif //PD_META_H
