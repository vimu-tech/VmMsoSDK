#include "plotrecord.h"
#include "VmMultMsoLib.h"
#include <QDebug>

PlotRecord::PlotRecord(OscWorkMode& workMode, unsigned short& captureChannel)
    : pWorkMode(workMode)
    , pCaptureChannel(captureChannel)
{

}

//释放资源
PlotRecord::~PlotRecord()
{
    if(bufferCh1!=nullptr)
        delete [] bufferCh1;
    if(bufferCh2!=nullptr)
        delete [] bufferCh2;
    if(bufferCh3!=nullptr)
        delete [] bufferCh3;
    if(bufferCh4!=nullptr)
        delete [] bufferCh4;

    if(trackCh1!=nullptr)
        delete trackCh1;
    if(trackCh2!=nullptr)
        delete trackCh2;
    if(trackCh3!=nullptr)
        delete trackCh3;
    if(trackCh4!=nullptr)
        delete trackCh4;

    if(trackFilter!=nullptr)
        delete [] trackFilter;
}

//设备改变，分配内存
void PlotRecord::devChanged(int oscChannelNum, int memLength)
{
    lock_guard lock(m_mutex);

    if(bufferLengthCh1!=memLength)
    {
        if(bufferCh1!=nullptr)
            delete [] bufferCh1;
        bufferCh1 = new double[memLength];
        bufferLengthCh1 = memLength;
    }

    if(bufferLengthCh2!=memLength)
    {
        if(bufferCh2!=nullptr)
            delete [] bufferCh2;
        if(oscChannelNum>=2)
        {
            bufferCh2 = new double[memLength];
            bufferLengthCh2 = memLength;
        }
    }

    if(bufferLengthCh3!=memLength)
    {
        if(bufferCh3!=nullptr)
            delete [] bufferCh3;
        if(oscChannelNum>=3)
        {
            bufferCh3 = new double[memLength];
            bufferLengthCh3 = memLength;
        }
    }

    if(bufferLengthCh4!=memLength)
    {
        if(bufferCh4!=nullptr)
            delete [] bufferCh4;
        if(oscChannelNum>=4)
        {
            bufferCh4 = new double[memLength];
            bufferLengthCh4 = memLength;
        }
    }
}

