#include "vmusbwave.h"
#include <QDebug>
#include <thread>
#include "arbwaves.h"

#define EN_LOG 0
#define EN_WATCH_DOG 1

VmUsbWave::VmUsbWave(QObject *parent)
    : QObject{parent}
    , plotRecord(workMode, mCaptureChannel)
{
     InitDll(EN_LOG, EN_WATCH_DOG);
     SetDevNoticeCallBack(this, UsbDevAddCallBack, UsbDevRemoveCallBack);
     SetDataReadyCallBack(this, UsbDataReadyCallBack);
     SetStreamRawDataReadyCallBack(this, StreamRawDataReadyCallBack);
     SetIOReadStateCallBack(this, IOReadStateCallBack);
     SetMultMeterCallBack(this, MultMeterUpdateCallBack);

     std::thread m_work_thread(m_work_thread_fun, this);
     m_work_thread.detach();

     //2秒后，扫描设备
     connect(&noteTimer, &QTimer::timeout, this, &VmUsbWave::noteTimeout);
     noteTimer.setSingleShot(true);
     noteTimer.start(2000);
}

VmUsbWave::~VmUsbWave()
{
    m_work_thread_quit = true;
    notify_work_thread_exit();
    while (!m_work_thread_quit_success)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };

    FinishDll();
}

void VmUsbWave::noteTimeout()
{
    ScanDevice();
}

void VmUsbWave::setWorkMode(int mode)
{
    if(workMode!=mode)
    {
        workMode = (OscWorkMode)mode;

        if(workMode==OscRealTime)
            InitOscRealTimeMode();
        else
            InitOscStreamMode();

        emit workModeChanged();
    }
}

void VmUsbWave::InitOscRealTimeMode()
{
    if(mDevId!=(unsigned int)-1)
    {
        SetOscCaptureMode(mDevId, 0);

        SetAcDc(mDevId, 0, false);
        SetAcDc(mDevId, 1, false);
        SetAcDc(mDevId, 2, false);
        SetAcDc(mDevId, 3, false);

        setAcEnableCh1(false);
        setAcEnableCh2(false);
        setAcEnableCh3(false);
        setAcEnableCh4(false);

        triggerMode = TriggerModeAuto;
        emit triggerModeChanged();
        SetTriggerMode(mDevId, triggerMode);

        triggerStyle = TriggerStyleRising;
        emit triggerStyleChanged();
        SetTriggerStyle(mDevId, triggerStyle);

        triggerSourceList.clear();
        triggerSourceList.push_back(ch1Name);
        if(mOscChannelNum>1)
            triggerSourceList.push_back(ch2Name);
        if(mOscChannelNum>2)
            triggerSourceList.push_back(ch3Name);
        if(mOscChannelNum>3)
            triggerSourceList.push_back(ch4Name);

        if(mDIOChannelNum>0)
        {
            triggerSourceList.push_back(dio0Name);
            triggerSourceList.push_back(dio1Name);
            triggerSourceList.push_back(dio2Name);
            triggerSourceList.push_back(dio3Name);
            triggerSourceList.push_back(dio4Name);
            triggerSourceList.push_back(dio5Name);
            triggerSourceList.push_back(dio6Name);
            triggerSourceList.push_back(dio7Name);
        }
        if(mDIOChannelNum>8)
        {
            triggerSourceList.push_back(dio8Name);
            triggerSourceList.push_back(dio9Name);
            triggerSourceList.push_back(dio10Name);
            triggerSourceList.push_back(dio11Name);
            triggerSourceList.push_back(dio12Name);
            triggerSourceList.push_back(dio13Name);
            triggerSourceList.push_back(dio13Name);
            triggerSourceList.push_back(dio14Name);
        }
        triggerSourceIndex = 0;
        emit triggerSourceListChanged();
        int code = triggerSourceIndex2Code(triggerSourceIndex);
        SetTriggerSource(mDevId, code);

        triggerLevelmV = 0;
        emit triggerLevelmVChanged();
        triggerSense = 0.1;
        emit triggerSenseChanged();

        double yIntserval = 1.0;
        if(code==0)
            yIntserval = (m_ch1_plot_range_max - m_ch1_plot_range_min)/10;
        else if(code==1)
            yIntserval = (m_ch2_plot_range_max - m_ch2_plot_range_min)/10;
        else if(code==2)
            yIntserval = (m_ch3_plot_range_max - m_ch3_plot_range_min)/10;
        else if(code==3)
            yIntserval = (m_ch4_plot_range_max - m_ch4_plot_range_min)/10;

        SetTriggerLevelmV(mDevId, triggerLevelmV, triggerSense*yIntserval);

        preTriggerPercent = 50;
        emit preTriggerPercentChanged();
        SetPreTriggerPercent(mDevId, preTriggerPercent);


        mSupportSamples.clear();
        int num = GetOscSupportSampleRateNum(mDevId);
        if(samples!=nullptr)
            delete []samples;
        samples = new unsigned int[num];
        num = GetOscSupportSampleRates(mDevId, samples, num);
        if(num)
        {
            for(int i=0; i<num; i++)
                mSupportSamples.push_back(QString::number(samples[i]));
        }
        emit supportSamplesChanged();

        sampleIndex = num-1;
        emit sampleIndexChanged();

        mCaptureLength = GetMemoryLength(mDevId);
        mCaptureLength = mCaptureLength*1024;//K转换

        //初始化缓冲区,对于大内存设备，这里会分配很大的内存，如果不需要，可以根据需要调整
        plotRecord.devChanged(mOscChannelNum, mCaptureLength);

        //为了刷新速度，demo使用1M，可以根据需求修改
        //For the refresh speed, the demo uses a maximum of 4M, which can be modified according to needs
        if(mCaptureLength>1024*1024)
            mCaptureLength = 1024*1024;

        emit captureLengthChanged();
    }
}

