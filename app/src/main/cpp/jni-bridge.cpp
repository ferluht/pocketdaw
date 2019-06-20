#include <jni.h>
#include <android/input.h>
#include "AudioEngine.h"

static AudioEngine *audioEngine = new AudioEngine();

extern "C" {

// Data callback stuff
JavaVM* theJvm;
jobject dataCallbackObj;
jmethodID midDataCallback;

JNIEXPORT void JNICALL
Java_com_pdaw_pd_MainActivity_touchEvent(JNIEnv *env, jobject obj, jint action, jfloat x, jfloat y) {
    switch (action) {
        case AMOTION_EVENT_ACTION_MOVE:
        case AMOTION_EVENT_ACTION_DOWN:
            audioEngine->connectLink();
            break;
        default:
            break;
    }
}

JNIEXPORT void JNICALL
Java_com_pdaw_pd_MainActivity_startEngine(JNIEnv *env, jobject /* this */) {
    audioEngine->start();
}

JNIEXPORT void JNICALL
Java_com_pdaw_pd_MainActivity_stopEngine(JNIEnv *env, jobject /* this */) {
    audioEngine->stop();
}

JNIEXPORT void JNICALL
Java_com_pdaw_pd_MainActivity_midiReceived(JNIEnv *env, jobject obj, jbyte cmd, jbyte val1, jbyte val2) {
    switch ((unsigned char) cmd) {
        case 0x90:
            audioEngine->receiveNote(val1, val2);
            break;
        case 0xB0:
            audioEngine->receiveCC(val1, val2);
            break;
        default:
            break;
    }
}

}