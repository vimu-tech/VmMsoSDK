#include "VmSimplePlot.h"
#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include "VmMultMsoLib.h"

VmSimplePlot::VmSimplePlot(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_timeaxis(Axis::X_AXIS_DANWEI::S, GetTimeMinAxisInterval(), GetTimeMaxAxisInterval())
    , mathTimeAxis(Axis::X_AXIS_DANWEI::FREQ, 0, Axis::AxisIntervalCount-1)
    , RegularSize(16,16)
    , RegularFont(qApp->font())
    , SmallFont(qApp->font())
    , m_support_top_info(true)
    , bootom_axis_disp_num(0)
    , m_cursor_value_pos_istop(true)
    , m_infotext_clr(SIMPLE_FACE_FONT)
    , m_clientRect(0,0,0,0)
{

}

VmSimplePlot::~VmSimplePlot()
{
    destoryAllAxis();
    destoryAllMathAxis();
}

//-----------------------------------------------axis------------------------------------------------------
bool VmSimplePlot::addAxis(const QString& m_trackname, QColor m_lineclr)
{
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
            return false;
    }
    Axis::Y_AXIS_DANWEI disp_danwei =  Axis::Y_AXIS_DANWEI::VOLTAGE;
    simpleaxis *axis=new simpleaxis(m_trackname, m_lineclr, Qt::SolidLine, 1, disp_danwei, GetYMinAxisInterval(disp_danwei), GetYMaxAxisInterval(disp_danwei));
    axisvector.push_back(axis);
    return true;
}

void VmSimplePlot::removeAxis(const QString& m_trackname)
{
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
        {
            delete (*Vector_Iter);
            axisvector.erase(Vector_Iter);
            break;
        }
    }
}

void VmSimplePlot::destoryAllAxis()
{
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        delete (*Vector_Iter);
        (*Vector_Iter) = NULL;
    }
    // 如果 list 声明对象为类，则 clear 不能释放该内存，要辅以 erase 或 pop 或 delete
    axisvector.erase(axisvector.begin(), axisvector.end());
    axisvector.clear();
}


void VmSimplePlot::setYAxisRange(const QString& m_trackname, double start, double value)
{
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
        {
            (*Vector_Iter)->setproperityvalue(start, value);
            //qInfo()<<m_trackname<<" "<<(*Vector_Iter)->getminrange()<<" "<<(*Vector_Iter)->getmaxrange();

            QVariantList list;
            list.push_back((*Vector_Iter)->getminrange());
            list.push_back((*Vector_Iter)->getmaxrange());
            emit yAxisRangeChanged(m_trackname, list);
            break;
        }
    }
}

double VmSimplePlot::getYAxisInterval(const QString& m_trackname)
{
    double interval = 0;
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
        {
            interval = (*Vector_Iter)->getinterval();
        }
    }
    return interval;
}

void VmSimplePlot::setTimeAxisRange(double start, double value)
{
    m_timeaxis.setrange(start, value);
    //qDebug() << "setTimeAxisRange start " << m_timeaxis.getminrange() << " range " << m_timeaxis.getrange();
}

//--------------------------------------------------------------------------------
#define Y_AXIS_PROPERITY_MIN 24
#define Y_AXIS_PROPERITY_MAX 33

#define TIME_AXIS_PROPERITY_MIN 33 //10ns
#define TIME_AXIS_PROPERITY_MAX  65 //54-100ms 65-500s

int VmSimplePlot::GetYMinAxisInterval(Axis::Y_AXIS_DANWEI danwei)
{
    return Y_AXIS_PROPERITY_MIN;
}

int VmSimplePlot::GetYMaxAxisInterval(Axis::Y_AXIS_DANWEI danwei)
{
    return Y_AXIS_PROPERITY_MAX;
}

int VmSimplePlot::GetTimeMinAxisInterval()
{
    return TIME_AXIS_PROPERITY_MIN;
}

