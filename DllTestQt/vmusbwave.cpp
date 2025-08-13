#include "vmusbwave.h"
#include <QDebug>
#include <thread>

#define EN_LOG 1
#define EN_WATCH_DOG 1

VmUsbWave::VmUsbWave(QObject *parent)
    : QObject{parent}
    , m_devState(0)
    , m_captureEnable(false)
    , m_captureLength(0)
    , m_real_length(0)
    , m_sample(0)
    //, buffer_ch1(NULL)
    //, buffer_ch2(NULL)
    //, buffer_length(0)
    , m_ch1_range_min(0)
    , m_ch1_range_max(0)
    , m_ch2_range_min(0)
    , m_ch2_range_max(0)
    , m_ch1_plot_range_min(0)
    , m_ch1_plot_range_max(0)
    , m_ch2_plot_range_min(0)
    , m_ch2_plot_range_max(0)
{
     InitDll(EN_LOG, EN_WATCH_DOG);
     SetDevNoticeCallBack(this, UsbDevAddCallBack, UsbDevRemoveCallBack);
     SetDataReadyCallBack(this, UsbDataReadyCallBack);
     SetIOReadStateCallBack(this, IOReadStateCallBack);
}

VmUsbWave::~VmUsbWave()
{
    //if(buffer_ch1!=NULL)
    //    delete [] buffer_ch1;
    //if(buffer_ch2!=NULL)
    //    delete [] buffer_ch2;
    FinishDll();
}

void CALLBACK VmUsbWave::UsbDevAddCallBack(void* ppara)
{
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;

    vmusbwave->m_captureLength = GetMemoryLength();
    //为了刷新速度，demo使用最大1M，可以根据需求修改
    //For the refresh speed, the demo uses a maximum of 4M, which can be modified according to needs
    if(vmusbwave->m_captureLength>1024*4)
        vmusbwave->m_captureLength = 1024*4;

    vmusbwave->m_sample = GetOscSampleRate();
    vmusbwave->setdevState(1);
    qInfo("UsbDevAddCallBack\n");
}

void CALLBACK VmUsbWave::UsbDevRemoveCallBack(void* ppara)
{
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;
    vmusbwave->setdevState(-1);
    qInfo("UsbDevRemoveCallBack\n");
}

void VmUsbWave::setdevState(int devstate)
{
    m_devState = devstate;
    emit devStateChanged(devstate);
}

void CALLBACK VmUsbWave::UsbDataReadyCallBack(void* ppara)
{
    //qDebug()<<"UsbDataReadyCallBack";
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;
    vmusbwave->UsbDataProcess();
}

 void VmUsbWave::UsbDataProcess()
 {
    emit updateDatas(m_real_length, m_sample);
 }

void VmUsbWave::qmlcallcpp_style1()
{
    qDebug()<<"qmlcallcpp_style1";
}

void VmUsbWave::qmlcallcpp_style2(const int8_t chn)
{
    qDebug()<<"qmlcallcpp_style2";
}

void VmUsbWave::scanDevice()
{
    ScanDevice();
}

void VmUsbWave::resetDll()
{
    //销毁Dll
    FinishDll();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //初始化Dll
    InitDll(EN_LOG, EN_WATCH_DOG);
    SetDevNoticeCallBack(this, UsbDevAddCallBack, UsbDevRemoveCallBack);
    SetDataReadyCallBack(this, UsbDataReadyCallBack);
}

void VmUsbWave::resetDevice()
{
    ResetDevice();
}

QStringList VmUsbWave::getsupportsamples()
{
    m_support_samples.clear();
    int num = GetOscSupportSampleRateNum();
    unsigned int* samples = new unsigned int[num];
    num = GetOscSupportSampleRates(samples, num);
    if(num)
    {
        for(int i=0; i<num; i++)
            m_support_samples.push_back(QString::number(samples[i]));
    }
    delete []samples;
    return m_support_samples;
}

void VmUsbWave::setsample(int s)
{
    m_sample = s;
    SetOscSampleRate(m_sample);
    qDebug()<<"setsample " << s;
}

int VmUsbWave::getsample()
{
    return m_sample;
}

int VmUsbWave::getMaxCaptureLength()
{
    return GetMemoryLength();
}

void VmUsbWave::setCaptureLength(int kb)
{
    m_captureLength = kb;
}

int VmUsbWave::getCaptureLength()
{
    return m_captureLength;
}

void VmUsbWave::captureEnable(bool en)
{
    m_captureEnable=en;
    if(m_captureEnable)
    {
        DisplayZoomCtrlCh1(true);
        DisplayZoomCtrlCh2(true);
        nextCapture();
    }
    //qDebug()<<"captureEnable " << en;
}

void VmUsbWave::nextCapture()
{
    if (m_captureEnable)
    {
        DisplayZoomCtrlCh1();
        DisplayZoomCtrlCh2();

        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        m_real_length = Capture(m_captureLength, 0x03, 0);
        m_real_length *= 1024;  //转换成长度
    }
}

void VmUsbWave::setFileName(QString fileName)
{
    mFileName = fileName;
    emit fileNameChanged();
}

bool VmUsbWave::getAcDcCh1()
{
    return GetAcDc(0);
}

void VmUsbWave::setAcDcCh1(bool ac)
{
    SetAcDc(0, ac? 1:0);
}

bool VmUsbWave::getAcDcCh2()
{
    return GetAcDc(1);
}

void VmUsbWave::setAcDcCh2(bool ac)
{
    SetAcDc(1, ac? 1:0);
}

