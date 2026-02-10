#ifndef VMUSBWAVE_H
#define VMUSBWAVE_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <QVariant>
#include <QObject>
#include <QTimer>
#include <mutex>
#include <condition_variable>
#include "VmMultMsoLib.h"
#include "defines.h"
#include "plotrecord.h"
#include "fftcal.h"
#include "filtercal.h"
#include "wavemng/wavefile.h"

class IOInfo
{
public:
    bool iOEnable = false;
    unsigned char iOInOut = 0;
    bool iOInIsHigh =false;
    IOOutput iOOutState = IOOutput0;
    double iOFreq = 1000;
    double iODuty =50;
};

class DdsInfo
{
public:
    bool enable = false;
    DdsMode mode = DdsModeContinuous;
    int waveIndex = 0;
    double vppmV = 1000;
    double biasmV = 0;
    double startPhase = 0;
    double freqHz = 1000.0;
    double duty = 50;

    double startFreqHz = 1000.0;
    double stopFreqHz = 100000.0;
    int sweepTimeUs = 10000;

    int burstStyle = 0;
    int loopsNum = 1;
    bool loopsNumInfinity = false;
    qint64 burstPeriod = 10000000;
    qint64 burstDelay = 0;

    DdsTriggerSource triggerSource = DdsTriggerSourceInternal;
    int ioIndex = 0;
    DdsOutput output = DdsOutputClose;
    DdsTriggerSourceEdge inEdge = DdsTriggerEdgeRising;
};

class VmUsbWave : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool devReday READ getDevReday NOTIFY devRedayChanged)

    Q_PROPERTY(int oscChannelNum READ getOscChannelNum NOTIFY oscChannelNumChanged)
    Q_PROPERTY(int ddsChannelNum READ getDdsChannelNum NOTIFY ddsChannelNumChanged)
    Q_PROPERTY(int dIOChannelNum READ getDIOChannelNum NOTIFY dIOChannelNumChanged)

    Q_PROPERTY(bool oscCheckCh1 READ getOscCheckCh1 WRITE setOscCheckCh1 NOTIFY oscCheckCh1Changed)
    Q_PROPERTY(bool oscCheckCh2 READ getOscCheckCh2 WRITE setOscCheckCh2 NOTIFY oscCheckCh2Changed)
    Q_PROPERTY(bool oscCheckCh3 READ getOscCheckCh3 WRITE setOscCheckCh3 NOTIFY oscCheckCh3Changed)
    Q_PROPERTY(bool oscCheckCh4 READ getOscCheckCh4 WRITE setOscCheckCh4 NOTIFY oscCheckCh4Changed)

    Q_PROPERTY(int workMode READ getWorkMode WRITE setWorkMode NOTIFY workModeChanged)

    Q_PROPERTY(bool acEnableCh1 READ getAcEnableCh1 WRITE setAcEnableCh1 NOTIFY acEnableCh1Changed)
    Q_PROPERTY(bool acEnableCh2 READ getAcEnableCh2 WRITE setAcEnableCh2 NOTIFY acEnableCh2Changed)
    Q_PROPERTY(bool acEnableCh3 READ getAcEnableCh3 WRITE setAcEnableCh3 NOTIFY acEnableCh3Changed)
    Q_PROPERTY(bool acEnableCh4 READ getAcEnableCh4 WRITE setAcEnableCh4 NOTIFY acEnableCh4Changed)

    Q_PROPERTY(int triggerMode READ getTriggerMode WRITE setTriggerMode NOTIFY triggerModeChanged)
    Q_PROPERTY(int triggerStyle READ getTriggerStyle WRITE setTriggerStyle NOTIFY triggerStyleChanged)
    Q_PROPERTY(QStringList triggerSourceList READ getTriggerSourceList NOTIFY triggerSourceListChanged)
    Q_PROPERTY(int triggerSourceIndex READ getTriggerSourceIndex WRITE setTriggerSourceIndex NOTIFY triggerSourceIndexChanged)
    Q_PROPERTY(int preTriggerPercent READ getPreTriggerPercent WRITE setPreTriggerPercent NOTIFY preTriggerPercentChanged)
    Q_PROPERTY(int triggerLevelmV READ getTriggerLevelmV NOTIFY triggerLevelmVChanged)
    Q_PROPERTY(double triggerSense READ getTriggerSense NOTIFY triggerSenseChanged)

    Q_PROPERTY(QStringList supportSamples READ getSupportSamples NOTIFY supportSamplesChanged)
    Q_PROPERTY(int sampleIndex READ getSampleIndex WRITE setSampleIndex NOTIFY sampleIndexChanged)

    Q_PROPERTY(int captureLength READ getCaptureLength WRITE setCaptureLength NOTIFY captureLengthChanged)
    Q_PROPERTY(bool captureEnable READ getCaptureEnable WRITE setCaptureEnable NOTIFY captureEnableChanged)

    //--------------------------------------IO--------------------------------------------
    Q_PROPERTY(QStringList iOList READ getIOList NOTIFY iOListChanged)

    //--------------------------------------DDS--------------------------------------------

    //-------------------------------------------------------------------------------------
    Q_PROPERTY(bool powerSupport READ getPowerSupport NOTIFY powerSupportChanged)

    //-------------------------------------------------------------------------------------
    Q_PROPERTY(bool meterSupport READ getMeterSupport NOTIFY meterSupportChanged)

    Q_PROPERTY(QString meterText READ getMeterText NOTIFY meterTextChanged)
    Q_PROPERTY(QString meterTextTop READ getMeterTextTop NOTIFY meterTextChanged)
    Q_PROPERTY(QString meterTextBottom READ getMeterTextBottom NOTIFY meterTextChanged)


