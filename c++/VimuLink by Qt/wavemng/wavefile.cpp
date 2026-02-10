#include "wavefile.h"
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

WaveFile::WaveFile()
{

}

WaveFile::~WaveFile()
{
    if(file!=nullptr)
        delete file;
}

bool WaveFile::startRecord(int sample, unsigned short captureChannel, unsigned int adcbit,
                           double zoomch1, double biasch1, double zoomch2, double biasch2,
                           double zoomch3, double biasch3, double zoomch4, double biasch4)
{
    //检查目录是否存在
    QString directoryPath = QCoreApplication::applicationDirPath() + QDir::separator() + datasDir;
    QDir directory(directoryPath);
    if (!directory.exists())
    {
        // 尝试创建目录
        if (directory.mkpath("."))
        {
            qDebug() << "directory create success " << directoryPath;
        }
    }

    // 获取当前日期 格式化为年-月-日
    QString TimeStr = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    mFilePath = directoryPath + QDir::separator() + TimeStr + ".vmlkdata" ;

    qDebug()<<mFilePath;

    stopRecord();
    file = new QFile(mFilePath);
    if(!file->open(QIODevice::ReadWrite))
        return false;

    mDataSize = 0;
    file->write((char*)&sample, sizeof(int));

    file->write((char*)&adcbit, sizeof(unsigned int));

    int chnnum = 0;
    if(captureChannel&0x01)
        chnnum++;
    if(captureChannel&0x02)
        chnnum++;
    if(captureChannel&0x04)
        chnnum++;
    if(captureChannel&0x08)
        chnnum++;
    file->write((char*)&chnnum, sizeof(int));

    if(captureChannel&0x01)
    {
        int source = SHIYUBX_SOURCE_CH1;
        file->write((char*)&source, sizeof(int));
        file->write((char*)&zoomch1, sizeof(double));
        file->write((char*)&biasch1, sizeof(double));
    }
    if(captureChannel&0x02)
    {
        int source = SHIYUBX_SOURCE_CH2;
        file->write((char*)&source, sizeof(int));
        file->write((char*)&zoomch2, sizeof(double));
        file->write((char*)&biasch2, sizeof(double));
    }
    if(captureChannel&0x04)
    {
        int source = SHIYUBX_SOURCE_CH3;
        file->write((char*)&source, sizeof(int));
        file->write((char*)&zoomch3, sizeof(double));
        file->write((char*)&biasch3, sizeof(double));
    }
    if(captureChannel&0x08)
    {
        int source = SHIYUBX_SOURCE_CH4;
        file->write((char*)&source, sizeof(int));
        file->write((char*)&zoomch4, sizeof(double));
        file->write((char*)&biasch4, sizeof(double));
    }

    file->write((char*)&mDataSize, sizeof(int64_t));

    return true;
}

bool WaveFile::writeDataSize(int64_t length)
{
    if(file!=nullptr)
    {
        file->seek(sizeof(int)+sizeof(double)*4);
        file->write((char*)&length, sizeof(int64_t));
        return true;
    }
    return false;
}

int64_t WaveFile::getDataStartPos()
{
    return sizeof(int)+sizeof(double)*4+sizeof(int64_t);
}

bool WaveFile::addWaves(char* buffer, int64_t length)
{
    if(file!=nullptr)
    {
        file->seek(getDataStartPos()+mDataSize);
        file->write(buffer, length);

        mDataSize+=length;
        writeDataSize(mDataSize);
        return true;
    }
    return false;
}

bool WaveFile::startRead(QString filePath)
{
    if(file!=nullptr)
        delete file;

    file = new QFile(filePath);
    if(!file->open(QIODevice::ReadOnly))
        return false;

    return true;
}

bool WaveFile::readWaves(char* buffer, int64_t begin, int64_t length)
{
    if(file!=nullptr)
    {
        file->seek(getDataStartPos()+begin);
        file->read(buffer, length);
        return true;
    }
    return false;
}

