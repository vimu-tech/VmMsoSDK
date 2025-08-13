
TEMPLATE = app
TARGET = DllTestQt

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
INCLUDEPATH += ./ ../SharedLibrary

HEADERS += ./plot/Axis.h \
            ./plot/PlotAxis.h \
            ./plot/PlotBasic.h \
            ./plot/PlotHelp.h \
            ./plot/plottype.h \
            ./plot/SimplePlotBasic.h \
            ./plot/VmSimplePlot.h \
            ./vmusbwave.h

# Source files
SOURCES += main.cpp \
	./plot/Axis.cpp  \
        ./plot/PlotAxis.cpp \
        ./plot/PlotBasic.cpp \
        ./plot/PlotHelp.cpp \
        ./plot/SimplePlotBasic.cpp \
        ./plot/VmSimplePlot.cpp \
	./vmusbwave.cpp

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
            LIBS += -L$$PWD/../SharedLibrary/Windows/X64/Debug/
        } else {
            LIBS += -L$$PWD/../SharedLibrary/Windows/X64/Release/
        }
    } else:contains(QMAKE_HOST.arch, i386) {
        CONFIG(debug, debug|release) {
            LIBS += -L$$PWD/../SharedLibrary/Windows/X86/Debug/
        } else {
            LIBS += -L$$PWD/../SharedLibrary/Windows/X86/Release/
        }
    } else:contains(QMAKE_HOST.arch, arm64) {
        CONFIG(debug, debug|release) {
            LIBS += -L$$PWD/../SharedLibrary/Windows/ARM64/Debug/
        } else {
            LIBS += -L$$PWD/../SharedLibrary/Windows/ARM64/Release/
        }
    }
    LIBS += vmmso.lib
}

linux {
    #vmmoslib_dir =  $$PWD/SharedLibrary/Ubuntu/aarch64-openeuler-linux-gnu-g++/Release
    vmmoslib_dir =  $$PWD/SharedLibrary/Ubuntu/aarch64-buildroot-linux-gnu-g++/Release
    #vmmoslib_dir = ./SharedLibrary/Ubuntu/aarch64-linux/Release
    #vmmoslib_dir = $$PWD/SharedLibrary/Ubuntu/X64/Debug/
    #vmmoslib_dir = $$PWD/SharedLibrary/Ubuntu/X64/Release/

    LIBS += -L$${vmmoslib_dir} -lvmmso
}


msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
