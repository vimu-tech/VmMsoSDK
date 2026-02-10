#ifndef VMCONVOL_H
#define VMCONVOL_H

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

/*-------------------------------------根据卷积公式计算-------------------------------------*/
SIGNAL_EXPORT void SIGNAL_DLL_CALL convol(double *x, int32_t m, double *y, int32_t n, double *z, int32_t fht_n);




/*------------------------------利用fht计算两个序列的卷积-------------------------------
     flag=0 基2fht
     flag=1 基4fht  
	 flag=2 基srfht   
	 flag=3 如果是4的整数次幂，采用DIT4_FHT；否则采用Sr_FHT   */
SIGNAL_EXPORT void SIGNAL_DLL_CALL convol_fht_basic(double *x, double *y, int32_t len, char flag);
/* 采用flag=3 如果是4的整数次幂，采用DIT4_FHT；否则采用Sr_FHT的方式计算 */
SIGNAL_EXPORT void SIGNAL_DLL_CALL convol_fht(double *x, int32_t x_N, double *h, int32_t h_N, int32_t fht_n, int32_t begin, int32_t len);
//-----------------------------重叠相加法---fht-------------------------------//
SIGNAL_EXPORT void SIGNAL_DLL_CALL convol_overlap_add_fht (double *x, int32_t x_N, double *h, int32_t h_N, int32_t fht_N);




/*---------------------------------用快速卷积计算两个序列卷积------------------------------------------------*/
SIGNAL_EXPORT void SIGNAL_DLL_CALL convol_fft(double *x, int32_t x_N, double *h, int32_t h_N, int32_t fft_N, int32_t begin, int32_t len);
//-----------------------------重叠相加法---快速卷积-------------------------------//
SIGNAL_EXPORT void SIGNAL_DLL_CALL convol_overlap_add_fft (double *x, int32_t x_N, double *h, int32_t h_N, int32_t fft_N);

#ifdef __cplusplus
}
#endif
#endif