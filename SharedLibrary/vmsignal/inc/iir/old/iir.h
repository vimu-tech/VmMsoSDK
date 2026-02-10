#ifndef _IIR
#define _IIR
#include <Windows.h>
#include <math.h>
#include "suanfa.h"
#include "jacobi_sn.h"
#include "complete_elliptic_integral_first_kind.h" 
#include "hyperbola.h"
#include "jacobi_cn.h"
#include "jacobi_dn.h"
#include "jacobi_elliptic_functions.h"
#include "complex_jacobi_elliptic_functions.h"
#include "inverse_jacobi_elliptic_functions.h"
#include "cplx_fun.h"

#ifdef __cplusplus
extern "C" 
{
#endif

int Cal_Btw_xishu(int N, int n, double *b, double *a);
double Cal_Btw_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a);
int Cal_Btw_N_moni(int band, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_Btw_N_mch(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_Btw_N_shxx(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_Btw_N(char bianhuan, int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
double Cal_Btw_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a);
char hstohz_mch_each(int n, double* b, double* a, double T);
char hstohz_mch(int ns, int n, double* b, double* a, double T);
int Cal_Btw_Hz_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a);
double Cal_Btw_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a);
char hstohz_shxx_each(int n, double* b, double* a, double T);
int hstohz_shxx(int ns, int n, double* b, double* a, double T);
double Cal_Btw_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a);
int hstohz(char bianhuan, int ns, int n,  double* b, double* a, double T);
char poles(int band, int N, double *fenmu, double *poles_re, double *poles_im);
int polezeros_zhj(int n, double *fenmu, double *poles_re, double *poles_im);
int poles_jl(int ns, int n, double *fenmu, double *poles_re, double *poles_im);
int poles_bl(int ns, int n, double *fenmu, double *poles_re, double *poles_im);
int zeros_jl(int ns, int n, double *fenzi, double *zeros_re, double *zeros_im);
int zeros_bl(int ns, int n, double *fenzi, double *femu, double *zeros_re, double *zeros_im);
void zoroschange_bl(double *K, int ns, int n, double *fenzi, double *femu);
void Hz_zhj_hejw(double K, double *b, double *a, int m, int n, double *x, int len, int sign);
void Hz_jl_hejw(double K, double *b, double *a, int n, int ns, double *x, int len, int sign);
void Hz_bl_hejw(double K, double *b, double *a, int n, int ns, double *x, int len, int sign);
void Hs_jl_hejw(double K, double *b, double *a, int n, int ns, double *x, long int maxpinlv, int len, int sign);
void resp(double K, double *y,  int ly, double *b, double *a, int m, int n, char morj);
void resp_jl(double K, double *b, double *a, int n, int ns, double *y, int len, char morj);
void hs_resp_jl(double K, double *b, double *a, int n, int ns, double *y, int len, char morj);
void resp_bl(double K, double *b, double *a, int n, int ns, double *y, int len, char morj);
void filter(double *b, double *a, int m, int n, double *x, int len, double *px, double *py);
void filte_jl(double K, double *b, double *a, int n, int ns, double *x, int len);
void filte_bl(double K, double *b, double *a, int n, int ns, double *x, int len);
void jltozhj(int ns, int n, double *b, double *a, double *bb, double *aa);
void bltozhj(int ns, int n, double *b, double *a, double *bb, double *aa);
void liushu_jl(int pole_n, int n, int ns, int ns_cur, double *bb, double *a, double* in_re, double* in_im, double *re, double *im);
char jltobl_each(int pole_n, int n, int ns, int ns_cur, double* b, double* a, double *bb, double *lastb, double *lasta);
char jltobl(int ns, int n, double *b, double *a);
double Cal_cbI_e(int ap);
double Cal_cbII_e(int as);
int Cal_cbI_N_moni(int band, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_cbII_N_moni(int band, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_cbI_N_mch(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_cbI_N_shxx(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_cbII_N_mch(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_cbII_N_shxx(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_cbI_N(char bianhuan, int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_cbII_N(char bianhuan, int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
char Cal_cbI_xishu(int N, int k, double e, double *b, double *a);
char Cal_cbII_xishu(int N, int k, double e, double *b, double *a);
double Cal_cbI_hs_basic(int band, int N,  double e, double omgp0, double B,double *b, double *a);
double Cal_cbII_hs_basic(int band, int N,  double e, double omgs0, double B,double *b, double *a);
double Cal_cbI_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double Cal_cbII_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double Cal_cbI_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double Cal_cbII_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double Cal_cbI_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a);
double Cal_cbII_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a);
double Cal_cbI_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double Cal_cbII_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double Cal_elliptic_K(double x, int n);
double Cal_k1tok(int N, double k1);
double Cal_ktok1(int N, double k);
int Cal_elliptic_N_basic(double wp, double ws, double ap, double as);
int Cal_elliptic_N_moni(int band, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_elliptic_N_mch(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_elliptic_N_shxx(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
int Cal_elliptic_N(char bianhuan, int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as);
double Cal_elliptic_xishu(int N, double wp, double ws, double ap, double as, int nn, double *b, double *a);
double Cal_elliptic_hs_basic(int band, int N,  double f1, double f2, double f3, double f4,  double ap, double as, double *b, double *a);
double Cal_elliptic_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double Cal_elliptic_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double Cal_elliptic_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a);
double Cal_elliptic_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a);
double bessel_ak(int n, int k);
char zhjtojl_bessel(int N, double *b, double *a, int n, double *bb, double *aa);
double bessel_hs(int N, double *b, double *a);
double bessel_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double bessel_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a);
double bessel_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a);
double bessel_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a);

#ifdef __cplusplus
}
#endif

#endif