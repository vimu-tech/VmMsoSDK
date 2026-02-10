#include "IirFilter.h"
#include <math.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "bilinear.h"
#include "buttap.h"
#include "butter.h"
#include "buttord.h"
#include "cheb1ord.h"
#include "cheb2ap.h"
#include "cheb2ord.h"
#include "cheby1.h"
#include "cheby2.h"
#include "ellip.h"
#include "ellipap.h"
#include "ellipord.h"
#include "sftrans.h"
#include "vmfreqz.h"
#include "vmresp.h"
#include "vmfilters.h"

CIirFilter::CIirFilter(LanguageType language/*=English*/)
	: CBasicFilter(language)
	, band(LOW)
	, iirstyle(0)
	, sample(1000)
	, Iirpinlv1(200)
	, Iirpinlv2(0)
	, Iirpinlv3(400)
	, Iirpinlv4(0)
	, ap(3)
	, as(15)
	, N(Cal_N())
	, wn1(0)
	, wn2(0)
	, g(0)
{
	
}

CIirFilter::~CIirFilter(void)
{
}

CIirFilter::CIirFilter(const CIirFilter &copy)
{
	band=copy.band;
	//bianhuanstyle=copy.bianhuanstyle;
	iirstyle=copy.iirstyle; 
	sample=copy.sample;
	Iirpinlv1=copy.Iirpinlv1;
	Iirpinlv2=copy.Iirpinlv2;
	Iirpinlv3=copy.Iirpinlv3;
	Iirpinlv4=copy.Iirpinlv4;
	ap=copy.ap;
	as=copy.as;
	N=copy.N;
	wn1=copy.wn1;
	wn2=copy.wn2;
	z=copy.z;
	p=copy.p;
	g=copy.g;
}

CIirFilter& CIirFilter::operator =(const CIirFilter& copy)
{
	band=copy.band;
	iirstyle=copy.iirstyle; 
	sample=copy.sample;
	Iirpinlv1=copy.Iirpinlv1;
	Iirpinlv2=copy.Iirpinlv2;
	Iirpinlv3=copy.Iirpinlv3;
	Iirpinlv4=copy.Iirpinlv4;
	ap=copy.ap;
	as=copy.as;
	N=copy.N;
	wn1=copy.wn1;
	wn2=copy.wn2;
	z=copy.z;
	p=copy.p;
	g=copy.g;

	return *this;
}
void CIirFilter::Clear()
{
	band=LOW;
	//bianhuanstyle=0;
    iirstyle=0; 
    sample=0; //采样率
    Iirpinlv1=0;
    Iirpinlv2=0;
    Iirpinlv3=0;
    Iirpinlv4=0;
	ap=0;
	as=0;
    N=0;//窗的长度
	wn1=0;
	wn2=0;
	z.clear();
	p.clear();
	g=0;
}

