//
// Created by Admin on 07.08.2019.
//

#ifndef PD_A_H
#define PD_A_H

#include <aaudio/AAudio.h>
#include <stdlib.h>
#include <vector>

#include <jni.h>
#include <oboe/Oboe.h>
#include <string>
#include <thread>
#include "FullDuplexPass.h"

class LiveEffectEngine : public oboe::AudioStreamCallback {
public:
    LiveEffectEngine();
    ~LiveEffectEngine();
    void setRecordingDeviceId(int32_t deviceId);
    void setPlaybackDeviceId(int32_t deviceId);
    /**
     * @param isOn
     * @return true if it succeeds
     */
    bool setEffectOn(bool isOn);

    /*
     * oboe::AudioStreamCallback interface implementation
     */
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream,
                                          void *audioData, int32_t numFrames) override;
    void onErrorBeforeClose(oboe::AudioStream *oboeStream, oboe::Result error) override;
    void onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error) override;

    bool setAudioApi(oboe::AudioApi);
    bool isAAudioSupported(void);

private:
    FullDuplexPass mFullDuplexPass;
    bool mIsEffectOn = false;
    int32_t mRecordingDeviceId = oboe::kUnspecified;
    int32_t mPlaybackDeviceId = oboe::kUnspecified;
    oboe::AudioFormat mFormat = oboe::AudioFormat::I16;
    int32_t mSampleRate = oboe::kUnspecified;
    int32_t mInputChannelCount = oboe::ChannelCount::Stereo;
    int32_t mOutputChannelCount = oboe::ChannelCount::Stereo;

    oboe::ManagedStream mRecordingStream;
    oboe::ManagedStream mPlayStream;

    oboe::AudioApi mAudioApi = oboe::AudioApi::AAudio;

    oboe::Result openStreams();
    void closeStream(oboe::ManagedStream &stream);

    oboe::AudioStreamBuilder *setupCommonStreamParameters(
            oboe::AudioStreamBuilder *builder);
    oboe::AudioStreamBuilder *setupRecordingStreamParameters(
            oboe::AudioStreamBuilder *builder);
    oboe::AudioStreamBuilder *setupPlaybackStreamParameters(
            oboe::AudioStreamBuilder *builder);
    void warnIfNotLowLatency(oboe::ManagedStream &stream);
};

class AObject {

public:

    static float sample_rate;

    inline void ASetSampleRate(float sample_rate_) {sample_rate = sample_rate_;}

    virtual bool ARender(float * audioData, int numFrames) {return true;}

    virtual bool ARender(double beat, float * audioData, int numFrames) {return true;}

    virtual bool ARender(float * lsample, float * rsample) {return true;}

    virtual bool ARender(double beat, float * lsample, float * rsample) {return true;}
};

class AEngine {
public:

    AObject * root_;
    AAudioStream *inputstream;
    AAudioStream *outputstream;

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
};


#endif //PD_A_H