int VmSimplePlot::GetTimeMaxAxisInterval()
{
    return TIME_AXIS_PROPERITY_MAX;
}

//--------------------------------------------------------------------------------
bool VmSimplePlot::addMathAxis(const QString& m_trackname, QColor m_lineclr)
{
    for (auto Vector_Iter = mathAxisVector.begin(); Vector_Iter != mathAxisVector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
            return false;
    }
    Axis::Y_AXIS_DANWEI disp_danwei =  Axis::Y_AXIS_DANWEI::VOLTAGE;
    simpleaxis *axis=new simpleaxis(m_trackname, m_lineclr, Qt::SolidLine, 1, disp_danwei, GetYMinAxisInterval(disp_danwei), GetYMaxAxisInterval(disp_danwei));
    mathAxisVector.push_back(axis);
    return true;
}

void VmSimplePlot::removeMathAxis(const QString& m_trackname)
{
    for (auto Vector_Iter = mathAxisVector.begin(); Vector_Iter != mathAxisVector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
        {
            delete (*Vector_Iter);
            mathAxisVector.erase(Vector_Iter);
            break;
        }
    }
}

void VmSimplePlot::destoryAllMathAxis()
{
    for (auto Vector_Iter = mathAxisVector.begin(); Vector_Iter != mathAxisVector.end(); ++Vector_Iter)
    {
        delete (*Vector_Iter);
        (*Vector_Iter) = NULL;
    }
    // 如果 list 声明对象为类，则 clear 不能释放该内存，要辅以 erase 或 pop 或 delete
    mathAxisVector.erase(mathAxisVector.begin(), mathAxisVector.end());
    mathAxisVector.clear();
}

void VmSimplePlot::setMathYAxisRange(const QString& m_trackname, double start, double value)
{
    for (auto Vector_Iter = mathAxisVector.begin(); Vector_Iter != mathAxisVector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
        {
            (*Vector_Iter)->setproperityvalue(start, value);
            //qInfo()<<m_trackname<<" "<<(*Vector_Iter)->getminrange()<<" "<<(*Vector_Iter)->getmaxrange();

            QVariantList list;
            list.push_back((*Vector_Iter)->getminrange());
            list.push_back((*Vector_Iter)->getmaxrange());
            emit yAxisRangeChanged(m_trackname, list);
            break;
        }
    }
}

void VmSimplePlot::setMathTimeAxisRange(double start, double range)
{
    mathTimeAxis.setstarttime(start);
    mathTimeAxis.setrange(range);
}

void VmSimplePlot::GetLeftArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points/*=NULL*/)
{
    points[0] = QPoint(x, y);
    points[1] = QPoint(x - width_arrow, y + heigth / 2);
    points[2] = QPoint(x - width, y + heigth / 2);
    points[3] = QPoint(x - width, y - heigth / 2);
    points[4] = QPoint(x - width_arrow, y - heigth / 2);

    if (trigger_points != NULL)
    {
        trigger_points[0].rx() = x - trigger_arrow_range;
        trigger_points[0].ry() = y;
        trigger_points[1].rx() = points[2].x() + trigger_arrow_range;
        trigger_points[1].ry() = y;
        trigger_points[2].rx() = points[2].x() + trigger_arrow_range;
        trigger_points[2].ry() = points[2].y() - trigger_arrow_range;
        trigger_points[3].rx() = points[2].x() + trigger_arrow_range;
        trigger_points[3].ry() = points[3].y() + trigger_arrow_range;
    }
}

void VmSimplePlot::GetRightArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points/*=NULL*/)
{
    points[0] = QPoint(x, y);
    points[1] = QPoint(x + width_arrow, y + heigth / 2);
    points[2] = QPoint(x + width, y + heigth / 2);
    points[3] = QPoint(x + width, y - heigth / 2);
    points[4] = QPoint(x + width_arrow, y - heigth / 2);

    if (trigger_points != NULL)
    {
        trigger_points[0].rx() = x + trigger_arrow_range;
        trigger_points[0].ry() = y;
        trigger_points[1].rx() = points[2].x() - trigger_arrow_range;
        trigger_points[1].ry() = y;
        trigger_points[2].rx() = points[2].x() - trigger_arrow_range;
        trigger_points[2].ry() = points[4].y() + trigger_arrow_range;
        trigger_points[3].rx() = points[2].x() - trigger_arrow_range;
        trigger_points[3].ry() = points[1].y() - trigger_arrow_range;
    }
}

