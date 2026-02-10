//#include "stdafx.h"
#include "BasicFilter.h"
#include <fstream>
#include <cstring>

CBasicFilter::CBasicFilter(LanguageType lang)
	: type(F_ERO)
	, language(lang)
{
}

CBasicFilter::~CBasicFilter(void)
{
}

CBasicFilter::FILTERTYPE CBasicFilter::GetFilterStyle(const std::string &namestr)
{
	std::fstream file(namestr, std::fstream::in | std::fstream::binary);

	char szTmp[10];
	memset(szTmp, 0, 10 * sizeof(char));
	file.read(szTmp, 3 * sizeof(char)) ;
	if ((strncmp(szTmp, "IIR", 3) == 0))
	{
		return IIR;
	}
	else if(strncmp(szTmp, "FIR", 3) == 0)
	{
		char firmethed;  //1窗函数法  2频率采样法
		file.read(&firmethed, sizeof(firmethed));
		if(firmethed==1)
			return FIR_WND;
		else
			return FIR_PREQ;
	}
	else
	{
		return F_ERO;
	}
}

std::string CBasicFilter::GetCompanyInfo()
{
	std::string str;

	if(language==Chinese)
	{
		str="/*-------------------------------------------------------------\n";
		str+="                       文件信息\n";
		str+="                 公司:微目电子科技\n";
		str+="                 软件:QFilter\n";
		str+="--------------------------------------------------------------*/\n";
	}
	else
	{
		str="/*-------------------------------------------------------------\n";
		str+="                   File Infomation\n";
		str+="     CompanyName:Vimu Electronic Technology\n";
		str+="                 Software:QFilter\n";
		str+="--------------------------------------------------------------*/\n";
	}
	return str;
}
