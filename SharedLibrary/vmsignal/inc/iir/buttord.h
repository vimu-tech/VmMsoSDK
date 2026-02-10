#ifndef BUTTORD_H
#define BUTTORD_H
#include <stdint.h>
#include "vmfilter_def.h"
#include "vmsignal_global.h"

#ifdef __cplusplus
extern "C" 
{
#endif

SIGNAL_EXPORT int32_t SIGNAL_DLL_CALL buttord(BANDTYPE type, double wp1, double wp2, double ws1, double ws2, double rp, double rs, char opt, double& wn1, double& wn2);

#ifdef __cplusplus
}
#endif
#endif