void VmSimplePlot::GetTopArrow5Points(QPoint* points, int x, int y, int heigth, int width, int heigth_arrow, int trigger_arrow_range, QPoint* trigger_points/*=NULL*/)
{
    points[0] = QPoint(x, y);
    points[1] = QPoint(x - width / 2, y - heigth_arrow);
    points[2] = QPoint(x - width / 2, y - heigth);
    points[3] = QPoint(x + width / 2, y - heigth);
    points[4] = QPoint(x + width / 2, y - heigth_arrow);

    if (trigger_points != NULL)
    {
        trigger_points[0].rx() = x;
        trigger_points[0].ry() = y - trigger_arrow_range;
        trigger_points[1].rx() = x;
        trigger_points[1].ry() = points[2].y() + trigger_arrow_range;
        trigger_points[2].rx() = points[2].x() + trigger_arrow_range;
        trigger_points[2].ry() = points[2].y() + trigger_arrow_range;
        trigger_points[3].rx() = points[3].x() - trigger_arrow_range;
        trigger_points[3].ry() = points[3].y() + trigger_arrow_range;
    }
}

void VmSimplePlot::GetBottomArrow5Points(QPoint* points, int x, int y, int heigth, int width, int heigth_arrow, int trigger_arrow_range, QPoint* trigger_points/*=NULL*/)
{
    points[0] = QPoint(x, y);
    points[1] = QPoint(x + width / 2, y + heigth_arrow);
    points[2] = QPoint(x + width / 2, y + heigth);
    points[3] = QPoint(x - width / 2, y + heigth);
    points[4] = QPoint(x - width / 2, y + heigth_arrow);

    if (trigger_points != NULL)
    {
        trigger_points[0].rx() = x;
        trigger_points[0].ry() = y + trigger_arrow_range;
        trigger_points[1].rx() = x;
        trigger_points[1].ry() = points[2].y() - trigger_arrow_range;
        trigger_points[2].rx() = points[3].x() + trigger_arrow_range;
        trigger_points[2].ry() = points[3].y() - trigger_arrow_range;
        trigger_points[3].rx() = points[2].x() - trigger_arrow_range;
        trigger_points[3].ry() = points[2].y() - trigger_arrow_range;
    }
}

bool VmSimplePlot::DrawPointsArrow(QPainter* painter, QColor clr, QPoint* points, int point_count)
{
    QPen pen(clr);
    pen.setWidth(1);
    painter->setPen(pen);

    QBrush brush(clr);
    painter->setBrush(brush);

    QPolygon polygon = QPolygon();
    for(int k=0; k<point_count; k++)
        polygon << points[k];
    painter->drawPolygon(polygon);
    return true;
}