public:
    explicit VmUsbWave(QObject *parent = nullptr);
    ~VmUsbWave();

    static void CALLBACK UsbDevAddCallBack(void* ppara, unsigned int dev_id);
    static void CALLBACK UsbDevRemoveCallBack(void* ppara, unsigned int dev_id);
    static void CALLBACK UsbDataReadyCallBack(void* ppara, unsigned int dev_id);
    static void CALLBACK IOReadStateCallBack(void* ppara, unsigned int dev_id, unsigned int state);
    static void CALLBACK StreamRawDataReadyCallBack(void* ppara, unsigned int dev_id, unsigned char* buffer, unsigned int buffer_size,
                                                    unsigned int failed, unsigned int success, unsigned long long int need_total_sample, unsigned long long int total_sample, unsigned long long int menoryuse);
    static int CALLBACK MultMeterUpdateCallBack(void* ppara, unsigned int dev_id, const char* text, const char* top, const char* bottom);

    bool getDevReday(){return mDevId!=(unsigned int)-1;};

    int getOscChannelNum() const { return mOscChannelNum; };
    int getDdsChannelNum() const { return mDdsChannelNum; };
    int getDIOChannelNum() const { return mDIOChannelNum; };

    bool getOscCheckCh1() const { return mOscCheckCh1; };
    void setOscCheckCh1(bool check){mOscCheckCh1 = check;};

    bool getOscCheckCh2() const { return mOscCheckCh2; };
    void setOscCheckCh2(bool check){mOscCheckCh2 = check;};

    bool getOscCheckCh3() const { return mOscCheckCh3; };
    void setOscCheckCh3(bool check){mOscCheckCh3 = check;};

    bool getOscCheckCh4() const { return mOscCheckCh4; };
    void setOscCheckCh4(bool check){mOscCheckCh4 = check;};

    //---------------------------------------------OSC------------------------------------------
    int getWorkMode() const { return workMode; };
    void setWorkMode(int mode);

    Q_INVOKABLE void setPlotRangeCh1(QVariantList list);
    Q_INVOKABLE void setPlotRangeCh2(QVariantList list);
    Q_INVOKABLE void setPlotRangeCh3(QVariantList list);
    Q_INVOKABLE void setPlotRangeCh4(QVariantList list);

    bool getAcEnableCh1() const { return acEnableCh1; };
    void setAcEnableCh1(bool en);

    bool getAcEnableCh2() const { return acEnableCh2; };
    void setAcEnableCh2(bool en);

    bool getAcEnableCh3() const { return acEnableCh3; };
    void setAcEnableCh3(bool en);

    bool getAcEnableCh4() const { return acEnableCh4; };
    void setAcEnableCh4(bool en);

    int getTriggerMode(){return triggerMode;}
    void setTriggerMode(int mode);

    int getTriggerStyle(){return triggerStyle;}
    void setTriggerStyle(int style);

    QStringList getTriggerSourceList(){return triggerSourceList;}
    int getTriggerSourceIndex(){return triggerSourceIndex;}
    void setTriggerSourceIndex(int style);

    int getPreTriggerPercent(){return preTriggerPercent;}
    void setPreTriggerPercent(int percent);

    int getTriggerLevelmV(){return triggerLevelmV;};

    double getTriggerSense(){return triggerSense;};

    Q_INVOKABLE void setTriggerLevelSense(int level, double sense, double yInterval);

    QStringList getSupportSamples(){return mSupportSamples;};

    int getSampleIndex(){return sampleIndex;};
    void setSampleIndex(int index);
    Q_INVOKABLE unsigned int getSample()
    {
        unsigned int s = 0;
        if(samples != nullptr)
            s = samples[sampleIndex];
        return s;
    };

    void setCaptureLength(int kb);
    int getCaptureLength();

    Q_INVOKABLE void readyNextCapture();

    PlotRecord* getPlotRecord(){return &plotRecord;};

    //-------------------------------------------IO--------------------------------------------
    // 通用IO属性访问（索引模式）
    Q_INVOKABLE bool getIOEnable(int index) const {return iOInfos[index].iOEnable;};
    Q_INVOKABLE void setIOEnable(int index, bool enable);

    Q_INVOKABLE int getIOInOut(int index) const {return iOInfos[index].iOInOut;};  // 0:Input, 1:Output
    Q_INVOKABLE void setIOInOut(int index, int direction);

    Q_INVOKABLE int getIOOutState(int index) const {return iOInfos[index].iOOutState;};  // 0,1,2,3
    Q_INVOKABLE void setIOOutState(int index, int state);

    Q_INVOKABLE double getIOFreq(int index) const {return iOInfos[index].iOFreq;};
    Q_INVOKABLE void setIOFreq(int index, double freq);

    Q_INVOKABLE double getIODuty(int index) const {return iOInfos[index].iODuty;};
    Q_INVOKABLE void setIODuty(int index, double duty);

    Q_INVOKABLE bool getIOInState(int index) const {return iOInfos[index].iOInIsHigh;};

    Q_INVOKABLE void iOPulseSyn();

    QStringList getIOList(){return iOList;};
    //--------------------------------------------------------------------------------------------------------
    Q_INVOKABLE int getDdsMode(int index);
    Q_INVOKABLE void setDdsMode(int index, int wave_index);

    Q_INVOKABLE int getDdsWaveIndex(int index);
    Q_INVOKABLE void setDdsWaveIndex(int index, int wave_index);

    Q_INVOKABLE double getDdsDuty(int index);
    Q_INVOKABLE void setDdsDuty(int index, double duty);

    Q_INVOKABLE double getDdsStartPhase(int index);
    Q_INVOKABLE void setDdsStartPhase(int index, double p);

    Q_INVOKABLE double getDdsFreq(int index);
    Q_INVOKABLE void setDdsFreq(int index, double freq);

    Q_INVOKABLE double getDdsStartFreq(int index);
    Q_INVOKABLE void setDdsStartFreq(int index, double freq);

    Q_INVOKABLE double getDdsStopFreq(int index);
    Q_INVOKABLE void setDdsStopFreq(int index, double freq);

    Q_INVOKABLE int getDdsSweepTimeUs(int index);
    Q_INVOKABLE void setDdsSweepTimeUs(int index, int us);

    Q_INVOKABLE int getDdsAmplitudeMv(int index);
    Q_INVOKABLE void setDdsAmplitudeMv(int index, int ampl);

    Q_INVOKABLE int getDdsBiasMv(int index);
    Q_INVOKABLE void setDdsBiasMv(int index, int bias);

    Q_INVOKABLE int getBurstStyle(int index);
    Q_INVOKABLE void setBurstStyle(int index, int style);

    Q_INVOKABLE int getLoopsNum(int index);
    Q_INVOKABLE void setLoopsNum(int index, int num);

    Q_INVOKABLE bool getLoopsNumInfinity(int index);
    Q_INVOKABLE void setLoopsNumInfinity(int index, bool style);

    Q_INVOKABLE qint64 getBurstPeriod(int index);
    Q_INVOKABLE void setBurstPeriod(int index, qint64 num);

    Q_INVOKABLE qint64 getBurstDelay(int index);
    Q_INVOKABLE void setBurstDelay(int index, qint64 num);

    Q_INVOKABLE int getDdsTriggerSource(int index);
    Q_INVOKABLE void setDdsTriggerSource(int index, int source);

    Q_INVOKABLE int getDdsIoIndex(int index);
    Q_INVOKABLE void setDdsIoIndex(int index, int ioindex);

    Q_INVOKABLE int getDdsOutput(int index);
    Q_INVOKABLE void setDdsOutput(int index, int out);

    Q_INVOKABLE int getDdsInputEdge(int index);
    Q_INVOKABLE void setDdsInputEdge(int index, int edge);

    Q_INVOKABLE bool getDdsOutputEnable(int index);
    Q_INVOKABLE void setDdsOutputEnable(int index, bool en);

    Q_INVOKABLE void setDdsMultSyn();
    Q_INVOKABLE void setDdsManualTrigger(int index);

    //---------------------------------------------------Power Supply-----------------------------------------------------
    bool getPowerSupport(){return powerSupport;};

    Q_INVOKABLE void setPosPowerEn(bool en);
    Q_INVOKABLE void setPosPowerV(double v);

    Q_INVOKABLE void setNegPowerEn(bool en);
    Q_INVOKABLE void setNegPowerV(double v);

    //------------------------------------------------------------------------------------------------------------
    bool getMeterSupport(){return meterSupport;};

    QString getMeterText(){return meterText;};
    QString getMeterTextTop(){return meterTextTop;};
    QString getMeterTextBottom(){return meterTextBottom;};

    Q_INVOKABLE void setMeterEn(bool en);
    Q_INVOKABLE void setMeterMode(unsigned int mode);


    //---------------------------------------------------FFT--------------------------------------------------------
    void setFftCal(FFTCal* cal){ fftCal = cal; }
    void setFilterCal(FilterCal* cal){ filterCal = cal; }
