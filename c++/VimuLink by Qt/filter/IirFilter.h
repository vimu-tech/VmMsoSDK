#pragma once
#include "BasicFilter.h"
#include "vmcplx_vector.h"

class CIirFilter :
	public CBasicFilter
{
public:
	explicit CIirFilter(LanguageType language=English);
	virtual ~CIirFilter(void);

private:
	CIirFilter(const CIirFilter &copy);
public:
	CIirFilter& operator =(const CIirFilter& copy);
    void Clear();

	int Cal_N();
	double Cal_Hz();
	virtual bool FupinDate(double* x, int length=2048) override;
	virtual bool FupindBDate(double* FMdB, int length=2048) override;
	virtual bool XiangpinDate(double* AM, int length=2048) override;
	virtual bool JieyueDate(double *jy, size_t *length) override;
	virtual bool MaichongDate(double *mch, size_t *length) override;
	virtual bool QunshiyanDate(double* qsy, int length=2048) override;
	virtual int ZerosDate(double *re, double *im) override;
	virtual int PolesDate(double *re, double *im) override;
	virtual bool SaveHz(const std::string &namestr, int format0, int format1, char fe, int xishu=1) override;
	virtual bool SaveZorePoles(const std::string &namestr, int format0, int format1, char fe, int xishu=1) override;
	virtual bool Savefdd(const std::string &namestr) override;
	virtual bool Loadfdd(const std::string &namestr) override;
	virtual std::string GetInformationstr() override;
	virtual int GetZerosN() override;
	virtual int GetPolesN() override;

	virtual std::string GetName() override
	{
		std::string name;
		const size_t num=pathname.find_last_of('\\');
		name=pathname.substr(num+1,pathname.length()-1);
		return name;
	};
	virtual std::string& GetPathName() override {return pathname;};
	//void SetPathName(CString & pathnamestr){pathname=pathnamestr;};

	void SetBand(BANDTYPE band);
    BANDTYPE Getband() const {return band;};
	void Setsample(int nsample){sample=nsample;};
	virtual int Getsample() override {return sample;};
    int Getpinlv1() const {return Iirpinlv1;};
	void Setpinlv1(int pinlv){Iirpinlv1=pinlv;};
	int Getpinlv2() const {return Iirpinlv2;};
	void Setpinlv2(int pinlv){Iirpinlv2=pinlv;};
	int Getpinlv3() const {return Iirpinlv3;};
	void Setpinlv3(int pinlv){Iirpinlv3=pinlv;};
	int Getpinlv4() const {return Iirpinlv4;};
	void Setpinlv4(int pinlv){Iirpinlv4=pinlv;};
	virtual int GetN() override {return N;};
	void SetN(int n){N=n;};
	virtual int GetAp() override {return ap;};
	void SetAp(int aap){ap=aap;};
	virtual int GetAs() override {return as;};
	void SetAs(int aas){as=aas;};
	char Getiirfirstyle() const {return iirstyle;};
	void Setiirfirstyle(char style){iirstyle=style;};
	virtual int GetTimeDisplayvalue() override {return sample/2;};

	virtual void Filter(int *date, size_t num) override;
	virtual void Filter(double *date, size_t num) override;
private:
	BANDTYPE band;  //类型
	//char bianhuanstyle; //变换方法  0脉冲响应不变法   1双线性变换法
	char iirstyle;  //巴特沃斯（0）  切比雪夫I（1） 切比雪夫II（2）  椭圆(3)  贝塞尔(4)
	int sample; //采样率
	int Iirpinlv1;
	int Iirpinlv2;
    int Iirpinlv3;
	int Iirpinlv4;//上下限频率
	
	int  ap;
	int  as;
	int  N;//阶数
	double wn1;
	double wn2;

	cplx_vector z;
	cplx_vector p;
	double g;
	std::vector<double> b,a;
};
