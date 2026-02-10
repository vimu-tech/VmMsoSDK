#ifndef PLOTPOINTS_H
#define PLOTPOINTS_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/


#include "points.h"
#include <stddef.h>

class plotpoints
{
public:
	explicit plotpoints(int len, bool xlog=false, bool ylog=false);
	virtual ~plotpoints(void);

	void changeNum(int len);
	bool IsRange(int x, double y);
	bool IsRange(double y);
	bool getpoint(int *x, double *y);
	bool GetDisplayPoint(int x, double *y);
	char GetXMeasurePoints(point* leftpoint, point* rightpoint, int* rightindex= nullptr);
	char GetDutyMeasurePoints(point* point1, point* point2, point* point3);
	char GetYMeasurePoints(point* leftpoint, point* rightpoint);
	char GetMinMaxPoints(int pixel_start, double x_start, int pixel_stop, double x_stop, point* min_point, point* max_point);
	int getpixelnum(){return pixel_num;};
	bool SetTimeOffset(double offset);

	//X横轴对数坐标
	bool logxsystem;
	//Y横轴对数坐标
	bool logysystem;

	//time轴范围
	double timemin;
	double timemax;
	//实际填充的time轴范围
	double timerealmin;
	double timerealmax;

	double samplesPerPixel;

	int cur_pixel_num;
	int pixel_num;
	int *pixel;
	double *minvalue;
	double *maxvalue;

	//X轴偏移
	int offset_x;

	double bufmin,bufmax;

	plotpoints& operator=(const plotpoints& copy);
	bool operator!=(const plotpoints& copy);
private:
	//禁止使用
	plotpoints(const plotpoints& t) = delete;
};

#endif
