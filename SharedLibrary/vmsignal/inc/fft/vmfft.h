#ifndef VMFFT_H
#define VMFFT_H

/********************************************************************************************************************
说明：
1,该数字信号处理库，使用标准的C语言编写的，没有针对任何的平台进行更好的优化。
2,在使用中发现任何的bug或者建议，可以联系我们。

Note:
1,This digital signal processing library, written in the standard C language, is not better optimized for any platform.
2,If you find any bugs or suggestions in use, you can contact us.

	www.vimu.top
	www.instrustar.com
*********************************************************************************************************************/

#include "vmsignal_global.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" 
{
#endif

//------------------------------------基2DIT---------------------------------
//基2DIT-FFT 2DIT-IFFT
SIGNAL_EXPORT void SIGNAL_DLL_CALL dit2_fft(double *x_Re,double *x_Im, uint32_t N, char sign);
//2DIT-IFFT  该算法加入了 溢出处理
SIGNAL_EXPORT void SIGNAL_DLL_CALL dit2_ifft(double *x_Re,double *x_Im, uint32_t N);
//------------------------------------基4DIF---------------------------------
SIGNAL_EXPORT void SIGNAL_DLL_CALL dit4_fft(double *x_Re,double *x_Im, uint32_t N);
//----------------------------------分裂基FFT-------Split-Radix---------------------------
SIGNAL_EXPORT void SIGNAL_DLL_CALL sr_fft(double *x_Re,double *x_Im, uint32_t N);

//----------------------------fft/ifft---将上面各种fft封装-----------------------
/*flag  0  DIT2_FFT
        1  DIF4_FFT
        2  Sr_FFT
		3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT
*/
SIGNAL_EXPORT void SIGNAL_DLL_CALL fft(double *x_Re,double *x_Im, uint32_t N, char flag);

//改算法与DIT2_IFFT不同，采用了共轭求取的方法
/*flag  0  DIT2_FFT
        1  DIF4_FFT
        2  Sr_FFT
		3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT
*/
SIGNAL_EXPORT void SIGNAL_DLL_CALL ifft(double *x_Re,double *x_Im, uint32_t N, char flag);

//-------------------------------------------信号的fft--------------------------------------
/*flag  0  DIT2_FFT
        1  DIF4_FFT
        2  Sr_FFT
		3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT
*/
SIGNAL_EXPORT void SIGNAL_DLL_CALL fft_signal(double *x_Re,double *x_Im,uint32_t N, uint32_t Real_N, char flag);

//-------------------------------------------幅频、相频计算--------------------------------------
typedef enum {
	FFT_Amplitude_Ref_V = 0x00000001,
	FFT_Amplitude_Ref_dBV = 0x00000002,
	FFT_Amplitude_Ref_dBmV = 0x00000004,
	FFT_Amplitude_Ref_dBm = 0x00000008,
	FFT_Amplitude_Ref_dBu = 0x00000010
} FFT_Amplitude_Ref_Type;

typedef enum {
	FFT_Phase_Ref_Deg= 0x10000001,
	FFT_Phase_Ref_Rad = 0x10000002,
} FFT_Phase_Ref_Type;

typedef enum {
	FFT_Display_Style_Magnitude = 0x00000001,
	FFT_Display_Style_Average = 0x00000002,
	FFT_Display_Style_PeakHold = 0x00000004
} FFT_Display_Style;

SIGNAL_EXPORT void SIGNAL_DLL_CALL Fft_Amplitude (double *x_Re,double *x_Im,double *af, uint32_t N, unsigned char rms);
SIGNAL_EXPORT double SIGNAL_DLL_CALL V_Conver_dBs(double v, double R, FFT_Amplitude_Ref_Type type);
SIGNAL_EXPORT void SIGNAL_DLL_CALL Fft_Amplitude_Conver_Type(double* af, uint32_t len, double R, FFT_Amplitude_Ref_Type type);

SIGNAL_EXPORT void SIGNAL_DLL_CALL Fft_Phase (double *x_Re, double *x_Im, double* af, double *pf, uint32_t N, FFT_Phase_Ref_Type ref_type);

#ifdef __cplusplus
}
#endif
#endif


