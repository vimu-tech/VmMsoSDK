#include "MemSummary.h"
#include <assert.h>

MemSummary::MemSummary(BlockFileInfo *pblockfileinfo)
: AbsSummary(pblockfileinfo)
{
	fullSummary=new char[mSummaryInfo.totalSummaryBytes];
}

MemSummary::~MemSummary(void)
{
	if(fullSummary!= nullptr)
	{
		delete [] fullSummary;
		fullSummary= nullptr;
	}
}

bool MemSummary::CalSummary(void *buf, int64_t start, uint64_t len)
{
   if(pblockfileinfo->format==doubleSample)
   {
        CalSummary2(fullSummary, (double*)buf, start, len);
   }
   else if(pblockfileinfo->format==floatSample)
   {
        CalSummary2(fullSummary, (float*)buf, start, len);
   }
   else if((pblockfileinfo->format==int24Sample)||(pblockfileinfo->format==int32Sample)||(pblockfileinfo->format==uint32Sample))
   {
       CalSummary2(fullSummary, (int*)buf, start, len);
   }
   else if(pblockfileinfo->format==int16Sample)
   {
        CalSummary2(fullSummary, (short*)buf, start, len);
   }
   else if(pblockfileinfo->format==int8Sample)
   {
        CalSummary2(fullSummary, (char*)buf, start, len);
   }
#ifdef _DEBUG
   else
   {
	    assert(false);
		return false;
   }
#endif
   return true;
}

/// Retrieves a portion of the 256-byte summary buffer from this BlockFile.  This
/// data provides information about the minimum value, the maximum
/// value, and the maximum RMS value for every group of 256 samples in the
/// file.
///
/// @param *buffer The area where the summary information will be
///                written.  It must be at least len*3 long.
/// @param start   The offset in 256-sample increments
/// @param len     The number of 256-sample summary frames to read
bool MemSummary::Read256(double *buffer, int64_t start, uint64_t len)
{
   assert(start >= 0);

   if (start+len > mSummaryInfo.frames256)
      len = mSummaryInfo.frames256 - start;

   memcpy(buffer,fullSummary + mSummaryInfo.offset256 + (start * mSummaryInfo.bytesPerFrame),
	   len*mSummaryInfo.bytesPerFrame);

   return true;
}

/// Retrieves a portion of the 64K summary buffer from this BlockFile.  This
/// data provides information about the minimum value, the maximum
/// value, and the maximum RMS value for every group of 64K samples in the
/// file.
///
/// @param *buffer The area where the summary information will be
///                written.  It must be at least len*3 long.
/// @param start   The offset in 64K-sample increments
/// @param len     The number of 64K-sample summary frames to read
bool MemSummary::Read64K(double *buffer, int64_t start, uint64_t len)
{
   assert(start >= 0);

   if (start+len > mSummaryInfo.frames64K)
      len = mSummaryInfo.frames64K - start;

    memcpy(buffer,fullSummary + mSummaryInfo.offset64K + (start * mSummaryInfo.bytesPerFrame),
	   len*mSummaryInfo.bytesPerFrame);

   return true;
}