int CIirFilter::Cal_N()
{
	N=-1;

	//检测参数 
	if(Iirpinlv1>=sample/2)
	{
        //DebugString("设计滤波器的频率应小于采样率的1/2");
		return -1;
	}
	if(Iirpinlv2>=sample/2)
	{
        //DebugString("设计滤波器的频率应小于采样率的1/2");
		return -1;
	}
	if(Iirpinlv3>=sample/2)
	{
        //DebugString("设计滤波器的频率应小于采样率的1/2");
		return -1;
	}
	if(Iirpinlv4>=sample/2)
	{
        //DebugString("设计滤波器的频率应小于采样率的1/2");
		return -1;
	}

	double wp1=0,wp2=0,ws1=0,ws2=0;
	switch(band)
	{
	case LOW:
		wp1=Iirpinlv1;
		ws1=Iirpinlv3;
		if(wp1>=ws1)
		{
            //DebugString("阻带截止频率应大于通带截止频率");
		    return -1;
		}
		break;
	case HIGH:
		wp1=Iirpinlv3;
		ws1=Iirpinlv1;
		if(wp1<=ws1)
		{
            //DebugString("阻带下限频率应小于通带上限频率");
		    return -1;
		}
		break;
	case PASS:
		wp1=Iirpinlv1;
		wp2=Iirpinlv2;
		ws1=Iirpinlv3;
		ws2=Iirpinlv4;
		if(wp1>=wp2)
	    {
            //DebugString("带通上限频率应该大于带通下限频率");
			return -1;
	    }
		if(ws1>=ws2)
		{
            //DebugString("上阻带截止频率应大于下阻带截止频率");
		    return -1;
	    }
		if(ws1>=wp1) 
		{
            //DebugString("下阻带截止频率应小于带通下限频率");
		    return -1;
		}
		if(ws2<=wp2) 
		{
            //DebugString("上阻带截止频率应大于带通上限频率");
		    return -1;
		}
		break;
	case STOP:
		wp1=Iirpinlv3;
		wp2=Iirpinlv4;
		ws1=Iirpinlv1;
		ws2=Iirpinlv2;
		if(ws1>=ws2)
	    {
           //DebugString("阻带上限频率应该大于阻带下限频率");
		   return -1;
		}
		if(wp2<=wp1)
		{
            //DebugString("上通带截止频率应大于下通带截止频率");
		    return -1;
		}
		if(wp1>=ws1) 
		{
            //DebugString("下通带截止频率应小于阻带下限频率");
		    return -1;
		}
		if(wp2<=ws2) 
		{
            //DebugString("上通带截止频率应大于阻带上限频率");
		    return -1;
		}
		break;
	};
	
	//归一化
	//double pi=4.0*atan(1.0);
	wp1=wp1*2/sample;
	wp2=wp2*2/sample;
	ws1=ws1*2/sample;
	ws2=ws2*2/sample;

	if(iirstyle==0)
		N=buttord(band,wp1,wp2,ws1,ws2,ap,as,'z',wn1,wn2);
	else if(iirstyle==1)
		N=cheb1ord(band,wp1,wp2,ws1,ws2,ap,as,'z',wn1,wn2);
	else if(iirstyle==2)
		N=cheb2ord(band,wp1,wp2,ws1,ws2,ap,as,'z',wn1,wn2);
	else if(iirstyle==3)
	{
		N=ellipord(band,wp1,wp2,ws1,ws2,ap,as,'z',wn1,wn2);
        //if(N==-2)
           //DebugString("椭圆滤波器最大衰减支持160dB");
	}
	N=((band==PASS)||(band==STOP))? N*2 : N;
	return N;
}

double CIirFilter::Cal_Hz()
{
	if(N<=0)
	{
        //DebugString("设计滤波器的阶数不能为0");
		return -1;
	}

	z.clear();
	p.clear();

	if(iirstyle==0)
		butter(N, wn1, wn2, band, 'z', z, p, g);
	else if(iirstyle==1)
		cheby1(N, wn1, wn2, ap, band, 'z', z, p, g);
    else if(iirstyle==2)
		cheby2(N, wn1, wn2, as, band, 'z', z, p, g);
    else if(iirstyle==3)
		ellip(N, wn1, wn2, ap, as, band, 'z', z, p, g);

	//计算分子分母
	b.clear();
	a.clear();
	zp2tfz(z, p, g, b, a);

	/*TRACE("%d  %f  %f\n",N, wn1,wn2);

	for(size_t i=0;i<z.size();i++)
		TRACE("%f+i%f\n",z[i].real(),z[i].imag());

    TRACE("\n");
	for(size_t i=0;i<p.size();i++)
		TRACE("%f+i%f\n",p[i].real(),p[i].imag());

	TRACE("g=%f\n",g);

	std::vector<double> b,a;
	zp2tfz(z, p, g, b, a);

	TRACE("b\n");
	for(size_t i=0;i<b.size();i++)
		TRACE("%f\n",b[i]);
	TRACE("a\n");
	for(size_t i=0;i<a.size();i++)
		TRACE("%f\n",a[i]);*/

	return g;
}

bool CIirFilter::FupinDate(double* x, int length)
{
	/*TRACE("b\n");
	for(size_t i=0;i<b.size();i++)
		TRACE("%f+i%f\n",b[i].real(),b[i].imag());
	TRACE("a\n");
	for(size_t i=0;i<a.size();i++)
		TRACE("%f+i%f\n",a[i].real(),a[i].imag());*/

	std::vector<double> res = freqz(b, a, length, 0);
	//std::vector<double> res1 = freqz(z, p, g, length, 0);

	//for(unsigned int i=0;i<100;i++)
		//TRACE("%f  %f\n",res[i*4],res1[i*4]);

	for(int i=0;i<length;i++)
		x[i]=res[i]; 
	return true;
}