bool VmUsbWave::isSupportHardTrigger()
{
    return IsSupportHardTrigger();
}

int VmUsbWave::getTriggerMode()
{
    return GetTriggerMode();
}

void VmUsbWave::setTriggerMode(int mode)
{
    SetTriggerMode(mode);
}

static unsigned int trigger_index[]={0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100};

int VmUsbWave::getTriggerStyle()
{
    unsigned int style = GetTriggerStyle();

    int i=0;
    for(int k=0; k<sizeof(trigger_index)/sizeof(int); k++)
    {
        if(style==trigger_index[k])
        {
            i=k;
            break;
        }
    }
    return i;
}

void VmUsbWave::setTriggerStyle(int style)
{
    SetTriggerStyle(trigger_index[style]);
}

int VmUsbWave::getTriggerSource()
{
    return GetTriggerSource();
}

void VmUsbWave::setTriggerSource(int sr)
{
    SetTriggerSource(sr);

    //对应IO和逻辑分析仪复用的设备，需要将IO打开并设置为输入
    if ((sr>=16)&&(sr<=23)&&IsSupportIODevice())
    {
        IOEnable(sr - 16, 1);
        SetIOInOut(sr - 16, 0);
    }
}

int VmUsbWave::getTriggerLevelmV()
{
    return GetTriggerLevelmV();
}

void VmUsbWave::setTriggerLevelmV(int level)
{
    if(getTriggerSource()==TRIGGER_SOURCE_CH1)
        SetTriggerLevelmV(level, (m_ch1_plot_range_max-m_ch1_plot_range_min)/10/5);
    else
        SetTriggerLevelmV(level, (m_ch2_plot_range_max-m_ch2_plot_range_min)/10/5);
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

void VmUsbWave::DisplayZoomCtrlCh1(bool start)
{
    if (start||((m_ch1_plot_range_min != m_ch1_range_min) || (m_ch1_plot_range_max != m_ch1_range_max)))
    {
        qInfo() << m_ch1_plot_range_min << " " << m_ch1_plot_range_max << " " << m_ch1_range_min << " " << m_ch1_range_max;
        SetOscChannelRangemV(0, m_ch1_plot_range_min*1000.0, m_ch1_plot_range_max*1000.0);
        m_ch1_range_min = m_ch1_plot_range_min;
        m_ch1_range_max = m_ch1_plot_range_max;
    }
}

void VmUsbWave::DisplayZoomCtrlCh2(bool start)
{
    if (start||((m_ch2_range_min != m_ch2_plot_range_min) || (m_ch2_range_max != m_ch2_plot_range_max)))
    {
        qInfo() << m_ch2_plot_range_min << " " << m_ch2_plot_range_max << " " << m_ch2_range_min << " " << m_ch2_range_max;
        SetOscChannelRangemV(1, m_ch2_plot_range_min*1000.0, m_ch2_plot_range_max*1000.0);
        m_ch2_range_min = m_ch2_plot_range_min;
        m_ch2_range_max = m_ch2_plot_range_max;
    }
}

bool VmUsbWave::isSupportDds()
{
    return IsSupportDDSDevice();
}

static unsigned short WAVE[] = {/*BX_SINE*/ 0x0001,           //Sine
                                /*BX_SQUARE*/ 0x0002,         //Square
                                /*BX_RAMP*/ 0x0004,			//Ramp
                                /*BX_PULSE*/ 0x0008,    //Pulse
                                /*BX_NOISE*/ 0x0010,  //Noise
                                /*BX_DC*/ 0x0020,  //Dc
                                /*BX_ARB*/ 0x0040};  //Arb}\

/*int VmUsbWave::getDdsWaveIndex()
{

}*/

void VmUsbWave::setDdsWaveIndex(int wave_index)
{
    SetDDSBoxingStyle(0, WAVE[wave_index]);
}

void VmUsbWave::setDdsDuty(int duty)
{
    SetDDSDutyCycle(0, duty);
}

void VmUsbWave::setDdsFreq(int freq)
{
    SetDDSPinlv(0, freq);
}

int VmUsbWave::getDdsAmplitudeMv()
{
    return GetDDSAmplitudeMv(0);
}

void VmUsbWave::setDdsAmplitudeMv(int ampl)
{
    SetDDSAmplitudeMv(0, ampl);
}

int VmUsbWave::getDdsBiasMv()
{
    return GetDDSBiasMv(0);
}

void VmUsbWave::setDdsBiasMv(int bias)
{
    SetDDSBiasMv(0, bias);
}

void VmUsbWave::DdsOutputEnable(bool en)
{
    DDSOutputEnable(0, en);
}

void VmUsbWave::IOReadStateCallBack(void* ppara, unsigned int state)
{
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;
    emit vmusbwave->updateIoState(state);
}

bool VmUsbWave::isSupportIo()
{
    return IsSupportIODevice();
}

void VmUsbWave::setIOEnable(unsigned char channel, unsigned char enable)
{
    IOEnable(channel, enable);
}

unsigned char VmUsbWave::getIOEnable(unsigned char channel)
{
    return IsIOEnable(channel);
}

void VmUsbWave::setIOInOut(unsigned char channel, unsigned char inout)
{
    SetIOInOut(channel, inout);
}

unsigned char VmUsbWave::getIOInOut(unsigned char channel)
{
    return GetIOInOut(channel);
}

void VmUsbWave::setIOOutState(unsigned char channel, unsigned char state)
{
    SetIOOutState(channel, state);
}

#include "moc_vmusbwave.cpp"
