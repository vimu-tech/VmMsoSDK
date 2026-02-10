#include "WaveTrackInfo.h"

WaveTrackInfo::WaveTrackInfo(QString &pnm, bool seek, sampleFormat fmt, SHIYUBX_SOURCE bx, uint64_t sam)
: filetype(PhysicsFile)
, pathname(pnm)
, m_format(fmt)
, m_boxing(bx)
, m_samples(sam)
{
} 

WaveTrackInfo::WaveTrackInfo(sampleFormat fmt, SHIYUBX_SOURCE bx, uint64_t sam)
: filetype(EmsFile)
, pathname()
, m_format(fmt)
, m_boxing(bx)
, m_samples(sam)
{
}

WaveTrackInfo::WaveTrackInfo(sampleFormat fmt, uint64_t sam)
: filetype(EmsFile)
, pathname()
, m_format(fmt)
, m_boxing(SHIYUBX_SOURCE_ZERO)
, m_samples(sam)
{
}

WaveTrackInfo::WaveTrackInfo(sampleFormat fmt)
: filetype(EmsFile)
, pathname()
, m_format(fmt)
, m_boxing(SHIYUBX_SOURCE_ZERO)
, m_samples(0)
{
}

WaveTrackInfo::~WaveTrackInfo()
{
}

WaveTrackInfo& WaveTrackInfo::operator=(const WaveTrackInfo& copy)
{
    filetype=copy.filetype;
    pathname=copy.pathname;
    m_format=copy.m_format;
    m_boxing=copy.m_boxing;
    m_samples=copy.m_samples;
	return *this;
}
