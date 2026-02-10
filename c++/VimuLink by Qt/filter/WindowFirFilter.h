#pragma once
#include "BasicFilter.h"
#include <assert.h>
#include <string>
#include "vmwindow.h"

class CWindowFirFilter :
	public CBasicFilter
{
public:
	explicit CWindowFirFilter(LanguageType language = English);
	virtual ~CWindowFirFilter(void);

private:
	CWindowFirFilter(const CWindowFirFilter& copy);
	CWindowFirFilter& operator = (const CWindowFirFilter& copy);
public:
	void Clear();

	int Cal_N();
	int Cal_Window_N(); //给定窗类型后计算N
	double Cal_Hz();
	virtual bool FupinDate(double* FM, int length = 2048) override;
	virtual bool FupindBDate(double* FMdB, int length = 2048) override;
	virtual bool XiangpinDate(double* AM, int length = 2048) override;
	virtual bool JieyueDate(double* jy, size_t* length) override;
	virtual bool MaichongDate(double* mch, size_t* length) override;
	virtual bool QunshiyanDate(double* qsy, int length = 2048) override;
	virtual int ZerosDate(double* re, double* im) override;
	virtual int PolesDate(double* re, double* im) override;
	virtual bool SaveHz(const std::string& namestr, int format0, int format1, char fe, int xishu = 1) override;
	virtual bool SaveZorePoles(const std::string& namestr, int format0, int format1, char fe, int xishu = 1) override;
	virtual bool Savefdd(const std::string& namestr) override;
	virtual bool Loadfdd(const std::string& namestr) override;
	virtual std::string GetInformationstr() override;
	virtual int GetZerosN() override;
	virtual int GetPolesN() override;

	virtual std::string GetName()override
	{
		std::string name;
		const size_t num = pathname.find_last_of('\\');
		name = pathname.substr(num + 1, pathname.length() - 1);
		return name;
	};
	virtual std::string& GetPathName() override { return pathname; };
	//void SetPathName(CString & newpathname){pathname=newpathname;};

	void SetBand(BANDTYPE band);
	BANDTYPE Getband() { return band; };
	void Setsample(int nsample) { sample = nsample; };
	virtual int Getsample() override { return sample; };
	int Getpinlv1() { return Firpinlv1; };
	void Setpinlv1(int pinlv) { Firpinlv1 = pinlv; };
	int Getpinlv2() { return Firpinlv2; };
	void Setpinlv2(int pinlv) { Firpinlv2 = pinlv; };
	void SetN(int n) { N = n; };
	virtual int GetN() override { return N; };
	virtual int GetAp() override { return -1; };
	void SetAp(int app) { assert(false); };
	virtual int GetAs() override { return -1; };
	void SetAs(int ass) { assert(false); };
	char Getiirfirstyle() { return window; };
	WINDOW_STYLE GetWndStyle() { return window; };
	void SetWndStyle(WINDOW_STYLE style) { window = style; };
	double GetWndPara() { return alpha_beta; };
	void SetWndPara(double para) { alpha_beta = para; };
	virtual int GetTimeDisplayvalue() override { return sample / 2; };

	virtual void Filter(int* date, size_t num) override;
	virtual void Filter(double* date, size_t num) override;
private:
	BANDTYPE band;  //类型
	WINDOW_STYLE window; //窗类型
	double alpha_beta;  //凯塞窗的a

	int sample; //采样率
	int Firpinlv1;
	int Firpinlv2;

	int  N;//窗的长度
	double* h;//根据N求的hn
};
