#ifndef FIR2_H
#define FIR2_H

#include "vmwindow.h"
#include "vmsignal_global.h"

#ifdef __cplusplus
extern "C" 
{
#endif



//----------------频率采样法设计任意形状的fir滤波器--------------------
SIGNAL_EXPORT char SIGNAL_DLL_CALL fir2(double* h, int32_t n, double* f, double* m, int32_t fm_n, int32_t grid_n, double ramp_n, WINDOW_STYLE window, double alpha_beta);

#ifdef __cplusplus
}
#endif
#endif
