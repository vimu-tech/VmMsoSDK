//#include "stdafx.h"
#include "FreqFirFilter.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <iostream>
#include "fir1.h"
#include "fir2.h"
#include "vmsuanfa.h"
#include "vmfilters.h"

CFreqFirFilter::CFreqFirFilter(LanguageType language/*=English*/)
	: CBasicFilter(language)
	, sample(1000)
	, N(20)
	, h(NULL)
	, norfreq(NULL)
	, af(NULL)
	, freq_af_n(0)
	, window(RECTANGLE)
	, alpha_beta(2.120)
	//, savecount(-1)
{
	type=FIR_PREQ;

	freq_af_n=7;
	norfreq=new double[freq_af_n];
	norfreq[0]=0; 
	norfreq[1]=0.2;
	norfreq[2]=0.3;
	norfreq[3]=0.4;
	norfreq[4]=0.6;
	norfreq[5]=0.7;
	norfreq[6]=1.0;

	af=new double[freq_af_n];
	af[0]=0.0; 
	af[1]=1.0;
	af[2]=1.0;
	af[3]=0.0;
	af[4]=1.0;
	af[5]=1.0;
	af[6]=0.0;
}

CFreqFirFilter::~CFreqFirFilter(void)
{
	if(h!=NULL)
		delete []h;
	if(af!=NULL)
		delete []af;
	if(norfreq!=NULL)
		delete []norfreq;
}

CFreqFirFilter::CFreqFirFilter(const CFreqFirFilter &copy)
{
	pathname=copy.pathname;
	sample=copy.sample;
	N=copy.N;
	freq_af_n=copy.freq_af_n;
	window=copy.window;
	alpha_beta=copy.alpha_beta;

	//if(h!=NULL)   //此处不能加，因为如果是按构造函数使用的话，h并没有初始化为NULL
		//delete []h;
	h = new double[N];
	memcpy(h, copy.h, (copy.N)*sizeof(double));
	af = new double[freq_af_n];
	memcpy(af, copy.af, (copy.freq_af_n)*sizeof(double));
	norfreq = new double[freq_af_n];
	memcpy(norfreq, copy.norfreq, (copy.freq_af_n)*sizeof(double));

	//savecount = copy.savecount;
}

CFreqFirFilter& CFreqFirFilter::operator =(const CFreqFirFilter& copy)
{
	pathname=copy.pathname;
	sample=copy.sample;
	N=copy.N;
	freq_af_n=copy.freq_af_n;
	window=copy.window;
	alpha_beta=copy.alpha_beta;

    if(h!=NULL)
		delete []h;
	h = new double[N];
	memcpy(h, copy.h, (copy.N)*sizeof(double));
	if(af!=NULL)
		delete []af;
	af = new double[freq_af_n];
	memcpy(af, copy.af, (copy.freq_af_n)*sizeof(double));
	if(norfreq!=NULL)
		delete []norfreq;
	norfreq = new double[freq_af_n];
	memcpy(norfreq, copy.norfreq, (copy.freq_af_n)*sizeof(double));

	//savecount = copy.savecount;
    return *this;
}

void CFreqFirFilter::Clear()
{
	sample=1000;
	N=0;
	freq_af_n=0;
	window=RECTANGLE;
	alpha_beta=2.120;

    if(h!=NULL)
		delete []h;
    h=NULL;//根据N求的hn
	if(af!=NULL)
		delete []af;
	af=NULL;
	if(norfreq!=NULL)
		delete []norfreq;
	norfreq=NULL;
}

double CFreqFirFilter::Cal_Hz(int n, double* f, double* m, int fm_n, WINDOW_STYLE windowsty, double alphabeta)
{
	N=n;
	freq_af_n=fm_n;
	window=windowsty;
	alpha_beta=alphabeta;

	if(norfreq!=NULL)
		delete[] norfreq;
	norfreq=new double[freq_af_n];
	memcpy(norfreq,f,freq_af_n*sizeof(double));

	if(af!=NULL)
		delete [] af;
	af=new double[freq_af_n];
	memcpy(af,m,freq_af_n*sizeof(double));

	if(h!=NULL) 
		delete [] h;
	h=new double[N+1];
	fir2(h, N, norfreq, af, freq_af_n, 512, 512/120.0, window, alpha_beta);

	//for(int i=0; i<N+1; i++)
		//TRACE("%f	",h[i]);
	//TRACE("\n");
	return 0;
}

