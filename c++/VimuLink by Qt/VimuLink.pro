
TEMPLATE = app
TARGET = VimuLink

# 设置 C++ 标准
CONFIG += c++17
CONFIG += qmltypes

QT += core quick qml serialport widgets

QML_IMPORT_NAME = VmSimplePlot CardCtrl FrontUartCtrl
QML_IMPORT_MAJOR_VERSION = 1

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#message("Current config: $$CONFIG")
#message("Host architecture: $$QMAKE_HOST.arch")

# Include directories
INCLUDEPATH += ./ ../../SharedLibrary ./plot ./wavemng ./filter \
            ../../SharedLibrary/vmsignal/inc ../../SharedLibrary/vmsignal/inc/window \
            ../../SharedLibrary/vmsignal/inc/fft ../../SharedLibrary/vmsignal/inc/fftcpp \
            ../../SharedLibrary/vmsignal/inc/fir ../../SharedLibrary/vmsignal/inc/iir

HEADERS += ./plot/Axis.h \
            ./plot/PlotBasic.h \
            ./plot/PlotHelp.h \
            ./plot/plottype.h \
            ./plot/SimplePlotBasic.h \
            ./plot/VmSimplePlot.h \
            ./wavemng/points.h \
            ./wavemng/AbsPhysicsBlockFile.h \
            ./wavemng/AbsSummary.h \
            ./wavemng/BlockFile.h \
            ./wavemng/BlockFileInfo.h \
            ./wavemng/EmsPhysicsBlockFile.h \
            ./wavemng/HdPhysicsBlockFile.h \
            ./wavemng/MemSummary.h \
            ./wavemng/plotpoints.h \
            ./wavemng/SummaryInfo.h \
            ./wavemng/TempFileMng.h \
            ./wavemng/TempFileMngRec.h \
            ./wavemng/wavefile.h \
            ./wavemng/wavemngdef.h \
            ./wavemng/WaveTrack.h \
            ./wavemng/WaveTrackInfo.h \
            ./vmusbwave.h \
            ./arbwaves.h \
            ./defines.h \
            ./plotrecord.h \
            ./fftcal.h \
            ./filter/BasicFilter.h \
            ./filter/FreqFirFilter.h \
            ./filter/IirFilter.h \
            ./filter/WindowFirFilter.h \
            ./filtercal.h

# Source files
SOURCES += main.cpp \
	./plot/Axis.cpp  \
        ./plot/PlotBasic.cpp \
        ./plot/PlotHelp.cpp \
        ./plot/SimplePlotBasic.cpp \
        ./plot/VmSimplePlot.cpp \
        ./wavemng/AbsPhysicsBlockFile.cpp \
        ./wavemng/AbsSummary.cpp \
        ./wavemng/BlockFile.cpp \
        ./wavemng/BlockFileInfo.cpp \
        ./wavemng/EmsPhysicsBlockFile.cpp \
        ./wavemng/HdPhysicsBlockFile.cpp \
        ./wavemng/MemSummary.cpp \
        ./wavemng/plotpoints.cpp \
        ./wavemng/SummaryInfo.cpp \
        ./wavemng/TempFileMng.cpp \
        ./wavemng/TempFileMngRec.cpp \
        ./wavemng/wavefile.cpp \
        ./wavemng/WaveTrack.cpp \
        ./wavemng/WaveTrackInfo.cpp \
	./vmusbwave.cpp \
        ./plotrecord.cpp \
        ./fftcal.cpp \
        ./filter/BasicFilter.cpp \
        ./filter/FreqFirFilter.cpp \
        ./filter/IirFilter.cpp \
        ./filter/WindowFirFilter.cpp \
        ./filtercal.cpp

RESOURCES += vmmso.qrc


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

#根据平台修改对应的位置
# 根据系统类型设置平台相关目录
win32 {
    contains(QMAKE_HOST.arch, x86_64) {
        CONFIG(debug, debug|release) {
            LIBS += -L$$PWD/../../SharedLibrary/Windows/X64/Debug/
        } else {
            LIBS += -L$$PWD/../../SharedLibrary/Windows/X64/Release/
        }
    } else:contains(QMAKE_HOST.arch, i386) {
        CONFIG(debug, debug|release) {
            LIBS += -L$$PWD/../../SharedLibrary/Windows/X86/Debug/
        } else {
            LIBS += -L$$PWD/../../SharedLibrary/Windows/X86/Release/
        }
    } else:contains(QMAKE_HOST.arch, arm64) {
        CONFIG(debug, debug|release) {
            LIBS += -L$$PWD/../../SharedLibrary/Windows/ARM64/Debug/
        } else {
            LIBS += -L$$PWD/../../SharedLibrary/Windows/ARM64/Release/
        }
    }
    LIBS += vmmultmso.lib
    LIBS += vmsignal.lib
}

linux {
    #vmmoslib_dir =  $$PWD/SharedLibrary/Ubuntu/aarch64-openeuler-linux-gnu-g++/Release
    #vmmoslib_dir =  $$PWD../../SharedLibrary/Ubuntu/aarch64-buildroot-linux-gnu-g++/Release
    #vmmoslib_dir = ./SharedLibrary/Ubuntu/aarch64-linux/Release
    #vmmoslib_dir = $$PWD/SharedLibrary/Ubuntu/X64/Debug/
    vmmoslib_dir = $$PWD/../../SharedLibrary/Ubuntu22.04/X64/Release/

    LIBS += -L$${vmmoslib_dir} -lvmmultmso -lvmsignal
}


msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