void PlotRecord::updateOscRealTimeDatas(unsigned int devId, int captureLength, unsigned int sample)
{
    lock_guard lock(m_mutex);

    mSample = sample;

    mTriggerPoint = ReadVoltageDatasTriggerPoint(devId);
    double timestart = (double)mTriggerPoint * -1000000000.0 / (double)(mSample);

    //直接读取double的电压数据，如果想要adc数据，可以修改为读取adc的api
    if((pCaptureChannel&0x01)==0x01)
    {
        bufferUsedLengthCh1 = ReadVoltageDatas(devId, 0, bufferCh1, captureLength);

        if(trackCh1!=nullptr)
        {
            if((!trackCh1->GetPathFileName().isEmpty())||(trackCh1->GetSample()!=mSample)||(trackCh1->GetCurLength()!=bufferUsedLengthCh1))
            {
                delete trackCh1;
                trackCh1 = nullptr;
            }
        }

        double timelegth = (double)bufferUsedLengthCh1 * 1000000000.0 / (double)(mSample);
        if(trackCh1==nullptr)
        {
            WaveTrackInfo trackinfoCh1(doubleSample, SHIYUBX_SOURCE_CH1, mSample);
            trackCh1 = new WaveTrack(ch1Name, 1, bufferUsedLengthCh1, trackinfoCh1, timestart, timelegth);
            trackCh1->AddData(bufferCh1, bufferUsedLengthCh1);
        }
        else
            trackCh1->ChangeData(bufferCh1, 0, bufferUsedLengthCh1, timestart);

        if(CalFreq(bufferCh1, bufferUsedLengthCh1, GetVoltageResolution(devId, 0), mSample))
        {
            freqCh1 = GetFreq();
            /*double GetPhase();
            double GetPositiveDuty();
            double GetNegativeDuty();*/
        }
        else
            freqCh1 = 0;
    }
    else
    {
        if(trackCh1!=nullptr)
            delete trackCh1;
        trackCh1 = nullptr;
    }

    if((pCaptureChannel&0x02)==0x02)
    {
        bufferUsedLengthCh2 = ReadVoltageDatas(devId, 1, bufferCh2, captureLength);

        if(trackCh2!=nullptr)
        {
            if((!trackCh2->GetPathFileName().isEmpty())||(trackCh2->GetSample()!=mSample)||(trackCh2->GetCurLength()!=bufferUsedLengthCh2))
            {
                delete trackCh2;
                trackCh2 = nullptr;
            }
        }

        double timelegth = (double)bufferUsedLengthCh2 * 1000000000.0 / (double)(mSample);
        if(trackCh2==nullptr)
        {
            WaveTrackInfo trackinfoCh2(doubleSample, SHIYUBX_SOURCE_CH2, mSample);
            trackCh2 = new WaveTrack(ch2Name, 1, bufferUsedLengthCh2, trackinfoCh2, timestart, timelegth);
            trackCh2->AddData(bufferCh2, bufferUsedLengthCh2);
        }
        else
            trackCh2->ChangeData(bufferCh2, 0, bufferUsedLengthCh2, timestart);

        if(CalFreq(bufferCh2, bufferUsedLengthCh2, GetVoltageResolution(devId, 1), mSample))
        {
            freqCh2 = GetFreq();
        }
        else
            freqCh2 = 0;
    }
    else
    {
        if(trackCh2!=nullptr)
            delete trackCh2;
        trackCh2 = nullptr;
    }

    if((pCaptureChannel&0x04)==0x04)
    {
        bufferUsedLengthCh3 = ReadVoltageDatas(devId, 2, bufferCh3, captureLength);

        if(trackCh3!=nullptr)
        {
            if((!trackCh3->GetPathFileName().isEmpty())||(trackCh3->GetSample()!=mSample)||(trackCh3->GetCurLength()!=bufferUsedLengthCh3))
            {
                delete trackCh3;
                trackCh3 = nullptr;
            }
        }

        double timelegth = (double)bufferUsedLengthCh3 * 1000000000.0 / (double)(mSample);
        if(trackCh3==nullptr)
        {
            WaveTrackInfo trackinfoCh3(doubleSample, SHIYUBX_SOURCE_CH3, mSample);
            trackCh3 = new WaveTrack(ch3Name, 1, bufferUsedLengthCh3, trackinfoCh3, timestart, timelegth);
            trackCh3->AddData(bufferCh3, bufferUsedLengthCh3);
        }
        else
            trackCh3->ChangeData(bufferCh3, 0, bufferUsedLengthCh3, timestart);

        if(CalFreq(bufferCh3, bufferUsedLengthCh3, GetVoltageResolution(devId, 2), mSample))
        {
            freqCh3 = GetFreq();
        }
        else
            freqCh3 = 0;
    }
    else
    {
        if(trackCh3!=nullptr)
            delete trackCh3;
        trackCh3 = nullptr;
    }

    if((pCaptureChannel&0x08)==0x08)
    {
        bufferUsedLengthCh4 = ReadVoltageDatas(devId, 3, bufferCh4, captureLength);

        if(trackCh4!=nullptr)
        {
            if((!trackCh4->GetPathFileName().isEmpty())||(trackCh4->GetSample()!=mSample)||(trackCh4->GetCurLength()!=bufferUsedLengthCh4))
            {
                delete trackCh4;
                trackCh4 = nullptr;
            }
        }

        double timelegth = (double)bufferUsedLengthCh4 * 1000000000.0 / (double)(mSample);
        if(trackCh4==nullptr)
        {
            WaveTrackInfo trackinfoCh4(doubleSample, SHIYUBX_SOURCE_CH4, mSample);
            trackCh4 = new WaveTrack(ch4Name, 1, bufferUsedLengthCh4, trackinfoCh4, timestart, timelegth);
            trackCh4->AddData(bufferCh4, bufferUsedLengthCh4);
        }
        else
            trackCh4->ChangeData(bufferCh4, 0, bufferUsedLengthCh4, timestart);

        if(CalFreq(bufferCh4, bufferUsedLengthCh4, GetVoltageResolution(devId, 3), mSample))
        {
            freqCh4 = GetFreq();
        }
        else
            freqCh4 = 0;
    }
    else
    {
        if(trackCh4!=nullptr)
            delete trackCh4;
        trackCh4 = nullptr;
    }
}

