//
// Created by Admin on 07.08.2019.
//

#ifndef PD_A_H
#define PD_A_H

#ifndef TARGET_IOS
    #include <aaudio/AAudio.h>
    #include <jni.h>
    #include <oboe/Oboe.h>
    #include <android_native_app_glue.h>
    #include <android/log.h>
    #include <oboe/samples/debug-utils/logging_macros.h>
    #include "FullDuplexStream.h"
#else
    //TODO
#endif

#include <stdlib.h>
#include <vector>
#include <string>
#include <thread>
#include <map>
#include <thread>
#include <mutex>

struct ADevicesInfo {
    std::map<std::string, int> inputs;
    std::map<std::string, int> outputs;
};

class AObject {

public:

    static float sample_rate;
    static float latency;

    inline void ASetSampleRate(float sample_rate_) {sample_rate = sample_rate_;}

    virtual bool ARender(float * audioData, int numFrames) {return true;}

    virtual bool ARender(const float * inputData, int inputFrames, float * outputData, int outputFrames) {
        ARender(outputData, outputFrames);
        return true;
    }

    virtual bool ARender(double beat, const float * inputData, int inputFrames, float * outputData, int outputFrames) {
        ARender(beat, outputData, outputFrames);
        return true;
    }

    virtual bool ARender(double beat, float * audioData, int numFrames) {
        if (numFrames == 1) {
            ARender(beat, audioData, audioData + 1);
        }
        return true;
    }

    virtual bool ARender(float * lsample, float * rsample) {return true;}

    virtual bool ARender(double beat, float * lsample, float * rsample) {return true;}
};

#ifndef TARGET_IOS
class FullDuplexPass : public FullDuplexStream {
public:

    AObject * root_;

    void setRoot(AObject * root) {
        root_ = root;
    }

    virtual oboe::DataCallbackResult
    onBothStreamsReady(const void *inputData, int numInputFrames, void *outputData,
                       int numOutputFrames) {
//        size_t bytesPerFrame = this->getOutputStream()->getBytesPerFrame();
//        size_t bytesToWrite = numInputFrames * bytesPerFrame;
//        size_t byteDiff = (numOutputFrames - numInputFrames) * bytesPerFrame;
//        size_t bytesToZero = (byteDiff > 0) ? byteDiff : 0;
//        memcpy(outputData, inputData, bytesToWrite);
//        memset((u_char*) outputData + bytesToWrite, 0, bytesToZero);
        root_->ARender(static_cast<const float *>(inputData), numInputFrames, static_cast<float *>(outputData), numOutputFrames);
        return oboe::DataCallbackResult::Continue;
    }
};
#else
    //TODO
#endif

#ifndef TARGET_IOS
class AEngine: public oboe::AudioStreamCallback {
    android_app *app_;
#else
class AEngine {
#endif
    
public:

    char storage_root[200];

    ADevicesInfo adevicesinfo;

    AObject * root_;
    
#ifndef TARGET_IOS
    AAudioStream *inputstream;
    AAudioStream *outputstream;
#else
    //TODO
#endif

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

#ifndef TARGET_IOS
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
#else
    //TODO
#endif

    void getRootDirectory();

private:

#ifndef TARGET_IOS
    FullDuplexPass mFullDuplexPass;
    
    int32_t mRecordingDeviceId = oboe::kUnspecified;
    int32_t mPlaybackDeviceId = oboe::kUnspecified;
    int32_t mSampleRate = oboe::kUnspecified;
    int32_t mInputChannelCount = oboe::ChannelCount::Stereo;
    int32_t mOutputChannelCount = oboe::ChannelCount::Stereo;
    
    oboe::AudioFormat mFormat = oboe::AudioFormat::Float;
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
#else
    //TODO
#endif
    
    void getDevices();

protected:
    AEngine() {
#ifndef TARGET_IOS
        assert(mOutputChannelCount == mInputChannelCount);
#else
        //TODO
#endif
    }

    ~AEngine() {
#ifndef TARGET_IOS
        mFullDuplexPass.stop();
        closeStream(mPlayStream);
        closeStream(mRecordingStream);
#else
        //TODO
#endif
    }
};


#endif //PD_A_H
