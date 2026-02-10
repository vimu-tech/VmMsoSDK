#ifndef VMSIGNAL_GLOBAL_H
#define VMSIGNAL_GLOBAL_H
#include "basic_global.h"


#if defined(SIGNAL_STATIC_LIBRARY)
    #define SIGNAL_EXPORT 
#elif defined(SIGNAL_SHARED_LIBRARY)
    #define SIGNAL_EXPORT Q_DECL_EXPORT
#else
    #define SIGNAL_EXPORT Q_DECL_IMPORT
#endif

#define SIGNAL_DLL_CALL Q_DECL_CALL

#endif // FFT_GLOBAL_H
