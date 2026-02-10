#pragma once
#include <vector>
#include <string>
#include "vmfilter_def.h"

#define H_N 4096
#define FILTER_FHT_N 4096

class CBasicFilter
{
public:
	typedef enum { English = 0x00000000, Chinese = 0x00000001 } LanguageType;
	typedef enum { F_ERO = 0, FIR_WND = 1, FIR_PREQ = 2, IIR = 3 } FILTERTYPE;

	CBasicFilter(LanguageType language = English);
	virtual ~CBasicFilter(void);

	FILTERTYPE GetFilterType() { return type; };
	//std::string &GetLastError(){return ErrorStr;};
	virtual bool FupinDate(double* x, int length = 2048) = 0;
	virtual bool FupindBDate(double* x, int length = 2048) = 0;
	virtual bool XiangpinDate(double* x, int length = 2048) = 0;
	virtual bool JieyueDate(double* jy, size_t* length) = 0;
	virtual bool MaichongDate(double* mch, size_t* length) = 0;
	virtual bool QunshiyanDate(double* qsy, int length = 2048) = 0;
	virtual int GetZerosN() = 0;
	virtual int GetPolesN() = 0;
	virtual int ZerosDate(double* re, double* im) = 0;
	virtual int PolesDate(double* re, double* im) = 0;
	virtual bool SaveHz(const std::string& namestr, int format0, int format1, char fe, int xishu = 1) = 0;
	virtual bool SaveZorePoles(const std::string& namestr, int format0, int format1, char fe, int xishu = 1) = 0;
	virtual bool Savefdd(const std::string& namestr) = 0;
	virtual bool Loadfdd(const std::string& namestr) = 0;
	virtual std::string GetInformationstr() = 0;
	virtual std::string GetName() = 0;
	virtual std::string& GetPathName() = 0;
	//virtual void SetPathName(CString & pathname)=0;
	virtual int Getsample() = 0;
	virtual int GetN() = 0;
	virtual int GetAp() = 0;
	virtual int GetAs() = 0;
	virtual int GetTimeDisplayvalue() = 0;

	virtual void Filter(int* date, size_t num) = 0;
	virtual void Filter(double* date, size_t num) = 0;

	static FILTERTYPE GetFilterStyle(const std::string& namestr);  //确定载入文件滤波器的类型

	std::string GetCompanyInfo();
protected:
	FILTERTYPE type;
	std::string pathname;
	LanguageType language;
};
