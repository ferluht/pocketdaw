//
// Created by Admin on 07.08.2019.
//

#include "A.h"

float AObject::sample_rate;
float AObject::latency = 0;

bool AEngine::start() {
#ifndef TARGET_IOS
    if (isAAudioSupported())
        setAudioApi(oboe::AudioApi::AAudio);
    else
        setAudioApi(oboe::AudioApi::OpenSLES);
    AObject::sample_rate = 48000;
    if ((mRecordingDeviceId == oboe::kUnspecified) ||
        (mPlaybackDeviceId == oboe::kUnspecified)) {
        getDevices();
        setInputDevice(getInputs()[0]);
        setOutputDevice(getOutputs()[1]);
    }
    auto success = openStreams() == oboe::Result::OK;
    if (success) {
        mFullDuplexPass.setRoot(root_);
        mFullDuplexPass.start();
        return true;
    }
    return false;
#else
    //TODO
    return true;
#endif
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
#ifndef TARGET_IOS
    mFullDuplexPass.stop();
    /*
    * Note: The order of events is important here.
    * The playback stream must be closed before the recording stream. If the
    * recording stream were to be closed first the playback stream's
    * callback may attempt to read from the recording stream
    * which would cause the app to crash since the recording stream would be
    * null.
    */
    closeStream(mPlayStream);
    closeStream(mRecordingStream);
#else
    // TODO
#endif
}




///////////////////////////////////////////////////

std::vector<std::string> AEngine::getInputs() {
    std::vector<std::string> ret;
    for (auto const& info : adevicesinfo.inputs)
        ret.push_back(info.first);
    return ret;
}

std::vector<std::string> AEngine::getOutputs() {
    std::vector<std::string> ret;
    for (auto const& info : adevicesinfo.outputs)
        ret.push_back(info.first);
    return ret;
}


void AEngine::setInputDevice(std::string name){
#ifndef TARGET_IOS
    auto device = adevicesinfo.inputs.find(name);
    mRecordingDeviceId = device->second;
#else
    
#endif
}

void AEngine::setOutputDevice(std::string name) {
#ifndef TARGET_IOS
    auto device = adevicesinfo.outputs.find(name);
    mPlaybackDeviceId = device->second;
#else
    
#endif
}

#ifndef TARGET_IOS
bool AEngine::isAAudioSupported() {
    oboe::AudioStreamBuilder builder;
    return builder.isAAudioSupported();
}

bool AEngine::setAudioApi(oboe::AudioApi api) {
    mAudioApi = api;
    return true;
}

oboe::Result  AEngine::openStreams() {
    // Note: The order of stream creation is important. We create the playback
    // stream first, then use properties from the playback stream
    // (e.g. sample rate) to create the recording stream. By matching the
    // properties we should get the lowest latency path
    oboe::AudioStreamBuilder inBuilder, outBuilder;
    setupPlaybackStreamParameters(&outBuilder);
    oboe::Result result = outBuilder.openManagedStream(mPlayStream);
    if (result != oboe::Result::OK) {
        mSampleRate = oboe::kUnspecified;
        return result;
    } else {
        mSampleRate = mPlayStream->getSampleRate();
    }
    warnIfNotLowLatency(mPlayStream);

    setupRecordingStreamParameters(&inBuilder);
    result = inBuilder.openManagedStream(mRecordingStream);
    if (result != oboe::Result::OK) {
        closeStream(mPlayStream);
        return result;
    }
    warnIfNotLowLatency(mRecordingStream);

    mFullDuplexPass.setInputStream(mRecordingStream.get());
    mFullDuplexPass.setOutputStream(mPlayStream.get());
    return result;
}

/**
 * Sets the stream parameters which are specific to recording,
 * including the sample rate which is determined from the
 * playback stream.
 *
 * @param builder The recording stream builder
 */
