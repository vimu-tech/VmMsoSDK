#include "iir.h"
#include "stdlib.h"
#include "stdio.h"
#include <memory.h>
#include "assert.h"

//--------------------------------------------------------------------------------模拟滤波器设计---------------------------------------------------------------------//
/*---------------------------计算巴特沃斯滤波器的N-------------------------
int      band------------整型变量。滤波器的通带形式。取值为1，2，3和4，分别对应低通、高通、带通和带阻滤波器
double   fl--------------双精度实型变量。
double   f2--------------双精度实型变量。
double   f3--------------双精度实型变量。
double   f4--------------双精度实型变量。
	     低通时，f1是通带边界频率，f2是阻带边界频率，f3=f4=0；
		 高通时，f1是阻带边界频率，f2是通带边界频率，f3=f4=0；
		 带通时，f2是通带下边界频率，f3是通带上边界频率，f1是阻带下边界频率，f4是阻带上边界频率；
		 带阻时，f1是通带下边界频率，f4是通带上边界频率，f2是阻带下边界频率，f3是阻带上边界频率。
int     ap---------------整型变量，通带最大衰减
int     as---------------整型变量，阻带最小衰减
*/
/*-------------------根据给定的N，计算巴特沃斯滤波器第n个基本节的系数---------------------
int      N---------------整型变量。总共的阶数N。
int      n---------------现在求的系数数组的基本节序数  偶数：0~N/2-1；奇数：0~(N-1)/2
double*  c---------------存放滤波器分母多项式的系数。分子系数为1
         此处的系数是根据巴特沃斯滤波器的极点分布特点算的。
返回填入数组中的系数个数
*/
int Cal_Btw_xishu(int N, int n, double *b, double *a)
{
	double pi;
	pi=4.0*atan(1.0);
	if((N%2!=0)&&(n==(N-1)/2)) //N是奇数且现在求的是最后一个的系数
	{
		*(b+2)=0.0;
		*(b+1)=0.0;
		*(b+0)=1.0;
		*(a+2)=0.0;
		*(a+1)=1.0;
		*(a+0)=1.0;
		return 2;
	}
	else
	{
		*(b+2)=0.0;
		*(b+1)=0.0;
		*(b+0)=1.0;
		*(a+2)=1.0;
		*(a+1)=-2*cos(pi*(0.5+((double)2*(double)n+(double)1)/((double)2*(double)N)));
		*(a+0)=1.0;
	}
	return 3;
}
/*--------------------计算模拟滤波器的N---------------------
*/
/*----------------------巴特沃斯滤波器求H(s)-----------------------
int      band------------整型变量。滤波器的通带形式。取值为1，2，3和4，分别对应低通、高通、带通和带阻滤波器
double   fl--------------双精度实型变量。
double   f2--------------双精度实型变量。
double   f3--------------双精度实型变量。
double   f4--------------双精度实型变量。
	     低通时，f1是通带边界频率，f2是阻带边界频率，f3=f4=0；
		 高通时，f1是阻带边界频率，f2是通带边界频率，f3=f4=0；
		 带通时，f2是通带下边界频率，f3是通带上边界频率，f1是阻带下边界频率，f4是阻带上边界频率；
		 带阻时，f1是通带下边界频率，f4是通带上边界频率，f2是阻带下边界频率，f3是阻带上边界频率。
int      ap--------------整型变量，通带最大衰减
int      as--------------整型变量，阻带最小衰减
double   omgc------------3dB截至频率
double*  b---------------双精度实型二位数组，体积为(N+1)/2*(n+1)。存放滤波器分子多项式的系数。
                         b[j][i]表示第j个n阶节的分子多项式的第i个系数
double*  a---------------双精度二位数组，体积为(N+1)/2*(n+1)。存放滤波器分母多项式的系数。
                         a[j][i]表示第jj个n阶节的分母多项式的第i个系数
		 其中n，是每节滤波器的阶数，低通和高通为2，带通和带阻为4

返回设计的滤波器的阶数N
我们在每个节数组的高地址，存放s阶数高的系数。例如，低通啊a[2]存放s^2的系数
*/
double Cal_Btw_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a)
{
	int i,num;
	double B,omg02,omg1,omg2;

	switch(band)
	{
	case 1:  //低通p=s/omgc
		//如果没有给出omgc，我们计算
	    if(omgc<=0)
	    {  
	       omgc=f1*pow((pow(10,0.1*ap)-1),-1.0/(double)(2*N)); 
	       //omgc=f2* pow((pow(10,0.1*as)-1),-1.0/(double)(2*N));
	    }
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_Btw_xishu(N, i, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				*(a+i*3+1)=1.0;
				*(a+i*3)=omgc;  
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=0.0;
				*(b+i*3)=omgc;
			}
			else
			{   //分母
				*(a+i*3+1)=*(a+i*3+1)*omgc;
				*(a+i*3)=*(a+i*3)*omgc*omgc;
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=0.0;
				*(b+i*3)=omgc*omgc;
			}
		}
		break;

	case 2:  //高通p=omgc/s
		if(omgc<=0)
	    {
	       omgc=f2*pow((pow(10,0.1*ap)-1),-1.0/(double)(2*N)); //??????????????????????
		   //f1=2*pi*f1/fs;
		   //f1=1/f1;
	       //omgc=f1* pow((pow(10,0.1*as)-1),-1.0/(double)(2*N));
		}
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_Btw_xishu(N, i, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				*(a+i*3+1)=1.0;
				*(a+i*3)=omgc;  
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=1.0;
				*(b+i*3)=0.0;
			}
			else
			{   //分母
				*(a+i*3+1)=*(a+i*3+1)*omgc;
				*(a+i*3)=*(a+i*3)*omgc*omgc;
				//分子
				*(b+i*3+2)=1.0;
				*(b+i*3+1)=0.0;
				*(b+i*3)=0.0;
			}
		}
		break;
	case 3:
		assert(N%2==0);

		omg1=f3;    
	    omg2=f2;
		B=omg1-omg2;
		omg02=omg1*omg2;

		for(i=0;i<(N/2+1)/2;i++)
		{
			num=Cal_Btw_xishu(N/2, i, b+i*5+2, a+i*5+2); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=1.0;
				*(a+i*5+1)=B;
				*(a+i*5)=omg02;  
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=0.0;
				*(b+i*5+1)=B;
				*(b+i*5)=0.0;
			}
			else
			{   //分母
				*(a+i*5)=omg02*omg02;
				*(a+i*5+1)=(*(a+i*5+3))*B*omg02;
				*(a+i*5+2)=B*B+2*omg02;
				*(a+i*5+3)=(*(a+i*5+3))*B;
				*(a+i*5+4)=1.0;
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=B*B;
				*(b+i*5+1)=0.0;
				*(b+i*5)=0.0;
			}
		}
		break;
	case 4:
		assert(N%2==0);
		omg1=f4;    
	    omg2=f1;
		B=omg1-omg2;
		omg02=omg1*omg2;
		for(i=0;i<(N/2+1)/2;i++)
		{
			num=Cal_Btw_xishu(N/2, i, b+i*5+2, a+i*5+2); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=1.0;
				*(a+i*5+1)=B;
				*(a+i*5)=omg02;  
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=1.0;
				*(b+i*5+1)=0.0;
				*(b+i*5)=omg02;
			}
			else
			{   //分母
				*(a+i*5)=omg02*omg02;
				*(a+i*5+1)=(*(a+i*5+3))*B*omg02;
				*(a+i*5+2)=B*B+2*omg02;
				*(a+i*5+3)=(*(a+i*5+3))*B;
				*(a+i*5+4)=1.0;
							
				//分子
				*(b+i*5+4)=1.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=2*omg02;
				*(b+i*5+1)=0.0;
				*(b+i*5)=omg02*omg02;
			}
		}
		break;
	}
	return 1.0;   
}

//--------------------------------------------------------------------------------数字滤波器设计------------------------------------------------------------------//

//----------------------------------------------------------脉冲响应不变法--------------------------------------------//
/*
 计算通过数字滤波器参数转换后的阶数N,因为我们的频率都是以Hz的形式给出的，故设计模拟时需*2*pi,但数字转换后的不用。
 */
/*---------------------------计算巴特沃斯滤波器的N-------------------------
int      band------------整型变量。滤波器的通带形式。取值为1，2，3和4，分别对应低通、高通、带通和带阻滤波器
double   fl--------------双精度实型变量。
double   f2--------------双精度实型变量。
double   f3--------------双精度实型变量。
double   f4--------------双精度实型变量。
	     低通时，f1是通带边界频率，f2是阻带边界频率，f3=f4=0；
		 高通时，f1是阻带边界频率，f2是通带边界频率，f3=f4=0；
		 带通时，f2是通带下边界频率，f3是通带上边界频率，f1是阻带下边界频率，f4是阻带上边界频率；
		 带阻时，f1是通带下边界频率，f4是通带上边界频率，f2是阻带下边界频率，f3是阻带上边界频率。
int     ap---------------整型变量，通带最大衰减
int     as---------------整型变量，阻带最小衰减
*/
int Cal_Btw_N_moni(int band, double f1, double f2, double f3, double f4, int ap, int as)  
{
	double kps,lps,B,hs1,hs2,hl,hu,h02,ls0,ls1;
	int N;
	switch(band)
	{
	case 1:
		kps=sqrt((pow(10.0,0.1*ap)-1)/(pow(10.0,0.1*as)-1));
		lps=f1/f2;   //此处频率需要归一化，但是相除就抵消了，故我们没有     
                                                         //思：此处的相除可以说明T的大小不会改变阶数N!!!!!!!!!!!!!!!!!!
        N=(int)(log10(kps)/log10(lps)+1);
		break;
	case 2:
		kps=sqrt((pow(10.0,0.1*ap)-1)/(pow(10.0,0.1*as)-1));
		lps=f1/f2;  //高通变低通f变倒数   f2是ap  //此处频率需要归一化，但是相除就抵消了，故我们没有
        N=(int)(log10(kps)/log10(lps)+1);
		break;
	case 3:
		B=f3-f2;
        hs1=f1/B; hs2=f4/B; hl=f2/B; hu=f3/B; h02=hl*hu;
		ls0=(hs2*hs2-h02)/hs2;  //归一化
		ls1=(h02-hs1*hs1)/hs1;
		ls0=min(ls0,ls1);
		lps=1/ls0;  
		kps=sqrt((pow(10.0,0.1*ap)-1)/(pow(10.0,0.1*as)-1));
        N=(int)(log10(kps)/log10(lps)+1);
		break;
	case 4:
		B=f4-f1;
        hs1=f2/B; hs2=f3/B; hl=f1/B; hu=f4/B; h02=hl*hu;
		ls0=hs1/(h02-hs1*hs1);  //归一化
		ls1=hs2/(hs2*hs2-h02);
		ls0=min(ls0,ls1);
		lps=1/ls0;  
		kps=sqrt((pow(10.0,0.1*ap)-1)/(pow(10.0,0.1*as)-1));
        N=(int)(log10(kps)/log10(lps)+1);
		break;
	}
	return N*(band>=3? 2:1);
}
int Cal_Btw_N_mch(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as) 
{
	double omg1,omg2,omg3,omg4;
    double pi;
	pi=4.0*atan(1.0);

	if(band==2||band==4)  //不支持高通和带阻
		return -2; 

	omg1=2*pi*f1/fs/T;   
	omg2=2*pi*f2/fs/T;
	if(band>=3)
	{
	    omg3=2*pi*f3/fs/T;
	    omg4=2*pi*f4/fs/T;
	}
	else
	{
		omg3=0;
	    omg4=0;
	}
	return Cal_Btw_N_moni(band, omg1 ,omg2, omg3, omg4, ap, as);
}
int Cal_Btw_N_shxx(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as)
{
	double omg1,omg2,omg3,omg4;
	double pi;
	pi=4.0*atan(1.0);

    assert(fs>2*f1);assert(fs>2*f2);assert(fs>2*f3);assert(fs>2*f4);

  	omg1=2/T*tan(pi*f1/fs);    //2/T*tan(2*pi*f1/fs/2);
	omg2=2/T*tan(pi*f2/fs);
	if(band>=3)
	{
	    omg3=2/T*tan(pi*f3/fs);
	    omg4=2/T*tan(pi*f4/fs);
	}
	else
	{
		omg3=0;
	    omg4=0;
	}

	return Cal_Btw_N_moni(band, omg1 ,omg2, omg3, omg4, ap, as);
}
int Cal_Btw_N(char bianhuan, int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as)
{
	if(bianhuan==0)
		return Cal_Btw_N_mch(band, T, fs, f1, f2, f3, f4, ap, as);
	else
		return Cal_Btw_N_shxx(band, T, fs, f1, f2, f3, f4, ap, as);
}
double Cal_Btw_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a)
{
	int i,num;
    double pi;
	double B,omg1,omg2,omg02;
	pi=4.0*atan(1.0);
    
	if((band==2)||(band==4))  //脉冲响应不变法只适合设计高通和带通
	{
		printf("脉冲响应不变法不适合设计高通和带阻");
		return -2;
	}
	
	switch(band)
	{
	case 1:  //低通p=s/omgc
		//如果没有给出omgc，我们计算
	    if(omgc<=0)
	    {
	       f1=2*pi*f1/fs/T;    //  2*pi*f1/fs/T
	       omgc=f1*pow((pow(10,0.1*ap)-1),-1.0/(double)(2*N)); //??????????????????????
		   //f2=2*pi*f2/fs/T; 
	       //omgc=f2* pow((pow(10,0.1*as)-1),-1.0/(double)(2*N));
	    }
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_Btw_xishu(N, i, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				*(a+i*3+1)=1.0;
				*(a+i*3)=omgc;  
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=0.0;
				*(b+i*3)=omgc;
			}
			else
			{   //分母
				*(a+i*3+2)=1.0;
				*(a+i*3+1)=*(a+i*3+1)*omgc;
				*(a+i*3)=*(a+i*3)*omgc*omgc;
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=0.0;
				*(b+i*3)=omgc*omgc;
			}
		}
		break;
	/*case 2:  //高通p=omgc/s
		if(omgc<=0)
	    {
	       f2=2*pi*f2/fs/T;
		   //f2=1/f2;
	       omgc=f2*pow((pow(10,0.1*ap)-1),-1.0/(double)(2*N)); //??????????????????????
		   //f1=2*pi*f1/fs/T;
		   //f1=1/f1;
	       //omgc=f1* pow((pow(10,0.1*as)-1),-1.0/(double)(2*N));
	    }
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_Btw_xishu(N, i, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				*(a+i*3+1)=1.0;
				*(a+i*3)=omgc;  
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=1.0;
				*(b+i*3)=0.0;
			}
			else
			{   //分母
				*(a+i*3+1)=*(a+i*3+1)*omgc;
				*(a+i*3)=*(a+i*3)*omgc*omgc;
				//分子
				*(b+i*3+2)=1.0;
				*(b+i*3+1)=0.0;
				*(b+i*3)=0.0;
			}
		}
		break;*/
	case 3:
		assert(N%2==0);
		omg1=2*pi*f3/fs/T;    //  2*pi*f1/fs/T
	    omg2=2*pi*f2/fs/T;
		B=omg1-omg2;
		omg02=omg1*omg2;

		for(i=0;i<(N/2+1)/2;i++)  
		{
			num=Cal_Btw_xishu(N/2, i, b+i*5+2, a+i*5+2); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=1.0;
				*(a+i*5+1)=B;
				*(a+i*5)=omg02;  
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=0.0;
				*(b+i*5+1)=B;
				*(b+i*5)=0.0;
			}
			else
			{   //分母
				*(a+i*5)=omg02*omg02;
				*(a+i*5+1)=(*(a+i*5+3))*B*omg02;
				*(a+i*5+2)=B*B+2*omg02;
				*(a+i*5+3)=(*(a+i*5+3))*B;
				*(a+i*5+4)=1.0;
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=B*B;
				*(b+i*5+1)=0.0;
				*(b+i*5)=0.0;
			}
		}
		break;
	/*case 4:
		assert(N%2==0);
		omg1=2*pi*f3/fs/T;    //  2*pi*f1/fs/T
	    omg2=2*pi*f2/fs/T;
		B=omg1-omg2;
		omg02=omg1*omg2;

		for(i=0;i<(N/2+1)/2;i++)
		{
			num=Cal_Btw_xishu(N/2, i, b+i*5+2, a+i*5+2); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=1.0;
				*(a+i*5+1)=B;
				*(a+i*5)=omg02;  
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=1.0;
				*(b+i*5+1)=0.0;
				*(b+i*5)=omg02;
			}
			else
			{   //分母
				*(a+i*5)=omg02*omg02;
				*(a+i*5+1)=(*(a+i*5+3))*B*omg02;
				*(a+i*5+2)=B*B+2*omg02;
				*(a+i*5+3)=(*(a+i*5+3))*B;
				*(a+i*5+4)=1.0;
							
				//分子
				*(b+i*5+4)=1.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=2*omg02;
				*(b+i*5+1)=0.0;
				*(b+i*5)=omg02*omg02;
			}
		}
		break;*/
	}
	return 1.0; 
}

