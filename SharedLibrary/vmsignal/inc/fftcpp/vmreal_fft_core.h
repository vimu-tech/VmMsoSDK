#ifndef VMREAL_FFT_CORE_H
#define VMREAL_FFT_CORE_H

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

#include "vmfft_core.h"
#include "vmfht_core.h"

class SIGNAL_EXPORT real_fft_core
{
public:
	real_fft_core(void);
	~real_fft_core(void);

	//---------------------------------实序列的FFT---------------------------------
	//实序列的FFT
	/*flag  0  DIT2_FFT
			1  DIF4_FFT
			2  Sr_FFT
			3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT */
	void real_fft(double *x_Re,double *x_Im,uint32_t N,char flag);

	/*flag  0  DIT2_FFT
			1  DIF4_FFT
			2  Sr_FFT
			3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT

			4  DIT2_FHT
			5  DIT4_FHT
			6  Sr_FHT
			7  如果是4的整数次幂，采用DIT4_FHT；否则采用Sr_FHT

			8  如果是4的整数次幂，采用DIT4_FHT；否则采用DIT4_FFT*/
	void real_fft_fht(double *x_Re,double *x_Im,uint32_t N,char flag);

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
	void real_fft_signal(double *x_Re, double *x_Im, uint32_t N, uint32_t Real_N, char flag);

private:
	fft_core m_fft;
	fht_core m_fht;

	double *m_sins, *m_coss;
	uint32_t m_length;

	int32_t get_sin_cos_num(uint32_t N);
	void sin_cos(uint32_t N);
};

#endif