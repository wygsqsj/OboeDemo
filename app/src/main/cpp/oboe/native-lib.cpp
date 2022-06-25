#include <jni.h>
#include <string>
#include <oboe/AudioStreamBuilder.h>
#include <oboe/Oboe.h>
#include <oboe/AudioStream.h>
#include "AudioRecord.h"
#include "AudioPlay.h"
#include <thread>
#include <stdlib.h>

#include "flux_base.h"
#include "resample_algorithm.h"
#include "audio_resampler.h"

AudioRecord *engine = new AudioRecord;

constexpr float kScaleI16ToFloat = 1.0f / 32768.0f;

void convertPcm16ToFloat(short *iArr, float *fArr, int size) {
    for (int i = 0; i < size; i++) {
        float val = iArr[i] * kScaleI16ToFloat;
        if (val > 1) val = 1;
        if (val < -1) val = -1;
        fArr[i] = val;
    }
}

void convertFloatToPcm16(const float *source, short *destination, int32_t numSamples) {
    for (int i = 0; i < numSamples; i++) {
        float fval = source[i];
//        fval += 1.0; // to avoid discontinuity at 0.0 caused by truncation
        fval *= 32768.0f;
        int32_t sample = static_cast<int32_t>(fval);
        // clip to 16-bit range
        if (sample < -32768) sample = -32768;
        if (sample > 32767) sample = 32767;
        destination[i] = sample;
    }
}

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

extern "C"
long datetime_getCurrentMillisecond() {
    struct timeval tv;
    long time = 0;
    gettimeofday(&tv, NULL);
    time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return time;
}


long startTime, endTime;

