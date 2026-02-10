#ifndef POINTS_H
#define POINTS_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

class point
{
public:
	double x;
	double y;

	explicit point()
		: x(0)
		, y(0)
	{
	};

	point(double xx, double yy)
		: x(xx)
		, y(yy)
	{
	};

	point(int xx, int yy)
		: x(xx)
		, y(yy)
	{
	};

	point(long xx, long yy)
		: x(xx)
		, y(yy)
	{
	};
};


#endif

