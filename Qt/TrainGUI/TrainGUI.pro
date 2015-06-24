#-------------------------------------------------
#
# Project created by QtCreator 2015-04-05T08:36:59
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11
QMAKE_CXXFLAGS += -Wno-deprecated

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrainGUI
TEMPLATE = app


SOURCES += main.cpp\
        traincontrols.cpp \
    log.cpp \
    ../../Codeblock/common/src/config.cpp \
    ../../Codeblock/common/src/utils.cpp \
    ../../lib/RCF-2.0.1.101/src/RCF/RCF.cpp \
    ../../lib/RCFProto-src-1.0.0.3/cpp/src/RCFProto.cpp \
    ../../Codeblock/common/Common-train-server1/src/TrainCommSession.cpp \
    ../../Codeblock/common/Common-train-server1/src/TrainOperationSession.cpp \
    ../../Codeblock/common/Common-train-server1/src/TrainSession.cpp \
    traincommunicationsserver.cpp \
    traincommunicationclient.cpp \
    ../../Codeblock/common/protobuf/RCFProtoServices.pb.cc \
    protobufpositioninformationserviceimpl.cpp \
    protobufsettrainmodecommandserviceimpl.cpp

HEADERS  += traincontrols.h \
    traincommunications.h \
    log.hpp \
    ../../Codeblock/common/include/config.hpp \
    ../../Codeblock/common/include/errors.hpp \
    ../../Codeblock/common/include/utils.hpp \
    ../../Codeblock/common/Common-train-server1/include/TrainCommSession.hpp \
    ../../Codeblock/common/Common-train-server1/include/TrainOperationSession.hpp \
    ../../Codeblock/common/Common-train-server1/include/TrainSession.hpp \
    traincommunicationsserver.h \
    protobufpositioninformationserviceimpl.hpp \
    traincommunicationclient.hpp \
    ../../Codeblock/common/protobuf/RCFProtoServices.pb.h \

FORMS    += traincontrols.ui

DEFINES += BOOST_LOG_DYN_LINK

unix:!macx: LIBS += -L$$PWD/../../lib/libboostlog/ -lboost_chrono
unix:!macx: LIBS += -L$$PWD/../../lib/libboostlog/ -lboost_date_time
unix:!macx: LIBS += -L$$PWD/../../lib/libboostlog/ -lboost_filesystem
unix:!macx: LIBS += -L$$PWD/../../lib/libboostlog/ -lboost_log
unix:!macx: LIBS += -L$$PWD/../../lib/libboostlog/ -lboost_log_setup
unix:!macx: LIBS += -L$$PWD/../../lib/libboostlog/ -lboost_regex
unix:!macx: LIBS += -L$$PWD/../../lib/libboostlog/ -lboost_system
unix:!macx: LIBS += -L$$PWD/../../lib/libboostlog/ -lboost_thread

INCLUDEPATH += $$PWD/../../lib/boost_1_57_0
DEPENDPATH += $$PWD/../../lib/boost_1_57_0

unix:!macx: LIBS += -L$$PWD/../../lib/libprotobuf/ -lprotobuf

INCLUDEPATH += $$PWD/../../lib/libprotobuf
DEPENDPATH += $$PWD/../../lib/libprotobuf

unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/ -lpthread

INCLUDEPATH += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu


unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/ -ldl

INCLUDEPATH += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu

INCLUDEPATH += $$PWD/../../Codeblock/common/include
DEPENDPATH += $$PWD/../../Codeblock/common/include

INCLUDEPATH += $$PWD/../../Codeblock/common/protobuf
DEPENDPATH += $$PWD/../../Codeblock/common/protobuf

INCLUDEPATH += $$PWD/../../lib/RCFProto-src-1.0.0.3/cpp/src
DEPENDPATH += $$PWD/../../lib/RCFProto-src-1.0.0.3/cpp/src

INCLUDEPATH += $$PWD/../../lib/RCF-2.0.1.101/include
DEPENDPATH += $$PWD/../../lib/RCF-2.0.1.101/include

INCLUDEPATH += $$PWD/../../Codeblock/common/Common-train-server1/include
DEPENDPATH += $$PWD/../../Codeblock/common/Common-train-server1/include

INCLUDEPATH += $$PWD/../../Codeblock/common/Common-train-internal/include
DEPENDPATH += $$PWD/../../Codeblock/common/Common-train-internal/include