int64_t WaveFile::readWaves(SHIYUBX_SOURCE readsource, double* buffer, int64_t begin, int64_t length)
{
    int readlength = 0;
    if(file!=nullptr)
    {
        file->seek(sizeof(int));

        unsigned int adcbit = 8;
        file->read((char*)&adcbit, sizeof(unsigned int));

        int chnnum = 0;
        file->read((char*)&chnnum, sizeof(int));

        double zoomch1 = 1.0;
        double biasch1 = 0.0;
        double zoomch2 = 1.0;
        double biasch2 = 0.0;
        double zoomch3 = 1.0;
        double biasch3 = 0.0;
        double zoomch4 = 1.0;
        double biasch4 = 0.0;

        int channelIndex = 0;
        for(int p=0; p<chnnum; p++)
        {
            int chnsource = SHIYUBX_SOURCE_ZERO;
            file->read((char*)&chnsource, sizeof(int));

            if(chnsource == SHIYUBX_SOURCE_CH1)
            {
                file->read((char*)&zoomch1, sizeof(double));
                file->read((char*)&biasch1, sizeof(double));
            }
            else if(chnsource == SHIYUBX_SOURCE_CH2)
            {
                file->read((char*)&zoomch2, sizeof(double));
                file->read((char*)&biasch2, sizeof(double));
            }
            else if(chnsource == SHIYUBX_SOURCE_CH3)
            {
                file->read((char*)&zoomch3, sizeof(double));
                file->read((char*)&biasch3, sizeof(double));
            }
            else if(chnsource == SHIYUBX_SOURCE_CH4)
            {
                file->read((char*)&zoomch4, sizeof(double));
                file->read((char*)&biasch4, sizeof(double));
            }

            if(chnsource<readsource)
                channelIndex++;
        }

        //如果是3个通道，需要占用4个通道对齐存储
        if(chnnum==3)
            chnnum=4;

        qDebug() << "chnnum " << chnnum<< " readsource " << readsource<< " channelIndex " << channelIndex;
        int blocksize = chnnum*(adcbit>8? 2:1);
        if(adcbit==8)
        {
            unsigned char* adcs = new unsigned char[length*blocksize];

            file->seek(getDataStartPos()+begin*blocksize);
            readlength = file->read((char*)adcs, length*blocksize);

            for(int k=0; k<length; k++)
                    buffer[k] = adcs[k*chnnum+channelIndex]*zoomch1+biasch1;

            delete [] adcs;
        }
        else if(adcbit==12)
        {
            unsigned short* adcs = new unsigned short[length*blocksize];

            file->seek(getDataStartPos()+begin*blocksize);
            readlength = file->read((char*)adcs, length*blocksize);

            for(int k=0; k<length; k++)
                buffer[k] = adcs[k*chnnum+channelIndex]*zoomch1+biasch1;

            delete [] adcs;
        }

        return readlength/blocksize;
    }
    return readlength;
}

bool WaveFile::stopRecord()
{
    if(file!=nullptr)
    {
        file->close();
        delete file;
        file = nullptr;
    }
    return true;
}


// 统计指定目录下的文件个数，并删除最早的文件
void WaveFile::processDirectory(int maxfilenum)
{
    QString directoryPath = QCoreApplication::applicationDirPath() + QDir::separator() + datasDir;
    QDir directory(directoryPath);
    if (!directory.exists())
    {
        qDebug() << "WaveFile Directory not exists: " << directoryPath;
        return;
    }

    // 筛选出文件（不包括目录）
    QStringList filters;
    filters << "*";
    directory.setNameFilters(filters);
    directory.setFilter(QDir::Files);

    QFileInfoList fileList = directory.entryInfoList();
    int fileCount = fileList.count();
    qDebug() << "file num: " << fileCount;

    if (fileCount <= maxfilenum)
    {
        qDebug() << "file num not larger maxnum";
        return;
    }

    // 找到最早的文件
    QFileInfo earliestFile = fileList.first();
    for (const QFileInfo& fileInfo : fileList)
    {
        if (fileInfo.created() < earliestFile.created())
        {
            earliestFile = fileInfo;
        }
    }

    // 删除最早的文件
    QFile file(earliestFile.absoluteFilePath());
    if (file.remove())
    {
        qDebug() << "delete file success: " << earliestFile.absoluteFilePath();
    }
    else
    {
        qDebug() << "delete file failed: " << earliestFile.absoluteFilePath();
    }
}

