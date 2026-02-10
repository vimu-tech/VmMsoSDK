#ifndef MEMSUMMARY_H
#define MEMSUMMARY_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/


#include "AbsSummary.h"

class MemSummary : public AbsSummary
{
public:
	explicit MemSummary(BlockFileInfo *pblockfileinfo);
	virtual ~MemSummary(void);

    virtual bool CalSummary(void *buf, int64_t start, uint64_t len);
	/// Returns the 256 byte summary data block
    virtual bool Read256(double *buffer, int64_t start, uint64_t len);
    /// Returns the 64K summary data block
    virtual bool Read64K(double *buffer, int64_t start, uint64_t len);
private:
	char* fullSummary;
private:
	//禁止使用
	MemSummary(const MemSummary& t) = delete;
	MemSummary& operator=(const MemSummary& copy) = delete;
};

#endif