oboe::AudioStreamBuilder *AEngine::setupRecordingStreamParameters(
        oboe::AudioStreamBuilder *builder) {
    // This sample uses blocking read() by setting callback to null
    builder->setCallback(nullptr)
            ->setDeviceId(mRecordingDeviceId)
            ->setDirection(oboe::Direction::Input)
            ->setSampleRate(mSampleRate)
            ->setChannelCount(mInputChannelCount);
    return setupCommonStreamParameters(builder);
}

/**
 * Sets the stream parameters which are specific to playback, including device
 * id and the dataCallback function, which must be set for low latency
 * playback.
 * @param builder The playback stream builder
 */
oboe::AudioStreamBuilder *AEngine::setupPlaybackStreamParameters(
        oboe::AudioStreamBuilder *builder) {
    builder->setCallback(this)
            ->setDeviceId(mPlaybackDeviceId)
            ->setDirection(oboe::Direction::Output)
            ->setChannelCount(mOutputChannelCount);

    return setupCommonStreamParameters(builder);
}

/**
 * Set the stream parameters which are common to both recording and playback
 * streams.
 * @param builder The playback or recording stream builder
 */
oboe::AudioStreamBuilder *AEngine::setupCommonStreamParameters(
        oboe::AudioStreamBuilder *builder) {
    // We request EXCLUSIVE mode since this will give us the lowest possible
    // latency.
    // If EXCLUSIVE mode isn't available the builder will fall back to SHARED
    // mode.
    builder->setAudioApi(mAudioApi)
            ->setFormat(mFormat)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency);
    return builder;
}


/**
 * Close the stream. AudioStream::close() is a blocking call so
 * the application does not need to add synchronization between
 * onAudioReady() function and the thread calling close().
 * [the closing thread is the UI thread in this sample].
 * @param stream the stream to close
 */
void AEngine::closeStream(oboe::ManagedStream &stream) {
    if (stream) {
        oboe::Result result = stream->close();
        if (result != oboe::Result::OK) {
            LOGE("Error closing stream. %s", oboe::convertToText(result));
        }
        LOGW("Successfully closed streams");
        stream.reset();
    }
}


/**
 * Warn in logcat if non-low latency stream is created
 * @param stream: newly created stream
 *
 */
void AEngine::warnIfNotLowLatency(oboe::ManagedStream &stream) {
    if (stream->getPerformanceMode() != oboe::PerformanceMode::LowLatency) {
        LOGW(
                "Stream is NOT low latency."
                "Check your requested format, sample rate and channel count");
    }
}

/**
 * Handles playback stream's audio request. In this sample, we simply block-read
 * from the record stream for the required samples.
 *
 * @param oboeStream: the playback stream that requesting additional samples
 * @param audioData:  the buffer to load audio samples for playback stream
 * @param numFrames:  number of frames to load to audioData buffer
 * @return: DataCallbackResult::Continue.
 */
oboe::DataCallbackResult AEngine::onAudioReady(
        oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    return mFullDuplexPass.onAudioReady(oboeStream, audioData, numFrames);
}

/**
 * Oboe notifies the application for "about to close the stream".
 *
 * @param oboeStream: the stream to close
 * @param error: oboe's reason for closing the stream
 */
void AEngine::onErrorBeforeClose(oboe::AudioStream *oboeStream,
                                          oboe::Result error) {
    LOGE("%s stream Error before close: %s",
         oboe::convertToText(oboeStream->getDirection()),
         oboe::convertToText(error));
}

/**
 * Oboe notifies application that "the stream is closed"
 *
 * @param oboeStream
 * @param error
 */
void AEngine::onErrorAfterClose(oboe::AudioStream *oboeStream,
                                         oboe::Result error) {
    LOGE("%s stream Error after close: %s",
         oboe::convertToText(oboeStream->getDirection()),
         oboe::convertToText(error));
}
#else
    // TODO
#endif


