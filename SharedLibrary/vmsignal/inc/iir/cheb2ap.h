#ifndef CHEB2AP_H
#define CHEB2AP_H
#include "vmcplx_vector.h"
#include "vmsignal_global.h"

SIGNAL_EXPORT void SIGNAL_DLL_CALL cheb2ap(int32_t n, double rs, cplx_vector& z, cplx_vector& p, double& k);

#endif