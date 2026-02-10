#ifndef VMSIMPLEPLOT_H
#define VMSIMPLEPLOT_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <QQuickPaintedItem>
#include "SimplePlotBasic.h"
#include "PlotBasic.h"
#include "plotrecord.h"
#include "fftcal.h"

class VmSimplePlot : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QStringList axisIntervalList READ getAxisIntervalList NOTIFY axisIntervalListChanged)
    Q_PROPERTY(QStringList timeAxisIntervalList READ getTimeAxisIntervalList NOTIFY timeAxisIntervalListChanged)

public:
    VmSimplePlot(QQuickItem *parent = 0);
    virtual ~VmSimplePlot();

    Q_INVOKABLE QStringList getAxisIntervalList(){
        QStringList list;
        Axis::Y_AXIS_DANWEI disp_danwei =  Axis::Y_AXIS_DANWEI::VOLTAGE;
        for(int i=GetYMinAxisInterval(disp_danwei); i<GetYMaxAxisInterval(disp_danwei); i++)
        {
            if(Axis::AxisInterval[i]<1.0)
                list.push_back(QString("%1mV").arg(Axis::AxisInterval[i]*1000.0, 7, 'f', 0));
            else
                list.push_back(QString("%1V").arg(Axis::AxisInterval[i], 7, 'f', 0));
        }
        return list;
    };
    Q_INVOKABLE int getAxisIntervalListLength(){
        return getAxisIntervalList().size();
    }

    //-----------------------------------------------waves axis------------------------------------------------------
    Q_INVOKABLE bool addAxis(const QString& m_trackname, QColor m_lineclr);
    Q_INVOKABLE void removeAxis(const QString& m_trackname);
    Q_INVOKABLE void destoryAllAxis();

    Q_INVOKABLE double getAxisInterval(int k){return Axis::AxisInterval[GetYMinAxisInterval(Axis::Y_AXIS_DANWEI::VOLTAGE)+k];};
    Q_INVOKABLE void setYAxisRange(const QString& m_trackname, double start, double value);
    Q_INVOKABLE double getYAxisInterval(const QString& m_trackname);

    Q_INVOKABLE QStringList getTimeAxisIntervalList(){
        return m_timeaxis.GetAxisScaleStrings();
    };
    Q_INVOKABLE double getTimeAxisInterval(int k){return m_timeaxis.GetAxisScaleStringValue(k);};
    Q_INVOKABLE void setTimeAxisRange(double start, double value);

    //-----------------------------------------------math axis------------------------------------------------------
    Q_INVOKABLE bool addMathAxis(const QString& m_trackname, QColor m_lineclr);
    Q_INVOKABLE void removeMathAxis(const QString& m_trackname);
    Q_INVOKABLE void destoryAllMathAxis();

    Q_INVOKABLE void setMathYAxisRange(const QString& m_trackname, double start, double value);

    Q_INVOKABLE void setMathTimeAxisRange(double start, double range);


    //更新增减线后，的位置问题  <XP没有初始化界面有问题>
    Q_INVOKABLE void computeAxisPos(){ComputeRects();};

    QColor backgroud_color() const;
    void setBackgroudColor(const QColor &color);

    Q_INVOKABLE void redraw(){update();};
    void paint(QPainter *painter);

    void setPlotRecord(PlotRecord* record){pPlotRecord = record;}
    void setFftCal(FFTCal* cal){ pFFTCal = cal; }
signals:
    void yAxisRangeChanged(QString trackname, QVariantList list);
    void axisIntervalListChanged();
    void timeAxisIntervalListChanged();
protected:
    virtual int GetYMinAxisInterval(Axis::Y_AXIS_DANWEI danwei);
    virtual int GetYMaxAxisInterval(Axis::Y_AXIS_DANWEI danwei);
    virtual int GetTimeMinAxisInterval();
    virtual int GetTimeMaxAxisInterval();

    void GetLeftArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points=NULL);
    void GetRightArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points=NULL);
    void GetTopArrow5Points(QPoint* points, int x, int y, int heigth, int width, int heigth_arrow, int trigger_arrow_range, QPoint* trigger_points=NULL);
    void GetBottomArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points=NULL);

    bool DrawPointsArrow(QPainter* painter, QColor clr, QPoint* points, int point_count);

    virtual bool UpdateLine(QPainter* painter, QPixmap& MemPixmap, QRect& m_rect, double time_min, double time_max, bool full=false);
    virtual bool UpdateLine(QPainter* painter, QPixmap& MemPixmap);

    virtual bool UpdateMathLine(QPainter* painter, QPixmap& MemPixmap, QRect& m_rect, double time_min, double time_max, bool full=false);
    virtual bool UpdateMathLine(QPainter* painter, QPixmap& MemPixmap);

    virtual void ComputeRects();

    virtual bool DrawBackgroud(QPainter *painter);
    virtual bool DrawGrid(QPainter *painter);
    virtual bool DrawYArrows(QPainter *painter, simpleaxis* axis);
    virtual bool DrawTimeArrows(QPainter *painter){return true;};
    virtual bool DrawTimeArrowsNotice(QPainter *painter);
    virtual bool DrawValues(QPainter *painter);
    virtual bool OtherDraw(QPainter *mempainter, QPainter *painter);

    void SetRange(double *dstmin, double *dstmax, int *dstproperity, int axis_count, double min, double max);

    QSize RegularSize;

    QFont RegularFont;
    QFont SmallFont;

    bool m_support_top_info;
    int bootom_axis_disp_num;//低端刻度行数
    bool m_cursor_value_pos_istop;

    //显示信息
    QColor m_infotext_clr;

    PlotHelp m_plothelp;

    QRect m_clientRect;

    plotarea m_plotarea;

    //waves
    simpledsotimeaxis m_timeaxis;
    std::vector<simpleaxis*> axisvector;
    PlotRecord* pPlotRecord = nullptr;

    //math
    simplerecordtimeaxis mathTimeAxis;
    std::vector<simpleaxis*> mathAxisVector;
    FFTCal* pFFTCal = nullptr;
};

#endif // VMSIMPLEPLOT_H
