#include "TempFileMng.h"
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QTextStream>

TempFileMng::TempFileMng()
    : m_tempfiledir("")
{
    m_tempfiledir = QDir::tempPath() + QDir::separator() + "Dgnxnxhfxy";

    QDir dir;
    if (!dir.exists(m_tempfiledir)) {
        dir.mkpath(m_tempfiledir);
    }
}

TempFileMng::~TempFileMng(void)
{
    // 递归删除临时目录
    QDir tempDir(m_tempfiledir);
    if (tempDir.exists()) {
        tempDir.removeRecursively();
    }

	DestoryAll();
}

void TempFileMng::DestoryAll()
{
    foreach (TempFileMngRec* rec, filelist) {
        delete rec;
    }
    filelist.clear();
}

QString TempFileMng::GetTempFilename(const QString &pathname)
{
    if (pathname.isEmpty()) {  // 对于内存文件
        return "";
    }

    QString filename;

    // 查找是否已存在记录
    foreach (TempFileMngRec* rec, filelist) {
        if (rec->m_pathname == pathname) {
            rec->m_count++;
            filename = QString("%1%2.sm")
                           .arg(rec->m_tempDirectory)
                           .arg(rec->m_count, 8, 10, QLatin1Char('0'));
            return filename;
        }
    }

    // 创建新记录
    TempFileMngRec* mngrec = new TempFileMngRec(pathname, CreateTempDirectory(pathname));
    filelist.append(mngrec);

    filename = QString("%1%2.sm")
                   .arg(mngrec->m_tempDirectory)
                   .arg(mngrec->m_count, 8, 10, QLatin1Char('0'));

    return filename;
}

QString TempFileMng::CreateTempDirectory(const QString &pathname)
{
    // 查找是否已存在相同的路径名
    foreach (TempFileMngRec* rec, filelist) {
        if (rec->m_pathname == pathname) {
            return rec->m_tempDirectory;
        }
    }

    // 生成基于时间的目录名
    QDateTime currentTime = QDateTime::currentDateTime();
    QString dirName = QString("%1%2%3%4%5")
                          .arg(currentTime.date().month())
                          .arg(currentTime.date().day())
                          .arg(currentTime.time().hour())
                          .arg(currentTime.time().minute())
                          .arg(currentTime.time().second());

    QString newfiledirectory = m_tempfiledir + QDir::separator() + dirName + QDir::separator();

    // 创建目录
    QDir dir;
    if (!dir.exists(newfiledirectory)) {
        dir.mkpath(newfiledirectory);
    }

    return newfiledirectory;
}
