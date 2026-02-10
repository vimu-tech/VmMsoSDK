#ifndef FILTERCAL_H
#define FILTERCAL_H

#include <QObject>
#include "wavemngdef.h"
#include "vmfft.h"
#include "BasicFilter.h"

class FilterCal: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enable READ getEnable WRITE setEnable NOTIFY enableChanged)
    Q_PROPERTY(bool fddLoaded READ getFddLoaded NOTIFY fddLoadedChanged)
    Q_PROPERTY(QString fddPathFileName READ getFddPathFileName NOTIFY fddPathFileNameChanged)

    Q_PROPERTY(int sourceIndex READ getSourceIndex WRITE setSourceIndex NOTIFY sourceIndexChanged)

    Q_PROPERTY(QStringList lengthList READ getLengthList NOTIFY lengthListChanged)
    Q_PROPERTY(int lengthIndex READ getLengthIndex WRITE setLengthIndex NOTIFY lengthIndexChanged)

public:
    FilterCal();
    ~FilterCal();

    bool getEnable(){return enable;}
    void setEnable(bool en)
    {
        if(enable!=en)
        {
            enable = en;
            emit enableChanged();
        }
    }

    bool getFddLoaded(){return fddLoaded;}

    QString getFddPathFileName(){return fddPathFileName;}

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

    bool updateVolDatas(double* vol, double triggerPoint, unsigned int length);
    double* getFilterBuffer(){return filter_buffer;}
    unsigned int getFilterLength(){return filter_length;}
    double getFilterTriggerPoint(){return filter_trigger;}

    Q_INVOKABLE bool readFddFile(QString filePathName);
signals:
    void enableChanged();
    void fddLoadedChanged();
    void fddPathFileNameChanged();
    void sourceIndexChanged();
    void lengthListChanged();
    void lengthIndexChanged();
    void windowStyleListChanged();
    void windowStyleIndexChanged();
    void windowParaChanged();
private:
    void multFactor(double* buf, unsigned int len, double factor);

    bool enable = false;
    bool fddLoaded = false;

    QString fddPathFileName = "";
    CBasicFilter* mFilter = nullptr;


    int sourceIndex = 0;
    int lengthIndex = 8;

    double *filter_buffer = nullptr;
    unsigned int filter_length = 0;
    double filter_trigger = 0;
};

#endif // FFTCAL_H
