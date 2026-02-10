#ifndef PLOTRECORD_H
#define PLOTRECORD_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include "defines.h"
#include "wavemng/WaveTrack.h"
#include <mutex>

class PlotRecord
{
public:
    PlotRecord(OscWorkMode& workMode, unsigned short& captureChannel);
    ~PlotRecord();

    void devChanged(int oscChannelNum, int memLength);

    void updateOscRealTimeDatas(unsigned int devId, int captureLength, unsigned int sample);

    bool startStreamBuffer(QString filename, unsigned int buffer_size, int sample,
                           unsigned short captureChannel, unsigned int adcbit, double totaltime);
    bool updateStreamBuffer(unsigned char* buffer, unsigned int buffer_size, unsigned short captureChannel, unsigned int adcbit,
                            double zoomch1, double biasch1, double zoomch2, double biasch2,
                            double zoomch3, double biasch3, double zoomch4, double biasch4);

    bool getPlotPoints(QString& trackName, plotpoints& ppoints);

    double getFreq(QString& trackName);

    double* getVolDatas(SHIYUBX_SOURCE source);
    int getVolDataLength(SHIYUBX_SOURCE source);
    double getTriggerPoint(){return mTriggerPoint;}

    //-----------------------------------------------------------------------------------------
    void updateFilterDatas(double* buffer, int triggerPoint, int length);
private:
    typedef std::lock_guard<std::recursive_mutex> lock_guard;
    std::recursive_mutex m_mutex;

    OscWorkMode& pWorkMode;
    unsigned short& pCaptureChannel;

    unsigned int mSample = 0;
    double mTriggerPoint = 0;

    //底层数据读取
    double* bufferCh1 = nullptr;
    double* bufferCh2 = nullptr;
    double* bufferCh3 = nullptr;
    double* bufferCh4 = nullptr;
    int bufferLengthCh1 = 0;
    int bufferLengthCh2 = 0;
    int bufferLengthCh3 = 0;
    int bufferLengthCh4 = 0;
    int bufferUsedLengthCh1 = 0;
    int bufferUsedLengthCh2 = 0;
    int bufferUsedLengthCh3 = 0;
    int bufferUsedLengthCh4 = 0;

    //数据缓冲和加速
    WaveTrack* trackCh1 = nullptr;
    WaveTrack* trackCh2 = nullptr;
    WaveTrack* trackCh3 = nullptr;
    WaveTrack* trackCh4 = nullptr;

    double freqCh1 = 0;
    double freqCh2 = 0;
    double freqCh3 = 0;
    double freqCh4 = 0;

    double mStreamTotalTime = 0;

    //-----------------------------------------------------------------------------------------
    WaveTrack* trackFilter = nullptr;
};

#endif // PLOTRECORD_H
