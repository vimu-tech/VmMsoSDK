#ifndef FIR1_H
#define FIR1_H
#include "vmwindow.h"
#include "vmfilter_def.h"
#include "vmsignal_global.h"

#ifdef __cplusplus
extern "C" 
{
#endif


SIGNAL_EXPORT char SIGNAL_DLL_CALL fir1(int32_t N, double *w, int32_t w_h, BANDTYPE type, WINDOW_STYLE window, double alpha_beta, SCALETYPE noscale, double *h);

SIGNAL_EXPORT char SIGNAL_DLL_CALL fir(int32_t N, BANDTYPE band, double fln, double fhn, WINDOW_STYLE window, double alpha_beta, double *h);

SIGNAL_EXPORT void SIGNAL_DLL_CALL fir_resp(int32_t N, double *hn, double *jy);

//-----------------------------------------幅频相频--------------------------------------------------------------
SIGNAL_EXPORT void SIGNAL_DLL_CALL fir_Hw(double *h, int32_t N, double *HRm, double *HIm, int32_t HN);
SIGNAL_EXPORT void SIGNAL_DLL_CALL fir_Amplitude(double *x_Re,double *x_Im,double *af,uint32_t N, char uselog10);
SIGNAL_EXPORT void SIGNAL_DLL_CALL fir_Phase(double *x_Re,double *x_Im,double *pf,int32_t N, int32_t left_N, int32_t right_N);
SIGNAL_EXPORT void SIGNAL_DLL_CALL fir_Phase_Daizu(double *x_Re,double *x_Im,double *pf,int32_t N, int32_t left_N, int32_t right_N);

#ifdef __cplusplus
}
#endif
#endif