public Q_SLOTS:
    void noteTimeout();

    void setCaptureEnable(bool enable);
    bool getCaptureEnable(){return mCaptureEnable;};

    int getMaxCaptureLength();
signals:
    void devRedayChanged();
    void captureEnableChanged();
    void fileNameChanged();
    void workModeChanged();
    void oscChannelNumChanged();
    void ddsChannelNumChanged();
    void dIOChannelNumChanged();
    void oscCheckCh1Changed();
    void oscCheckCh2Changed();
    void oscCheckCh3Changed();
    void oscCheckCh4Changed();
    void acEnableCh1Changed();
    void acEnableCh2Changed();
    void acEnableCh3Changed();
    void acEnableCh4Changed();
    void triggerSourceListChanged();
    void triggerModeChanged();
    void triggerStyleChanged();
    void triggerSourceIndexChanged();
    void triggerSenseChanged();
    void preTriggerPercentChanged();
    void triggerLevelmVChanged();
    void supportSamplesChanged();
    void sampleIndexChanged();
    void captureLengthChanged();
    void dataUpdated();
    // 通知单个IO状态变化
    void ioStateChanged(int index);
    void ioInStateChanged(int index);
    void iOListChanged();
    void powerSupportChanged();
    void meterSupportChanged();
    void meterTextChanged();
