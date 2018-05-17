QMAKE_CXXFLAGS += -std=c++11

QT       += widgets

TARGET = comparisonTest
#TEMPLATE = lib
MOC_DIR = ../src
UI_HEADERS_DIR = ../src
UI_SOURCES_DIR = ../src

#DEFINES += TODOAPP_LIBRARY

SOURCES += \
    deadlinedjob.cpp \
    job.cpp \
    joboverlord.cpp \
    deadlinedjoboverlord.cpp \
    comparisonTest.cpp

HEADERS +=\
    job.h \
    deadlinedjob.h \
    joboverlord.h \
    deadlinedjoboverlord.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
