#ifndef BILINEAR_H
#define BILINEAR_H
#include "vmcplx_vector.h"
#include "vmsignal_global.h"

SIGNAL_EXPORT bool SIGNAL_DLL_CALL bilinear(cplx_vector z, cplx_vector p, double g, double T, cplx_vector& Zz, cplx_vector& Zp, double& Zg);

#endif