bool PlotRecord::getPlotPoints(QString& trackName, plotpoints& ppoints)
{
    lock_guard lock(m_mutex);

    bool success = false;
    if((trackName==ch1Name)&&(trackCh1!=nullptr))
        success = trackCh1->GetPlotPoints(&ppoints);
    else if((trackName==ch2Name)&&(trackCh2!=nullptr))
        success = trackCh2->GetPlotPoints(&ppoints);
    else if((trackName==ch3Name)&&(trackCh3!=nullptr))
        success = trackCh3->GetPlotPoints(&ppoints);
    else if((trackName==ch4Name)&&(trackCh4!=nullptr))
        success = trackCh4->GetPlotPoints(&ppoints);
    else if((trackName==filterName)&&(trackFilter!=nullptr))
        success = trackFilter->GetPlotPoints(&ppoints);
    //qDebug()<<"GetPlotPoints success" << success <<" cur_pixel_num "<< ppoints.cur_pixel_num;
    return success;
}

double PlotRecord::getFreq(QString& trackName){
    double freq = 0;

    if(trackName==ch1Name)
        freq = freqCh1;
    else if(trackName==ch2Name)
        freq = freqCh2;
    else if(trackName==ch3Name)
        freq = freqCh3;
    else if(trackName==ch4Name)
        freq = freqCh4;

    return freq;
}

double* PlotRecord::getVolDatas(SHIYUBX_SOURCE source)
{
    if(source==SHIYUBX_SOURCE_CH1)
        return bufferCh1;
    else if(source==SHIYUBX_SOURCE_CH2)
        return bufferCh2;
    else if(source==SHIYUBX_SOURCE_CH3)
        return bufferCh3;
    else if(source==SHIYUBX_SOURCE_CH4)
        return bufferCh4;
    return nullptr;
}

int PlotRecord::getVolDataLength(SHIYUBX_SOURCE source)
{
    if(source==SHIYUBX_SOURCE_CH1)
        return bufferUsedLengthCh1;
    else if(source==SHIYUBX_SOURCE_CH2)
        return bufferUsedLengthCh2;
    else if(source==SHIYUBX_SOURCE_CH3)
        return bufferUsedLengthCh3;
    else if(source==SHIYUBX_SOURCE_CH4)
        return bufferUsedLengthCh4;
    return 0;
}

bool PlotRecord::startStreamBuffer(QString filepathname, unsigned int buffer_size, int sample,
                                   unsigned short captureChannel, unsigned int adcbit, double totaltime)
{
    lock_guard lock(m_mutex);

    mSample = sample;

    int chnnum = 0;
    if(captureChannel&0x01)
        chnnum++;
    if(captureChannel&0x02)
        chnnum++;
    if(captureChannel&0x04)
        chnnum++;
    if(captureChannel&0x08)
        chnnum++;

    unsigned int length = buffer_size/chnnum/((adcbit>8)? 2:1);
    double m_timelegth = (double)length * 1000000000.0 / (double)(mSample);
    //qDebug()<<"PlotRecord::startStreamBuffer length "<< length << " m_timelegth " << m_timelegth;

    if(trackCh1!=nullptr)
        delete trackCh1;
    trackCh1=nullptr;
    if(captureChannel&0x01)
    {
        WaveTrackInfo trackinfoCh1(filepathname, true, doubleSample, SHIYUBX_SOURCE_CH1, mSample);
        trackCh1 = new WaveTrack(ch1Name, -1, length, trackinfoCh1, 0, m_timelegth);
    }

    if(trackCh2!=nullptr)
        delete trackCh2;
    trackCh2=nullptr;
    if(captureChannel&0x02)
    {
        WaveTrackInfo trackinfoCh2(filepathname, true, doubleSample, SHIYUBX_SOURCE_CH2, mSample);
        trackCh2 = new WaveTrack(ch2Name, -1, length, trackinfoCh2, 0, m_timelegth);
    }

    if(trackCh3!=nullptr)
        delete trackCh3;
    trackCh3=nullptr;
    if(captureChannel&0x04)
    {
        WaveTrackInfo trackinfoCh3(filepathname, true, doubleSample, SHIYUBX_SOURCE_CH3, mSample);
        trackCh3 = new WaveTrack(ch3Name, -1, length, trackinfoCh3, 0, m_timelegth);
    }

    if(trackCh4!=nullptr)
        delete trackCh4;
    trackCh4=nullptr;
    if(captureChannel&0x08)
    {
        WaveTrackInfo trackinfoCh4(filepathname, true, doubleSample, SHIYUBX_SOURCE_CH4, mSample);
        trackCh4 = new WaveTrack(ch4Name, -1, length, trackinfoCh4, 0, m_timelegth);
    }

    mStreamTotalTime = totaltime;
    return true;
}

