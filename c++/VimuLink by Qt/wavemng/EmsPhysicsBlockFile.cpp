#include "EmsPhysicsBlockFile.h"
#include <assert.h>
#include <memory.h>

EmsPhysicsBlockFile::EmsPhysicsBlockFile(BlockFileInfo *pblockfileinfo)
: AbsPhysicsBlockFile(pblockfileinfo)
{
	file_buffer = new char[pblockfileinfo->length*SAMPLE_SIZE(pblockfileinfo->format)];
}

EmsPhysicsBlockFile::~EmsPhysicsBlockFile(void)
{
	if(file_buffer != nullptr)
	{
		delete [] file_buffer;
		file_buffer = nullptr;
	} 
}
//******************只能支持前面数据填满，在填后面*******************************
bool EmsPhysicsBlockFile::ChangeData(int64_t block_offset, void* buffer, uint64_t buffer_len)
{
	assert(pblockfileinfo->length = block_offset + buffer_len);
    pblockfileinfo->curlength = block_offset + buffer_len;
	memcpy(file_buffer + block_offset *SAMPLE_SIZE(pblockfileinfo->format), buffer, buffer_len*SAMPLE_SIZE(pblockfileinfo->format));
	return true;
}

bool EmsPhysicsBlockFile::Read(int64_t block_offset, double *buffer, uint64_t* buffer_len)
{
	assert(block_offset + *buffer_len <= pblockfileinfo->length);
	*buffer_len = *buffer_len >= pblockfileinfo->curlength - block_offset ? pblockfileinfo->curlength - block_offset : *buffer_len;

	if(pblockfileinfo->format==doubleSample)
   {
	   memcpy(buffer, (char*)file_buffer + block_offset * SAMPLE_SIZE(pblockfileinfo->format), *buffer_len*SAMPLE_SIZE(pblockfileinfo->format));
   }
   else if(pblockfileinfo->format==floatSample)
   {
	   for(size_t i=0;i<*buffer_len;i++)
		   buffer[i]=((float*)file_buffer)[i+ block_offset];
   }
   else if((pblockfileinfo->format==int24Sample)||(pblockfileinfo->format==int32Sample)||(pblockfileinfo->format==uint32Sample))
   {
	   for(size_t i=0;i<*buffer_len;i++)
		   buffer[i]=((int*)file_buffer)[i+ block_offset];
   }
   else if(pblockfileinfo->format==int16Sample)
   {
	   for(size_t i=0;i<*buffer_len;i++)
		   buffer[i]=((short*)file_buffer)[i+ block_offset];
   }
   if(pblockfileinfo->format==int8Sample)
   {
	   for(size_t i=0;i<*buffer_len;i++)
		   buffer[i]=((char*)file_buffer)[i+ block_offset];
   }
 
   return true;
}
