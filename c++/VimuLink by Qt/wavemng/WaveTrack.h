#ifndef WAVETRACK_H
#define WAVETRACK_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/


#include <list>
#include "BlockFile.h"
#include "plotpoints.h"
#include "TempFileMng.h"

class WaveRecord
{
public:
    WaveRecord(void);
    virtual ~WaveRecord(void);

private:
	bool dirty;
	plotpoints *ppoints;
	//std::mutex m_lock;
public:
	//plotpoints *GetPlotPoints(){return ppoints;};
	void Dirty(){dirty=true;};
	bool IsDirty(){return dirty;};
	void Clear()
	{
		dirty=true; 
		if(ppoints!= nullptr)
			ppoints->samplesPerPixel=0.0;
	};

	bool GetRecord(plotpoints* points);
	void SetRecord(plotpoints* points);
	bool GetPoints(plotpoints* points, int from, int start, int len);

	bool IsInRange(double start_x, double stop_x)
	{
		bool in = true;
		if (ppoints != nullptr)
			in = (start_x >= ppoints->timerealmin) && (stop_x <= ppoints->timerealmax);
		return in;
	}

	bool IsRange(int x, double y)
	{
		if(ppoints!= nullptr)
			return ppoints->IsRange(x,y);
		return false;
	}

	bool IsRange(double y)
	{
		if(ppoints!= nullptr)
			return ppoints->IsRange(y);
		return false;
	}

	bool GetPoint(int *x, double *y)
	{
		if(ppoints!= nullptr)
			return ppoints->getpoint(x,y);
		return false;
	}

	bool GetDisplayPoint(int x, double *y)
	{
		if(ppoints!= nullptr)
			return ppoints->GetDisplayPoint(x,y);
		return false;
	}
	
	char GetXMeasurePoints(point* leftpoint, point* rightpoint)
	{
		if(ppoints!= nullptr)
			return ppoints->GetXMeasurePoints(leftpoint, rightpoint);
		return false;
	}

	char GetDutyMeasurePoints(point* point1, point* point2, point* point3)
	{
		if(ppoints!= nullptr)
			return ppoints->GetDutyMeasurePoints(point1, point2, point3);
		return false;
	}

	char GetYMeasurePoints(point* leftpoint, point* rightpoint)
	{
		if(ppoints!= nullptr)
			return ppoints->GetYMeasurePoints(leftpoint, rightpoint);
		return false;
	}

	char GetMinMaxPoints(int pixel_start, double x_start, int pixel_stop, double x_stop, point* min_point, point* max_point)
	{
		if (ppoints != nullptr)
			return ppoints->GetMinMaxPoints(pixel_start, x_start, pixel_stop, x_stop, min_point, max_point);
		return false;
	}

	bool GetPlotRecordRange(double *min, double *max)
	{
		if((ppoints!= nullptr)&&(!dirty))
		{
			*min=ppoints->bufmin;
			*max=ppoints->bufmax;
			return true;
		}
		return false;
	}
};

class WaveTrack
{
public:
	//用于wave格式数据
    WaveTrack(const QString& name, int maxcount, uint64_t blocklen, WaveTrackInfo& waveinfo,
                                 double timebegin, double timeblocklen);
	virtual ~WaveTrack(void);

    const QString& GetName() {return name;};
    const QString& GetPathFileName() {return trackinfo.GetPathName();};

    //track信息
    sampleFormat GetFormat(){return trackinfo.GetSampleFormat();};
    SHIYUBX_SOURCE GetBoxingId(){return trackinfo.GetBoxingId();};
    uint64_t GetSample(){return trackinfo.GetSamples();};

    virtual bool ChangeData(void* buf, int64_t offset, uint64_t len, double timebegin);
    virtual bool ChangeData(void* buf, int64_t offset, uint64_t len);
    virtual bool AddData(void* buf, uint64_t len);
    virtual bool GetData(double* buf, int64_t start, uint64_t* len);

	void DestoryAll();

    virtual void Reset();
    virtual void MoveTime(double move);

    virtual bool IsRange(int x, double y) {return plotrecord.IsRange(x,y);};
    virtual bool IsRange(double y) {return plotrecord.IsRange(y);};
    virtual bool GetPoint(int *x, double *y) {return plotrecord.GetPoint(x,y);};
    virtual bool GetDisplayPoint(int x, double *y) {return plotrecord.GetDisplayPoint(x,y);};
    virtual char GetXMeasurePoints(point* leftpoint, point* rightpoint) {return plotrecord.GetXMeasurePoints(leftpoint, rightpoint);};
    virtual char GetDutyMeasurePoints(point* point1, point* point2, point* point3) {return plotrecord.GetDutyMeasurePoints(point1, point2, point3);};
    virtual char GetYMeasurePoints(point* leftpoint, point* rightpoint) {return plotrecord.GetYMeasurePoints(leftpoint, rightpoint);};
    virtual char GetIntervalFromMinToMaxRms(int pixel_start, double x_start, int pixel_stop, double x_stop, double* rms);
    virtual bool GetPlotPoints(plotpoints* points);
    virtual bool GetFullPlotPoints(plotpoints* points);

    virtual bool GetXAxisRange(double *min, double *max);
    virtual bool GetPlotRecordRange(double *min, double *max) {return plotrecord.GetPlotRecordRange(min, max);};

    virtual uint64_t GetCurLength();
    virtual double GetTimeMinResolution(double defaultresolution) {return curblockcount<=0? defaultresolution : time_blocklen/blocklen;};

private:
	//block list
	std::list<BlockFile*> filelist;
    //记录上次绘图数据情况
    WaveRecord plotrecord;
    WaveRecord fullplotrecord;

	//用于记录仪模式的数据叠计数
	int64_t adddata_count;

	//时间轴的起始，和每个block代表的时间
	double time_begin,time_blocklen;

    //Track名字
    QString name;
    //track信息
    WaveTrackInfo trackinfo;

    //最大Block的个数
    int maxblockcount;
    //现在填满的Block个数
    int curblockcount;
    //每个Block的大小
    int64_t blocklen;

    //临时文件
    TempFileMng tempmng;

private:
	//拷贝构造函数禁止
	//WaveTrack(WaveTrack& t);
	WaveTrack(const WaveTrack& t);

	bool AddData(void* buf, int64_t offset, uint64_t len);

    bool GetPlotPoints(WaveRecord& plotrcd, plotpoints* points);

	int GenPlotPoints(uint64_t divisor, int plotbegin, double totalreadbegin, double readbegin, uint64_t readlen,  plotpoints* points);

	//读取文件计算rms
	double ReadAndCalRma(uint64_t readbegin, uint64_t readlen);
	double ReadAndCalFromMinToMaxRma(uint64_t readbegin, uint64_t* readlen);
};

#endif
