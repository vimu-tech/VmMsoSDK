#ifndef VMFHT_H
#define VMFHT_H

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

//----------------------基2fht------------------------
SIGNAL_EXPORT void SIGNAL_DLL_CALL dit2_fht(double *x, uint32_t n);
//----------------------基4fht-------------------------
SIGNAL_EXPORT void SIGNAL_DLL_CALL dit4_fht(double *x, uint32_t n);
//----------------------分裂基fht-----Split-Radix--------------------
SIGNAL_EXPORT void SIGNAL_DLL_CALL sr_fht(double *x, uint32_t n);


//------------------------fht--------------------------
/*   flag=0 基2fht
     flag=1 基4fht  
	 flag=2 基srfht   
	 flag=3 如果是4的整数次幂，采用DIT4_FHT；否则采用Sr_FHT

	 sign=1 fht
	 sign=-1 ifht
*/
SIGNAL_EXPORT void SIGNAL_DLL_CALL fht(double *x, uint32_t n, char flag, char sign);

//---------------------fht_dft-----------------------
/*   flag=0 基2fht
     flag=1 基4fht  
	 flag=2 基srfht   
	 flag=3 如果是4的整数次幂，采用DIT4_FHT；否则采用Sr_FHT

注意：值支持实数的dft
*/
SIGNAL_EXPORT void SIGNAL_DLL_CALL fht_dft(double *XRe, double *XIm, uint32_t N, char flag);

#ifdef __cplusplus
}
#endif
#endif