//extern "C" {
//
//// Data callback stuff
//JavaVM *theJvm;
//jobject dataCallbackObj;
//jmethodID midDataCallback;
//
//JNIEXPORT void JNICALL
//Java_com_pdaw_pd_MainActivity_midiEvent(JNIEnv *env, jobject obj, jbyte status_byte, jbyte data_byte_1, jbyte data_byte_2) {
//    MData md{
//            (double) 0,
//            (unsigned char)status_byte,
//            (unsigned char)data_byte_1,
//            (unsigned char)data_byte_2
//    };
//    MEngine::getMEngine().putInput(md);
//}
//}


void AEngine::getDevices() {
#ifndef TARGET_IOS
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, nullptr);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID getInputs = jni->GetMethodID(clazz, "getInputAudioDevices", "()[Ljava/lang/String;");
    jmethodID getOutputs = jni->GetMethodID(clazz, "getOutputAudioDevices", "()[Ljava/lang/String;");
    auto in_devices = static_cast<jobjectArray>(jni->CallObjectMethod(app_->activity->clazz, getInputs));
    auto out_devices = static_cast<jobjectArray>(jni->CallObjectMethod(app_->activity->clazz, getOutputs));

    adevicesinfo.inputs.clear();
    adevicesinfo.outputs.clear();

    if (in_devices) {
        int devicesCount = jni->GetArrayLength(in_devices);

        for (int i = 0; i < devicesCount; i++) {
            jstring string = (jstring) (jni->GetObjectArrayElement(in_devices, i));
            const jclass stringClass = jni->GetObjectClass(string);
            const jmethodID getBytes = jni->GetMethodID(stringClass, "getBytes",
                                                        "(Ljava/lang/String;)[B");
            const jbyteArray stringJbytes = (jbyteArray) jni->CallObjectMethod(string, getBytes,
                                                                               jni->NewStringUTF(
                                                                                       "UTF-8"));

            size_t length = (size_t) jni->GetArrayLength(stringJbytes);
            jbyte *pBytes = jni->GetByteArrayElements(stringJbytes, NULL);

            std::string device_info = std::string((char *) pBytes, length);
            auto comma = device_info.find(",");
            std::string device_name = device_info.substr(0, comma);
            int id = std::stoi(device_info.erase(0, comma + 1));

            adevicesinfo.inputs.insert({device_name, id});
            jni->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

            jni->DeleteLocalRef(stringJbytes);
            jni->DeleteLocalRef(stringClass);
        }
    }

    if (out_devices) {
        int devicesCount = jni->GetArrayLength(out_devices);

        for (int i = 0; i < devicesCount; i++) {
            jstring string = (jstring) (jni->GetObjectArrayElement(out_devices, i));
            const jclass stringClass = jni->GetObjectClass(string);
            const jmethodID getBytes = jni->GetMethodID(stringClass, "getBytes",
                                                        "(Ljava/lang/String;)[B");
            const jbyteArray stringJbytes = (jbyteArray) jni->CallObjectMethod(string, getBytes,
                                                                               jni->NewStringUTF(
                                                                                       "UTF-8"));

            size_t length = (size_t) jni->GetArrayLength(stringJbytes);
            jbyte *pBytes = jni->GetByteArrayElements(stringJbytes, NULL);

            std::string device_info = std::string((char *) pBytes, length);
            auto comma = device_info.find(",");
            std::string device_name = device_info.substr(0, comma);
            int id = std::stoi(device_info.erase(0, comma + 1));

            adevicesinfo.outputs.insert({device_name, id});
            jni->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

            jni->DeleteLocalRef(stringJbytes);
            jni->DeleteLocalRef(stringClass);
        }
    }

    app_->activity->vm->DetachCurrentThread();
#else
    // TODO
#endif
}


void AEngine::getRootDirectory() {
#ifndef TARGET_IOS
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, nullptr);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "getStorageRoot", "()Ljava/lang/String;");
    jstring s = (jstring) jni->CallObjectMethod(app_->activity->clazz, methodID);

    sprintf(storage_root, "%s" , jni->GetStringUTFChars(s, 0));

    app_->activity->vm->DetachCurrentThread();
#else
    // TODO
#endif
}
