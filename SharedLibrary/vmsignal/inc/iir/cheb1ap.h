#ifndef CHEB1AP_H
#define CHEB1AP_H
#include "vmcplx_vector.h"
#include "vmsignal_global.h"

SIGNAL_EXPORT void SIGNAL_DLL_CALL cheb1ap(int32_t n, double rp, cplx_vector& p, double& k);

#endif