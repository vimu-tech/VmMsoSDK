#include "WindowFirFilter.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cstring>
#include "fir1.h"
#include "fir2.h"
#include "vmfilters.h"
#include "vmsuanfa.h"

CWindowFirFilter::CWindowFirFilter(LanguageType language/*=English*/)
: CBasicFilter(language)
, band(LOW)
, window(KAISER)
, sample(1000)
, Firpinlv1(200)
, Firpinlv2(0)
, N(10)
, alpha_beta(2.120)
, h(NULL)//根据N求的hn
{
	type=FIR_WND;
}

CWindowFirFilter::~CWindowFirFilter(void)
{
	if(h!=NULL)
		delete []h;
}

CWindowFirFilter::CWindowFirFilter(const CWindowFirFilter &copy)
{
	band=copy.band;
	window=copy.window;
	alpha_beta = copy.alpha_beta;
	sample=copy.sample;
	Firpinlv1=copy.Firpinlv1;
	Firpinlv2=copy.Firpinlv2;
	//as=copy.as;
	N=copy.N;
	//if(h!=NULL)   //此处不能加，因为如果是按构造函数使用的话，h并没有初始化为NULL
		//delete []h;
	h = new double[N];
	memcpy(h, copy.h, (copy.N)*sizeof(double));
}

CWindowFirFilter& CWindowFirFilter::operator =(const CWindowFirFilter& copy)
{
	band=copy.band;
	window=copy.window;
	alpha_beta = copy.alpha_beta;
	sample=copy.sample;
	Firpinlv1=copy.Firpinlv1;
	Firpinlv2=copy.Firpinlv2;
	//as=copy.as;
	N=copy.N;
    if(h!=NULL)
		delete []h;
	h = new double[N];
	memcpy(h, copy.h, (copy.N)*sizeof(double));
    return *this;
}

void CWindowFirFilter::Clear()
{
	band=LOW;
    window=RECTANGLE; //窗类型
	sample=0; //采样率
	Firpinlv1=0;
	Firpinlv2=0;
	N=0;//窗的长度
    if(h!=NULL)
		delete []h;
	h=NULL;//根据N求的hn
}

int CWindowFirFilter::Cal_N()
{
	N=-1;

	/*double wp1=0,wp2=0,ws1=0,ws2=0;
	switch(band)
	{
	case LOW:
		wp1=(double)Firpinlv1*2.0/(double)sample;
		ws1=(double)Firpinlv3*2.0/(double)sample;
		break;
	case HIGH:
		wp1=(double)Firpinlv3*2.0/(double)sample;
		ws1=(double)Firpinlv1*2.0/(double)sample;
		break;
	case PASS:
		wp1=(double)Firpinlv1*2.0/(double)sample;
		wp2=(double)Firpinlv2*2.0/(double)sample;
		ws1=(double)Firpinlv3*2.0/(double)sample;
		ws2=(double)Firpinlv4*2.0/(double)sample;
		break;
	case STOP:
		wp1=(double)Firpinlv3*2.0/(double)sample;
		wp2=(double)Firpinlv4*2.0/(double)sample;
		ws1=(double)Firpinlv1*2.0/(double)sample;
		ws2=(double)Firpinlv2*2.0/(double)sample;
		break;
	};

	N=firord(band, wp1, wp2, ws1, ws2, as, window, &alpha_beta);*/
    
	return N;
}

int CWindowFirFilter::Cal_Window_N()
{
	N=-1;
	return N;
}

double CWindowFirFilter::Cal_Hz()
{
	if(N<=0)
	{
        //DebugString("设计滤波器的窗的长度不能为0");
		return -1;
	}

	if(h!=NULL)
		delete [] h;
	h = new double[N];
	if(h==NULL)
	{
        //DebugString("分配内存错误！");
		return -2;
	}

	char res;
	if((band==LOW)||(band==HIGH))
		res=fir(N, band, (double)(Firpinlv1)*2.0/(double)sample, 0, window, alpha_beta, h);
		//res=fir(N, band, (double)(Firpinlv1+Firpinlv3)/(double)sample, 0, window, alpha_beta, h); //(Firpinlv1+Firpinlv3)/2.0*2.0/sample
	else
		res=fir(N, band, (double)(Firpinlv1)*2.0/(double)sample, (double)(Firpinlv2)*2.0/(double)sample, window, alpha_beta, h);
		//res=fir(N, band, (double)(Firpinlv1+Firpinlv3)/(double)sample, (double)(Firpinlv2+Firpinlv4)/(double)sample, window, alpha_beta, h);

	if(!res)
	{
	     if(h!=NULL)
			delete [] h;
	     h=NULL;
         //DebugString("计算Hz出错！");
		 return -3;
	}

	return 1.0;
}

