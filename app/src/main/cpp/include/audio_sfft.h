/***
	audio_sFFT 2017/9/25 23:40
	copyright: AIMusic
 ***/

#ifndef AUDIO_SFFT_H
#define AUDIO_SFFT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct {
	float duration;
	float *referFreArr;
	int referFreLength;

	int isBeforeSame; // 是否和前音相同
	float beforeDuration; // 之前时长,一短一长
	int enableDetectEndPoint; // 是否侦测端点 针对相同音

	float *tieFreArr; // isBeforeSame==1情况下 不包含前音
	int tieFreLength;

	float *validFreArr;
	int validFreLength;

	int isFirst; // 是否第一个识别

} SFFT_ReferData;

typedef struct {
	float *dataArr;
	int dataLength;

	int frameLength;
	int samplate;
	
	int curPackageIndex;

} SFFT_PackageData;

typedef struct {
	int status;
	float *correctFreArr;

	int isExistEndPoint; // 是否存在端点
	int offsetTime; // 偏移时间

	float domEnergy;
	float freEnergy;

	float *lessArr;
	int lessLength;
	float *overArr;
	int overLength;

} SFFT_Result;

typedef struct OpaqueAudioSFFT *AudioSFFT;

int audioSFFT_new(AudioSFFT *audioSFFT);
void audioSFFT_setRecognizeType(AudioSFFT audioSFFT,int *type);

void audioSFFT_sfft(AudioSFFT audioSFFT,SFFT_ReferData *referData,SFFT_PackageData *packageData,SFFT_Result *result);

void audioSFFT_free(AudioSFFT audioSFFT);

void audioSFFT_setDebugFile(AudioSFFT audioSFFT,FILE *file);
void audioSFFT_enableMatchPathDebug(AudioSFFT audioSFFT,int flag);
void audioSFFT_enablePackageFDHDebug(AudioSFFT audioSFFT,int flag);
void audioSFFT_enableSlideProveDebug(AudioSFFT audioSFFT,int flag);
void audioSFFT_enableEndPointDebug(AudioSFFT audioSFFT,int flag);
void audioSFFT_enableSlideEnergyDebug(AudioSFFT audioSFFT,int flag);

#ifdef __cplusplus
}
#endif

#endif





