#ifndef VMDHT_H
#define VMDHT_H

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

#include <stdint.h>

#ifdef __cplusplus
extern "C" 
{
#endif

//sign=1 dht 
//sign=-1 idht
void dht(double *x, uint32_t n, double *X, uint32_t N, char sign);

//dht-->dft
void dht_dft(double *x, uint32_t n, double *XRe, double *XIm, uint32_t N);


#ifdef __cplusplus
}
#endif
#endif