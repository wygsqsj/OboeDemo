#include <jni.h>
#include <string>
#include <oboe/AudioStreamBuilder.h>
#include <oboe/Oboe.h>
#include <oboe/AudioStream.h>
#include "AudioEngine.h"
#include <thread>


AudioEngine *engine = new AudioEngine;


void start_record(char *pcmPath) {
    engine->start(pcmPath);
}


void stop_record() {
    engine->stop();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_oboedemo_MainActivity_startRecord(JNIEnv *env, jobject thiz, jstring pcmPath_) {
    char *pcmPath = const_cast<char *>(env->GetStringUTFChars(pcmPath_, 0));
    start_record(pcmPath);

    env->ReleaseStringUTFChars(pcmPath_, pcmPath);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_oboedemo_MainActivity_stopRecord(JNIEnv *env, jobject thiz) {
    stop_record();
//    pthread_exit(&recordThread);
}