bool CIirFilter::FupindBDate(double* x, int length)
{
	std::vector<double> res = freqz(b, a, length, 1);

	for(int i=0;i<length;i++)
		x[i]=res[i]; 
	return true;
}

bool CIirFilter::XiangpinDate(double* x, int length)
{
	std::vector<double> res = freqz(b, a, length, 2);

	for(int i=0;i<length;i++)
		x[i]=res[i]; 
	return true;
}

bool CIirFilter::JieyueDate(double *jy, size_t *length)
{
	*length=20*(b.size()+z.size()-1);
	std::vector<double> y = resp(b, a, *length, 0);

	for(size_t i=0;i<*length;i++)
		jy[i]=y[i]; 
	return true;
}

bool CIirFilter::MaichongDate(double *mch, size_t *length)
{
	*length=20*(b.size()+z.size()-1);
	std::vector<double> y = resp(b, a, *length, 1);

	for(size_t i=0;i<*length;i++)
		mch[i]=y[i]; 
	return true;
}

bool CIirFilter::QunshiyanDate(double* qsy, int length)
{
	std::vector<double> res = freqz(b, a, length, 2);

    double dw=(double)sample/2.0/length; 

	for(int i=2;i<length;i++) 
        qsy[i]=(res[i]-res[i-1])/dw;
	
	qsy[0]=qsy[1]=qsy[2];

	return true;
}

int CIirFilter::ZerosDate(double *re, double *im)
{
	for(size_t i=0;i<z.size();i++)
	{
		re[i]=z[i].real();
		im[i]=z[i].imag();
	}

	return z.size();
}

int CIirFilter::PolesDate(double *re, double *im)
{
	for(size_t i=0;i<p.size();i++)
	{
		re[i]=p[i].real();
		im[i]=p[i].imag();
	}
	return p.size();
}

