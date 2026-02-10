#ifndef PLOTBISIC_H
#define PLOTBISIC_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <QRect>
#include <vector>
#include "Axis.h"
#include "plottype.h"
#include "PlotHelp.h"


class basictimeaxis
{
public:
    basictimeaxis(Axis::X_AXIS_DANWEI disp_danwei, int IntervalMinIndex = 0, int IntervalMaxIndex = Axis::AxisIntervalCount-1)
		: axis_resolution_x(0.0)
		, axis_count_x(10)
		, m_axis_danwei(disp_danwei)
		, m_Axis_clr(SIMPLE_AXIS_CLR)
		, AxisIntervalMinIndex(IntervalMinIndex)
		, AxisIntervalMaxIndex(IntervalMaxIndex)
	{
	}

	~basictimeaxis()
	{
	};

	virtual void SetAxisDanwei(Axis::X_AXIS_DANWEI dabwei) { m_axis_danwei = dabwei; };
	Axis::X_AXIS_DANWEI& GetAxisDanwei() { return m_axis_danwei; };

	int GetAxisIntervalMinIndex() { return AxisIntervalMinIndex; };
	int GetAxisIntervalMaxIndex() { return AxisIntervalMaxIndex; };

	virtual double getminrange()=0;
	virtual double getmaxrange()=0;
	virtual double getrange()=0;
	virtual void setrange(double start, double value) = 0;
	virtual void changeproperity(double value, int zoomcenterpos) = 0;

	virtual double getaxisindexvalue() = 0;

	virtual void movetime(double time) = 0;

	double movex2value(int move);

	//pos & value
	double converpos2value(int pos);
	int convervalue2pos(double value);
	double move2value(int movey);
public:
	double axis_resolution_x;
	int axis_count_x;

    QRect	m_axisRect;		// bottom x time axis

    QColor m_Axis_clr;
protected:
	int AxisIntervalMinIndex;  //最小刻度间隔index
	int AxisIntervalMaxIndex;  //最大刻度间隔index

	Axis::X_AXIS_DANWEI m_axis_danwei;
};

class simpleaxis
{
public:
    simpleaxis(const QString& trackname, QColor lineclr, Qt::PenStyle linestyle = Qt::SolidLine, int linewidth = 1,
               Axis::Y_AXIS_DANWEI disp_danwei = Axis::Y_AXIS_DANWEI::NO, int IntervalMinIndex = 0, int IntervalMaxIndex = Axis::AxisIntervalCount-1)
        : m_trackname(trackname)
        , m_lineclr(lineclr)
        , m_linestyle(linestyle)
        , m_linewidth(linewidth)
        , m_disp_danwei(disp_danwei)
        , m_start(-Axis::AxisInterval[IntervalMinIndex] * 5)
        , m_axis_index(IntervalMinIndex)
        , axis_count_y(10)
        , axis_resolution_y(0.0)
        , AxisIntervalMinIndex(IntervalMinIndex)
        , AxisIntervalMaxIndex(IntervalMaxIndex)
        , m_arrow_value(0)
    {
    };

    ~simpleaxis()
    {
    };

    virtual void SetAxisDanwei(Axis::Y_AXIS_DANWEI dabwei, int IntervalMinIndex, int IntervalMaxIndex);
    Axis::Y_AXIS_DANWEI& GetAxisDanwei() { return m_disp_danwei; };

    double getinterval() { return getaxisindexvalue(); };
    double getminrange() { return getstart(); };
    double getmaxrange() { return getstart() + getrange(); };
    double getrange() { return getaxisindexvalue() * axis_count_y; };
    void move(double move) { m_move(move); };

    virtual void setproperityvalue(double start, double value);

    //pos & value
    double converpos2value(int pos);
    int convervalue2pos(double value);
    double movey2value(int movey);
    double getyarrowvalue() { return m_arrow_value; };
    int getyarrowpos() { return convervalue2pos(m_arrow_value); };

    //zoom
    /*virtual bool VerZoomInPos(int zoompos);
    virtual bool VerZoomOutPos(int zoompos);*/
protected:
    double getstart() { return m_start; };
    void setstart(double start) { m_start = start; };
    void m_move(double move) { m_start += move ; };

    double getaxisindexvalue() { return axisindex2value(m_axis_index); };
private:
    int value2axisindex(double value);
    double axisindex2value(int index);
public:
    QString m_trackname;

    QColor m_lineclr; // 线色
    Qt::PenStyle m_linestyle;  // 线风格// 实线、虚线、点线、点划线、点点划线（SOLID, DASH, DOT, DASHDOT, DASHDOTDOT）
    int m_linewidth; // 线宽

    int axis_count_y;  //刻度 Y 轴方向的线数;
    double axis_resolution_y;// 刻度 Y 方向间距
    QRect		m_axisRect;		// axisi rect
    QRect		m_chnRect;
    QPoint      m_chnleftbuttonpoint;
protected:
    int AxisIntervalMinIndex;  //最小刻度间隔index
    int AxisIntervalMaxIndex;  //最大刻度间隔index
private:
    Axis::Y_AXIS_DANWEI m_disp_danwei;
    double	m_start;
    int m_axis_index;  //使用哪个缩放比例 数组中的值

    double m_arrow_value;
};

class plotarea
{
public:
    QRect		m_plotRect;			// clientRect - margins
    bool bDrawGrid;// 是否绘网格
    Qt::PenStyle nGridStyle;// 网格线型

    int grid_begin_x; //绘制grid的开始坐标
    double grid_resolution_x;// 网格 X、Y 方向间距
    double grid_resolution_y;
    int grid_count_x;  //网格X轴方向的线数
    int grid_count_y;  //网格Y轴方向的线数

    QColor m_bgClr, m_bgOutClr, m_gridClr;

    plotarea()
		: bDrawGrid(true)
        , nGridStyle(Qt::SolidLine)
        , grid_begin_x(0)
		, grid_resolution_x(0)
		, grid_resolution_y(0)
		, grid_count_x(10)
		, grid_count_y(10)
		, m_bgClr(SIMPLE_PLOT_BACKCLR)
        , m_bgOutClr(SIMPLE_PLOT_OUTBACKCLR)
        , m_gridClr(SIMPLE_GRID_CLR)
	{
	}
};

#endif
