

#ifndef FLUX_BASE_H
#define FLUX_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

// window相关
typedef enum{
	Window_Rect=0, 
	Window_Hann,
	Window_Hamm,

	Window_Blackman,
	Window_Kaiser,

	Window_Bartlett,
	Window_Triang,

	Window_Flattop,
	Window_Gauss,

	Window_Blackman_Harris,
	Window_Blackman_Nuttall,
	Window_Bartlett_Hann,

	Window_Bohman,

	Window_Tukey, // tapered cosine

} WindowType;

typedef enum{
	FilterBand_LowPass=0, 
	FilterBand_HighPass,

	FilterBand_BandPass,
	FilterBand_BandStop, // Rejection

	// FilterBand_AllPass,

} FilterBandType;

// spectrum&&spectrogram 相关
typedef enum{
	SpectralData_Power=0,
	SpectralData_Mag,

} SpectralDataType;

typedef enum{
	SpectralFilterBank_Linear=0,

	SpectralFilterBank_Mel, 
	SpectralFilterBank_Bark,
	SpectralFilterBank_Erb, 

	SpectralFilterBank_Log, // similar Constant-Q
	SpectralFilterBank_Deep, // similar Constant-Q 

	SpectralFilterBank_Chroma, // stft-chroma

	SpectralFilterBank_LogChroma, // similar cqt-chroma
	SpectralFilterBank_DeepChroma, // similar cqt-chroma

} SpectralFilterBankType;

typedef enum{
	SpectralFilterBankStyle_Slaney=0, // Triang
	SpectralFilterBankStyle_ESTI, // Bartlett
	SpectralFilterBankStyle_Gammatone, // gammatone

	SpectralFilterBankStyle_Rect,
	SpectralFilterBankStyle_Hann, 
	SpectralFilterBankStyle_Hamm,

	SpectralFilterBankStyle_Blackman, 
	SpectralFilterBankStyle_Bohman,

	SpectralFilterBankStyle_Kaiser,
	SpectralFilterBankStyle_Gauss,

} SpectralFilterBankStyleType;

typedef enum{
	SpectralFilterBankNormal_None=0, // same hight

	SpectralFilterBankNormal_Area, // normal(same hight)/same area
	SpectralFilterBankNormal_BandWidth, 
	
} SpectralFilterBankNormalType;

typedef enum{
	ChromaDataNormal_None=0,

	ChromaDataNormal_Max, 
	ChromaDataNormal_Min, 

	ChromaDataNormal_P2, 
	ChromaDataNormal_P1, 
	
} ChromaDataNormalType;

typedef enum{
	CepstralRectify_Log=0,
	CepstralRectify_CubicRoot,

} CepstralRectifyType;

typedef enum{
	PaddingPosition_Center=0,
	PaddingPosition_Right,
	PaddingPosition_Left,

} PaddingPositionType;

typedef enum{
	PaddingMode_Constant=0,
	PaddingMode_Reflect,
	PaddingMode_Wrap, // repeat

} PaddingModeType;


#ifdef __cplusplus
}
#endif

#endif