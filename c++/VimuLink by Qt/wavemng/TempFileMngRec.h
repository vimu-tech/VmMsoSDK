#ifndef TEMPFILEMNGREC_H
#define TEMPFILEMNGREC_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/


#include <QString>

class TempFileMngRec
{
public:
	TempFileMngRec(void);
    TempFileMngRec(QString pathname, QString tempDirectory);
	virtual ~TempFileMngRec(void);

    QString m_pathname;
    QString m_tempDirectory;
    int m_count;
private:
	//拷贝构造函数禁止
	//TempFileMngRec(TempFileMngRec& t);
	TempFileMngRec(const TempFileMngRec& t);
};

#endif
