#ifndef SUMMARYINFO_H
#define SUMMARYINFO_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <cstddef>
#include "wavemngdef.h"
/****************************************************************//**

\class SummaryInfo
\brief Works with BlockFile to hold info about max and min and RMS 
over multiple samples, which in turn allows rapid drawing when zoomed 
out.

*//*******************************************************************/

const static int headerTagLen = 19;
const static char headerTag[headerTagLen + 1] = "DgnXnxhfxyBlockFile";

class SummaryInfo
{
public:
	SummaryInfo(sampleFormat format, size_t samples);
	virtual ~SummaryInfo(void);

   int              fields; /* Usually 3 for Min, Max, RMS */
   sampleFormat     format;
   int              bytesPerFrame;
   size_t           frames64K;
   size_t           offset64K;
   size_t           frames256;
   size_t           offset256;
   size_t           totalSummaryBytes;
};

#endif

