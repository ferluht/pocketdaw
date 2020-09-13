//
// Created by Admin on 07.08.2019.
//

#include "A.h"
#include <android/log.h>
#include <thread>
#include <mutex>

float AObject::sample_rate;

// Double-buffering offers a good tradeoff between latency and protection against glitches.
constexpr int32_t kBufferSizeInBursts = 4;

aaudio_data_callback_result_t dataCallback(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames) {

    AAudioStream *inputStream = ((AEngine *) userData)->inputstream;
    int64_t timeout = 0;
    aaudio_result_t result =
            AAudioStream_read(inputStream, audioData, numFrames, timeout);

//    if (result == numFrames)
//        return AAUDIO_CALLBACK_RESULT_STOP;
//    if (result >= 0) {
//        memset(static_cast<float*>(audioData) + result * 2, 0,
//               sizeof(float) * (numFrames - result) * 2);
//        return AAUDIO_CALLBACK_RESULT_CONTINUE;
//    }
//    return AAUDIO_CALLBACK_RESULT_STOP;

    for(int i = numFrames; i < numFrames * 2; i++) {
        (static_cast<float *>(audioData))[i] = (static_cast<float *>(audioData))[i - numFrames];
    }

    ((AEngine *) userData)->root_->ARender(static_cast<float *>(audioData), numFrames);
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void errorCallback(AAudioStream *stream,
                   void *userData,
                   aaudio_result_t error){
    if (error == AAUDIO_ERROR_DISCONNECTED){
        std::function<void(void)> restartFunction = std::bind(&AEngine::restart,
                                                              static_cast<AEngine *>(userData));
        new std::thread(restartFunction);
    }
}

bool AEngine::start() {
    AAudioStreamBuilder *streamBuilder;
    AAudio_createStreamBuilder(&streamBuilder);
//    AAudioStreamBuilder_setDeviceId(streamBuilder, 0);
    AAudioStreamBuilder_setFormat(streamBuilder, AAUDIO_FORMAT_PCM_FLOAT);
    AAudioStreamBuilder_setDirection(streamBuilder, AAUDIO_DIRECTION_INPUT);
    AAudioStreamBuilder_setSharingMode(streamBuilder, AAUDIO_SHARING_MODE_SHARED);
    AAudioStreamBuilder_setChannelCount(streamBuilder, 1);
//    AAudioStreamBuilder_setPerformanceMode(streamBuilder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
    AAudioStreamBuilder_setErrorCallback(streamBuilder, ::errorCallback, this);

    // Opens the stream.
    aaudio_result_t result = AAudioStreamBuilder_openStream(streamBuilder, &inputstream);
    if (result != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AEngine", "Error opening stream %s",
                            AAudio_convertResultToText(result));
        return false;
    }


    AAudio_createStreamBuilder(&streamBuilder);
    AAudioStreamBuilder_setFormat(streamBuilder, AAUDIO_FORMAT_PCM_FLOAT);
    AAudioStreamBuilder_setDirection(streamBuilder, AAUDIO_DIRECTION_OUTPUT);
    AAudioStreamBuilder_setSharingMode(streamBuilder, AAUDIO_SHARING_MODE_SHARED);
    AAudioStreamBuilder_setChannelCount(streamBuilder, 2);
    AAudioStreamBuilder_setPerformanceMode(streamBuilder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
    AAudioStreamBuilder_setDataCallback(streamBuilder, ::dataCallback, this);
    AAudioStreamBuilder_setErrorCallback(streamBuilder, ::errorCallback, this);

    // Opens the stream.
    result = AAudioStreamBuilder_openStream(streamBuilder, &outputstream);
    if (result != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AEngine", "Error opening stream %s",
                            AAudio_convertResultToText(result));
        return false;
    }

//    // Retrieves the sample rate of the stream for our oscillator.
    int32_t sampleRate = AAudioStream_getSampleRate(outputstream);
    root_->ASetSampleRate(sampleRate);

    // Sets the buffer size.
    AAudioStream_setBufferSizeInFrames(
            outputstream, AAudioStream_getFramesPerBurst(outputstream) * kBufferSizeInBursts);

    AAudioStream_setBufferSizeInFrames(
            inputstream, AAudioStream_getFramesPerBurst(inputstream) * kBufferSizeInBursts);

    // Starts the stream.
    result = AAudioStream_requestStart(outputstream);
    if (result != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AEngine", "Error starting stream %s",
                            AAudio_convertResultToText(result));
        return false;
    }

    // Starts the stream.
    result = AAudioStream_requestStart(inputstream);
    if (result != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AEngine", "Error starting stream %s",
                            AAudio_convertResultToText(result));
        return false;
    }

    AAudioStreamBuilder_delete(streamBuilder);
    return true;
}

void AEngine::restart(){

    static std::mutex restartingLock;
    if (restartingLock.try_lock()){
        stop();
        start();
        restartingLock.unlock();
    }
}

void AEngine::stop() {
    if (inputstream != nullptr) {
        AAudioStream_requestStop(inputstream);
        AAudioStream_close(inputstream);
    }
    if (outputstream != nullptr) {
        AAudioStream_requestStop(outputstream);
        AAudioStream_close(outputstream);
    }
}