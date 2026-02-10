#ifndef EMSPHYSICSBLOCKFILE_H
#define EMSPHYSICSBLOCKFILE_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include "AbsPhysicsBlockFile.h"
#include "BlockFileInfo.h"

class EmsPhysicsBlockFile :
	public AbsPhysicsBlockFile
{
public:
	explicit EmsPhysicsBlockFile(BlockFileInfo *pblockfileinfo);
	virtual ~EmsPhysicsBlockFile(void);

	virtual bool ChangeData(int64_t block_offset, void* buffer, uint64_t buffer_len) override;
	virtual bool Read(int64_t block_offset, double *buffer, uint64_t* buffer_len) override;

private:
	char *file_buffer;
private:
	//禁止使用
	EmsPhysicsBlockFile(const EmsPhysicsBlockFile& t) = delete;
	EmsPhysicsBlockFile& operator=(const EmsPhysicsBlockFile& copy) = delete;
};

#endif

