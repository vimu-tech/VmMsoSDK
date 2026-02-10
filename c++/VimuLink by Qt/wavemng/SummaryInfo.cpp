#include "SummaryInfo.h"


SummaryInfo::SummaryInfo(sampleFormat fmt, size_t samples)
{
   format = fmt;

   fields = 2; /* min, max */

   bytesPerFrame = SAMPLE_SIZE(format) * fields;

   frames64K = (samples + 65535) / 65536;
   frames256 = frames64K * 256;

   offset64K = headerTagLen;
   offset256 = offset64K + (frames64K * bytesPerFrame);
   totalSummaryBytes = offset256 + (frames256 * bytesPerFrame);
}

SummaryInfo::~SummaryInfo(void)
{
}



