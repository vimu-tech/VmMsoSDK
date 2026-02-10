#include "plotpoints.h"
#include <stddef.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>

//---------------------------------------------plotpoints---------------------------------------------------------//
plotpoints::plotpoints(int len, bool xlog, bool ylog)
: logxsystem(xlog)
, logysystem(ylog)
, samplesPerPixel(0)
, cur_pixel_num(0)
, pixel_num(len)
, timemin(0)
, timemax(0)
, timerealmin(0)
, timerealmax(0)
, offset_x(0)
, bufmin(0)
, bufmax(0)
{
	//DebugString("plotpoints::plotpoints\n");
	pixel=new int[pixel_num];
	minvalue=new double[pixel_num];
	maxvalue=new double[pixel_num];
	//rmsvalue=new double[pixel_num];
}

plotpoints::~plotpoints(void)
{
	//DebugString("...plotpoints::~plotpoints pixel_num=%d  samplesPerPixel=%f pixel=%x minvalue=%x maxvalue=%x\n",
	//  pixel_num,samplesPerPixel,pixel,minvalue,maxvalue);
	if(pixel!= nullptr)
		delete []pixel;
	if(minvalue!= nullptr)
		delete []minvalue;
	if(maxvalue!= nullptr)
		delete []maxvalue;
	//if(rmsvalue!=nullptr)
		//delete []rmsvalue;
	//DebugString("...plotpoints::~plotpoints  ok\n");
}

void plotpoints::changeNum(int len)
{
	if (pixel != nullptr)
		delete[]pixel;
	if (minvalue != nullptr)
		delete[]minvalue;
	if (maxvalue != nullptr)
		delete[]maxvalue;

	cur_pixel_num = 0;
	pixel_num = len;

	pixel = new int[pixel_num];
	minvalue = new double[pixel_num];
	maxvalue = new double[pixel_num];
}

bool plotpoints::getpoint(int *x, double *y)
{
	bool res=false;
	if(*x<=pixel[0])
	{
		*x=pixel[0];
		*y=*y-minvalue[0]>=maxvalue[0]-*y? maxvalue[0]:minvalue[0];
		res=true;
	}
	else if(*x>=pixel[cur_pixel_num-1])
	{
		*x=pixel[cur_pixel_num-1];
		*y=*y-minvalue[cur_pixel_num-1]>=maxvalue[cur_pixel_num-1]-*y? maxvalue[cur_pixel_num-1]:minvalue[cur_pixel_num-1];
		res=true;
	}
	else
	{
		for(int i=0;(i<pixel_num-1)&&(pixel[i+1]!=-1);i++)
		{
			if((pixel[i]>=*x)&&(*x<=pixel[i+1]))
			{
				*x=*x-pixel[i]>=pixel[i+1]-*x? pixel[i+1]:pixel[i];
				*y=*y-minvalue[i]>=maxvalue[i]-*y? maxvalue[i]:minvalue[i];
				res=true;
				break;
			}
		}
	}
	return res;
}

bool plotpoints::GetDisplayPoint(int x, double *y)
{
	bool res=false;
	if((x>=pixel[0])&&(x<=pixel[cur_pixel_num-1]))
	{
		for(int i=0;(i<pixel_num-1)&&(pixel[i+1]!=-1);i++)
		{
			if(pixel[i]==x)
			{
				*y = (maxvalue[i]-minvalue[i])/2+minvalue[i];
				res=true;
				break;
			}
			else if(pixel[i+1]==x)
			{
				*y = (maxvalue[i+1]-minvalue[i+1])/2+minvalue[i+1];
				res=true;
				break;
			}
			else if((pixel[i]<x)&&(x<pixel[i+1]))
			{
				double yi=(maxvalue[i]-minvalue[i])/2+minvalue[i];
				double yi1=(maxvalue[i+1]-minvalue[i+1])/2+minvalue[i+1];
				*y = (x-pixel[i])*(yi1-yi)/(pixel[i+1]-pixel[i])+yi;
				res=true;
				break;
			}
		}
	}
	return res;
}