void VmUsbWave::InitOscStreamMode()
{
    if(mDevId!=(unsigned int)-1)
    {
        SetOscCaptureMode(mDevId, 1);

        SetStreamAcDc(mDevId, 0, false);
        SetStreamAcDc(mDevId, 1, false);
        SetStreamAcDc(mDevId, 2, false);
        SetStreamAcDc(mDevId, 3, false);

        setAcEnableCh1(false);
        setAcEnableCh2(false);
        setAcEnableCh3(false);
        setAcEnableCh4(false);

        mSupportSamples.clear();
        int num = GetStreamSupportSampleRateNum(mDevId);
        if(samples!=nullptr)
            delete []samples;
        samples = new unsigned int[num];
        num = GetStreamSupportSampleRates(mDevId, samples, num);
        if(num)
        {
            for(int i=0; i<num; i++)
                mSupportSamples.push_back(QString::number(samples[i]));
        }
        emit supportSamplesChanged();

        //使用1M采样率，
        sampleIndex = 9;
        emit sampleIndexChanged();

        //采集10M数据
        mCaptureLength = 10*1000*1024;
        emit captureLengthChanged();
    }
}


void VmUsbWave::setPlotRangeCh1(QVariantList list)
{
    if(list.size()==2)
    {
        m_ch1_plot_range_min = list.at(0).toDouble()*1000;
        m_ch1_plot_range_max = list.at(1).toDouble()*1000;
        qInfo() << "setPlotRangeCh1 " << m_ch1_plot_range_min << " " << m_ch1_plot_range_max;
    }
}

void VmUsbWave::setPlotRangeCh2(QVariantList list)
{
    if(list.size()==2)
    {
        m_ch2_plot_range_min = list.at(0).toDouble()*1000;
        m_ch2_plot_range_max = list.at(1).toDouble()*1000;
        qInfo() << "setPlotRangeCh2 " << m_ch2_plot_range_min << " " << m_ch2_plot_range_max;
    }
}

void VmUsbWave::setPlotRangeCh3(QVariantList list)
{
    if(list.size()==2)
    {
        m_ch3_plot_range_min = list.at(0).toDouble()*1000;
        m_ch3_plot_range_max = list.at(1).toDouble()*1000;
        qInfo() << "setPlotRangeCh3 " << m_ch3_plot_range_min << " " << m_ch3_plot_range_max;
    }
}

void VmUsbWave::setPlotRangeCh4(QVariantList list)
{
    if(list.size()==2)
    {
        m_ch4_plot_range_min = list.at(0).toDouble()*1000;
        m_ch4_plot_range_max = list.at(1).toDouble()*1000;
        qInfo() << "setPlotRangeCh4 " << m_ch4_plot_range_min << " " << m_ch4_plot_range_max;
    }
}

void VmUsbWave::DisplayZoomCtrlCh1(bool start)
{
    if (start||((m_ch1_plot_range_min != m_ch1_range_min) || (m_ch1_plot_range_max != m_ch1_range_max)))
    {
        qInfo() << m_ch1_plot_range_min << " " << m_ch1_plot_range_max << " " << m_ch1_range_min << " " << m_ch1_range_max;
        SetOscChannelRangemV(mDevId, 0, m_ch1_plot_range_min, m_ch1_plot_range_max);
        m_ch1_range_min = m_ch1_plot_range_min;
        m_ch1_range_max = m_ch1_plot_range_max;
    }
}

void VmUsbWave::DisplayZoomCtrlCh2(bool start)
{
    if (start||((m_ch2_range_min != m_ch2_plot_range_min) || (m_ch2_range_max != m_ch2_plot_range_max)))
    {
        qInfo() << m_ch2_plot_range_min << " " << m_ch2_plot_range_max << " " << m_ch2_range_min << " " << m_ch2_range_max;
        SetOscChannelRangemV(mDevId, 1, m_ch2_plot_range_min, m_ch2_plot_range_max);
        m_ch2_range_min = m_ch2_plot_range_min;
        m_ch2_range_max = m_ch2_plot_range_max;
    }
}

void VmUsbWave::DisplayZoomCtrlCh3(bool start)
{
    if (start||((m_ch3_range_min != m_ch3_plot_range_min) || (m_ch3_range_max != m_ch3_plot_range_max)))
    {
        qInfo() << m_ch3_plot_range_min << " " << m_ch3_plot_range_max << " " << m_ch3_range_min << " " << m_ch3_range_max;
        SetOscChannelRangemV(mDevId, 2, m_ch3_plot_range_min, m_ch3_plot_range_max);
        m_ch3_range_min = m_ch3_plot_range_min;
        m_ch3_range_max = m_ch3_plot_range_max;
    }
}

void VmUsbWave::DisplayZoomCtrlCh4(bool start)
{
    if (start||((m_ch4_range_min != m_ch4_plot_range_min) || (m_ch4_range_max != m_ch4_plot_range_max)))
    {
        qInfo() << m_ch4_plot_range_min << " " << m_ch4_plot_range_max << " " << m_ch4_range_min << " " << m_ch4_range_max;
        SetOscChannelRangemV(mDevId, 3, m_ch4_plot_range_min, m_ch4_plot_range_max);
        m_ch4_range_min = m_ch4_plot_range_min;
        m_ch4_range_max = m_ch4_plot_range_max;
    }
}

void VmUsbWave::setAcEnableCh1(bool en)
{
    if(acEnableCh1!=en)
    {
        acEnableCh1 = en;
        if(workMode==OscRealTime)
            SetAcDc(mDevId, 0, en);
        else
            SetStreamAcDc(mDevId, 0, en);
        emit acEnableCh1Changed();
    }
}

