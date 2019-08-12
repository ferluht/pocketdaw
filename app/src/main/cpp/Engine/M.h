//
// Created by Admin on 07.08.2019.
//

#ifndef PD_MIDIOBJECT_H
#define PD_MIDIOBJECT_H

#include <stdlib.h>
#include <vector>
#include <queue>
#include <cmath>
#include <list>

#define NOTEOFF_HEADER 128
#define NOTEON_HEADER 144
#define POLYPRESSURE_HEADER 160
#define CC_HEADER 176
#define PITCHWHEEL_HEADER 224

#define CC_MODWHEEL 1
#define CC_BREATH_CONTROLLER 2
#define CC_AFTERTOUCH 3
#define CC_FOOT_CONTROLLER 4
#define CC_PORTAMENTO_TIME 5
#define CC_DATA_ENTRY 6
#define CC_MAIN_VOLUME 7
#define CC_DAMPER_PEDAL 64
#define CC_PORTAMENTO 65
#define CC_SOSTENUTO 66
#define CC_SOFT_PEDAL 67
#define CC_CHORUS 93


#define BASE_NOTE 61.0
#define POWER_BASE 2.0
#define SEMITONES 24.0

struct MData {
    double beat;
    unsigned char status;
    unsigned char data1;
    unsigned char data2;
};

class MObject {

    MObject * MInput;
    std::list<MObject *> MOutputs;

public:

    MObject(){
        MInput = nullptr;
    }

    virtual inline void MConnect(MObject * mo) {
        MOutputs.push_back(mo);
        mo->MInput = this;
    }

    virtual inline void MDisconnect(MObject * mo) {
        MOutputs.remove(mo);
        mo->MInput = nullptr;
    }

    virtual inline void MOut(MData cmd) {
        for (auto const& out : MOutputs) out->MIn(cmd);
    }

    virtual inline void MIn(MData cmd) {
        MOut(cmd);
    }

    virtual void MRender(double beat) {};
};

class MEngine : public MObject {

public:

    static MEngine &getMEngine() {
        static MEngine mengine;
        return mengine;
    }

    MEngine(MEngine const &) = delete;             // Copy construct
    MEngine(MEngine &&) = delete;                  // Move construct
    MEngine &operator=(MEngine const &) = delete;  // Copy assign
    MEngine &operator=(MEngine &&) = delete;      // Move assign

protected:

    MEngine() {
        // Constructor code goes here.
    }

    ~MEngine() {
        // Destructor code goes here.
    }
};

#endif //PD_MIDIOBJECT_H
