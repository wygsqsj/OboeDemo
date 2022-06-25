
#ifndef AUDIO_RECOGNIZE_H
#define AUDIO_RECOGNIZE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#import "audio_analyze.h"


typedef struct {
	int *soundArr; // 调音下模式不存在
	int soundLength;

	float *data;
	int frameLength;
	float sampleRate;
    
    char *_tuneArr;
    char _stringNum;
    int _adKey;
    
} RecognizeData;

typedef struct {
	float fre; // 真实频率
    float fre1; // 映射频率
	float freAmp;

	float correctFre; // 针对fre1
	float power; // 针对freAmp
    
    // 针对chord识别数据
    int *chord_statusArr;
    float *chord_freArr;
    int chord_status;
    int chord_length;
    // 针对chord验证数据
    int chord_validStatus;
    
} ToneData;

// 消除同八度音
typedef struct{
	float fre;
	int k;
} FreValid;

#define AUDIO_DEBUG_COPY(ds,ss,length,type){	\
			type *ds1;	\
			type *ss1;	\
			ds1=(type *)ds;	\
			ss1=(type *)ss;	\
			for(int i=0;i<length;i++){	\
				ds1[i]=ss1[i];	\
			}	\
		}	\

// flow 流动模式 声音识别
int audio_recognize(RecognizeData *recData,EnergyData *outData);
// tune 调音模式 乐音识别
ToneData *audio_tune(RecognizeData *recData,float *freArr,int length);
ToneData *audio_tuneWeak(RecognizeData *recData,float *freArr,int length);
ToneData *audio_chordTune(RecognizeData *recData,float *freArr,int length,float *validFreArr,int validLength,int *isValidExtend);
// control 控制模式 强乐音识别
int audio_control1(RecognizeData *recData,EnergyData *outData);

// 注:老版本 可能要废弃
ToneData *audio_tune1(RecognizeData *recData,EnergyData *outData);
ToneData *audio_control(RecognizeData *recData);


void __debugDebugEnergyData(EnergyData *data,int index,int index1);
void __copyDebugEnergyData(EnergyData *inData,EnergyData *outData);
void __freeDebugEnergyData(EnergyData *energyData);

    
#ifdef __cplusplus
}
#endif

#endif
