#-------------------------------------------------
#
# Project created by QtCreator 2012-04-26T15:01:12
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = basketball
TEMPLATE = app


SOURCES += main.cpp \
    glwidget.cpp \
    glaid.cpp \
    myworld.cpp \
    ball.cpp \
    ring.cpp \
    flat.cpp \
    basketball.cpp \
    floor.cpp \
    audiencecamera.cpp \
    playercamera.cpp \
    zcamera.cpp \
    3ds.cpp \
    basket.cpp \
    soundcontroller.cpp \
    net.cpp \
    myglobal.cpp \
    walls.cpp \
    audience.cpp \
    stadium.cpp

HEADERS  += \
    glwidget.h \
    glaid.h \
    point3d.h \
    myglobal.h \
    myworld.h \
    ball.h \
    ring.h \
    flat.h \
    basketball.h \
    floor.h \
    abstractcamera.h \
    audiencecamera.h \
    playercamera.h \
    zcamera.h \
    3ds.h \
    basket.h \
    soundcontroller.h \
    net.h \
    walls.h \
    audience.h \
    stadium.h

LIBS += -L ../lib/
LIBS += -lfmod

INCLUDEPATH += ../include/

RESOURCES += \
    images.qrc

RC_FILE = icon.rc