/*------------------------------计算脉冲响应不变法每阶的系数---------------------------
n     ---------阶数，可以是1，2，4  例如：1+s为1
a     ---------分母

注意：1、此算法没有考虑重极点的情况
      2、此算法将会改变a和b中的值
*/
char hstohz_mch_each(int n, double* b, double* a, double T)
{
	int i;
	double *lingdianre,*lingdianim;
	double *ar0,*ai0,*br0,*bi0,*ar1,*ai1,*br1,*bi1,*aor, *aoi, *bor, *boi;
	double a1,a2,a3,Are,Aim,Bre,Bim,Cre,Cim,Dre,Dim,fzre,fzim,fmre,fmim;
	double   hign_b;
	switch(n)
	{
	case 1://单个极点
		{
			hign_b=0.0;
			//处理分子最高系数项不为0的情况
            if(b[1]!=0.0)
			{
				hign_b=b[1]/a[1];
				b[0]=b[0]-b[1]*a[0]/a[1];
				b[1]=0.0;
			}

			//分子
			b[2]=b[0]/a[1];  //在H(s)中b[0]存s^0
			b[0]=0.0;
            b[1]=0.0;
            
            //分母
		    a1=a[0];
		    a2=a[1];
		    a3=a[2];

		    a[0]=0.0;
			a1=-a1/a2*T;
		    a[1]=-exp(a1);
		    a[2]=1.0;

			//处理分子最高系数项不为0的情况
            if(hign_b!=0.0)
			{
				for(i=0;i<3;i++)  //理论上虚部应该是0
			    {
				    b[i]+=hign_b*a[i];
			    }
			}

			//将求得的分母系数，最高的变成1,本来就是1，不用
			break;
		}
	case 2://两个极点形式
		{
			hign_b=0.0;
			//处理分子最高系数项不为0的情况
            if(b[2]!=0.0)
			{
				hign_b=b[2]/a[2];
				b[0]=b[0]-b[2]*a[0]/a[2];
				b[1]=b[1]-b[2]*a[1]/a[2];
				b[2]=0.0;
			}

			lingdianre=malloc(2*sizeof(double));
			lingdianim=malloc(2*sizeof(double));
			//printf("输入系数为：\n%f\t%f\t%f\n",a[2], a[1],a[0]);
			if(qrrt(2, a, lingdianre, lingdianim, 0.00001, 100)<0)
				return -1;
			//printf("请的极点为：\n%f+j%f\t%f+j%f\n",lingdianre[0], lingdianim[0],lingdianre[1], lingdianim[1]);
            //A
			fzre=b[0]+b[1]*lingdianre[0];
			fzim=b[1]*lingdianim[0];
			fmre=lingdianre[0]-lingdianre[1];
			fmim=lingdianim[0]-lingdianim[1];
			cplx_divtozhj(&fzre, &fzim, &fmre, &fmim);
			Are=fzre;
			Aim=fzim;
			//B
			fzre=b[0]+b[1]*lingdianre[1];
		    fzim=b[1]*lingdianim[1];
			fmre=lingdianre[1]-lingdianre[0];
			fmim=lingdianim[1]-lingdianim[0];
			cplx_divtozhj(&fzre, &fzim, &fmre, &fmim);
			Bre=fzre;
			Bim=fzim;

			ar0=malloc(2*sizeof(double));
			ai0=malloc(2*sizeof(double));
			br0=malloc(2*sizeof(double));
			bi0=malloc(2*sizeof(double));
			ar1=malloc(2*sizeof(double));
			ai1=malloc(2*sizeof(double));
			br1=malloc(2*sizeof(double));
			bi1=malloc(2*sizeof(double));
			aor=malloc(3*sizeof(double));
			aoi=malloc(3*sizeof(double));
			bor=malloc(3*sizeof(double));
			boi=malloc(3*sizeof(double));
         
			ar0[1]=1;ai0[1]=0.0;
			fzre=lingdianre[0]*T;
			fzim=lingdianim[0]*T;
			e_pow(&fzre, &fzim);
			ar0[0]=-fzre;ai0[0]=-fzim;
			br0[1]=Are;bi0[1]=Aim;
			br0[0]=0.0;bi0[0]=0.0;

			ar1[1]=1;ai1[1]=0.0;
			fzre=lingdianre[1]*T;
			fzim=lingdianim[1]*T;
			e_pow(&fzre, &fzim);
			ar1[0]=-fzre;ai1[0]=-fzim;
			br1[1]=Bre;bi1[1]=Bim;
			br1[0]=0.0;bi1[0]=0.0;

			cplx_fmdxsh_add(1, 1, ar0, ai0, br0, bi0, ar1, ai1, br1, bi1, aor, aoi, bor, boi);

			for(i=0;i<3;i++)  //理论上虚部应该是0
			{
			     if((fabs(aoi[i])>0.000001)||(fabs(boi[i])>0.000001))
					 return -1;
				 a[i]=aor[i];     //我们在数组的高地址装z^0
				 b[i]=bor[i];
			}

			//处理分子最高系数项不为0的情况
            if(hign_b!=0.0)
			{
				for(i=0;i<3;i++)  //理论上虚部应该是0
			    {
				    b[i]+=hign_b*a[i];
			    }
			}
			//将求得的分母系数，最高的变成1
			*(a+1)/=*(a+2);
		    *(a)/=*(a+2);
			*(b+2)/=*(a+2);
			*(b+1)/=*(a+2);
			*(b)/=*(a+2);
			*(a+2)=1; 

			free(lingdianre);
	        free(lingdianim);
	        free(ar0);
	        free(ai0);
	        free(br0);
	        free(bi0);
	        free(ar1);
	        free(ai1);
	        free(br1);
	        free(bi1);
	        free(aor);
        	free(aoi);
        	free(bor);
        	free(boi);
			break;
		}
	case 4://	四个极点形式
		{
			hign_b=0.0;
			//处理分子最高系数项不为0的情况
            if(b[4]!=0.0)
			{
				hign_b=b[4]/a[4];
				b[0]=b[0]-b[4]*a[0]/a[4];
				b[1]=b[1]-b[4]*a[1]/a[4];
				b[2]=b[0]-b[4]*a[2]/a[4];
				b[3]=b[1]-b[4]*a[3]/a[4];
				b[4]=0.0;
			}

			lingdianre=malloc(4*sizeof(double));
			lingdianim=malloc(4*sizeof(double));
			//printf("输入系数为：\n%f\t%f\t%f\t%f\t%f\n",a[4], a[3],a[2], a[1],a[0]);
			if(qrrt(4, a, lingdianre, lingdianim, 0.00001, 100)<0)
				return -1;
			//printf("请的极点为：\n%f+j%f\t%f+j%f\t%f+j%f\t%f+j%f\n",lingdianre[0], lingdianim[0],lingdianre[1], lingdianim[1],lingdianre[2], lingdianim[2],lingdianre[3], lingdianim[3]);
            //A分子
			fzre=b[0]+b[1]*lingdianre[0];
			fzim=b[1]*lingdianim[0];
			Are=lingdianre[0];  //s*s
			Aim=lingdianim[0];
			cplx_pow(&Are, &Aim, 2);
			fzre+=b[2]*Are;
            fzim+=b[2]*Aim;
			Are=lingdianre[0];  //s*s*s
			Aim=lingdianim[0];
			cplx_pow(&Are, &Aim, 3);
			fzre+=b[3]*Are;
            fzim+=b[3]*Aim;
            //A分母
			cplx_mul(lingdianre[0]-lingdianre[1],lingdianim[0]-lingdianim[1], lingdianre[0]-lingdianre[2], lingdianim[0]-lingdianim[2], &Are, &Aim);
			cplx_mul(Are,Aim, lingdianre[0]-lingdianre[3], lingdianim[0]-lingdianim[3], &fmre, &fmim);

			cplx_divtozhj(&fzre, &fzim, &fmre, &fmim);
			Are=fzre;
			Aim=fzim;
            //printf("A：%f+j%f\n",Are,Aim);
			//B分子
			fzre=b[0]+b[1]*lingdianre[1];
			fzim=b[1]*lingdianim[1];
			Bre=lingdianre[1];  //s*s
			Bim=lingdianim[1];
			cplx_pow(&Bre, &Bim, 2);
			fzre+=b[2]*Bre;
            fzim+=b[2]*Bim;
			Bre=lingdianre[1];  //s*s*s
			Bim=lingdianim[1];
			cplx_pow(&Bre, &Bim, 3);
			fzre+=b[3]*Bre;
            fzim+=b[3]*Bim;
            //B分母
			cplx_mul(lingdianre[1]-lingdianre[0],lingdianim[1]-lingdianim[0], lingdianre[1]-lingdianre[2], lingdianim[1]-lingdianim[2], &Bre, &Bim);
			cplx_mul(Bre,Bim, lingdianre[1]-lingdianre[3], lingdianim[1]-lingdianim[3], &fmre, &fmim);

			cplx_divtozhj(&fzre, &fzim, &fmre, &fmim);
			Bre=fzre;
			Bim=fzim;
            //printf("B：%f+j%f\n",Bre,Bim);
			//C分子
			fzre=b[0]+b[1]*lingdianre[2];
			fzim=b[1]*lingdianim[2];
			Cre=lingdianre[2];  //s*s
			Cim=lingdianim[2];
			cplx_pow(&Cre, &Cim, 2);
			fzre+=b[2]*Cre;
            fzim+=b[2]*Cim;
			Cre=lingdianre[2];  //s*s*s
			Cim=lingdianim[2];
			cplx_pow(&Cre, &Cim, 3);
			fzre+=b[3]*Cre;
            fzim+=b[3]*Cim;
            //C分母
			cplx_mul(lingdianre[2]-lingdianre[0],lingdianim[2]-lingdianim[0], lingdianre[2]-lingdianre[1], lingdianim[2]-lingdianim[1], &Cre, &Cim);
			cplx_mul(Cre,Cim, lingdianre[2]-lingdianre[3], lingdianim[2]-lingdianim[3], &fmre, &fmim);

			cplx_divtozhj(&fzre, &fzim, &fmre, &fmim);
			Cre=fzre;
			Cim=fzim;
            //printf("C：%f+j%f\n",Cre,Cim);
			//D分子
			fzre=b[0]+b[1]*lingdianre[3];
			fzim=b[1]*lingdianim[3];
			Dre=lingdianre[3];  //s*s
			Dim=lingdianim[3];
			cplx_pow(&Dre, &Dim, 2);
			fzre+=b[2]*Dre;
            fzim+=b[2]*Dim;
			Dre=lingdianre[3];  //s*s*s
			Dim=lingdianim[3];
			cplx_pow(&Dre, &Dim, 3);
			fzre+=b[3]*Dre;
            fzim+=b[3]*Dim;
            //D分母
			cplx_mul(lingdianre[3]-lingdianre[0],lingdianim[3]-lingdianim[0], lingdianre[3]-lingdianre[1], lingdianim[3]-lingdianim[1], &Dre, &Dim);
			cplx_mul(Dre,Dim, lingdianre[3]-lingdianre[2], lingdianim[3]-lingdianim[2], &fmre, &fmim);

			cplx_divtozhj(&fzre, &fzim, &fmre, &fmim);
			Dre=fzre;
			Dim=fzim;
            //printf("D：%f+j%f\n",Dre,Dim);
			ar0=malloc(4*sizeof(double));
			ai0=malloc(4*sizeof(double));
			br0=malloc(4*sizeof(double));
			bi0=malloc(4*sizeof(double));
			ar1=malloc(4*sizeof(double));
			ai1=malloc(4*sizeof(double));
			br1=malloc(4*sizeof(double));
			bi1=malloc(4*sizeof(double));
			aor=malloc(5*sizeof(double));
			aoi=malloc(5*sizeof(double));
			bor=malloc(5*sizeof(double));
			boi=malloc(5*sizeof(double));
         
			ar0[1]=1;ai0[1]=0.0;
			fzre=lingdianre[0]*T;
			fzim=lingdianim[0]*T;
			e_pow(&fzre, &fzim);
			ar0[0]=-fzre;ai0[0]=-fzim;  //printf("ar0[0]+ai0[0]：%f+j%f\tar1[1]+ai1[1]：%f+j%f\n",ar0[0],ai0[0],ar0[1],ai0[1]);
			br0[1]=Are;bi0[1]=Aim;
			br0[0]=0.0;bi0[0]=0.0;

			ar1[1]=1;ai1[1]=0.0;
			fzre=lingdianre[1]*T;
			fzim=lingdianim[1]*T;
			e_pow(&fzre, &fzim);
			ar1[0]=-fzre;ai1[0]=-fzim;  //printf("ar1[0]+ai1[0]：%f+j%f\tar1[1]+ai1[1]：%f+j%f\n",ar1[0],ai1[0],ar1[1],ai1[1]);
			br1[1]=Bre;bi1[1]=Bim;
			br1[0]=0.0;bi1[0]=0.0;

			cplx_fmdxsh_add(1, 1, ar0, ai0, br0, bi0, ar1, ai1, br1, bi1, aor, aoi, bor, boi);
			
			for(i=0;i<3;i++)
			{
				ar0[i]=aor[i];ai0[i]=aoi[i];
				br0[i]=bor[i];bi0[i]=boi[i];
			}

			ar1[1]=1;ai1[1]=0.0;
			fzre=lingdianre[2]*T;
			fzim=lingdianim[2]*T;
			e_pow(&fzre, &fzim);
			ar1[0]=-fzre;ai1[0]=-fzim;
			br1[1]=Cre;bi1[1]=Cim;
			br1[0]=0.0;bi1[0]=0.0;

			cplx_fmdxsh_add(2, 1, ar0, ai0, br0, bi0, ar1, ai1, br1, bi1, aor, aoi, bor, boi);
			
			for(i=0;i<4;i++)
			{
				ar0[i]=aor[i];ai0[i]=aoi[i];
				br0[i]=bor[i];bi0[i]=boi[i];
			}

			ar1[1]=1;ai1[1]=0.0;
			fzre=lingdianre[3]*T;
			fzim=lingdianim[3]*T;
			e_pow(&fzre, &fzim);
			ar1[0]=-fzre;ai1[0]=-fzim;
			br1[1]=Dre;bi1[1]=Dim;
			br1[0]=0.0;bi1[0]=0.0;
			cplx_fmdxsh_add(3, 1, ar0, ai0, br0, bi0, ar1, ai1, br1, bi1, aor, aoi, bor, boi);
			//printf("结果:");
			//for(i=0;i<5;i++)
			    //printf("aor[%d]+aoi[%d]=%f+j%f\n",i,i,aor[i],aoi[i]);
			//for(i=0;i<5;i++)
			    //printf("bor[%d]+boi[%d]=%f+j%f\n",i,i,bor[i],boi[i]);

			for(i=0;i<5;i++)  //理论上虚部应该是0
			{
			     if((fabs(aoi[i])>0.000001)||(fabs(boi[i])>0.000001))
					 return -1;
				 a[i]=aor[i];   //我们在数组的高地址装z^0
				 b[i]=bor[i];
			}

			//处理分子最高系数项不为0的情况
            if(hign_b!=0.0)
			{
				for(i=0;i<5;i++)  
			    {
				    b[i]+=hign_b*a[i];
			    }
			}

			//将求得的分母系数，最高的变成1
			*(a+3)/=*(a+4);
			*(a+2)/=*(a+4);
			*(a+1)/=*(a+4);
			*(a)/=*(a+4);
			*(b+4)/=*(a+4);
			*(b+3)/=*(a+4);
			*(b+2)/=*(a+4);
			*(b+1)/=*(a+4);
			*(b)/=*(a+4);
			*(a+4)=1; 

			free(lingdianre);
	        free(lingdianim);
	        free(ar0);
	        free(ai0);
	        free(br0);
	        free(bi0);
	        free(ar1);
	        free(ai1);
	        free(br1);
	        free(bi1);
	        free(aor);
        	free(aoi);
        	free(bor);
        	free(boi);
			break;
		}
	}
	return 1;
}

char hstohz_mch(int ns, int n, double* b, double* a, double T)
{
    int i;
	if(n==2)
	{
		jltobl(ns, 2, b, a);  //先将级联转换成并联
		for(i=0;i<ns;i++)
	   {   
		   if(fabs(*(a+i*3+2))<0.000000001)
		   { 
			   if(hstohz_mch_each(1, b+i*3, a+i*3, T)==-1)
				   return -1;
		   }
		   else
		   {   
			   if(hstohz_mch_each(2, b+i*3, a+i*3, T)==-1)
				   return -1;
		   }
	    }
	}
	else if(n==4)
	{
		jltobl(ns, 4, b, a);  //先将级联转换成并联
		for(i=0;i<ns;i++)
	   {
		   if((fabs(*(a+i*5+3))<0.000000001)&&(fabs(*(a+i*5+4))<0.000000001))
		   {   //分子	
			    if(hstohz_mch_each(2, b+i*5, a+i*5, T)==-1)
				   return -1;
		   }
		   else
		   {   
			    if(hstohz_mch_each(4, b+i*5, a+i*5, T)==-1)
				   return -1;
		   }
		}
	}
    return 1;
}
//----------------------------------------------------------双线性变换法-----------------------------------------------//

/*----------------------直接设计，不是先设计模拟，再转换成数字
*/
int Cal_Btw_Hz_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a)
{
	int i;
    double pi;
	double B,omg02,omg1,omg2;
	double x;
	pi=4.0*atan(1.0);
   
	switch(band)
	{
	case 1:  //低通p=s/omgc
		//如果没有给出omgc，我们计算
	    if(omgc<=0)
	   {
		    omg1=2/T*tan(pi*f1/fs);    //2/T*tan(2*pi*f1/fs/2);
	        omgc=omg1* pow((pow(10,0.1*ap)-1),-1.0/(double)(2*N)); //??????????????????????
			//omg2=2/T*tan(pi*f2/fs);
	        //omgc=omg2*pow((pow(10,0.1*as)-1),-1.0/(double)(2*N));
	    }

		for(i=0;i<(N+1)/2;i++)
		{
			x=cos(pi*(0.5+((double)2*(double)i+(double)1)/((double)2*(double)N)));
	        //y=sin(pi*(0.5+((double)2*(double)i+(double)1)/((double)2*(double)N)));

			if((N%2!=0)&&(i==(N-1)/2))
			{    //分母 
				*(a+i*3+2)=2.0+omgc*T;
				*(a+i*3+1)=omgc*T-2.0;
				*(a+i*3)=0.0;  
				//分子
				*(b+i*3+2)=omgc*T;
				*(b+i*3+1)=omgc*T;
				*(b+i*3)=0.0;

				//将求得的分母系数，最高的变成1
				*(a+i*3+1)/=*(a+i*3+2);
				*(a+i*3)/=*(a+i*3+2);
				*(b+i*3+2)/=*(a+i*3+2);
				*(b+i*3+1)/=*(a+i*3+2);
				*(b+i*3)/=*(a+i*3+2);
				*(a+i*3+2)=1; 
			}
			else
			{  //分母 
				*(a+i*3+2)=4.0-4.0*T*x*omgc+omgc*omgc*T*T;  
				*(a+i*3+1)=2*omgc*omgc*T*T-8;
				*(a+i*3)=4.0+4.0*T*x*omgc+omgc*omgc*T*T;
				//分子
				*(b+i*3+2)=omgc*omgc*T*T; 
				*(b+i*3+1)=2*omgc*omgc*T*T;
				*(b+i*3)=omgc*omgc*T*T;

				//将求得的分母系数，最高的变成1
				*(a+i*3+1)/=*(a+i*3+2);
				*(a+i*3)/=*(a+i*3+2);
				*(b+i*3+2)/=*(a+i*3+2);
				*(b+i*3+1)/=*(a+i*3+2);
				*(b+i*3)/=*(a+i*3+2);
				*(a+i*3+2)=1; 
			}
		}
		break;
	case 2:  
		//如果没有给出omgc，我们计算
	    if(omgc<=0)
	   {
		    omg2=2/T*tan(pi*f2/fs);
			//omg2=1/omg2;
	        omgc=omg2* pow((pow(10,0.1*ap)-1),-1.0/(double)(2*N)); //??????????????????????   
		    //omg1=2/T*tan(pi*f1/fs);    //2/T*tan(2*pi*f1/fs/2);
			//omg1=1/omg1;
			//omgc=omg1* pow((pow(10,0.1*as)-1),-1.0/(double)(2*N));  //此处f2是ap
	       
	    }
		for(i=0;i<(N+1)/2;i++)
		{
			x=cos(pi*(0.5+((double)2*(double)i+(double)1)/((double)2*(double)N)));
	        //y=sin(pi*(0.5+((double)2*(double)i+(double)1)/((double)2*(double)N)));

			if((N%2!=0)&&(i==(N-1)/2))
			{    //分母 
				*(a+i*3+2)=2.0+omgc*T;
				*(a+i*3+1)=omgc*T-2.0;
				*(a+i*3)=0.0;  
				//分子
				*(b+i*3+2)=2;
				*(b+i*3+1)=-2;
				*(b+i*3)=0.0;

				//将求得的分母系数，最高的变成1
				*(a+i*3+1)/=*(a+i*3+2);
				*(a+i*3)/=*(a+i*3+2);
				*(b+i*3+2)/=*(a+i*3+2);
				*(b+i*3+1)/=*(a+i*3+2);
				*(b+i*3)/=*(a+i*3+2);
				*(a+i*3+2)=1; 
			}
			else
			{  //分母 
				*(a+i*3+2)=4.0-4.0*T*x*omgc+omgc*omgc*T*T;  
				*(a+i*3+1)=2*omgc*omgc*T*T-8;
				*(a+i*3)=4.0+4.0*T*x*omgc+omgc*omgc*T*T;
				//分子
				*(b+i*3+2)=4; 
				*(b+i*3+1)=-8;
				*(b+i*3)=4;

				//将求得的分母系数，最高的变成1
				*(a+i*3+1)/=*(a+i*3+2);
				*(a+i*3)/=*(a+i*3+2);
				*(b+i*3+2)/=*(a+i*3+2);
				*(b+i*3+1)/=*(a+i*3+2);
				*(b+i*3)/=*(a+i*3+2);
				*(a+i*3+2)=1; 
			}
		}
		break;
	case 3:
		omg1=2/T*tan(pi*f3/fs);    //2/T*tan(2*pi*f1/fs/2);
	    omg2=2/T*tan(pi*f2/fs);
		B=omg1-omg2;
		omg02=omg1*omg2;

		for(i=0;i<(N/2+1)/2;i++)
		{
			x=cos(pi*(0.5+((double)2*(double)i+(double)1)/((double)2*(double)N/2)));
	        //y=sin(pi*(0.5+((double)2*(double)i+(double)1)/((double)2*(double)N/2)));

			if((N/2%2!=0)&&(i==(N/2-1)/2))
			{    //分母 
				*(a+i*5+4)=4.0+2.0*T*B+omg02*T*T;
				*(a+i*5+3)=2.0*omg02*T*T-8.0;
				*(a+i*5+2)=4.0-2.0*T*B+omg02*T*T;
				*(a+i*5+1)=0.0;
				*(a+i*5)=0.0;  
				//分子
				*(b+i*5+4)=2.0*T*B;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=-2.0*T*B;
				*(b+i*5+1)=0.0;
				*(b+i*5)=0.0;

				//将求得的分母系数，最高的变成1
				*(a+i*5+3)/=*(a+i*5+4);
				*(a+i*5+2)/=*(a+i*5+4);
				*(a+i*5+1)/=*(a+i*5+4);
				*(a+i*5)/=*(a+i*5+4);
				*(b+i*5+4)/=*(a+i*5+4);
				*(b+i*5+3)/=*(a+i*5+4);
				*(b+i*5+2)/=*(a+i*5+4);
				*(b+i*5+1)/=*(a+i*5+4);
				*(b+i*5)/=*(a+i*5+4);
				*(a+i*5+4)=1; 
			}
			else
			{  //分母 
				*(a+i*5+4)=16.0-16.0*x*B*T+4.0*T*T*(2*omg02+B*B)-4.0*x*B*omg02*T*T*T+omg02*omg02*T*T*T*T;
				*(a+i*5+3)=-64+32*x*B*T-8*x*B*omg02*T*T*T+4.0*omg02*omg02*T*T*T*T;  
				*(a+i*5+2)=96.0-8.0*T*T*(2*omg02+B*B)+6.0*omg02*omg02*T*T*T*T;
				*(a+i*5+1)=-64-32*x*B*T+8*x*B*omg02*T*T*T+4.0*omg02*omg02*T*T*T*T;
				*(a+i*5)=16.0+16.0*x*B*T+8.0*omg02*T*T+4.0*T*T*B*B+4.0*x*B*T*T*T*omg02+omg02*omg02*T*T*T*T;
				//分子
				*(b+i*5+4)=4.0*T*T*B*B;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=-8.0*T*T*B*B;
				*(b+i*5+1)=0.0;
				*(b+i*5)=4.0*T*T*B*B;

				//将求得的分母系数，最高的变成1
				*(a+i*5+3)/=*(a+i*5+4);
				*(a+i*5+2)/=*(a+i*5+4);
				*(a+i*5+1)/=*(a+i*5+4);
				*(a+i*5)/=*(a+i*5+4);
				*(b+i*5+4)/=*(a+i*5+4);
				*(b+i*5+3)/=*(a+i*5+4);
				*(b+i*5+2)/=*(a+i*5+4);
				*(b+i*5+1)/=*(a+i*5+4);
				*(b+i*5)/=*(a+i*5+4);
				*(a+i*5+4)=1; 
			}
		}
		break;
	case 4:
		omg1=2/T*tan(pi*f4/fs);    //2/T*tan(2*pi*f1/fs/2);
	    omg2=2/T*tan(pi*f1/fs);
		B=omg1-omg2;
		omg02=omg1*omg2;

		for(i=0;i<(N/2+1)/2;i++)
		{
			x=cos(pi*(0.5+((double)2*(double)i+(double)1)/((double)2*(double)N/2)));
	        //y=sin(pi*(0.5+((double)2*(double)i+(double)1)/((double)2*(double)N/2)));

			if((N/2%2!=0)&&(i==(N/2-1)/2))
			{    //分母 
				*(a+i*5+4)=4.0+2.0*T*B+omg02*T*T;
				*(a+i*5+3)=2.0*omg02*T*T-8.0;
				*(a+i*5+2)=4.0-2.0*T*B+omg02*T*T;
				*(a+i*5+1)=0.0;
				*(a+i*5)=0.0;  
				//分子
				*(b+i*5+4)=4.0+omg02*T*T;
				*(b+i*5+3)=2.0*omg02*T*T-8.0;
				*(b+i*5+2)=4.0+omg02*T*T;
				*(b+i*5+1)=0.0;
				*(b+i*5)=0.0;

				//将求得的分母系数，最高的变成1
				*(a+i*5+3)/=*(a+i*5+4);
				*(a+i*5+2)/=*(a+i*5+4);
				*(a+i*5+1)/=*(a+i*5+4);
				*(a+i*5)/=*(a+i*5+4);
				*(b+i*5+4)/=*(a+i*5+4);
				*(b+i*5+3)/=*(a+i*5+4);
				*(b+i*5+2)/=*(a+i*5+4);
				*(b+i*5+1)/=*(a+i*5+4);
				*(b+i*5)/=*(a+i*5+4);
				*(a+i*5+4)=1; 
			}
			else
			{  //分母 
				*(a+i*5+4)=16.0-16.0*x*B*T+4.0*T*T*(2*omg02+B*B)-4.0*x*B*omg02*T*T*T+omg02*omg02*T*T*T*T;
				*(a+i*5+3)=-64+32*x*B*T-8*x*B*omg02*T*T*T+4.0*omg02*omg02*T*T*T*T;  
				*(a+i*5+2)=96.0-8.0*T*T*(2*omg02+B*B)+6.0*omg02*omg02*T*T*T*T;
				*(a+i*5+1)=-64-32*x*B*T+8*x*B*omg02*T*T*T+4.0*omg02*omg02*T*T*T*T;
				*(a+i*5)=16.0+16.0*x*B*T+8.0*omg02*T*T+4.0*T*T*B*B+4.0*x*B*T*T*T*omg02+omg02*omg02*T*T*T*T;

				//分子
				*(b+i*5+4)=16.0+8.0*omg02*T*T+omg02*omg02*T*T*T*T;
				*(b+i*5+3)=-64+4.0*omg02*omg02*T*T*T*T;
				*(b+i*5+2)=96.0-16.0*T*T*omg02+6.0*omg02*omg02*T*T*T*T;
				*(b+i*5+1)=-64+4.0*omg02*omg02*T*T*T*T;
				*(b+i*5)=16.0+8.0*omg02*T*T+omg02*omg02*T*T*T*T;

				//将求得的分母系数，最高的变成1
				*(a+i*5+3)/=*(a+i*5+4);
				*(a+i*5+2)/=*(a+i*5+4);
				*(a+i*5+1)/=*(a+i*5+4);
				*(a+i*5)/=*(a+i*5+4);
				*(b+i*5+4)/=*(a+i*5+4);
				*(b+i*5+3)/=*(a+i*5+4);
				*(b+i*5+2)/=*(a+i*5+4);
				*(b+i*5+1)/=*(a+i*5+4);
				*(b+i*5)/=*(a+i*5+4);
				*(a+i*5+4)=1; 
			}
		}
		break;
	}
	return N;  //?????????????????
}
/*----------------------根据数字指标设计模拟的滤波器----------------------------------
*/
double Cal_Btw_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a)
{
	int i,num;
    double pi;
	double B,omg02,omg1,omg2;
	pi=4.0*atan(1.0);
	
	switch(band)
	{
	case 1:  //低通p=s/omgc
		//如果没有给出omgc，我们计算
	    if(omgc<=0)
	    {
	       f1=2/T*tan(pi*f1/fs);    //2/T*tan(2*pi*f1/fs/2);
	       omgc=f1*pow((pow(10,0.1*ap)-1),-1.0/(double)(2*N)); //??????????????????????
		   //f2=2/T*tan(pi*f2/fs);
	       //omgc=f2* pow((pow(10,0.1*as)-1),-1.0/(double)(2*N));
	    }
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_Btw_xishu(N, i, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				*(a+i*3+1)=1.0;
				*(a+i*3)=omgc;  
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=0.0;
				*(b+i*3)=omgc;
			}
			else
			{   //分母
				*(a+i*3+1)=*(a+i*3+1)*omgc;
				*(a+i*3)=*(a+i*3)*omgc*omgc;
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=0.0;
				*(b+i*3)=omgc*omgc;
			}
		}
		break;
	case 2:  //高通p=omgc/s
		if(omgc<=0)
	    {
	       f2=2/T*tan(pi*f2/fs);    //2/T*tan(2*pi*f1/fs/2);
		   //f2=1/f2;
	       omgc=f2*pow((pow(10,0.1*ap)-1),-1.0/(double)(2*N)); //??????????????????????
		   //f1=2/T*tan(pi*f1/fs);
		   //f1=1/f1;
	       //omgc=f1* pow((pow(10,0.1*as)-1),-1.0/(double)(2*N));
	    }
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_Btw_xishu(N, i, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				*(a+i*3+1)=1.0;
				*(a+i*3)=omgc;  
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=1.0;
				*(b+i*3)=0.0;
			}
			else
			{   //分母
				*(a+i*3+1)=*(a+i*3+1)*omgc;
				*(a+i*3)=*(a+i*3)*omgc*omgc;
				//分子
				*(b+i*3+2)=1.0;
				*(b+i*3+1)=0.0;
				*(b+i*3)=0.0;
			}
		}
		break;
	case 3:
		assert(N%2==0);
		//求omgc用于去归一化，利用omgp=1和求得的N
	    //omgc=pow((pow(10,0.1*ap)-1),-1.0/(double)(N)); //omgp=1, 2*N/2 ??????????????????????

		omg1=2/T*tan(pi*f3/fs);    //2/T*tan(2*pi*f1/fs/2);
	    omg2=2/T*tan(pi*f2/fs);
		B=omg1-omg2;
		omg02=omg1*omg2;

		for(i=0;i<(N/2+1)/2;i++)
		{
			num=Cal_Btw_xishu(N/2, i, b+i*5+2, a+i*5+2); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=1.0;
				*(a+i*5+1)=B;
				*(a+i*5)=omg02;  
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=0.0;
				*(b+i*5+1)=B;
				*(b+i*5)=0.0;
			}
			else
			{   //分母
				*(a+i*5)=omg02*omg02;
				*(a+i*5+1)=(*(a+i*5+3))*B*omg02;
				*(a+i*5+2)=B*B+2*omg02;
				*(a+i*5+3)=(*(a+i*5+3))*B;
				*(a+i*5+4)=1.0;
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=B*B;
				*(b+i*5+1)=0.0;
				*(b+i*5)=0.0;
			}
		}
		break;
	case 4:
		assert(N%2==0);
		omg1=2/T*tan(pi*f4/fs);    //2/T*tan(2*pi*f1/fs/2);
	    omg2=2/T*tan(pi*f1/fs);
		B=omg1-omg2;
		omg02=omg1*omg2;
		for(i=0;i<(N/2+1)/2;i++)
		{
			num=Cal_Btw_xishu(N/2, i, b+i*5+2, a+i*5+2); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=1.0;
				*(a+i*5+1)=B;
				*(a+i*5)=omg02;  
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=1.0;
				*(b+i*5+1)=0.0;
				*(b+i*5)=omg02;
			}
			else
			{   //分母
				*(a+i*5)=omg02*omg02;
				*(a+i*5+1)=(*(a+i*5+3))*B*omg02;
				*(a+i*5+2)=B*B+2*omg02;
				*(a+i*5+3)=(*(a+i*5+3))*B;
				*(a+i*5+4)=1.0;
							
				//分子
				*(b+i*5+4)=1.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=2*omg02;
				*(b+i*5+1)=0.0;
				*(b+i*5)=omg02*omg02;
			}
		}
		break;
	}
	return 1.0;  
}

