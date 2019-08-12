//
// Created by Admin on 07.08.2019.
//

#include "M.h"
#include <jni.h>

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
    MEngine::getMEngine().MOut(md);
}

}