#ifndef HDPHYSICSBLOCKFILE_H
#define HDPHYSICSBLOCKFILE_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include "AbsPhysicsBlockFile.h"
#include "BlockFileInfo.h"

class HdPhysicsBlockFile:
    public AbsPhysicsBlockFile
{
public:
	explicit HdPhysicsBlockFile(BlockFileInfo *pblockfileinfo);
	virtual ~HdPhysicsBlockFile(void);

    virtual bool ChangeData(int64_t block_offset, void* buffer, uint64_t buffer_len);
    virtual bool Read(int64_t block_offset, double *buffer, uint64_t* buffer_len);
};

#endif