//新版重采样
extern "C"
JNIEXPORT void JNICALL
Java_com_example_oboedemo_MainActivity_startReSampling(JNIEnv *env, jobject thiz,
                                                       jint _resampleQualityType) {

    //重采样原始数据
    FILE *beforeFile = fopen("/sdcard/Android/data/com.example.oboedemo/files/convertBefore.pcm",
                             "rb+");
    //重采样后的数据，float形式
    FILE *laterFile = fopen(
            "/sdcard/Android/data/com.example.oboedemo/files/convertLaterFloat.pcm", "wb+");

    ResampleQualityType resampleQualityType = static_cast<ResampleQualityType>(_resampleQualityType);
    char *qualityTypeName;
    switch (resampleQualityType) {
        case 0:
            qualityTypeName = "best.pcm";
            break;
        case 1:
            qualityTypeName = "Mid.pcm";
            break;
        case 2:
            qualityTypeName = "Fast.pcm";
            break;
    }
    char *basePath = "/sdcard/Android/data/com.example.oboedemo/files/convertLaterShort";
    char *shortPcmPath = (char *) calloc(1, strlen(basePath) + strlen(qualityTypeName) + 1);
    strcpy(shortPcmPath, basePath);
    strcat(shortPcmPath, qualityTypeName);
    //重采样后的数据，short形式
    FILE *laterShortFile = fopen(shortPcmPath, "wb+");

    //构建重采样对象
    ResampleObj resampleObj = NULL;
    int isScale = 1;
    int isContinue = 1;
    resampleObj_new(&resampleObj, &resampleQualityType, &isScale, &isContinue);
/***
	sinc right
	zeroNum 64 一般16/32/64
	nbit 9 一般5~9 每个zero-cross 1<<nbit samples
	winType hann
	value kaiser/gauss 5/2.5
	rollOff 0.945 一般0.8~0.95
****/
    /*   int zeroNum = 64;
       int nbit = 9;
       WindowType windowType = Window_Hann;
       float value = 2.5;
       float rollOff = 0.945;
       resampleObj_newWithWindow(&resampleObj, &zeroNum, &nbit, &windowType, &value,
                                 &rollOff, &isScale, &isContinue);*/
    //设置采样率
    resampleObj_setSamplate(resampleObj, 44100, 32000);
    //是否是连续重采样
    resampleObj_enableContinue(resampleObj, isContinue);

    //获取pcm数据长度
    int fd1 = fileno(beforeFile);
    //将读写位置移到文件尾为了获取文件大小
    int beforeFileLength = lseek(fd1, 0, SEEK_END);
    //再设置偏移到文件开头
    fseek(beforeFile, 0, SEEK_SET);

    /* //一次性写入
     //原始的数据长度,对应转换后的
    int sLen = beforeFileLength * sizeof(uint8_t) * 1.0f / sizeof(short);
    short leftPcmArr[sLen];
    //从文件中获取pcm原始数据
    fread(leftPcmArr, beforeFileLength * sizeof(uint8_t), 1, beforeFile);

    LOGI("全部的数据长度：%d ,short 数据长度:%d", beforeFileLength, sLen);
      startTime = datetime_getCurrentMillisecond();
      //设置重采样大小
     resampleObj_calDataLength(resampleObj, beforeFileLength);

     //重采样后的数据大小
     float *floatPcmArr = static_cast<float *>(malloc(sizeof(float) * sLen));
     convertPcm16ToFloat(leftPcmArr, floatPcmArr, sLen);

      float *floatTarget = static_cast<float *>(malloc(sizeof(float) * sLen));
      int resLen = resampleObj_resample(resampleObj, floatPcmArr, sLen, floatTarget);

      endTime = datetime_getCurrentMillisecond();
      LOGI("新版重采样时间：%f ,resampleQualityType:%d", (endTime - startTime) / 1000.0,
           resampleQualityType);

      LOGI("原始数据长度 beforeFileLength = %d  resLen = %d ", beforeFileLength, resLen);

      int16_t *shortTarget = static_cast<int16_t *>(malloc(sizeof(float) / sizeof(short) * resLen));
      convertFloatToPcm16(floatTarget, shortTarget, resLen);
      fwrite(shortTarget, resLen * sizeof(float) / sizeof(short), 1, laterShortFile);

      fwrite(floatTarget, resLen * sizeof(float), 1, laterFile);

      LOGI("全部写入完成,返回的float数据长度为：%d", resLen);*/

    //多次写入
    //原始的数据长度,对应转换后的
    int sLen = beforeFileLength * sizeof(uint8_t);
    uint8_t *leftPcmArr = static_cast<uint8_t *>(malloc(sLen));
    //从文件中获取pcm原始数据
    fread(leftPcmArr, sLen, 1, beforeFile);

    int currentIndex = 0;//在原始数据中的位置
    int resFloatLength = 0, oriFloatLength = 0; //float数据个数
    int curSLen = 5733 * 2; //index 在原始数据中，字节

    int floatPcmLength = curSLen / 2;

    //生成的全部数据
    float *floatAllTarget = static_cast<float *>(malloc(sizeof(float) * beforeFileLength / 2));

    //不断塞入数据
    float *curFloatPcmArr = static_cast<float *>(malloc(sizeof(float) * floatPcmLength));
    uint8_t *curUint8PcmArr = reinterpret_cast<uint8_t *>(static_cast<float *>(malloc(
            sizeof(uint8_t) * curSLen)));

    bool isFirst = true;
    for (int i = 0; i < beforeFileLength;) {
//        if (i + curSLen > beforeFileLength && isFirst) {
//            curSLen = beforeFileLength - i;
//            LOGI("到达末尾 i = %d,curSLen=%d ,floatPcmLength:%d", i, curSLen, floatPcmLength);
//            isFirst = false;
//        }

        LOGI("当前oriFloatPcm 数据:%d,%d", *leftPcmArr, *(leftPcmArr + 1));

        memset(curUint8PcmArr, 0, sizeof(uint8_t) * curSLen);
        //复制到新的数组中
        memcpy(curUint8PcmArr, leftPcmArr + i, sizeof(uint8_t) * curSLen);

        LOGI("当前 curUint8PcmArr 数据:%d,%d", *curUint8PcmArr, *(curUint8PcmArr + 1));

        //uint8 转成 float
        oboe::convertPcm16ToFloat(reinterpret_cast<const int16_t *>(curUint8PcmArr), curFloatPcmArr,
                                  floatPcmLength);

        //设置重采样大小
        resampleObj_calDataLength(resampleObj, curSLen);

        //重采样
        int resLen = resampleObj_resample(resampleObj, curFloatPcmArr,
                                          curSLen / 2,
                                          floatAllTarget + resFloatLength);
        oriFloatLength += curSLen / 2;
        resFloatLength += resLen;

        //写入float数据
        fwrite(floatAllTarget + resFloatLength - resLen, resLen * sizeof(float), 1, laterFile);
        fflush(laterFile);

        int16_t *shortTarget = static_cast<int16_t *>(malloc(sizeof(short) * resLen));
        //将重采样后的float转成short
        convertFloatToPcm16(floatAllTarget + resFloatLength - resLen, shortTarget, resLen);
        //写入short数据
        fwrite(shortTarget, resLen * sizeof(int16_t), 1, laterShortFile);
        fflush(laterShortFile);

        LOGI("原始数据长度 beforeFileLength = %d  resLen = %d, curSLen长度 = %d,i = %d", beforeFileLength,
             resLen,
             curSLen, i);

        i += curSLen;
    }


    /* while (currentIndex < beforeFileLength) {
         int split = currentIndex + curSLen;
         LOGI("当前indext:%d ,split:%d, 总数量： %d", currentIndex, split, sLen);
         uint8_t *curPcmArr = static_cast<uint8_t *>(malloc(sizeof(uint8_t) * curSLen));
         for (; currentIndex < split; ++currentIndex) {
             if (currentIndex < beforeFileLength) {
                 curPcmArr[currentIndex % curSLen] = leftPcmArr[currentIndex];
             } else {
                 curSLen = currentIndex % curSLen;
                 LOGI("读取到最后 currentIndex:%d", currentIndex);
                 break;
             }
         }
         //设置重采样大小
         resampleObj_calDataLength(resampleObj, curSLen / 2);

         //将当前的short转换成float

         //short数据个数 = float数据个数
         int floatPcmLength = curSLen * sizeof(uint8_t) / sizeof(short);

         float *floatPcmArr = static_cast<float *>(malloc(sizeof(float) * floatPcmLength));
         oboe::convertPcm16ToFloat(reinterpret_cast<const int16_t *>(curPcmArr), floatPcmArr,
                                   floatPcmLength);

         //重采样后的数据大小
         float *floatTarget = static_cast<float *>(
                 malloc(sizeof(float) * floatPcmLength));
         oriFloatLength += floatPcmLength;

         //重采样
         int resLen = resampleObj_resample(resampleObj, floatPcmArr, floatPcmLength, floatTarget);

         resFloatLength += resLen;

         LOGI("原始数据长度 beforeFileLength = %d  resLen = %d curSLen长度 = %d", beforeFileLength, resLen,
              curSLen);

         //写入float数据
         fwrite(floatTarget, resLen * sizeof(float), 1, laterFile);

         LOGI("写入short数据");
         short *shortTarget = static_cast<int16_t *>(malloc(sizeof(short) * resLen));
         //将重采样后的float转成short
         oboe::convertFloatToPcm16(floatTarget, shortTarget, resLen);
         //写入short数据
         fwrite(shortTarget, resLen * sizeof(short), 1, laterShortFile);
     }*/

    LOGI("全部写入完成,返回的float数据长度为：%d,原先float数据长度：%d", resFloatLength, oriFloatLength);

    /*//多次写入float形式
    //原始的数据长度,对应转换后的
    int sLen = beforeFileLength * sizeof(uint8_t);
    int floatPcmLength = sLen / sizeof(short);
    short leftPcmArr[floatPcmLength];
    //从文件中获取pcm原始数据
    fread(leftPcmArr, floatPcmLength * sizeof(short), 1, beforeFile);


    float *oriFloatPcm = static_cast<float *>(malloc(sizeof(float) * floatPcmLength));
    convertPcm16ToFloat(leftPcmArr, oriFloatPcm, floatPcmLength);
    int resFloatLength = 0, oriFloatLength = 0;

    //生成的全部数据
    float *floatAllTarget = static_cast<float *>(malloc(sizeof(float) * floatPcmLength));

    //不断塞入数据
    int curSLen = 5733;
    float *curFloatPcmArr = static_cast<float *>(malloc(sizeof(float) * curSLen));

    bool isFirst = true;
    for (int i = 0; i < floatPcmLength;) {
        if (i + curSLen > floatPcmLength && isFirst) {
            curSLen = floatPcmLength - i;
            LOGI("到达末尾 i = %d,curSLen=%d ,floatPcmLength:%d", i, curSLen, floatPcmLength);
            isFirst = false;
        }

        LOGI("当前oriFloatPcm 数据:%f,%f", *oriFloatPcm, *(oriFloatPcm + 1));

        memset(curFloatPcmArr, 0, sizeof(float) * curSLen);
        //复制到新的数组中
        memcpy(curFloatPcmArr, oriFloatPcm + i, sizeof(float) * curSLen);

        LOGI("当前 curFloatPcmArr 数据:%f,%f", *curFloatPcmArr, *(curFloatPcmArr + 1));

        //设置重采样大小
        resampleObj_calDataLength(resampleObj, curSLen);

        //重采样
        int resLen = resampleObj_resample(resampleObj, curFloatPcmArr,
                                          curSLen,
                                          floatAllTarget + resFloatLength);
        oriFloatLength += curSLen;
        resFloatLength += resLen;

        //写入float数据
        fwrite(floatAllTarget + resFloatLength - resLen, resLen * sizeof(float), 1, laterFile);
        fflush(laterFile);

        int16_t *shortTarget = static_cast<int16_t *>(malloc(sizeof(short) * resLen));
        //将重采样后的float转成short
        convertFloatToPcm16(floatAllTarget + resFloatLength - resLen, shortTarget, resLen);
        //写入short数据
        fwrite(shortTarget, resLen * sizeof(int16_t), 1, laterShortFile);
        fflush(laterShortFile);

        LOGI("原始数据长度 beforeFileLength = %d  resLen = %d curSLen长度 = %d", beforeFileLength, resLen,
             curSLen);

        i += curSLen;
    }

    LOGI("全部写入完成,返回的float数据长度为：%d,原先float数据长度：%d", resFloatLength, oriFloatLength);
    */

    if (laterShortFile != NULL) {
        fclose(laterShortFile);
    }


    if (resampleObj != NULL) {
        resampleObj_free(resampleObj);
        resampleObj = NULL;
    }

    fclose(beforeFile);
    fclose(laterFile);

}