void VmSimplePlot::ComputeRects()
{
    m_clientRect = QRect(0,0,width(),height());

    //m_plotarea.m_plotRect
    int height = (m_support_top_info? TOP_INFO_NUM:0 + TOP_INTERVA_NUM ) * RegularSize.height();
    height += (WAVEPOSBAR_NO_INTERVA_NUM) * RegularSize.height();
    m_plotarea.m_plotRect.setTop(m_clientRect.top() + height);

    height = (bootom_axis_disp_num + BOTTOM_INTERVA_TOP_NUM + BOTTOM_INTERVA_BOTTOM_NUM ) * RegularSize.height();
    m_plotarea.m_plotRect.setBottom(m_clientRect.bottom() - height);

    int width = (LEFT_INFO_NUM + LEFT_INTERVA_NUM*2 ) * RegularSize.height();
    m_plotarea.m_plotRect.setLeft(m_clientRect.left() + width);

    width = (RIGHT_INFO_NUM + RIGHT_INTERVA_NUM*2 ) * RegularSize.height();
    m_plotarea.m_plotRect.setRight(m_clientRect.right() - width);

    m_plotarea.grid_resolution_x = m_plotarea.m_plotRect.width()/m_plotarea.grid_count_x;
    m_plotarea.grid_resolution_y = m_plotarea.m_plotRect.height()/m_plotarea.grid_count_y;
    //重新调整大小
    QPoint center = m_plotarea.m_plotRect.center();
    m_plotarea.m_plotRect.setLeft(center.x()-m_plotarea.grid_resolution_x*m_plotarea.grid_count_x/2);
    m_plotarea.m_plotRect.setRight(center.x()+m_plotarea.grid_resolution_x*m_plotarea.grid_count_x/2);
    m_plotarea.m_plotRect.setTop(center.y()-m_plotarea.grid_resolution_y*m_plotarea.grid_count_y/2);
    m_plotarea.m_plotRect.setBottom(center.y()+m_plotarea.grid_resolution_y*m_plotarea.grid_count_y/2);

    //m_timeaxis.m_axisRect
    m_timeaxis.m_axisRect.setLeft(m_plotarea.m_plotRect.left());
    m_timeaxis.m_axisRect.setRight(m_plotarea.m_plotRect.right());
    m_timeaxis.m_axisRect.setTop(m_plotarea.m_plotRect.bottom());
    m_timeaxis.m_axisRect.setBottom(m_clientRect.bottom());
    m_timeaxis.axis_resolution_x=m_plotarea.grid_resolution_x;
    m_timeaxis.axis_count_x=m_plotarea.grid_count_x;

    //Y Axis
    QRect m_axisRect;
    m_axisRect.setLeft(m_clientRect.left());
    m_axisRect.setRight(m_plotarea.m_plotRect.left());
    m_axisRect.setTop(m_plotarea.m_plotRect.top());
    m_axisRect.setBottom(m_plotarea.m_plotRect.bottom());
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        (*Vector_Iter)->m_axisRect=m_axisRect;
        (*Vector_Iter)->axis_resolution_y = m_plotarea.grid_resolution_y;
        (*Vector_Iter)->axis_count_y = m_plotarea.grid_count_y;
        //(*Vector_Iter)->setcenterpos( (*Vector_Iter)->m_axisRect.top+(*Vector_Iter)->m_deault_center_pos_bili*(*Vector_Iter)->m_axisRect.Height() );
    }

    for (auto Vector_Iter = mathAxisVector.begin(); Vector_Iter != mathAxisVector.end(); ++Vector_Iter)
    {
        (*Vector_Iter)->m_axisRect=m_axisRect;
        (*Vector_Iter)->axis_resolution_y = m_plotarea.grid_resolution_y;
        (*Vector_Iter)->axis_count_y = m_plotarea.grid_count_y;
        //(*Vector_Iter)->setcenterpos( (*Vector_Iter)->m_axisRect.top+(*Vector_Iter)->m_deault_center_pos_bili*(*Vector_Iter)->m_axisRect.Height() );
    }

}

