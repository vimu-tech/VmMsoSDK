#include "BlockFileInfo.h"

BlockFileInfo::BlockFileInfo(WaveTrackInfo& trackinfo, int64_t ofs, uint64_t len, QString tempname)
    : filetype(trackinfo.GetTrackFileType())
    , pathname(trackinfo.GetPathName())
	, format(trackinfo.GetSampleFormat())
	, boxing(trackinfo.GetBoxingId())
    , tempfilename(tempname)
    //, temp_rawfilename(tempname+ "raw")
	, offset(ofs)
	, length(len)
	, curlength(-1)
{
	//DebugString("BlockFileInfo offset=%lld length=%lld\n", offset, length);
}

BlockFileInfo::~BlockFileInfo(void)
{
}

BlockFileInfo& BlockFileInfo::operator=(const BlockFileInfo& copy)
{
    filetype=copy.filetype;
	pathname=copy.pathname;
	offset=copy.offset;
	length=copy.length;
	curlength=copy.curlength;
	format=copy.format;
	boxing=copy.boxing;
    tempfilename=copy.tempfilename;
    //temp_rawfilename=copy.temp_rawfilename;

	return *this;
}
