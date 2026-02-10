#ifndef _SPLINE3INTERP
#define _SPLINE3INTERP
#include "vmsignal_global.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" 
{
#endif

SIGNAL_EXPORT void SIGNAL_DLL_CALL calcCoefs(double *xi, double *yi, int32_t N, double M0, double Mn, double *coefs);

SIGNAL_EXPORT double SIGNAL_DLL_CALL evaluate(double *xi, int32_t N, double **coefs, double x );

SIGNAL_EXPORT void SIGNAL_DLL_CALL evaluates(double *xi, double *yi, int32_t N, double M0, double Mn, int32_t insert_n, double *insert_y);

#ifdef __cplusplus
}
#endif

#endif