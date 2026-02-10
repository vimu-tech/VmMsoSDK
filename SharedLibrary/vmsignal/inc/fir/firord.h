#ifndef FIRORD_H
#define FIRORD_H
#include "vmwindow.h"
#include "vmfilter_def.h"
#include "vmsignal_global.h"

#ifdef __cplusplus
extern "C" 
{
#endif

SIGNAL_EXPORT char SIGNAL_DLL_CALL needallpara(WINDOW_STYLE wnd_style);
SIGNAL_EXPORT int32_t SIGNAL_DLL_CALL firmaxas(WINDOW_STYLE wnd_style, int32_t as);
SIGNAL_EXPORT int32_t SIGNAL_DLL_CALL firord(BANDTYPE band, double wp1, double wp2, double ws1, double ws2, double as, WINDOW_STYLE wnd_style, double *bate);

#ifdef __cplusplus
}
#endif
#endif
