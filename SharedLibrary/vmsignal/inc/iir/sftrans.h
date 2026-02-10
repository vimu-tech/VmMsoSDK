#ifndef SFTRANS_H
#define SFTRANS_H

#include <complex>
#include "vmcplx_vector.h"
#include "vmfilter_def.h"
#include "vmsignal_global.h"

SIGNAL_EXPORT char SIGNAL_DLL_CALL sftrans_lp(cplx_vector z, cplx_vector p, double g, double w, cplx_vector& sz, cplx_vector& sp, double& sg);

SIGNAL_EXPORT char SIGNAL_DLL_CALL sftrans_hp(cplx_vector z, cplx_vector p, double g, double w, cplx_vector& sz, cplx_vector& sp, double& sg);

SIGNAL_EXPORT char SIGNAL_DLL_CALL sftrans_bp(cplx_vector z, cplx_vector p, double g, double w0, double w1, cplx_vector& sz, cplx_vector& sp, double& sg);

SIGNAL_EXPORT char SIGNAL_DLL_CALL sftrans_sb(cplx_vector z, cplx_vector p, double g, double w0, double w1, cplx_vector& sz, cplx_vector& sp, double& sg);

SIGNAL_EXPORT char SIGNAL_DLL_CALL sftrans(BANDTYPE type, cplx_vector z, cplx_vector p, double g, double w0, double w1, cplx_vector& sz, cplx_vector& sp, double& sg);

#endif