void VmUsbWave::setAcEnableCh2(bool en)
{
    if(acEnableCh2!=en)
    {
        acEnableCh2 = en;
        if(workMode==OscRealTime)
            SetAcDc(mDevId, 1, en);
        else
            SetStreamAcDc(mDevId, 1, en);
        emit acEnableCh2Changed();
    }
}

void VmUsbWave::setAcEnableCh3(bool en)
{
    if(acEnableCh3!=en)
    {
        acEnableCh3 = en;
        if(workMode==OscRealTime)
            SetAcDc(mDevId, 2, en);
        else
            SetStreamAcDc(mDevId, 2, en);
        emit acEnableCh3Changed();
    }
}

void VmUsbWave::setAcEnableCh4(bool en)
{
    if(acEnableCh4!=en)
    {
        acEnableCh4 = en;
        if(workMode==OscRealTime)
            SetAcDc(mDevId, 3, en);
        else
            SetStreamAcDc(mDevId, 3, en);
        emit acEnableCh4Changed();
    }
}

void VmUsbWave::setTriggerMode(int mode)
{
    if(triggerMode!=mode)
    {
        triggerMode = (TriggerMode)mode;
        SetTriggerMode(mDevId, mode);
        emit triggerModeChanged();
    }
}

static unsigned int triggerStyleIndex[]={0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100};

void VmUsbWave::setTriggerStyle(int style)
{
    if(triggerStyle!=style)
    {
        triggerStyle = (TriggerStyle)style;
        SetTriggerStyle(mDevId, triggerStyleIndex[style]);
        emit triggerStyleChanged();
    }
}

int VmUsbWave::triggerSourceIndex2Code(int index)
{
    QString srcname = triggerSourceList.at(index);
    int code = 0;
    if(srcname==ch1Name)
        code = 0;
    else if(srcname==ch2Name)
        code = 1;
    else if(srcname==ch3Name)
        code = 2;
    else if(srcname==ch4Name)
        code = 3;
    else if(srcname==dio0Name)
        code = 16;
    else if(srcname==dio1Name)
        code = 17;
    else if(srcname==dio2Name)
        code = 18;
    else if(srcname==dio3Name)
        code = 19;
    else if(srcname==dio4Name)
        code = 20;
    else if(srcname==dio5Name)
        code = 21;
    else if(srcname==dio6Name)
        code = 22;
    else if(srcname==dio7Name)
        code = 23;
    else if(srcname==dio8Name)
        code = 24;
    else if(srcname==dio9Name)
        code = 25;
    else if(srcname==dio10Name)
        code = 26;
    else if(srcname==dio11Name)
        code = 27;
    else if(srcname==dio12Name)
        code = 28;
    else if(srcname==dio13Name)
        code = 29;
    else if(srcname==dio14Name)
        code = 30;
    else if(srcname==dio15Name)
        code = 31;
    return code;
}

void VmUsbWave::setTriggerSourceIndex(int index)
{
    if(triggerSourceIndex!=index)
    {
        triggerSourceIndex = index;
        emit triggerSourceIndexChanged();

        int code = triggerSourceIndex2Code(triggerSourceIndex);

        SetTriggerSource(mDevId, code);

        //对应IO和逻辑分析仪复用的设备，需要将IO打开并设置为输入
        if ((code>=16)&&(code<=31)&&IsSupportIODevice(mDevId))
        {
            IOEnable(mDevId, code - 16, 1);
            SetIOInOut(mDevId, code - 16, 0);
        }
    }
}

void VmUsbWave::setPreTriggerPercent(int percent)
{
    if(preTriggerPercent!=percent)
    {
        preTriggerPercent = percent;
        SetPreTriggerPercent(mDevId, preTriggerPercent);
        emit preTriggerPercentChanged();
    }
}

void VmUsbWave::setTriggerLevelSense(int levelmV, double sense, double yIntervalmV)
{
    if((triggerLevelmV!=levelmV)||(triggerSense!=sense))
    {
        triggerLevelmV = levelmV;
        triggerSense = sense;
        SetTriggerLevelmV(mDevId, triggerLevelmV, sense*yIntervalmV);
    }
}


void VmUsbWave::setSampleIndex(int index)
{
    if(sampleIndex!=index)
    {
        sampleIndex = index;
        SetOscSampleRate(mDevId, samples[sampleIndex]);
        qDebug()<<"setSampleIndex " << samples[sampleIndex];

        emit sampleIndexChanged();
    }
}

void CALLBACK VmUsbWave::UsbDevAddCallBack(void* ppara, unsigned int dev_id)
{
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;

    //只使用第一个回调的设备，其他设备关闭
    if(vmusbwave->mDevId==(unsigned int)-1)
    {
        vmusbwave->mDevId = dev_id;
        emit vmusbwave->devRedayChanged();

        vmusbwave->mOscChannelNum = GetOscChannelNum(dev_id);
        emit vmusbwave->oscChannelNumChanged();
        vmusbwave->mDdsChannelNum = GetDDSChannelNum(dev_id);
        emit vmusbwave->ddsChannelNumChanged();
        vmusbwave->mDIOChannelNum = GetSupportIoNumber(dev_id);
        emit vmusbwave->dIOChannelNumChanged();

        vmusbwave->mOscCheckCh1 = true;
        vmusbwave->mOscCheckCh2 = vmusbwave->mOscChannelNum>1? true:false;
        vmusbwave->mOscCheckCh3 = vmusbwave->mOscChannelNum>2? true:false;
        vmusbwave->mOscCheckCh4 = vmusbwave->mOscChannelNum>3? true:false;
        emit vmusbwave->oscCheckCh1Changed();
        emit vmusbwave->oscCheckCh2Changed();
        emit vmusbwave->oscCheckCh3Changed();
        emit vmusbwave->oscCheckCh4Changed();

        //初始化为实时模式
        vmusbwave->setWorkMode(OscRealTime);
        vmusbwave->InitOscRealTimeMode();

        //
        vmusbwave->initIOs();

        //
        vmusbwave->powerSupport = IsSupportPowerSupplyDevice(dev_id);
        emit vmusbwave->powerSupportChanged();

        //
        vmusbwave->meterSupport = IsSupportMultMeterDevice(dev_id);
        emit vmusbwave->meterSupportChanged();

        vmusbwave->meterText = "";
        vmusbwave->meterTextTop = "";
        vmusbwave->meterTextBottom = "";
        emit vmusbwave->meterTextChanged();
    }
    else
    {
        //close the not using device
        CloseDevice(dev_id);
    }

    qInfo("UsbDevAddCallBack\n");
}