//返回值  0 错误   -1 谷底  1谷峰
char plotpoints::GetXMeasurePoints(point* leftpoint, point* rightpoint, int* rightindex)
{
	char res=0;

	if(rightindex!= nullptr)
		*rightindex=0;

	if((leftpoint->x<=pixel[0])||(leftpoint->x>=pixel[cur_pixel_num-1]))
	{
		res=0;
	}
	else
	{
		char leftres=-2,rightres=2;
		//找到x位置
		int weizhi=0;
		for(weizhi=0;(weizhi<pixel_num-1)&&(pixel[weizhi+1]!=-1);weizhi++)
		{
			if((pixel[weizhi]<=leftpoint->x)&&(pixel[weizhi+1]>=leftpoint->x))
				break;
		}

		//left
		for(int i=weizhi;i>0;i--)
		{
			if((leftpoint->y<=maxvalue[i-1])&&(leftpoint->y>=minvalue[i])) //谷底
			{
				if(fabs(maxvalue[i-1]-leftpoint->y)>=fabs(leftpoint->y-minvalue[i]))
				{
					leftpoint->x=pixel[i];
					leftpoint->y=minvalue[i];
				}
				else
				{
					leftpoint->x=pixel[i-1];
					leftpoint->y=maxvalue[i-1];
				}
				leftres=-1;
				break;
			}
			else if((leftpoint->y<=maxvalue[i])&&(leftpoint->y>=minvalue[i-1])) //谷峰
			{
				if(fabs(leftpoint->y-maxvalue[i])>=fabs(minvalue[i-1]-leftpoint->y))
				{
					leftpoint->x=pixel[i-1];
					leftpoint->y=minvalue[i-1];
				}
				else
				{
					leftpoint->x=pixel[i];
					leftpoint->y=maxvalue[i];
				}
				leftres=1;
				break;
			}
		}
		//right
		for(int i=weizhi;(i<pixel_num-1)&&(pixel[i+1]!=-1);i++)
		{ 
			if((rightpoint->y>=minvalue[i])&&(rightpoint->y<=maxvalue[i+1])) //谷底
			{
				if(fabs(rightpoint->y-minvalue[i])>=fabs(maxvalue[i+1]-rightpoint->y))
				{
					rightpoint->x=pixel[i+1];
					rightpoint->y=maxvalue[i+1];
					if(rightindex!= nullptr)
						*rightindex=i+1;
				}
				else
				{
					rightpoint->x=pixel[i];
					rightpoint->y=minvalue[i];
					if(rightindex!= nullptr)
						*rightindex=i;
				}
				rightres=-1;
				break;
			}
			else if((rightpoint->y<=maxvalue[i])&&(rightpoint->y>=minvalue[i+1])) //谷峰
			{
				if(fabs(maxvalue[i]-rightpoint->y)>=fabs(rightpoint->y-minvalue[i+1]))
				{
					rightpoint->x=pixel[i+1];
					rightpoint->y=minvalue[i+1];
					if(rightindex!= nullptr)
						*rightindex=i+1;
				}
				else
				{
					rightpoint->x=pixel[i];
					rightpoint->y=maxvalue[i];
					if(rightindex!= nullptr)
						*rightindex=i;
				}
				rightres=1;
				break;
			}
		}	
		if(leftres==rightres)
			res=leftres;
		else
			res=0;
	}
	return res;
}

//返回值    0 错误    1 找到宽度部分    2 找整个周期
char plotpoints::GetDutyMeasurePoints(point* point1, point* point2, point* point3)
{
	char result=0;
	int point2index=0;

	if(GetXMeasurePoints(point1, point2, &point2index)!=0)
	{
		result=1;

		for(int i=point2index+1;(i<pixel_num-1)&&(pixel[i+1]!=-1);i++)
		{ 
			if((point3->y>=minvalue[i])&&(point3->y<=maxvalue[i+1])) //谷底
			{
				if(fabs(point3->y-minvalue[i])>=fabs(maxvalue[i+1]-point3->y))
				{
					point3->x=pixel[i+1];
					point3->y=maxvalue[i+1];
				}
				else
				{
					point3->x=pixel[i];
					point3->y=minvalue[i];
				}
				result=2;
				break;
			}
			else if((point3->y<=maxvalue[i])&&(point3->y>=minvalue[i+1])) //谷峰
			{
				if(fabs(maxvalue[i]-point3->y)>=fabs(point3->y-minvalue[i+1]))
				{
					point3->x=pixel[i+1];
					point3->y=minvalue[i+1];
				}
				else
				{
					point3->x=pixel[i];
					point3->y=maxvalue[i];
				}
				result=2;
				break;
			}
		}	
	}

	return result;
}

//返回值  0 错误   -1 下降  1上升
char plotpoints::GetYMeasurePoints(point* leftpoint, point* rightpoint)
{
	char res=0;
	char leftres=-2,rightres=2;
	//找到x位置
	int weizhi=0;
	for(weizhi=0;(weizhi<pixel_num-1)&&(pixel[weizhi+1]!=-1);weizhi++)
	{
		if((pixel[weizhi]<=leftpoint->x)&&(pixel[weizhi+1]>=leftpoint->x))
			break;
	}

	//left
	for(int i=weizhi-2;i>1;i--)
	{
		if((maxvalue[i-2]<maxvalue[i])&&(maxvalue[i-1]<=maxvalue[i])&&(maxvalue[i]>=maxvalue[i+1])&&(maxvalue[i]>=maxvalue[i+2])) //下降
		{
			leftpoint->x=pixel[i];
			leftpoint->y=maxvalue[i];
			leftres=-1;
			break;
		}
		else if((maxvalue[i-2]>maxvalue[i])&&(maxvalue[i-1]>=maxvalue[i])&&(maxvalue[i]<=maxvalue[i+1])&&(maxvalue[i]<=maxvalue[i+2])) //上升
		{
			leftpoint->x=pixel[i];
			leftpoint->y=minvalue[i];
			leftres=1;
			break;
		}
	}
	//right
	for(int i=weizhi+2;(i<pixel_num-2)&&(pixel[i+1]!=-1);i++)
	{ 
		if((maxvalue[i-2]>=maxvalue[i])&&(maxvalue[i-1]>=maxvalue[i])&&(maxvalue[i]<=maxvalue[i+1])&&(maxvalue[i]<maxvalue[i+2])) //下降
		{
			rightpoint->x=pixel[i];
			rightpoint->y=minvalue[i];
			rightres=-1;
			break;
		}
		else if((maxvalue[i-2]<=maxvalue[i])&&(maxvalue[i-1]<=maxvalue[i])&&(maxvalue[i]>=maxvalue[i+1])&&(maxvalue[i]>maxvalue[i+2])) //上升
		{
			rightpoint->x=pixel[i];
			rightpoint->y=maxvalue[i];
			rightres=1;
			break;
		}
	}	
	if(leftres==rightres)
		res=leftres;
	else
		res=0;
	return res;
}

