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
#include <jni.h>

#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <string>

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
#define SEMITONES 12.0

struct MData {
    double beat;
    unsigned char status;
    unsigned char data1;
    unsigned char data2;
};

class MObject {

private:

    MObject * MInput;
    std::list<MObject *> MOutputs;
    bool mapping_mode;

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

    virtual void MRender(double beat) {}

    inline virtual bool MMap(MData) { return false; }

    inline virtual void MEnableMapping(bool state) {
        mapping_mode = state;
        for (auto const& out : MOutputs) out->MEnableMapping(state);
    }
};

class MEngine : public MObject {

    android_app *app_;

public:

    static MEngine &getMEngine() {
        static MEngine mengine;
        return mengine;
    }

    MEngine(MEngine const &) = delete;             // Copy construct
    MEngine(MEngine &&) = delete;                  // Move construct
    MEngine &operator=(MEngine const &) = delete;  // Copy assign
    MEngine &operator=(MEngine &&) = delete;      // Move assign

    std::list<std::string> getDevices();
    void connectDevice(std::string deviceName);

    void attachApp(android_app *app) {
        app_ = app;
    }

protected:

    MEngine() {
        // Constructor code goes here.
    }

    ~MEngine() {
        // Destructor code goes here.
    }
};

#endif //PD_MIDIOBJECT_H
