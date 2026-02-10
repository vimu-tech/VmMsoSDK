#ifndef _JAC_E_F
#define _JAC_E_F

#ifdef __cplusplus
extern "C" 
{
#endif

void Jacobi_sn_cn_dn(double u, char arg, double x, double* sn, double* cn, double* dn);
void Jacobi_cs_ds_ns(double u, char arg, double x, double* cs, double* ds, double* ns);
void Jacobi_sc_dc_nc(double u, char arg, double x, double* sc, double* dc, double* nc);
void Jacobi_sd_cd_nd(double u, char arg, double x, double* sd, double* cd, double* nd);

#ifdef __cplusplus
}
#endif

#endif