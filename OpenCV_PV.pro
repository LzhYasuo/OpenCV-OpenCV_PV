#-------------------------------------------------
#
# Project created by QtCreator 2021-07-27T13:33:25
#
#-------------------------------------------------

QT       += core gui

INCLUDEPATH += F:/lib_opencv/include/opencv \
               F:/lib_opencv/include/opencv2 \
               F:/lib_opencv/include

LIBS += -L F:/lib_opencv/lib/libopencv_*.a

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCV_PV
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    Gui/OpenCV_PV_Widget.cpp \
    Gui/OpenCVBaseCon.cpp \
    Filter/PVFilter.cpp \
    Pub/PubQss.cpp \
    Filter/PVActionPro.cpp \
    Gui/OpenCV_PV_GLWidget.cpp \
    Gui/OpenCV_PV_Btn.cpp \
    Task/OpenCV_PV_Thread.cpp

HEADERS += \
    Gui/OpenCV_PV_Widget.h \
    Gui/OpenCVBaseCon.h \
    Filter/PVFilter.h \
    Pub/PubQss.h \
    Filter/PVActionPro.h \
    Gui/OpenCV_PV_GLWidget.h \
    Gui/OpenCV_PV_Btn.h \
    Task/OpenCV_PV_Thread.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    paint.qrc
