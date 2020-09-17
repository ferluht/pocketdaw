//
// Created by Admin on 07.08.2019.
//

#ifndef PD_MIDIOBJECT_H
#define PD_MIDIOBJECT_H

#ifndef TARGET_IOS
    #include <jni.h>
    #include <android_native_app_glue.h>
    #include <android/native_window_jni.h>
#else
    //TODO
#endif

#include <stdlib.h>
#include <vector>
#include <queue>
#include <cmath>
#include <list>
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

struct MData {
    double beat;
    unsigned char status;
    unsigned char data1;
    unsigned char data2;
};

//bool operator!=(const MData& a1, const MData& a2)
//{
//    return (a1.beat != a2.beat) || (a1.status != a2.status) || (a1.data1 != a2.data1) ||
//           (a1.data2 != a2.data2);
//}

const MData NULLMIDI = {0,0,0,0};

class MObject {

private:

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

    virtual void MRender(double beat) {}
};

class MEngine : public MObject {

#ifndef TARGET_IOS
    android_app *app_;
#else
    //TODO
#endif

    std::list<MData> incoming;

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
    void sendMidi(uint8_t * data, int offset, int length, long timestamp);
    void sendNoteOn(uint8_t note, uint8_t velocity);
    void sendNoteOff(uint8_t note);
    void sendCC(uint8_t num, uint8_t value);
    inline MData getInput() {
        if (!incoming.empty()) {
            auto ret = incoming.front();
            incoming.pop_front();
            return ret;
        }
        return NULLMIDI;
    }

    inline void putInput(MData msg) {
        incoming.push_back(msg);
    }

#ifndef TARGET_IOS
    inline void attachApp(android_app *app) {
        app_ = app;
    }
#else
    //TODO
#endif

protected:

    MEngine() {
        // Constructor code goes here.
    }

    ~MEngine() {
        // Destructor code goes here.
    }
};

#endif //PD_MIDIOBJECT_H
