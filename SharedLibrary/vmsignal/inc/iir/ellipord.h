#ifndef ELLIPORD_H
#define ELLIPORD_H
#include <stdint.h>
#include "vmfilter_def.h"
#include "vmsignal_global.h"

SIGNAL_EXPORT int32_t SIGNAL_DLL_CALL ellipord(BANDTYPE type, double wp1, double wp2, double ws1, double ws2, double rp, double rs, char opt, double& wn1, double& wn2);

#endif