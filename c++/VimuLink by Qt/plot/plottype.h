#ifndef PLOTTYPE_H
#define PLOTTYPE_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <QColor>

#define FOCUS_CLR  QColor(255, 255, 0)
#define PASSFAIL_OUT_BACKCLR  QColor(10, 64, 1)
#define PASSFAIL_BACKCLR  QColor(10, 10, 10)

//显示模式
#define SIMPLE_AXIS_CLR  QColor(180,200,180)
#define SIMPLE_GRID_CLR  QColor(32,148,132)
#define SIMPLE_PLOT_BACKCLR  QColor(42,47,48)  //RGB(15,15,15) //RGB(64,64,64)  //
#define SIMPLE_PLOT_OUTBACKCLR  QColor(55,55,55)  //RGB(43,51,31)  //RGB(6,9,9)
#define SIMPLE_FACE_FONT  QColor(255, 255, 0)
#define SIMPLE_BAR_BACK_CLR SIMPLE_PLOT_BACKCLR
#define SIMPLE_BAR_BACK_LIGHT_CLR SIMPLE_GRID_CLR //SIMPLE_PLOT_BACKCLR_LIGHT

//-------------------------------------------------------------------------
#define DEFAULT_LOGIC_ERROR_CLR QColor(255, 0, 0)
#define DEFAULT_LOGIC_WRANING_CLR QColor(255, 255, 0)

#define LINE_DEFAULT_CH1_CLR QColor(255, 255, 0)
#define LINE_DEFAULT_CH2_CLR QColor(0, 255, 255)
#define LINE_DEFAULT_MATH_CLR QColor(255, 0, 0)
#define LINE_DEFAULT_LISR_CLR QColor(255, 255, 0)
#define LINE_DEFAULT_XIANGHU_CLR QColor(255, 0, 255)
#define LINE_DEFAULT_SWEEP_CLR QColor(255, 255, 0)
#define LINE_DEFAULT_LOGIC_CLR QColor(255, 251, 240)
#define LINE_DEFAULT_FILTER1_CLR QColor(0, 255, 0)
#define LINE_DEFAULT_FILTER2_CLR QColor(160, 160, 164)

#define LINE_PRINT_DEFAULT_CH1_CLR QColor(200, 0, 0)
#define LINE_PRINT_DEFAULT_CH2_CLR QColor(0, 0, 255)
#define LINE_PRINT_DEFAULT_MATH_CLR QColor(0, 200, 0)
#define LINE_PRINT_DEFAULT_LISR_CLR QColor(200, 0, 0)
#define LINE_PRINT_DEFAULT_XIANGHU_CLR QColor(200, 0, 200)
#define LINE_PRINT_DEFAULT_SWEEP_CLR QColor(200, 0, 0)
#define LINE_PRINT_DEFAULT_LOGIC_CLR QColor(200, 0, 0)
#define LINE_PRINT_DEFAULT_FILTER1_CLR QColor(0, 200, 0)
#define LINE_PRINT_DEFAULT_FILTER2_CLR QColor(128, 0, 128)



#define TOP_INFO_NUM 1//一行文字
#define TOP_INTERVA_NUM 0 //一行空格
#define WAVEPOSBAR_NO_INTERVA_NUM 1.5
#define BOTTOM_INTERVA_BOTTOM_NUM 0.5
#define BOTTOM_INTERVA_TOP_NUM 1
#define LEFT_INFO_NUM 3
#define LEFT_INTERVA_NUM 0 //一行空格
#define RIGHT_INFO_NUM 3
#define RIGHT_INTERVA_NUM 0 //一行空格

#define SIMPLOT_AXISBAR_INTERVA_NUM 0.8
#define SIMPLOT_AXISBAR_WIDTH_NUM 9
//#define SIMPLOT_AXISBAR_DB_WIDTH_NUM 7.5
#define SIMPLOT_AXISBAR_FREQ_WIDTH_NUM 7

#define SMALL_FONT_BILI 0.9

#define ARROW_RANGE_NUM 0.1
#define Y_ARROW_WIDTH_NUM 2
#define Y_ARROW_HEIGHT_NUM 1.1
#define Y_ARROW_SHORT_WIDTH_NUM 0.5
#define X_ARROW_WIDTH_NUM 1.1
#define X_ARROW_HEIGHT_NUM 2
#define X_ARROW_SHORT_HEIGHT_NUM 0.5

#define SMALL_ARROW_RANGE_NUM 0.15
#define Y_SMALL_ARROW_WIDTH_NUM 1
#define Y_SMALL_ARROW_HEIGHT_NUM 0.8
#define Y_SMALL_ARROW_SHORT_WIDTH_NUM 0.2
#define X_SMALL_ARROW_WIDTH_NUM 0.8
#define X_SMALL_ARROW_HEIGHT_NUM 1
#define X_SMALL_ARROW_SHORT_HEIGHT_NUM 0.2

//Selectline上下三角的大小
#define LINRRANGLE_NUM 0.3125

//大步进值线的长度
#define WIDTH_LARGRAXIS_NUM  0.3125
//小步进值线的长度
#define WIDTH_SMALLAXIS_NUM  0.125
//Grid长度
#define GRID_PRO_SMALL_N 1
#define GRID_SMALL_1 1
#define GRID_LARGE_1 2

#define AXIS_LENGTH_NUM 0.3125
#define AXIS_ARROW_NUM 0.25
#define AXIS_OUTSIDE_NUM 0.125
#define AXIS_INTERVAL_NUM 0.75

#define TOP_BOTTOM_DRAW_INV_NUM 0.4

#define SIMPLE_OUTSIDE_NUM 0.25

#endif