bool CWindowFirFilter::FupinDate(double* FM, int length)//2048
{
	double *Hrm=new double[length];
	double *Him=new double[length];

	fir_Hw(h, N, Hrm, Him, length);
	fir_Amplitude(Hrm, Him, FM, length, 0);  //只求一半

	delete []Hrm;
	delete []Him;
	return true;
}

bool CWindowFirFilter::FupindBDate(double* FMdB, int length)
{
	double *Hrm=new double[length];
	double *Him=new double[length];

	fir_Hw(h, N, Hrm, Him, length);
	fir_Amplitude(Hrm, Him, FMdB, length, 1);  //只求一半

	delete []Hrm;
	delete []Him;
	return true;
}

bool CWindowFirFilter::XiangpinDate(double* AM, int length)
{
	double *Hrm=new double[length];
	double *Him=new double[length];

	fir_Hw(h, N, Hrm, Him, length);

	//   计算大致不用考虑相位-2×pi的点
	//int nl, nr;
	
	if(band==LOW)
	{
		//nr = length*(Firpinlv1)/sample;
		fir_Phase(Hrm, Him, AM, length, 0, length);
	}
	else if(band ==HIGH )
	{
		//nl = length*(Firpinlv1)/sample;
		fir_Phase(Hrm, Him, AM, length, 0, length);
	}
	else if(band ==PASS )
	{
		//nl = length*(Firpinlv1)/sample;
		//nr = length*(Firpinlv2)/sample;
		fir_Phase(Hrm, Him, AM, length, 0, length);
	}
	else if(band ==STOP )
	{
		//nl = length*(Firpinlv1)/sample;
		//nr = length*(Firpinlv2)/sample;
		fir_Phase(Hrm, Him, AM, length, 0, length);
		//fir_Phase_Daizu(Hrm, Him, AM, length, nl, nr);
	}

	delete []Hrm;
	delete []Him;

	return true;
}

bool CWindowFirFilter::JieyueDate(double *jy, size_t *length)
{
	fir_resp(N, h, jy);
    *length=N;
	return true;
}
//---------求得的Hz的系数就是
bool CWindowFirFilter::MaichongDate(double *mch, size_t *length)
{
	memcpy(mch, h, N*sizeof(double));
	*length=N;
	return true;
}
//群时延又称为包络时延，信号传输时，总相移随角频率而变化的速度，亦即相位一频率特性曲线的斜率。
//t(w)＝dB(w)/dw………即为传输系统的群时延。  B(w)为相位一频率特性
bool CWindowFirFilter::QunshiyanDate(double* qsy, int length)
{
	double *Hrm=new double[2*length];
	double *Him=new double[2*length];

	double dw=(double)sample/2.0/length;
	//double dw=4.0*atan(1.0)/H_N; 

	fir_Hw(h, N, Hrm, Him, 2*length);

	double value = 0;
	int i = 0;
	int nl, nr;
	if(band==LOW)
	{
		nr = length*(Firpinlv1)/sample;
		fir_Phase(Hrm, Him, qsy, length, 0, length);
		i=nr/2;
		value=(qsy[i+2]-qsy[i-2])/4/dw;  //value=(int)((qsy[i+2]-qsy[i-2])/4/dw*100000);
	}
	else if(band==HIGH )
	{
		nl = length*(Firpinlv1)/sample;
		fir_Phase(Hrm, Him, qsy, length, 0, length);
		i=(length-nl)/2+nl;
		value=(qsy[i+2]-qsy[i-2])/4/dw;  //value=(int)((qsy[i+2]-qsy[i-2])*100000/4/dw);
	}
	else if(band==PASS )
	{
		nl = length*Firpinlv1/sample;
		nr = length*Firpinlv2/sample;
		fir_Phase(Hrm, Him, qsy, length, 0, length);
		i=(nr-nl)/2+nl;
		value=(qsy[i+2]-qsy[i-2])/4/dw; //value=(int)((qsy[i+2]-qsy[i-2])*100000/4/dw);
	}
	else if(band==STOP )
	{
		nl = length*Firpinlv1/sample;
		nr = length*Firpinlv2/sample;
		fir_Phase(Hrm, Him, qsy, length, 0, length);
		//fir_Phase_Daizu(Hrm, Him, qsy, length, 0, length);
		i=nl/2;
		value=(qsy[i+2]-qsy[i-2])/4/dw; //value=(int)((qsy[i+2]-qsy[i-2])*100000/4/dw); 
	}

	for(int p=0;p<length;p++)
          qsy[p]=value;

	delete []Hrm;
	delete []Him;
	return true;
}

