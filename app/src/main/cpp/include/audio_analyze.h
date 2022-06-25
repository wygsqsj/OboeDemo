
#ifndef AUDIO_ANALYZE_H
#define AUDIO_ANALYZE_H

#ifdef __cplusplus
extern "C" {
#endif

// 注:ios标准形式
//#if defined(__GNUC__) && ( defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__MACOS_CLASSIC__) )
//    #define IPHONE_FLAG 1
//#else
//    #define IPHONE_FLAG 0
//#endif

// 所有平台一致性
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
	#define IPHONE_FLAG 1
#else
	#define IPHONE_FLAG 0
#endif

// 最大乐音数目(玄的个数) 重点分析倍数 泛音求证个数 品最大数目
// 归运行环境 ???
#define DARKNUM 6
#define DARKTIMES 2
#define OVERTONE 2
#define GRADENUM 20
// 频率基准值 最低值倍数
#define BASEVALUE 55.0
#define BASETIME 7

// 分析配置 全局配置
typedef struct {
	int darkNum; // 最大玄数目
	int darkTimes; // 分析倍数

	int gradeNum; // 品数目
	float baseValue; // 频率基准值
	int baseTime; // 八度个数
	float filterDB; // 过滤分贝
	// 调试相关
	int energyDebugFlag; // 是否能级调试标志
	int metaDataDebugFlag;
	int fftDebugFlag;

} AnalyRunTime;

typedef struct {
	float *data;
	int numFrames; // 帧数或采样长度
	float sampleRate; // 采样率

} AnalyData;

typedef struct {
	int *indexArr;
	float *ripeAmpArr; // 针对DB计算 易于比较和过滤
	float *rawAmpArr; // 原始 fft计算振幅
	float *ripeFreArr; // 精确频率
	float *rawFreArr; // 未修正
	float *correctFreArr; // 修正
	float *leftRawAmpArr;
	float *rightRawAmpArr;
	int count;
	// 追加 进化
	float *fftArr;	// fft 变化后一般取值
	float sampleRate; // 采样率
	int frameNums;	// 帧的个数

} MetaData;

// 能量热力图 Energy HeatMap 可能会发生 跃迁现象 jump
typedef struct {
    float **freArr;
    int *lengthArr;
    float **powerArr; // 能量 以振幅表示
    float **leftPowerArr;
    float **rightPowerArr;
    float **correctFreArr;
    int **indexArr;
    int *jumpFlags; // 是否跃迁
    int count;
    int frameNums;	// 帧的个数

    MetaData *metaData; // 追加原始数据

} EnergyData;

typedef struct {
    EnergyData *energyData;

} AnalyResult;


// 频率表 1. 算精确的频率值 powf(2,1.0/12) 440 调制频率值
// 2. 吉他上对应关系 (玄,品)+音符 ABCDEFG
typedef struct {
	float value; // 精确频率值
	int note; // 3*100+11
} FreValue;

// 算法之 推测 Guess
// 声音状态的转化  推测阶段----假定尽可能多的乐音 存在不确定 乐音不确定/确定
typedef struct {
	float *voiceArr; // 声音未知 需要去伪和求证
	int voiceCount;
	float *toneUnsureArr; // 乐音不确定 需要求证
	int toneUnsureCount;
	float *toneSureArr; // 乐音确定 需要再次求证
	int toneSureCount;

} GuessResult;

typedef struct {
	int noFront; // 不回溯
	int jump; // 跃迁
	int flag; // 测试
} GuessFlag;


// 算法之 证明 Prove
typedef struct {

} ProveData;

// 算法之 综合 complex 智能化建议结果
typedef struct {

} ComplexData;

// 针对 玄和品
typedef struct {
	int x;	// 1~6
	int y;	// 0~20
} Coord;

// 比较倍数结构
typedef struct {
	int type; // 0 不成倍数 1 正倍数 2 倒倍数
	int k; // 倍数
} TimesValue;

// 2014/1/5 17:23 最新状况
// 注意区分noise 噪音 和 blur 毛边现象
// 加窗可以消除泄露的问题 但减弱了振幅能量


/**
	metaData:源数据
	dstData：目的结果数据
	size:分析长度
	lower:过滤下限
	upper:过滤上限
**/
#define FILTERMetaData(metaData,dstData,size,lower,upper) {	\
			int count=metaData->count;	\
			int *indexArr=metaData->indexArr;	\
			float *valueArr=metaData->valueArr;	\
			int *indexArr1=dstData->indexArr;	\
			float *valueArr1=dstData->valueArr;	\
			indexArr1=calloc(count,sizeof(int));	\
			valueArr1==calloc(count,sizeof(float));	\
			int c=0;	\
			float _d1=0;	\
			if(upper!=0){ \
				for(int i=0;i<count;i++){	\
					_d1=20*log10f(valueArr[i]/(2*size));	\
					if(_d1<upper){	\
						if(_d1>=lower){	\
							c++;	\
							valueArr1[c-1]=valueArr[i];	\
							indexArr1[c-1]=indexArr[i];	\
						}	\
						else {	\
							dstData->count=c;	\
							break;	\
						}	\
					}	\
				}	\
			}	\
			else if(upper==0){	\
				for(int i=0;i<count;i++){	\
					_d1=20*log10f(valueArr[i]/(2*size));	\
					if(_d1>=lower){	\
						if(_d1>=lower){	\
							c++;	\
							valueArr1[c-1]=valueArr[i];	\
							indexArr1[c-1]=indexArr[i];	\
						}	\
						else {	\
							dstData->count=c;	\
							break;	\
						}	\
					}	\
				}	\
			}	\
		}

