#include "fftcal.h"
#include "defines.h"
#include "./vmsignal/inc/fft/vmcal_m.h"
#include "./vmsignal/inc/fft/vmreal_fft.h"
#include <QDebug>

FFTCal::FFTCal()
{

}

FFTCal::~FFTCal()
{
    if (re != nullptr)
        delete[] re;
    if (im != nullptr)
        delete[] im;
    if (m_mag_buffer != nullptr)
        delete[] m_mag_buffer;
    if (m_ph_buffer != nullptr)
        delete[] m_ph_buffer;

    if(trackFFT!=nullptr)
        delete [] trackFFT;
}

QStringList FFTCal::getLengthList()
{
    QStringList lt;
    for(int k=0; k<sizeof(FFTN)/sizeof(FFTN[0]); k++)
        lt.push_back(QString("%1").arg(FFTN[k]));
    return lt;
}

QStringList FFTCal::getWindowStyleList()
{
    QStringList lt;
    for(int k=0; k<sizeof(FFTN)/sizeof(FFTN[0]); k++)
        lt.push_back(QString(Window_Name[k]));
    return lt;
}

void FFTCal::multFactor(double* buf, unsigned int len, double factor)
{
    for (unsigned int i = 0; i < len; i++)
        buf[i] *= factor;
}

bool FFTCal::updateVolDatas(double* buffer, unsigned int length, unsigned int sample)
{
    mSample = sample;

    unsigned int fft_n = FFTN[lengthIndex];
    if (bufferLength != fft_n)
    {
        if (re != nullptr)
            delete[] re;
        if (im != nullptr)
            delete[] im;
        if (m_mag_buffer != nullptr)
            delete[] m_mag_buffer;
        if (m_ph_buffer != nullptr)
            delete[] m_ph_buffer;

        re = new double[fft_n];
        im = new double[fft_n];
        m_mag_buffer = new double[fft_n];
        m_ph_buffer = new double[fft_n];

        bufferLength = fft_n;
    }

    //开始计算
    unsigned int data_len = fft_n < length ? fft_n : length;

    //缓冲区
    memcpy(re, buffer, data_len * sizeof(double));
    if (data_len < fft_n)
        memset(re + data_len, 0, sizeof(double) * (fft_n - data_len));
    memset(im, 0, fft_n * sizeof(double));

    //加窗
    double window_gain = window_i(windowStyle, re, data_len, windowPara, 0); //alpha_beta=0 使用len，不是fft_n
    //fft
    real_fft_signal(re, im, fft_n, data_len, 8);

    //除以窗函数的相干增益
    multFactor(re, fft_n, 1.0/window_gain);
    multFactor(im, fft_n, 1.0/window_gain);

    Fft_Amplitude(re, im, m_mag_buffer, fft_n, true);

    for (unsigned int i = 0; i < fft_n / 2; i++)//将后半部分，叠加大前     i=0是直流分量,结果是实际的2倍
        m_mag_buffer[i] = m_mag_buffer[i] * 2;

    Fft_Amplitude_Conver_Type(m_mag_buffer, fft_n / 2, 50, ampRefType);

    //
    updateDatas(m_mag_buffer, fft_n / 2);

    return true;
}

//----------------------------------------------------------------------------------------------------
void FFTCal::updateDatas(double* buffer, int length)
{
    lock_guard lock(m_mutex);

    //qDebug()<< "updateDatas " << length;

    if(length>0)
    {
        if(trackFFT!=nullptr)
        {
            if(trackFFT->GetCurLength()!=length)
            {
                delete trackFFT;
                trackFFT = nullptr;
            }
            else if (trackFFT->GetSample() != mSample)
            {
                delete trackFFT;
                trackFFT = nullptr;
            }
        }

        if(trackFFT==nullptr)
        {
            WaveTrackInfo trackinfo(doubleSample, mSample);
            trackFFT = new WaveTrack(fftName, 1, length, trackinfo, 0, mSample/2);
            trackFFT->AddData(buffer, length);
        }
        else
            trackFFT->ChangeData(buffer, 0, length);
    }
    else
    {
        if(trackFFT!=nullptr)
            delete trackFFT;
        trackFFT = nullptr;
    }
}

bool FFTCal::getPlotPoints(QString& trackName, plotpoints& ppoints)
{
    lock_guard lock(m_mutex);

    qDebug()<< "getPlotPoints " << trackName;

    bool success = false;
    if((trackName==fftName)&&(trackFFT!=nullptr))
        success = trackFFT->GetPlotPoints(&ppoints);
    //qDebug()<<"GetPlotPoints success" << success <<" cur_pixel_num "<< ppoints.cur_pixel_num;
    return success;
}

