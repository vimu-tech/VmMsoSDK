#ifndef _C_J_E_F
#define _C_J_E_F

#ifdef __cplusplus
extern "C" 
{
#endif

#include "jacobi_sn.h"
#include "jacobi_dn.h"
#include "jacobi_cn.h"


void complex_Jacobi_sn(double ure, double uim, char arg, double x, double *snre, double *snim);
void complex_Jacobi_cn(double ure, double uim, char arg, double x, double *snre, double *snim);
void complex_Jacobi_dn(double ure, double uim, char arg, double x, double *snre, double *snim);
void complex_Jacobi_cd(double ure, double uim, char arg, double x, double *cdre, double *cdim);

#ifdef __cplusplus
}
#endif

#endif