#ifndef VMFREQZ_H
#define VMFREQZ_H

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

SIGNAL_EXPORT std::vector<double> SIGNAL_DLL_CALL freqz(const std::vector<double> b,  const std::vector<double> a, int32_t len, int32_t sign);

SIGNAL_EXPORT std::vector<double> SIGNAL_DLL_CALL freqz(const cplx_vector b,  const cplx_vector a, int32_t len, int32_t sign);

SIGNAL_EXPORT std::vector<double> SIGNAL_DLL_CALL freqz(const cplx_vector z,  const cplx_vector p, double g, int32_t len, int32_t sign);
#endif