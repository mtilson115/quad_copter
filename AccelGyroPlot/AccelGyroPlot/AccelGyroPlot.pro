#-------------------------------------------------
#
# Project created by QtCreator 2014-04-13T09:38:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = AccelGyroPlot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../../../../qcustomplot/qcustomplot.cpp \
    serial_port.cpp

HEADERS  += mainwindow.h \
    ../../../../../qcustomplot/qcustomplot.h \
    serial_port.h

FORMS    += mainwindow.ui
