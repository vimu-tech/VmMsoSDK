#ifndef BLOCKFILE_H
#define BLOCKFILE_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <string>
#include "BlockFileInfo.h"
#include "MemSummary.h"
#include "AbsPhysicsBlockFile.h"

class BlockFile
{
public:
	explicit BlockFile(BlockFileInfo &blockinfo);
	virtual ~BlockFile(void);

	uint64_t GetCurLen(){return fileinfo.curlength;};

	void ChangeBlockFileInfo(const BlockFileInfo &info);
	BlockFileInfo& GetBlockFileInfo() { return fileinfo; };

	//bool ChangeData(int64_t block_offset, void* buf, int64_t buffer_offset, uint64_t buffer_len);
	bool ChangeData(int64_t block_offset, void* buf, uint64_t buffer_len);

	//
	virtual bool Read(int64_t block_offset, double *buffer, uint64_t* len);
     /// Returns the 256 byte summary data block
    virtual bool Read256(double *buffer, int64_t start, uint64_t* len);
    /// Returns the 64K summary data block
    virtual bool Read64K(double *buffer, int64_t start, uint64_t* len);
	//
	bool ReadMinMax(double *bufmin, double *bufmax);

private:
	BlockFileInfo fileinfo;
    MemSummary *summary;
    AbsPhysicsBlockFile *phyfile;

private:
	//禁止使用
	BlockFile(const BlockFile& t) = delete;
	BlockFile& operator=(const BlockFile& copy) = delete;
};

#endif
