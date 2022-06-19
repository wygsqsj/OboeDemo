#include <jni.h>
#include <string>
#include <oboe/AudioStreamBuilder.h>
#include <oboe/Oboe.h>
#include <oboe/AudioStream.h>
#include "AudioRecord.h"
#include "AudioPlay.h"
#include <thread>


AudioRecord *engine = new AudioRecord;


void start_record(char *pcmPath) {
    engine->start(pcmPath);
}

void stop_record() {
    engine->stop();
}

AudioPlay *play = new AudioPlay;

void start_play(char *mp3Path) {
    play->start(mp3Path);
}

void stop_play() {
    play->stop();
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



//开始播放mp3
extern "C"
JNIEXPORT void JNICALL
Java_com_example_oboedemo_MainActivity_startPlay(JNIEnv *env, jobject thiz, jstring _mp3_path) {
    char *mp3_path = const_cast<char *>(env->GetStringUTFChars(_mp3_path, 0));
    start_play(mp3_path);
    env->ReleaseStringUTFChars(_mp3_path, mp3_path);
}

//停止播放mp3
extern "C"
JNIEXPORT void JNICALL
Java_com_example_oboedemo_MainActivity_stopPlay(JNIEnv *env, jobject thiz) {
    stop_play();
}