char plotpoints::GetMinMaxPoints(int pixel_start, double x_start, int pixel_stop, double x_stop, point* min_point, point* max_point)
{
	char res = 0;

	if ((pixel_start >= pixel[0]) && (pixel_stop <= pixel[cur_pixel_num - 1]))
	{
		for (int i = 0; i < cur_pixel_num - 1; i++)
		{
			if ((pixel[i] >= pixel_start) && (pixel[i] <= pixel_stop))
			{
				if (res == 0)
				{
					res = 1;
					max_point->x = min_point->x = pixel[i];
					min_point->y = minvalue[i];
					max_point->y = maxvalue[i];
				}
				else
				{
					if (minvalue[i] < min_point->y)
					{
						min_point->x = pixel[i];
						min_point->y = minvalue[i];
					}
					if (maxvalue[i] > max_point->y)
					{
						max_point->x = pixel[i];
						max_point->y = maxvalue[i];
					}
				}
			}
		}
	}

	if (res == 1)
	{
		if (cur_pixel_num >= 2)
		{
			//将点转换为时间
			min_point->x = (min_point->x - pixel[0]) * (timerealmax - timerealmin) / (pixel[cur_pixel_num - 1] - pixel[0]) + timerealmin;
			max_point->x = (max_point->x - pixel[0]) * (timerealmax - timerealmin) / (pixel[cur_pixel_num - 1] - pixel[0]) + timerealmin;
		}
		else
		{
			res = 0;
		}
	}

	return res;
}

plotpoints& plotpoints::operator=(const plotpoints& copy)
{
	logxsystem=copy.logxsystem;
	logysystem=copy.logysystem;

	timemin=copy.timemin;
	timemax=copy.timemax;
	timerealmin=copy.timerealmin;
	timerealmax=copy.timerealmax;

	samplesPerPixel=copy.samplesPerPixel;
	pixel_num=copy.pixel_num;
	cur_pixel_num=copy.cur_pixel_num;

	offset_x=copy.offset_x;

	if(pixel!= nullptr)
		memcpy(pixel,copy.pixel,pixel_num*sizeof(int));

	if(minvalue!= nullptr)
		memcpy(minvalue,copy.minvalue,pixel_num*sizeof(double));

	if(maxvalue!= nullptr)
		memcpy(maxvalue,copy.maxvalue,pixel_num*sizeof(double));

	//计算最值
	if ((pixel != nullptr) && (minvalue != nullptr) && (maxvalue != nullptr))
	{
		bufmin = minvalue[0];
		bufmax = maxvalue[0];
		for (int i = 1; (i < pixel_num) && (pixel[i] != -1); i++)
		{
			bufmin = bufmin < minvalue[i] ? bufmin : minvalue[i];
			bufmax = bufmax > maxvalue[i] ? bufmax : maxvalue[i];
		}
	}
	return *this;
}

bool plotpoints::IsRange(int x, double y)
{
	bool res=false;
	if(((x>=pixel[0])&&(x<=pixel[cur_pixel_num-1]))&&((bufmin<=y)&&(y<=bufmax)))
		res=true;
	return res;
}

bool plotpoints::IsRange(double y)
{
	bool res=false;
	if((bufmin<=y)&&(y<=bufmax))
		res=true;
	return res;
}

bool plotpoints::operator!=(const plotpoints& copy)
{
    if((logxsystem!=copy.logxsystem)||(logysystem!=copy.logysystem)||(timemin!=copy.timemin)||(timemax!=copy.timemax)||(pixel_num!=copy.pixel_num)
		||(timerealmin!=copy.timerealmin)||(timerealmax!=copy.timerealmax))
		return true;
	return false;
}

bool plotpoints::SetTimeOffset(double offset)
{
	//timemin+=offset;
	//timemax+=offset;
	//timerealmin+=offset;
	//timerealmax+=offset;
	offset_x=offset*pixel_num/(timemax-timemin);

	return true;
}