/*--------------------将模拟滤波器用双线性变化法转换成数字滤波器----------------------
int      band------------滤波器类型
double*  b---------------双精度实型二位数组，体积为(N+1)/2*(n+1)。存放滤波器分子多项式的系数。
                         b[j][i]表示第j个n阶节的分子多项式的第i个系数，最后存放转换后的系数
double*  a---------------双精度二位数组，体积为(N+1)/2*(n+1)。存放滤波器分母多项式的系数。
                         a[j][i]表示第jj个n阶节的分母多项式的第i个系数，最后存放转换后的系数
		    其中n，是每节滤波器的阶数，低通和高通为2，带通和带阻为4
int      N---------------滤波器的阶数N。
double   T---------------采样间隔
*/
char hstohz_shxx_each(int n, double* b, double* a, double T)
{
	double a1,a2,a3,a4,a5;
	switch(n)
	{
	case 1:
		a1=b[0];
		a2=b[1];
		a3=b[2];   //阶数最高位
		b[0]=0.0;
		b[1]=a1*T-2.0*a2;
		b[2]=2.0*a2+a1*T;

		a1=a[0];
		a2=a[1];
		a3=a[2];
		a[0]=0.0;
		a[1]=a1*T-2.0*a2;
		a[2]=2.0*a2+a1*T;

		//将求得的分母系数，最高的变成1
		*(a+1)/=*(a+2);
		*(a)/=*(a+2);
		*(b+2)/=*(a+2);
		*(b+1)/=*(a+2);
		*(b)/=*(a+2);
		*(a+2)=1; 
		break;
	case 2:
		a1=b[0];
		a2=b[1];
		a3=b[2];
		b[0]=4.0*a3-2.0*T*a2+T*T*a1;
		b[1]=-8.0*a3+2*T*T*a1;
		b[2]=4.0*a3+2.0*T*a2+T*T*a1;

		a1=a[0];
		a2=a[1];
		a3=a[2];
		a[0]=4.0*a3-2.0*T*a2+T*T*a1;
		a[1]=-8.0*a3+2*T*T*a1;
		a[2]=4.0*a3+2.0*T*a2+T*T*a1;

	    //将求得的分母系数，最高的变成1
		*(a+1)/=*(a+2);
		*(a)/=*(a+2);
		*(b+2)/=*(a+2);
		*(b+1)/=*(a+2);
		*(b)/=*(a+2);
		*(a+2)=1; 
		break;
	case 4:
		a1=b[0];
		a2=b[1];
		a3=b[2];
	    a4=b[3];
	    a5=b[4];
		b[0]=a1*T*T*T*T-2.0*a2*T*T*T+4.0*a3*T*T-8.0*a4*T+16*a5;
		b[1]=4.0*a1*T*T*T*T-4.0*a2*T*T*T+16.0*a4*T-64*a5;
		b[2]=6.0*a1*T*T*T*T-8.0*a3*T*T+96*a5;
	    b[3]=4.0*a1*T*T*T*T+4.0*a2*T*T*T-16.0*a4*T-64*a5; 
	    b[4]=a1*T*T*T*T+2.0*a2*T*T*T+4.0*a3*T*T+8.0*a4*T+16*a5;

		a1=a[0];
		a2=a[1];
		a3=a[2];
		a4=a[3];
		a5=a[4];
		a[0]=a1*T*T*T*T-2.0*a2*T*T*T+4.0*a3*T*T-8.0*a4*T+16*a5;
		a[1]=4.0*a1*T*T*T*T-4.0*a2*T*T*T+16.0*a4*T-64*a5;
	    a[2]=6.0*a1*T*T*T*T-8.0*a3*T*T+96*a5;
	    a[3]=4.0*a1*T*T*T*T+4.0*a2*T*T*T-16.0*a4*T-64*a5; 
	    a[4]=a1*T*T*T*T+2.0*a2*T*T*T+4.0*a3*T*T+8.0*a4*T+16*a5;

	    //将求得的分母系数，最高的变成1
		*(a+3)/=*(a+4);
		*(a+2)/=*(a+4);
		*(a+1)/=*(a+4);
		*(a)/=*(a+4);
		*(b+4)/=*(a+4);
		*(b+3)/=*(a+4);
		*(b+2)/=*(a+4);
		*(b+1)/=*(a+4);
		*(b)/=*(a+4);
		*(a+4)=1; 
		break;
	}
	return 1;
}
int hstohz_shxx(int ns, int n, double* b, double* a, double T)
{
    int i;
	if(n==2)
	{
		for(i=0;i<ns;i++)
	   {   
		   if(fabs(*(a+i*3+2))<0.000000001)
		   { 
			   if(hstohz_shxx_each(1, b+i*3, a+i*3, T)==-1)
				   return -1;
		   }
		   else
		   {   
			   if(hstohz_shxx_each(2, b+i*3, a+i*3, T)==-1)
				   return -1;
		   }
	    }
	}
	else if(n==4)
	{
		for(i=0;i<ns;i++)
	   {
		   if((fabs(*(a+i*5+3))<0.000000001)&&(fabs(*(a+i*5+4))<0.000000001))
		   {   	
			    if(hstohz_shxx_each(2, b+i*5, a+i*5, T)==-1)
				   return -1;
		   }
		   else
		   {   
			    if(hstohz_shxx_each(4, b+i*5, a+i*5, T)==-1)
				   return -1;
		   }
		}
	}
	return 1;
}
double Cal_Btw_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double omgc, double *b, double *a)
{
	if(bianhuan==0)
		return Cal_Btw_Hs_mch(band, N, T, fs, f1, f2, f3, f4, ap, as, omgc, b, a);
	else
        return Cal_Btw_Hs_shxx(band, N, T, fs, f1, f2, f3, f4, ap, as, omgc, b, a);
}
int hstohz(char bianhuan, int ns, int n,  double* b, double* a, double T)
{
	if(bianhuan==0)
		return hstohz_mch(ns, n, b, a, T);
	else
        return hstohz_shxx(ns, n, b, a, T);
}
/*-------------------------------------计算级联形式滤波器的零点和极点----------------------------------------
band    --------滤波器的类型
N       --------求的滤波器的阶数（低通、高通为N，带通带阻2N 为其实际的阶数
fenmu   --------求得滤波器的分母系数（以级联的形式给出）
zeros   --------求得的零点数组（低通和高通为N，带通和带阻为2N）
poles_re -------极点实部的数组（低通和高通为N，带通和带阻为2N）
poles_im -------极点虚部的数组（低通和高通为N，带通和带阻为2N）
zeros_n，ploes_n   ------以后可能使用
*/
char poles(int band, int N, double *fenmu, double *poles_re, double *poles_im)
{
	int i;
	
	switch(band)
	{
	case 1:
		{
			for(i=0;i<N/2;i++)
		    {
	            if(qrrt(2, fenmu+i*3, poles_re+i*2, poles_im+i*2, 0.00001, 100)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		           return 0;
		    }
			if(N%2!=0)
			{
				*(poles_re+i*2)=-*(fenmu+i*3+1);
                *(poles_im+i*2)=0.0;
			}
			break;
		}
	case 2:
		{
			for(i=0;i<N/2;i++)
		    {
	            if(qrrt(2, fenmu+i*3, poles_re+i*2, poles_im+i*2, 0.00001, 100)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		           return 0;
		    }
			if(N%2!=0)
			{
				*(poles_re+i*2)=-*(fenmu+i*3+1);
                *(poles_im+i*2)=0.0;
			}
			break;
		}
	case 3:
	case 4:
		{
		    for(i=0;i<N/4;i++)
		    {
	            if(qrrt(4, fenmu+i*5, poles_re+i*4, poles_im+i*4, 0.00001, 100)<0)  //分母乘以z^4后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		           return 0;
		    }
			if(N/2%2!=0)
			{
				if(qrrt(2, fenmu+i*5+2, poles_re+i*4, poles_im+i*4, 0.00001, 100)<0)  //+2是因为此时z-^4和z-^3为0，也就是分母乘以z^4后，对应的z幂低的系数为0
		          return 0;
			}
			break;
		}
	}
	return 1;
}
/*--------------------------------直接型极零点------------------------------------
double *fenmu---------双精度实型数组，体积为n+1。存放滤波器分母多项式的系数
int    n----------整型变量。滤波器的阶数。

返回实际的极点数目

注意：这种方法比较简便，但是，当阶数很大时，利用qrrt求得的值就不再准确了
*/
int polezeros_zhj(int n, double *fenmu, double *poles_re, double *poles_im)
{
	int i,j,k;
	for(i=0;i<n;i++)
	{
		if(fabs(fenmu[i])>0.00000001)
			break;
	}
	for(j=0;j<n;j++)
	{
		if(fabs(fenmu[n-j])>0.00000001)
			break;
	}
	if(n<=i+j)
		return -1;
	if(qrrt(n-i-j, fenmu+i, poles_re, poles_im, 0.000001, 300)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		   return -1;
	 //为0
	for(k=0;k<j;k++)
	{
          poles_re[n-i-j+k]=0.0;
		  poles_im[n-i-j+k]=0.0;
	}

	return n-i;
}
/*--------------------------------级联型的极点-------------------------------
double *fenmu---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分母多项式的系数
int    n----------整型变量。滤波器每节的阶数。
int   ns----------整型变量。滤波器n阶节数：L

返回实际的极点数目
*/
int poles_jl(int ns, int n, double *fenmu, double *poles_re, double *poles_im)
{
	//先将级联变成直接的形式，再调用直接型的求极点
	////-------------本来这种方法比较简便，但是，当阶数很大时，利用qrrt求得的值就不再准确了
	/*int i,num;
    double *temp=malloc((n*ns+1)*sizeof(double));
	double *bb=malloc((n*ns+1)*sizeof(double));

	if(ns==1)
       memcpy(bb, fenmu, (n+1)*sizeof(double));
	else
	{
	   duoxiangshi_mul(fenmu, fenmu+n+1, n, n, bb);
	   for(i=2;i<ns;i++)
	  {
		  duoxiangshi_mul(bb, fenmu+i*(n+1), i*n, n,temp);
		  memcpy(bb, temp, ((i+1)*n+1)*sizeof(double));
	   }
	}

	num=polezeros_zhj(n*ns, bb, poles_re, poles_im);
	free(temp);
	free(bb);
	return num;*/
	int num=0,i,l=0,r=0,k;
	if(n<3)
	{
		for(i=0;i<ns;i++)
		{
			for(l=0;l<3;l++)
			{
				if(fabs(*(fenmu+i*3+l))>0.00000001)
					break;
			}
			for(r=0;r<3;r++)
			{
				if(fabs(*(fenmu+i*3+2-r))>0.00000001)
					break;
			}
            if(l+r==2)
				continue;
            else if(l+r>=3)
				return -1;
           
			if(qrrt(2-l-r, fenmu+i*3+l, poles_re+num, poles_im+num, 0.000001, 200)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
			num+=2-r-l;
            for(k=0;k<r;k++)
			{
				poles_re[num+k]=0.0;
                poles_im[num+k]=0.0;
			}
			num+=r;
		}
	}
	else if(n<5)
	{
		for(i=0;i<ns;i++)
		{
			for(l=0;l<5;l++)
			{
				if(fabs(*(fenmu+i*5+l))>0.00000001)
					break;
			}
			for(r=0;r<5;r++)
			{
				if(fabs(*(fenmu+i*5+4-r))>0.00000001)
					break;
			}
            if(l+r==4)
				continue;
			else if(l+r>=5)
				return -1;
           
			if(qrrt(4-l-r, fenmu+i*5+l, poles_re+num, poles_im+num, 0.000001, 200)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
            num+=4-r-l;
            for(k=0;k<r;k++)
			{
				poles_re[num+k]=0.0;
                poles_im[num+k]=0.0;
			}
			num+=r;
		}
	}
	else
	    return -1;
	return num;
}
/*--------------------------------并联型的极点-------------------------------
double *fenmu---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分母多项式的系数
int    n----------整型变量。滤波器每节的阶数。
int   ns----------整型变量。滤波器n阶节数：L

返回实际的极点数目
*/
int poles_bl(int ns, int n, double *fenmu, double *poles_re, double *poles_im)
{
	//先将并联变成直接的形式，再调用直接型的求极点
	////-------------本来这种方法比较简便，但是，当阶数很大时，利用qrrt求得的值就不再准确了
	/*int i,num;
    double *temp=malloc((n*ns+1)*sizeof(double));
	double *bb=malloc((n*ns+1)*sizeof(double));

	if(ns==1)
       memcpy(bb, fenmu, (n+1)*sizeof(double));
	else
	{
	   duoxiangshi_mul(fenmu, fenmu+n+1, n, n, bb);
	   for(i=2;i<ns;i++)
	  {
		  duoxiangshi_mul(bb, fenmu+i*(n+1), i*n, n,temp);
		  memcpy(bb, temp, ((i+1)*n+1)*sizeof(double));
	   }
	}

	num=polezeros_zhj(n*ns, bb, poles_re, poles_im);
	free(temp);
	free(bb);
	return num;*/
	/*------------因为并联和级联的分母去得的极点是相同的，故我们可以和级联的算法一样
	*/
	int num=0,i,l=0,r=0,k;
	if(n<3)
	{
		for(i=0;i<ns;i++)
		{
			for(l=0;l<3;l++)
			{
				if(fabs(*(fenmu+i*3+l))>0.00000001)
					break;
			}
			for(r=0;r<3;r++)
			{
				if(fabs(*(fenmu+i*3+2-r))>0.00000001)
					break;
			}
            if(l+r==2)
				continue;
            else if(l+r>=3)
				return -1;
           
			if(qrrt(2-l-r, fenmu+i*3+l, poles_re+num, poles_im+num, 0.000001, 300)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
			 num+=2-r-l;
            for(k=0;k<r;k++)
			{
				poles_re[num+k]=0.0;
                poles_im[num+k]=0.0;
			}
			num+=r;
		}
	}
	else if(n<5)
	{
		for(i=0;i<ns;i++)
		{
			for(l=0;l<5;l++)
			{
				if(fabs(*(fenmu+i*5+l))>0.00000001)
					break;
			}
			for(r=0;r<5;r++)
			{
				if(fabs(*(fenmu+i*5+4-r))>0.00000001)
					break;
			}
            if(l+r==4)
				continue;
			else if(l+r>=5)
				return -1;
           
			if(qrrt(4-l-r, fenmu+i*5+l, poles_re+num, poles_im+num, 0.000001, 300)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
            num+=4-r-l;
            for(k=0;k<r;k++)
			{
				poles_re[num+k]=0.0;
                poles_im[num+k]=0.0;
			}
			num+=r;
		}
	}
	else
	    return -1;
	return num;
}
/*--------------------------------级联型的零点-------------------------------
double *fenzi---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分子多项式的系数
int    n----------整型变量。滤波器每节的阶数。
int   ns----------整型变量。滤波器n阶节数：L

返回实际的零点数目
*/
int zeros_jl(int ns, int n, double *fenzi, double *zeros_re, double *zeros_im)
{
	int num=0,i,l,r,k;
	if(n<3)
	{
		for(i=0;i<ns;i++)
		{
			for(l=0;l<3;l++)
			{
				if(fabs(*(fenzi+i*3+l))>0.00000001)
					break;
			}
			for(r=0;r<3;r++)
			{
				if(fabs(*(fenzi+i*3+2-r))>0.00000001)
					break;
			}
			if(l+r==2)
				continue;
            else if(l+r>=3)
				return -1;
           
			if(qrrt(2-l-r, fenzi+i*3+l, zeros_re+num, zeros_im+num, 0.00001, 100)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
            num+=2-r-l;
            for(k=0;k<r;k++)
			{
				zeros_re[num+k]=0.0;
                zeros_im[num+k]=0.0;
			}
			num+=r;
		}

		/*for(i=0;i<ns;i++)
		{
			if(*(fenzi+i*3)<0.00000001)
			{
				if(*(fenzi+i*3+2)>0.00000001)
				{
				   *(zeros_re+num)=-*(fenzi+i*3+1)/(*(fenzi+i*3+2));
                   *(zeros_im+num)=0.0;
				   num++;
				}
			}
			else  //此处我们假定了z^0也就是数组系数的最高位那个不为0
			{
	            if(qrrt(2, fenzi+i*3, zeros_re+num, zeros_im+num, 0.00001, 100)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
				num+=2;
			}
		}*/
	}
	else if(n<5)
	{
		for(i=0;i<ns;i++)
		{
			for(l=0;l<5;l++)
			{
				if(fabs(*(fenzi+i*5+l))>0.00000001)
					break;
			}
			for(r=0;r<5;r++)
			{
				if(fabs(*(fenzi+i*5+4-r))>0.00000001)
					break;
			}
            if(l+r==4)
				continue;
			else if(l+r>=5)
				return -1;
           
			if(qrrt(4-l-r, fenzi+i*5+l, zeros_re+num, zeros_im+num, 0.00001, 100)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
            num+=4-r-l;
            for(k=0;k<r;k++)
			{
				zeros_re[num+k]=0.0;
                zeros_im[num+k]=0.0;
			}
			num+=r;
		}

		/*for(i=0;i<ns;i++)
		{
			if((*(fenzi+i*5)<0.00000001)&&(*(fenzi+i*5+1)<0.00000001)&&(*(fenzi+i*5+2)<0.00000001)&&(*(fenzi+i*5+3)<0.00000001))
			{
				//if(*(fenzi+i*5+4)>0.00000001)
				//{
				  // *(poles_re+num)=0.0;
                  // *(poles_im+num)=0.0;
				  // num++;
				//}
				;
			}
			else if((*(fenzi+i*5)<0.00000001)&&(*(fenzi+i*5+1)<0.00000001)&&(*(fenzi+i*5+2)<0.00000001))
			{
				if(*(fenzi+i*5+4)>0.00000001)
				{
				   *(zeros_re+num)=-*(fenzi+i*5+3)/(*(fenzi+i*5+4));
                   *(zeros_im+num)=0.0;
				   num++;
				}
			}
			else if((*(fenzi+i*5)<0.00000001)&&(*(fenzi+i*5+1)<0.00000001))  //此处我们假定了z^0也就是数组系数的最高位那个不为0
			{
				if(qrrt(2, fenzi+i*5+2, zeros_re+num, zeros_im+num, 0.00001, 100)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
				num+=2;
			}
			else if(*(fenzi+i*5)<0.00000001) //此处我们假定了z^0也就是数组系数的最高位那个不为0
			{
				if(qrrt(3, fenzi+i*5+1, zeros_re+num, zeros_im+num, 0.00001, 100)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
				num+=3; 
			}
			else  //此处我们假定了z^0也就是数组系数的最高位那个不为0
			{
	            if(qrrt(4, fenzi+i*5, zeros_re+num, zeros_im+num, 0.00001, 100)<0)  //分母乘以z^2后，对应的数组的低字节正好是z的幂高的系数，正好与qrrt顺序相同
		            return -1;
				num+=4;
			}
		}*/
	}
	else
	    return -1;
	return num;
}
/*--------------------------------并联型的零点-------------------------------
double *fenzi---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分子多项式的系数
int    n----------整型变量。滤波器每节的阶数。
int   ns----------整型变量。滤波器n阶节数：L

返回实际的零点数目
*/
int zeros_bl(int ns, int n, double *fenzi, double *femu, double *zeros_re, double *zeros_im)
{
	//先将并联变成直接的形式，再调用直接型的求极点
	////-------------本来这种方法比较简便，但是，当阶数很大时，利用qrrt求得的值就不再准确了
	int num;
    double *temp=malloc((n*ns+1)*sizeof(double));
	double *bb=malloc((n*ns+1)*sizeof(double));
	double *aa=malloc((n*ns+1)*sizeof(double));

	bltozhj(ns, n, fenzi, femu, bb, aa);

	num=polezeros_zhj(n*ns, bb, zeros_re, zeros_im);

	free(temp);
	free(aa);
	free(bb);
	return num;
}
/*----------------------------------用于将园外的零点调入园内------------------------------*/
void zoroschange_bl(double *K, int ns, int n, double *fenzi, double *femu)
{
	char msgbuf[100];
	int i,num,realnum,j;
	double *zeros_re, *zeros_im, b[3],a[3];

	if(n>=5) return;
	num=n*ns;
	zeros_re=malloc(num*sizeof(double));
	zeros_im=malloc(num*sizeof(double));
   // bb=malloc((num+1)*sizeof(double));
	//bbtemp=malloc((num+1)*sizeof(double));

    //将大于1的零点移入园内
	realnum=zeros_bl(ns, n, fenzi, femu, zeros_re, zeros_im);

	if(*K==0.0)
		*K=1.0;
	OutputDebugString("\n");
	for(i=0;i<realnum;i++)
	{
		if((fabs(zeros_re[i])>1.0))
		{
			if(zeros_im[i]<0.000000001)
	      	{
	            sprintf(msgbuf, "%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);
				*K/=zeros_re[i];                 sprintf(msgbuf, "%f\n", *K);    OutputDebugString(msgbuf);
                zeros_re[i]=1/zeros_re[i];  sprintf(msgbuf, "变化后：%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);
    		}
			else
			{
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i+1], zeros_im[i+1]);    OutputDebugString(msgbuf);
				*K/=zeros_re[i]*zeros_re[i]+zeros_im[i]*zeros_im[i];     sprintf(msgbuf, "%f\n", *K);    OutputDebugString(msgbuf);
				cplx_daoshu(zeros_re+i, zeros_im+i);       sprintf(msgbuf, "变化后：%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);
				cplx_daoshu(zeros_re+i+1, zeros_im+i+1);   sprintf(msgbuf, "变化后：%f+j%f\n", zeros_re[i+1], zeros_im[i+1]);    OutputDebugString(msgbuf);
				i++;
			}
		}
		else if(fabs(zeros_im[i])>1.0)
		{
			sprintf(msgbuf, "%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);
			sprintf(msgbuf, "%f+j%f\n", zeros_re[i+1], zeros_im[i+1]);    OutputDebugString(msgbuf);
			*K/=zeros_re[i]*zeros_re[i]+zeros_im[i]*zeros_im[i];   sprintf(msgbuf, "%f\n", *K);    OutputDebugString(msgbuf);
			cplx_daoshu(zeros_re+i, zeros_im+i);               sprintf(msgbuf, "变化后：%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);
			cplx_daoshu(zeros_re+i+1, zeros_im+i+1);           sprintf(msgbuf, "变化后：%f+j%f\n", zeros_re[i+1], zeros_im[i+1]);    OutputDebugString(msgbuf);
			i++;
		}
	}
   /* //将变后的零点变成直接形式
	i=0;
	if(zeros_im[i]<0.000000001)
	{
		b[0]=0.0; b[1]=-zeros_re[i]; b[2]=1.0;
		i++;
	}
	else if((zeros_re[i]==zeros_re[i+1])&&(zeros_im[i]=-zeros_im[i+1]))
	{
		b[0]=zeros_re[i]*zeros_re[i]+zeros_im[i]*zeros_im[i]; b[1]=-2*zeros_re[i]; b[2]=1.0;
		i+=2;
	}
	else
		return -1;

	bbtemp[0]=b[0];  bbtemp[1]=b[1];   bbtemp[2]=b[2];
    j=2;
    for(;i<realnum;i++)
	{
		if(zeros_im[i]<0.000000001)
    	{
     		b[0]=0.0; b[1]=-zeros_re[i]; b[2]=1.0;
     		i++;
    	}
    	else if((zeros_re[i]==zeros_re[i+1])&&(zeros_im[i]=-zeros_im[i+1]))
      	{
    		b[0]=zeros_re[i]*zeros_re[i]+zeros_im[i]*zeros_im[i]; b[1]=-2*zeros_re[i]; b[2]=1.0;
	    	i+=2;
    	}
    	else
	     	return -1;
		duoxiangshi_mul(2, j, b, bbtemp, bb);
		j+=2;
		memcpy(bbtemp, bb, (j+1)*sizeof(double));
	}*/
   //将零点变成级联
	
	if(n<3)
	{
		OutputDebugString("\n");
    	for(i=0,j=0;(i<realnum)&&(j<ns);i++,j++)
		{
			if(zeros_im[i]<0.000000001)
			{
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);

				fenzi[j*3]=0.0;
				fenzi[j*3+1]=-zeros_re[i];
				fenzi[j*3+2]=1.0;
			}
			else  //利用零点共轭对称出现
			{
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i+1], zeros_im[i+1]);    OutputDebugString(msgbuf);

				fenzi[j*3]=zeros_re[i]*zeros_re[i]+zeros_im[i]*zeros_im[i];
				fenzi[j*3+1]=-2*zeros_re[i];
				fenzi[j*3+2]=1.0;
				i++;
			}	
		}
	}
	else if(n<5)
	{
		OutputDebugString("\n");
		for(i=0,j=0;(i<realnum)&&(j<ns);)
		{
			if(zeros_im[i]<0.000000001)
			{
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);

				b[j*3]=0.0;
				b[j*3+1]=-zeros_re[i];
				b[j*3+2]=1.0;
				i++;
			}
			else  //利用零点共轭对称出现
			{
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i+1], zeros_im[i+1]);    OutputDebugString(msgbuf);

				b[j*3]=zeros_re[i]*zeros_re[i]+zeros_im[i]*zeros_im[i];
				b[j*3+1]=-2*zeros_re[i];
				b[j*3+2]=1.0;
				i+=2;
			}	
			if(zeros_im[i]<0.000000001)
			{
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);

				a[0]=0.0;
				a[1]=-zeros_re[i];
				a[2]=1.0;
				i++;
			}
			else  //利用零点共轭对称出现
			{
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i], zeros_im[i]);    OutputDebugString(msgbuf);
				sprintf(msgbuf, "%f+j%f\n", zeros_re[i+1], zeros_im[i+1]);    OutputDebugString(msgbuf);

				a[0]=zeros_re[i]*zeros_re[i]+zeros_im[i]*zeros_im[i];
				a[1]=-2*zeros_re[i];
				a[2]=1.0;
				i+=2;
			}
			duoxiangshi_mul(2, 2, a, b, fenzi+j*5);
			j++;
		}
	}



	free(zeros_re);
	free(zeros_im);
	//free(bb);
	//free(bbtemp);
}
//----------------------------------------------------------------------------幅频、相频-------------------------------------------------------------------------//
//--------------计算直接型数字滤波器的幅频和相频响应---------------------------//
/*
            b(0)+b(1)z^(-1)+ ... + b(lb)z^(-lb)
       H(z)=------------------------------------
            1.0 +a(1)z^(-1)+ ... + a(la)z^(-la)
/*
double *b------双精度实型二位数组，体积为(m+1)。存放滤波器分子多项式的系数
double *a------双精度实型二位数组，体积为(m+1)。存放滤波器分母多项式的系数
int    m-------滤波器分子多项式的阶数
int    n-------滤波器分子多项式的阶数
double *x------双精度实型一维数组，长度为len。
               当sign=0时，存放滤波器频率响应|H(w)|,
			   sign=1时，存放分贝表示的滤波器幅频响应|H(w)|,
			   sign=2时，存放滤波器的相频响应fai(omg)
int   len------整型变量。频率响应的长度。
int   sign-----整型变量。
*/
void Hz_zhj_hejw(double K, double *b, double *a, int m, int n, double *x, int len, int sign)
{
    int i,j;
	double bre,bim,are,aim,zr,zi,freq,pi,absa, tempim;
	pi=4.0*atan(1.0);

	for(i=0;i<len;i++)
	{
		freq=i*0.5/(len-1);
		zr=cos(-2.0*pi*freq);
		zi=sin(-2.0*pi*freq);
		bre=b[0];
		bim=0.0;
		for(j=1;j<m;j++)
		{
			bre+=b[j]*zr;
			bim+=b[j]*zi;
		}
		are=1.0;
		aim=0.0;
		for(j=1;j<n;j++)
		{
			are+=a[j]*zr;
			aim+=a[j]*zi;
		}
		absa=are*are+aim*aim;
		x[i]=are*bre+bim*aim;
		tempim=bim*are-bre*aim;
		x[i]/=absa;
		tempim/=absa; //存储虚部

		x[i]*=K;
		tempim*=K;

        if(sign==0)
		{
			x[i]=sqrt(x[i]*x[i]+tempim*tempim);
		}
		else if(sign==1)
		{
			x[i]=sqrt(x[i]*x[i]+tempim*tempim);
			if(x[i]==0&&i>=1)
                x[i]=x[i-1];//防止突变，故不赋值x[k]=10.0*log10(1.0e-40);
			else if(x[i]==0&&i==0)
				x[i]=10.0*log10(1.0e-10);
			else
                x[i]=10.0*log10(x[i]);
		}
		else if(sign==2)
		{
			if(x[i]==0&&i>=1)
			   x[i]=x[i-1];  //防止突变，故不赋值给0
			else if(x[i]==0&&i==0)
				x[i]=0.0;
			else
                x[i]=atan2(tempim,x[i]);
		}
	}
}

//--------------计算级联型数字滤波器的幅频和相频响应--------------------//
/*
double K-------增益K
double *b------双精度实型二位数组，体积为ns*(n+1)。存放滤波器分子多项式的系数，
               b[j][i]表示第j个n阶节分子多项式的第i个系数。
double *a------双精度实型二位数组，体积为ns*(n+1)。存放滤波器分母多项式的系数，
               a[j][i]表示第j个n阶节分母多项式的第i个系数。
int    n-------整型变量。级联型滤波器每节的阶数。   b(0)+b(1)z^(-1)+ b(2)z^(2) 则为2
int   ns-------整型变量。级联型滤波器每n阶节数L   
double *x------双精度实型一维数组，长度为len。
               当sign=0时，存放滤波器频率响应|H(w)|,
			   sign=1时，存放分贝表示的滤波器幅频响应|H(w)|,
			   sign=2时，存放滤波器的相频响应fai(omg)
int   len------整型变量。频率响应的长度。
int   sign-----整型变量。
*/
void Hz_jl_hejw(double K, double *b, double *a, int n, int ns, double *x, int len, int sign)
{
	int i,j,k,r,n1,zero_begin;
	double ar,ai,br,bi,zr,zi,freq,tempim,lasttempim,absa,numbr,numbi,numar,numai,temp,pi,pfk0,pfk1;
    n1=n+1;
	pi=4.0*atan(1.0);
    r=0;
	zero_begin=0;
	for(k=0;k<len;k++)
	{
		freq=k*0.5/(len-1);
		x[k]=1.0;

		for(j=0;j<ns;j++)
		{
			br=0.0;
		    bi=0.0;
			for(i=0;i<n1;i++)
			{
				zr=cos(2.0*pi*freq*(n-i));   //可以考虑用数组代替
		        zi=-sin(2.0*pi*freq*(n-i));
				br+=b[j*n1+i]*zr;
				bi+=b[j*n1+i]*zi;
			}

			if(j==0) //如果是第一个值，我们存入numbr，numbi
			{
                numbr=br;
	            numbi=bi;
			}
			else
			{
				temp=numbr*br-numbi*bi;
			    numbi=numbi*br+numbr*bi;
			    numbr=temp;
			}

			ar=0.0;
		    ai=0.0;
			for(i=0;i<n+1;i++)
			{
				zr=cos(2.0*pi*freq*(n-i));
		        zi=-sin(2.0*pi*freq*(n-i));
				ar+=a[j*n1+i]*zr;
				ai+=a[j*n1+i]*zi;
			}

			if(j==0) //如果是第一个值，我们存入numbr，numbi
			{
                numar=ar;
	            numai=ai;
			}
			else
			{
			    temp=numar*ar-numai*ai;
			    numai=numai*ar+numar*ai;
			    numar=temp;
			}
		}
		absa=numar*numar+numai*numai;
		if(absa==0)
		{
			zero_begin++;
			continue;
		}
		else
		{
			x[k]=numar*numbr+numbi*numai;
		    tempim=numbi*numar-numbr*numai;
		    x[k]/=absa;
		    tempim/=absa; //存储虚部
		}

		x[k]=x[k]*K;  //增益
		tempim=tempim*K;

		 if(sign==0)
		{
			x[k]=sqrt(x[k]*x[k]+tempim*tempim);
		}
		else if(sign==1)
		{
			x[k]=sqrt(x[k]*x[k]+tempim*tempim);
			//if((x[k]<(1.0e-200))&&(k>=1))
			if((x[k]==0)&&(k>=1))
                x[k]=x[k-1];//防止突变，故不赋值x[k]=10.0*log10(1.0e-40);
			//else if((x[k]<(1.0e-200))&&(k==0))
			else if((x[k]==0)&&(k==0))
				x[k]=20.0*log10(1.0e-100);
			else
                x[k]=20.0*log10(x[k]);
		}
		else if(sign==2)
		{
			if(k==(zero_begin))
			{
				lasttempim=tempim;
				continue;
			}
			if(x[k]==0&&k>=1)
			    x[k]=x[k-1];  //防止突变，故不赋值给0
			else if(x[k]==0&&k==0)
				x[k]=0.0;
			else
			{
                //x[k]=atan2(tempim,x[k]);

			   if((k-1)>=0)
			   {
				   pfk0= atan2(lasttempim, x[k]);
		           pfk1 = atan2(tempim, x[k]);
			   }
			   if((k-1)>=0 && pfk1>1.50 && pfk0<-1.50)
		      {
			       r++;
			       x[k]=atan2(tempim, x[k])-2*pi*r;
		      }
		      else if((k-1)>=0 && pfk1<-1.50 && pfk0>1.50)
		      {
			      r--;
			      x[k]=atan2(tempim, x[k])-2*pi*r;
		      }
		      else
			      x[k]=atan2(tempim, x[k])-2*pi*r;
			}
		}
	lasttempim=tempim;
	}
	//处理高通开始处全是0的情况
    if(sign==1)
	{
		for(i=0;i<len;i++)
		{
			if(x[i]!=-1000)  //-1000 = 10.0*log10(1.0e-100);
				break;
		}
		for(k=0;k<i;k++)
		    x[k]=x[i];//-i+k;
	}
	for(i=zero_begin;i>=0;i--)
	{
		x[i]=x[zero_begin];
	}
}
//--------------计算并联型数字滤波器的幅频和相频响应--------------------//
/*
double *b------双精度实型二位数组，体积为ns*(n+1)。存放滤波器分子多项式的系数，
               b[j][i]表示第j个n阶节分子多项式的第i个系数。
double *a------双精度实型二位数组，体积为ns*(n+1)。存放滤波器分母多项式的系数，
               a[j][i]表示第j个n阶节分母多项式的第i个系数。
int    n-------整型变量。级联型滤波器每节的阶数。   b(0)+b(1)z^(-1)+ b(2)z^(2) 则为2
int   ns-------整型变量。级联型滤波器每n阶节数L   
double *x------双精度实型一维数组，长度为len。
               当sign=0时，存放滤波器频率响应|H(w)|,
			   sign=1时，存放分贝表示的滤波器幅频响应|H(w)|,
			   sign=2时，存放滤波器的相频响应fai(omg)
int   len------整型变量。频率响应的长度。
int   sign-----整型变量。
*/
void Hz_bl_hejw(double K, double *b, double *a, int n, int ns, double *x, int len, int sign)
{
	int i,j,k,r,n1;
	double ar,ai,br,bi,zr,zi,freq,tempim,lasttempim,absa,numbr,numbi,numar,numai,pi,pfk0,pfk1;
    n1=n+1;
	pi=4.0*atan(1.0);
    r=0;
	for(k=0;k<len;k++)
	{
		freq=k*0.5/(len-1);
		x[k]=1.0;

		for(j=0;j<ns;j++)
		{
			br=0.0;
		    bi=0.0;
			for(i=0;i<n1;i++)
			{
				zr=cos(2.0*pi*freq*(n-i));   //可以考虑用数组代替
		        zi=-sin(2.0*pi*freq*(n-i));
				br+=b[j*n1+i]*zr;
				bi+=b[j*n1+i]*zi;
			}

			ar=0.0;
		    ai=0.0;
			for(i=0;i<n+1;i++)
			{
				zr=cos(2.0*pi*freq*(n-i));
		        zi=-sin(2.0*pi*freq*(n-i));
				ar+=a[j*n1+i]*zr;
				ai+=a[j*n1+i]*zi;
			}

			if(j==0) //如果是第一个值，我们存入numbr，numbi
			{
				absa=ar*ar+ai*ai;
				numar=(ar*br+bi*ai)/absa;
				numai=(bi*ar-br*ai)/absa;
			}
			else
			{
				absa=ar*ar+ai*ai;
				numbr=(ar*br+bi*ai)/absa;
			    numbi=(bi*ar-br*ai)/absa;
				numar+=numbr;
				numai+=numbi;
			}
		}
		x[k]=numar;
		tempim=numai; //存储虚部

		x[k]=x[k]*K;  //增益
		tempim=tempim*K;
		
		//x[k]/=ns;//因为是相当于ns个滤波器并行处理，故除以ns相当于一个
		//tempim/=ns;
		 if(sign==0)
		{
			x[k]=sqrt(x[k]*x[k]+tempim*tempim);
			//x[k]/=ns;//因为是相当于ns个滤波器并行处理，故除以ns相当于一个
		}
		else 
		if(sign==1)
		{
			x[k]=sqrt(x[k]*x[k]+tempim*tempim);
			//x[k]/=ns;//因为是相当于ns个滤波器并行处理，故除以ns相当于一个
			if((x[k]==0)&&(k>=1))
                x[k]=x[k-1];//防止突变，故不赋值x[k]=10.0*log10(1.0e-40);
			else if((x[k]==0)&&(k==0))
				x[k]=20.0*log10(1.0e-100);
			else
                x[k]=20.0*log10(x[k]);
		}
		else if(sign==2)
		{
			if(x[k]==0&&k>=1)
			   x[k]=x[k-1];  //防止突变，故不赋值给0
			else if(x[k]==0&&k==0)
				x[k]=0.0;
			else
			{
                //x[k]=atan2(tempim,x[k]);
			   if((k-1)>=0)
			   {
				   pfk0= atan2(lasttempim, x[k]);
		           pfk1 = atan2(tempim, x[k]);
			   }
			   if((k-1)>=0 && pfk1>1.50 && pfk0<-1.50)
		      {
			       r++;
			       x[k]=atan2(tempim, x[k])-2*pi*r;
		      }
		      else if((k-1)>=0 && pfk1<-1.50 && pfk0>1.50)
		      {
			      r--;
			      x[k]=atan2(tempim, x[k])-2*pi*r;
		      }
		      else
			      x[k]=atan2(tempim, x[k])-2*pi*r;
			}
		}
	lasttempim=tempim;
	}
	//处理高通开始处全是0的情况
    if(sign==1)
	{
		for(i=0;i<len;i++)
		{
			if(x[i]!=-1000)  //-1000 = 10.0*log10(1.0e-100);
				break;
		}
		for(k=0;k<i;k++)
		    x[k]=x[i];//-i+k;
	}
}
//--------------计算级联型模拟滤波器的幅频和相频响应--------------------//
/*
double *b------双精度实型二位数组，体积为ns*(n+1)。存放滤波器分子多项式的系数，
               b[j][i]表示第j个n阶节分子多项式的第i个系数。
double *a------双精度实型二位数组，体积为ns*(n+1)。存放滤波器分母多项式的系数，
               a[j][i]表示第j个n阶节分母多项式的第i个系数。
int    n-------整型变量。级联型滤波器每节的阶数。   b(0)+b(1)z^(-1)+ b(2)z^(2) 则为2
int   ns-------整型变量。级联型滤波器每n阶节数L   
double *x------双精度实型一维数组，长度为len。
               当sign=0时，存放滤波器频率响应|H(w)|,
			   sign=1时，存放分贝表示的滤波器幅频响应|H(w)|,
			   sign=2时，存放滤波器的相频响应fai(omg)
int   len------整型变量。频率响应的长度。
int   sign-----整型变量。
*/
void Hs_jl_hejw(double K, double *b, double *a, int n, int ns, double *x, long int maxpinlv, int len, int sign)
{
	int i,j,k,r,n1;
	double ar,ai,br,bi,re,im,freq,tempim,lasttempim,absa,numbr,numbi,numar,numai,temp,pi,pfk0,pfk1;
    n1=n+1;
	pi=4.0*atan(1.0);
    r=0;
	for(k=0;k<len;k++)
	{
		freq=maxpinlv*k/(len-1); 
		x[k]=1.0;

		for(j=0;j<ns;j++)
		{
			br=0.0;
		    bi=0.0;
			for(i=0;i<n1;i++)
			{
				cplx_imn(freq, i, &re, &im);
				br+=b[j*n1+i]*re;
				bi+=b[j*n1+i]*im;
			}

			if(j==0) //如果是第一个值，我们存入numbr，numbi
			{
                numbr=br;
	            numbi=bi;
			}
			else
			{
				temp=numbr*br-numbi*bi;
			    numbi=numbi*br+numbr*bi;
			    numbr=temp;
			}

		    ai=0.0;
			ar=0.0;
			for(i=0;i<n+1;i++)
			{
				cplx_imn(freq, i, &re, &im);
				ar+=a[j*n1+i]*re;
				ai+=a[j*n1+i]*im;
			}

			if(j==0) //如果是第一个值，我们存入numbr，numbi
			{
                numar=ar;
	            numai=ai;
			}
			else
			{
			    temp=numar*ar-numai*ai;
			    numai=numai*ar+numar*ai;
			    numar=temp;
			}
		}
		absa=numar*numar+numai*numai;
		x[k]=numar*numbr+numbi*numai;
		tempim=numbi*numar-numbr*numai;
		x[k]/=absa;
		tempim/=absa; //存储虚部

		x[k]*=K;
        tempim*=K;
		 if(sign==0)
		{
			x[k]=sqrt(x[k]*x[k]+tempim*tempim);
		}
		else if(sign==1)
		{
			x[k]=sqrt(x[k]*x[k]+tempim*tempim);
			//if((x[k]<(1.0e-200))&&(k>=1))
			if((x[k]==0)&&(k>=1))
                x[k]=x[k-1];//防止突变，故不赋值x[k]=10.0*log10(1.0e-40);
			//else if((x[k]<(1.0e-200))&&(k==0))
			else if((x[k]==0)&&(k==0))
				x[k]=20.0*log10(1.0e-100);
			else
                x[k]=20.0*log10(x[k]);
		}
		else if(sign==2)
		{
			if(x[k]==0&&k>=1)
			   x[k]=x[k-1];  //防止突变，故不赋值给0
			else if(x[k]==0&&k==0)
				x[k]=0.0;
			else
			{
                //x[k]=atan2(tempim,x[k]);

			   if((k-1)>=0)
			   {
				   pfk0= atan2(lasttempim, x[k]);
		           pfk1 = atan2(tempim, x[k]);
			   }
			   if((k-1)>=0 && pfk1>1.50 && pfk0<-1.50)
		      {
			       r++;
			       x[k]=atan2(tempim, x[k])-2*pi*r;
		      }
		      else if((k-1)>=0 && pfk1<-1.50 && pfk0>1.50)
		      {
			      r--;
			      x[k]=atan2(tempim, x[k])-2*pi*r;
		      }
		      else
			      x[k]=atan2(tempim, x[k])-2*pi*r;
			}
		}
	lasttempim=tempim;
	}
	//处理高通开始处全是0的情况
    if(sign==1)
	{
		for(i=0;i<len;i++)
		{
			if(x[i]!=-1000)  //-1000 = 10.0*log10(1.0e-100);
				break;
		}
		for(k=0;k<i;k++)
		    x[k]=x[i];//-i+k;
	}
}
//----------------------------------------------------数字滤波器的时域响应---------------------------------------------//
/*---------------------------计算直接型滤波器的时域响应----单位脉冲响应、单位阶跃响应（其实就是滤波，但不可用于滤波处理）-----------------------
double *y------双精度实型一维数组，长度为lx。存放滤波器的输出序列。
ly-------------整型变量。输出序列的长度。
b--------------双精度实型一维数组，长度为(m+1)。存放滤波器分子多项式的系数。
a--------------双精度实型一维数组，长度为(n+1)。存放滤波器分母多项式的系数。
m--------------整型变量。滤波器分子多项式的阶数。
n--------------整型变量。滤波器分母多项式的阶数。
char  morj--------布尔变量。输出单位脉冲响应、单位阶跃响应
*/
void resp(double K, double *y,  int ly, double *b, double *a, int m, int n, char morj)
{
	int k,i,i1;
	double x[3];

	if(morj==1) // 单位脉冲响应的输入序列
	{
	   x[0]=1;
	   x[1]=0;
	   x[2]=0;
	}
	else // 单位阶跃响应的输入序列
	{
	   x[0]=1;
	   x[1]=1;
	   x[2]=1;
	}
	
	for(k=0;k<ly;k++)
	{
		y[k]=0.0;
		for(i=0;i<=m;i++)
		{
			if((k-i)>=0)
			{
				i1=((k-i)<3)?(k-i):(3-1);   //此处的处理，当分子系数不存在时，它把最后一个当作是其系数（正好与单位脉冲响应、单位阶跃响应定义吻合）
				                             //，可见这个算法不能用于滤波处理
				y[k]=y[k]+b[m-i]*x[i1];
			}
		}
		for(i=1;i<=n;i++)
		{
			if((k-i)>=0)
				y[k]=y[k]-a[n-i]*y[k-i];
		}
	}
	for(k=0;k<ly;k++)  
	    x[k]=x[k]*K;
}
/*----------------------------级联型数字滤波器的时域响应----单位脉冲响应、单位阶跃响应（其实就是滤波，但不可用于滤波处理）------------------
double K-------增益K
double *b---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分子多项式的系数，b[j][i]
                  表示第j个n阶节的分子多项式的第i个系数。
double *a---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分母多项式的系数，b[j][i]
                  表示第j个n阶节的分母多项式的第i个系数。
int    n----------整型变量。滤波器每节的阶数。
int   ns----------整型变量。滤波器n阶节数：L
double *y---------双精度实型一维数组，长度为len。存放单位脉冲响应、单位阶跃响应的输出序列； 
int   len---------整型变量。单位脉冲响应、单位阶跃响应输出序列的长度。
char  morj--------布尔变量。输出单位脉冲响应、单位阶跃响应
*/
void resp_jl(double K, double *b, double *a, int n, int ns, double *y, int len, char morj)
{
	int i,j,k,n1;
	double *x;
	n1=n+1;
	
	x=malloc(len*sizeof(double));

	if(morj==1) // 单位脉冲响应的输入序列
	{
	   x[0]=1;
	   memset(x+1, 0, (len-1)*sizeof(double));
	}
	else // 单位阶跃响应的输入序列
	{
	   for(k=0;k<len;k++)
		   x[k]=1.0;
	}

	memset(y, 0, len*sizeof(double));
	for(k=0;k<len;k++)  //求出第一节输出
	{
		y[k]=b[n]*x[k];
		for(i=1;i<n1;i++)
	    {
			if((k-i)>=0)
			{
			    y[k]+=b[n-i]*x[k-i]-a[n-i]*y[k-i];
			} 
		}
	}
    memcpy(x, y, len*sizeof(double));

	for(j=1;j<ns;j++)  //处理后面的级联
	{
		memset(y, 0, len*sizeof(double));
		for(k=0;k<len;k++)
		{
			y[k]=b[j*n1+n]*x[k];
		    for(i=1;i<n1;i++)
		   {
			   if((k-i)>=0)
			   {
				   y[k]+=b[j*n1+n-i]*x[k-i]-a[j*n1+n-i]*y[k-i];
			   }
		   }
		}
		memcpy(x, y, len*sizeof(double));
	}
	for(k=0;k<len;k++)  
	    y[k]=y[k]*K;

	free(x);
}
void hs_resp_jl(double K, double *b, double *a, int n, int ns, double *y, int len, char morj)
{
	/*int i,j,k,n1;
	double *x;
	n1=n+1;
	
	x=malloc(len*sizeof(double));

	if(morj==1) // 单位脉冲响应的输入序列
	{
	   x[0]=1;
	   memset(x+1, 0, (len-1)*sizeof(double));
	}
	else // 单位阶跃响应的输入序列
	{
	   for(k=0;k<len;k++)
		   x[k]=1;
	}

	memset(y, 0, len*sizeof(double));
	for(k=0;k<len;k++)  //求出第一节输出
	{
		y[k]=b[0]*x[k];
		for(i=1;i<n1;i++)
	    {
			if((k-i)>=0)
			{
			    y[k]+=b[i]*x[k-i]-a[i]*y[k-i];
			} 
		}
	}
    memcpy(x, y, len*sizeof(double));

	for(j=1;j<ns;j++)  //处理后面的级联
	{
		memset(y, 0, len*sizeof(double));
		for(k=0;k<len;k++)
		{
			y[k]=b[j*n1]*x[k];
		    for(i=1;i<n1;i++)
		   {
			   if((k-i)>=0)
			   {
				   y[k]+=b[j*n1+i]*x[k-i]-a[j*n1+i]*y[k-i];
			   }
		   }
		}
		memcpy(x, y, len*sizeof(double));
	}
	for(k=0;k<len;k++)  
	    y[k]=y[k]*K;

	free(x);*/
}
/*----------------------------并联型数字滤波器的时域响应----单位脉冲响应、单位阶跃响应（其实就是滤波，但不可用于滤波处理）------------------
double K-------增益K
double *b---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分子多项式的系数，b[j][i]
                  表示第j个n阶节的分子多项式的第i个系数。
double *a---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分母多项式的系数，b[j][i]
                  表示第j个n阶节的分母多项式的第i个系数。
int    n----------整型变量。滤波器每节的阶数。
int   ns----------整型变量。滤波器n阶节数：L
double *y---------双精度实型一维数组，长度为len。存放单位脉冲响应、单位阶跃响应的输出序列； 
int   len---------整型变量。单位脉冲响应、单位阶跃响应输出序列的长度。
char  morj--------布尔变量。输出单位脉冲响应、单位阶跃响应
*/
void resp_bl(double K, double *b, double *a, int n, int ns, double *y, int len, char morj)
{
	int i,j,k,n1;
	double *x;
	n1=n+1;
	
	x=malloc(len*sizeof(double));

	if(morj==1) // 单位脉冲响应的输入序列
	{
	   x[0]=1;
	   memset(x+1, 0, (len-1)*sizeof(double));
	}
	else // 单位阶跃响应的输入序列
	{
	   for(k=0;k<len;k++)
		   x[k]=1.0;
	}

	memset(y, 0, len*sizeof(double));
	for(j=0;j<ns;j++)  
	{	
		for(k=0;k<len;k++)
		{
			y[k]+=b[j*n1+n]*x[k];
		    for(i=1;i<n1;i++)
		   {
			   if((k-i)>=0)
			   {
				   y[k]+=b[j*n1+n-i]*x[k-i]-a[j*n1+n-i]*y[k-i];
			   }
		   }
		}
	}
	for(j=0;j<len;j++)  
	{
	    y[j]=y[j]*K;//增益K
	}
	free(x);
}
/*---------------------------用直接型IIR数字滤波器进行数字滤波--------------------
int b----------双精度实型一维数组，长度为(m+1)。存放滤波器分子多项式的系数。
iny a----------双精度实型一维数组，长度为(n+1)。存放滤波器分母多项式的系数。
int m----------整型变量。滤波器分子多项式的阶数。
int n----------整型变量。滤波器分母多项式的阶数。
double *x------双精度实型一维数组，长度为len。开始时存放滤波器的输入序列，最后存放滤波器的输出序列；
               在分块处理时，它用于表示当前块内的滤波器的输入序列与输出序列。
int len--------整型变量。输入序列与输出序列的长度；在分块处理时，它用于表示块的长度。
double *px-----双精度实型一维数组，长度为(m+1)。在分块处理时，它用于保存前一块滤波时的(m+1)个输入序列值。
double *py-----双精度实型一维数组，长度为(m+1)。在分块处理时，它用于保存前一块滤波时的(m+1)个输出序列值。

    当输入序列x(n)很长时，由于计算机内存的限制，常将其分成彼此连接的若干块进行处理。数组px与py就是
专为分块处理而设置的。px用于保存前一块滤波时的(m+1)个输入序列值，即px[]={x(k),x(k-1),...,x(k-m)};py用于
保存前一块滤波时的n个输出序列值，即py[]={y(k-1),y(k-2),...,y(k-n)}。通常，我们假定滤波器的初始条件为零，
因此数组px[]与py[]在滤波前都要初始化为零。
*/
void filter(double *b, double *a, int m, int n, double *x, int len, double *px, double *py)
{
	int k,i;
	for(k=0;k<len;k++)
	{
		px[0]=x[k];
		x[k]=0.0;
		for(i=0;i<=m;i++)
		{
			x[k]=x[k]+b[i]*px[i];
		}
		for(i=1;i<=n;i++)
		{
			x[k]=x[k]-a[i]*py[i];
		}

		//将px与py移动，将新的值填入
		for(i=m;i>=1;i--)
		{
			px[i]=px[i-1];
		}
		for(i=n;i>=2;i--)
		{
			py[i]=py[i-1];
		}
		py[1]=x[k];
	}
}
/*-----------------------------------级联型数字滤波器的滤波处理---------------------------------
double *b---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分子多项式的系数，b[j][i]
                  表示第j个n阶节的分子多项式的第i个系数。
double *a---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分母多项式的系数，b[j][i]
                  表示第j个n阶节的分母多项式的第i个系数。
int    n----------整型变量。滤波器每节的阶数。
int   ns----------整型变量。滤波器n阶节数：L
double *x------双精度实型一维数组，长度为len。开始时存放滤波器的输入序列，最后存放滤波器的输出序列；
               在分块处理时，它用于表示当前块内的滤波器的输入序列与输出序列。
int len--------整型变量。输入序列与输出序列的长度；在分块处理时，它用于表示块的长度。
double *px-----双精度实型一维数组，长度为ns*(n+1)。在分块处理时，它用于保存前一块滤波时的(n+1)个输入序列值。
double *py-----双精度实型一维数组，长度为ns*(n+1)。在分块处理时，它用于保存前一块滤波时的(n)个输出序列值。

    当输入序列x(n)很长时，由于计算机内存的限制，常将其分成彼此连接的若干块进行处理。数组px与py就是
专为分块处理而设置的。px用于保存前一块滤波时的(n+1)个输入序列值，即px[j][]={x(k),x(k-1),...,x(k-n)};py用于
保存前一块滤波时的n个输出序列值，即py[j][]={y(k-1),y(k-2),...,y(k-n)}。通常，我们假定滤波器的初始条件为零，
因此数组px[][]与py[][]在滤波前都要初始化为零。
*/
void filte_jl(double K, double *b, double *a, int n, int ns, double *x, int len)
{
	int i,j,k,n1;
	double *y;
	n1=n+1;
	
    y=malloc(len*sizeof(double));

	memset(y, 0, len*sizeof(double));
	for(k=0;k<len;k++)  //求出第一节输出
	{
		y[k]=b[n]*x[k];
		for(i=1;i<n1;i++)
	    {
			if((k-i)>=0)
			{
			    y[k]+=b[n-i]*x[k-i]-a[n-i]*y[k-i];
			} 
		}
	}
    memcpy(x, y, len*sizeof(double));


	for(j=1;j<ns;j++)  //处理后面的级联
	{
		memset(y, 0, len*sizeof(double));
		for(k=0;k<len;k++)
		{
			y[k]=b[j*n1+n]*x[k];
		    for(i=1;i<n1;i++)
		   {
			   if((k-i)>=0)
			   {       
				   y[k]+=b[j*n1+n-i]*x[k-i]-a[j*n1+n-i]*y[k-i];
			   }
		   }
		}
		memcpy(x, y, len*sizeof(double));
	}
	for(k=0;k<len;k++)  
	    y[k]=y[k]*K;

    memcpy(x,y,len*sizeof(double));

	free(y);
}
/*-----------------------------------并联型数字滤波器的滤波处理---------------------------------
double *b---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分子多项式的系数，b[j][i]
                  表示第j个n阶节的分子多项式的第i个系数。
double *a---------双精度实型二维数组，体积为ns*(n+1)。存放滤波器分母多项式的系数，b[j][i]
                  表示第j个n阶节的分母多项式的第i个系数。
int    n----------整型变量。滤波器每节的阶数。
int   ns----------整型变量。滤波器n阶节数：L
double *x------双精度实型一维数组，长度为len。存放滤波器的输入序列；
               在分块处理时，它用于表示当前块内的滤波器的输入序列。
double *y------双精度实型一维数组，长度为len。存放滤波器的输入序列；
               在分块处理时，它用于表示当前块内的滤波器的输出序列。
int len--------整型变量。输入序列与输出序列的长度；在分块处理时，它用于表示块的长度。
double *px-----双精度实型一维数组，长度为ns*(n+1)。在分块处理时，它用于保存前一块滤波时的(n+1)个输入序列值。
double *py-----双精度实型一维数组，长度为ns*(n+1)。在分块处理时，它用于保存前一块滤波时的(n)个输出序列值。

    当输入序列x(n)很长时，由于计算机内存的限制，常将其分成彼此连接的若干块进行处理。数组px与py就是
专为分块处理而设置的。px用于保存前一块滤波时的(n+1)个输入序列值，即px[j][]={x(k),x(k-1),...,x(k-n)};py用于
保存前一块滤波时的n个输出序列值，即py[j][]={y(k-1),y(k-2),...,y(k-n)}。通常，我们假定滤波器的初始条件为零，
因此数组px[][]与py[][]在滤波前都要初始化为零。
*/
void filte_bl(double K, double *b, double *a, int n, int ns, double *x, int len)
{
     int i,j,k,n1;
	double *y;
	n1=n+1;
	
	y=malloc(len*sizeof(double));

	memset(y, 0, len*sizeof(double));
	for(j=0;j<ns;j++)  
	{	
		for(k=0;k<len;k++)
		{
			y[k]+=b[j*n1+n]*x[k];
		    for(i=1;i<n1;i++)
		   {
			   if((k-i)>=0)
			   {
				   y[k]+=b[j*n1+n-i]*x[k-i]-a[j*n1+n-i]*y[k-i];
			   }
		   }
		}
	}
	for(j=0;j<len;j++)  
	{
	    y[j]=y[j]*K;//增益K
	}
	memcpy(x,y,len*sizeof(double));
	free(y);
}
/*-----------------------------------将级联形式的滤波器系数转换成直接形式的系数----------------------------
注意：分子分母必须这样的给出：a0+a1*x^1+a2*x^2+....+an*x^n 或  a0*x^n+a1*x^(n-1)+a2*x^(n-2)+....+an
int      ns--------------滤波器的节数。
int      n---------------滤波器每节的阶数。a0+a1*x+a2*x^2 则n=2
double*  b---------------双精度实型二位数组，体积为(N+1)/2*(n+1)。存放滤波器分子多项式的系数。
                         b[j][i]表示第j个n阶节的分子多项式的第i个系数
double*  a---------------双精度二位数组，体积为(N+1)/2*(n+1)。存放滤波器分母多项式的系数。
                         a[j][i]表示第jj个n阶节的分母多项式的第i个系数

		                 其中n，是每节滤波器的阶数，低通和高通为2，带通和带阻为4
double*  bb--------------转换后的滤波器分子系数数组，体积为n*ns+1
double*  aa--------------转换后的滤波器分母系数数组，体积为n*ns+1
*/
void jltozhj(int ns, int n, double *b, double *a, double *bb, double *aa)
{
	int i;

    double *temp=NULL;

	if(ns<=1)
	{
		memcpy(bb,b,(n+1)*sizeof(double));
		memcpy(aa,a,(n+1)*sizeof(double));
		return ;
	}

	temp=malloc((n*ns+1)*sizeof(double));
	duoxiangshi_mul(n, n, b, b+n+1, bb);
	for(i=2;i<ns;i++)
	{
		duoxiangshi_mul(i*n, n, bb, b+i*(n+1), temp);
		memcpy(bb, temp, ((i+1)*n+1)*sizeof(double));
	}

	duoxiangshi_mul(n, n, a, a+n+1, aa);
	for(i=2;i<ns;i++)
	{
		duoxiangshi_mul(i*n, n, aa, a+i*(n+1), temp);
		memcpy(aa, temp, ((i+1)*n+1)*sizeof(double));
	}

	free(temp);
}
/*---------------------------------将并联型的滤波器转换成直接型的滤波器-------------------------------
注意：分子分母必须这样的给出：a0+a1*x^1+a2*x^2+....+an*x^n 或  a0*x^-n+a1*x^-(n-1)+a2*x^-(n-2)+....+an
int      ns--------------滤波器的节数。
int      n---------------滤波器每节的阶数。a0+a1*x+a2*x^2 则n=2
double*  b---------------双精度实型二位数组，体积为(N+1)/2*(n+1)。存放滤波器分子多项式的系数。
                         b[j][i]表示第j个n阶节的分子多项式的第i个系数
double*  a---------------双精度二位数组，体积为(N+1)/2*(n+1)。存放滤波器分母多项式的系数。
                         a[j][i]表示第jj个n阶节的分母多项式的第i个系数

		                 其中n，是每节滤波器的阶数，低通和高通为2，带通和带阻为4
double*  bb--------------转换后的滤波器分子系数数组，体积为n*ns+1
double*  aa--------------转换后的滤波器分母系数数组，体积为n*ns+1
*/
void bltozhj(int ns, int n, double *b, double *a, double *bb, double *aa)
{
	int i;

    double *tempa=NULL,*tempb=NULL;

	if(ns<=1)
	{
		memcpy(bb,b,(n+1)*sizeof(double));
		memcpy(aa,a,(n+1)*sizeof(double));
		return ;
	}

	tempa=malloc((n*ns+1)*sizeof(double));
	tempb=malloc((n*ns+1)*sizeof(double));

	duoxiangshi_add(n, n, a, b, a+n+1, b+n+1, aa, bb);

	for(i=2;i<ns;i++)
	{
		duoxiangshi_add(i*n, n, aa, bb, a+i*(n+1), b+i*(n+1), tempa, tempb);
		memcpy(bb, tempb, ((i+1)*n+1)*sizeof(double));
		memcpy(aa, tempa, ((i+1)*n+1)*sizeof(double));
	}

	free(tempa);
	free(tempb);
}

/*-----------------------------将级联型的滤波器转换成并联型的滤波器（2阶或4阶）---------------------------
//这个函数用于求解级联型H(s) 的一个极点处的留数
pole_n   --------滤波器极点个数
n     --------阶数
ns    --------节数
ns_cur -------现在求的那个极点位于哪个节(0~ns-1)
double*  b---------------双精度实型二位数组，体积为(N+1)/2*(n+1)。存放滤波器分子多项式的系数。
                         b[j][i]表示第j个n阶节的分子多项式的第i个系数
double*  bb--------------直接型滤波器的分子系数,长度ns*n+1
double*  a---------------双精度二位数组，体积为(N+1)/2*(n+1)。存放滤波器分母多项式的系数。
                         a[j][i]表示第jj个n阶节的分母多项式的第i个系数
double*  in_re --------极点的实部数阻，长度n
double*  in_im --------极点的虚部数阻，长度n,其中将要求对应点留数的那个极点放在数组第一个位置
re    --------求得的实部
im    --------求得的虚部
*/
void liushu_jl(int pole_n, int n, int ns, int ns_cur, double *bb, double *a, double* in_re, double* in_im, double *re, double *im)
{
	int i,j;
	double rrr,imm,sumre,sumim,temp,sumare,sumaim,tr,ti;

	/*sumre=0.0;
	sumim=0.0;
	for(i=0;i<ns;i++)  //处理分子极点带入
	{
		tr=b[i*(n+1)]; //此处因为我们虽是一个极点，但还是占用3的阶的形式表示，故
		ti=0.0;
		for(j=1;j<n+1;j++)
		{
			rrr=in_re[0];
		    imm=in_im[0];
		    cplx_pow(&rrr, &imm, j);
		    tr+=b[i*(n+1)+j]*rrr;
            ti+=b[i*(n+1)+j]*imm;
		}

		if(i==0) //如果是第一个值，我们存入sumre，sumim
		{
            sumre=tr;
	        sumim=ti;
		}
		else
		{
			temp=sumre*tr-sumim*ti;
			sumim=sumim*tr+sumre*ti;
			sumre=temp;
		}
	}*/
	sumre=bb[0];
	sumim=0.0;
	for(i=1;i<ns*n+1;i++)
	{
		rrr=in_re[0];
		imm=in_im[0];
		cplx_pow(&rrr, &imm, i);
		sumre+=bb[i]*rrr;
        sumim+=bb[i]*imm;
	}

	//处理分母极点的带入
	sumare=0.0;
	sumaim=0.0;
	for(i=0;i<ns;i++)  //处理分子极点带入
	{
		if(ns_cur==i)
		{
			if(pole_n==1)  //处理只有一个极点的特殊
				continue;
			for(j=1;j<pole_n;j++)
			{
				rrr=in_re[0]-in_re[j];
                imm=in_im[0]-in_im[j];
				if(j==1)
				{
					tr=rrr;
				    ti=imm;
				}
				else
				{
					temp=tr*rrr-ti*imm;
			        ti=ti*rrr+tr*imm;
		        	tr=temp;
				}
			}
		}
		else
		{
			tr=a[i*(n+1)];
		    ti=0.0;
	    	for(j=1;j<n+1;j++)
	    	{
		    	rrr=in_re[0];
		        imm=in_im[0];
		        cplx_pow(&rrr, &imm, j);
	    	    tr+=a[i*(n+1)+j]*rrr;
                ti+=a[i*(n+1)+j]*imm;
	    	}
		}

		if(i==0) //如果是第一个值，我们存入sumare，sumaim
		{
            sumare=tr;
	        sumaim=ti;
		}
		else
		{
			temp=sumare*tr-sumaim*ti;
			sumaim=sumaim*tr+sumare*ti;
			sumare=temp;
		}
	}

	temp=sumare*sumare+sumaim*sumaim;
    *re=(sumre*sumare+sumim*sumaim)/temp;
    *im=(sumim*sumare-sumre*sumaim)/temp;
}
/*------------
int      pole_n --------滤波器极点个数
int      n---------------阶数
int      ns-------------滤波器的节数
int      ns_cur---------现在求的那个极点位于哪个节(0~ns-1)
double*  b---------------双精度实型二位数组，体积为(N+1)/2*(n+1)。存放滤波器分子多项式的系数。
                         b[j][i]表示第j个n阶节的分子多项式的第i个系数
double*  a---------------双精度二位数组，体积为(N+1)/2*(n+1)。存放滤波器分母多项式的系数。
                         a[j][i]表示第jj个n阶节的分母多项式的第i个系数
double*  bb--------------直接型滤波器的分子系数,长度ns*n+1
double   *lastb
double   *lasta----------存放原先的b和a中值，用于求留数
*/
char jltobl_each(int pole_n, int n, int ns, int ns_cur, double* b, double* a, double *bb, double *lastb, double *lasta)
{
	int j;
	double *lingdianre,*lingdianim;
	double ljdvlure1,ljdvluim1,ljdvlure2,ljdvluim2,ljdvlure3,ljdvluim3,ljdvlure4,ljdvluim4;
	double *ar0,*ai0,*br0,*bi0,*ar1,*ai1,*br1,*bi1,*aor, *aoi, *bor, *boi;
    char result;

	//char date[100];  //用于调试数据

	result=1;
	switch(pole_n)
	{
	case 1:
		ljdvlure2=-a[0]/a[1];
		ljdvluim2=0.0;
		liushu_jl(pole_n, n, ns, ns_cur, bb, lasta, &ljdvlure2, &ljdvluim2, &ljdvlure1,&ljdvluim1);
        if((fabs(ljdvluim1)>0.000001))
			return result=-1;
		b[0]=ljdvlure1;
		b[1]=0.0;
		b[2]=0.0;   

		//sprintf(date,"jidian:%f+j%f\tliushu:%f+j%f\n",ljdvlure2,ljdvluim2,ljdvlure1,ljdvluim1);  OutputDebugString(date); 
		break;
	case 2:
		lingdianre=malloc(n*sizeof(double));
		lingdianim=malloc(n*sizeof(double));
		ar0=malloc(2*sizeof(double));
		ai0=malloc(2*sizeof(double));
		br0=malloc(2*sizeof(double));
		bi0=malloc(2*sizeof(double));
		ar1=malloc(2*sizeof(double));
		ai1=malloc(2*sizeof(double));
		br1=malloc(2*sizeof(double));
		bi1=malloc(2*sizeof(double));
		aor=malloc(3*sizeof(double));
		aoi=malloc(3*sizeof(double));
		bor=malloc(3*sizeof(double));
		boi=malloc(3*sizeof(double));

		if(qrrt(pole_n, a, lingdianre, lingdianim, 0.00001, 100)<0)
	    {
			result=-1;
			goto END;
		}
		//求极点的留数
		liushu_jl(pole_n, n, ns, ns_cur, bb, lasta,  lingdianre,lingdianim, &ljdvlure1,&ljdvluim1);
		br0[0]=ljdvlure1; bi0[0]=ljdvluim1;
		br0[1]=0.0; bi0[1]=0.0;
		ar0[0]=-lingdianre[0]; ai0[0]=-lingdianim[0];
		ar0[1]=1.0; ai0[1]=0.0;

		//sprintf(date,"\njidian:%f+j%f\tliushu:%f+j%f\n",lingdianre[0],lingdianim[0],ljdvlure1,ljdvluim1);  OutputDebugString(date);
        
        ljdvlure2=lingdianre[1]; lingdianre[1]=lingdianre[0]; lingdianre[0]=ljdvlure2;
		ljdvlure2=lingdianim[1]; lingdianim[1]=lingdianim[0]; lingdianim[0]=ljdvlure2;
		liushu_jl(pole_n, n, ns, ns_cur, bb, lasta, lingdianre,lingdianim, &ljdvlure2,&ljdvluim2);
        
		br1[0]=ljdvlure2; bi1[0]=ljdvluim2;
		br1[1]=0.0; bi1[1]=0.0;
		ar1[0]=-lingdianre[0]; ai1[0]=-lingdianim[0];//上面颠倒了顺序
		ar1[1]=1.0; ai1[1]=0.0;

		//sprintf(date,"jidian:%f+j%f\tliushu:%f+j%f\n",lingdianre[0],lingdianim[0],ljdvlure2,ljdvluim2);  OutputDebugString(date); 
 
		cplx_fmdxsh_add(1, 1, ar0, ai0, br0, bi0, ar1, ai1, br1, bi1, aor, aoi, bor, boi);

		for(j=0;j<n+1;j++)
		{
			if((fabs(aoi[j])>0.000001)||(fabs(boi[j])>0.000001))
			{
				result=-1;
				goto END;
			}
			a[j]=aor[j];     
			b[j]=bor[j];
		}

END:	free(lingdianre);
	    free(lingdianim);
		free(ar0);
	    free(ai0);
	    free(br0);
	    free(bi0);
	    free(ar1);
	    free(ai1);
	    free(br1);
	    free(bi1);
	    free(aor);
        free(aoi);
        free(bor);
        free(boi);
		break;
	case 4:
		lingdianre=malloc(n*sizeof(double));
		lingdianim=malloc(n*sizeof(double));
		ar0=malloc(4*sizeof(double));
		ai0=malloc(4*sizeof(double));
		br0=malloc(4*sizeof(double));
		bi0=malloc(4*sizeof(double));
		ar1=malloc(4*sizeof(double));
		ai1=malloc(4*sizeof(double));
		br1=malloc(4*sizeof(double));
		bi1=malloc(4*sizeof(double));
		aor=malloc(5*sizeof(double));
		aoi=malloc(5*sizeof(double));
		bor=malloc(5*sizeof(double));
		boi=malloc(5*sizeof(double));

		if(qrrt(pole_n, a, lingdianre, lingdianim, 0.00001, 100)<0)
	    {
			result=-1;
			goto END2;
		}
		//求两个极点的留数
		liushu_jl(pole_n, n, ns, ns_cur, bb, lasta,  lingdianre,lingdianim, &ljdvlure1,&ljdvluim1);
        br0[0]=ljdvlure1; bi0[0]=ljdvluim1;
		br0[1]=0.0; bi0[1]=0.0;
		ar0[0]=-lingdianre[0]; ai0[0]=-lingdianim[0];
		ar0[1]=1.0; ai0[1]=0.0;

		//sprintf(date,"\njidian:%f+j%f\tliushu:%f+j%f\n",lingdianre[0],lingdianim[0],ljdvlure1,ljdvluim1);  OutputDebugString(date); 

        ljdvlure2=lingdianre[1]; lingdianre[1]=lingdianre[0]; lingdianre[0]=ljdvlure2;
		ljdvlure2=lingdianim[1]; lingdianim[1]=lingdianim[0]; lingdianim[0]=ljdvlure2;
		liushu_jl(pole_n, n, ns, ns_cur, bb, lasta, lingdianre,lingdianim, &ljdvlure2,&ljdvluim2);
        br1[0]=ljdvlure2; bi1[0]=ljdvluim2;
		br1[1]=0.0; bi1[1]=0.0;
		ar1[0]=-lingdianre[0]; ai1[0]=-lingdianim[0];
		ar1[1]=1.0; ai1[1]=0.0;
		
		//sprintf(date,"jidian:%f+j%f\tliushu:%f+j%f\n",lingdianre[0],lingdianim[0],ljdvlure2,ljdvluim2);  OutputDebugString(date); 

		cplx_fmdxsh_add(1, 1, ar0, ai0, br0, bi0, ar1, ai1, br1, bi1, aor, aoi, bor, boi);

		for(j=0;j<3;j++)
		{
			ar0[j]=aor[j];ai0[j]=aoi[j];
			br0[j]=bor[j];bi0[j]=boi[j];
		}

		ljdvlure3=lingdianre[2]; lingdianre[2]=lingdianre[0]; lingdianre[0]=ljdvlure3;
		ljdvlure3=lingdianim[2]; lingdianim[2]=lingdianim[0]; lingdianim[0]=ljdvlure3;
		liushu_jl(pole_n, n, ns, ns_cur, bb, lasta, lingdianre,lingdianim, &ljdvlure3,&ljdvluim3);
		br1[0]=ljdvlure3; bi1[0]=ljdvluim3;
		br1[1]=0.0; bi1[1]=0.0;
		ar1[0]=-lingdianre[0]; ai1[0]=-lingdianim[0];
		ar1[1]=1.0; ai1[1]=0.0;

		//sprintf(date,"jidian:%f+j%f\tliushu:%f+j%f\n",lingdianre[0],lingdianim[0],ljdvlure3,ljdvluim3);  OutputDebugString(date); 

		cplx_fmdxsh_add(2, 1, ar0, ai0, br0, bi0, ar1, ai1, br1, bi1, aor, aoi, bor, boi);

		for(j=0;j<4;j++)
		{
			ar0[j]=aor[j];ai0[j]=aoi[j];
			br0[j]=bor[j];bi0[j]=boi[j];
		}

		ljdvlure4=lingdianre[3]; lingdianre[3]=lingdianre[0]; lingdianre[0]=ljdvlure4;
		ljdvlure4=lingdianim[3]; lingdianim[3]=lingdianim[0]; lingdianim[0]=ljdvlure4;
		liushu_jl(pole_n, n, ns, ns_cur, bb, lasta, lingdianre,lingdianim, &ljdvlure4,&ljdvluim4);
		br1[0]=ljdvlure4; bi1[0]=ljdvluim4;
		br1[1]=0.0; bi1[1]=0.0;
		ar1[0]=-lingdianre[0]; ai1[0]=-lingdianim[0];
		ar1[1]=1.0; ai1[1]=0.0;

		///sprintf(date,"jidian:%f+j%f\tliushu:%f+j%f\n",lingdianre[0],lingdianim[0],ljdvlure4,ljdvluim4);  OutputDebugString(date); 

		cplx_fmdxsh_add(3, 1, ar0, ai0, br0, bi0, ar1, ai1, br1, bi1, aor, aoi, bor, boi);

		for(j=0;j<n+1;j++)
		{
			if((fabs(aoi[j])>0.000001)||(fabs(boi[j])>0.000001))
			{
				result=-1;
				goto END2;
			}
			a[j]=aor[j];     
			b[j]=bor[j];
		}

END2:	free(lingdianre);
	    free(lingdianim);
		free(ar0);
	    free(ai0);
	    free(br0);
	    free(bi0);
	    free(ar1);
	    free(ai1);
	    free(br1);
	    free(bi1);
	    free(aor);
        free(aoi);
        free(bor);
        free(boi);
		break;
	}
	return result;
}
/*注意：分子分母必须这样的给出：a0+a1*x^1+a2*x^2+....+an*x^n 或  a0*x^-n+a1*x^-(n-1)+a2*x^-(n-2)+....+an
int      ns--------------滤波器的节数。
int      n---------------滤波器每节的阶数。a0+a1*x+a2*x^2 则n=2
double*  b---------------双精度实型二位数组，体积为(N+1)/2*(n+1)。存放滤波器分子多项式的系数。
                         b[j][i]表示第j个n阶节的分子多项式的第i个系数
double*  a---------------双精度二位数组，体积为(N+1)/2*(n+1)。存放滤波器分母多项式的系数。
                         a[j][i]表示第jj个n阶节的分母多项式的第i个系数
*/
char jltobl(int ns, int n, double *b, double *a)
{
	int i,j;
    double *aa,*bb,*lasta,*lastb;
	double hign_vlu;

	//char date[100];

	aa=malloc((ns*n+1)*sizeof(double));
	bb=malloc((ns*n+1)*sizeof(double));
	lasta=malloc((ns*(n+1))*sizeof(double));//存放分子和分母数组的值
	lastb=malloc((ns*(n+1))*sizeof(double));

	memcpy(lasta,a,(ns*(n+1))*sizeof(double));
	memcpy(lastb,b,(ns*(n+1))*sizeof(double));

	//先检测分子最高项的系数是否为0,如果不为0先变成1+H(s)'的形式
    jltozhj(ns, n, b, a, bb, aa);
	hign_vlu=0.0;
	//先检测a最高系数是否是0，如果是先处理
	for(j=0;j<ns*n+1;j++)
	{
		if(fabs(aa[ns*n-j])>0.000000001)
			break;
	}
    if(bb[ns*n-j]!=0)
	{
		hign_vlu=bb[ns*n-j]/aa[ns*n-j];
		for(i=0;i<ns*n-j;i++)
		{
			bb[i]=bb[i]-hign_vlu*aa[i];
			//sprintf(date,"\nhign_vlu=%f\tbb[%d]=%f\t",hign_vlu, i,bb[i]);  OutputDebugString(date); 
		}
		bb[ns*n-j]=0.0;
	}
    //处理分子最高项是0的形式
	if(n==2)
	{
	    for(i=0;i<ns;i++)  //处理每个阶
	    {
			if(*(a+i*(n+1)+2)==0)//对于其中只有单个极点的处理
			{
				if(ns==1)//只有一个多项式且是单极点，不用变化
					break;
				jltobl_each(1, 2, ns, i, b+i*(n+1), a+i*(n+1), bb, lastb, lasta);
			} 
			else
			{
		        jltobl_each(2, 2, ns, i, b+i*(n+1), a+i*(n+1), bb,  lastb, lasta);
			}
	    }
		
	}
	else if(n==4)
	{
		for(i=0;i<ns;i++)  //处理每个阶   其中只对两个和四个极点的处理，其余的不考虑
	    {
			if((*(a+i*(n+1)+3)==0)&&(*(a+i*(n+1)+4)==0))//对于其中只有两个个极点的处理
			{
				jltobl_each(2, 4, ns, i, b+i*(n+1), a+i*(n+1),  bb, lastb, lasta);
			}
			else //对于其中只有四个个极点的处理
			{
		        jltobl_each(4, 4, ns, i, b+i*(n+1), a+i*(n+1),  bb, lastb, lasta);
			}
	    }
	}
	else
	{
		free(aa);
	    free(bb);
		return -1;
	}

	if(hign_vlu!=0.0)//将多余的系数加到第一个多项式
	{
		for(i=0;i<n+1;i++)
			b[i]+=hign_vlu*a[i];
	}

	free(aa);
	free(bb);
	free(lasta);
	free(lastb);
	return 1;
}


//---------------------------------------切比雪夫I、II型-----------------------------------------
//----------------------计算滤波参数e--------------------
double Cal_cbI_e(int ap) 
{
	return sqrt(pow(10.0,0.1*ap)-1);
}
double Cal_cbII_e(int as)
{
	return 1/sqrt(pow(10.0,0.1*as)-1);
}
/*--------------------------------------计算阶数n--------------------------------
*/
int Cal_cbI_N_moni(int band, double f1, double f2, double f3, double f4, int ap, int as) 
{
	double B,hs1,hs2,hl,hu,h02,ls0,ls1;
	int N;

	switch(band)
	{
	case 1:
		N=(int)(acosh(sqrt((pow(10.0,0.1*as)-1)/(pow(10.0,0.1*ap)-1)))/acosh(f2/f1)+1);
		break;
	case 2:
		//f1=1/f1;
		//f2=1/f2;//相除抵消，故不再归一化
        N=(int)(acosh(sqrt((pow(10.0,0.1*as)-1)/(pow(10.0,0.1*ap)-1)))/acosh(f2/f1)+1);
		break;
	case 3:
		B=f3-f2;
        hs1=f1/B; hs2=f4/B; hl=f2/B; hu=f3/B; h02=hl*hu;
		ls0=(hs2*hs2-h02)/hs2;  //归一化
		ls1=(h02-hs1*hs1)/hs1;
		ls0=min(ls0,ls1);
        N=(int)(acosh(sqrt((pow(10.0,0.1*as)-1)/(pow(10.0,0.1*ap)-1)))/acosh(ls0)+1);  //lp=1
		break;
	case 4:
		B=f4-f1;
        hs1=f2/B; hs2=f3/B; hl=f1/B; hu=f4/B; h02=hl*hu;
		ls0=hs1/(h02-hs1*hs1);  //归一化
		ls1=hs2/(hs2*hs2-h02);
		ls0=min(ls0,ls1);
		N=(int)(acosh(sqrt((pow(10.0,0.1*as)-1)/(pow(10.0,0.1*ap)-1)))/acosh(ls0)+1);  //lp=1
		break;
	}
	return N*(band>=3? 2:1);
}
int Cal_cbII_N_moni(int band, double f1, double f2, double f3, double f4, int ap, int as) 
{
	double B,hs1,hs2,hl,hu,h02,ls0,ls1,e;
	int N;

	e=Cal_cbII_e(as);
	switch(band)
	{
	case 1:
		N=(int)(acosh(1/(e*sqrt((pow(10.0,0.1*ap)-1))))/acosh(f2/f1)+1);
		break;
	case 2:
		//f1=1/f1;
		//f2=1/f2;//相除抵消，故不再归一化
        N=(int)(acosh(1/(e*sqrt((pow(10.0,0.1*ap)-1))))/acosh(f2/f1)+1);
		break;
	case 3:
		B=f3-f2;
        hs1=f1/B; hs2=f4/B; hl=f2/B; hu=f3/B; h02=hl*hu;
		ls0=(hs2*hs2-h02)/hs2;  //归一化
		ls1=(h02-hs1*hs1)/hs1;
		ls0=min(ls0,ls1);
        N=(int)(acosh(1/(e*sqrt((pow(10.0,0.1*ap)-1))))/acosh(ls0)+1);  //lp=1
		break;
	case 4:
		B=f4-f1;
        hs1=f2/B; hs2=f3/B; hl=f1/B; hu=f4/B; h02=hl*hu;
		ls0=hs1/(h02-hs1*hs1);  //归一化
		ls1=hs2/(hs2*hs2-h02);
		ls0=min(ls0,ls1);
		N=(int)(acosh(1/(e*sqrt((pow(10.0,0.1*ap)-1))))/acosh(ls0)+1);  //lp=1
		break;
	}
	return N*(band>=3? 2:1);
}
int Cal_cbI_N_mch(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as) 
{
	double omg1,omg2,omg3,omg4;
    double pi;
	pi=4.0*atan(1.0);

	if((band==2)||(band==4))  //脉冲响应不变法只适合设计高通和带通
	{
		printf("脉冲响应不变法不适合设计高通和带阻");
		return -2;
	}

	omg1=2*pi*f1/fs/T;   
	omg2=2*pi*f2/fs/T;
	if(band>=3)
	{
	    omg3=2*pi*f3/fs/T;
	    omg4=2*pi*f4/fs/T;
	}
	else
	{
		omg3=0;
	    omg4=0;
	}
	return Cal_cbI_N_moni(band, omg1 ,omg2, omg3, omg4, ap, as);
}
int Cal_cbI_N_shxx(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as)
{
	double omg1,omg2,omg3,omg4;
	double pi;
	pi=4.0*atan(1.0);

    assert(fs>2*f1);assert(fs>2*f2);assert(fs>2*f3);assert(fs>2*f4);

  	omg1=2/T*tan(pi*f1/fs);    //2/T*tan(2*pi*f1/fs/2);
	omg2=2/T*tan(pi*f2/fs);
	if(band>=3)
	{
	    omg3=2/T*tan(pi*f3/fs);
	    omg4=2/T*tan(pi*f4/fs);
	}
	else
	{
		omg3=0;
	    omg4=0;
	}

	return Cal_cbI_N_moni(band, omg1 ,omg2, omg3, omg4, ap, as);
}
int Cal_cbII_N_mch(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as) 
{
	double omg1,omg2,omg3,omg4;
    double pi;
	pi=4.0*atan(1.0);

	if((band==2)||(band==4))  //脉冲响应不变法只适合设计高通和带通
	{
		printf("脉冲响应不变法不适合设计高通和带阻");
		return -2;
	}

	omg1=2*pi*f1/fs/T;   
	omg2=2*pi*f2/fs/T;
	if(band>=3)
	{
	    omg3=2*pi*f3/fs/T;
	    omg4=2*pi*f4/fs/T;
	}
	else
	{
		omg3=0;
	    omg4=0;
	}
	return Cal_cbII_N_moni(band, omg1 ,omg2, omg3, omg4, ap, as);
}
int Cal_cbII_N_shxx(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as)
{
	double omg1,omg2,omg3,omg4;
	double pi;
	pi=4.0*atan(1.0);

    assert(fs>2*f1);assert(fs>2*f2);assert(fs>2*f3);assert(fs>2*f4);

  	omg1=2/T*tan(pi*f1/fs);    //2/T*tan(2*pi*f1/fs/2);
	omg2=2/T*tan(pi*f2/fs);
	if(band>=3)
	{
	    omg3=2/T*tan(pi*f3/fs);
	    omg4=2/T*tan(pi*f4/fs);
	}
	else
	{
		omg3=0;
	    omg4=0;
	}

	return Cal_cbII_N_moni(band, omg1 ,omg2, omg3, omg4, ap, as);
}
int Cal_cbI_N(char bianhuan, int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as)
{
	if(bianhuan==0)
		return Cal_cbI_N_mch(band, T, fs, f1, f2, f3, f4, ap, as);
	else
		return Cal_cbI_N_shxx(band, T, fs, f1, f2, f3, f4, ap, as);

}
int Cal_cbII_N(char bianhuan, int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as)
{
	if(bianhuan==0)
		return Cal_cbII_N_mch(band, T, fs, f1, f2, f3, f4, ap, as);
	else
		return Cal_cbII_N_shxx(band, T, fs, f1, f2, f3, f4, ap, as);

}
//----------------------------------计算切比雪夫I型每阶的系数------------------------
//k     ------   0~(N-1)/2
char Cal_cbI_xishu(int N, int k, double e, double *b, double *a)
{
	double ok,omgk,bete;
    double pi;
	pi=4.0*atan(1.0);

	bete=asinh(1/e)/N;
	ok=-sinh(bete)*sin((2*(k+1)-1)*pi/(2*N));
	omgk=cosh(bete)*cos((2*(k+1)-1)*pi/(2*N));

	if((N%2!=0)&&(k==(N-1)/2)) //N是奇数且现在求的是最后一个的系数
	{
		b[0]=-ok;
	    b[1]=0.0;
      	b[2]=0.0;
    	a[0]=-ok;
	    a[1]=1.0;
	    a[2]=0.0;
		return 2;
	}
	else
	{
		b[0]=ok*ok+omgk*omgk;
	    b[1]=0.0;
      	b[2]=0.0;
    	a[0]=ok*ok+omgk*omgk;
	    a[1]=-2*ok;
	    a[2]=1.0;
	}
	return 3;
}
//k     ------   0~(N-1)/2
char Cal_cbII_xishu(int N, int k, double e, double *b, double *a)
{
	double ok,omgk,bete,qkre,qkim,qk2,zk2;
    double pi;
	pi=4.0*atan(1.0);

	bete=asinh(1/e)/N;
	ok=-sinh(bete)*sin((2*(k+1)-1)*pi/(2*N));
	omgk=cosh(bete)*cos((2*(k+1)-1)*pi/(2*N));

	qkre=ok/(ok*ok+omgk*omgk);
	qkim=-omgk/(ok*ok+omgk*omgk);
    qk2=qkre*qkre+qkim*qkim;
	zk2=1/cos((2*(k+1)-1)*pi/(2*N));
	zk2=zk2*zk2;

	if((N%2!=0)&&(k==(N-1)/2)) //N是奇数且现在求的是最后一个的系数
	{
		b[0]=-1/ok;
	    b[1]=0.0;
      	b[2]=0.0;
    	a[0]=-1/ok;
	    a[1]=1.0;
	    a[2]=0.0;
		return 2;
	}
	else
	{
		b[0]=qk2;
	    b[1]=0.0;
      	b[2]=qk2/zk2;
    	a[0]=qk2;
	    a[1]=-2*qkre;
	    a[2]=1.0;
	}
	return 3;
}
//----------------------------------计算切比雪夫I型的H(s)------------------------------
/*
int band     ---------滤波器类型
int N        ---------滤波器阶数
double omgc0 ---------当低通和高通时，存放omgc的值；带通和带阻时，存放omg0的值
double B     ---------当低通和高通时，无用；带通和带阻时，存放B的值
double e     ---------存放滤波器的参数e=sqrt(pow(10.0,0.01*ap)-1)
double *b    ---------滤波器系数的分子，其中以2阶或4阶的形式给出
double *a    ---------滤波器系数的分母，其中以2阶或4阶的形式给出

返回滤波器的增益K
*/
double Cal_cbI_hs_basic(int band, int N,  double e, double omgp0, double B,double *b, double *a)
{
	int i,num;
	double temp,P;

	switch(band)
	{
	case 1:
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_cbI_xishu(N, i, e, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				*(a+i*3+1)=*(a+i*3+1);
				*(a+i*3)=*(a+i*3)*omgp0;  
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=*(b+i*3+1);
				*(b+i*3)=*(b+i*3)*omgp0;
			}
			else
			{   //分母
				*(a+i*3+2)=*(a+i*3+2);
				*(a+i*3+1)=*(a+i*3+1)*omgp0;
				*(a+i*3)=*(a+i*3)*omgp0*omgp0;
				//分子
				*(b+i*3+2)=*(b+i*3+2);
				*(b+i*3+1)=*(b+i*3+1)*omgp0;
				*(b+i*3)=*(b+i*3)*omgp0*omgp0;
			}
		}
		break;
	case 2:
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_cbI_xishu(N, i, e, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				temp=*(a+i*3+1);
				*(a+i*3+1)=*(a+i*3);
				*(a+i*3)=temp*omgp0;  
				//分子
				*(b+i*3+2)=0.0;
				temp=*(b+i*3+1);
				*(b+i*3+1)=*(b+i*3);
				*(b+i*3)=temp*omgp0; 
			}
			else
			{   //分母
				temp=*(a+i*3+2);
				*(a+i*3+2)=*(a+i*3);
				*(a+i*3+1)=*(a+i*3+1)*omgp0;
				*(a+i*3)=temp*omgp0*omgp0;
				//分子
				temp=*(b+i*3+2);
				*(b+i*3+2)=*(b+i*3);
				*(b+i*3+1)=*(b+i*3+1)*omgp0;
				*(b+i*3)=temp*omgp0*omgp0;
			}
		}
		break;
	case 3:
		for(i=0;i<(N/2+1)/2;i++)  
		{
			num=Cal_cbI_xishu(N/2, i,e, b+i*5, a+i*5); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=*(a+i*5+1);
				temp=*(a+i*5+1);
				*(a+i*5+1)=*(a+i*5)*B;
				*(a+i*5)=temp*omgp0*omgp0;
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=*(b+i*5+1);
				temp=*(b+i*5+1);
				*(b+i*5+1)=*(b+i*5)*B;
				*(b+i*5)=temp*omgp0*omgp0;
			}
			else
			{   //分母
				temp=a[i*5+2];
				*(a+i*5+4)=a[i*5+2];
				*(a+i*5+3)=a[i*5+1]*B;
				*(a+i*5+2)=2*a[i*5+2]*omgp0*omgp0+a[i*5]*B*B;
				*(a+i*5+1)=*(a+i*5+1)*B*omgp0*omgp0;
				*(a+i*5)=temp*omgp0*omgp0*omgp0*omgp0;
	
				//分子
				temp=b[i*5+2];
				*(b+i*5+4)=b[i*5+2];
				*(b+i*5+3)=b[i*5+1]*B;
				*(b+i*5+2)=2*b[i*5+2]*omgp0*omgp0+b[i*5]*B*B;
				*(b+i*5+1)=*(b+i*5+1)*B*omgp0*omgp0;
				*(b+i*5)=temp*omgp0*omgp0*omgp0*omgp0;
			}
		}
		break;
	case 4:
		for(i=0;i<(N/2+1)/2;i++)  
		{
			num=Cal_cbI_xishu(N/2, i, e, b+i*5, a+i*5); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=*(a+i*5);
				*(a+i*5+1)=*(a+i*5+1)*B;
				*(a+i*5)=*(a+i*5)*omgp0*omgp0;  
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=*(b+i*5);
				*(b+i*5+1)=*(b+i*5+1)*B;
				*(b+i*5)=*(b+i*5)*omgp0*omgp0;  
			}
			else
			{   //分母
				*(a+i*5+4)=a[i*5];	
				*(a+i*5+3)=a[i*5+1]*B;	
				*(a+i*5+2)=2*a[i*5]*omgp0*omgp0+a[i*5+2]*B*B;
				*(a+i*5+1)=a[i*5+1]*B*omgp0*omgp0;
				*(a+i*5)=a[i*5]*omgp0*omgp0*omgp0*omgp0;	
	
				//分子
				*(b+i*5+4)=b[i*5];
				*(b+i*5+3)=b[i*5+1]*B;	
				*(b+i*5+2)=2*b[i*5]*omgp0*omgp0+b[i*5+2]*B*B;
				*(b+i*5+1)=b[i*5+1]*B*omgp0*omgp0;
				*(b+i*5)=b[i*5]*omgp0*omgp0*omgp0*omgp0;
			}
		}
		break;
	}

	if(band<=2)
	{
		if(N%2==0)
    		P=1/sqrt(1+e*e);
    	else
	    	P=1.0;
	}
	else
	{
		if(N/2%2==0)
    		P=1/sqrt(1+e*e);
    	else
	    	P=1.0;
	}

	return P;
}
double Cal_cbII_hs_basic(int band, int N,  double e, double omgs0, double B,double *b, double *a)
{
	int i,num;
	double temp;

	switch(band)
	{
	case 1:
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_cbII_xishu(N, i, e, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				*(a+i*3+1)=*(a+i*3+1);
				*(a+i*3)=*(a+i*3)*omgs0;  
				//分子
				*(b+i*3+2)=0.0;
				*(b+i*3+1)=*(b+i*3+1);
				*(b+i*3)=*(b+i*3)*omgs0;
			}
			else
			{   //分母
				*(a+i*3+2)=*(a+i*3+2);
				*(a+i*3+1)=*(a+i*3+1)*omgs0;
				*(a+i*3)=*(a+i*3)*omgs0*omgs0;
				//分子
				*(b+i*3+2)=*(b+i*3+2);
				*(b+i*3+1)=*(b+i*3+1)*omgs0;
				*(b+i*3)=*(b+i*3)*omgs0*omgs0;
			}
		}
		break;
	case 2:
		for(i=0;i<(N+1)/2;i++)
		{
			num=Cal_cbII_xishu(N, i, e, b+i*3, a+i*3);
			if(num==2) //最后一个
			{    //分母
				*(a+i*3+2)=0.0;
				temp=*(a+i*3+1);
				*(a+i*3+1)=*(a+i*3);
				*(a+i*3)=temp*omgs0;  
				//分子
				*(b+i*3+2)=0.0;
				temp=*(b+i*3+1);
				*(b+i*3+1)=*(b+i*3);
				*(b+i*3)=temp*omgs0; 
			}
			else
			{   //分母
				temp=*(a+i*3+2);
				*(a+i*3+2)=*(a+i*3);
				*(a+i*3+1)=*(a+i*3+1)*omgs0;
				*(a+i*3)=temp*omgs0*omgs0;
				//分子
				temp=*(b+i*3+2);
				*(b+i*3+2)=*(b+i*3);
				*(b+i*3+1)=*(b+i*3+1)*omgs0;
				*(b+i*3)=temp*omgs0*omgs0;
			}
		}
		break;
	case 3:
		for(i=0;i<(N/2+1)/2;i++)  
		{
			num=Cal_cbII_xishu(N/2, i,e, b+i*5, a+i*5); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=*(a+i*5+1);
				temp=*(a+i*5+1);
				*(a+i*5+1)=*(a+i*5)*B;
				*(a+i*5)=temp*omgs0*omgs0;
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=*(b+i*5+1);
				temp=*(b+i*5+1);
				*(b+i*5+1)=*(b+i*5)*B;
				*(b+i*5)=temp*omgs0*omgs0;
			}
			else
			{   //分母
				temp=a[i*5+2];
				*(a+i*5+4)=a[i*5+2];
				*(a+i*5+3)=a[i*5+1]*B;
				*(a+i*5+2)=2*a[i*5+2]*omgs0*omgs0+a[i*5]*B*B;
				*(a+i*5+1)=*(a+i*5+1)*B*omgs0*omgs0;
				*(a+i*5)=temp*omgs0*omgs0*omgs0*omgs0;
	
				//分子
				temp=b[i*5+2];
				*(b+i*5+4)=b[i*5+2];
				*(b+i*5+3)=b[i*5+1]*B;
				*(b+i*5+2)=2*b[i*5+2]*omgs0*omgs0+b[i*5]*B*B;
				*(b+i*5+1)=*(b+i*5+1)*B*omgs0*omgs0;
				*(b+i*5)=temp*omgs0*omgs0*omgs0*omgs0;
			}
		}
		break;
	case 4:
		for(i=0;i<(N/2+1)/2;i++)  
		{
			num=Cal_cbII_xishu(N/2, i, e, b+i*5, a+i*5); //将归一化低通系数填入5个系数数组中低的3个
			if(num==2) //最后一个
			{    //分母
				*(a+i*5+4)=0.0;
				*(a+i*5+3)=0.0;
				*(a+i*5+2)=*(a+i*5);
				*(a+i*5+1)=*(a+i*5+1)*B;
				*(a+i*5)=*(a+i*5)*omgs0*omgs0;  
				//分子
				*(b+i*5+4)=0.0;
				*(b+i*5+3)=0.0;
				*(b+i*5+2)=*(b+i*5);
				*(b+i*5+1)=*(b+i*5+1)*B;
				*(b+i*5)=*(b+i*5)*omgs0*omgs0;  
			}
			else
			{   //分母
				*(a+i*5+4)=a[i*5];	
				*(a+i*5+3)=a[i*5+1]*B;	
				*(a+i*5+2)=2*a[i*5]*omgs0*omgs0+a[i*5+2]*B*B;
				*(a+i*5+1)=a[i*5+1]*B*omgs0*omgs0;
				*(a+i*5)=a[i*5]*omgs0*omgs0*omgs0*omgs0;	
	
				//分子
				*(b+i*5+4)=b[i*5];
				*(b+i*5+3)=b[i*5+1]*B;	
				*(b+i*5+2)=2*b[i*5]*omgs0*omgs0+b[i*5+2]*B*B;
				*(b+i*5+1)=b[i*5+1]*B*omgs0*omgs0;
				*(b+i*5)=b[i*5]*omgs0*omgs0*omgs0*omgs0;	
			}
		}
		break;
	}

	return 1.0;
}
/*----------------------------------用双线性变化法设计切比雪夫I型-----------------------
*/
double Cal_cbI_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
    double pi,K;
	double B,omg02,omg1,omg2,e;
	pi=4.0*atan(1.0);
	
	e=Cal_cbI_e(ap);

	switch(band)
	{
	case 1:  
		K=Cal_cbI_hs_basic(band, N, e, f1, 0, b, a);
		break;
	case 2: 
		K=Cal_cbI_hs_basic(band, N, e, f2, 0, b, a);
		break;
	case 3:
		assert(N%2==0);
		omg1=f3;    //2/T*tan(2*pi*f1/fs/2);
	    omg2=f2;
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);

		K=Cal_cbI_hs_basic(band, N, e, omg02, B, b, a);
		break;
	case 4:
		assert(N%2==0);
		omg1=f4;    //2/T*tan(2*pi*f1/fs/2);
	    omg2=f1;
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);
		K=Cal_cbI_hs_basic(band, N, e, omg02, B, b, a);
		break;
	}
	return K;  
}
double Cal_cbII_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
    double pi,K;
	double B,omg02,omg1,omg2,e;
	pi=4.0*atan(1.0);
	
	e=Cal_cbII_e(as);

	switch(band)
	{
	case 1: 
		f2=cosh(acosh(1.0/e/sqrt(pow(10.0,0.1*ap)-1))/N)*f1;  //优化????????????????  
		K=Cal_cbII_hs_basic(band, N, e, f2, 0, b, a);
		break;
	case 2: 
		f1=1/(cosh(acosh(1.0/e/sqrt(pow(10.0,0.1*ap)-1.0))/N)*(1/f2));  //优化???????????????? 
		K=Cal_cbII_hs_basic(band, N, e, f1, 0, b, a);
		break;
	case 3:
		assert(N%2==0);
		omg1=f4;    
	    omg2=f1;
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);

		/*f2=(f2*f2-omg02*omg02)/B/f2;
		f3=(f3*f3-omg02*omg02)/B/f3;
		f2=cosh(acosh(1.0/e/sqrt(pow(10.0,0.1*ap)-1))/N)*f2;
		f2=(B*f2+sqrt(B*B*f2*f2+4*omg02*omg02))/2;
		f3=cosh(acosh(1.0/e/sqrt(pow(10.0,0.1*ap)-1))/N)*f3;
		f3=(B*f3+sqrt(B*B*f3*f3+4*omg02*omg02))/2;
		B=f3-f2;
		omg02=sqrt(f2*f3);*/
		K=Cal_cbII_hs_basic(band, N, e, omg02, B, b, a);
		break;
	case 4:
		assert(N%2==0);
		omg1=f3; 
	    omg2=f2;
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);
		K=Cal_cbII_hs_basic(band, N, e, omg02, B, b, a);
		break;
	}
	return K;  
}

