#include "filtercal.h"
#include "defines.h"
#include "vmfilters.h"
#include <QFile>
#include "WindowFirFilter.h"
#include "FreqFirFilter.h"
#include "IirFilter.h"
#include <QDebug>

FilterCal::FilterCal()
{

}

FilterCal::~FilterCal()
{
    if (mFilter != nullptr)
        delete mFilter;

    if (filter_buffer != nullptr)
        delete filter_buffer;
}

QStringList FilterCal::getLengthList()
{
    QStringList lt;
    for(int k=0; k<sizeof(FFTN)/sizeof(FFTN[0]); k++)
        lt.push_back(QString("%1").arg(FFTN[k]));
    return lt;
}

void FilterCal::multFactor(double* buf, unsigned int len, double factor)
{
    for (unsigned int i = 0; i < len; i++)
        buf[i] *= factor;
}

bool FilterCal::readFddFile(QString filePathName)
{
    if (mFilter != nullptr)
        delete mFilter;

    fddPathFileName = filePathName;

    CBasicFilter::FILTERTYPE type = CBasicFilter::GetFilterStyle(filePathName.toStdString());
    if (type == CBasicFilter::FIR_WND)
    {
        mFilter = new CWindowFirFilter(CBasicFilter::English);
    }
    else if (type == CBasicFilter::FIR_PREQ)
    {
        mFilter = new CFreqFirFilter(CBasicFilter::English);
    }
    else if (type == CBasicFilter::IIR)
    {
        mFilter = new CIirFilter(CBasicFilter::English);
    }
    fddLoaded = mFilter->Loadfdd(filePathName.toStdString());

    emit fddLoadedChanged();
    emit fddPathFileNameChanged();

    return mFilter!=nullptr;
}

bool FilterCal::updateVolDatas(double* buffer, double triggerPoint, unsigned int length)
{
    unsigned int filter_max_length = FFTN[lengthIndex];

    if (mFilter != nullptr)
    {
        unsigned int uselength = length > filter_max_length ? filter_max_length : length;

        if (filter_buffer == nullptr)
            filter_buffer = new double[uselength];
        else if (filter_length != uselength)
        {
            delete[] filter_buffer;
            filter_buffer = new double[uselength];
        }
        filter_length = uselength;

        //从触发点，来截图数据
        filter_trigger = (double)filter_length * triggerPoint / (double)length;
        int start = triggerPoint - filter_trigger;
        qDebug()<<" filter_length " << filter_length << "start " << start;
        memcpy(filter_buffer, buffer + start, filter_length * sizeof(double));
        //滤波
        if (mFilter != nullptr)
            mFilter->Filter(filter_buffer, filter_length);
    }
    else
        return false;
    return true;
}