AnalyResult *analyzeData(AnalyData *inData);
void calData(float *arr,int size,float sample,Coord *list,int count);
void calData1(float *arr,int size,float sample,Coord *coord);
MetaData *calMetaData(float *arr,int size,float sample);

// 测试调用 工具方法
void _dealFFT(float *input,int size, float **arr);
EnergyData *_dealData(float *arr,int size,float sample);
FreValue *getFre(int x,int y);
FreValue *getFreWithTuneArrAndAdKey(int x,int y,char *tuneArr,char stringNum,int adKey);
FreValue *getFre1(float value);
FreValue *getFre2(Coord *coord);
int getIndexFromFre(float value,float sample,int frameNums);
int _getNote(FreValue *fre);
int getNoteValue(int x,int y);
int getNoteValue1(float value);
int getNoteValue2(Coord *coord);
void getCoord(float value,Coord *coords,int *length);
int calNote(float value,float step);
int compareValue(float value1,float value2);
int hasIn(int *arr,int length,int value);
int getIndex(float *arr,int length,float value);
TimesValue *getTimes(float cur,float nex,int flag);
void frontTimes(float *arr,int length,float value,int *indexs,int *len,int flag);
void deleteFromIndex(float *arr,int length,int *indexArr,int indexLength);
void deleteFromIndex1(int *arr,int length,int *indexArr,int indexLength);
void sortMetaData(MetaData *metaData,int flag);
void filterMetaData(MetaData *metaData,MetaData *dstData,int size,float lower,float upper);

void setFilterDB(float db);
void setEnergyDebug(int flag);
void setFftDebug(int flag);
void setMetaDataDebug(int flag);
// int16--->float 转换
void convertInt16ToFloat(short *iArr,int size,float *fArr);
// 算法之推测
GuessResult *guess(float *arr,int length,int recall,int flag);
EnergyData *_guess(MetaData *metaData,int length,GuessFlag *guessFlag);
void analyzeGuess(MetaData *metaData,int count);
// 算法之论证
void prove();
void analyzeProve();
// 算法之综合
void complex();
void analyzeComplex();

// Ext 乐音和人声的区别
int  __augmentSingleToneRecognize(float fre,EnergyData *energyData);

// 内存相关
void freeEnergyData(EnergyData *data);
void freeMetaData(MetaData *metaData);
void freeAnalyResult(AnalyResult *analyResult);
void freeGuessResult(GuessResult *guessData);

// 算法之校正
float rectScaleCorrect(float cur,float left,float right,int index,float sample,int size);
float hanScaleCorrect(float cur,float left,float right,int index,float sample,int size);



// 调试打印工具方法
void debugFFT(float *arr,int size);
void debugMetaData(MetaData *metaData);
void debugFre(float *arr,int size);
void debugIndex(int *arr,int size);
void debugGuessResult(GuessResult *result);
void debugEnergyData(EnergyData *data,int flag1,int flag2);
void debugEnergyData1(EnergyData *data);

//#if IPHONE_FLAG
//void debugComplex(DSPSplitComplex *complex,int length);
//#endif


#ifdef __cplusplus
}
#endif

#endif