double Cal_cbI_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
    double pi,K;
	double B,omg02,omg1,omg2,e;
	pi=4.0*atan(1.0);
	
	e=Cal_cbI_e(ap);

	switch(band)
	{
	case 1:  //低通p=s/omgc
	    f1=2/T*tan(pi*f1/fs);    //2/T*tan(2*pi*f1/fs/2); 
		K=Cal_cbI_hs_basic(band, N, e, f1, 0, b, a);
		break;
	case 2:  //高通p=omgc/s
	    f2=2/T*tan(pi*f2/fs);    //2/T*tan(2*pi*f1/fs/2);
		K=Cal_cbI_hs_basic(band, N, e, f2, 0, b, a);
		break;
	case 3:
		assert(N%2==0);
		omg1=2/T*tan(pi*f3/fs);    //2/T*tan(2*pi*f1/fs/2);
	    omg2=2/T*tan(pi*f2/fs);
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);

		K=Cal_cbI_hs_basic(band, N, e, omg02, B, b, a);
		break;
	case 4:
		assert(N%2==0);
		omg1=2/T*tan(pi*f4/fs);    //2/T*tan(2*pi*f1/fs/2);
	    omg2=2/T*tan(pi*f1/fs);
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);
		K=Cal_cbI_hs_basic(band, N, e, omg02, B, b, a);
		break;
	}
	return K;  
}