bool CIirFilter::SaveHz(const std::string &namestr, int format0, int format1, char fe, int xishu)
{
	std::fstream file(namestr, std::fstream::out);
	pathname=namestr;

	//更新hn显示去的数据
	file<<GetCompanyInfo();

	if(language==Chinese)
	{
		file<<"/*--------------------------------说明-----------------------------------\n";
		file<<"   H(z)的给出形式为\n";
	}
	else
	{
		file<<"/*--------------------------------Information-----------------------------------\n";
	}
	file<<"                        b(0)+b(1)z^(-1)+ ... + b(n)z^(-n)\n";
	file<<"                 H(z)=------------------------------------------\n";
	file<<"                        a(0)+a(1)z^(-1)+ ... + a(n)z^(-n)\n";
    file<<"*/\n";

	if(language==Chinese)
	{
		switch(band)
		{
		case LOW:
			file<<"/*\n低通\n采样率 "<<sample<<"\n通带截止频率 "<<Iirpinlv1<<"\n阻带截止频率 "<<Iirpinlv3<<"\n";
			break;
		case HIGH:
			file<<"/*\n高通\n采样率 "<<sample<<"\n阻带下限频率 "<<Iirpinlv1<<"\n通带上限频率 "<<Iirpinlv3<<"\n";
			break;
		case PASS:
			file<<"/*\n带通\n采样率 "<<sample<<"\n带通下限频率 "<<Iirpinlv1<<"\n带通上限频率 "<<Iirpinlv2<<"\n下阻带截止频率 "<<Iirpinlv3<<"\n上阻带截止频率 "<<Iirpinlv4<<"\n";
			break;
		case STOP:
			file<<"/*\n带阻\n采样率 "<<sample<<"\n阻带下限频率 "<<Iirpinlv1<<"\n阻带上限频率 "<<Iirpinlv2<<"\n下通带截止频率 "<<Iirpinlv3<<"\n上通带截止频率 "<<Iirpinlv4<<"\n";
			break;
		}
		file<<"通带最大衰减"<<ap<<"\n阻带最小衰减"<<as<<"\n";

		switch(iirstyle)
		{
		case 0:
			file<<"巴特沃斯型\n阶数"<<N<<"\n";
			break;
		case 1:
			file<<"切比雪夫I型\n阶数"<<N<<"\n";
			break;
		case 2:
			file<<"切比雪夫II型\n阶数"<<N<<"\n";
			break;
		case 3:
			file<<"椭圆型\n阶数"<<N<<"\n";
			break;
		}
	}
	else
	{
		switch(band)
		{
		case LOW:
			file<<"/*\nLowpass\nSample "<<sample<<"\nPass "<<Iirpinlv1<<"\nStop "<<Iirpinlv3<<"\n";
			break;
		case HIGH:
			file<<"/*\nHighpass\nSample "<<sample<<"\nStop "<<Iirpinlv1<<"\nPass "<<Iirpinlv3<<"\n";
			break;
		case PASS:
			file<<"/*\nBandpass\nSample "<<sample<<"\nPass "<<Iirpinlv1<<"\nPass "<<Iirpinlv2<<"\nStop "<<Iirpinlv3<<"\nStop "<<Iirpinlv4<<"\n";
			break;
		case STOP:
			file<<"/*\nBandstop\nSample "<<sample<<"\nStop "<<Iirpinlv1<<"\nStop "<<Iirpinlv2<<"\nPass "<<Iirpinlv3<<"\nPass "<<Iirpinlv4<<"\n";
			break;
		}
		file<<"Ap "<<ap<<"\nAs "<<as<<"\n";

		switch(iirstyle)
		{
		case 0:
			file<<"Butterworth\nOrder "<<N<<"\n";
			break;
		case 1:
			file<<"Chebyshev Type I\nOrder "<<N<<"\n";
			break;
		case 2:
			file<<"Chebyshev Type II\nOrder "<<N<<"\n";
			break;
		case 3:
			file<<"elliptic\nOrder "<<N<<"\n";
			break;
		}
	}

	char temp[50];
	if(fe=='f')
	{
		if(language==Chinese)
			file<<"乘以系数:"<<xishu<<"\n*/\n\nint b["<<b.size()<<"]={\n              ";
		else
			file<<"Multiplied Coefficient:"<<xishu<<"\n*/\n\nint b["<<b.size()<<"]={\n              ";

	    for(size_t i=0;i<b.size();i++)
	    {
		    if(i==b.size()-1)
                sprintf(temp,"%*.*f",format0, format1, b[i]*xishu);
	        else
	   	     	sprintf(temp,"%*.*f, ",format0, format1, b[i]*xishu);
	     	file<<temp;
	     	if(i>0 && i!=(b.size()-1) &&(i+1)%5==0)
		            file<<"\n              ";
    	}
        file<<"\n              };";

	    file<<"\nint a["<<a.size()<<"]={\n              ";
	    for(size_t i=0;i<a.size();i++)
	    {
	    	if(i==a.size()-1)
                sprintf(temp,"%*.*f",format0, format1, a[i]*xishu);
	        else
	    		sprintf(temp,"%*.*f, ",format0, format1, a[i]*xishu);
     		file<<temp;
	    	if(i>0 && i!=(a.size()-1) &&(i+1)%5==0)
	    	        file<<"\n              ";
	    }
    	file<<"\n              };";
	}
	else
	{
		if(language==Chinese)
			file<<"乘以系数:"<<xishu<<"\n*/\n\nint b["<<b.size()<<"]={\n              ";
		else
			file<<"Multiplied Coefficient:"<<xishu<<"\n*/\n\nint b["<<b.size()<<"]={\n              ";

	    for(size_t i=0;i<b.size();i++)
	    {
		    if(i==b.size()-1)
                sprintf(temp,"%*.*e",format0, format1, b[i]*xishu);
	        else
	   	     	sprintf(temp,"%*.*e, ",format0, format1, b[i]*xishu);
	     	file<<temp;
	     	if(i>0 && i!=(b.size()-1) &&(i+1)%5==0)
		            file<<"\n              ";
    	}
        file<<"\n              };";

	    file<<"\nint a[%d]={\n              ";
	    for(size_t i=0;i<a.size();i++)
	    {
	    	if(i==a.size()-1)
                sprintf(temp,"%*.*e",format0, format1, a[i]*xishu);
	        else
	    		sprintf(temp,"%*.*e, ",format0, format1, a[i]*xishu);
     		file<<temp;
	    	if(i>0 && i!=(a.size()-1) &&(i+1)%5==0)
	    	        file<<"\n              ";
	    }
    	file<<"\n              };";
	}
	
	file.close();
	file.clear();
	return true;
}

