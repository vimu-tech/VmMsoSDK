#ifndef VMFILTERS_H
#define VMFILTERS_H

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


#include "vmcplx_vector.h"
#include "vmsignal_global.h"

//------------------------------------------fir------------------------------------------
SIGNAL_EXPORT void SIGNAL_DLL_CALL filters(double *hn, int32_t N, int32_t *date, int32_t num);
SIGNAL_EXPORT void SIGNAL_DLL_CALL filters(double *hn, int32_t N, double *date, int32_t num);

SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_fht(double *hn, int32_t N, int32_t *date, int32_t fht_n);
SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_fht(double *hn, int32_t N, double *date, int32_t fht_n);

SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_fft(double *hn, int32_t N, int32_t *date, int32_t fft_n);
SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_fft(double *hn, int32_t N, double *date, int32_t fft_n);

SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_overlap_add_fht(double *hn, int32_t N, int32_t *date, int32_t num, int32_t fht_n);
SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_overlap_add_fht(double *hn, int32_t N, double *date, int32_t num, int32_t fht_n);

SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_overlap_add_fft(double *hn, int32_t N, int32_t *date, int32_t num, int32_t fft_n);
SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_overlap_add_fft(double *hn, int32_t N, double *date, int32_t num, int32_t fft_n);

//-----------------------------------------iir-------------------------------------------------
SIGNAL_EXPORT void SIGNAL_DLL_CALL filters(const std::vector<double> b, const std::vector<double> a, std::vector<double>& x, int32_t len);

SIGNAL_EXPORT void SIGNAL_DLL_CALL filters(double* b, int32_t bn, double* a, int32_t an, int32_t* x, int32_t len);
SIGNAL_EXPORT void SIGNAL_DLL_CALL filters(double* b, int32_t bn, double* a, int32_t an, double* x, int32_t len);

SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_overlap_add_fht(double* b, int32_t bn, double* a, int32_t an, int32_t* x, int32_t len, int32_t fft_n);
SIGNAL_EXPORT void SIGNAL_DLL_CALL filters_overlap_add_fht(double* b, int32_t bn, double* a, int32_t an, double* x, int32_t len, int32_t fft_n);

#endif