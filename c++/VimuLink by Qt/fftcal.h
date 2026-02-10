#ifndef FFTCAL_H
#define FFTCAL_H

#include <QObject>
#include "./wavemng/wavemngdef.h"
#include "./vmsignal/inc/window/vmwindow.h"
#include "./vmsignal/inc/fft/vmfft.h"
#include "./vmsignal/inc/fft/vmfftinfo.h"
#include "wavemng/WaveTrack.h"
#include <mutex>

class FFTCal: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool fftCheck READ getFFTCheck WRITE setFFTCheck NOTIFY fftCheckChanged)

    Q_PROPERTY(int sourceIndex READ getSourceIndex WRITE setSourceIndex NOTIFY sourceIndexChanged)

    Q_PROPERTY(QStringList lengthList READ getLengthList NOTIFY lengthListChanged)
    Q_PROPERTY(int lengthIndex READ getLengthIndex WRITE setLengthIndex NOTIFY lengthIndexChanged)

    Q_PROPERTY(QStringList windowStyleList READ getWindowStyleList NOTIFY windowStyleListChanged)
    Q_PROPERTY(int windowStyleIndex READ getWindowStyleIndex WRITE setWindowStyleIndex NOTIFY windowStyleIndexChanged)

    Q_PROPERTY(double windowPara READ getWindowPara WRITE setWindowPara NOTIFY windowParaChanged)

public:
    FFTCal();
    ~FFTCal();

    bool getFFTCheck(){return fftCheck;}
    void setFFTCheck(bool check)
    {
        if(fftCheck!=check)
        {
            fftCheck = check;
            emit fftCheckChanged();
        }
    }

    int getSourceIndex(){return sourceIndex;}
    void setSourceIndex(int index)
    {
        if(sourceIndex!=index)
        {
            sourceIndex = index;
            emit sourceIndexChanged();
        }
    }

    SHIYUBX_SOURCE getSource()
    {
        return sourceList[sourceIndex];
    }

    QStringList getLengthList();
    int getLengthIndex(){return lengthIndex;}
    void setLengthIndex(int index)
    {
        if(lengthIndex!=index)
        {
            lengthIndex = index;
            emit lengthIndexChanged();
        }
    }

    QStringList getWindowStyleList();
    int getWindowStyleIndex(){return windowStyle;}
    void setWindowStyleIndex(int index)
    {
        if(windowStyle!=index)
        {
            windowStyle = (WINDOW_STYLE)index;
            emit windowStyleIndexChanged();
        }
    }

    double getWindowPara(){return windowPara;}
    void setWindowPara(double p)
    {
        if(windowPara!=p)
        {
            windowPara = p;
            emit windowParaChanged();
        }
    }

    bool updateVolDatas(double* vol, unsigned int length, unsigned int sample);
    void updateDatas(double* buffer, int length);

    bool getPlotPoints(QString& trackName, plotpoints& ppoints);

signals:
    void fftCheckChanged();
    void sourceIndexChanged();
    void lengthListChanged();
    void lengthIndexChanged();
    void windowStyleListChanged();
    void windowStyleIndexChanged();
    void windowParaChanged();
private: 
    void multFactor(double* buf, unsigned int len, double factor);

    bool fftCheck = false;
    int sourceIndex = 0;
    int lengthIndex = 8;
    WINDOW_STYLE windowStyle = BLACKMAN;
    double windowPara = 0;
    FFT_Amplitude_Ref_Type ampRefType = FFT_Amplitude_Ref_V;

    double *re = nullptr;
    double *im = nullptr;
    double* m_mag_buffer = nullptr;
    double* m_ph_buffer = nullptr;
    unsigned int bufferLength = 0;

    fftinfo m_fft_info;

    unsigned int mSample = 0;
    //-----------------------------------------------------------------------------------------
    typedef std::lock_guard<std::recursive_mutex> lock_guard;
    std::recursive_mutex m_mutex;

    WaveTrack* trackFFT = nullptr;

};

#endif // FFTCAL_H
