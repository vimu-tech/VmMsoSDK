#include "TempFileMngRec.h"

TempFileMngRec::TempFileMngRec()
: m_pathname("")
, m_tempDirectory("")
, m_count(0)
{
}

TempFileMngRec::TempFileMngRec(QString pathname, QString tempDirectory)
	: m_pathname(pathname)
	, m_tempDirectory(tempDirectory)
	, m_count(0)
{

}

TempFileMngRec::~TempFileMngRec()
{
}
