#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T14:31:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PushMe
TEMPLATE = app


SOURCES += main.cpp\
        pushme.cpp \
    log.cpp

HEADERS  += pushme.h \
    log.hpp

FORMS    += pushme.ui

DEFINES += BOOST_LOG_DYN_LINK

unix:!macx: LIBS += -L$$PWD/../../../lib/libboostlog/ -lboost_chrono
unix:!macx: LIBS += -L$$PWD/../../../lib/libboostlog/ -lboost_date_time
unix:!macx: LIBS += -L$$PWD/../../../lib/libboostlog/ -lboost_filesystem
unix:!macx: LIBS += -L$$PWD/../../../lib/libboostlog/ -lboost_log
unix:!macx: LIBS += -L$$PWD/../../../lib/libboostlog/ -lboost_log_setup
unix:!macx: LIBS += -L$$PWD/../../../lib/libboostlog/ -lboost_regex
unix:!macx: LIBS += -L$$PWD/../../../lib/libboostlog/ -lboost_system
unix:!macx: LIBS += -L$$PWD/../../../lib/libboostlog/ -lboost_thread
INCLUDEPATH += $$PWD/../../../lib/boost_1_57_0
DEPENDPATH += $$PWD/../../../lib/boost_1_57_0
