#-------------------------------------------------
#
# Project created by QtCreator 2013-04-24T17:31:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_RCDecode
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    SamplingGrid.cpp \
    RsDecode.cpp \
    QRCodeInfo.cpp \
    QRcodeImage.cpp \
    Point.cpp \
    Line.cpp \
    Galois.cpp \
    FinderPattern.cpp \
    ContentDecoder.cpp \
    Axis.cpp \
    AlignmentPattern.cpp

HEADERS  += dialog.h \
    SamplingGrid.h \
    RsDecode.h \
    QRCodeInfo.h \
    QRcodeImage.h \
    Point.h \
    Line.h \
    Galois.h \
    FinderPattern.h \
    ContentDecoder.h \
    Axis.h \
    AlignmentPattern.h

FORMS    += dialog.ui
