#include "BlockFile.h"
#include <assert.h>
#include "EmsPhysicsBlockFile.h"
#include "HdPhysicsBlockFile.h"

BlockFile::BlockFile(BlockFileInfo &blockinfo)
: fileinfo(blockinfo)
{
    summary=new MemSummary(&fileinfo);
    if(blockinfo.filetype==EmsFile)
        phyfile=new EmsPhysicsBlockFile(&fileinfo);
    else
        phyfile=new HdPhysicsBlockFile(&fileinfo);
}

BlockFile::~BlockFile(void)
{
	if(summary!= nullptr)
		delete summary;
	if(phyfile!= nullptr)
		delete phyfile;
}

void BlockFile::ChangeBlockFileInfo(const BlockFileInfo &info)
{
	fileinfo=info;
}

/*bool BlockFile::ChangeData(int64_t block_offset, void* buf, int64_t buffer_offset, uint64_t buffer_len)
{
	bool result=false;
	result = phyfile->ChangeData(block_offset, (char*)buf + buffer_offset *SAMPLE_SIZE(fileinfo.format), buffer_len);
	result &= summary->CalSummary((char*)buf+ buffer_offset *SAMPLE_SIZE(fileinfo.format), block_offset, buffer_len);
	return result;
}*/

bool BlockFile::ChangeData(int64_t block_offset, void* buf, uint64_t buffer_len)
{
	bool result=false;
	result = phyfile->ChangeData(block_offset, buf, buffer_len);
	result &= summary->CalSummary(buf, block_offset, buffer_len);
	return result;
}

bool BlockFile::Read(int64_t block_offset, double *fbuffer, uint64_t* len)
{
	return phyfile->Read(block_offset, fbuffer, len);
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
bool BlockFile::Read256(double *buffer, int64_t start, uint64_t* len)
{
   uint64_t cur=(fileinfo.curlength-start+255)/256;
   *len=*len>cur? cur:*len;
   summary->Read256(buffer,start,*len);
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
bool BlockFile::Read64K(double *buffer, int64_t start, uint64_t* len)
{
   uint64_t cur=(fileinfo.curlength-start+65535)/65536;
   *len=*len>cur? cur:*len;
   summary->Read64K(buffer,start,*len);
   return true;
}

bool BlockFile::ReadMinMax(double *bufmin, double *bufmax)
{
	return summary->ReadMinMax(bufmin,bufmax);
}