void CALLBACK VmUsbWave::UsbDevRemoveCallBack(void* ppara, unsigned int dev_id)
{
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;

    if(vmusbwave->mDevId==dev_id)
    {
        vmusbwave->mDevId = -1;
        emit vmusbwave->devRedayChanged();
    }

    qInfo("UsbDevRemoveCallBack\n");
}

int VmUsbWave::getMaxCaptureLength()
{
    return GetMemoryLength(mDevId);
}

void VmUsbWave::setCaptureLength(int kb)
{
    mCaptureLength = kb*1024;
    qInfo() << "setCaptureLength " <<mCaptureLength;
}

int VmUsbWave::getCaptureLength()
{
    return mCaptureLength/1024;
}

void VmUsbWave::setCaptureEnable(bool en)
{
    if(mCaptureEnable!=en)
    {
        mCaptureEnable=en;

        if(workMode==OscRealTime)
        {
            if(mCaptureEnable)
            {
                DisplayZoomCtrlCh1(true);
                DisplayZoomCtrlCh2(true);
                DisplayZoomCtrlCh3(true);
                DisplayZoomCtrlCh4(true);

                readyNextCapture();
            }
        }
        else
        {
            SetStreamChannelRangemV(mDevId, 0, m_ch1_plot_range_min, m_ch1_plot_range_max);
            SetStreamChannelRangemV(mDevId, 1, m_ch2_plot_range_min, m_ch2_plot_range_max);
            SetStreamChannelRangemV(mDevId, 2, m_ch3_plot_range_min, m_ch3_plot_range_max);
            SetStreamChannelRangemV(mDevId, 3, m_ch4_plot_range_min, m_ch4_plot_range_max);

            //采集通道
            mCaptureChannel = 0;
            if((mOscChannelNum>=1)&&mOscCheckCh1)
                mCaptureChannel |= 0x01;
            if((mOscChannelNum>=2)&&mOscCheckCh2)
                mCaptureChannel |= 0x02;
            if((mOscChannelNum>=3)&&mOscCheckCh3)
                mCaptureChannel |= 0x04;
            if((mOscChannelNum>=4)&&mOscCheckCh4)
                mCaptureChannel |= 0x08;

            StreamCapture(mDevId, 1, (int64_t)mCaptureLength/1000ll, mCaptureChannel, samples[sampleIndex]);
        }
        emit captureEnableChanged();
    }
    //qDebug()<<"captureEnable " << en;
}

void VmUsbWave::readyNextCapture()
{
    if (mCaptureEnable)
    {
        //每次采集前，更新采集范围
        DisplayZoomCtrlCh1();
        DisplayZoomCtrlCh2();
        DisplayZoomCtrlCh3();
        DisplayZoomCtrlCh4();

        //采集通道
        mCaptureChannel = 0;
        if((mOscChannelNum>=1)&&mOscCheckCh1)
            mCaptureChannel |= 0x01;
        if((mOscChannelNum>=2)&&mOscCheckCh2)
            mCaptureChannel |= 0x02;
        if((mOscChannelNum>=3)&&mOscCheckCh3)
            mCaptureChannel |= 0x04;
        if((mOscChannelNum>=4)&&mOscCheckCh4)
            mCaptureChannel |= 0x08;

        mRealCaptureLength = Capture(mDevId, mCaptureLength/1024, mCaptureChannel, 1);
        mRealCaptureLength *= 1024;  //转换成长度
    }
}

void CALLBACK VmUsbWave::UsbDataReadyCallBack(void* ppara, unsigned int dev_id)
{
    //qDebug()<<"UsbDataReadyCallBack";
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;
    if(vmusbwave->mDevId==dev_id)
    {
        if(!vmusbwave->m_work_thread_quit)
        {
            unique_lock lock(vmusbwave->m_work_thread_mutex);
            vmusbwave->m_work_thread_condition_variable.notify_one();
        }
    }
}


