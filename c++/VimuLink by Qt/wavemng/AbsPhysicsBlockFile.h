#ifndef ABSPHYSICSBLOCKFILE_H
#define ABSPHYSICSBLOCKFILE_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <string>
#include "wavemngdef.h"
#include "BlockFileInfo.h"

class AbsPhysicsBlockFile
{
public:
	explicit AbsPhysicsBlockFile(BlockFileInfo *pblockfileinfo);
	virtual ~AbsPhysicsBlockFile(void);

	virtual bool ChangeData(int64_t block_offset, void* buffer, uint64_t buffer_len)=0;
	virtual bool Read(int64_t block_offset, double *buffer, uint64_t* buffer_len)=0;

protected:
	BlockFileInfo *pblockfileinfo;
};

#endif

