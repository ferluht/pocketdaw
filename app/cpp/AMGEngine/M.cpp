//
// Created by Admin on 07.08.2019.
//

#include "M.h"
#include <jni.h>
#include <string>

void MEngine::sendNoteOn(uint8_t note, uint8_t velocity) {
    uint8_t data[3] = {NOTEON_HEADER, note, velocity};
    sendMidi(data, 0, 3, 0);
}

void MEngine::sendNoteOff(uint8_t note) {
    uint8_t data[3] = {NOTEOFF_HEADER, note, 0};
    sendMidi(data, 0, 3, 0);
}

void MEngine::sendCC(uint8_t num, uint8_t value) {
    uint8_t data[3] = {CC_HEADER, num, value};
    sendMidi(data, 0, 3, 0);
}

std::list<std::string> MEngine::getDevices() {
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, nullptr);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "getMidiDevices", "()[Ljava/lang/String;");
    auto devices = static_cast<jobjectArray>(jni->CallObjectMethod(app_->activity->clazz, methodID));

    std::list<std::string> ret;

    if (devices) {
        int devicesCount = jni->GetArrayLength(devices);

        for (int i = 0; i < devicesCount; i++) {
            jstring string = (jstring) (jni->GetObjectArrayElement(devices, i));
            const jclass stringClass = jni->GetObjectClass(string);
            const jmethodID getBytes = jni->GetMethodID(stringClass, "getBytes",
                                                        "(Ljava/lang/String;)[B");
            const jbyteArray stringJbytes = (jbyteArray) jni->CallObjectMethod(string, getBytes,
                                                                               jni->NewStringUTF(
                                                                                       "UTF-8"));

            size_t length = (size_t) jni->GetArrayLength(stringJbytes);
            jbyte *pBytes = jni->GetByteArrayElements(stringJbytes, NULL);

            std::string device = std::string((char *) pBytes, length);
            ret.push_back(device);
            jni->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

            jni->DeleteLocalRef(stringJbytes);
            jni->DeleteLocalRef(stringClass);
        }
    }
    app_->activity->vm->DetachCurrentThread();

    return ret;
}

void MEngine::connectDevice(std::string deviceName) {
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, nullptr);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "connectMidiDevice", "(Ljava/lang/String;)V");
    jstring jDeviceName = jni->NewStringUTF(deviceName.c_str());
    jni->CallVoidMethod(app_->activity->clazz, methodID, jDeviceName);

    app_->activity->vm->DetachCurrentThread();
}

void MEngine::sendMidi(uint8_t *data, int offset, int length, long timestamp) {
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, nullptr);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "sendMidi", "([BIIJ)V");

    jbyteArray jData = jni->NewByteArray(length);
    jni->SetByteArrayRegion(jData, 0, length, reinterpret_cast<const jbyte*>(data));

    jni->CallVoidMethod(app_->activity->clazz, methodID,
            jData, static_cast<const jint>(offset),
            static_cast<const jint>(length), static_cast<const jlong>(timestamp));

    app_->activity->vm->DetachCurrentThread();
}

extern "C" {

// Data callback stuff
JavaVM *theJvm;
jobject dataCallbackObj;
jmethodID midDataCallback;

JNIEXPORT void JNICALL
Java_com_pdaw_pd_MainActivity_midiEvent(JNIEnv *env, jobject obj, jbyte status_byte, jbyte data_byte_1, jbyte data_byte_2) {
    MData md{
        (double) 0,
        (unsigned char)status_byte,
        (unsigned char)data_byte_1,
        (unsigned char)data_byte_2
    };
    MEngine::getMEngine().putInput(md);
}
}