void VmSimplePlot::paint(QPainter *painter)
{
    ComputeRects();

    //内存绘图
    QPixmap MemPixmap(m_clientRect.width(), m_clientRect.height());
    QPainter MemPainter(&MemPixmap);

    //网格背景
    DrawBackgroud(&MemPainter);
    DrawGrid(&MemPainter);

    for(auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
        DrawYArrows(&MemPainter, (*Vector_Iter));

    for(auto Vector_Iter = mathAxisVector.begin(); Vector_Iter != mathAxisVector.end(); ++Vector_Iter)
        DrawYArrows(&MemPainter, (*Vector_Iter));

    DrawTimeArrows(&MemPainter);
    DrawValues(&MemPainter);
    // 绘曲线
    UpdateLine(&MemPainter, MemPixmap);
    UpdateMathLine(&MemPainter, MemPixmap);

    // 绘制继承类的多余绘图
    OtherDraw(&MemPainter, painter);

    painter->drawPixmap(0, 0, MemPixmap);
}

bool VmSimplePlot::DrawBackgroud(QPainter *painter)
{
    // 填充背景
    QBrush brush(m_plotarea.m_bgOutClr);
    painter->fillRect(m_clientRect, brush);
    QBrush brush1(m_plotarea.m_bgClr);
    painter->fillRect(m_plotarea.m_plotRect, brush1);
    return true;
}

bool VmSimplePlot::DrawGrid(QPainter *painter)
{
    // 画网格
   if (m_plotarea.bDrawGrid)
    {
        QPen pen(m_plotarea.m_gridClr);
        pen.setStyle(m_plotarea.nGridStyle);
        int line_large = 1;
        pen.setWidth(line_large*2);

        //画边框
        painter->setPen(pen);
        painter->drawRect(m_plotarea.m_plotRect);

        pen.setWidth(line_large);
        painter->setPen(pen);

        // 网格竖线
        for(int l=0; l<=m_plotarea.grid_count_x; l++)
        {
            int x = (int)(m_plotarea.m_plotRect.left()+m_plotarea.grid_begin_x+l*m_plotarea.grid_resolution_x);
            if(x>m_plotarea.m_plotRect.right()+1)
                continue;

            //画大步进线
            //画小步进线用到和确定字体的大小
            double smallaxis = m_plotarea.grid_resolution_y/5.0;
            for(int i=0;i<=m_plotarea.grid_count_y;i++)
            {
                double y=m_plotarea.m_plotRect.top()+i*m_plotarea.grid_resolution_y;

                if (l > 0 && l < m_plotarea.grid_count_x)
                {
                    painter->drawLine((l == 5) || (i == 5) ? x - line_large*GRID_LARGE_1 : x - line_large, (int)y,
                                      (l == 5) || (i == 5) ? x + line_large*GRID_LARGE_1 : x + line_large, (int)y);
                }

                // 画小步进线
                if(y<m_plotarea.m_plotRect.bottom())
                {
                    if(smallaxis<=0)
                        break;  //防止因图太小出现死循环
                    for(int j=1; j<5; j++)
                    {
                        painter->drawLine(l==5? x-line_large*GRID_SMALL_1 :x-line_large, (int)(y+smallaxis*j),
                                          l==5? x+line_large*GRID_SMALL_1 :x+line_large, (int)(y+smallaxis*j));
                    }
                }
            }
        }

        // 网格横线
        for(int l=0; l<=m_plotarea.grid_count_y; l++)
        {
            int y = (int)(m_plotarea.m_plotRect.top()+l*m_plotarea.grid_resolution_y);
            if(y>m_plotarea.m_plotRect.bottom()+1)
                continue;

            //画小步进线用到
            double smallaxis = m_timeaxis.axis_resolution_x/5.0;
            //画大步进线
            for(int i=0; i<=m_timeaxis.axis_count_x; i++) //为了消除启动网格移动后最后一个刻度超出界限
            {
                double x = m_timeaxis.m_axisRect.left()+i*m_timeaxis.axis_resolution_x;
                if (l > 0 && l < m_plotarea.grid_count_y && (x>=m_timeaxis.m_axisRect.left()) )
                {
                    painter->drawLine((int)x, (l == 5) || (i == 5) ? y-line_large*GRID_LARGE_1 :y-line_large,
                                      (int)x, (l == 5) || (i == 5) ? y+line_large*GRID_LARGE_1 :y+line_large);
                }
                //画小步进线
                if(i!=m_timeaxis.axis_count_x+1)
                {
                    if(smallaxis<=0)
                        break;  //防止因图太小出现死循环
                    for(int j=0; j<5; j++)
                    {
                        if( x+smallaxis*j < m_timeaxis.m_axisRect.left()) continue;
                        if( x+smallaxis*j > m_timeaxis.m_axisRect.right()) break;
                        painter->drawLine( (int)(x+smallaxis*j), l==5? y-line_large*GRID_SMALL_1 :y-line_large,
                                           (int)(x+smallaxis*j), l==5? y+line_large*GRID_SMALL_1 :y+line_large);
                    }
                }
            }
        }
    }// 绘网格完毕
    return true;
}

bool VmSimplePlot::DrawYArrows(QPainter *painter, simpleaxis* axis)
{
    //-----------------------------y center----------------------------------
    QPoint points[5];
    {
        painter->setFont(SmallFont);
        QFontMetrics fontMetrics(SmallFont);
        int pixelsWidth = fontMetrics.horizontalAdvance(axis->m_trackname);
        int pixelsHeight = fontMetrics.height();

        int left=m_plotarea.m_plotRect.left()-4;
        //qDebug() << axis->m_trackname << " getyarrowpos " << axis->getyarrowpos() << " " << axis->m_axisRect;
        if(axis->getyarrowpos()<axis->m_axisRect.top())
        {
            GetBottomArrow5Points(points, left-X_ARROW_WIDTH_NUM*RegularSize.height()/2, m_plotarea.m_plotRect.top(), X_ARROW_HEIGHT_NUM*RegularSize.height(),
                                    X_ARROW_WIDTH_NUM*RegularSize.height(), X_ARROW_SHORT_HEIGHT_NUM*RegularSize.height(), ARROW_RANGE_NUM*RegularSize.height());
            DrawPointsArrow(painter, axis->m_lineclr, points, 5);

            axis->m_chnRect=QRect(left-X_ARROW_WIDTH_NUM*RegularSize.height()/2, m_plotarea.m_plotRect.top(),
                                            X_ARROW_WIDTH_NUM*RegularSize.height()/2, X_ARROW_HEIGHT_NUM*RegularSize.height());

            painter->setPen(m_plotarea.m_bgOutClr);
            painter->translate(axis->m_chnRect.center().x(), axis->m_chnRect.bottom());
            painter->rotate(-90);
            painter->drawText(0, 0, axis->m_trackname);
            painter->rotate(90);
            painter->translate(0,0);
        }
        else if(axis->getyarrowpos()>axis->m_axisRect.bottom())
        {
            GetTopArrow5Points(points, left-X_ARROW_WIDTH_NUM*RegularSize.height()/2, m_plotarea.m_plotRect.bottom(), X_ARROW_HEIGHT_NUM*RegularSize.height(),
                                X_ARROW_WIDTH_NUM*RegularSize.height(), X_ARROW_SHORT_HEIGHT_NUM*RegularSize.height(), ARROW_RANGE_NUM*RegularSize.height());
            DrawPointsArrow(painter, axis->m_lineclr, points, 5);

            axis->m_chnRect = QRect(left-X_ARROW_WIDTH_NUM*RegularSize.height()/2, m_plotarea.m_plotRect.bottom()-X_ARROW_HEIGHT_NUM*RegularSize.height(),
                                            X_ARROW_WIDTH_NUM*RegularSize.height(), X_ARROW_HEIGHT_NUM*RegularSize.height());

            painter->setPen(m_plotarea.m_bgOutClr);
            painter->translate(axis->m_chnRect.center().x(), axis->m_chnRect.top());
            painter->rotate(90);
            painter->drawText(0, 0, axis->m_trackname);
            painter->rotate(-90);
            painter->translate(0,0);
        }
        else
        {
            GetLeftArrow5Points(points, axis->m_axisRect.right(), axis->getyarrowpos(), Y_ARROW_HEIGHT_NUM*RegularSize.height(),
                                    Y_ARROW_WIDTH_NUM*RegularSize.height(), Y_ARROW_SHORT_WIDTH_NUM*RegularSize.height(), ARROW_RANGE_NUM*RegularSize.height());
            DrawPointsArrow(painter, axis->m_lineclr, points, 5);

            axis->m_chnRect = QRect(axis->m_axisRect.right()-Y_ARROW_WIDTH_NUM*RegularSize.height(), axis->getyarrowpos()-pixelsHeight/2,
                                                Y_ARROW_WIDTH_NUM*RegularSize.height(), pixelsHeight);
            painter->setPen(m_plotarea.m_bgOutClr);
            painter->drawText(axis->m_chnRect, Qt::AlignLeft, axis->m_trackname);
        }
    }
    return true;
}

bool VmSimplePlot::DrawTimeArrowsNotice(QPainter *painter)
{
    painter->setPen(m_infotext_clr);
    painter->setFont(RegularFont);

    QString str;
    m_plothelp.XAxisValueStr(str, m_timeaxis.GetAxisDanwei(), m_timeaxis.GetHorizontalOffset());
    str = "T-> " + str;
    painter->drawText(m_plotarea.m_plotRect.left()+8, m_plotarea.m_plotRect.top()+8, str);

    return true;
}

bool VmSimplePlot::DrawValues(QPainter *painter)
{
    if(pPlotRecord==nullptr)
        return false;

    painter->setFont(RegularFont);
    QFontMetrics fontMetrics(SmallFont);
    int pixelsHeight = fontMetrics.height();

    int index = 1;
    for(auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        painter->setPen((*Vector_Iter)->m_lineclr);
        QString str = m_plothelp.FrequenceStr(pPlotRecord->getFreq((*Vector_Iter)->m_trackname));
        painter->drawText(m_plotarea.m_plotRect.left()+8, m_plotarea.m_plotRect.bottom()-8-index*pixelsHeight, str);
        index++;
    }
    return true;
}

bool VmSimplePlot::UpdateLine(QPainter* painter, QPixmap& MemPixmap, QRect& rect, double time_min, double time_max, bool full)
{
    if(pPlotRecord==nullptr)
        return false;
    //qDebug() << "rect " << rect << "time_min " << time_min<< "time_max " << time_max;

    // 遍历每一条曲线
    QPoint* pts = new QPoint[rect.width()*2];
    int pts_count = 0;
    int x = rect.x();

    for (auto list_Iter = axisvector.begin(); list_Iter != axisvector.end(); ++list_Iter)
    {
        QPen pen((*list_Iter)->m_lineclr);
        pen.setWidth((*list_Iter)->m_linewidth);
        pen.setStyle((*list_Iter)->m_linestyle);
        painter->setPen(pen);

        double nMin = (*list_Iter)->getminrange();
        double nRange = (*list_Iter)->getrange();
        int nH = rect.bottom() - rect.top();
        int Bt = rect.bottom();

        pts_count = 0;
        plotpoints valuepoints(rect.width());
        valuepoints.timemin = time_min;
        valuepoints.timemax = time_max;
        if(pPlotRecord->getPlotPoints((*list_Iter)->m_trackname, valuepoints))
        {
            for(int i=0;i<valuepoints.cur_pixel_num;i++)
            {
                pts[pts_count].rx()=x+valuepoints.pixel[i];
                pts[pts_count].ry()=Bt-(int)((valuepoints.minvalue[i]-nMin)*nH/nRange);
                pts_count++;
                if(valuepoints.minvalue[i]!=valuepoints.maxvalue[i])
                {
                    pts[pts_count].rx()=x+valuepoints.pixel[i];
                    pts[pts_count].ry()=Bt-(int)((valuepoints.maxvalue[i]-nMin)*nH/nRange);
                    pts_count++;
                }
            }
        }
        painter->drawPolyline(pts, pts_count);
    }

    delete [] pts;
    return true;
}

bool VmSimplePlot::UpdateLine(QPainter* painter, QPixmap& MemPixmap)
{
    UpdateLine(painter, MemPixmap, m_plotarea.m_plotRect, m_timeaxis.getminrange(), m_timeaxis.getmaxrange());
    return true;
}

bool VmSimplePlot::UpdateMathLine(QPainter* painter, QPixmap& MemPixmap, QRect& rect, double time_min, double time_max, bool full)
{
    if(pFFTCal==nullptr)
        return false;
    //qDebug() << "rect " << rect << "time_min " << time_min<< "time_max " << time_max;

    // 遍历每一条曲线
    QPoint* pts = new QPoint[rect.width()*2];
    int pts_count = 0;
    int x = rect.x();

    for (auto list_Iter = mathAxisVector.begin(); list_Iter != mathAxisVector.end(); ++list_Iter)
    {
        QPen pen((*list_Iter)->m_lineclr);
        pen.setWidth((*list_Iter)->m_linewidth);
        pen.setStyle((*list_Iter)->m_linestyle);
        painter->setPen(pen);

        double nMin = (*list_Iter)->getminrange();
        double nRange = (*list_Iter)->getrange();
        int nH = rect.bottom() - rect.top();
        int Bt = rect.bottom();

        pts_count = 0;
        plotpoints valuepoints(rect.width());
        valuepoints.timemin = time_min;
        valuepoints.timemax = time_max;
        if(pFFTCal->getPlotPoints((*list_Iter)->m_trackname, valuepoints))
        {
            for(int i=0;i<valuepoints.cur_pixel_num;i++)
            {
                pts[pts_count].rx()=x+valuepoints.pixel[i];
                pts[pts_count].ry()=Bt-(int)((valuepoints.minvalue[i]-nMin)*nH/nRange);
                pts_count++;
                if(valuepoints.minvalue[i]!=valuepoints.maxvalue[i])
                {
                    pts[pts_count].rx()=x+valuepoints.pixel[i];
                    pts[pts_count].ry()=Bt-(int)((valuepoints.maxvalue[i]-nMin)*nH/nRange);
                    pts_count++;
                }
            }
        }
        painter->drawPolyline(pts, pts_count);
    }

    delete [] pts;
    return true;
}

bool VmSimplePlot::UpdateMathLine(QPainter* painter, QPixmap& MemPixmap)
{
    UpdateMathLine(painter, MemPixmap, m_plotarea.m_plotRect, mathTimeAxis.getminrange(), mathTimeAxis.getmaxrange());
    return true;
}

bool VmSimplePlot::OtherDraw(QPainter *mempainter, QPainter *painter)
{
    mempainter->setFont(RegularFont);

    int index = 0;
    for(auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        mempainter->setPen((*Vector_Iter)->m_lineclr);
        QString str;
        m_plothelp.YAxisValueStr(str, (*Vector_Iter)->GetAxisDanwei(), (*Vector_Iter)->getinterval());
        mempainter->drawText(m_plotarea.m_plotRect.left()+index*RegularSize.height()*8, m_plotarea.m_plotRect.bottom()+RegularSize.height(), str);
        index++;
    }

    for(auto Vector_Iter = mathAxisVector.begin(); Vector_Iter != mathAxisVector.end(); ++Vector_Iter)
    {
        mempainter->setPen((*Vector_Iter)->m_lineclr);
        QString str;
        m_plothelp.YAxisValueStr(str, (*Vector_Iter)->GetAxisDanwei(), (*Vector_Iter)->getinterval());
        mempainter->drawText(m_plotarea.m_plotRect.left()+index*RegularSize.height()*8, m_plotarea.m_plotRect.bottom()+RegularSize.height(), str);
        index++;
    }

    mempainter->setPen(m_infotext_clr);
    QString str;
    m_plothelp.XAxisValueStr(str, m_timeaxis.GetAxisDanwei(), m_timeaxis.getaxisindexvalue());
    mempainter->drawText(m_plotarea.m_plotRect.right()-RegularSize.height()*8, m_plotarea.m_plotRect.bottom()+RegularSize.height(), str);

    return true;
}