//原本重采样
extern "C"
JNIEXPORT void JNICALL
Java_com_example_oboedemo_MainActivity_startOldReSampling(JNIEnv *env, jobject thiz) {

    //重采样原始数据
    FILE *beforeFile = fopen("/sdcard/Android/data/com.example.oboedemo/files/convertBefore.pcm",
                             "rb+");
//重采样后的数据
    FILE *laterFile = fopen(
            "/sdcard/Android/data/com.example.oboedemo/files/convertLater.pcm", "wb+");

    //获取pcm数据长度
    int fd1 = fileno(beforeFile);
    //将读写位置移到文件尾为了获取文件大小
    int beforeFileLength = lseek(fd1, 0, SEEK_END);
    //再设置偏移到文件开头
    fseek(beforeFile, 0, SEEK_SET);

    //构建重采样对象
    AudioResampler ar = NULL;
    audioResampler_new(&ar, 44100, 32000, 4);

    //原始的数据长度,对应转换后的
    int sLen = beforeFileLength * sizeof(uint8_t) * 1.0f / sizeof(short);
    short leftPcmArr[sLen];
    //从文件中获取pcm原始数据
    fread(leftPcmArr, beforeFileLength * sizeof(uint8_t), 1, beforeFile);

    //单次写入
    startTime = datetime_getCurrentMillisecond();
    //重采样后的数据大小
    int resLen = sLen;
    short int pcm_buffer_resample[sLen];
    audioResampler_process(ar, leftPcmArr, &sLen, pcm_buffer_resample, &resLen);

    endTime = datetime_getCurrentMillisecond();
    LOGI("旧版重采样时间：%f", (endTime - startTime) / 1000.0);

    LOGI("原始数据长度 beforeFileLength = %d  resLen = %d ", beforeFileLength, resLen);

    LOGI("写入数据");
    fwrite(pcm_buffer_resample, resLen * sizeof(short), 1, laterFile);

    /* //多次写入
     int currentIndex = 0;
     int curSLen = 4096;
     short curPcmArr[4096];
     while (currentIndex < sLen) {
         int split = currentIndex + 4096;
         LOGI("当前indext:%d ,split:%d, 总数量： %d", currentIndex, split, sLen);
         for (; currentIndex < split; ++currentIndex) {
             if (currentIndex < sLen) {
                 curPcmArr[currentIndex % 4096] = leftPcmArr[currentIndex];
             } else {
                 curSLen = currentIndex % 4096;
                 LOGI("读取到最后 currentIndex:%d", currentIndex);
                 break;
             }
         }
         //重采样后的数据大小
         int resLen = curSLen;
         short int pcm_buffer_resample[curSLen];

         audioResampler_process(ar, curPcmArr, &curSLen, pcm_buffer_resample, &resLen);

         LOGI("原始数据长度 beforeFileLength = %d  resLen = %d curSLen长度 = %d", beforeFileLength, resLen,
              curSLen);

         LOGI("写入数据");
         fwrite(pcm_buffer_resample, resLen * sizeof(short), 1, laterFile);
     }*/

    if (ar != NULL) {
        audioResampler_free(ar);
        ar = NULL;
    }

    fclose(beforeFile);
    fclose(laterFile);

}