private:
    void DisplayZoomCtrlCh1(bool start = false);
    void DisplayZoomCtrlCh2(bool start = false);
    void DisplayZoomCtrlCh3(bool start = false);
    void DisplayZoomCtrlCh4(bool start = false);

    void InitOscRealTimeMode();
    void InitOscStreamMode();
    void initIOs();

    int triggerSourceIndex2Code(int index);

    void DataUpdateMsg();
    bool StreamDataUpdateMsg();

    void ddsIOCtrl(int index);
private:
    //--------------------------------------hard info------------------------------
    unsigned int mDevId = -1;
    int mOscChannelNum = 4;
    int mDdsChannelNum = 2;
    int mDIOChannelNum = 8;
\
    bool mOscCheckCh1 = true;
    bool mOscCheckCh2 = true;
    bool mOscCheckCh3 = true;
    bool mOscCheckCh4 = true;

    //---------------------------------------------OSC------------------------------------------
    OscWorkMode workMode = OscRealTime;

    PlotRecord plotRecord;

    std::atomic<bool> m_work_thread_quit_success = true;
    int m_work_thread();
    static void m_work_thread_fun(void* parg);
    std::atomic<bool> m_work_thread_quit = false;

    typedef std::unique_lock<std::mutex> unique_lock;
    std::mutex m_work_thread_mutex;
    std::condition_variable m_work_thread_condition_variable;
    void notify_work_thread_exit();

    //---------------------------------------------OSC real time------------------------------------------------
    double m_ch1_range_min = 0, m_ch1_range_max = 0;
    double m_ch2_range_min = 0, m_ch2_range_max = 0;
    double m_ch3_range_min = 0, m_ch3_range_max = 0;
    double m_ch4_range_min = 0, m_ch4_range_max = 0;
    double m_ch1_plot_range_min = 0, m_ch1_plot_range_max = 0;
    double m_ch2_plot_range_min = 0, m_ch2_plot_range_max = 0;
    double m_ch3_plot_range_min = 0, m_ch3_plot_range_max = 0;
    double m_ch4_plot_range_min = 0, m_ch4_plot_range_max = 0;

    bool acEnableCh1 = false;
    bool acEnableCh2 = false;
    bool acEnableCh3 = false;
    bool acEnableCh4 = false;

    QStringList triggerSourceList;
    TriggerMode triggerMode = TriggerModeAuto;
    TriggerStyle triggerStyle = TriggerStyleRising;
    int triggerSourceIndex = 0;
    int triggerLevelmV = 0;
    double triggerSense = 0.1;
    int preTriggerPercent = 50;

    QStringList mSupportSamples;
    unsigned int* samples = nullptr;
    int sampleIndex = 0;

    unsigned short mCaptureChannel = 0x03;
    int mCaptureLength = 0;
    int mRealCaptureLength = 0;
    bool mCaptureEnable = false;

    //---------------------------------------------OSC stream------------------------------------------------
    WaveFile streamFile;

    bool mStreamFailed = false;
    bool mStreamSuccess = false;

    //---------------------------------------------IOs------------------------------------------------
    IOInfo iOInfos[16];

    QStringList iOList;
    //---------------------------------------------DDS------------------------------------------------
    DdsInfo ddsInfo[2];

    //---------------------------------------------------Power Supply-------------------------------
    bool powerSupport = false;

    //---------------------------------------------------Power Supply-------------------------------
    bool meterSupport = false;
    QString meterText = "";
    QString meterTextTop = "";
    QString meterTextBottom = "";

    //-----------------------------------------FFT----------------------------------------
    FFTCal* fftCal = nullptr;
    FilterCal* filterCal = nullptr;

    QTimer noteTimer;
};

#endif // VMUSBWAVE_H
