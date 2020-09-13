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
#include <android_native_app_glue.h>
#include <map>
#include "FullDuplexStream.h"

struct ADevicesInfo {
    std::map<std::string, int> inputs;
    std::map<std::string, int> outputs;
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

class FullDuplexPass : public FullDuplexStream {
public:

    AObject * root_;

    void setRoot(AObject * root) {
        root_ = root;
    }

    virtual oboe::DataCallbackResult
    onBothStreamsReady(const void *inputData, int numInputFrames, void *outputData,
                       int numOutputFrames) {
        size_t bytesPerFrame = this->getOutputStream()->getBytesPerFrame();
        size_t bytesToWrite = numInputFrames * bytesPerFrame;
        size_t byteDiff = (numOutputFrames - numInputFrames) * bytesPerFrame;
        size_t bytesToZero = (byteDiff > 0) ? byteDiff : 0;
        memcpy(outputData, inputData, bytesToWrite);
        memset((u_char*) outputData + bytesToWrite, 0, bytesToZero);
        root_->ARender(static_cast<float *>(outputData), numOutputFrames);
        return oboe::DataCallbackResult::Continue;
    }
};

class AEngine: public oboe::AudioStreamCallback {
public:

    android_app *app_;

    ADevicesInfo adevicesinfo;

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

    ///////////////////////////////////////////

    void setInputDevice(std::string name);
    void setOutputDevice(std::string name);

    std::vector<std::string> getInputs();
    std::vector<std::string> getOutputs();

    /*
     * oboe::AudioStreamCallback interface implementation
     */
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream,
                                          void *audioData, int32_t numFrames) override;
    void onErrorBeforeClose(oboe::AudioStream *oboeStream, oboe::Result error) override;
    void onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error) override;

    bool setAudioApi(oboe::AudioApi);
    bool isAAudioSupported(void);

    inline void attachApp(android_app *app) {
        app_ = app;
    }

private:

    FullDuplexPass mFullDuplexPass;
    int32_t mRecordingDeviceId = oboe::kUnspecified;
    int32_t mPlaybackDeviceId = oboe::kUnspecified;
    oboe::AudioFormat mFormat = oboe::AudioFormat::Float;
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

    void getDevices();

protected:
    AEngine() {
        assert(mOutputChannelCount == mInputChannelCount);
    }

    ~AEngine() {
        mFullDuplexPass.stop();
        closeStream(mPlayStream);
        closeStream(mRecordingStream);
    }
};


#endif //PD_A_H