void CALLBACK VmUsbWave::StreamRawDataReadyCallBack(void* ppara, unsigned int dev_id, unsigned char* buffer, unsigned int buffer_size,
                                                    unsigned int failed, unsigned int success, unsigned long long int need_total_sample, unsigned long long int total_sample, unsigned long long int menoryuse)
{
    VmUsbWave* pthis = (VmUsbWave*)ppara;
    if(pthis->mDevId == dev_id)
    {
        qDebug() << "StreamDataReadyCallBack " << " dev_id " << dev_id << " buffer_size " << buffer_size << " failed " << failed <<
            " success " << success << " need_total_sample " << need_total_sample <<" total_sample " << total_sample <<" menoryuse " << menoryuse;

        //为了提升stream的数据读取速度，使用SDK的线程来操作
        if(!failed)
        {
            double zoomch1 = pthis->mCaptureChannel&0x01? ReadStreamADCToVoltageZoom(dev_id, 0) : 1.0;
            double biasch1 = pthis->mCaptureChannel&0x01? ReadStreamADCToVoltageBias(dev_id, 0) : 0.0;

            double zoomch2 = pthis->mCaptureChannel&0x02? ReadStreamADCToVoltageZoom(dev_id, 1) : 1.0;
            double biasch2 = pthis->mCaptureChannel&0x02? ReadStreamADCToVoltageBias(dev_id, 1) : 0.0;

            double zoomch3 = pthis->mCaptureChannel&0x04? ReadStreamADCToVoltageZoom(dev_id, 2) : 1.0;
            double biasch3 = pthis->mCaptureChannel&0x04? ReadStreamADCToVoltageBias(dev_id, 2) : 0.0;

            double zoomch4 = pthis->mCaptureChannel&0x08? ReadStreamADCToVoltageZoom(dev_id, 3) : 1.0;
            double biasch4 = pthis->mCaptureChannel&0x08? ReadStreamADCToVoltageBias(dev_id, 3) : 0.0;

            if(!pthis->streamFile.isRecording())
            {
                pthis->streamFile.startRecord(pthis->samples[pthis->sampleIndex], pthis->mCaptureChannel, GetOscAdcBit(pthis->mDevId),
                                              zoomch1, biasch1, zoomch2, biasch2, zoomch3, biasch3, zoomch4, biasch4);
                pthis->plotRecord.startStreamBuffer(pthis->streamFile.getFileName(), buffer_size, pthis->samples[pthis->sampleIndex],
                                                    pthis->mCaptureChannel, GetOscAdcBit(pthis->mDevId), pthis->mCaptureLength*1000000000.0);
            }
            pthis->streamFile.addWaves((char*)buffer, buffer_size);

            pthis->plotRecord.updateStreamBuffer(buffer, buffer_size, pthis->mCaptureChannel, GetOscAdcBit(pthis->mDevId),
                                                 zoomch1, biasch1, zoomch2, biasch2, zoomch3, biasch3, zoomch4, biasch4);
        }

        if(failed||success)
            pthis->streamFile.stopRecord();

        pthis->mStreamFailed = failed;
        pthis->mStreamSuccess = success;

        //通知刷新屏幕
        if(!pthis->m_work_thread_quit)
        {
            unique_lock lock(pthis->m_work_thread_mutex);
            pthis->m_work_thread_condition_variable.notify_one();
        }
    }
}

void VmUsbWave::notify_work_thread_exit()
{
    qDebug() <<"UsbDevMng::~NotifyDevThreadExit...";
    unique_lock lock(m_work_thread_mutex);
    m_work_thread_condition_variable.notify_one();
    qDebug() << "... UsbDevMng::~NotifyDevThreadExit";
}

void VmUsbWave::m_work_thread_fun(void* parg)
{
    VmUsbWave* pthis = (VmUsbWave*)parg;
    pthis->m_work_thread();
}

//工作线程，为了dll回调可以尽快返回，太多的占用时间，可能导致看门狗复位
int VmUsbWave::m_work_thread()
{
    qDebug() << "m_work_thread ...";
    m_work_thread_quit_success = false;
    while (true)
    {
        if (m_work_thread_quit)
            break;

        unique_lock lock(m_work_thread_mutex);
        m_work_thread_condition_variable.wait(lock);

        if (m_work_thread_quit)
            break;

        if(workMode==OscRealTime)
            DataUpdateMsg();
        else
            StreamDataUpdateMsg();
    }
    m_work_thread_quit_success = true;
    qDebug() << "... m_work_thread";
    return 0;
}

void VmUsbWave::DataUpdateMsg()
{
    plotRecord.updateOscRealTimeDatas(mDevId, mRealCaptureLength, samples[sampleIndex]);

    if(fftCal!=nullptr)
        fftCal->updateVolDatas(plotRecord.getVolDatas(fftCal->getSource()), plotRecord.getVolDataLength(fftCal->getSource()), samples[sampleIndex]);

    if(filterCal!=nullptr)
    {
        if(filterCal->updateVolDatas(plotRecord.getVolDatas(fftCal->getSource()), plotRecord.getTriggerPoint(), plotRecord.getVolDataLength(fftCal->getSource())))
            plotRecord.updateFilterDatas(filterCal->getFilterBuffer(), filterCal->getFilterTriggerPoint(), filterCal->getFilterLength());
    }

    emit dataUpdated();
}

bool VmUsbWave::StreamDataUpdateMsg()
{
    qDebug()<<"StreamDataUpdateMsg ....";
    bool result = false;

    if(mStreamFailed||mStreamSuccess)
    {
        mCaptureEnable = false;
        emit captureEnableChanged();
    }

    emit dataUpdated();
    qDebug()<<".... StreamDataUpdateMsg";
    return result;
}

//-----------------------------------------IO----------------------------------------------
//例子只演示了8个IO的操作
void VmUsbWave::initIOs()
{
    if(mDevId != (unsigned int)-1)
    {
        iOList.clear();

        for(int i=0; i<mDIOChannelNum; i++)
        {
            iOInfos[i].iOEnable = false;
            IOEnable(mDevId, i, 0);

            iOInfos[i].iOInOut = 0;
            SetIOInOut(mDevId, i, 0);

            iOInfos[i].iOOutState = IOOutput0;
            SetIOOutState(mDevId, i, iOInfos[i].iOOutState);

            iOInfos[i].iOFreq = 1000;
            iOInfos[i].iODuty = 50;
            SetIOPulseData(mDevId, i, iOInfos[i].iOFreq, iOInfos[i].iODuty);

            emit ioStateChanged(i);

            iOList.push_back(QString("IO%1").arg(i));
        }

        emit iOListChanged();
    }
}

