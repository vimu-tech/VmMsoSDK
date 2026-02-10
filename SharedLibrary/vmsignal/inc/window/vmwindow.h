#ifndef VMWINDOW_H
#define VMWINDOW_H

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
#include <math.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum {
				RECTANGLE=0, 
				BARTLETT=1, 
				TRIANGULAR=2,
				COSINE=3, 
				HANNING=4, 
				BARTLETT_HANNING=5,
                HAMMING=6, 
				BLACKMAN=7, 
				BLACKMAN_HARRIS=8, 
				TUKEY=9, 
				NUTTALL=10, 
				FLATTOP=11, 
				BOHMAN=12, 
				PARZEN=13, 
				LANCZOS=14, 
				KAISER=15, 
				GAUSS=16, 
				DOLPH_CHEBYSHEV=17
} WINDOW_STYLE;

static const char* Window_Name[]={
	"rectangle",
	"bartlett",
	"triangular",
	"cosine",
	"hanning",
	"bartlett_hanning",  //5
	"hamming",
	"blackman",
	"blackman_Harris",
	"tukey",
	"Nuttall",
	"FlatTop",
	"Bohman",
	"Parzen",
	"Lanczos",
	"kaiser",
	"gauss",
	"dolph_chebyshev"
};

#define WINDOW_IS_NEED_PARA(window) ((window==TUKEY)||(window==KAISER)||(window==GAUSS)||(window==DOLPH_CHEBYSHEV))
SIGNAL_EXPORT double SIGNAL_DLL_CALL getdefaultpara(WINDOW_STYLE window);
SIGNAL_EXPORT void SIGNAL_DLL_CALL getpararange(WINDOW_STYLE window, double *min, double *max);
//-------------------------------------------------------window---------------------------------------------------------
SIGNAL_EXPORT char SIGNAL_DLL_CALL window(WINDOW_STYLE type, int32_t N, double *hn, double alpha_beta, char normalize);

//矩形窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL rectangle(int32_t N, double *hn);
//三角窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL bartlett(int32_t N, double *hn);
//三角窗  起点和终点不是0
SIGNAL_EXPORT void SIGNAL_DLL_CALL triangular(int32_t N, double *hn);
//余弦窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL cosine(int32_t N, double *hn);
//汉宁窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL hanning(int32_t N, double *hn);
//三角汉宁窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL bartlett_hanning(int32_t N, double *hn);
//海明窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL hamming(int32_t N, double *hn);
//布莱克曼窗 
SIGNAL_EXPORT void SIGNAL_DLL_CALL blackman(int32_t N, double *hn);
//Blackman–Harris
SIGNAL_EXPORT void SIGNAL_DLL_CALL blackman_Harris(int32_t N, double *hn);
//图基窗
SIGNAL_EXPORT char SIGNAL_DLL_CALL tukey(int32_t N, double *hn, double alpha);
//Nuttall窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL Nuttall(int32_t N, double *hn);
//平顶窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL FlatTop(int32_t N, double *hn);
//Bohman 窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL Bohman(int32_t N, double *hn);
//Parzen window  窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL Parzen(int32_t N, double *hn);
//兰乔斯法窗
SIGNAL_EXPORT void SIGNAL_DLL_CALL Lanczos(int32_t N, double *hn);
//凯塞——贝塞尔窗 
SIGNAL_EXPORT void SIGNAL_DLL_CALL kaiser(int32_t N, double *hn, double beta);
//高斯窗
SIGNAL_EXPORT char SIGNAL_DLL_CALL gauss(int32_t N, double *hn, double alpha);
//N必须为偶数
SIGNAL_EXPORT char SIGNAL_DLL_CALL dolph_chebyshev_fft(int32_t N, double *hn, double at);
//道尔夫-切比雪夫窗  当盘瓣的最大值固定时，道尔夫-切比雪夫窗可使主瓣幅度为最小
//另外，该窗函数的特点是所有的盘瓣振幅都相等
//注意：N必须为奇数。
SIGNAL_EXPORT void SIGNAL_DLL_CALL dolph_chebyshev(int32_t N, double *hn, double at);
                                         

//--------------------------------------------------------------加窗-------------------------------------------------------------
//return the coherent gain of the window function
SIGNAL_EXPORT double SIGNAL_DLL_CALL window_i(WINDOW_STYLE type, double *x, int32_t N, double alpha_beta, char normalize);
//-----------------------------------------------------------窗的平方和--------------------------------------------------------------
SIGNAL_EXPORT double SIGNAL_DLL_CALL window_i2sum(WINDOW_STYLE type, int32_t N, double alpha_beta, char normalize);
//-------------------------------------------------------------------------------------------------------------------------------
SIGNAL_EXPORT void SIGNAL_DLL_CALL window_HW(double *h, int32_t N, double *HRm, double *HIm, int32_t HN);
SIGNAL_EXPORT void SIGNAL_DLL_CALL window_Amplitude(double *x_Re,double *x_Im,double *af,uint32_t N, char uselog10, char normalize);
SIGNAL_EXPORT void SIGNAL_DLL_CALL window_Phase(double *x_Re,double *x_Im,double *pf,uint32_t N);

#ifdef __cplusplus
}
#endif

#endif