#ifndef VMCPLX_FUN_H
#define VMCPLX_FUN_H

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



#include <complex>
#include "vmcplx_vector.h"

//void cplx_zeros(std::complex<double> c[], int32_t num);
//void cplx_zeros(cplx_vector c, uint32_t num);

//std::complex<double> cplx_prod(std::complex<double> c[], int32_t num);
std::complex<double> cplx_prod(cplx_vector c);


#endif