bool CIirFilter::SaveZorePoles(const std::string &namestr, int format0, int format1, char fe, int xishu)
{
	std::fstream file(namestr, std::fstream::out);
	pathname=namestr;

	//更新hn显示去的数据
	file<<GetCompanyInfo();

	if(language==Chinese)
	{
		file<<"/*---------------------------------零极点形式----------------------------------*/\n";
		switch(band)
		{
		case LOW:
			file<<"/*\n低通\n采样率 "<<sample<<"\n通带截止频率 "<<Iirpinlv1<<"\n阻带截止频率 "<<Iirpinlv3<<"\n";
			break;
		case HIGH:
			file<<"/*\n高通\n采样率 "<<sample<<"\n阻带下限频率 "<<Iirpinlv1<<"\n通带上限频率 "<<Iirpinlv3<<"\n";
			break;
		case PASS:
			file<<"/*\n带通\n采样率 "<<sample<<"\n带通下限频率 "<<Iirpinlv1<<"\n带通上限频率 "<<Iirpinlv2<<"\n下阻带截止频率 "<<Iirpinlv3<<"\n上阻带截止频率 "<<Iirpinlv4<<"\n";
			break;
		case STOP:
			file<<"/*\n带阻\n采样率 "<<sample<<"\n阻带下限频率 "<<Iirpinlv1<<"\n阻带上限频率 "<<Iirpinlv2<<"\n下通带截止频率 "<<Iirpinlv3<<"\n上通带截止频率 "<<Iirpinlv4<<"\n";
			break;
		}
		file<<"通带最大衰减"<<ap<<"\n阻带最小衰减"<<as<<"\n";

		switch(iirstyle)
		{
		case 0:
			file<<"巴特沃斯型\n阶数"<<N<<"\n";
			break;
		case 1:
			file<<"切比雪夫I型\n阶数"<<N<<"\n";
			break;
		case 2:
			file<<"切比雪夫II型\n阶数"<<N<<"\n";
			break;
		case 3:
			file<<"椭圆型\n阶数"<<N<<"\n";
			break;
		}
	}
	else
	{
		file<<"/*---------------------------------Zero-Pole----------------------------------*/\n";
		switch(band)
		{
		case LOW:
			file<<"/*\nLowpass\nSample "<<sample<<"\nPass "<<Iirpinlv1<<"\nStop "<<Iirpinlv3<<"\n";
			break;
		case HIGH:
			file<<"/*\nHighpass\nSample "<<sample<<"\nStop "<<Iirpinlv1<<"\nPass "<<Iirpinlv3<<"\n";
			break;
		case PASS:
			file<<"/*\nBandpass\nSample "<<sample<<"\nPass "<<Iirpinlv1<<"\nPass "<<Iirpinlv2<<"\nStop "<<Iirpinlv3<<"\nStop "<<Iirpinlv4<<"\n";
			break;
		case STOP:
			file<<"/*\nBandstop\nSample "<<sample<<"\nStop "<<Iirpinlv1<<"\nStop "<<Iirpinlv2<<"\nPass "<<Iirpinlv3<<"\nPass "<<Iirpinlv4<<"\n";
			break;
		}
		file<<"Ap "<<ap<<"\nAs "<<as<<"\n";

		switch(iirstyle)
		{
		case 0:
			file<<"Butterworth\nOrder "<<N<<"\n";
			break;
		case 1:
			file<<"Chebyshev Type I\nOrder "<<N<<"\n";
			break;
		case 2:
			file<<"Chebyshev Type II\nOrder "<<N<<"\n";
			break;
		case 3:
			file<<"elliptic\nOrder "<<N<<"\n";
			break;
		}
	}

	if(language==Chinese)
	{
		file<<"乘以系数:"<<xishu<<"\n*/\n\n";
		file<<"/*---------------------------------零点----------------------------------*/\n";
	}
	else
	{
		file<<"Multiplied Coefficient:"<<xishu<<"\n*/\n\n";
		file<<"/*---------------------------------Zeros----------------------------------*/\n";
	}

	char temp[60];
	if(fe=='f')
	{
    	for(size_t i=0;i<z.size();i++)
     	{
            sprintf(temp,"\t%*.*f+i%*.*f\n", format0, format1, z[i].real()*xishu, format0, format1, z[i].imag()*xishu);
		    file<<temp;
	    }
	}
	else
	{
		for(size_t i=0;i<z.size();i++)
     	{
		    sprintf(temp,"\t%*.*e+i%*.*e\n", format0, format1, z[i].real()*xishu, format0, format1, z[i].imag()*xishu);
		    file<<temp;
	    }
	}
	if(language==Chinese)
		file<<"\n/*---------------------------------极点----------------------------------*/\n";
	else
		file<<"\n/*---------------------------------Poles----------------------------------*/\n";

	if(fe=='f')
	{
    	for(size_t i=0;i<p.size();i++)
     	{
            sprintf(temp,"\t%*.*f+i%*.*f\n", format0, format1, p[i].real()*xishu, format0, format1, p[i].imag()*xishu);
		    file<<temp;
	    }
	}
	else
	{
		for(size_t i=0;i<p.size();i++)
     	{
		    sprintf(temp,"\t%*.*e+i%*.*e\n", format0, format1, p[i].real()*xishu, format0, format1, p[i].imag()*xishu);
		    file<<temp;
	    }
	}

	file.close();
	file.clear();
	return true;
}

