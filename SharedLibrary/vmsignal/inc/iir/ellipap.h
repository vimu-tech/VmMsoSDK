#ifndef ELLIPAP_H
#define ELLIPAP_H
#include "vmcplx_vector.h"
#include "vmsignal_global.h"

SIGNAL_EXPORT void SIGNAL_DLL_CALL ellipap(int32_t n, double rp, double rs, cplx_vector& z, cplx_vector& p, double& k);

#endif