bool CFreqFirFilter::FupinDate(double* FM, int length)
{
	double *Hrm=new double[length];
	double *Him=new double[length];

	fir_Hw(h, N+1, Hrm, Him, length);
	fir_Amplitude(Hrm, Him, FM, length, 0);  //只求一半

	delete []Hrm;
	delete []Him;
	return true;
}

bool CFreqFirFilter::FupindBDate(double* FMdB, int length)
{
	double *Hrm=new double[length];
	double *Him=new double[length];

	fir_Hw(h, N+1, Hrm, Him, length);
	fir_Amplitude(Hrm, Him, FMdB, length, 1);  //只求一半

	delete []Hrm;
	delete []Him;
	return true;
}

bool CFreqFirFilter::XiangpinDate(double* AM, int length)
{
	double *Hrm=new double[2*length];
	double *Him=new double[2*length];

	fir_Hw(h, N+1, Hrm, Him, 2*length);
	fir_Phase(Hrm, Him, AM, length, 0, length);
	
	delete []Hrm;
	delete []Him;
	return true;
}

bool CFreqFirFilter::JieyueDate(double *jy, size_t *length)
{
	fir_resp(N+1, h, jy);
    *length=N+1;
	return true;
}

bool CFreqFirFilter::MaichongDate(double *mch, size_t *length)
{
	memcpy(mch, h, (N+1)*sizeof(double));
	*length=N+1;
	return true;
}

bool CFreqFirFilter::QunshiyanDate(double* qsy, int length)
{
	double *Hrm=new double[2*length];
	double *Him=new double[2*length];

	double dw=(double)sample/2.0/length;

	fir_Hw(h, N+1, Hrm, Him, 2*length);
	fir_Phase(Hrm, Him, qsy, length, 0, length);

	double last_qsy=qsy[1];
	double temp;
	for(int i=2;i<length;i++) 
	{
		temp=qsy[i];
        qsy[i]=(qsy[i]-last_qsy)/dw;
		last_qsy=temp;
	}
	qsy[0]=qsy[1]=qsy[2];

	delete []Hrm;
	delete []Him;
	return true;
}

int CFreqFirFilter::GetZerosN()
{
	int zeronum=0;
	 for(int i=0;i<(N+1)/2;i++)
	 {
		if(fabs(*((h)+N-i))<0.000000001)
			zeronum++;
		else 
			break;
	 }
	 return N-zeronum;
}

int CFreqFirFilter::GetPolesN()
{
	return N;
}

int CFreqFirFilter::ZerosDate(double *re, double *im)
{
	//将z^n次这一项如果是0的话去掉,否则调用qrrt（）失败
	 int zeronum=0;
	 for(int i=0;i<(N+1)/2;i++)
	 {
		if(fabs(*((h)+N-i))<0.000000001)
			zeronum++;
		else 
			break;
	 }

	 if(qrrt(N-zeronum, h, re, im, 0.00001, 100)<0)
		 return -1;
	return N-zeronum;
}

int CFreqFirFilter::PolesDate(double *re, double *im)
{
	//存放极点FIR有N阶重极点0
    for(int i=0;i<N;i++)
	{
		re[i]=0.0;
		im[i]=0.0;
	}
	return N;
}

bool CFreqFirFilter::SaveHz(const std::string &namestr, int format0, int format1, char fe, int xishu)
{
	std::fstream file(namestr, std::fstream::out);
	pathname=namestr;

	//更新hn显示去的数据
	file<<GetCompanyInfo();

	if(language==Chinese)
	{
		file<<"/*--------------------------------说明-----------------------------------\r\n";
		file<<"   H(z)的给出形式为\r\n";
	}
	else
		file<<"/*----------------------------Information-----------------------------------\n";

	file<<"                        H(z)=b(0)+b(1)z^(-1)+ ... + b(n)z^(-n)\r\n";
    file<<"*/\n";

	if(language==Chinese)
	{
		file<<"/*\n频率采样法\n";
		file<<"归一化频率(0-1)\n";
	}
	else
	{
		file<<"/*\nFreq Sample Method\n";
		file<<"Normal Freq(0-1)\n";
	}

	char temp[60];
	for(int i=0;i<freq_af_n;i++)
	{
		sprintf(temp,"%0.4f  ",norfreq[i]);
		file<<temp;
	}
	file<<"\n";

	if(language==Chinese)
		file<<"频率点幅度\n";
	else
		file<<"Margin of sample points\n";

	for(int i=0;i<freq_af_n;i++)
	{
		sprintf(temp,"%0.4f  ",af[i]);
		file<<temp;
	}
	file<<"\n";

	if(language==Chinese)
	{
		file<<"窗类型:"<<Window_Name[window]<<"\n长度 "<<N<<"\n";	
		file<<"乘以系数:"<<xishu<<"\n";
	}
	else
	{
		file<<"Window Type:"<<Window_Name[window]<<"\nLength "<<N<<"\n";	
		file<<"Multiplied Coefficient:"<<xishu<<"\n";
	}
	file<<"*/\n\n";

	file<<"int h["<<N<<"]={\n              ";
	if(fe=='f')
	{
    	for(int i=0;i<N;i++)
     	{
		    if(i==N-1)
                sprintf(temp,"%*.*f",format0, format1,(h)[i]*xishu);
	        else
		    	sprintf(temp,"%*.*f, ",format0, format1,(h)[i]*xishu);
		    file<<temp;
		    if(i>0 && i!=(N-1) &&(i+1)%5==0)
	   	        file<<"\n              ";
	    }
	}
	else
	{
		for(int i=0;i<N;i++)
     	{
		    if(i==N-1)
                sprintf(temp,"%*.*e",format0, format1,(h)[i]*xishu);
	        else
		    	sprintf(temp,"%*.*e, ",format0, format1,(h)[i]*xishu);
		    file<<temp;
		    if(i>0 && i!=(N-1) &&(i+1)%5==0)
	   	        file<<"\n              ";
	    }
	}
	file<<"\n              };";

	file.close();
	file.clear();
	return true;
}