bool CIirFilter::Savefdd(const std::string &namestr)
{
	std::fstream file(namestr, std::fstream::out | std::fstream::binary);
	pathname=namestr;

	file.write("IIR", 3);
	char bianhuanstyle=1; //变换方法  1双线性变换法  2脉冲响应不变法   
	file.write(&bianhuanstyle, sizeof(bianhuanstyle));
	file.write((char*)(&band), sizeof(band));
	file.write((char*)(&iirstyle), sizeof(iirstyle));
	file.write((char*)(&sample), sizeof(sample));
	file.write((char*)(&Iirpinlv1), sizeof(Iirpinlv1));
	file.write((char*)(&Iirpinlv2), sizeof(Iirpinlv2));
	file.write((char*)(&Iirpinlv3), sizeof(Iirpinlv3));
	file.write((char*)(&Iirpinlv4), sizeof(Iirpinlv4));
	file.write((char*)(&ap), sizeof(ap));
	file.write((char*)(&as), sizeof(as));
	file.write((char*)(&N), sizeof(N));

	//------------------------z--------------------
	assert(sizeof(uint32_t)==4);
	uint32_t size=z.size();
	file.write((char*)(&size), sizeof(uint32_t));
	for(size_t i=0;i<z.size();i++)
	{
		std::complex<double> zi=z[i];
		file.write((char*)(&zi), sizeof(std::complex<double>));
	}

	//------------------------p--------------------
	size=p.size();
	file.write((char*)(&size), sizeof(uint32_t));
	for(unsigned int i=0;i<p.size();i++)
	{
		std::complex<double> pi=p[i];
		file.write((char*)(&pi), sizeof(std::complex<double>));
	}

	//-----------------------g-----------------------
	file.write((char*)(&g), sizeof(g));
	return true;
}