void VmUsbWave::setIOEnable(int index, bool enable)
{
    iOInfos[index].iOEnable = enable;
    if(mDevId != (unsigned int)-1)
    {
        IOEnable(mDevId, index, enable);

        SetIOInOut(mDevId, index, iOInfos[index].iOInOut? 1:0);
        SetIOOutState(mDevId, index, iOInfos[index].iOOutState);
        SetIOPulseData(mDevId, index, iOInfos[index].iOFreq, iOInfos[index].iODuty);
    }
}

void VmUsbWave::setIOInOut(int index, int direction)
{
    iOInfos[index].iOInOut = direction;
    if(mDevId != (unsigned int)-1)
        SetIOInOut(mDevId, index, direction);
}

void VmUsbWave::setIOOutState(int index, int state)
{
    iOInfos[index].iOOutState = (IOOutput)state;
    if(mDevId != (unsigned int)-1)
        SetIOOutState(mDevId, index, state);
}

void VmUsbWave::setIOFreq(int index, double freq)
{
    iOInfos[index].iOFreq = freq;
    if(mDevId != (unsigned int)-1)
        SetIOPulseData(mDevId, index, iOInfos[index].iOFreq, iOInfos[index].iODuty);
}

void VmUsbWave::setIODuty(int index, double duty)
{
    iOInfos[index].iODuty = duty;
    if(mDevId != (unsigned int)-1)
        SetIOPulseData(mDevId, index, iOInfos[index].iOFreq, iOInfos[index].iODuty);
}

void VmUsbWave::iOPulseSyn()
{
    if(mDevId != (unsigned int)-1)
        SetIOPulseSyn(mDevId);
}

void VmUsbWave::IOReadStateCallBack(void* ppara, unsigned int dev_id, unsigned int state)
{
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;
    if(vmusbwave->mDevId==dev_id)
    {
        for(int i=0; i<16; i++)
        {
            unsigned int mask =0x0001;
            bool high = (state&(mask<<i));

            if(vmusbwave->iOInfos[i].iOInIsHigh != high)
            {
                vmusbwave->iOInfos[i].iOInIsHigh = high;
                emit vmusbwave->ioInStateChanged(i);
            }
        }
    }
}

//------------------------------------------DDS--------------------------------------------
int VmUsbWave::getDdsMode(int index)
{
    return ddsInfo[index].mode;
}

void VmUsbWave::setDdsMode(int index, int mode)
{
    ddsInfo[index].mode = (DdsMode)mode;
    if(mDevId != (unsigned int)-1)
    {
        //如果已经打开，关闭，重新初始化
        if(ddsInfo[index].enable)
        {
            DDSOutputEnable(mDevId, index, false);
            setDdsOutputEnable(index, ddsInfo[index].enable);
        }
    }
}

static unsigned short WAVE[] = {/*BX_SINE*/ 0x0001,           //Sine
                                /*BX_SQUARE*/ 0x0002,         //Square
                                /*BX_RAMP*/ 0x0004,			//Ramp
                                /*BX_PULSE*/ 0x0008,    //Pulse
                                /*BX_NOISE*/ 0x0010,  //Noise
                                /*BX_DC*/ 0x0020,  //Dc
                                /*BX_ARB*/ 0x0040};  //Arb}

int VmUsbWave::getDdsWaveIndex(int index)
{
    return ddsInfo[index].waveIndex;
}

void VmUsbWave::setDdsWaveIndex(int index, int wave_index)
{
    ddsInfo[index].waveIndex = wave_index;
    if(mDevId != (unsigned int)-1)
    {
        if(WAVE[ddsInfo[index].waveIndex]==DdsWaveArb)
        {
            if(GetDDSDepth(mDevId)==4096)
                UpdateDDSArbBuffer(mDevId, index, lanczos4096, 4096);
            else if(GetDDSDepth(mDevId)==8192)
                UpdateDDSArbBuffer(mDevId, index, lanczos8192, 8192);
        }
        SetDDSBoxingStyle(mDevId, index, WAVE[ddsInfo[index].waveIndex]);
    }
}

double VmUsbWave::getDdsDuty(int index)
{
    return ddsInfo[index].duty;
}

void VmUsbWave::setDdsDuty(int index, double duty)
{
    ddsInfo[index].duty = duty;
    if(mDevId != (unsigned int)-1)
        SetDDSDutyCycle(mDevId, index, ddsInfo[index].duty);
}

double VmUsbWave::getDdsStartPhase(int index)
{
    return ddsInfo[index].startPhase;
}

void VmUsbWave::setDdsStartPhase(int index, double p)
{
    ddsInfo[index].startPhase = p;
    if(mDevId != (unsigned int)-1)
        SetDDSPhase(mDevId, index, ddsInfo[index].startPhase);
}

double VmUsbWave::getDdsFreq(int index)
{
    return ddsInfo[index].freqHz;
}

void VmUsbWave::setDdsFreq(int index, double freq)
{
    ddsInfo[index].freqHz = freq;
    if(mDevId != (unsigned int)-1)
        SetDDSFreq(mDevId, index, ddsInfo[index].freqHz);
}

double VmUsbWave::getDdsStartFreq(int index)
{
    return ddsInfo[index].startFreqHz;
}

void VmUsbWave::setDdsStartFreq(int index, double freq)
{
    ddsInfo[index].startFreqHz = freq;
    if(mDevId != (unsigned int)-1)
        SetDDSSweepStartFreq(mDevId, index, ddsInfo[index].startFreqHz);
}

double VmUsbWave::getDdsStopFreq(int index)
{
    return ddsInfo[index].stopFreqHz;
}

