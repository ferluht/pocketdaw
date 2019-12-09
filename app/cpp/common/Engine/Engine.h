//
// Created by Admin on 08.08.2019.
//

#ifndef PD_META_H
#define PD_META_H

#include "A.h"
#include "G.h"
#include "M.h"

class AGObject : virtual public AObject, virtual public GUI::GObject {};

class AMObject : virtual public AObject, virtual public MObject {};

class MGObject : virtual public MObject, virtual public GUI::GObject {};

class AMGObject : virtual public AObject, virtual public GUI::GObject, virtual public MObject {};

class Engine : MObject{

public:

    AEngine * audio;
    MEngine * midi;
    GUI::GEngine * graphic;

    Engine(AMGObject * root_){
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

//    static void HandleCmd(struct android_app *app, int32_t cmd);

//    static int32_t HandleInput(android_app *app, AInputEvent *event);
};

#endif //PD_META_H
