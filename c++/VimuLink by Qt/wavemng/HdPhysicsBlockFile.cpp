#include "HdPhysicsBlockFile.h"
#include "wavefile.h"
#include <assert.h>
#include <QDebug>

HdPhysicsBlockFile::HdPhysicsBlockFile(BlockFileInfo *blockfileinfo)
: AbsPhysicsBlockFile(blockfileinfo)
{
}

HdPhysicsBlockFile::~HdPhysicsBlockFile(void)
{
}

bool HdPhysicsBlockFile::ChangeData(int64_t block_offset, void* buffer, uint64_t buffer_len)
{
	assert(pblockfileinfo->length = block_offset + buffer_len);
    pblockfileinfo->curlength = block_offset + buffer_len; //此处如果是改变中间位置的数据，需要再一次处理
	return true;
}

bool HdPhysicsBlockFile::Read(int64_t block_offset, double *buffer, uint64_t* buffer_len)
{
	assert(pblockfileinfo->curlength >= block_offset + *buffer_len);

    WaveFile file;
    file.startRead(pblockfileinfo->pathname);

    *buffer_len = file.readWaves(pblockfileinfo->boxing, buffer, pblockfileinfo->offset + block_offset, *buffer_len);

    qDebug()<<"HdPhysicsBlockFile::Read " << *buffer_len;
    return true;
}
