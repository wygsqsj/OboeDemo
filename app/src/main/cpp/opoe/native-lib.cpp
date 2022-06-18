#include <jni.h>
#include <string>
#include <oboe/AudioStreamBuilder.h>
#include <oboe/Oboe.h>
#include <oboe/AudioStream.h>

class MyCallback : public oboe::AudioStreamCallback {
public:
    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {


        auto *outputData = static_cast<float *>(audioData);

        const float amplitude = 0.2f;
        for (int i = 0; i < numFrames; ++i) {
            outputData[i] = ((float) drand48() - 0.5f) * 2 * amplitude;
        }

        return oboe::DataCallbackResult::Continue;
    }
};

void start_record();

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_oboedemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject thiz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}



extern "C"
JNIEXPORT void JNICALL
Java_com_example_oboedemo_MainActivity_startRecord(JNIEnv *env, jobject thiz) {
    start_record();
}

void start_record() {

}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_oboedemo_MainActivity_stopRecord(JNIEnv *env, jobject thiz) {
}