int CWindowFirFilter::GetZerosN()
{
	//将z^n次这一项如果是0的话去掉,否则调用qrrt（）失败
	 int zeronum=0;
	 for(int i=0;i<N/2;i++)
	 {
		if(fabs(*((h)+N-1-i))<0.000000001)
			zeronum++;
		else 
			break;
	 }
	 return N-zeronum-1;
}

int CWindowFirFilter::GetPolesN()
{
	return N-1;
}

int CWindowFirFilter::ZerosDate(double *re, double *im)
{
	 //将hn的顺颠倒，以便满足qrrt的顺序,但是由于hn的对称性我们根本不需要颠倒
	 /*double temp;
	 for(int i=0;i<windowlength/2;i++)
	 {
		 temp=*((m_firstruct.h)+i);
         *((m_firstruct.h)+i)= *((m_firstruct.h)+windowlength-1-i);
         *((m_firstruct.h)+windowlength-1-i)=temp;
	 }*/
	 //将z^n次这一项如果是0的话去掉,否则调用qrrt（）失败
	 int zeronum=0;
	 for(int i=0;i<N/2;i++)
	 {
		if(fabs(*((h)+N-1-i))<0.000000001)
			zeronum++;
		else 
			break;
	 }

	 if(qrrt(N-zeronum-1, h, re, im, 0.00001, 100)<0)
		 return -1;
	return N-zeronum-1;
}

int CWindowFirFilter::PolesDate(double *re, double *im)
{
	//存放极点FIR有N-1阶重极点0
    for(int i=0;i<N-1;i++)
	{
		re[i]=0.0;
		im[i]=0.0;
	}
	return N-1;
}

