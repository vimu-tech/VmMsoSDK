#ifndef WAVETRACKINFO_H
#define WAVETRACKINFO_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/


#include <QString>
#include <assert.h>
#include "wavemngdef.h"

class WaveTrackInfo
{
public:
    WaveTrackInfo(QString &pathname, bool seek, sampleFormat format, SHIYUBX_SOURCE boxing, uint64_t samples);
	WaveTrackInfo(sampleFormat format, SHIYUBX_SOURCE boxing, uint64_t samples);
	WaveTrackInfo(sampleFormat format, uint64_t samples);
	explicit WaveTrackInfo(sampleFormat format);
	virtual ~WaveTrackInfo();

	WaveTrackInfo& operator=(const WaveTrackInfo& copy);

    TrackFileType GetTrackFileType(){return filetype;};
    QString& GetPathName(){return pathname;};

	sampleFormat GetSampleFormat(){return m_format;};
	SHIYUBX_SOURCE GetBoxingId(){return m_boxing;};
	uint64_t GetSamples(){assert(m_samples!=0); return m_samples;};
private:
    TrackFileType filetype;

    QString pathname;

	//数据格式
	sampleFormat m_format;
	//
	SHIYUBX_SOURCE m_boxing;
	//采样率
	uint64_t m_samples;
};

#endif