bool CFreqFirFilter::SaveZorePoles(const std::string &namestr, int format0, int format1, char fe, int xishu)
{
	std::fstream file(namestr, std::fstream::out);
	pathname=namestr;

	//更新hn显示去的数据
	file<<GetCompanyInfo();

	if(language==Chinese)
	{
		file<<"/*---------------------------------零极点形式----------------------------------*/\n";
		file<<"/*\n频率采样法\n";
		file<<"归一化频率(0-1)\n";
	}
	else
	{
		file<<"/*---------------------------------Zero-Pole----------------------------------*/\n";
		file<<"/*\nFreq Sample Method\n";
		file<<"Normal Freq(0-1)\n";
	}

	char temp[60];
	for(int i=0;i<freq_af_n;i++)
	{
		sprintf(temp,"%0.4f  ",norfreq[i]);
		file<<temp;
	}
	file<<"\n";

	if(language==Chinese)
		file<<"频率点幅度\n";
	else
		file<<"Margin of sample points\n";

	for(int i=0;i<freq_af_n;i++)
	{
		sprintf(temp,"%0.4f  ",af[i]);
		file<<temp;
	}
	file<<"\n";

	if(language==Chinese)
	{
		file<<"窗类型:"<<Window_Name[window]<<"\n长度 "<<N<<"\n";	
		file<<"乘以系数:"<<xishu<<"\n";
	}
	else
	{
		file<<"Window Type:"<<Window_Name[window]<<"\nLength "<<N<<"\n";	
		file<<"Multiplied Coefficient:"<<xishu<<"\n";
	}
	file<<"*/\n\n";

	size_t zeronum=GetZerosN();
	double *re=new double[zeronum];
	double *im=new double[zeronum];

	ZerosDate(re, im);

	if(language==Chinese)
		file<<"/*---------------------------------零点----------------------------------*/\n";
	else
		file<<"/*--------------------------------Zeros----------------------------------*/\n";

	if(fe=='f')
	{
    	for(size_t i=0;i<zeronum;i++)
     	{
            sprintf(temp,"\t%*.*f+i%*.*f\n", format0, format1, re[i]*xishu, format0, format1, im[i]*xishu);
		    file<<temp;
	    }
	}
	else
	{
		for(size_t i=0;i<zeronum;i++)
     	{
		    sprintf(temp,"\t%*.*e+i%*.*e\n", format0, format1, re[i]*xishu, format0, format1, im[i]*xishu);
		    file<<temp;
	    }
	}

	delete []re;
	delete []im;

	file.close();
	file.clear();
	return true;
}

bool CFreqFirFilter::Savefdd(const std::string &namestr)
{
	std::fstream file(namestr, std::fstream::out | std::fstream::binary);
	pathname=namestr;

	file.write("FIR", 3);
	char firmethed=2;  //1窗函数法  2频率采样法
	file.write(&firmethed, sizeof(firmethed));
	file.write((char*)(&window), sizeof(window));
	file.write((char*)(&alpha_beta), sizeof(alpha_beta));
	file.write((char*)(&sample), sizeof(sample));
	
	assert(sizeof(freq_af_n)==4);
	file.write((char*)(&freq_af_n), sizeof(freq_af_n));
	for(int i=0;i<freq_af_n;i++)
		file.write((char*)(norfreq+i), sizeof(double));
	for(int i=0;i<freq_af_n;i++)
		file.write((char*)(af+i), sizeof(double));

	file.write((char*)(&N), sizeof(N));
	for(int i=0;i<N;i++)
		file.write((char*)(h+i), sizeof(double));

	return true;
}