bool CIirFilter::Loadfdd(const std::string &namestr)
{
	std::fstream file(namestr, std::fstream::in | std::fstream::binary);
	pathname=namestr;

	char szTmp[10];
	memset(szTmp, 0, 10 * sizeof(char));
	file.read(szTmp, 3 * sizeof(char)) ;
	if ((strncmp(szTmp, "IIR", 3) != 0)) 
	{
		return false;
	}

	char bianhuanstyle; //变换方法  1双线性变换法  2脉冲响应不变法   
    file.read(&bianhuanstyle, sizeof(bianhuanstyle));
	file.read((char*)(&band), sizeof(band));
    file.read((char*)(&iirstyle), sizeof(iirstyle));
    file.read((char*)(&sample), sizeof(sample));
    file.read((char*)(&Iirpinlv1), sizeof(Iirpinlv1));
    file.read((char*)(&Iirpinlv2), sizeof(Iirpinlv2));
    file.read((char*)(&Iirpinlv3), sizeof(Iirpinlv3));
    file.read((char*)(&Iirpinlv4), sizeof(Iirpinlv4));
    file.read((char*)(&ap), sizeof(ap));
    file.read((char*)(&as), sizeof(as));
    file.read((char*)(&N), sizeof(N));

	//------------------------z--------------------
	assert(sizeof(uint32_t)==4);
	uint32_t size = 0;
	file.read((char*)(&size), sizeof(uint32_t));
	z.resize(size);
	for(size_t i=0;i<size;i++)
	{
		std::complex<double> zi;
		file.read((char*)(&zi), sizeof(std::complex<double>));
		z[i]=zi;
	}

	//------------------------p--------------------
	file.read((char*)(&size), sizeof(uint32_t));
	p.resize(size);
	for(size_t i=0;i<size;i++)
	{
		std::complex<double> pi;
		file.read((char*)(&pi), sizeof(std::complex<double>));
		p[i]=pi;
	}

	//-----------------------g-----------------------
	file.read((char*)(&g), sizeof(g));

	/*for(size_t i=0;i<z.size();i++)
		TRACE("%f+i%f\n",z[i].real(),z[i].imag());

    TRACE("\n");
	for(size_t i=0;i<p.size();i++)
		TRACE("%f+i%f\n",p[i].real(),p[i].imag());

	TRACE("g=%f\n",g);*/

	zp2tfz(z, p, g, b, a);
	return true;
}

std::string CIirFilter::GetInformationstr()
{
	std::ostringstream temp;

	if(language==Chinese)
	{
		switch(band)
		{
		case LOW:
			temp<<"低通\n采样率:"<<sample<<"\n通带截止频率:"<<Iirpinlv1<<"\n阻带截止频率:"<<Iirpinlv3<<"\n";
			break;
		case HIGH:
			temp<<"高通\n采样率:"<<sample<<"\n阻带下限频率:"<<Iirpinlv1<<"\n通带上限频率:"<<Iirpinlv3<<"\n";
			break;
		case PASS:
			temp<<"带通\n采样率:"<<sample<<"\n带通下限频率:"<<Iirpinlv1<<"\n带通上限频率:"<<Iirpinlv2<<"\n下阻带截止频率:"<<Iirpinlv3<<"\n上阻带截止频率:"<<Iirpinlv4<<"\n";
			break;
		case STOP:
			temp<<"带阻\n采样率:"<<sample<<"\n阻带下限频率:"<<Iirpinlv1<<"\n阻带上限频率:"<<Iirpinlv2<<"\n下通带截止频率:"<<Iirpinlv3<<"\n上通带截止频率:"<<Iirpinlv4<<"\n";
			break;
		}
		temp<<"通带最大衰减"<<ap<<"\n阻带最小衰减"<<as<<"\n";

		switch(iirstyle)
		{
		case 0:
			temp<<"巴特沃斯型\n阶数"<<N<<"\n";
			break;
		case 1:
			temp<<"切比雪夫I型\n阶数"<<N<<"\n";
			break;
		case 2:
			temp<<"切比雪夫II型\n阶数"<<N<<"\n";
			break;
		case 3:
			temp<<"椭圆型\n阶数"<<N<<"\n";
			break;
		}
	}
	else
	{
		switch(band)
		{
		case LOW:
			temp<<"Lowpass\nSample "<<sample<<"\nPass "<<Iirpinlv1<<"\nStop "<<Iirpinlv3<<"\n";
			break;
		case HIGH:
			temp<<"Highpass\nSample "<<sample<<"\nStop "<<Iirpinlv1<<"\nPass "<<Iirpinlv3<<"\n";
			break;
		case PASS:
			temp<<"Bandpass\nSample "<<sample<<"\nPass "<<Iirpinlv1<<"\nPass "<<Iirpinlv2<<"\nStop "<<Iirpinlv3<<"\nStop "<<Iirpinlv4<<"\n";
			break;
		case STOP:
			temp<<"Bandstop\nSample "<<sample<<"\nStop "<<Iirpinlv1<<"\nStop "<<Iirpinlv2<<"\nPass "<<Iirpinlv3<<"\nPass "<<Iirpinlv4<<"\n";
			break;
		}
		temp<<"Ap "<<ap<<"\nAs "<<as<<"\n";

		switch(iirstyle)
		{
		case 0:
			temp<<"Butterworth\nOrder "<<N<<"\n";
			break;
		case 1:
			temp<<"Chebyshev Type I\nOrder "<<N<<"\n";
			break;
		case 2:
			temp<<"Chebyshev Type II\nOrder "<<N<<"\n";
			break;
		case 3:
			temp<<"elliptic\nOrder "<<N<<"\n";
			break;
		}
	}
	std::string Firhneidt=temp.str();
	return Firhneidt;
}

