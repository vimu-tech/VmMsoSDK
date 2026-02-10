#pragma once
#include "BasicFilter.h"
#include <assert.h>
#include "vmwindow.h"

class CFreqFirFilter :
	public CBasicFilter
{
public:
	explicit CFreqFirFilter(LanguageType language = English);
	virtual ~CFreqFirFilter(void);

private:
	CFreqFirFilter(const CFreqFirFilter& copy);
public:
	CFreqFirFilter& operator = (const CFreqFirFilter& copy);
	void Clear();

	double Cal_Hz(int n, double* f, double* m, int fm_n, WINDOW_STYLE window, double alpha_beta);
	virtual bool FupinDate(double* FM, int length = 4096) override;
	virtual bool FupindBDate(double* FMdB, int length = 4096) override;
	virtual bool XiangpinDate(double* AM, int length = 4096) override;
	virtual bool JieyueDate(double* jy, size_t* length) override;
	virtual bool MaichongDate(double* mch, size_t* length) override;
	virtual bool QunshiyanDate(double* qsy, int length = 4096) override;
	virtual int GetZerosN() override;
	virtual int GetPolesN() override;
	virtual int ZerosDate(double* re, double* im) override;
	virtual int PolesDate(double* re, double* im) override;
	virtual bool SaveHz(const std::string& namestr, int format0, int format1, char fe, int xishu = 1) override;
	virtual bool SaveZorePoles(const std::string& namestr, int format0, int format1, char fe, int xishu = 1) override;
	virtual bool Savefdd(const std::string& namestr) override;
	virtual bool Loadfdd(const std::string& namestr) override;
	virtual std::string GetInformationstr() override;

	virtual std::string GetName() override
	{
		std::string name;
		const size_t num = pathname.find_last_of('\\');
		name = pathname.substr(num + 1, pathname.length() - 1);
		return name;
	};
	virtual std::string& GetPathName() override { return pathname; };
	//void SetPathName(CString & newpathname){pathname=newpathname;};

	virtual int Getsample() override { return sample; };
	virtual int GetN() override { return N; };
	int GetNorFreq(double** freq) { *freq = norfreq; return freq_af_n; };
	int GetAf(double** aaf) { *aaf = af; return freq_af_n; };
	virtual int GetAp() override { return -1; };
	virtual int GetAs() override { return -1; };
	WINDOW_STYLE GetWndStyle() { return window; };
	void SetWndStyle(WINDOW_STYLE style) { window = style; };
	double GetWndPara() { return alpha_beta; };
	void SetWndPara(double para) { alpha_beta = para; };
	virtual int GetTimeDisplayvalue() override { return sample / 2; };

	virtual void Filter(int* date, size_t num) override;
	virtual void Filter(double* date, size_t num) override;
private:
	int sample; //采样率

	double* norfreq; //归一化频率
	double* af; //频率点对应的幅度(0~1)
	int freq_af_n; //给出频率和幅度的个数

	WINDOW_STYLE window; //窗类型
	double alpha_beta;  //凯塞窗的a

	int  N;//长度
	double* h;//根据N求的hn

	//int savecount;
};

