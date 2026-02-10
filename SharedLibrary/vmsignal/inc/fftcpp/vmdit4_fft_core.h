#ifndef VMDIT4_FFT_CORE_H
#define VMDIT4_FFT_CORE_H

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

class SIGNAL_EXPORT dit4_fft_core
{
public:
	dit4_fft_core(void);
	~dit4_fft_core(void);

	//------------------------------------基4DIF---------------------------------
	void dit4_fft(double *x_Re,double *x_Im,uint32_t N); 
	//----------------------------------分裂基FFT-------Split-Radix---------------------------
	void sr_fft(double *x_Re,double *x_Im, uint32_t N);

private:
	double *m_sins, *m_coss;
	uint32_t m_length;

	int32_t get_sin_cos_num(uint32_t N);
	void sin_cos(uint32_t N);
};

#endif