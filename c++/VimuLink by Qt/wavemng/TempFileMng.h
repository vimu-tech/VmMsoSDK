#ifndef TEMPFILEMNG_H
#define TEMPFILEMNG_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/


#include <QString>
#include <QList>
#include "TempFileMngRec.h"

class TempFileMng
{
public:
	TempFileMng();
	virtual ~TempFileMng(void);

    QString GetTempFilename(const QString &pathname);
private:
    QString m_tempfiledir;
    QList<TempFileMngRec*> filelist;

	void DestoryAll();
    QString CreateTempDirectory(const QString &pathname);
};

#endif
