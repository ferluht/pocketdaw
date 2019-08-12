//
// Created by Admin on 07.08.2019.
//

#ifndef PD_A_H
#define PD_A_H

#include <aaudio/AAudio.h>
#include <stdlib.h>
#include <vector>

class AObject {

public:

    static float sample_rate;

    inline void ASetSampleRate(float sample_rate_) {sample_rate = sample_rate_;}

    virtual void ARender(float * audioData, int numFrames) {}

    virtual void ARender(float * lsample, float * rsample) {};

    virtual void ARender(double beat, float * lsample, float * rsample) {};
};

class AEngine {
public:

    bool start();
    void stop();
    void restart();

    void setRoot(AObject * root) {
        root_ = root;
    }

    static AEngine& getAEngine() {
        static AEngine aengine;
        return aengine;
    }

    AEngine(AEngine const&) = delete;             // Copy construct
    AEngine(AEngine&&) = delete;                  // Move construct
    AEngine& operator=(AEngine const&) = delete;  // Copy assign
    AEngine& operator=(AEngine &&) = delete;      // Move assign

protected:
    AEngine() {
        // Constructor code goes here.
    }

    ~AEngine() {
        // Destructor code goes here.
    }

    AObject * root_;
    AAudioStream *stream_;
};


#endif //PD_A_H
