#include <jni.h>
#include <android/input.h>
#include "Orchestration/Master.h"
#include "Orchestration/Midi.h"
#include "AudioEngine.h"

//static Master master;
//static AudioEngine *audioEngine = new AudioEngine(&master);

//extern "C" {
//
//// Data callback stuff
//JavaVM* theJvm;
//jobject dataCallbackObj;
//jmethodID midDataCallback;
//
//JNIEXPORT void JNICALL
//Java_com_pdaw_pd_MainActivity_touchEvent(JNIEnv *env, jobject obj, jint action, jfloat x, jfloat y) {
//    switch (action) {
//        case AMOTION_EVENT_ACTION_MOVE:
//        case AMOTION_EVENT_ACTION_DOWN:
//            master.link.enable(true);
//            break;
//        default:
//            break;
//    }
//}
//
//JNIEXPORT void JNICALL
//Java_com_pdaw_pd_TeapotNativeActivity_startEngine(JNIEnv *env, jobject /* this */) {
//    audioEngine->start();
//}
//
//JNIEXPORT void JNICALL
//Java_com_pdaw_pd_MainActivity_stopEngine(JNIEnv *env, jobject /* this */) {
//    audioEngine->stop();
//}
//
//JNIEXPORT void JNICALL
//Java_com_pdaw_pd_MainActivity_midiEvent(JNIEnv *env, jobject obj, jbyte status_byte, jbyte data_byte_1, jbyte data_byte_2) {
//    MidiData md(status_byte, data_byte_1, data_byte_2);
//    master.receiveMIDI(md);
//}
//
//}