double Cal_cbII_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
    double pi,K;
	double B,omg02,omg1,omg2,e;
	pi=4.0*atan(1.0);
	
	e=Cal_cbII_e(as);

	switch(band)
	{
	case 1:  //低通p=s/omgc
	    f2=2/T*tan(pi*f2/fs);    
		f1=2/T*tan(pi*f1/fs);                               //优化????????????????
		f2=cosh(acosh(1.0/e/sqrt(pow(10.0,0.1*ap)-1))/N)*f1;  //优化????????????????  
		K=Cal_cbII_hs_basic(band, N, e, f2, 0, b, a);//优化????????????????  
		break;
	case 2:  //高通p=omgc/s
	    f1=2/T*tan(pi*f1/fs);    
		f2=2/T*tan(pi*f2/fs);           //优化????????????????
		f1=1/(cosh(acosh(1.0/e/sqrt(pow(10.0,0.1*ap)-1.0))/N)*(1/f2));  //优化????????????????  
		K=Cal_cbII_hs_basic(band, N, e, f1, 0, b, a); //优化????????????????  
		break;
	case 3:
		assert(N%2==0);
		omg1=2/T*tan(pi*f4/fs);    
	    omg2=2/T*tan(pi*f1/fs);
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);
		//new_ws=1/(cosh(acosh(1.0/e/sqrt(pow(10.0,0.1*ap)-1.0))/N)*1)*B;  //优化????????????????  f1
		K=Cal_cbII_hs_basic(band, N, e, omg02, B, b, a);
		break;
	case 4:
		assert(N%2==0);
		omg1=2/T*tan(pi*f3/fs);    
	    omg2=2/T*tan(pi*f2/fs);
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);
		K=Cal_cbII_hs_basic(band, N, e, omg02, B, b, a);
		break;
	}
	return K;  
}