void VmUsbWave::setDdsStopFreq(int index, double freq)
{
    ddsInfo[index].stopFreqHz = freq;
    if(mDevId != (unsigned int)-1)
        SetDDSSweepStopFreq(mDevId, index, ddsInfo[index].stopFreqHz);
}

int VmUsbWave::getDdsSweepTimeUs(int index)
{
    return ddsInfo[index].sweepTimeUs;
}

void VmUsbWave::setDdsSweepTimeUs(int index, int us)
{
    ddsInfo[index].sweepTimeUs = us;
    if(mDevId != (unsigned int)-1)
        SetDDSSweepTime(mDevId, index, ddsInfo[index].sweepTimeUs*1000ll);
}

int VmUsbWave::getDdsAmplitudeMv(int index)
{
    return ddsInfo[index].vppmV;
}

void VmUsbWave::setDdsAmplitudeMv(int index, int ampl)
{
    ddsInfo[index].vppmV = ampl;
    if(mDevId != (unsigned int)-1)
        SetDDSAmplitudeMv(mDevId, index, ddsInfo[index].vppmV);
}

int VmUsbWave::getDdsBiasMv(int index)
{
    return ddsInfo[index].biasmV;
}

void VmUsbWave::setDdsBiasMv(int index, int bias)
{
    ddsInfo[index].biasmV = bias;
    if(mDevId != (unsigned int)-1)
        SetDDSBiasMv(mDevId, index, ddsInfo[index].biasmV);
}

int VmUsbWave::getBurstStyle(int index)
{
    return ddsInfo[index].burstStyle;
}

void VmUsbWave::setBurstStyle(int index, int style)
{
    ddsInfo[index].burstStyle = style;
    if(mDevId != (unsigned int)-1)
        SetDDSBurstStyle(mDevId, index, ddsInfo[index].burstStyle);
}

int VmUsbWave::getLoopsNum(int index)
{
    return ddsInfo[index].loopsNum;
}

void VmUsbWave::setLoopsNum(int index, int num)
{
    ddsInfo[index].loopsNum = num;
    if(mDevId != (unsigned int)-1)
        SetDDSLoopsNum(mDevId, index, ddsInfo[index].loopsNum);
}

bool VmUsbWave::getLoopsNumInfinity(int index)
{
    return ddsInfo[index].loopsNumInfinity;
}

void VmUsbWave::setLoopsNumInfinity(int index, bool em)
{
    ddsInfo[index].loopsNumInfinity = em;
    if(mDevId != (unsigned int)-1)
        SetDDSLoopsNumInfinity(mDevId, index, ddsInfo[index].loopsNumInfinity);
}

qint64 VmUsbWave::getBurstPeriod(int index)
{
    return ddsInfo[index].burstPeriod;
}

void VmUsbWave::setBurstPeriod(int index, qint64 num)
{
    ddsInfo[index].burstPeriod = num;
    if(mDevId != (unsigned int)-1)
        SetDDSBurstPeriodNs(mDevId, index, ddsInfo[index].burstPeriod);
}

qint64 VmUsbWave::getBurstDelay(int index)
{
    return ddsInfo[index].burstDelay;
}

void VmUsbWave::setBurstDelay(int index, qint64 num)
{
    ddsInfo[index].burstDelay = num;
    if(mDevId != (unsigned int)-1)
        SetDDSBurstDelayNs(mDevId, index, ddsInfo[index].burstDelay);
}

void VmUsbWave::ddsIOCtrl(int index)
{
    if((ddsInfo[index].mode == DdsModeSweep)||(ddsInfo[index].mode == DdsModeBurst))
    {
        if((ddsInfo[index].triggerSource==DdsTriggerSourceInternal)||(ddsInfo[index].triggerSource==DdsTriggerSourceManual))
        {
            if(ddsInfo[index].output!=DdsOutputClose)
            {
                int ioIndex = ddsInfo[index].ioIndex;

                SetDDSOutputGateEnge(mDevId, index, ddsInfo[index].output);

                SetIOInOut(mDevId, ioIndex, 1);
                SetIOOutState(mDevId, ioIndex, IOOutputDDSGate);
                IOEnable(mDevId, ioIndex, true);
                emit ioStateChanged(ioIndex);
            }
        }
        else //DdsTriggerSourceExternal
        {
            int ioIndex = ddsInfo[index].ioIndex;

            SetDDSTriggerSourceIo(mDevId, index, ioIndex);
            SetDDSTriggerSourceEnge(mDevId, index, ddsInfo[index].inEdge);

            SetIOInOut(mDevId, ioIndex, 0);
            IOEnable(mDevId, ioIndex, true);
            emit ioStateChanged(ioIndex);
        }
    }
}

int VmUsbWave::getDdsTriggerSource(int index)
{
    return ddsInfo[index].triggerSource;
}

void VmUsbWave::setDdsTriggerSource(int index, int source)
{
    ddsInfo[index].triggerSource = (DdsTriggerSource)source;
    if(mDevId != (unsigned int)-1)
        ddsIOCtrl(index);
}

int VmUsbWave::getDdsIoIndex(int index)
{
    return ddsInfo[index].ioIndex;
}

void VmUsbWave::setDdsIoIndex(int index, int ioindex)
{
    ddsInfo[index].ioIndex = ioindex;
    if(mDevId != (unsigned int)-1)
    {
        ddsIOCtrl(index);
    }
}

int VmUsbWave::getDdsOutput(int index)
{
    return ddsInfo[index].output;
}

void VmUsbWave::setDdsOutput(int index, int out)
{
    ddsInfo[index].output = (DdsOutput)out;
    if(mDevId != (unsigned int)-1)
    {
        if(ddsInfo[index].output != DdsOutputClose)
            ddsIOCtrl(index);
        else
            SetDDSOutputGateEnge(mDevId, index, ddsInfo[index].output);
    }
}

