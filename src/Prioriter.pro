#-------------------------------------------------
#
# Project created by QtCreator 2017-10-24T17:13:01
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11

QT       += widgets

TARGET = prioriter
#TEMPLATE = lib
MOC_DIR = ../src
UI_HEADERS_DIR = ../src
UI_SOURCES_DIR = ../src

#RC_ICONS = ../images/Sknife.ico

#DEFINES += TODOAPP_LIBRARY

FORMS += \
    addoverlord.ui \
    addjob.ui \
    mainwindow.ui \
    jobedit.ui

SOURCES += \
    deadlinedjob.cpp \
    job.cpp \
    joboverlord.cpp \
    deadlinedjoboverlord.cpp \
    window.cpp \
    main.cpp \
    addjob.cpp \
    addoverlord.cpp \
    jobtreeitem.cpp \
    editjob.cpp

HEADERS +=\
    job.h \
    deadlinedjob.h \
    joboverlord.h \
    deadlinedjoboverlord.h \
    window.h \
    addjob.h \
    addoverlord.h \
    jobtreeitem.h \
    editjob.h

RESOURCES += \
    resources.qrc