bool CWindowFirFilter::SaveHz(const std::string &namestr, int format0, int format1, char fe, int xishu)
{
	std::fstream file(namestr, std::fstream::out);
	pathname=namestr;

	//更新hn显示去的数据
	file<<GetCompanyInfo();

	if(language==Chinese)
	{
		file<<"/*--------------------------------说明-----------------------------------\n";
		file<<"   H(z)的给出形式为\n";
		file<<"                        H(z)=b(0)+b(1)z^(-1)+ ... + b(n)z^(-n)\n";
		file<<"*/\n";

		file<<"/*\n窗函数法\n";
		switch(band)
		{
		case LOW:
			file<<"低通\n采样率 "<<sample<<"\n通带截止频率 "<<Firpinlv1<<"\n";
			break;
		case HIGH:
			file<<"高通\n采样率 "<<sample<<"\n通带上限频率 "<<Firpinlv1<<"\n";
			break;
		case PASS:
			file<<"带通\n采样率 "<<sample<<"\n带通下限频率 "<<Firpinlv1<<"  带通上限频率 "<<Firpinlv2<<"\n";
			break;
		case STOP:
			file<<"带阻\n采样率 "<<sample<<"\n阻带下限频率 "<<Firpinlv1<<"  阻带上限频率 "<<Firpinlv2<<"\n";
			break;
		}

		file<<"窗类型:"<<Window_Name[window]<<"\n";
		if(WINDOW_IS_NEED_PARA(window))
		{
			file<<"窗参数:"<<alpha_beta<<"\n";
		}
		file<<"长度 "<<N<<"\n";

		file<<"乘以系数:"<<xishu<<"\n*/\n\n";
	}
	else
	{
		file<<"/*----------------------------Information-----------------------------------\n";
		file<<"                        H(z)=b(0)+b(1)z^(-1)+ ... + b(n)z^(-n)\n";
		file<<"*/\n";

		file<<"/*\nWindow Method\n";
		switch(band)
		{
		case LOW:
			file<<"Lowpass\nSample "<<sample<<"\nPass "<<Firpinlv1<<"\n";
			break;
		case HIGH:
			file<<"Highpass\nSample "<<sample<<"\nPass "<<Firpinlv1<<"\n";
			break;
		case PASS:
			file<<"Bandpass\nSample "<<sample<<"\nPass "<<Firpinlv1<<"\nPass "<<Firpinlv2<<"\n";
			break;
		case STOP:
			file<<"Bandstop\nSample "<<sample<<"\nStop "<<Firpinlv1<<"\nStop "<<Firpinlv2<<"\n";
			break;
		}

		file<<"Window Type:"<<Window_Name[window]<<"\n";
		if(WINDOW_IS_NEED_PARA(window))
		{
			file<<"Window Para:"<<alpha_beta<<"\n";
		}
		file<<"Length "<<N<<"\n";

		file<<"Multiplied Coefficient:"<<xishu<<"\n*/\n\n";
	}

	file<<"int h["<<N<<"]={\n              ";
	char temp[50];
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

bool CWindowFirFilter::SaveZorePoles(const std::string &namestr, int format0, int format1, char fe, int xishu)
{
	std::fstream file(namestr, std::fstream::out);
	pathname=namestr;

	//更新hn显示去的数据
	file<<GetCompanyInfo();

	if(language==Chinese)
	{
		file<<"/*---------------------------------零极点形式----------------------------------*/\n";

		file<<"/*\n窗函数法\n";
		switch(band)
		{
		case LOW:
			file<<"低通\n采样率 "<<sample<<"\n通带截止频率 "<<Firpinlv1<<"\n";
			break;
		case HIGH:
			file<<"高通\n采样率 "<<sample<<"\n通带上限频率 "<<Firpinlv1<<"\n";
			break;
		case PASS:
			file<<"带通\n采样率 "<<sample<<"\n带通下限频率 "<<Firpinlv1<<"  带通上限频率 "<<Firpinlv2<<"\n";
			break;
		case STOP:
			file<<"带阻\n采样率 "<<sample<<"\n阻带下限频率 "<<Firpinlv1<<"  阻带上限频率 "<<Firpinlv2<<"\n";
			break;
		}

		file<<"窗类型:"<<Window_Name[window]<<"\n";
		if(WINDOW_IS_NEED_PARA(window))
		{
			file<<"窗参数:"<<alpha_beta<<"\n";
		}
		file<<"长度 "<<N<<"\n";

		file<<"乘以系数:"<<xishu<<"\n*/\n\n";
		file<<"/*---------------------------------零点----------------------------------*/\n";
	}
	else
	{
		file<<"/*---------------------------------Zero-Pole----------------------------------*/\n";

		file<<"/*\nWindow Method\n";
		switch(band)
		{
		case LOW:
			file<<"Lowpass\nSample "<<sample<<"\nPass "<<Firpinlv1<<"\n";
			break;
		case HIGH:
			file<<"Highpass\nSample "<<sample<<"\nPass "<<Firpinlv1<<"\n";
			break;
		case PASS:
			file<<"Bandpass\nSample "<<sample<<"\nPass "<<Firpinlv1<<"\nPass "<<Firpinlv2<<"\n";
			break;
		case STOP:
			file<<"Bandstop\nSample "<<sample<<"\nStop "<<Firpinlv1<<"\nStop "<<Firpinlv2<<"\n";
			break;
		}

		file<<"Window Type:"<<Window_Name[window]<<"\n";
		if(WINDOW_IS_NEED_PARA(window))
		{
			file<<"Window Para:"<<alpha_beta<<"\n";
		}
		file<<"Length "<<N<<"\n";

		file<<"Multiplied Coefficient:"<<xishu<<"\n*/\n\n";
		file<<"/*---------------------------------Zeros---------------------------------*/\n";
	}

	int zeronum=GetZerosN();
	double *re=new double[zeronum];
	double *im=new double[zeronum];
	ZerosDate(re, im);

	char temp[60];
	if(fe=='f')
	{
    	for(int i=0;i<zeronum;i++)
     	{
			sprintf(temp,"\t%*.*f+i%*.*f\n",format0, format1, re[i]*xishu, format0, format1, im[i]*xishu);
			file<<temp;
	    }
	}
	else
	{
		for(int i=0;i<zeronum;i++)
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

bool CWindowFirFilter::Savefdd(const std::string &namestr)
{
	std::fstream file(namestr, std::fstream::out | std::fstream::binary);
	pathname=namestr;

	file.write("FIR", 3);
	char firmethed=1;  //1窗函数法  2频率采样法
	file.write(&firmethed, sizeof(firmethed));
	file.write((char*)(&band), sizeof(band));
	file.write((char*)(&window), sizeof(window));
	file.write((char*)(&alpha_beta), sizeof(alpha_beta));
	file.write((char*)(&sample), sizeof(sample));
	file.write((char*)(&Firpinlv1), sizeof(Firpinlv1));
	file.write((char*)(&Firpinlv2), sizeof(Firpinlv2));
	file.write((char*)(&N), sizeof(N));

	for(int i=0;i<N;i++)
		file.write((char*)(h+i), sizeof(double));

	return true;
}

bool CWindowFirFilter::Loadfdd(const std::string &namestr)
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
	file.read(&firmethed, sizeof(firmethed)); assert(firmethed==1);
	file.read((char*)(&band), sizeof(band));
    file.read((char*)(&window), sizeof(window));
	file.read((char*)(&alpha_beta), sizeof(alpha_beta));
    file.read((char*)(&sample), sizeof(sample));
    file.read((char*)(&Firpinlv1), sizeof(Firpinlv1));
	file.read((char*)(&Firpinlv2), sizeof(Firpinlv2));
    file.read((char*)(&N), sizeof(N));

	if(h!=NULL)
		delete []h;
	h=new double[N];

	for(int i=0;i<N;i++)
		 file.read((char*)(h+i), sizeof(double));
	return true;
}

std::string CWindowFirFilter::GetInformationstr()
{
	std::ostringstream temp;

	if(language==Chinese)
	{
		temp<<"窗函数法\n";
		switch(band)
		{
		case LOW:
			temp<<"低通\n采样率:"<<sample<<"\n通带截止频率 "<<Firpinlv1<<"\n";
			break;
		case HIGH:
			temp<<"高通\n采样率:"<<sample<<"\n阻带下限频率 "<<Firpinlv1<<"\n";
			break;
		case PASS:
			temp<<"带通\n采样率:"<<sample<<"\n带通下限频率 "<<Firpinlv1<<"\n带通上限频率 "<<Firpinlv2<<"\n";
			break;
		case STOP:
			temp<<"带阻\n采样率:"<<sample<<"\n阻带下限频率 "<<Firpinlv1<<"\n阻带上限频率 "<<Firpinlv2<<"\n";
			break;
		}

		temp<<"窗类型:"<<Window_Name[window]<<"\n";
		if(WINDOW_IS_NEED_PARA(window))
		{
			temp<<"窗参数:"<<alpha_beta<<"\n";
		}
		temp<<"长度 "<<N<<"\n";
	}
	else
	{
		temp<<"Window Method\n";
		switch(band)
		{
		case LOW:
			temp<<"Lowpass\nSample:"<<sample<<"\nPass "<<Firpinlv1<<"\n";
			break;
		case HIGH:
			temp<<"Highpass\nSample:"<<sample<<"\nPass "<<Firpinlv1<<"\n";
			break;
		case PASS:
			temp<<"Bandpass\nSample:"<<sample<<"\nPass "<<Firpinlv1<<"\nPass "<<Firpinlv2<<"\n";
			break;
		case STOP:
			temp<<"Bandstop\nSample:"<<sample<<"\nStop "<<Firpinlv1<<"\nStop "<<Firpinlv2<<"\n";
			break;
		}

		temp<<"Window Type:"<<Window_Name[window]<<"\n";
		if(WINDOW_IS_NEED_PARA(window))
		{
			temp<<"Window Para:"<<alpha_beta<<"\n";
		}
		temp<<"Length "<<N<<"\n";
	}

	std::string Firhneidt=temp.str();
	return Firhneidt;
}


void CWindowFirFilter::SetBand(BANDTYPE nband)
{
	band=nband;
	switch(band)
	{
	case LOW:
		Firpinlv1=sample/5;
		Firpinlv2=0;
		N=10;
		break;
	case HIGH:
		Firpinlv1=sample*3/10;
		Firpinlv2=0;
		N=11;
		break;
	case PASS:
		Firpinlv1=sample/5;
		Firpinlv2=sample*2/5;
		N=10;
		break;
	case STOP:
		Firpinlv1=sample/5;
		Firpinlv2=sample*2/5;
		N=11;
		break;
	};
}

void CWindowFirFilter::Filter(int *date, size_t num)
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

void CWindowFirFilter::Filter(double *date, size_t num)
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
