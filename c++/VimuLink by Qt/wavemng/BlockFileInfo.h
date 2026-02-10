#ifndef BLOCKFILEINFO_H
#define BLOCKFILEINFO_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/


#include <QString>
#include "wavemngdef.h"
#include "WaveTrackInfo.h"

class BlockFileInfo
{
public:
    BlockFileInfo(WaveTrackInfo& trackinfo, int64_t offset, uint64_t length, QString tempfilename);
	virtual ~BlockFileInfo(void);

	BlockFileInfo& operator=(const BlockFileInfo& copy);

    TrackFileType filetype;

	//硬盘文件路径和名字
    QString pathname;
	//统计文件名
    QString tempfilename;
	//不支持seek,临时文件
    //QString temp_rawfilename;
	//用于硬盘文件的读取时
    int64_t offset;  

	uint64_t length;
	uint64_t curlength;

	sampleFormat format;	
	SHIYUBX_SOURCE boxing;
};

#endif

