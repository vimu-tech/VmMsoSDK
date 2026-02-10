#ifndef VMFILTER_DEF_H
#define VMFILTER_DEF_H

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

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum {NOSCALE=0, SCALE=1} SCALETYPE;
typedef enum {LOW=0, HIGH=1, PASS=2, STOP=3, DC_0=4, DC_1=5} BANDTYPE;

//typedef enum filter_trye{low, high, pass, stop}filter_trye;

#ifdef __cplusplus
}
#endif
#endif