/*----------------------------------用脉冲响应不变法设计切比雪夫I型-------------------------
*/
double Cal_cbI_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a)
{
    double pi,K;
	double B,omg1,omg2,omg02,e;
	pi=4.0*atan(1.0);
	
	if((band==2)||(band==4))  //脉冲响应不变法只适合设计高通和带通
	{
		printf("脉冲响应不变法不适合设计高通和带阻");
		return -2;
	}

	e=Cal_cbI_e(ap);
	switch(band)
	{
	case 1:  //低通p=s/omgc
	    f1=2*pi*f1/fs/T;    //  2*pi*f1/fs/T
		K=Cal_cbI_hs_basic(band, N, e, f1, 0, b, a);
		break;
	/*case 2:  //高通p=omgc/s
	    f2=2*pi*f2/fs/T;
		K=Cal_cbI_hs_basic(band, N, e, f2, 0, b, a);
		break;*/
	case 3:
		assert(N%2==0);
		omg1=2*pi*f3/fs/T;    //  2*pi*f1/fs/T
	    omg2=2*pi*f2/fs/T;
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);

		K=Cal_cbI_hs_basic(band, N, e, omg02, B, b, a);
		break;
	/*case 4:
		assert(N%2==0);
		omg1=2*pi*f3/fs/T;    //  2*pi*f1/fs/T
	    omg2=2*pi*f2/fs/T;
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);

		K=Cal_cbI_hs_basic(band, N, e, omg02, B, b, a);
		break;*/
	}
	return K; 
}

