#include "AbsSummary.h"
#include <math.h>
#include <assert.h>


AbsSummary::AbsSummary(BlockFileInfo *blockfileinfo)
: pblockfileinfo(blockfileinfo)
, mSummaryInfo(doubleSample, blockfileinfo->length)
, mMin(0)
, mMax(0)
{
}

AbsSummary::~AbsSummary(void)
{
}




