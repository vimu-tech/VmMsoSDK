#ifndef VMREAL_FFT_H
#define VMREAL_FFT_H

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

//---------------------------------实序列的FFT---------------------------------
//实序列的FFT
/*flag  0  DIT2_FFT
        1  DIF4_FFT
        2  Sr_FFT
		3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT */
SIGNAL_EXPORT void SIGNAL_DLL_CALL real_fft(double *x_Re,double *x_Im,uint32_t N,char flag);

/*flag  0  DIT2_FFT
        1  DIF4_FFT
        2  Sr_FFT
		3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT

		4  DIT2_FHT
		5  DIT4_FHT
		6  Sr_FHT
		7  如果是4的整数次幂，采用DIT4_FHT；否则采用Sr_FHT

		8  如果是4的整数次幂，采用DIT4_FHT；否则采用DIT4_FFT*/
SIGNAL_EXPORT void SIGNAL_DLL_CALL real_fft_fht(double *x_Re,double *x_Im,uint32_t N,char flag);

//实信号的fft
/*flag  0  DIT2_FFT
        1  DIF4_FFT
        2  Sr_FFT
		3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT

		4  DIT2_FHT
		5  DIT4_FHT
		6  Sr_FHT
		7  如果是4的整数次幂，采用DIT4_FHT；否则采用Sr_FHT
		
		8 如果是4的整数次幂，采用DIT4_FHT；否则采用DIT4_FFT*/
SIGNAL_EXPORT void SIGNAL_DLL_CALL real_fft_signal(double *x_Re, double *x_Im, uint32_t N, uint32_t Real_N, char flag);


#ifdef __cplusplus
}
#endif
#endif