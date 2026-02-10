
/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QTranslator>
#include <QQuickWindow>
#include <QFont>
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    #include <QQuickStyle>
#endif

#include "vmusbwave.h"
#include "fftcal.h"
#include "filtercal.h"
#include "plot/VmSimplePlot.h"
#include "plotrecord.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    //QQuickWindow::setSceneGraphBackend("software");

    qmlRegisterType<VmUsbWave>("Vimu.Qml.Ctrls", 1, 0, "VmUsbWave");
    qmlRegisterType<VmSimplePlot>("Vimu.Qml.Ctrls", 1, 0, "VmSimplePlot");
    qmlRegisterType<FFTCal>("Vimu.Qml.Ctrls", 1, 0, "FFTCal");
    qmlRegisterType<FilterCal>("Vimu.Qml.Ctrls", 1, 0, "FilterCal");

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    QQuickStyle::setStyle("Universal");  //Universal Material Fusion
#endif

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "VMMSO_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    //const QUrl url(u"qrc:/qml/main.qml"_qs);
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QList<QObject*> root_objs = engine.rootObjects();
    if(root_objs.size()!=0)
    {
        VmSimplePlot* vmdsoplot = nullptr;
        QObject* pobj = root_objs.at(0)->findChild<QObject*>("objectvmdsoplot");
        if(pobj!=nullptr)
            vmdsoplot = dynamic_cast<VmSimplePlot*>(pobj);

        VmUsbWave* vmusbwave = nullptr;
        pobj = root_objs.at(0)->findChild<QObject*>("objectvmusbwave");
        if(pobj!=nullptr)
            vmusbwave = dynamic_cast<VmUsbWave*>(pobj);

        FFTCal* fftcal = nullptr;
        pobj = root_objs.at(0)->findChild<QObject*>("objectfft");
        if(pobj!=nullptr)
            fftcal = dynamic_cast<FFTCal*>(pobj);

        FilterCal* filtercal = nullptr;
        pobj = root_objs.at(0)->findChild<QObject*>("objectfilter");
        if(pobj!=nullptr)
            filtercal = dynamic_cast<FilterCal*>(pobj);

        //将数据存储器的指针，传给绘图
        if((vmdsoplot!=nullptr)&&(vmusbwave!=nullptr))
            vmdsoplot->setPlotRecord(vmusbwave->getPlotRecord());

        if((fftcal!=nullptr)&&(vmdsoplot!=nullptr))
            vmdsoplot->setFftCal(fftcal);

        if((fftcal!=nullptr)&&(vmusbwave!=nullptr))
            vmusbwave->setFftCal(fftcal);

        if((filtercal!=nullptr)&&(vmusbwave!=nullptr))
            vmusbwave->setFilterCal(filtercal);



        //QVariant rValue;
        //QVariant msg = "Hello for C++ Add";
        //QMetaObject::invokeMethod(root_objs->at(0), "usbDevCallBack",  Q_RETURN_ARG(QVariant,rValue),  Q_ARG(QVariant, msg));
    }

    int exe_res = app.exec();
    return exe_res;
}