bool PlotRecord::updateStreamBuffer(unsigned char* buffer, unsigned int buffer_size, unsigned short captureChannel, unsigned int adcbit,
                                    double zoomch1, double biasch1, double zoomch2, double biasch2,
                                    double zoomch3, double biasch3, double zoomch4, double biasch4)
{
    lock_guard lock(m_mutex);

    int chnnum = 0;
    if(captureChannel&0x01)
        chnnum++;
    if(captureChannel&0x02)
        chnnum++;
    if(captureChannel&0x04)
        chnnum++;
    if(captureChannel&0x08)
        chnnum++;

    //如果是3个通道，需要占用4个通道对齐存储
    if(chnnum==3)
        chnnum=4;

    unsigned int length = buffer_size/chnnum/((adcbit>8)? 2:1);
    assert(length<=bufferLengthCh1);

    qDebug()<<"PlotRecord::UpdateStreamBuffer chnnum "<< chnnum << " length " << length;

    if(adcbit>8)
    {
        int chnindex = 0;
        if(captureChannel&0x01)
        {
            for(unsigned int k=0; k<length; k++)
                bufferCh1[k] = ((unsigned short*)buffer)[k*chnnum+chnindex]*zoomch1+biasch1;
            chnindex++;
        }

        if(captureChannel&0x02)
        {
            for(unsigned int k=0; k<length; k++)
                bufferCh2[k] = ((unsigned short*)buffer)[k*chnnum+chnindex]*zoomch2+biasch2;
            chnindex++;
        }

        if(captureChannel&0x04)
        {
            for(unsigned int k=0; k<length; k++)
                bufferCh3[k] = ((unsigned short*)buffer)[k*chnnum+chnindex]*zoomch3+biasch3;
            chnindex++;
        }

        if(captureChannel&0x08)
        {
            for(unsigned int k=0; k<length; k++)
                bufferCh4[k] = ((unsigned short*)buffer)[k*chnnum+chnindex]*zoomch4+biasch4;
            chnindex++;
        }
    }
    else
    {
        int chnindex = 0;
        if(captureChannel&0x01)
        {
            for(unsigned int k=0; k<length; k++)
                bufferCh1[k] = ((unsigned char*)buffer)[k*chnnum+chnindex]*zoomch1+biasch1;
            chnindex++;
        }

        if(captureChannel&0x02)
        {
            for(unsigned int k=0; k<length; k++)
                bufferCh2[k] = ((unsigned char*)buffer)[k*chnnum+chnindex]*zoomch2+biasch2;
            chnindex++;
        }

        if(captureChannel&0x04)
        {
            for(unsigned int k=0; k<length; k++)
                bufferCh3[k] = ((unsigned char*)buffer)[k*chnnum+chnindex]*zoomch3+biasch3;
            chnindex++;
        }

        if(captureChannel&0x08)
        {
            for(unsigned int k=0; k<length; k++)
                bufferCh4[k] = ((unsigned char*)buffer)[k*chnnum+chnindex]*zoomch4+biasch4;
            chnindex++;
        }
    }

    if((captureChannel&0x01)&&(trackCh1!=nullptr))
        trackCh1->AddData(bufferCh1, length);
    if((captureChannel&0x02)&&(trackCh2!=nullptr))
        trackCh2->AddData(bufferCh2, length);
    if((captureChannel&0x04)&&(trackCh3!=nullptr))
        trackCh3->AddData(bufferCh3, length);
    if((captureChannel&0x08)&&(trackCh4!=nullptr))
        trackCh4->AddData(bufferCh4, length);

    qDebug()<<"PlotRecord::UpdateStreamBuffer length "<< length;
    return true;
}

void PlotRecord::updateFilterDatas(double* buffer, int triggerPoint, int length)
{
    double timestart = (double)triggerPoint * -1000000000.0 / (double)(mSample);

    //直接读取double的电压数据，如果想要adc数据，可以修改为读取adc的api
    if(length>0)
    {
        if(trackFilter!=nullptr)
        {
            if(trackFilter->GetCurLength()!=length)
            {
                delete trackFilter;
                trackFilter = nullptr;
            }
        }

        double timelegth = (double)length * 1000000000.0 / (double)(mSample);
        if(trackFilter==nullptr)
        {
            WaveTrackInfo trackinfo(doubleSample, mSample);
            trackFilter = new WaveTrack(filterName, 1, length, trackinfo, timestart, timelegth);
            trackFilter->AddData(buffer, length);
        }
        else
            trackFilter->ChangeData(buffer, 0, length, timestart);
    }
    else
    {
        if(trackFilter!=nullptr)
            delete trackFilter;
        trackFilter = nullptr;
    }
}
