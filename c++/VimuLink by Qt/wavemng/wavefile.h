#ifndef WAVEFILE_H
#define WAVEFILE_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <QString>
#include <QFile>
#include "wavemngdef.h"

class WaveFile
{
public:
    WaveFile();
    ~WaveFile();

    bool startRecord(int sample, unsigned short captureChannel, unsigned int adcbit,
                     double zoomch1, double biasch1, double zoomch2, double biasch2, double zoomch3, double biasch3, double zoomch4, double biasch4);
    bool isRecording(){return file!=nullptr;};
    QString getFileName(){return mFilePath;}
    bool stopRecord();
    bool addWaves(char* buffer, int64_t length);

    bool startRead(QString filePath);
    bool readWaves(char* buffer, int64_t begin, int64_t length);
    int64_t readWaves(SHIYUBX_SOURCE source, double* buffer, int64_t begin, int64_t length);

    void processDirectory(int maxfilenum);
private:
    bool writeDataSize(int64_t length);
    int64_t getDataStartPos();

    int mSample = 0;
    double mZoomCh1 = 1.0, mZoomCh2 = 1.0;
    double mBiasCh1 = 0, mBiasCh2 = 0;
    int64_t mDataSize = 0;

    QString datasDir = ("datas");
    QString mFilePath = "";
    QFile* file = nullptr;

};

#endif // WAVEFILE_H