double Cal_cbII_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a)
{
    double pi,K;
	double B,omg1,omg2,omg02,e;
	pi=4.0*atan(1.0);
	
	if((band==2)||(band==4))  //脉冲响应不变法只适合设计高通和带通
	{
		printf("脉冲响应不变法不适合设计高通和带阻");
		return -2;
	}

	e=Cal_cbI_e(ap);
	switch(band)
	{
	case 1:  
	    f2=2*pi*f2/fs/T;  
		f1=2*pi*f1/fs/T;      //优化???????????????? 
		f2=cosh(acosh(1.0/e/sqrt(pow(10.0,0.1*ap)-1))/N)*f1;  //优化????????????????  
		K=Cal_cbII_hs_basic(band, N, e, f2, 0, b, a);
		break;
	/*case 2:  //高通p=omgc/s
	    f2=2*pi*f2/fs/T;
		K=Cal_cbII_hs_basic(band, N, e, f2, 0, b, a);
		break;*/
	case 3:
		assert(N%2==0);
		omg1=2*pi*f4/fs/T;   
	    omg2=2*pi*f1/fs/T;
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);

		K=Cal_cbII_hs_basic(band, N, e, omg02, B, b, a);
		break;
	/*case 4:
		assert(N%2==0);
		omg1=2*pi*f3/fs/T;    //  2*pi*f1/fs/T
	    omg2=2*pi*f2/fs/T;
		B=omg1-omg2;
		omg02=sqrt(omg1*omg2);

		K=Cal_cbII_hs_basic(band, N, e, omg02, B, b, a);
		break;*/
	}
	return K; 
}

double Cal_cbI_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
	if(bianhuan==0)
		return  Cal_cbI_Hs_mch(band, N, T, fs, f1, f2, f3, f4, ap, as, b, a);
	else
		return  Cal_cbI_Hs_shxx(band, N, T, fs, f1, f2, f3, f4, ap, as, b, a);
} 
double Cal_cbII_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
	if(bianhuan==0)
		return  Cal_cbII_Hs_mch(band, N, T, fs, f1, f2, f3, f4, ap, as, b, a);
	else
		return  Cal_cbII_Hs_shxx(band, N, T, fs, f1, f2, f3, f4, ap, as, b, a);
} 