int CIirFilter::GetZerosN()
{
	return z.size();
}

int CIirFilter::GetPolesN()
{
	return p.size();
}

void CIirFilter::SetBand(BANDTYPE nband)
{
	band=nband;
	switch(band)
	{
	case LOW:
		Iirpinlv1=sample/5;
		Iirpinlv2=0;
		Iirpinlv3=sample*2/5;
		Iirpinlv4=0;
		break;
	case HIGH:
		Iirpinlv1=sample/5;
		Iirpinlv2=0;
		Iirpinlv3=sample*2/5;
		Iirpinlv4=0;
		break;
	case PASS:
		Iirpinlv1=sample/5;
		Iirpinlv2=sample*3/10;
		Iirpinlv3=sample/10;
		Iirpinlv4=sample*2/5;
		break;
	case STOP:
		Iirpinlv1=sample/5;
		Iirpinlv2=sample*3/10;
		Iirpinlv3=sample/10;
		Iirpinlv4=sample*2/5;
		break;
	};
}

void CIirFilter::Filter(int *date, size_t num)
{
	//assert(num<=64*1024);

	int m=(int)(log((double)num)/log(2.0));
	unsigned int FFT_LEN;
	if(pow((double)2.0,m)==num)
		FFT_LEN=(int)(pow((double)2.0,m));
	else
		FFT_LEN=(int)(pow((double)2.0,m+1));	

	double *bb=new double[b.size()];
	double *aa=new double[a.size()];

	for(size_t i=0;i<b.size();i++)
		bb[i]=b[i];
	for(size_t i=0;i<a.size();i++)
		aa[i]=a[i];

	double *x=new double[FFT_LEN];

	for(size_t i=0;i<num;i++)
		x[i]=date[i];

	filters_overlap_add_fht(bb, b.size(), aa, a.size(), x, num, FILTER_FHT_N);

	for(size_t i=0;i<num;i++)
		date[i]=(int)(x[i]);

	delete []x;
	delete []bb;
	delete []aa;
}

void CIirFilter::Filter(double *date, size_t num)
{
	//assert(num<=64*1024);

	int m=(int)(log((double)num)/log(2.0));
	unsigned int FFT_LEN;
	if(pow((double)2.0,m)==num)
		FFT_LEN=(int)(pow((double)2.0,m));
	else
		FFT_LEN=(int)(pow((double)2.0,m+1));	

	double *bb=new double[b.size()];
	double *aa=new double[a.size()];

	for(size_t i=0;i<b.size();i++)
		bb[i]=b[i];
	for(size_t i=0;i<a.size();i++)
		aa[i]=a[i];

	double *x=new double[FFT_LEN];

	memcpy(x, date, num*sizeof(double));
	if(FFT_LEN!=num)
		memset(x+num, 0, (FFT_LEN-num)*sizeof(double));
	//for(size_t i=0;i<num;i++)
	//	x[i]=date[i];

	filters_overlap_add_fht(bb, b.size(), aa, a.size(), x, num, FILTER_FHT_N);

	memcpy(date, x, num*sizeof(double));
	//for(size_t i=0;i<num;i++)
	//	date[i]=(int)(x[i]);

	delete []x;
	delete []bb;
	delete []aa;
}
