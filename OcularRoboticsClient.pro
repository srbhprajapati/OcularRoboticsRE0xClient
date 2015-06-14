#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T19:56:10
#
#-------------------------------------------------

QT       += core gui network widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += network

TARGET = OcularRoboticsClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientglwidget.cpp \
    clientglthread.cpp \
    udphost.cpp

HEADERS  += mainwindow.h \
    clientglwidget.h \
    clientglthread.h \
    udphost.h

FORMS    += mainwindow.ui