int VmUsbWave::getDdsInputEdge(int index)
{
    return ddsInfo[index].inEdge;
}

void VmUsbWave::setDdsInputEdge(int index, int edge)
{
    ddsInfo[index].inEdge = (DdsTriggerSourceEdge)edge;
    if(mDevId != (unsigned int)-1)
    {
        SetDDSTriggerSourceEnge(mDevId, index, ddsInfo[index].inEdge);
    }
}

bool VmUsbWave::getDdsOutputEnable(int index)
{
    return ddsInfo[index].enable;
}

void VmUsbWave::setDdsOutputEnable(int index, bool en)
{
    ddsInfo[index].enable = en;
    if(mDevId != (unsigned int)-1)
    {
        if(ddsInfo[index].enable)
        {
            SetDDSOutMode(mDevId, index, ddsInfo[index].mode);

            if(WAVE[ddsInfo[index].waveIndex]==DdsWaveArb)
            {
                if(GetDDSDepth(mDevId)==4096)
                    UpdateDDSArbBuffer(mDevId, index, lanczos4096, 4096);
                else if(GetDDSDepth(mDevId)==8192)
                    UpdateDDSArbBuffer(mDevId, index, lanczos8192, 8192);
            }
            SetDDSBoxingStyle(mDevId, index, WAVE[ddsInfo[index].waveIndex]);
            SetDDSDutyCycle(mDevId, index, ddsInfo[index].duty);
            SetDDSAmplitudeMv(mDevId, index, ddsInfo[index].vppmV);
            SetDDSBiasMv(mDevId, index, ddsInfo[index].biasmV);
            SetDDSPhase(mDevId, index, ddsInfo[index].startPhase);

            if(ddsInfo[index].mode==DdsModeContinuous)
            {
                SetDDSFreq(mDevId, index, ddsInfo[index].freqHz);
            }
            else if(ddsInfo[index].mode==DdsModeSweep)
            {
                SetDDSSweepStartFreq(mDevId, index, ddsInfo[index].startFreqHz);
                SetDDSSweepStopFreq(mDevId, index, ddsInfo[index].stopFreqHz);
                SetDDSSweepTime(mDevId, index, ddsInfo[index].sweepTimeUs*1000ll);
            }
            else
            {
                SetDDSFreq(mDevId, index, ddsInfo[index].freqHz);
                SetDDSBurstStyle(mDevId, index, ddsInfo[index].burstStyle);
                SetDDSLoopsNum(mDevId, index, ddsInfo[index].loopsNum);
                SetDDSLoopsNumInfinity(mDevId, index, ddsInfo[index].loopsNumInfinity);
                SetDDSBurstPeriodNs(mDevId, index, ddsInfo[index].burstPeriod);
                SetDDSBurstDelayNs(mDevId, index, ddsInfo[index].burstDelay);
            }

            ddsIOCtrl(index);
        }
        DDSOutputEnable(mDevId, index, ddsInfo[index].enable);
    }
}

void VmUsbWave::setDdsMultSyn()
{
    if(mDevId != (unsigned int)-1)
        DDSMultSyn(mDevId);
}

void VmUsbWave::setDdsManualTrigger(int index)
{
    if(mDevId != (unsigned int)-1)
        DDSManualTrigger(mDevId, index);
}

//---------------------------------------------------Power Supply-----------------------------------------------------
//通道0--正电压
void VmUsbWave::setPosPowerEn(bool en)
{
    if(mDevId != (unsigned int)-1)
        PowerSupplyEnable(mDevId, 0, en);
}

void VmUsbWave::setPosPowerV(double v)
{
    if(mDevId != (unsigned int)-1)
        SetPowerSupplyVoltage(mDevId, 0, v*1000);
}

//通道1--负电压
void VmUsbWave::setNegPowerEn(bool en)
{
    if(mDevId != (unsigned int)-1)
        PowerSupplyEnable(mDevId, 1, en);
}

void VmUsbWave::setNegPowerV(double v)
{
    if(mDevId != (unsigned int)-1)
        SetPowerSupplyVoltage(mDevId, 1, v*1000);
}


//----------------------------------------------------------------------------------------------
int CALLBACK VmUsbWave::MultMeterUpdateCallBack(void* ppara, unsigned int dev_id, const char* text, const char* top, const char* bottom)
{
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;

    if(vmusbwave->mDevId==dev_id)
    {
        vmusbwave->meterText = QString::fromUtf8(text);
        vmusbwave->meterTextTop = QString::fromUtf8(top);
        if((uint8_t)bottom[0]==0xB8)
        {
            vmusbwave->meterTextBottom = "Ω";
        }
        else if((uint8_t)bottom[1]==0xB8) //欧姆
        {
            if(bottom[0]==u8'u')
                vmusbwave->meterTextBottom = "uΩ";
            else if(bottom[0]==u8'm')
                vmusbwave->meterTextBottom = "mΩ";
            else if((uint8_t)bottom[0]==0xA6)
                vmusbwave->meterTextBottom = "kΩ";
        }
        else
            vmusbwave->meterTextBottom = QString::fromUtf8(bottom);

        emit vmusbwave->meterTextChanged();
    }

    //qDebug() << "MultMeterUpdateCallBack " << vmusbwave->meterText << vmusbwave->meterTextTop << vmusbwave->meterTextBottom;

    return 1;
}

void VmUsbWave::setMeterEn(bool en)
{
    if(mDevId != (unsigned int)-1)
        MultMeterEnable(mDevId,0,en);
}

void VmUsbWave::setMeterMode(unsigned int mode)
{
    if(mDevId != (unsigned int)-1)
        SetMultMeterMode(mDevId,0,mode,1);
}



