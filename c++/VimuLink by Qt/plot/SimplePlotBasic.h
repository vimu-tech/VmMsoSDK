#ifndef SIMPLEPLOTBASIC_H
#define SIMPLEPLOTBASIC_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include "PlotBasic.h"

class simplebasictimeaxis : public basictimeaxis
{
public:
	simplebasictimeaxis(Axis::X_AXIS_DANWEI disp_danwei, int IntervalMinIndex = 0, int IntervalMaxIndex = 63)
		: basictimeaxis(disp_danwei, IntervalMinIndex, IntervalMaxIndex)
	{
	}

	virtual void adjust();
	virtual void reset();

    //virtual bool ZoomInPos(QPoint point) = 0;
    //virtual bool ZoomOutPos(QPoint point) = 0;

	double GetHorizontalOffset() { return getminrange() + getrange()/2; };
	void SetHorizontalOffset(double offset) { movetime(offset - GetHorizontalOffset()); };
};

class simpledsotimeaxis : public simplebasictimeaxis
{
public:
	simpledsotimeaxis(Axis::X_AXIS_DANWEI disp_danwei, int IntervalMinIndex = 0, int IntervalMaxIndex = 63)
		: simplebasictimeaxis(disp_danwei, IntervalMinIndex, IntervalMaxIndex)
		, m_start(-Axis::AxisInterval[IntervalMinIndex] * 5)
		, m_axis_index(IntervalMinIndex)
		, m_trigger(0)
        , m_arrowleftbuttonpoint( QPoint(-1,-1))
	{
		gen_aixs_scales();
	}

	~simpledsotimeaxis()
	{
		destroy_aixs_scales();
	};

	virtual void SetAxisDanwei(Axis::X_AXIS_DANWEI dabwei);

	double getminrange(){return m_start;};
	double getmaxrange(){return m_start + getrange();};
	double getrange(){return getaxisindexvalue(m_axis_index)*axis_count_x;};
	void setrange(double start, double value)
	{
		m_start = start;
		setaxisindex(value);
	}

    virtual double getaxisindexvalue() { return getaxisindexvalue(m_axis_index); };

	virtual void changeproperity(double value, int zoomcenterpos);

	void movetime(double time){ m_start +=time;};

    //virtual bool ZoomInPos(QPoint point);
    //virtual bool ZoomOutPos(QPoint point);

	//Scale String
    QStringList GetAxisScaleStrings() { return m_aixs_scale_strs; };
    double GetAxisScaleStringValue(int index) { return getaxisindexvalue(index + AxisIntervalMinIndex); };
	int GetAxisScaleStringIndex() { return getaxisindex() - AxisIntervalMinIndex; };
	int GetAxisScaleStringCount() { return m_aixs_scale_strs.size(); }
	void SetAxisScaleStringIndex(int index, int zoomcenterpos) { changeproperity(getaxisindexvalue(index + AxisIntervalMinIndex), zoomcenterpos); };
private:
	int getaxisindex() { return m_axis_index; };
	int setaxisindex(double value) { return m_axis_index = value2axisindex(value); };
	double getaxisindexvalue(int index) { return Axis::AxisInterval[index]; };

	int value2axisindex(double value);

	void gen_aixs_scales();
	void destroy_aixs_scales();
public:
	//-------------------
	double	m_trigger;
    QRect		m_arrowRect;
    QPoint      m_arrowleftbuttonpoint;

private:
	double m_start;
	int m_axis_index;  //使用哪个缩放比例 数组中的值
    QStringList m_aixs_scale_strs;
};


class simplerecordtimeaxis : public simplebasictimeaxis
{
public:
    simplerecordtimeaxis(Axis::X_AXIS_DANWEI disp_danwei, int IntervalMinIndex, int IntervalMaxIndex)
        : simplebasictimeaxis(disp_danwei, IntervalMinIndex, IntervalMaxIndex)
        , m_start_time(-Axis::AxisInterval[IntervalMinIndex] * 5)
        , m_total_time(Axis::AxisInterval[IntervalMinIndex] * 5)
        , m_centerRect(0,0,0,0)
        , m_centerleftbuttonpoint(-1,-1)
        , m_center(1000)
        , Axis_Is_No_PointNum(2)
    {
    }

    double getminrange() { return m_start_time; };
    double getmaxrange() { return m_start_time + m_total_time; };
    double getrange() { return m_total_time; };

    void setstarttime(double time) { m_start_time = time; };
    void setrange(double range) { m_total_time = range; };
    void setrange(double start, double value) { assert(false); }
    virtual void changeproperity(double value, int zoomcenterpos) { assert(false); }

    double getaxisindexvalue() { return m_total_time / axis_count_x; };

    void adjustmaxtime(double max_time)
    {
        m_start_time = max_time - m_total_time;
    };

    void settimeaxisrange(double min, double max, bool force, bool large)
    {
        if (force)
        {
            m_start_time = min;
            m_total_time = max - min;
        }
        else
        {
            const double interval = (max - min) / axis_count_x;
            int i = AxisIntervalMinIndex;
            for (; i <= AxisIntervalMaxIndex; i++)
            {
                if ((Axis::AxisInterval[i] <= interval) && (Axis::AxisInterval[i + 1] > interval))
                    break;
            }
            if (large && (interval != Axis::AxisInterval[i]))
                i++;
            m_start_time = (int)(min / Axis::AxisInterval[i] - 0.5) * Axis::AxisInterval[i];
            m_total_time = Axis::AxisInterval[i] * axis_count_x;
        }
    }

    void gettimeaxisrange(double* min, double* max)
    {
        *min = m_start_time;
        *max = m_start_time + m_total_time;
    }

    void movetime(double time){ m_start_time +=time;};

    //virtual bool movetime(double time, bool is_have_wave, double wave_time_min, double wave_time_max) override;
    //virtual bool ZoomInPos(QPoint point, double resolution, bool is_have_wave, double wave_time_min, double wave_time_max) override;
    //virtual bool ZoomInPos(double time_max, double resolution, bool is_have_wave, double wave_time_min, double wave_time_max);
    //virtual bool ZoomOutPos(QPoint point, bool is_have_wave, double wave_time_min, double wave_time_max) override;
    //virtual bool ZoomOutPos(double time_max, bool is_have_wave, double wave_time_min, double wave_time_max);

    void SetTimeTotalRange(double start, double range);
    void GetTimeTotalRange(double* start, double* range);
    //double GetTimeTotalRange();

    virtual void SetAxisDanwei(Axis::X_AXIS_DANWEI dabwei);

    void SetTimeCenter(double center);
    double GetTimeCenter();
    double GetTimePlotPosCenter();

    virtual double GetTimeAdjustIntreval();

    //-------------------
    QRect		m_centerRect;
    QPoint      m_centerleftbuttonpoint;

    int Axis_Is_No_PointNum;  //当X_AXIS_NO的时候 显示数据小数点位数
private:
    double	m_center;

    double m_start_time;
    double m_total_time;
};

#endif