bool CFreqFirFilter::Loadfdd(const std::string &namestr)
{
	std::fstream file(namestr, std::fstream::in | std::fstream::binary);
	pathname=namestr;

	char szTmp[10];
	memset(szTmp, 0, 10 * sizeof(char));
	file.read(szTmp, 3 * sizeof(char)) ;
	if ((strncmp(szTmp, "FIR", 3) != 0)) 
	{
		return false;
	}
	char firmethed;  //1窗函数法  2频率采样法
	file.read(&firmethed, sizeof(firmethed)); assert(firmethed==2);
    file.read((char*)(&window), sizeof(window));
	file.read((char*)(&alpha_beta), sizeof(alpha_beta));
    file.read((char*)(&sample), sizeof(sample));

	file.read((char*)(&freq_af_n), sizeof(freq_af_n));

	if(norfreq!=NULL)
		delete []norfreq;
	norfreq=new double[freq_af_n];
	for(int i=0;i<freq_af_n;i++)
		file.read((char*)(norfreq+i), sizeof(double));

	if(af!=NULL)
		delete []af;
	af=new double[freq_af_n];
	for(int i=0;i<freq_af_n;i++)
		file.read((char*)(af+i), sizeof(double));

    file.read((char*)(&N), sizeof(N));
	if(h!=NULL)
		delete []h;
	h=new double[N];
	for(int i=0;i<N;i++)
		 file.read((char*)(h+i), sizeof(double));
	return true;
}

std::string CFreqFirFilter::GetInformationstr()
{
	
	std::ostringstream temp;

	if(language==Chinese)
	{
		temp<<"频率采样法\n";
		temp<<"归一化频率(0-1)\n";
	}
	else
	{
		temp<<"Freq Sample Method\n";
		temp<<"Normal Freq(0-1)\n";
	}

	temp.precision(4); //小数位数4位
	for(int i=0;i<freq_af_n;i++)
	{
		temp<<norfreq[i]<<" ";
	}
	temp<<"\n";

	if(language==Chinese)
		temp<<"频率点幅度\n";
	else
		temp<<"Margin of sample points\n";

	for(int i=0;i<freq_af_n;i++)
	{
		temp<<af[i]<<" ";
	}
	temp<<"\n";

	if(language==Chinese)
		temp<<"窗类型:"<<Window_Name[window]<<"\n"<<"长度"<<N<<"\n";	
	else
		temp<<"Window Type:"<<Window_Name[window]<<"\n"<<"Length "<<N<<"\n";	

	std::string Firhneidt=temp.str();
	return Firhneidt;
}

void CFreqFirFilter::Filter(int *date, size_t num)
{
	int m=(int)(log((double)num)/log(2.0));
	unsigned int FFT_LEN;
	if(pow((double)2.0,m)==num)
		FFT_LEN=(int)(pow((double)2.0,m));
	else
		FFT_LEN=(int)(pow((double)2.0,m+1));

	double *x=new double[FFT_LEN];

	for(size_t i=0;i<num;i++)
		x[i]=date[i];

	filters_overlap_add_fht(h, N, x, num, FILTER_FHT_N);

	for(size_t i=0;i<num;i++)
		date[i]=(int)(x[i]);

	delete []x;
}

void CFreqFirFilter::Filter(double *date, size_t num)
{
	int m=(int)(log((double)num)/log(2.0));
	unsigned int FFT_LEN;
	if(pow((double)2.0,m)==num)
		FFT_LEN=(int)(pow((double)2.0,m));
	else
		FFT_LEN=(int)(pow((double)2.0,m+1));

	double *x=new double[FFT_LEN];

	memcpy(x, date, num*sizeof(double));
	if(FFT_LEN!=num)
		memset(x+num, 0, (FFT_LEN-num)*sizeof(double));
	//for(size_t i=0;i<num;i++)
	//	x[i]=date[i];

	filters_overlap_add_fht(h, N, x, num, FILTER_FHT_N);

	memcpy(date, x, num*sizeof(double));
	//for(size_t i=0;i<num;i++)
	//	date[i]=(int)(x[i]);

	delete []x;
}
