TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lboost_system
LIBS += -lpthread
LIBS += -lboost_program_options
LIBS += -lmongocxx -lbsoncxx
LIBS += -lsystemd

# used for enabling DEBUG logging
DEFINES += DEBUG

INCLUDEPATH += /usr/include/mongocxx/v_noabi
INCLUDEPATH += /usr/include/bsoncxx/v_noabi

SOURCES += main.cpp \
    localserver.cpp \
    databasemanager.cpp \
    mongomanager.cpp

HEADERS += \
    localserver.h \
    databasemanager.h \
    mongomanager.h \
    utils.hpp

