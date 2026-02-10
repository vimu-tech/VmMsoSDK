#ifndef VMFFT_CORE_H
#define VMFFT_CORE_H

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

#include "vmdit2_fft_core.h"
#include "vmdit4_fft_core.h"

class SIGNAL_EXPORT fft_core
{
public:
	fft_core(void);
	~fft_core(void);

	//----------------------------fft/ifft---将上面各种fft封装-----------------------
	/*flag  0  DIT2_FFT
			1  DIF4_FFT
			2  Sr_FFT
			3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT
	*/
	void fft(double *x_Re,double *x_Im, uint32_t N, char flag);

	//改算法与DIT2_IFFT不同，采用了共轭求取的方法
	/*flag  0  DIT2_FFT
			1  DIF4_FFT
			2  Sr_FFT
			3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT
	*/
	void ifft(double *x_Re,double *x_Im, uint32_t N, char flag);
	//-------------------------------------------信号的fft--------------------------------------
	/*flag  0  DIT2_FFT
			1  DIF4_FFT
			2  Sr_FFT
			3  如果是4的整数次幂，采用DIF4_FFT；否则采用Sr_FFT
	*/
	void fft_signal(double *x_Re,double *x_Im,uint32_t N, char flag); 
private:

	dit2_fft_core m_dit2;
	dit4_fft_core m_dit4;
};

#endif