//-------------------------------------椭圆滤波器的设计-----------------------------------------
//第I类完全椭圆积分
//n      -----------积分采样点数
double Cal_elliptic_K(double x, int n)
{
	int i;
	double pi,tht,val;
	pi=4.0*atan(1.0);

	val=0.0;
	for(i=0;i<n;i++)
	{
		tht=i*pi/2/(n-1);
		val+=1/sqrt(1-x*x*sin(tht)*sin(tht))*pi/2/(n-1);
	}
	return val;
}
//从N和k1求出k
double Cal_k1tok(int N, double k1)
{
	double K1p,k1p,sn,ui,k;
	int i,L;
    L=N/2;
	K1p=Complete_Elliptic_Integral_First_Kind('k', sqrt(1-k1*k1));
	k1p=sqrt(1-k1*k1);

	k=pow(k1p,N);
	for(i=1;i<=L;i++)
	{
		ui=(double)(2*i-1)/(double)N;
	    sn=Jacobi_sn(ui*K1p,'k', k1p);
		sn=pow(sn,4);
		k*=sn;
	}
	return  k=sqrt(1-k*k); 
}
//从N和k求出k1
double Cal_ktok1(int N, double k)
{
	double sn,ui,k1,K;
	int i,L;
    L=N/2;

	K=Complete_Elliptic_Integral_First_Kind('k', k);   
	k1=pow(k,N);
	for(i=1;i<=L;i++)
	{
		ui=(double)(2*i-1)/(double)N;
	    sn=Jacobi_sn(ui*K,'k', k);
		sn=pow(sn,4);
		k1*=sn;
	}
	return  k1; 
}
//------------------------------------------------------------------------------------------------------
int Cal_elliptic_N_basic(double wp, double ws, double ap, double as)
{
	double k,k1,ep,es,K,Kp,K1,K1p;
	int N;

	if(as>160)
		return -2;
	k=wp/ws;
	ep=sqrt(pow((long double)10.0,(long double)(0.1*ap))-1);
	es=sqrt(pow((long double)10.0,(long double)(0.1*as))-1);
	k1=ep/es;
    K=Complete_Elliptic_Integral_First_Kind('k', k);   
	Kp=Complete_Elliptic_Integral_First_Kind('k', sqrt(1.0-k*k));
	K1=Complete_Elliptic_Integral_First_Kind('k', k1);
	K1p=Complete_Elliptic_Integral_First_Kind('k', sqrt(1.0-k1*k1));
    N=(int)(K1p*K/K1/Kp+1);

	return N; 
} 
int Cal_elliptic_N_moni(int band, double f1, double f2, double f3, double f4, int ap, int as)
{
	double wp,ws;
	double B,hs1,hs2,hl,hu,h02,ls0,ls1;
	int N;

	if(as>160)
		return -3;

	if(band==1)
	{
		wp=f1;
		ws=f2;
	}
	else if(band==2)
	{
		f1=1/f1;
		f2=1/f2; 
		wp=f2;
		ws=f1;
	}
	else if(band==3)
	{
		B=f3-f2;
        hs1=f1/B; hs2=f4/B; hl=f2/B; hu=f3/B; h02=hl*hu;
		ls0=(hs2*hs2-h02)/hs2;  //归一化
		ls1=(h02-hs1*hs1)/hs1;
		ls0=min(ls0,ls1);
		wp=1;
		ws=ls0;
	}
	else if(band==4)
	{
		B=f4-f1;  
        hs1=f2/B; hs2=f3/B; hl=f1/B; hu=f4/B; h02=hl*hu;
		ls0=hs1/(h02-hs1*hs1); 
		ls1=hs2/(hs2*hs2-h02);
		ls0=min(ls0,ls1);
		wp=1;
		ws=ls0;
	}

	N=Cal_elliptic_N_basic(wp,ws,ap,as);

	return N*(band>=3? 2:1);
}
int Cal_elliptic_N_mch(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as) 
{
    double pi;
	int N;
	pi=4.0*atan(1.0);

	if((band==2)||(band==4))  //脉冲响应不变法只适合设计高通和带通
	{
		printf("脉冲响应不变法不适合设计高通和带阻");
		return -2;
	}

	if(band==1)
	{
		f1=2*pi*f1/fs/T;   
	    f2=2*pi*f2/fs/T;
	    f3=0;
	    f4=0;
	}
	else if(band==3)
	{
		f1=2*pi*f1/fs/T;   
	    f2=2*pi*f2/fs/T;
		f3=2*pi*f3/fs/T;
	    f4=2*pi*f4/fs/T;	
	}
	N=Cal_elliptic_N_moni(band, f1, f2, f3, f4, ap,as);
	return N;
}
int Cal_elliptic_N_shxx(int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as)
{
    double pi;
	int N;
	pi=4.0*atan(1.0);

	if(band==1)
	{
		f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs); 
		f3=0.0;
		f4=0.0;	
	}
	else if(band==2)
	{
		f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs);  
		f3=0.0;
		f4=0.0;
	}
	else if(band==3)
	{
		f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs); 
		f3=2/T*tan(pi*f3/fs); 
		f4=2/T*tan(pi*f4/fs); 
	}
	else if(band==4)
	{
		f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs); 
		f3=2/T*tan(pi*f3/fs); 
		f4=2/T*tan(pi*f4/fs); 
	}
	N=Cal_elliptic_N_moni(band, f1, f2, f3, f4, ap,as);

	return N;
}
int Cal_elliptic_N(char bianhuan, int band, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as)
{
	if(bianhuan==0)
		return  Cal_elliptic_N_mch(band, T, fs, f1, f2, f3, f4, ap, as);
	else
		return  Cal_elliptic_N_shxx(band, T, fs, f1, f2, f3, f4, ap, as);
}
/*-----------------------------------------计算椭圆滤波器的系数-------------------------------------------
int     n    ------阶数
double  wp   ------通带截至频率
double  ws   ------阻带截至频率
double  ap   ------通带最大衰减
double  as   ------阻带最小衰减
int     nn   ------每次存放系数的偏移地址3或5
double  *a   ------分子系数数组，长度为n奇数：(n+1)/2×3 偶数 n/2×3
double  *b   ------分母系数数组，长度为n奇数：(n+1)/2×3 偶数 n/2×3
                   b2s^2+b0*ai
            ----------------------
			     a2*s^2+a1*s+a0

		                     b0
 如果n是奇数，最后一项   ---------  我们令b0=1.0,a1=1.0,a0=p0  就正好和其他的相同形式，自己记得特殊处理就好了
		                   a1*s+a0
*/
//返回H0
double Cal_elliptic_xishu(int N, double wp, double ws, double ap, double as, int nn, double *b, double *a)
{
	double k,k1,K,K1,ep,es,ui,temp,re,im,v0,H0;
    int  L,r,i;

    k=wp/ws;
	ep=sqrt(pow(10.0,0.1*ap)-1);
	es=sqrt(pow(10.0,0.1*as)-1);
	k1=ep/es;
    K=Complete_Elliptic_Integral_First_Kind('k', k);   
	//Kp=Complete_Elliptic_Integral_First_Kind('k', sqrt(1-k*k));
	K1=Complete_Elliptic_Integral_First_Kind('k', k1);
	//K1p=Complete_Elliptic_Integral_First_Kind('k',sqrt(1-k1*k1));

    //新k
	k=Cal_k1tok(N, k1); 
	K=Complete_Elliptic_Integral_First_Kind('k', k);   
	//ws=wp/k;

    /*//新k1
    k1=Cal_ktok1(N,k);   
	K1=Complete_Elliptic_Integral_First_Kind('k', k1);
	//新ep
    ep=k1*sqrt(pow(10.0,0.1*as)-1);*/

    L=N/2;
	r=N%2;

	v0=Inverse_Jacobi_cn(ep/sqrt(1+ep*ep), 'k', sqrt(1-k1*k1));   
	v0=v0/N/K1;

	for(i=0;i<L;i++)
	{
		ui=(double)(2*(i+1)-1)/(double)N;
		Jacobi_sd_cd_nd(ui*K, 'k', k, &temp, &im, &temp); 	
		re=0.0;
		im=1/(im*k);

		b[i*nn]=1.0;
		b[i*nn+1]=0.0;
		b[i*nn+2]=1/(re*re+im*im);

        complex_Jacobi_cd(ui*K, -v0*K, 'k', k, &re, &im);
		temp=re;
        re=-im;
		im=temp;

		a[i*nn]=1.0; 
		a[i*nn+1]=re==0? 0.0: -2*re/(re*re+im*im);
		a[i*nn+2]=1/(re*re+im*im);

	}
	if(r!=0)
	{
		b[i*nn]=1.0;
		b[i*nn+1]=0.0;
		b[i*nn+2]=0.0;

		complex_Jacobi_sn(0, v0*K, 'k', k, &re, &im);
		assert(re<0.0000000001);
		temp=re;
        re=-im;
		im=temp;

		a[i*nn]=1.0;
		a[i*nn+1]=-1/re;
		a[i*nn+2]=0.0;	
	}

   if(r==0)
		H0=1/sqrt(1+ep*ep);  //pow(10.0,-ap/20);
	else
		H0=1;

	return H0;
}
double Cal_elliptic_hs_basic(int band, int N,  double f1, double f2, double f3, double f4,  double ap, double as, double *b, double *a)
{
	int i;
	double temp,aa, H0;
    double B,omg02;
	double hs1,hs2,hl,hu,h02,ls0,ls1;
    //char date[100];

	switch(band)
	{
	case 1:
		aa=f1;
		H0=Cal_elliptic_xishu(N, f1, f2, ap, as, 3, b, a);

		for(i=0;i<(N+1)/2;i++)
		{
			if((N%2!=0)&&(i==(N-1)/2))
			{
				 //分母
	    		*(a+i*3+2)=0.0;
	    		*(a+i*3+1)=*(a+i*3+1);
		    	*(a+i*3)=*(a+i*3)*aa;  
		    	//分子
	    		*(b+i*3+2)=0.0;
	    		*(b+i*3+1)=*(b+i*3+1);
	    		*(b+i*3)=*(b+i*3)*aa;
			}
			else
			{
			     //分母
				*(a+i*3+2)=*(a+i*3+2);
		    	*(a+i*3+1)=*(a+i*3+1)*aa;
		    	*(a+i*3)=*(a+i*3)*aa*aa;
		     	//分子
		    	*(b+i*3+2)=*(b+i*3+2);
		    	*(b+i*3+1)=*(b+i*3+1)*aa;
		    	*(b+i*3)=*(b+i*3)*aa*aa;
			}
		}

		break;
	case 2:
		aa=f2;
		f1=1/f1;
		f2=1/f2; 
  
		H0=Cal_elliptic_xishu(N, f2, f1, ap, as, 3, b, a);
	   
		for(i=0;i<(N+1)/2;i++)
		{
			if((N%2!=0)&&(i==(N-1)/2))
			{
				 //分母
		    	*(a+i*3+2)=0.0;
	    		temp=*(a+i*3+1);
	    		*(a+i*3+1)=*(a+i*3);
	    		*(a+i*3)=temp*aa;  
    			//分子
    			*(b+i*3+2)=0.0;
    			temp=*(b+i*3+1);
	    		*(b+i*3+1)=*(b+i*3);
	    		*(b+i*3)=temp*aa; 
			}
			else
			{
                //分母
	    		temp=*(a+i*3+2);
	    		*(a+i*3+2)=*(a+i*3);
	    		*(a+i*3+1)=*(a+i*3+1)*aa;
    			*(a+i*3)=temp*aa*aa;
    			//分子
    			temp=*(b+i*3+2);
    			*(b+i*3+2)=*(b+i*3);
    			*(b+i*3+1)=*(b+i*3+1)*aa;
    			*(b+i*3)=temp*aa*aa;
			}
		}
		/* for(i=0;i<(N+1)/2;i++)
    	{
            sprintf(date,"b[%d]=%f\tb[%d]=%f\tb[%d]=%f\n",i*3+2,b[i*3+2], i*3+1, b[i*3+1], i*3, b[i*3]);
            OutputDebugString(date);
			sprintf(date,"a[%d]=%f\ta[%d]=%f\ta[%d]=%f\n",i*3+2,a[i*3+2], i*3+1, a[i*3+1],  i*3, a[i*3]);
            OutputDebugString(date);
	    }*/

		break;
	case 3:
		 //变为低通、归一化
		B=f3-f2;
        hs1=f1/B; hs2=f4/B; hl=f2/B; hu=f3/B; h02=hl*hu;
		ls0=(hs2*hs2-h02)/hs2;  //归一化
		ls1=(h02-hs1*hs1)/hs1;
		ls0=min(ls0,ls1);
		H0=Cal_elliptic_xishu(N/2, 1, ls0, ap, as, 5, b, a);  //将归一化低通系数填入5个系数数组中低的3个
		omg02=sqrt(f2*f3);
		for(i=0;i<(N/2+1)/2;i++)  
		{ 
			if((N/2%2!=0)&&(i==(N/2-1)/2))
			{
				//分母
	    		*(a+i*5+4)=0.0;
	    		*(a+i*5+3)=0.0;
	    		*(a+i*5+2)=*(a+i*5+1);
	    		temp=*(a+i*5+1);
	    		*(a+i*5+1)=*(a+i*5)*B;
	    		*(a+i*5)=temp*omg02*omg02;
				//分子
	    		*(b+i*5+4)=0.0;
	    		*(b+i*5+3)=0.0;
	    		*(b+i*5+2)=*(b+i*5+1);
		    	temp=*(b+i*5+1);
	    		*(b+i*5+1)=*(b+i*5)*B;
	    		*(b+i*5)=temp*omg02*omg02;
			}
			else
			{
                 //分母
	    		temp=a[i*5+2];
	    		*(a+i*5+4)=a[i*5+2];
	    		*(a+i*5+3)=a[i*5+1]*B;
	    		*(a+i*5+2)=2*a[i*5+2]*omg02*omg02+a[i*5]*B*B;
	    		*(a+i*5+1)=*(a+i*5+1)*B*omg02*omg02;
	    		*(a+i*5)=temp*omg02*omg02*omg02*omg02;
	
    			//分子
    			temp=b[i*5+2];
    			*(b+i*5+4)=b[i*5+2];
    			*(b+i*5+3)=b[i*5+1]*B;
    			*(b+i*5+2)=2*b[i*5+2]*omg02*omg02+b[i*5]*B*B;
	    		*(b+i*5+1)=*(b+i*5+1)*B*omg02*omg02;
	    		*(b+i*5)=temp*omg02*omg02*omg02*omg02;
			}
		}
		break;
	case 4:
		 //变为低通、归一化
		B=f4-f1;  
        hs1=f2/B; hs2=f3/B; hl=f1/B; hu=f4/B; h02=hl*hu;
		ls0=hs1/(h02-hs1*hs1); 
		ls1=hs2/(hs2*hs2-h02);
		ls0=min(ls0,ls1);
		H0=Cal_elliptic_xishu(N/2, 1, ls0, ap, as, 5, b, a);  //将归一化低通系数填入5个系数数组中低的3个
		omg02=sqrt(f1*f4);
		for(i=0;i<(N/2+1)/2;i++)  
		{
			if((N/2%2!=0)&&(i==(N/2-1)/2))
			{
				//分母
	    		*(a+i*5+4)=0.0;
	    		*(a+i*5+3)=0.0;
	    		*(a+i*5+2)=*(a+i*5);
	    		*(a+i*5+1)=*(a+i*5+1)*B;
	     		*(a+i*5)=*(a+i*5)*omg02*omg02;  
	    		//分子
	      		*(b+i*5+4)=0.0;
	    		*(b+i*5+3)=0.0;
	    		*(b+i*5+2)=*(b+i*5);
	    		*(b+i*5+1)=*(b+i*5+1)*B;
		    	*(b+i*5)=*(b+i*5)*omg02*omg02;  
			}
			else
			{
                //分母
	    		*(a+i*5+4)=a[i*5];	
	    		*(a+i*5+3)=a[i*5+1]*B;	
	    		*(a+i*5+2)=2*a[i*5]*omg02*omg02+a[i*5+2]*B*B;
	    		*(a+i*5+1)=a[i*5+1]*B*omg02*omg02;
	    		*(a+i*5)=a[i*5]*omg02*omg02*omg02*omg02;	
	
	    		//分子
	    		*(b+i*5+4)=b[i*5];
	    		*(b+i*5+3)=b[i*5+1]*B;	
	    		*(b+i*5+2)=2*b[i*5]*omg02*omg02+b[i*5+2]*B*B;
	    		*(b+i*5+1)=b[i*5+1]*B*omg02*omg02;
	    		*(b+i*5)=b[i*5]*omg02*omg02*omg02*omg02;	
			}
		}
		break;
	}

	return H0;
}
double Cal_elliptic_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
    double pi,K;
	pi=4.0*atan(1.0);
	
	switch(band)
	{
	case 1:  //低通
		f3=0.0;
		f4=0.0;
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	case 2:  //高通
		f3=0.0;
		f4=0.0;
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	case 3:
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	case 4:
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	}
	return K; 
}
double Cal_elliptic_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
    double pi,K;
	pi=4.0*atan(1.0);
	
	switch(band)
	{
	case 1:  //低通
	    f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs); 
		f3=0.0;
		f4=0.0;
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	case 2:  //高通
	    f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs);  
		f3=0.0;
		f4=0.0;
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	case 3:
		f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs); 
		f3=2/T*tan(pi*f3/fs); 
		f4=2/T*tan(pi*f4/fs); 
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	case 4:
		f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs); 
		f3=2/T*tan(pi*f3/fs); 
		f4=2/T*tan(pi*f4/fs); 
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	}
	return K; 
}
double Cal_elliptic_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a)
{
    double pi,K;
	pi=4.0*atan(1.0);
	
	if((band==2)||(band==4))  //脉冲响应不变法只适合设计高通和带通
	{
		printf("脉冲响应不变法不适合设计高通和带阻");
		return -2;
	}

	switch(band)
	{
	case 1:  //低通
	    f1=2*pi*f1/fs/T;    
		f2=2*pi*f2/fs/T;
		f3=0.0;
		f4=0.0;
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	/*case 2:  //高通
	    f1=2*pi*f1/fs/T; 
		f2=2*pi*f2/fs/T;   
		f3=0.0;
		f4=0.0;
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;*/
	case 3:
		f1=2*pi*f1/fs/T;
		f2=2*pi*f2/fs/T;
		f3=2*pi*f3/fs/T;    
		f4=2*pi*f4/fs/T; 
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	/*case 4:
		f1=2*pi*f1/fs/T;
		f2=2*pi*f2/fs/T;
		f3=2*pi*f3/fs/T;    
		f4=2*pi*f4/fs/T; 
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;*/
	}
	return K; 
}

double Cal_elliptic_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a)
{
	if(bianhuan==0)
		return  Cal_elliptic_Hs_mch(band, N, T, fs, f1, f2, f3, f4, ap, as, b, a);
	else
		return  Cal_elliptic_Hs_shxx(band, N, T, fs, f1, f2, f3, f4, ap, as, b, a);
}
//----------------------------------贝塞尔滤波器的设计------------------------------------------
/*-----------------------计算贝塞尔多项式系数------------------------------
*/
double bessel_ak(int n, int k)
{
	int i;
	long double val;

	val=n-k+1;
	for(i=n-k+2;i<=2*n-k;i++)
	{
		val*=i;
		//sprintf(date,"i=%d\t%lf\n",i,val);
        //OutputDebugString(date);
	}
	val=val/(int)pow((double)2.0,n-k);
	for(i=1;i<=k;i++)
		val=val/i;
	return val;
}
//-------------------------计算贝塞尔滤波器系数-----------------------------
/*--------------------------------------将直接形式的转换成级联形式(只适用与贝塞尔转换n==2)------------------------------
N     ---------阶数
b     ----------滤波器分子多项数组，长度N+1
a     ----------滤波器分母多项数组，长度N+1
n     ----------转换后每节的阶数n
bb    ----------转换后分子系数二维数组，长度N/n*(n+1)
aa    ----------转换后分母系数二维数组，长度N/n*(n+1)
*/
char zhjtojl_bessel(int N, double *b, double *a, int n, double *bb, double *aa)
{
	///char date[100];
	int i,k,conut=N;
	double tempre,tempim;
	double *ljdianre=malloc(N*sizeof(double));
	double *ljdianim=malloc(N*sizeof(double));
	assert(n==2);
	//分母
	if(qrrt(N, a, ljdianre, ljdianim, 0.0000001, 300)<0) 
		   return -1;
    //将极点共轭成对排列,这种排序要不是贝塞尔的极点分布，应该改写
	conut=N;
    for(i=0; i<N-1;i++)   
	{
		for(k=1;k<conut;k++)
		{
			if(ljdianre[k]>ljdianre[k-1])
			{
				tempre=ljdianre[k-1];
			    tempim=ljdianim[k-1];
				ljdianre[k-1]=ljdianre[k];
			    ljdianim[k-1]=ljdianim[k];
				ljdianre[k]=tempre;
				ljdianim[k]=tempim;
			}
			else if((ljdianre[k]==ljdianre[k-1]))
			{
				if(fabs(ljdianim[k])>fabs(ljdianim[k-1]))
				{
					tempim=ljdianim[k-1];
			        ljdianim[k-1]=ljdianim[k];
				    ljdianim[k]=tempim;
				}
			}
		}
		conut--;
	}	

	for(i=0;i<N/n;i++)
	{
		//sprintf(date,"\n%f+j%f\t%f+j%f\n",ljdianre[i*2],ljdianim[i*2],ljdianre[i*2+1],ljdianim[i*2+1]);  OutputDebugString(date);
		aa[i*(n+1)]=fabs(ljdianre[i*2]*ljdianre[i*2+1])+fabs(ljdianim[i*2]*ljdianim[i*2+1]);
		aa[i*(n+1)+1]=-(ljdianre[i*2]+ljdianre[i*2+1]); //保证虚部的和为0
		aa[i*(n+1)+2]=1.0;
	}
	if(N%2!=0)
	{
		//sprintf(date,"%f+j%f\n",ljdianre[i*2],ljdianim[i*2]);  OutputDebugString(date);
		aa[i*(n+1)]=-ljdianre[i*2];
		aa[i*(n+1)+1]=1.0;
		aa[i*(n+1)+2]=0.0;
	}

	//分子,利用贝塞尔，分子只有b[0]=bessel_ak(N, 0);
	memset(bb,0,(N+1)*sizeof(double));
	bb[0]=b[0];
	for(i=1;i<(N+1)/n;i++)
	{
		bb[i*(n+1)]=1.0;
		bb[i*(n+1)+1]=0.0;
		bb[i*(n+1)+2]=0.0;
	}

	free(ljdianre);
	free(ljdianim);
	return 1;
}

/*N   ----------滤波器阶数
b     ----------分子系数二维数组，长度N/n*(n+1)
a     ----------分母系数二维数组，长度N/n*(n+1)
wo    ----------截至频率

返回增益K
*/ 
double bessel_hs(int N, double *b, double *a)  //最大25阶
{
	//char date[100];
	int i;
	double result=1.0;
	double *fenzi,*fenmu;

	if(N>25)
		return -2;

	fenzi=malloc((N+1)*sizeof(double));
	fenmu=malloc((N+1)*sizeof(double));
	memset(fenzi,0,(N+1)*sizeof(double));
	fenmu[0]=fenzi[0]=bessel_ak(N, 0);

	//sprintf(date,"%f\t",fenmu[0]);
    //OutputDebugString(date);
	for(i=1;i<=N;i++)
	{
		fenmu[i]=bessel_ak(N,i);
        //sprintf(date,"%f\t",fenmu[i]);
        //OutputDebugString(date);
	}
    
	result=zhjtojl_bessel(N, fenzi, fenmu, 2, b, a);

	free(fenzi);
	free(fenmu);
	return result;
}

double bessel_Hs_moni(int band, int N, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
	int i,temp;
	double aa, H0;
    double B,omg02;
	double hs1,hs2,hl,hu,h02,ls0,ls1;
    double *fenzi,*fenmu;
	if(band>2)
		temp=(N/2+1)/2*3;
	else
		temp=(N+1)/2*3;
	fenzi=malloc(temp*sizeof(double));
	fenmu=malloc(temp*sizeof(double));

	switch(band)
	{
	case 1:
		aa=f1;
		H0=bessel_hs(N, fenzi, fenmu);

		for(i=0;i<(N+1)/2;i++)
		{
			if((N%2!=0)&&(i==(N-1)/2))
			{
				 //分母
	    		*(a+i*3+2)=0.0;
	    		*(a+i*3+1)=*(fenmu+i*3+1);
		    	*(a+i*3)=*(fenmu+i*3)*aa;  
		    	//分子
	    		*(b+i*3+2)=0.0;
	    		*(b+i*3+1)=*(fenzi+i*3+1);
	    		*(b+i*3)=*(fenzi+i*3)*aa;
			}
			else
			{
			     //分母
				*(a+i*3+2)=*(fenmu+i*3+2);
		    	*(a+i*3+1)=*(fenmu+i*3+1)*aa;
		    	*(a+i*3)=*(fenmu+i*3)*aa*aa;
		     	//分子
		    	*(b+i*3+2)=*(fenzi+i*3+2);
		    	*(b+i*3+1)=*(fenzi+i*3+1)*aa;
		    	*(b+i*3)=*(fenzi+i*3)*aa*aa;
			}
		}

		break;
	case 2:
		aa=f2;
  
		H0=bessel_hs(N, fenzi, fenmu);
	   
		for(i=0;i<(N+1)/2;i++)
		{
			if((N%2!=0)&&(i==(N-1)/2))
			{
				 //分母
		    	*(a+i*3+2)=0.0;
	    		*(a+i*3+1)=*(fenmu+i*3);
	    		*(a+i*3)=*(fenmu+i*3+1)*aa;  
    			//分子
    			*(b+i*3+2)=0.0;
	    		*(b+i*3+1)=*(fenzi+i*3);
	    		*(b+i*3)=*(fenzi+i*3+1)*aa; 
			}
			else
			{
                //分母
	    		*(a+i*3+2)=*(fenmu+i*3);
	    		*(a+i*3+1)=*(fenmu+i*3+1)*aa;
    			*(a+i*3)=*(fenmu+i*3+2)*aa*aa;
    			//分子
    			*(b+i*3+2)=*(fenzi+i*3);
    			*(b+i*3+1)=*(fenzi+i*3+1)*aa;
    			*(b+i*3)=*(fenzi+i*3+2)*aa*aa;
			}
		}

		break;
	case 3:
		 //变为低通、归一化
		B=f3-f2;
        hs1=f1/B; hs2=f4/B; hl=f2/B; hu=f3/B; h02=hl*hu;
		ls0=(hs2*hs2-h02)/hs2;  //归一化
		ls1=(h02-hs1*hs1)/hs1;
		ls0=min(ls0,ls1);
		H0=bessel_hs(N/2, fenzi, fenmu);
		omg02=sqrt(f2*f3);
		for(i=0;i<(N/2+1)/2;i++)  
		{ 
			if((N/2%2!=0)&&(i==(N/2-1)/2))
			{
				//分母
	    		*(a+i*5+4)=0.0;
	    		*(a+i*5+3)=0.0;
	    		*(a+i*5+2)=*(fenmu+i*3+1);
	    		*(a+i*5+1)=*(fenmu+i*3)*B;
	    		*(a+i*5)=*(fenmu+i*3+1)*omg02*omg02;
				//分子
	    		*(b+i*5+4)=0.0;
	    		*(b+i*5+3)=0.0;
	    		*(b+i*5+2)=*(fenzi+i*3+1);
	    		*(b+i*5+1)=*(fenzi+i*3)*B;
	    		*(b+i*5)=*(fenzi+i*3+1)*omg02*omg02;
			}
			else
			{
                 //分母
	    		*(a+i*5+4)=fenmu[i*3+2];
	    		*(a+i*5+3)=fenmu[i*3+1]*B;
	    		*(a+i*5+2)=2*fenmu[i*3+2]*omg02*omg02+fenmu[i*3]*B*B;
	    		*(a+i*5+1)=*(fenmu+i*3+1)*B*omg02*omg02;
	    		*(a+i*5)=fenmu[i*3+2]*omg02*omg02*omg02*omg02;
	
    			//分子
    			*(b+i*5+4)=fenzi[i*3+2];
    			*(b+i*5+3)=fenzi[i*3+1]*B;
    			*(b+i*5+2)=2*fenzi[i*3+2]*omg02*omg02+fenzi[i*3]*B*B;
	    		*(b+i*5+1)=*(fenzi+i*3+1)*B*omg02*omg02;
	    		*(b+i*5)=fenzi[i*3+2]*omg02*omg02*omg02*omg02;
			}
		}
		break;
	case 4:
		 //变为低通、归一化
		B=f4-f1;  
        hs1=f2/B; hs2=f3/B; hl=f1/B; hu=f4/B; h02=hl*hu;
		ls0=hs1/(h02-hs1*hs1); 
		ls1=hs2/(hs2*hs2-h02);
		ls0=min(ls0,ls1);
		H0=bessel_hs(N/2, fenzi, fenmu);
		omg02=sqrt(f1*f4);
		for(i=0;i<(N/2+1)/2;i++)  
		{
			if((N/2%2!=0)&&(i==(N/2-1)/2))
			{
				//分母
	    		*(a+i*5+4)=0.0;
	    		*(a+i*5+3)=0.0;
	    		*(a+i*5+2)=*(fenmu+i*3);
	    		*(a+i*5+1)=*(fenmu+i*3+1)*B;
	     		*(a+i*5)=*(fenmu+i*3)*omg02*omg02;  
	    		//分子
	      		*(b+i*5+4)=0.0;
	    		*(b+i*5+3)=0.0;
	    		*(b+i*5+2)=*(fenzi+i*3);
	    		*(b+i*5+1)=*(fenzi+i*3+1)*B;
		    	*(b+i*5)=*(fenzi+i*3)*omg02*omg02;  
			}
			else
			{
                //分母
	    		*(a+i*5+4)=fenmu[i*3];	
	    		*(a+i*5+3)=fenmu[i*3+1]*B;	
	    		*(a+i*5+2)=2*fenmu[i*3]*omg02*omg02+fenmu[i*3+2]*B*B;
	    		*(a+i*5+1)=fenmu[i*3+1]*B*omg02*omg02;
	    		*(a+i*5)=fenmu[i*3]*omg02*omg02*omg02*omg02;	
	
	    		//分子
	    		*(b+i*5+4)=fenzi[i*3];
	    		*(b+i*5+3)=fenzi[i*3+1]*B;	
	    		*(b+i*5+2)=2*fenzi[i*3]*omg02*omg02+fenzi[i*3+2]*B*B;
	    		*(b+i*5+1)=fenzi[i*3+1]*B*omg02*omg02;
	    		*(b+i*5)=fenzi[i*3]*omg02*omg02*omg02*omg02;	
			}
		}
		break;
	}

	free(fenzi);
	free(fenmu);
	return H0;
}
double bessel_Hs_shxx(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as, double *b, double *a)
{
	 double pi,K;
	pi=4.0*atan(1.0);
	
	switch(band)
	{
	case 1:  //低通
	    f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs); 
		f3=0.0;
		f4=0.0;
		K=bessel_Hs_moni(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	case 2:  //高通
	    f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs);  
		f3=0.0;
		f4=0.0;
		K=bessel_Hs_moni(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	case 3:
		f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs); 
		f3=2/T*tan(pi*f3/fs); 
		f4=2/T*tan(pi*f4/fs); 
		K=bessel_Hs_moni(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	case 4:
		f1=2/T*tan(pi*f1/fs); 
		f2=2/T*tan(pi*f2/fs); 
		f3=2/T*tan(pi*f3/fs); 
		f4=2/T*tan(pi*f4/fs); 
		K=bessel_Hs_moni(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	}
	return K; 
}
double bessel_Hs_mch(int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a)
{
    double pi,K;
	pi=4.0*atan(1.0);
	
	if((band==2)||(band==4))  //脉冲响应不变法只适合设计高通和带通
	{
		printf("脉冲响应不变法不适合设计高通和带阻");
		return -2;
	}

	switch(band)
	{
	case 1:  //低通
	    f1=2*pi*f1/fs/T;    
		f2=2*pi*f2/fs/T;
		f3=0.0;
		f4=0.0;
		K=bessel_Hs_moni(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	/*case 2:  //高通
	    f1=2*pi*f1/fs/T; 
		f2=2*pi*f2/fs/T;   
		f3=0.0;
		f4=0.0;
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;*/
	case 3:
		f1=2*pi*f1/fs/T;
		f2=2*pi*f2/fs/T;
		f3=2*pi*f3/fs/T;    
		f4=2*pi*f4/fs/T; 
		K=bessel_Hs_moni(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;
	/*case 4:
		f1=2*pi*f1/fs/T;
		f2=2*pi*f2/fs/T;
		f3=2*pi*f3/fs/T;    
		f4=2*pi*f4/fs/T; 
		K=Cal_elliptic_hs_basic(band, N,  f1, f2, f3, f4, ap, as, b, a);
		break;*/
	}
	return K; 
}

double bessel_Hs(char bianhuan, int band, int N, double T, double fs, double f1, double f2, double f3, double f4, int ap, int as,  double *b, double *a)
{
	if(bianhuan==0)
		return  bessel_Hs_mch(band, N, T, fs, f1, f2, f3, f4, ap, as, b, a);
	else
		return  bessel_Hs_shxx(band, N, T, fs, f1, f2, f3, f4, ap, as, b, a);
}