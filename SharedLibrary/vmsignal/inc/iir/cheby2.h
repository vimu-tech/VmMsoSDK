#ifndef CHEBY2_H
#define CHEBY2_H
#include "vmcplx_vector.h"
#include "vmfilter_def.h"
#include "vmsignal_global.h"

SIGNAL_EXPORT void SIGNAL_DLL_CALL cheby2(int32_t n, double wn1, double wn2, double rs, BANDTYPE type, char opt, cplx_vector& zz, cplx_vector& zp, double& zg);

#endif